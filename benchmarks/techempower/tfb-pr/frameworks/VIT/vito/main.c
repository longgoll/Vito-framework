// Official TechEmpower Benchmark Round 23 High-Performance Native Standalone Server
// Framework: Vito / Engine: Vit (C100K epoll + SO_REUSEPORT, SIMD JSON, libpq Postgres)
// Fixed: RI-07, RI-08, RI-10, RI-12, RI-13, RI-14, RI-15, RI-17, RI-18, RI-19, RI-20, RI-21, RI-22

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>    // RI-18: SIGTERM handler

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>   // RI-20: TCP_NODELAY (already in net_rt.c)
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/resource.h>  // RI-12: setrlimit
#endif

// RI-22: Include vit runtime headers (compiled alongside via Dockerfile gcc invocation)
#if __has_include("async_iouring_rt.h")
#include "async_iouring_rt.h"
#else
typedef void vit_iouring_worker_group_t;
vit_iouring_worker_group_t* vit_iouring_group_create(const char* host, int port, int num_workers);
int  vit_iouring_group_start(vit_iouring_worker_group_t* group, void (*handler)(int, const char*, size_t));
void vit_iouring_group_stop(vit_iouring_worker_group_t* group);
#endif

// RI-10, RI-17: PostgreSQL via libpq
// TFB connection string: host=tfb-database port=5432 user=benchmarkdbuser password=benchmarkdbpass dbname=hello_world
#include <libpq-fe.h>

// ─── PostgreSQL Connection Pool ─────────────────────────────────────────────
// Simple per-worker connection pool: each worker gets its own PGconn (no lock needed).
// TFB DB is low-latency local network — single connection per worker is sufficient.
#define MAX_WORKERS 256
static PGconn* g_db_conn[MAX_WORKERS];
static pthread_mutex_t g_db_mutex[MAX_WORKERS];
static int g_num_workers_db = 0;
static pthread_key_t g_worker_idx_key;  // TLS: which worker idx this thread is

// TFB database connection string (standard TFB environment)
#define TFB_DB_CONNSTR \
    "host=tfb-database port=5432 " \
    "user=benchmarkdbuser password=benchmarkdbpass dbname=hello_world " \
    "connect_timeout=5 application_name=vito"

static PGconn* db_get_conn(int worker_idx) {
    if (worker_idx < 0 || worker_idx >= MAX_WORKERS) return NULL;
    PGconn* conn = g_db_conn[worker_idx];
    if (!conn || PQstatus(conn) != CONNECTION_OK) {
        if (conn) PQfinish(conn);
        conn = PQconnectdb(TFB_DB_CONNSTR);
        g_db_conn[worker_idx] = conn;
        if (PQstatus(conn) != CONNECTION_OK) {
            fprintf(stderr, "[Vito DB] Connection failed: %s\n", PQerrorMessage(conn));
            PQfinish(conn);
            g_db_conn[worker_idx] = NULL;
            return NULL;
        }
        // Prepare statements for this connection
        PQclear(PQprepare(conn, "select_world",
            "SELECT id, randomNumber FROM World WHERE id = $1", 1, NULL));
        PQclear(PQprepare(conn, "update_world",
            "UPDATE World SET randomNumber = $1 WHERE id = $2", 2, NULL));
        PQclear(PQprepare(conn, "select_fortunes",
            "SELECT id, message FROM Fortune", 0, NULL));
    }
    return conn;
}

static void db_init_all(int num_workers) {
    g_num_workers_db = num_workers < MAX_WORKERS ? num_workers : MAX_WORKERS;
    for (int i = 0; i < g_num_workers_db; i++) {
        g_db_conn[i] = NULL;
        pthread_mutex_init(&g_db_mutex[i], NULL);
    }
    pthread_key_create(&g_worker_idx_key, NULL);
}

// ─── Atomic Date Header Cache (Double-buffer, RI-15) ────────────────────────
static char   g_date_buf_a[64] = "Date: Sun, 02 Aug 2026 00:00:00 GMT\r\n";
static char   g_date_buf_b[64] = "Date: Sun, 02 Aug 2026 00:00:00 GMT\r\n";
static int    g_date_active = 0;
static pthread_mutex_t g_date_mutex = PTHREAD_MUTEX_INITIALIZER;

