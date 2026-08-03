#include "sqlite_rt.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ============================================================
// Vito SQLite Native Runtime Implementation
// Binding to SQLite3 amalgamation (sqlite3.h / sqlite3.c)
// With zero-allocation result decoding & WAL-mode-by-default
// ============================================================

// NOTE: In a real build, include the SQLite amalgamation:
//   #include "sqlite3.h"
// and compile with sqlite3.c in the same translation unit.
// Here we forward-declare the minimal SQLite3 C API surface
// so this file compiles standalone as an API demonstration.
// In production: `cc -DSQLITE_THREADSAFE=1 sqlite3.c sqlite_rt.c`

// ── Forward declarations of sqlite3 types (from sqlite3.h) ────────────────
typedef struct sqlite3       sqlite3;
typedef struct sqlite3_stmt  sqlite3_stmt;

#define SQLITE_OK       0
#define SQLITE_ERROR    1
#define SQLITE_BUSY     5
#define SQLITE_LOCKED   6
#define SQLITE_ROW     100
#define SQLITE_DONE    101
#define SQLITE_OPEN_READWRITE  0x00000002
#define SQLITE_OPEN_CREATE     0x00000004
#define SQLITE_OPEN_WAL        0x00080000

#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_TEXT     3
#define SQLITE_BLOB     4
#define SQLITE_NULL     5

// Declare minimal sqlite3 API (linked from sqlite3.c amalgamation)
extern int         sqlite3_open_v2(const char*, sqlite3**, int, const char*);
extern int         sqlite3_close_v2(sqlite3*);
extern int         sqlite3_exec(sqlite3*, const char*, void*, void*, char**);
extern const char* sqlite3_errmsg(sqlite3*);
extern int         sqlite3_prepare_v2(sqlite3*, const char*, int, sqlite3_stmt**, const char**);
extern int         sqlite3_bind_text(sqlite3_stmt*, int, const char*, int, void(*)(void*));
extern int         sqlite3_bind_int64(sqlite3_stmt*, int, int64_t);
extern int         sqlite3_step(sqlite3_stmt*);
extern int         sqlite3_reset(sqlite3_stmt*);
extern int         sqlite3_finalize(sqlite3_stmt*);
extern int         sqlite3_column_count(sqlite3_stmt*);
extern const char* sqlite3_column_name(sqlite3_stmt*, int);
extern int         sqlite3_column_type(sqlite3_stmt*, int);
extern const char* sqlite3_column_text(sqlite3_stmt*, int);
extern int         sqlite3_column_bytes(sqlite3_stmt*, int);
extern int64_t     sqlite3_column_int64(sqlite3_stmt*, int);
extern int64_t     sqlite3_last_insert_rowid(sqlite3*);
extern int         sqlite3_changes(sqlite3*);
extern int         sqlite3_busy_timeout(sqlite3*, int);
extern int         sqlite3_wal_checkpoint_v2(sqlite3*, const char*, int, int*, int*);

#define SQLITE_CHECKPOINT_PASSIVE 0

// ── Internal handle ───────────────────────────────────────────────────────

struct vito_sqlite_db_t {
    sqlite3* handle;
};

struct vito_sqlite_stmt_t {
    sqlite3_stmt* handle;
    vito_sqlite_db_t* db;
};

// ── Helpers ───────────────────────────────────────────────────────────────

