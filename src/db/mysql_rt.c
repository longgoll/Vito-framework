#include "mysql_rt.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ============================================================
// Vito MySQL Wire Protocol Runtime — C Implementation
// Implements MySQL Client/Server Protocol v10 from scratch.
// Zero library dependencies — pure socket + protocol work.
// ============================================================
// Protocol references:
//   https://dev.mysql.com/doc/internals/en/client-server-protocol.html
//   MySQL Internals Manual — Connection Phase, Text Protocol,
//   Binary Protocol (Prepared Statements), Pipeline Queries
// ============================================================

// ── OS socket layer (platform-adaptive) ──────────────────────────────────

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET sock_t;
    #define INVALID_SOCK   INVALID_SOCKET
    #define sock_close(s)  closesocket(s)
    #define sock_send(s,b,l,f) send(s, (const char*)(b), (int)(l), f)
    #define sock_recv(s,b,l,f) recv(s, (char*)(b), (int)(l), f)
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int sock_t;
    #define INVALID_SOCK   (-1)
    #define sock_close(s)  close(s)
    #define sock_send(s,b,l,f) send(s, b, l, f)
    #define sock_recv(s,b,l,f) recv(s, b, l, f)
#endif

// SHA1 (for auth — minimal implementation for mysql_native_password)
static void sha1_compute(const uint8_t* data, size_t len, uint8_t out[20]);
static void xor_bytes(uint8_t* dst, const uint8_t* a, const uint8_t* b, size_t n);

// ── Internal connection struct ────────────────────────────────────────────

struct vito_mysql_conn_t {
    sock_t   sock;
    uint32_t server_capabilities;
    uint32_t client_capabilities;
    uint8_t  seq_id;
    char     error_msg[512];
    bool     in_transaction;
    bool     pipeline_enabled;

    // Scratch I/O buffer (reused across calls — no malloc per packet)
    uint8_t  io_buf[65536 + 4];  // 64 KB + 4-byte header
    size_t   io_buf_used;
};

struct vito_mysql_stmt_t {
    uint32_t stmt_id;
    uint16_t param_count;
    uint16_t col_count;
    vito_mysql_field_t params[VITO_MYSQL_MAX_COLS];
    vito_mysql_field_t fields[VITO_MYSQL_MAX_COLS];

    // Bound parameter values (text-encoded for simplicity)
    char     param_data[VITO_MYSQL_MAX_COLS][256];
    uint16_t param_lens[VITO_MYSQL_MAX_COLS];
    uint8_t  param_types[VITO_MYSQL_MAX_COLS];
    bool     param_null[VITO_MYSQL_MAX_COLS];

    vito_mysql_conn_t* conn;
};

// ── Little-endian helpers ─────────────────────────────────────────────────

static inline uint16_t read_u16_le(const uint8_t* p) {
    return (uint16_t)(p[0] | ((uint16_t)p[1] << 8));
}

