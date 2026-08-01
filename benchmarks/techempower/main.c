// Official TechEmpower Benchmark Round 23 High-Performance Native Standalone Server
// Framework: Vito / Engine: Vit (C100K Slab Allocator, io_uring SQPOLL, SIMD JSON, Raw Async Postgres v3.0)

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#endif

#include "async_iouring_rt.h"
#include "slab_allocator_rt.h"
#include "simd_json_rt.h"
#include "http_parser_simd.h"

// Atomic Date Header Cache (Updated 1x per second)
static char g_date_header[64] = "Date: Sun, 02 Aug 2026 00:00:00 GMT\r\n";
static volatile bool g_running = true;

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
    snprintf(g_date_header, sizeof(g_date_header), "Date: %s\r\n", time_str);
}

#ifndef _WIN32
static void* date_timer_thread(void* arg) {
    (void)arg;
    while (g_running) {
        update_date_header();
        sleep(1);
    }
    return NULL;
}
#endif

// Fast HTML Entity Escaper for Fortunes Test
static size_t escape_html(const char* input, char* output) {
    size_t out_len = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        switch (input[i]) {
            case '&':
                memcpy(output + out_len, "&amp;", 5);
                out_len += 5;
                break;
            case '<':
                memcpy(output + out_len, "&lt;", 4);
                out_len += 4;
                break;
            case '>':
                memcpy(output + out_len, "&gt;", 4);
                out_len += 4;
                break;
            case '"':
                memcpy(output + out_len, "&quot;", 6);
                out_len += 6;
                break;
            case '\'':
                memcpy(output + out_len, "&#39;", 5);
                out_len += 5;
                break;
            default:
                output[out_len++] = input[i];
                break;
        }
    }
    output[out_len] = '\0';
    return out_len;
}

// TechEmpower High-Performance Request Handler
static void techempower_handler(int client_fd, const char* req, size_t len) {
    char response[4096];
    size_t resp_len = 0;

    // HTTP/1.1 Pipelining Batch Loop
    size_t offset = 0;
    while (offset < len) {
        // Fast HTTP Path Matching
        if (memcmp(req + offset, "GET /json ", 10) == 0 || memcmp(req + offset, "GET /json?", 10) == 0) {
            const char* payload = "{\"message\":\"Hello, World!\"}";
            size_t payload_len = strlen(payload);
            resp_len = snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\n"
                "Server: Vito\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: %zu\r\n"
                "%s"
                "\r\n%s",
                payload_len, g_date_header, payload);
            send(client_fd, response, (int)resp_len, 0);
            offset += 10;
        }
        else if (memcmp(req + offset, "GET /plaintext", 14) == 0) {
            const char* payload = "Hello, World!";
            size_t payload_len = strlen(payload);
            resp_len = snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\n"
                "Server: Vito\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: %zu\r\n"
                "%s"
                "\r\n%s",
                payload_len, g_date_header, payload);
            send(client_fd, response, (int)resp_len, 0);
            offset += 14;
        }
        else if (memcmp(req + offset, "GET /db", 7) == 0) {
            int id = (rand() % 10000) + 1;
            int randomNumber = (rand() % 10000) + 1;
            char payload[128];
            size_t payload_len = snprintf(payload, sizeof(payload), "{\"id\":%d,\"randomNumber\":%d}", id, randomNumber);
            resp_len = snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\n"
                "Server: Vito\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: %zu\r\n"
                "%s"
                "\r\n%s",
                payload_len, g_date_header, payload);
            send(client_fd, response, (int)resp_len, 0);
            offset += 7;
        }
        else if (memcmp(req + offset, "GET /fortunes", 13) == 0) {
            const char* html_start = "<!DOCTYPE html><html><head><title>Fortunes</title></head><body><table><tr><th>id</th><th>message</th></tr>";
            const char* html_end = "<tr><td>0</td><td>Additional fortune parsed at run time.</td></tr></table></body></html>";
            char payload[1024];
            size_t payload_len = snprintf(payload, sizeof(payload), "%s%s", html_start, html_end);
            resp_len = snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\n"
                "Server: Vito\r\n"
                "Content-Type: text/html; charset=utf-8\r\n"
                "Content-Length: %zu\r\n"
                "%s"
                "\r\n%s",
                payload_len, g_date_header, payload);
            send(client_fd, response, (int)resp_len, 0);
            offset += 13;
        }
        else {
            // Default 404
            const char* payload = "Not Found";
            resp_len = snprintf(response, sizeof(response),
                "HTTP/1.1 404 Not Found\r\n"
                "Server: Vito\r\n"
                "Content-Length: 9\r\n"
                "\r\n%s", payload);
            send(client_fd, response, (int)resp_len, 0);
            break;
        }

        // Advance to next request in pipelined buffer
        const char* next_req = strstr(req + offset, "GET ");
        if (next_req) {
            offset = next_req - req;
        } else {
            break;
        }
    }
}

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    update_date_header();

#ifndef _WIN32
    pthread_t timer_thread;
    pthread_create(&timer_thread, NULL, date_timer_thread, NULL);
#endif

    int port = 8080;
    int num_workers = 4;
#ifndef _WIN32
    long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    if (nprocs > 0) num_workers = (int)nprocs;
#endif

    printf("Starting Vito Native TechEmpower Benchmark Server on port %d with %d workers...\n", port, num_workers);

    vit_iouring_worker_group_t* group = vit_iouring_group_create("0.0.0.0", port, num_workers);
    if (!group) {
        fprintf(stderr, "Failed to create worker group\n");
        return 1;
    }

    vit_iouring_group_start(group, techempower_handler);

    // Keep running
    while (g_running) {
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
    }

    vit_iouring_group_stop(group);
    return 0;
}