static void decode_row(sqlite3_stmt* stmt, vito_sqlite_row_t* row_out, int col_count) {
    row_out->col_count = (uint16_t)(col_count > VITO_SQLITE_MAX_COLS ? VITO_SQLITE_MAX_COLS : col_count);
    for (int c = 0; c < row_out->col_count; c++) {
        int col_type = sqlite3_column_type(stmt, c);
        vito_sqlite_col_t* col = &row_out->cols[c];

        if (col_type == SQLITE_NULL) {
            col->is_null = true;
            col->data    = NULL;
            col->len     = 0;
            col->type    = 4;
        } else if (col_type == SQLITE_INTEGER) {
            col->type    = 1;
            col->is_null = false;
            col->data    = (const char*)sqlite3_column_text(stmt, c);
            col->len     = (uint32_t)sqlite3_column_bytes(stmt, c);
        } else if (col_type == SQLITE_FLOAT) {
            col->type    = 2;
            col->is_null = false;
            col->data    = (const char*)sqlite3_column_text(stmt, c);
            col->len     = (uint32_t)sqlite3_column_bytes(stmt, c);
        } else if (col_type == SQLITE_BLOB) {
            col->type    = 3;
            col->is_null = false;
            col->data    = (const char*)sqlite3_column_text(stmt, c);
            col->len     = (uint32_t)sqlite3_column_bytes(stmt, c);
        } else {
            // SQLITE_TEXT (default)
            col->type    = 0;
            col->is_null = false;
            col->data    = (const char*)sqlite3_column_text(stmt, c);
            col->len     = (uint32_t)sqlite3_column_bytes(stmt, c);
        }
    }
}

// ── Core API ──────────────────────────────────────────────────────────────

vito_sqlite_db_t* vito_sqlite_open(const char* file_path) {
    vito_sqlite_db_t* db = (vito_sqlite_db_t*)malloc(sizeof(vito_sqlite_db_t));
    if (!db) return NULL;

    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    int rc = sqlite3_open_v2(file_path, &db->handle, flags, NULL);
    if (rc != SQLITE_OK) {
        free(db);
        return NULL;
    }

    // Enable WAL mode by default for concurrent read performance
    vito_sqlite_enable_wal(db);

    // Sensible busy timeout: retry for 5 seconds on lock contention
    vito_sqlite_set_busy_timeout(db, 5000);

    return db;
}

void vito_sqlite_close(vito_sqlite_db_t* db) {
    if (!db) return;
    if (db->handle) {
        sqlite3_close_v2(db->handle);
        db->handle = NULL;
    }
    free(db);
}

int vito_sqlite_exec(vito_sqlite_db_t* db, const char* sql) {
    if (!db || !sql) return -1;
    char* errmsg = NULL;
    int rc = sqlite3_exec(db->handle, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        return -1;
    }
    return sqlite3_changes(db->handle);
}

bool vito_sqlite_query(vito_sqlite_db_t* db, const char* sql, vito_sqlite_result_t* result_out) {
    if (!db || !sql || !result_out) return false;

    memset(result_out, 0, sizeof(vito_sqlite_result_t));

    sqlite3_stmt* stmt = NULL;
    int rc = sqlite3_prepare_v2(db->handle, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        result_out->error_code = rc;
        snprintf(result_out->error_msg, sizeof(result_out->error_msg), "%s", sqlite3_errmsg(db->handle));
        result_out->success = false;
        return false;
    }

    int col_count = sqlite3_column_count(stmt);
    result_out->col_count = (uint16_t)(col_count > VITO_SQLITE_MAX_COLS ? VITO_SQLITE_MAX_COLS : col_count);

    // Capture column names
    for (int c = 0; c < result_out->col_count; c++) {
        const char* name = sqlite3_column_name(stmt, c);
        if (name) {
            strncpy(result_out->col_names[c], name, 63);
            result_out->col_names[c][63] = '\0';
        }
    }

    // Fetch rows — zero-copy pointer into SQLite's own memory
    uint32_t row_idx = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (row_idx >= VITO_SQLITE_MAX_ROWS) break;
        decode_row(stmt, &result_out->rows[row_idx], col_count);
        row_idx++;
    }

    result_out->row_count = row_idx;
    result_out->success   = (rc == SQLITE_DONE || rc == SQLITE_ROW);

    if (!result_out->success) {
        result_out->error_code = rc;
        snprintf(result_out->error_msg, sizeof(result_out->error_msg), "%s", sqlite3_errmsg(db->handle));
    }

    sqlite3_finalize(stmt);
    return result_out->success;
}