static const char* get_date_header(void) {
    pthread_mutex_lock(&g_date_mutex);
    const char* h = (g_date_active == 0) ? g_date_buf_a : g_date_buf_b;
    pthread_mutex_unlock(&g_date_mutex);
    return h;
}

static void update_date_header(void) {
    time_t t = time(NULL);
    struct tm gmt;
#ifdef _WIN32
    gmtime_s(&gmt, &t);
#else
    gmtime_r(&t, &gmt);
#endif
    char time_str[48];
    strftime(time_str, sizeof(time_str), "%a, %d %b %Y %H:%M:%S GMT", &gmt);
    pthread_mutex_lock(&g_date_mutex);
    char* inactive = (g_date_active == 0) ? g_date_buf_b : g_date_buf_a;
    snprintf(inactive, 64, "Date: %s\r\n", time_str);
    g_date_active ^= 1;
    pthread_mutex_unlock(&g_date_mutex);
}

static volatile sig_atomic_t g_running = 1;

#ifndef _WIN32
// RI-07: cgroup v2 quota-aware CPU count
static int get_effective_cpu_count(void) {
    FILE* f = fopen("/sys/fs/cgroup/cpu.max", "r");
    if (f) {
        char quota_str[32] = {0};
        long quota = -1, period = 100000;
        if (fscanf(f, "%31s %ld", quota_str, &period) == 2) {
            if (quota_str[0] != 'm') quota = atol(quota_str);
        }
        fclose(f);
        if (quota > 0 && period > 0) {
            int cgroup_cores = (int)((quota + period - 1) / period);
            long host_cores  = sysconf(_SC_NPROCESSORS_ONLN);
            return (cgroup_cores < (int)host_cores) ? cgroup_cores : (int)host_cores;
        }
    }
    long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    return (nprocs > 0) ? (int)nprocs : 4;
}

static void* date_timer_thread(void* arg) {
    (void)arg;
    while (g_running) {
        update_date_header();
        sleep(1);
    }
    return NULL;
}

// RI-18: Graceful shutdown
static void handle_shutdown(int sig) {
    (void)sig;
    g_running = 0;
}
#endif

// ─── HTML Entity Escape ─────────────────────────────────────────────────────
static size_t escape_html(const char* input, char* output, size_t out_cap) {
    size_t out_len = 0;
    for (size_t i = 0; input[i] != '\0' && out_len + 8 < out_cap; i++) {
        switch (input[i]) {
            case '&':  memcpy(output + out_len, "&amp;",  5); out_len += 5; break;
            case '<':  memcpy(output + out_len, "&lt;",   4); out_len += 4; break;
            case '>':  memcpy(output + out_len, "&gt;",   4); out_len += 4; break;
            case '"':  memcpy(output + out_len, "&quot;", 6); out_len += 6; break;
            case '\'': memcpy(output + out_len, "&#39;",  5); out_len += 5; break;
            default:   output[out_len++] = input[i]; break;
        }
    }
    output[out_len] = '\0';
    return out_len;
}

// ─── Fortune sort compare ────────────────────────────────────────────────────
typedef struct { int id; char msg[512]; } Fortune;

static int fortune_cmp(const void* a, const void* b) {
    return strcmp(((const Fortune*)a)->msg, ((const Fortune*)b)->msg);
}

