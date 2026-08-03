#ifndef VITO_MYSQL_RT_H
#define VITO_MYSQL_RT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================
// Vito MySQL Native Runtime — Zero-Dependency Wire Protocol
// Implements MySQL Client/Server Protocol v10 (MySQL 5.7+/8.0+)
// with Binary Result Set decoding and COM_QUERY pipelining
// ============================================================

#define VITO_MYSQL_MAX_COLS        64
#define VITO_MYSQL_MAX_ROWS        512
#define VITO_MYSQL_PACKET_MAX_LEN  (16 * 1024 * 1024)  // 16 MB

// Client capability flags (subset for Vito)
#define MYSQL_CAP_PROTOCOL_41      0x00000200
#define MYSQL_CAP_SECURE_CONNECTION 0x00008000
#define MYSQL_CAP_PLUGIN_AUTH      0x00080000
#define MYSQL_CAP_CONNECT_WITH_DB  0x00000008
#define MYSQL_CAP_LONG_FLAG        0x00000004
#define MYSQL_CAP_DEPRECATE_EOF    0x01000000

// COM_ command bytes
#define MYSQL_COM_QUIT             0x01
#define MYSQL_COM_QUERY            0x03
#define MYSQL_COM_STMT_PREPARE     0x16
#define MYSQL_COM_STMT_EXECUTE     0x17
#define MYSQL_COM_STMT_CLOSE       0x19
#define MYSQL_COM_PING             0x0E
#define MYSQL_COM_RESET_CONNECTION 0x1F

// MySQL field types (for binary result set)
#define MYSQL_TYPE_DECIMAL     0x00
#define MYSQL_TYPE_TINY        0x01
#define MYSQL_TYPE_SHORT       0x02
#define MYSQL_TYPE_LONG        0x03
#define MYSQL_TYPE_FLOAT       0x04
#define MYSQL_TYPE_DOUBLE      0x05
#define MYSQL_TYPE_NULL        0x06
#define MYSQL_TYPE_TIMESTAMP   0x07
#define MYSQL_TYPE_LONGLONG    0x08
#define MYSQL_TYPE_INT24       0x09
#define MYSQL_TYPE_DATE        0x0A
#define MYSQL_TYPE_TIME        0x0B
#define MYSQL_TYPE_DATETIME    0x0C
#define MYSQL_TYPE_VARCHAR     0x0F
#define MYSQL_TYPE_BIT         0x10
#define MYSQL_TYPE_VAR_STRING  0xFD
#define MYSQL_TYPE_STRING      0xFE
#define MYSQL_TYPE_BLOB        0xFC

// Error codes
#define VITO_MYSQL_OK            0
#define VITO_MYSQL_ERR_CONNECT   1
#define VITO_MYSQL_ERR_AUTH      2
#define VITO_MYSQL_ERR_QUERY     3
#define VITO_MYSQL_ERR_PACKET    4
#define VITO_MYSQL_ERR_TIMEOUT   5

// Connection configuration
typedef struct {
    const char* host;
    uint16_t    port;           // Default: 3306
    const char* user;
    const char* password;
    const char* database;
    uint32_t    connect_timeout_ms;
    uint32_t    read_timeout_ms;
    bool        use_tls;
    bool        pipeline_mode;  // Enable COM_QUERY pipelining (MySQL 8.0+)
} vito_mysql_config_t;

// A single column value in a result row
typedef struct {
    const uint8_t* data;        // Raw bytes (NULL if is_null)
    uint32_t       len;
    uint8_t        mysql_type;  // MYSQL_TYPE_* constant
    bool           is_null;
    bool           is_unsigned;
} vito_mysql_col_t;

// Field metadata (from column definition packet)
typedef struct {
    char     name[64];
    char     table[64];
    uint8_t  type;
    uint16_t flags;
    uint8_t  decimals;
    bool     is_unsigned;
} vito_mysql_field_t;

// One decoded row from a result set
typedef struct {
    uint16_t        col_count;
    vito_mysql_col_t cols[VITO_MYSQL_MAX_COLS];
} vito_mysql_row_t;

