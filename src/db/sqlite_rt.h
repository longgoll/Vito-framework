#ifndef VITO_SQLITE_RT_H
#define VITO_SQLITE_RT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================
// Vito SQLite Native Runtime (Zero-Allocation FFI Binding)
// Wraps SQLite3 amalgamation with arena-friendly result sets
// ============================================================

#define VITO_SQLITE_MAX_COLS    64
#define VITO_SQLITE_MAX_ROWS    512
#define VITO_SQLITE_OK          0
#define VITO_SQLITE_ERROR       1
#define VITO_SQLITE_BUSY        5
#define VITO_SQLITE_LOCKED      6
#define VITO_SQLITE_CONSTRAINT  19

// Opaque handle to an open SQLite database connection
typedef struct vito_sqlite_db_t vito_sqlite_db_t;

// Opaque handle to a prepared statement
typedef struct vito_sqlite_stmt_t vito_sqlite_stmt_t;

// A single column value — pointer into the result buffer (zero-copy)
typedef struct {
    const char* data;      // UTF-8 string pointer (null if NULL)
    uint32_t    len;       // Byte length of data
    bool        is_null;   // True if the column value is SQL NULL
    uint8_t     type;      // 0=text, 1=int, 2=float, 3=blob, 4=null
} vito_sqlite_col_t;

// One decoded row of query results
typedef struct {
    uint16_t         col_count;
    vito_sqlite_col_t cols[VITO_SQLITE_MAX_COLS];
} vito_sqlite_row_t;

// Full result set from a SELECT query (stack-allocated rows)
typedef struct {
    uint32_t          row_count;
    uint16_t          col_count;
    char              col_names[VITO_SQLITE_MAX_COLS][64];
    vito_sqlite_row_t rows[VITO_SQLITE_MAX_ROWS];
    int               error_code;
    char              error_msg[256];
    bool              success;
} vito_sqlite_result_t;

// WAL checkpoint result
typedef struct {
    int log_frames;         // Frames in WAL log
    int checkpointed;       // Frames successfully checkpointed
    bool ok;
} vito_sqlite_checkpoint_t;

// --- Core API ---

// Open a SQLite database file. Returns NULL on failure.
// WAL mode is enabled automatically for concurrency.
vito_sqlite_db_t* vito_sqlite_open(const char* file_path);

// Close an open database handle and release all resources.
void vito_sqlite_close(vito_sqlite_db_t* db);

// Execute a non-SELECT statement (CREATE, INSERT, UPDATE, DELETE, etc.)
// Returns number of rows affected, or -1 on error.
int vito_sqlite_exec(vito_sqlite_db_t* db, const char* sql);

// Execute a SELECT statement and populate result into pre-allocated result_out.
// Returns true on success.
bool vito_sqlite_query(vito_sqlite_db_t* db, const char* sql, vito_sqlite_result_t* result_out);

// Prepare a statement for repeated use with bound parameters.
vito_sqlite_stmt_t* vito_sqlite_prepare(vito_sqlite_db_t* db, const char* sql);

// Bind a text parameter by index (1-based) to a prepared statement.
bool vito_sqlite_bind_text(vito_sqlite_stmt_t* stmt, int idx, const char* val, int len);

// Bind an integer parameter by index (1-based) to a prepared statement.
bool vito_sqlite_bind_int(vito_sqlite_stmt_t* stmt, int idx, int64_t val);

// Step (execute) a prepared statement and fetch one row at a time.
// Returns: 1 = SQLITE_ROW (more data), 0 = SQLITE_DONE, -1 = error
int vito_sqlite_step(vito_sqlite_stmt_t* stmt, vito_sqlite_row_t* row_out);

// Reset prepared statement for re-use.
void vito_sqlite_reset(vito_sqlite_stmt_t* stmt);

// Finalize (destroy) a prepared statement.
void vito_sqlite_finalize(vito_sqlite_stmt_t* stmt);

// Get the last auto-inserted rowid for the connection.
int64_t vito_sqlite_last_insert_rowid(vito_sqlite_db_t* db);

// Get the last error message as a C string.
const char* vito_sqlite_errmsg(vito_sqlite_db_t* db);

// --- Transaction API ---
bool vito_sqlite_begin(vito_sqlite_db_t* db);
bool vito_sqlite_commit(vito_sqlite_db_t* db);
bool vito_sqlite_rollback(vito_sqlite_db_t* db);

// --- WAL Mode & Maintenance ---

// Enable WAL (Write-Ahead Logging) mode for better concurrency.
// Called automatically by vito_sqlite_open.
bool vito_sqlite_enable_wal(vito_sqlite_db_t* db);

// Perform a WAL checkpoint. Returns stats about frames synced.
vito_sqlite_checkpoint_t vito_sqlite_checkpoint(vito_sqlite_db_t* db);

// Set busy timeout in milliseconds (retries locked reads/writes).
void vito_sqlite_set_busy_timeout(vito_sqlite_db_t* db, int ms);

#ifdef __cplusplus
}
#endif

#endif // VITO_SQLITE_RT_H