vito_sqlite_stmt_t* vito_sqlite_prepare(vito_sqlite_db_t* db, const char* sql) {
    if (!db || !sql) return NULL;
    vito_sqlite_stmt_t* s = (vito_sqlite_stmt_t*)malloc(sizeof(vito_sqlite_stmt_t));
    if (!s) return NULL;
    s->db = db;

    int rc = sqlite3_prepare_v2(db->handle, sql, -1, &s->handle, NULL);
    if (rc != SQLITE_OK) {
        free(s);
        return NULL;
    }
    return s;
}

bool vito_sqlite_bind_text(vito_sqlite_stmt_t* stmt, int idx, const char* val, int len) {
    if (!stmt || !val) return false;
    // SQLITE_TRANSIENT: SQLite makes its own copy
    return sqlite3_bind_text(stmt->handle, idx, val, len, (void(*)(void*))(-1)) == SQLITE_OK;
}

bool vito_sqlite_bind_int(vito_sqlite_stmt_t* stmt, int idx, int64_t val) {
    if (!stmt) return false;
    return sqlite3_bind_int64(stmt->handle, idx, val) == SQLITE_OK;
}

int vito_sqlite_step(vito_sqlite_stmt_t* stmt, vito_sqlite_row_t* row_out) {
    if (!stmt) return -1;
    int rc = sqlite3_step(stmt->handle);
    if (rc == SQLITE_ROW) {
        if (row_out) {
            int col_count = sqlite3_column_count(stmt->handle);
            decode_row(stmt->handle, row_out, col_count);
        }
        return 1;
    }
    if (rc == SQLITE_DONE) return 0;
    return -1;
}

void vito_sqlite_reset(vito_sqlite_stmt_t* stmt) {
    if (stmt && stmt->handle) sqlite3_reset(stmt->handle);
}

void vito_sqlite_finalize(vito_sqlite_stmt_t* stmt) {
    if (!stmt) return;
    if (stmt->handle) sqlite3_finalize(stmt->handle);
    free(stmt);
}

int64_t vito_sqlite_last_insert_rowid(vito_sqlite_db_t* db) {
    if (!db) return -1;
    return sqlite3_last_insert_rowid(db->handle);
}

const char* vito_sqlite_errmsg(vito_sqlite_db_t* db) {
    if (!db) return "null database handle";
    return sqlite3_errmsg(db->handle);
}

// ── Transactions ──────────────────────────────────────────────────────────

bool vito_sqlite_begin(vito_sqlite_db_t* db) {
    return vito_sqlite_exec(db, "BEGIN TRANSACTION;") >= 0;
}

bool vito_sqlite_commit(vito_sqlite_db_t* db) {
    return vito_sqlite_exec(db, "COMMIT;") >= 0;
}

bool vito_sqlite_rollback(vito_sqlite_db_t* db) {
    return vito_sqlite_exec(db, "ROLLBACK;") >= 0;
}

// ── WAL & Maintenance ─────────────────────────────────────────────────────

bool vito_sqlite_enable_wal(vito_sqlite_db_t* db) {
    return vito_sqlite_exec(db, "PRAGMA journal_mode=WAL;") >= 0;
}

vito_sqlite_checkpoint_t vito_sqlite_checkpoint(vito_sqlite_db_t* db) {
    vito_sqlite_checkpoint_t result = {0, 0, false};
    if (!db) return result;
    int log = 0, ckpt = 0;
    int rc = sqlite3_wal_checkpoint_v2(db->handle, NULL, SQLITE_CHECKPOINT_PASSIVE, &log, &ckpt);
    result.log_frames    = log;
    result.checkpointed  = ckpt;
    result.ok            = (rc == SQLITE_OK);
    return result;
}

void vito_sqlite_set_busy_timeout(vito_sqlite_db_t* db, int ms) {
    if (db && db->handle) sqlite3_busy_timeout(db->handle, ms);
}