// Full result set
typedef struct {
    bool              success;
    int               error_code;
    char              error_msg[512];
    uint32_t          affected_rows;
    uint64_t          insert_id;
    uint32_t          row_count;
    uint16_t          col_count;
    vito_mysql_field_t fields[VITO_MYSQL_MAX_COLS];
    vito_mysql_row_t  rows[VITO_MYSQL_MAX_ROWS];
} vito_mysql_result_t;

// Prepared statement handle
typedef struct vito_mysql_stmt_t vito_mysql_stmt_t;

// Connection handle
typedef struct vito_mysql_conn_t vito_mysql_conn_t;

// ── Protocol-level packet building (public for pipeline use) ──────────────

// Build a raw COM_QUERY packet into out_buf. Returns packet length or 0 on error.
size_t mysql_build_com_query(const char* sql, uint8_t seq_id,
                             uint8_t* out_buf, size_t out_cap);

// Parse a MySQL 3-byte little-endian packet length from header.
uint32_t mysql_read_packet_len(const uint8_t* header);

// Read a length-encoded integer (lenenc) from buffer at *offset.
// Advances *offset past the integer. Returns value.
uint64_t mysql_read_lenenc_int(const uint8_t* buf, size_t buf_len, size_t* offset);

// Read a length-encoded string. Sets *str_out and *str_len, advances *offset.
bool mysql_read_lenenc_str(const uint8_t* buf, size_t buf_len, size_t* offset,
                           const uint8_t** str_out, uint32_t* str_len);

// Build COM_STMT_EXECUTE packet for a prepared statement.
size_t mysql_build_stmt_execute(uint32_t stmt_id, const vito_mysql_col_t* params,
                                uint16_t param_count, uint8_t seq_id,
                                uint8_t* out_buf, size_t out_cap);

// ── Connection API ────────────────────────────────────────────────────────

// Open a new connection and perform handshake + auth.
vito_mysql_conn_t* vito_mysql_connect(const vito_mysql_config_t* config);

// Close the connection gracefully (sends COM_QUIT).
void vito_mysql_close(vito_mysql_conn_t* conn);

// Ping the server (keeps connection alive). Returns true on pong.
bool vito_mysql_ping(vito_mysql_conn_t* conn);

// Get last error message for a connection.
const char* vito_mysql_errmsg(vito_mysql_conn_t* conn);

// ── Query API ─────────────────────────────────────────────────────────────

// Execute a query with full result set decoding into result_out.
bool vito_mysql_query(vito_mysql_conn_t* conn, const char* sql,
                      vito_mysql_result_t* result_out);

// Execute a non-SELECT statement. Returns affected rows or -1 on error.
int64_t vito_mysql_exec(vito_mysql_conn_t* conn, const char* sql);

// ── Prepared Statements ───────────────────────────────────────────────────

// Prepare a statement. Returns handle or NULL on error.
vito_mysql_stmt_t* vito_mysql_prepare(vito_mysql_conn_t* conn, const char* sql);

// Bind text parameter (1-based index).
bool vito_mysql_bind_text(vito_mysql_stmt_t* stmt, int idx, const char* val, int len);

// Bind int64 parameter (1-based index).
bool vito_mysql_bind_int(vito_mysql_stmt_t* stmt, int idx, int64_t val);

// Bind NULL parameter.
bool vito_mysql_bind_null(vito_mysql_stmt_t* stmt, int idx);

// Execute prepared statement and fetch result.
bool vito_mysql_stmt_execute(vito_mysql_stmt_t* stmt, vito_mysql_result_t* result_out);

// Close and free a prepared statement.
void vito_mysql_stmt_close(vito_mysql_stmt_t* stmt);

// ── Transaction API ───────────────────────────────────────────────────────

bool vito_mysql_begin(vito_mysql_conn_t* conn);
bool vito_mysql_commit(vito_mysql_conn_t* conn);
bool vito_mysql_rollback(vito_mysql_conn_t* conn);

// ── Pipeline Query API (MySQL 8.0+ / MariaDB 10.2+) ─────────────────────

// Send multiple SQL statements in one round-trip.
// Returns number of result sets decoded, or -1 on error.
int vito_mysql_pipeline_exec(vito_mysql_conn_t* conn,
                             const char* sql_batch,      // Semicolon-separated
                             vito_mysql_result_t* results_out,
                             int max_results);

#ifdef __cplusplus
}
#endif

#endif // VITO_MYSQL_RT_H