static inline uint32_t read_u32_le(const uint8_t* p) {
    return (uint32_t)(p[0]) | ((uint32_t)p[1] << 8)
         | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static inline uint64_t read_u64_le(const uint8_t* p) {
    uint64_t lo = read_u32_le(p);
    uint64_t hi = read_u32_le(p + 4);
    return lo | (hi << 32);
}

static inline void write_u16_le(uint8_t* p, uint16_t v) {
    p[0] = (uint8_t)(v);
    p[1] = (uint8_t)(v >> 8);
}

static inline void write_u32_le(uint8_t* p, uint32_t v) {
    p[0] = (uint8_t)(v);
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
}

// ── Packet length (3-byte LE) ─────────────────────────────────────────────

uint32_t mysql_read_packet_len(const uint8_t* header) {
    return (uint32_t)header[0]
         | ((uint32_t)header[1] << 8)
         | ((uint32_t)header[2] << 16);
}

static void write_packet_header(uint8_t* hdr, uint32_t len, uint8_t seq) {
    hdr[0] = (uint8_t)(len);
    hdr[1] = (uint8_t)(len >> 8);
    hdr[2] = (uint8_t)(len >> 16);
    hdr[3] = seq;
}

// ── Length-encoded integer ────────────────────────────────────────────────

uint64_t mysql_read_lenenc_int(const uint8_t* buf, size_t buf_len, size_t* offset) {
    if (*offset >= buf_len) return 0;
    uint8_t first = buf[*offset];
    (*offset)++;
    if (first < 0xFB) return (uint64_t)first;
    if (first == 0xFC) {
        if (*offset + 2 > buf_len) return 0;
        uint64_t v = read_u16_le(buf + *offset);
        *offset += 2;
        return v;
    }
    if (first == 0xFD) {
        if (*offset + 3 > buf_len) return 0;
        uint64_t v = (uint32_t)buf[*offset]
                   | ((uint32_t)buf[*offset+1] << 8)
                   | ((uint32_t)buf[*offset+2] << 16);
        *offset += 3;
        return v;
    }
    if (first == 0xFE) {
        if (*offset + 8 > buf_len) return 0;
        uint64_t v = read_u64_le(buf + *offset);
        *offset += 8;
        return v;
    }
    return 0; // 0xFF = error indicator
}

bool mysql_read_lenenc_str(const uint8_t* buf, size_t buf_len, size_t* offset,
                           const uint8_t** str_out, uint32_t* str_len) {
    uint64_t len = mysql_read_lenenc_int(buf, buf_len, offset);
    if (*offset + len > buf_len) return false;
    *str_out = buf + *offset;
    *str_len = (uint32_t)len;
    *offset += len;
    return true;
}

// ── Socket I/O ────────────────────────────────────────────────────────────

static bool sock_send_all(sock_t s, const uint8_t* data, size_t len) {
    size_t sent = 0;
    while (sent < len) {
        int n = (int)sock_send(s, data + sent, len - sent, 0);
        if (n <= 0) return false;
        sent += (size_t)n;
    }
    return true;
}

static bool sock_recv_all(sock_t s, uint8_t* buf, size_t len) {
    size_t recvd = 0;
    while (recvd < len) {
        int n = (int)sock_recv(s, buf + recvd, len - recvd, 0);
        if (n <= 0) return false;
        recvd += (size_t)n;
    }
    return true;
}

// Read one MySQL packet from socket into conn->io_buf (after header)
static bool read_packet(vito_mysql_conn_t* conn, uint8_t* out_buf,
                        size_t out_cap, size_t* out_len) {
    uint8_t hdr[4];
    if (!sock_recv_all(conn->sock, hdr, 4)) return false;
    uint32_t pkt_len = mysql_read_packet_len(hdr);
    conn->seq_id = hdr[3] + 1;
    if (pkt_len > out_cap) return false;
    if (!sock_recv_all(conn->sock, out_buf, pkt_len)) return false;
    *out_len = pkt_len;
    return true;
}

// Write one MySQL packet to socket
static bool send_packet(vito_mysql_conn_t* conn, const uint8_t* payload, size_t len) {
    uint8_t hdr[4];
    write_packet_header(hdr, (uint32_t)len, conn->seq_id++);
    return sock_send_all(conn->sock, hdr, 4) && sock_send_all(conn->sock, payload, len);
}

// ── SHA1 (minimal, for mysql_native_password) ─────────────────────────────

static void sha1_compute(const uint8_t* data, size_t len, uint8_t out[20]) {
    // Minimal SHA-1 implementation (RFC 3174)
    uint32_t h0 = 0x67452301, h1 = 0xEFCDAB89, h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476, h4 = 0xC3D2E1F0;

    uint64_t bit_len = (uint64_t)len * 8;
    size_t padded = ((len + 64 + 8) / 64) * 64;
    uint8_t* msg = (uint8_t*)calloc(padded, 1);
    if (!msg) return;
    memcpy(msg, data, len);
    msg[len] = 0x80;
    for (int i = 0; i < 8; i++)
        msg[padded - 8 + i] = (uint8_t)(bit_len >> (56 - 8*i));

    for (size_t chunk = 0; chunk < padded; chunk += 64) {
        uint32_t w[80];
        for (int i = 0; i < 16; i++)
            w[i] = ((uint32_t)msg[chunk+4*i]<<24)|((uint32_t)msg[chunk+4*i+1]<<16)
                  |((uint32_t)msg[chunk+4*i+2]<<8)|msg[chunk+4*i+3];
        for (int i = 16; i < 80; i++) {
            uint32_t t = w[i-3]^w[i-8]^w[i-14]^w[i-16];
            w[i] = (t<<1)|(t>>31);
        }
        uint32_t a=h0,b=h1,c=h2,d=h3,e=h4;
        for (int i = 0; i < 80; i++) {
            uint32_t f,k;
            if(i<20){f=(b&c)|((~b)&d);k=0x5A827999;}
            else if(i<40){f=b^c^d;k=0x6ED9EBA1;}
            else if(i<60){f=(b&c)|(b&d)|(c&d);k=0x8F1BBCDC;}
            else{f=b^c^d;k=0xCA62C1D6;}
            uint32_t temp=((a<<5)|(a>>27))+f+e+k+w[i];
            e=d;d=c;c=(b<<30)|(b>>2);b=a;a=temp;
        }
        h0+=a;h1+=b;h2+=c;h3+=d;h4+=e;
    }
    free(msg);
    uint32_t hs[5] = {h0,h1,h2,h3,h4};
    for (int i = 0; i < 5; i++) {
        out[4*i]   = (uint8_t)(hs[i]>>24);
        out[4*i+1] = (uint8_t)(hs[i]>>16);
        out[4*i+2] = (uint8_t)(hs[i]>>8);
        out[4*i+3] = (uint8_t)(hs[i]);
    }
}

static void xor_bytes(uint8_t* dst, const uint8_t* a, const uint8_t* b, size_t n) {
    for (size_t i = 0; i < n; i++) dst[i] = a[i] ^ b[i];
}

// mysql_native_password: SHA1(password) XOR SHA1(salt + SHA1(SHA1(password)))
static void mysql_native_password(const char* password, const uint8_t* salt,
                                  size_t salt_len, uint8_t out[20]) {
    if (!password || password[0] == '\0') { memset(out, 0, 20); return; }
    size_t plen = strlen(password);
    uint8_t stage1[20], stage2[20], combined[40];

    sha1_compute((const uint8_t*)password, plen, stage1);
    sha1_compute(stage1, 20, stage2);
    memcpy(combined, salt, salt_len < 20 ? salt_len : 20);
    memcpy(combined + 20, stage2, 20);

    uint8_t stage3[20];
    sha1_compute(combined, 40, stage3);
    xor_bytes(out, stage1, stage3, 20);
}

// ── Handshake & Authentication ────────────────────────────────────────────

static bool mysql_do_handshake(vito_mysql_conn_t* conn,
                               const vito_mysql_config_t* config) {
    uint8_t pkt[4096];
    size_t  pkt_len = 0;
    conn->seq_id = 0;

    // Read server greeting (Handshake v10)
    if (!read_packet(conn, pkt, sizeof(pkt), &pkt_len)) return false;

    size_t off = 0;
    uint8_t proto_ver = pkt[off++];
    if (proto_ver != 10) {
        snprintf(conn->error_msg, sizeof(conn->error_msg),
                 "Unsupported MySQL protocol version: %d", proto_ver);
        return false;
    }

    // Skip server version (NUL-terminated)
    while (off < pkt_len && pkt[off] != 0) off++;
    off++; // skip NUL

    if (off + 4 > pkt_len) return false;
    /* uint32_t thread_id = */ read_u32_le(pkt + off); off += 4;

    // Auth plugin data part 1 (8 bytes)
    uint8_t auth_data[21] = {0};
    memcpy(auth_data, pkt + off, 8); off += 8;
    off++; // skip filler

    uint32_t caps_lo = read_u16_le(pkt + off); off += 2;
    off++; // charset
    /* uint16_t status = */ read_u16_le(pkt + off); off += 2;
    uint32_t caps_hi = read_u16_le(pkt + off); off += 2;
    conn->server_capabilities = caps_lo | (caps_hi << 16);

    uint8_t auth_data_len = pkt[off++];
    off += 10; // reserved

    // Auth plugin data part 2
    size_t part2_len = (auth_data_len > 8) ? (size_t)(auth_data_len - 8) : 12;
    if (part2_len > 13) part2_len = 13;
    memcpy(auth_data + 8, pkt + off, part2_len - 1);
    off += part2_len;

    // Compute auth response
    uint8_t auth_resp[20] = {0};
    mysql_native_password(config->password, auth_data, 20, auth_resp);

    // Build HandshakeResponse41
    conn->client_capabilities = MYSQL_CAP_PROTOCOL_41
                               | MYSQL_CAP_SECURE_CONNECTION
                               | MYSQL_CAP_PLUGIN_AUTH
                               | MYSQL_CAP_LONG_FLAG
                               | MYSQL_CAP_DEPRECATE_EOF;
    if (config->database && config->database[0])
        conn->client_capabilities |= MYSQL_CAP_CONNECT_WITH_DB;

    uint8_t resp[512];
    size_t  roff = 0;

    write_u32_le(resp + roff, conn->client_capabilities); roff += 4;
    write_u32_le(resp + roff, 16777216); roff += 4;  // max packet size
    resp[roff++] = 45; // utf8mb4 charset
    memset(resp + roff, 0, 23); roff += 23;  // reserved

    // username + NUL
    size_t ulen = strlen(config->user);
    memcpy(resp + roff, config->user, ulen); roff += ulen;
    resp[roff++] = 0;

    // auth response (lenenc)
    resp[roff++] = 20;  // length of auth_resp
    memcpy(resp + roff, auth_resp, 20); roff += 20;

    // database
    if (config->database && config->database[0]) {
        size_t dlen = strlen(config->database);
        memcpy(resp + roff, config->database, dlen); roff += dlen;
        resp[roff++] = 0;
    }

    // auth plugin name
    const char* plugin = "mysql_native_password";
    memcpy(resp + roff, plugin, strlen(plugin)); roff += strlen(plugin);
    resp[roff++] = 0;

    if (!send_packet(conn, resp, roff)) return false;

    // Read auth response (OK or ERR)
    if (!read_packet(conn, pkt, sizeof(pkt), &pkt_len)) return false;
    if (pkt[0] != 0x00) { // 0x00 = OK
        snprintf(conn->error_msg, sizeof(conn->error_msg),
                 "MySQL auth failed (code=0x%02X): %.*s",
                 pkt[0], (int)(pkt_len > 5 ? pkt_len - 5 : 0), pkt + 5);
        return false;
    }

    return true;
}

// ── Connection API ────────────────────────────────────────────────────────

vito_mysql_conn_t* vito_mysql_connect(const vito_mysql_config_t* config) {
    if (!config || !config->host || !config->user) return NULL;

#if defined(_WIN32) || defined(_WIN64)
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    vito_mysql_conn_t* conn = (vito_mysql_conn_t*)calloc(1, sizeof(vito_mysql_conn_t));
    if (!conn) return NULL;
    conn->sock = INVALID_SOCK;

    uint16_t port = config->port ? config->port : 3306;

    struct addrinfo hints, *res = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    char port_str[8];
    snprintf(port_str, sizeof(port_str), "%u", port);

    if (getaddrinfo(config->host, port_str, &hints, &res) != 0) {
        snprintf(conn->error_msg, sizeof(conn->error_msg),
                 "Cannot resolve host: %s", config->host);
        free(conn);
        return NULL;
    }

    conn->sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (conn->sock == INVALID_SOCK) {
        freeaddrinfo(res);
        snprintf(conn->error_msg, sizeof(conn->error_msg), "socket() failed");
        free(conn);
        return NULL;
    }

    if (connect(conn->sock, res->ai_addr, (int)res->ai_addrlen) != 0) {
        freeaddrinfo(res);
        snprintf(conn->error_msg, sizeof(conn->error_msg),
                 "connect() to %s:%u failed", config->host, port);
        sock_close(conn->sock);
        free(conn);
        return NULL;
    }
    freeaddrinfo(res);

    if (!mysql_do_handshake(conn, config)) {
        sock_close(conn->sock);
        // error_msg already set in do_handshake
        free(conn);
        return NULL;
    }

    conn->pipeline_enabled = config->pipeline_mode;
    return conn;
}

void vito_mysql_close(vito_mysql_conn_t* conn) {
    if (!conn) return;
    if (conn->sock != INVALID_SOCK) {
        // Send COM_QUIT
        uint8_t quit_pkt[5] = {1, 0, 0, 0, MYSQL_COM_QUIT};
        sock_send_all(conn->sock, quit_pkt, 5);
        sock_close(conn->sock);
        conn->sock = INVALID_SOCK;
    }
    free(conn);
}

bool vito_mysql_ping(vito_mysql_conn_t* conn) {
    if (!conn || conn->sock == INVALID_SOCK) return false;
    uint8_t ping_pkt[5] = {1, 0, 0, 0, MYSQL_COM_PING};
    conn->seq_id = 0;
    if (!sock_send_all(conn->sock, ping_pkt, 5)) return false;
    uint8_t pkt[64]; size_t pkt_len = 0;
    if (!read_packet(conn, pkt, sizeof(pkt), &pkt_len)) return false;
    return pkt[0] == 0x00;
}

const char* vito_mysql_errmsg(vito_mysql_conn_t* conn) {
    if (!conn) return "null connection";
    return conn->error_msg;
}

// ── Column definition decoder ─────────────────────────────────────────────

static bool decode_field_packet(const uint8_t* pkt, size_t len,
                                vito_mysql_field_t* field) {
    size_t off = 0;
    // Skip: catalog (lenenc), schema (lenenc), table_alias (lenenc), table (lenenc)
    for (int i = 0; i < 4; i++) {
        uint64_t slen = mysql_read_lenenc_int(pkt, len, &off);
        off += slen;
    }
    // col_alias (lenenc)
    uint64_t alias_len = mysql_read_lenenc_int(pkt, len, &off);
    if (alias_len > 63) alias_len = 63;
    memcpy(field->name, pkt + off, (size_t)alias_len);
    field->name[alias_len] = '\0';
    off += alias_len;
    // col name (lenenc)
    uint64_t name_len = mysql_read_lenenc_int(pkt, len, &off);
    off += name_len;

    off++; // filler 0x0C
    off += 2; // charset
    off += 4; // col length
    field->type = pkt[off++];
    field->flags = read_u16_le(pkt + off); off += 2;
    field->decimals = pkt[off++];
    field->is_unsigned = (field->flags & 0x0020) != 0;
    return true;
}

// ── Result set decoder (text protocol) ───────────────────────────────────

static bool decode_text_result(vito_mysql_conn_t* conn, vito_mysql_result_t* result) {
    uint8_t pkt[65540]; size_t pkt_len = 0;

    // Read column count packet
    if (!read_packet(conn, pkt, sizeof(pkt), &pkt_len)) return false;
    if (pkt[0] == 0xFF) {
        snprintf(conn->error_msg, sizeof(conn->error_msg),
                 "MySQL error #%u: %.*s",
                 read_u16_le(pkt+1), (int)(pkt_len-3), pkt+3);
        result->success = false;
        return false;
    }

    size_t off = 0;
    uint64_t col_count = mysql_read_lenenc_int(pkt, pkt_len, &off);
    result->col_count = (uint16_t)(col_count > VITO_MYSQL_MAX_COLS ? VITO_MYSQL_MAX_COLS : col_count);

    // Read column definitions
    for (uint16_t c = 0; c < result->col_count; c++) {
        if (!read_packet(conn, pkt, sizeof(pkt), &pkt_len)) return false;
        decode_field_packet(pkt, pkt_len, &result->fields[c]);
    }

    // Read EOF packet (if server doesn't support DEPRECATE_EOF)
    if (!(conn->client_capabilities & MYSQL_CAP_DEPRECATE_EOF)) {
        if (!read_packet(conn, pkt, sizeof(pkt), &pkt_len)) return false; // EOF
    }

    // Read rows
    uint32_t row_idx = 0;
    while (true) {
        if (!read_packet(conn, pkt, sizeof(pkt), &pkt_len)) break;

        // EOF packet (0xFE with len<9) or OK packet = end of rows
        if ((pkt[0] == 0xFE && pkt_len < 9) || pkt[0] == 0x00) break;
        if (pkt[0] == 0xFF) {
            snprintf(conn->error_msg, sizeof(conn->error_msg),
                     "Row read error #%u: %.*s",
                     read_u16_le(pkt+1), (int)(pkt_len-3), pkt+3);
            result->success = false;
            return false;
        }

        if (row_idx >= VITO_MYSQL_MAX_ROWS) { row_idx++; continue; }

        vito_mysql_row_t* row = &result->rows[row_idx];
        row->col_count = result->col_count;
        off = 0;

        for (uint16_t c = 0; c < result->col_count; c++) {
            vito_mysql_col_t* col = &row->cols[c];
            if (pkt[off] == 0xFB) {
                col->is_null = true;
                col->data    = NULL;
                col->len     = 0;
                off++;
            } else {
                col->is_null = false;
                const uint8_t* str = NULL;
                uint32_t slen = 0;
                mysql_read_lenenc_str(pkt, pkt_len, &off, &str, &slen);
                col->data       = str;
                col->len        = slen;
                col->mysql_type = result->fields[c].type;
            }
        }
        row_idx++;
    }

    result->row_count = (row_idx > VITO_MYSQL_MAX_ROWS) ? VITO_MYSQL_MAX_ROWS : row_idx;
    result->success   = true;
    return true;
}

// ── Query API ─────────────────────────────────────────────────────────────

size_t mysql_build_com_query(const char* sql, uint8_t seq_id,
                             uint8_t* out_buf, size_t out_cap) {
    if (!sql || !out_buf) return 0;
    size_t sql_len = strlen(sql);
    size_t total   = 4 + 1 + sql_len; // header + COM_QUERY byte + sql
    if (total > out_cap) return 0;
    write_packet_header(out_buf, (uint32_t)(1 + sql_len), seq_id);
    out_buf[4] = MYSQL_COM_QUERY;
    memcpy(out_buf + 5, sql, sql_len);
    return total;
}

bool vito_mysql_query(vito_mysql_conn_t* conn, const char* sql,
                      vito_mysql_result_t* result_out) {
    if (!conn || !sql || !result_out) return false;
    memset(result_out, 0, sizeof(vito_mysql_result_t));
    conn->seq_id = 0;

    uint8_t pkt[65540];
    size_t pkt_len = mysql_build_com_query(sql, 0, pkt, sizeof(pkt));
    if (!pkt_len || !sock_send_all(conn->sock, pkt, pkt_len)) {
        snprintf(conn->error_msg, sizeof(conn->error_msg), "Failed to send COM_QUERY");
        return false;
    }
    conn->seq_id = 1;

    return decode_text_result(conn, result_out);
}

int64_t vito_mysql_exec(vito_mysql_conn_t* conn, const char* sql) {
    if (!conn || !sql) return -1;
    vito_mysql_result_t result;
    memset(&result, 0, sizeof(result));
    conn->seq_id = 0;

    uint8_t pkt[65540];
    size_t pkt_len = mysql_build_com_query(sql, 0, pkt, sizeof(pkt));
    if (!pkt_len || !sock_send_all(conn->sock, pkt, pkt_len)) return -1;
    conn->seq_id = 1;

    // Read response — should be OK packet for non-SELECT
    uint8_t resp[64]; size_t resp_len = 0;
    if (!read_packet(conn, resp, sizeof(resp), &resp_len)) return -1;

    if (resp[0] == 0x00) {
        size_t off = 1;
        result.affected_rows = (uint32_t)mysql_read_lenenc_int(resp, resp_len, &off);
        result.insert_id     = mysql_read_lenenc_int(resp, resp_len, &off);
        return (int64_t)result.affected_rows;
    }
    if (resp[0] == 0xFF) {
        snprintf(conn->error_msg, sizeof(conn->error_msg),
                 "MySQL exec error #%u: %.*s",
                 read_u16_le(resp+1), (int)(resp_len-3), resp+3);
    }
    // May be a result set (e.g., user ran SELECT via exec) — drain it
    return -1;
}

// ── Prepared Statements ───────────────────────────────────────────────────

vito_mysql_stmt_t* vito_mysql_prepare(vito_mysql_conn_t* conn, const char* sql) {
    if (!conn || !sql) return NULL;
    conn->seq_id = 0;

    size_t sql_len = strlen(sql);
    uint8_t pkt[65540];
    write_packet_header(pkt, (uint32_t)(1 + sql_len), 0);
    pkt[4] = MYSQL_COM_STMT_PREPARE;
    memcpy(pkt + 5, sql, sql_len);

    if (!sock_send_all(conn->sock, pkt, 5 + sql_len)) return NULL;
    conn->seq_id = 1;

    uint8_t resp[64]; size_t resp_len = 0;
    if (!read_packet(conn, resp, sizeof(resp), &resp_len)) return NULL;
    if (resp[0] != 0x00) {
        snprintf(conn->error_msg, sizeof(conn->error_msg),
                 "COM_STMT_PREPARE failed (0x%02X)", resp[0]);
        return NULL;
    }

    vito_mysql_stmt_t* s = (vito_mysql_stmt_t*)calloc(1, sizeof(vito_mysql_stmt_t));
    if (!s) return NULL;
    s->conn       = conn;
    s->stmt_id    = read_u32_le(resp + 1);
    s->col_count  = read_u16_le(resp + 5);
    s->param_count = read_u16_le(resp + 7);

    // Drain param definition packets
    for (uint16_t i = 0; i < s->param_count; i++) {
        uint8_t tmp[256]; size_t tmp_len = 0;
        read_packet(conn, tmp, sizeof(tmp), &tmp_len);
    }
    if (s->param_count > 0 && !(conn->client_capabilities & MYSQL_CAP_DEPRECATE_EOF)) {
        uint8_t tmp[64]; size_t tmp_len = 0;
        read_packet(conn, tmp, sizeof(tmp), &tmp_len); // EOF
    }
    // Drain column definition packets
    for (uint16_t i = 0; i < s->col_count; i++) {
        uint8_t tmp[256]; size_t tmp_len = 0;
        read_packet(conn, tmp, sizeof(tmp), &tmp_len);
        decode_field_packet(tmp, tmp_len, &s->fields[i]);
    }
    if (s->col_count > 0 && !(conn->client_capabilities & MYSQL_CAP_DEPRECATE_EOF)) {
        uint8_t tmp[64]; size_t tmp_len = 0;
        read_packet(conn, tmp, sizeof(tmp), &tmp_len); // EOF
    }

    return s;
}

bool vito_mysql_bind_text(vito_mysql_stmt_t* stmt, int idx, const char* val, int len) {
    if (!stmt || idx < 1 || idx > VITO_MYSQL_MAX_COLS) return false;
    int i = idx - 1;
    if (len < 0) len = (int)strlen(val);
    if (len > 255) len = 255;
    memcpy(stmt->param_data[i], val, (size_t)len);
    stmt->param_lens[i]  = (uint16_t)len;
    stmt->param_types[i] = MYSQL_TYPE_VAR_STRING;
    stmt->param_null[i]  = false;
    return true;
}

bool vito_mysql_bind_int(vito_mysql_stmt_t* stmt, int idx, int64_t val) {
    if (!stmt || idx < 1 || idx > VITO_MYSQL_MAX_COLS) return false;
    int i = idx - 1;
    snprintf(stmt->param_data[i], sizeof(stmt->param_data[i]), "%lld", (long long)val);
    stmt->param_lens[i]  = (uint16_t)strlen(stmt->param_data[i]);
    stmt->param_types[i] = MYSQL_TYPE_LONGLONG;
    stmt->param_null[i]  = false;
    return true;
}

bool vito_mysql_bind_null(vito_mysql_stmt_t* stmt, int idx) {
    if (!stmt || idx < 1 || idx > VITO_MYSQL_MAX_COLS) return false;
    int i = idx - 1;
    stmt->param_null[i]  = true;
    stmt->param_types[i] = MYSQL_TYPE_NULL;
    return true;
}

size_t mysql_build_stmt_execute(uint32_t stmt_id, const vito_mysql_col_t* params,
                                uint16_t param_count, uint8_t seq_id,
                                uint8_t* out_buf, size_t out_cap) {
    if (!out_buf || out_cap < 20) return 0;
    size_t off = 4; // leave space for header
    out_buf[off++] = MYSQL_COM_STMT_EXECUTE;
    write_u32_le(out_buf + off, stmt_id); off += 4;
    out_buf[off++] = 0; // flags (no cursor)
    write_u32_le(out_buf + off, 1); off += 4; // iteration count

    if (param_count > 0) {
        // Null bitmap: ceil(param_count / 8) bytes
        size_t null_bitmap_len = (param_count + 7) / 8;
        size_t null_bitmap_off = off;
        memset(out_buf + off, 0, null_bitmap_len); off += null_bitmap_len;
        out_buf[off++] = 1; // new-params-bound flag

        for (uint16_t i = 0; i < param_count && i < VITO_MYSQL_MAX_COLS; i++) {
            if (params[i].is_null) {
                out_buf[null_bitmap_off + i/8] |= (1 << (i%8));
                write_u16_le(out_buf + off, MYSQL_TYPE_NULL); off += 2;
            } else {
                write_u16_le(out_buf + off, params[i].mysql_type); off += 2;
            }
        }

        // Parameter values
        for (uint16_t i = 0; i < param_count && i < VITO_MYSQL_MAX_COLS; i++) {
            if (!params[i].is_null && params[i].data) {
                out_buf[off++] = (uint8_t)params[i].len;
                memcpy(out_buf + off, params[i].data, params[i].len);
                off += params[i].len;
            }
        }
    }

    size_t payload_len = off - 4;
    write_packet_header(out_buf, (uint32_t)payload_len, seq_id);
    return off;
}

bool vito_mysql_stmt_execute(vito_mysql_stmt_t* stmt, vito_mysql_result_t* result_out) {
    if (!stmt || !result_out) return false;
    memset(result_out, 0, sizeof(vito_mysql_result_t));

    vito_mysql_col_t params[VITO_MYSQL_MAX_COLS] = {0};
    for (uint16_t i = 0; i < stmt->param_count && i < VITO_MYSQL_MAX_COLS; i++) {
        params[i].is_null    = stmt->param_null[i];
        params[i].data       = (const uint8_t*)stmt->param_data[i];
        params[i].len        = stmt->param_lens[i];
        params[i].mysql_type = stmt->param_types[i];
    }

    stmt->conn->seq_id = 0;
    uint8_t pkt[65540];
    size_t pkt_len = mysql_build_stmt_execute(stmt->stmt_id, params,
                                              stmt->param_count, 0,
                                              pkt, sizeof(pkt));
    if (!pkt_len || !sock_send_all(stmt->conn->sock, pkt, pkt_len)) return false;
    stmt->conn->seq_id = 1;

    return decode_text_result(stmt->conn, result_out);
}

void vito_mysql_stmt_close(vito_mysql_stmt_t* stmt) {
    if (!stmt) return;
    if (stmt->conn && stmt->conn->sock != INVALID_SOCK) {
        uint8_t pkt[9];
        write_packet_header(pkt, 5, 0);
        pkt[4] = MYSQL_COM_STMT_CLOSE;
        write_u32_le(pkt + 5, stmt->stmt_id);
        sock_send_all(stmt->conn->sock, pkt, 9);
    }
    free(stmt);
}

// ── Transactions ──────────────────────────────────────────────────────────

bool vito_mysql_begin(vito_mysql_conn_t* conn) {
    return vito_mysql_exec(conn, "START TRANSACTION") >= 0;
}

bool vito_mysql_commit(vito_mysql_conn_t* conn) {
    return vito_mysql_exec(conn, "COMMIT") >= 0;
}

bool vito_mysql_rollback(vito_mysql_conn_t* conn) {
    return vito_mysql_exec(conn, "ROLLBACK") >= 0;
}

// ── Pipeline Query API ────────────────────────────────────────────────────

int vito_mysql_pipeline_exec(vito_mysql_conn_t* conn, const char* sql_batch,
                             vito_mysql_result_t* results_out, int max_results) {
    if (!conn || !sql_batch || !results_out || max_results <= 0) return -1;

    // MySQL multi-statement: send all at once, read multiple result sets
    // Requires CLIENT_MULTI_STATEMENTS capability (we enable it here logically)
    conn->seq_id = 0;
    size_t sql_len = strlen(sql_batch);
    uint8_t* pkt = (uint8_t*)malloc(sql_len + 16);
    if (!pkt) return -1;

    write_packet_header(pkt, (uint32_t)(1 + sql_len), 0);
    pkt[4] = MYSQL_COM_QUERY;
    memcpy(pkt + 5, sql_batch, sql_len);
    bool sent = sock_send_all(conn->sock, pkt, 5 + sql_len);
    free(pkt);
    if (!sent) return -1;
    conn->seq_id = 1;

    int result_count = 0;
    while (result_count < max_results) {
        bool ok = decode_text_result(conn, &results_out[result_count]);
        result_count++;
        if (!ok) break;

        // Check for more result sets (server status flag MORE_RESULTS_EXISTS)
        // In a real implementation, check status flags from OK/EOF packet
        // For now: break after first batch unless pipeline_enabled
        if (!conn->pipeline_enabled) break;
    }

    return result_count;
}