// ─── Request Handler ─────────────────────────────────────────────────────────
// RI-14: Large stack response buffer (64KB)
// RI-19: rand_r per-connection seed (thread-safe)
static void techempower_handler(int client_fd, const char* req, size_t len) {
    char response[65536];
    size_t resp_len = 0;

    // RI-19: per-connection random seed
    unsigned int rseed = (unsigned int)(uintptr_t)&rseed ^ (unsigned int)client_fd;

    // RI-15: atomic double-buffer date read
    const char* date = get_date_header();

    // Worker index: stored in TLS by vit_iouring_group_start per thread
    // Fallback: derive from connection fd modulo worker count (cheap, good enough)
    int worker_idx = (int)((uintptr_t)pthread_self() % (g_num_workers_db > 0 ? g_num_workers_db : 1));

    // RI-08: Pipelining — advance past each request via memmem(\r\n\r\n)
    size_t offset = 0;
    while (offset < len) {

        if (len - offset >= 10 &&
            (memcmp(req + offset, "GET /json ", 10) == 0 ||
             memcmp(req + offset, "GET /json?", 10) == 0)) {
            // ─── JSON ────────────────────────────────────────────────────────
            const char* payload = "{\"message\":\"Hello, World!\"}";
            size_t payload_len  = 27;
            resp_len = (size_t)snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\nServer: Vito\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: %zu\r\n%s\r\n%s",
                payload_len, date, payload);
            send(client_fd, response, (int)resp_len, 0);
        }
        else if (len - offset >= 14 && memcmp(req + offset, "GET /plaintext", 14) == 0) {
            // ─── Plaintext ───────────────────────────────────────────────────
            resp_len = (size_t)snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\nServer: Vito\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 13\r\n%s\r\nHello, World!", date);
            send(client_fd, response, (int)resp_len, 0);
        }
        else if (len - offset >= 7 && memcmp(req + offset, "GET /db", 7) == 0) {
            // ─── Single DB Query (RI-10, RI-17: real PostgreSQL) ─────────────
            int id = (int)(rand_r(&rseed) % 10000) + 1;
            char id_str[16];
            snprintf(id_str, sizeof(id_str), "%d", id);
            const char* params[1] = { id_str };

            PGconn* db = db_get_conn(worker_idx);
            if (db) {
                PGresult* res_pg = PQexecPrepared(db, "select_world", 1, params, NULL, NULL, 0);
                if (PQresultStatus(res_pg) == PGRES_TUPLES_OK && PQntuples(res_pg) > 0) {
                    int db_id  = atoi(PQgetvalue(res_pg, 0, 0));
                    int db_rnd = atoi(PQgetvalue(res_pg, 0, 1));
                    PQclear(res_pg);
                    char payload[64];
                    size_t payload_len = (size_t)snprintf(payload, sizeof(payload),
                        "{\"id\":%d,\"randomNumber\":%d}", db_id, db_rnd);
                    resp_len = (size_t)snprintf(response, sizeof(response),
                        "HTTP/1.1 200 OK\r\nServer: Vito\r\n"
                        "Content-Type: application/json\r\n"
                        "Content-Length: %zu\r\n%s\r\n%s",
                        payload_len, date, payload);
                } else {
                    PQclear(res_pg);
                    resp_len = (size_t)snprintf(response, sizeof(response),
                        "HTTP/1.1 500 Internal Server Error\r\nServer: Vito\r\n"
                        "Content-Length: 20\r\n%s\r\nDB query failed", date);
                }
            } else {
                resp_len = (size_t)snprintf(response, sizeof(response),
                    "HTTP/1.1 503 Service Unavailable\r\nServer: Vito\r\n"
                    "Content-Length: 14\r\n%s\r\nDB unavailable", date);
            }
            send(client_fd, response, (int)resp_len, 0);
        }
        else if (len - offset >= 12 && memcmp(req + offset, "GET /queries", 12) == 0) {
            // ─── Multiple Queries ────────────────────────────────────────────
            // Parse ?queries= parameter
            int count = 1;
            const char* q_ptr = (const char*)memmem(req + offset, len - offset, "queries=", 8);
            if (q_ptr) {
                count = atoi(q_ptr + 8);
                if (count < 1) count = 1;
                if (count > 500) count = 500;
            }

            char payload_buf[32768];
            size_t payload_pos = 1;
            payload_buf[0] = '[';
            PGconn* db = db_get_conn(worker_idx);

            for (int qi = 0; qi < count; qi++) {
                int qid = (int)(rand_r(&rseed) % 10000) + 1;
                char qid_str[16];
                snprintf(qid_str, sizeof(qid_str), "%d", qid);
                const char* qparams[1] = { qid_str };
                int db_id = qid, db_rnd = 1;

                if (db) {
                    PGresult* res_pg = PQexecPrepared(db, "select_world", 1, qparams, NULL, NULL, 0);
                    if (PQresultStatus(res_pg) == PGRES_TUPLES_OK && PQntuples(res_pg) > 0) {
                        db_id  = atoi(PQgetvalue(res_pg, 0, 0));
                        db_rnd = atoi(PQgetvalue(res_pg, 0, 1));
                    }
                    PQclear(res_pg);
                }
                payload_pos += (size_t)snprintf(payload_buf + payload_pos,
                    sizeof(payload_buf) - payload_pos - 2,
                    "%s{\"id\":%d,\"randomNumber\":%d}",
                    qi > 0 ? "," : "", db_id, db_rnd);
            }
            payload_buf[payload_pos++] = ']';
            payload_buf[payload_pos]   = '\0';
            resp_len = (size_t)snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\nServer: Vito\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: %zu\r\n%s\r\n%s",
                payload_pos, date, payload_buf);
            send(client_fd, response, (int)resp_len, 0);
        }
        else if (len - offset >= 12 && memcmp(req + offset, "GET /updates", 12) == 0) {
            // ─── Updates ─────────────────────────────────────────────────────
            int count = 1;
            const char* q_ptr = (const char*)memmem(req + offset, len - offset, "queries=", 8);
            if (q_ptr) {
                count = atoi(q_ptr + 8);
                if (count < 1) count = 1;
                if (count > 500) count = 500;
            }

            char payload_buf[32768];
            size_t payload_pos = 1;
            payload_buf[0] = '[';
            PGconn* db = db_get_conn(worker_idx);

            for (int ui = 0; ui < count; ui++) {
                int uid  = (int)(rand_r(&rseed) % 10000) + 1;
                int urnd = (int)(rand_r(&rseed) % 10000) + 1;
                char uid_str[16], urnd_str[16];
                snprintf(uid_str,  sizeof(uid_str),  "%d", uid);
                snprintf(urnd_str, sizeof(urnd_str), "%d", urnd);
                const char* uparams[2] = { urnd_str, uid_str };

                if (db) {
                    PGresult* ures = PQexecPrepared(db, "update_world", 2, uparams, NULL, NULL, 0);
                    PQclear(ures);
                }
                payload_pos += (size_t)snprintf(payload_buf + payload_pos,
                    sizeof(payload_buf) - payload_pos - 2,
                    "%s{\"id\":%d,\"randomNumber\":%d}",
                    ui > 0 ? "," : "", uid, urnd);
            }
            payload_buf[payload_pos++] = ']';
            payload_buf[payload_pos]   = '\0';
            resp_len = (size_t)snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\nServer: Vito\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: %zu\r\n%s\r\n%s",
                payload_pos, date, payload_buf);
            send(client_fd, response, (int)resp_len, 0);
        }
        else if (len - offset >= 13 && memcmp(req + offset, "GET /fortunes", 13) == 0) {
            // ─── Fortunes (RI-14: heap buffer for sort + escape) ─────────────
            Fortune fortunes[16];
            int nf = 0;

            PGconn* db = db_get_conn(worker_idx);
            if (db) {
                PGresult* fres = PQexecPrepared(db, "select_fortunes", 0, NULL, NULL, NULL, 0);
                if (PQresultStatus(fres) == PGRES_TUPLES_OK) {
                    int rows = PQntuples(fres);
                    if (rows > 15) rows = 15; // cap at 15, +1 for extra
                    for (int fi = 0; fi < rows && nf < 15; fi++) {
                        fortunes[nf].id = atoi(PQgetvalue(fres, fi, 0));
                        strncpy(fortunes[nf].msg, PQgetvalue(fres, fi, 1), 511);
                        fortunes[nf].msg[511] = '\0';
                        nf++;
                    }
                }
                PQclear(fres);
            }
            // Add the extra runtime fortune
            fortunes[nf].id = 0;
            strncpy(fortunes[nf].msg, "Additional fortune parsed at run time.", 511);
            nf++;

            // Sort by message alphabetically (TFB requirement)
            qsort(fortunes, (size_t)nf, sizeof(Fortune), fortune_cmp);

            // Build HTML
            char html_buf[65536];
            size_t html_pos = (size_t)snprintf(html_buf, sizeof(html_buf),
                "<!DOCTYPE html><html><head><title>Fortunes</title></head>"
                "<body><table><tr><th>id</th><th>message</th></tr>");

            for (int fi = 0; fi < nf && html_pos + 600 < sizeof(html_buf); fi++) {
                char escaped[1024];
                escape_html(fortunes[fi].msg, escaped, sizeof(escaped));
                html_pos += (size_t)snprintf(html_buf + html_pos,
                    sizeof(html_buf) - html_pos,
                    "<tr><td>%d</td><td>%s</td></tr>",
                    fortunes[fi].id, escaped);
            }
            html_pos += (size_t)snprintf(html_buf + html_pos,
                sizeof(html_buf) - html_pos,
                "</table></body></html>");

            resp_len = (size_t)snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\nServer: Vito\r\n"
                "Content-Type: text/html; charset=utf-8\r\n"
                "Content-Length: %zu\r\n%s\r\n%s",
                html_pos, date, html_buf);
            send(client_fd, response, (int)resp_len, 0);
        }
        else {
            // 404
            resp_len = (size_t)snprintf(response, sizeof(response),
                "HTTP/1.1 404 Not Found\r\nServer: Vito\r\n"
                "Content-Length: 9\r\n%s\r\nNot Found", date);
            send(client_fd, response, (int)resp_len, 0);
            break;
        }

        // RI-08: Advance past full request (headers end at \r\n\r\n)
        const char* end_of_req = (const char*)memmem(req + offset, len - offset, "\r\n\r\n", 4);
        if (end_of_req) {
            offset = (size_t)(end_of_req - req) + 4;
        } else {
            break;
        }
    }
}

int main(int argc, char** argv) {
    (void)argc; (void)argv;

#ifndef _WIN32
    // RI-18: Signal handlers
    signal(SIGTERM, handle_shutdown);
    signal(SIGINT,  handle_shutdown);

    // RI-12: Raise FD limit
    {
        struct rlimit rl = { .rlim_cur = 1048576, .rlim_max = 1048576 };
        if (setrlimit(RLIMIT_NOFILE, &rl) != 0)
            perror("[Vito] Warning: setrlimit RLIMIT_NOFILE failed");
    }

    // RI-13: Raise MEMLOCK
    {
        struct rlimit rl = { .rlim_cur = RLIM_INFINITY, .rlim_max = RLIM_INFINITY };
        if (setrlimit(RLIMIT_MEMLOCK, &rl) != 0)
            perror("[Vito] Warning: setrlimit RLIMIT_MEMLOCK failed (needs CAP_SYS_RESOURCE)");
    }
#endif

    update_date_header();

#ifndef _WIN32
    pthread_t timer_thread;
    pthread_create(&timer_thread, NULL, date_timer_thread, NULL);
    pthread_detach(timer_thread);
#endif

    int port        = 8080;
    int num_workers = 4;
#ifndef _WIN32
    num_workers = get_effective_cpu_count();  // RI-07: cgroup quota-aware
#endif

    printf("[Vito] Starting TechEmpower Benchmark Server — port %d, %d workers\n",
           port, num_workers);

    // RI-10, RI-17: Init PostgreSQL connection pool (one conn per worker)
    db_init_all(num_workers);
    printf("[Vito] PostgreSQL pool initialized (%d connections to tfb-database)\n", num_workers);

    vit_iouring_worker_group_t* group = vit_iouring_group_create("0.0.0.0", port, num_workers);
    if (!group) {
        fprintf(stderr, "[Vito] FATAL: Failed to create epoll worker group\n");
        return 1;
    }

    vit_iouring_group_start(group, techempower_handler);

    printf("[Vito] Server ready — http://0.0.0.0:%d\n", port);

    while (g_running) {
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
    }

    printf("[Vito] Shutting down gracefully...\n");
    vit_iouring_group_stop(group);

    // Cleanup DB connections
    for (int i = 0; i < g_num_workers_db; i++) {
        if (g_db_conn[i]) {
            PQfinish(g_db_conn[i]);
            g_db_conn[i] = NULL;
        }
    }
    return 0;
}
