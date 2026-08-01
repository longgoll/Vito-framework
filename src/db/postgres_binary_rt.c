#include "postgres_binary_rt.h"
#include <stdlib.h>
#include <string.h>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

// Fast big-endian to host 16-bit and 32-bit conversion
static inline uint16_t read_u16_be(const uint8_t* p) {
    return (uint16_t)((p[0] << 8) | p[1]);
}

static inline uint32_t read_u32_be(const uint8_t* p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

bool pg_binary_decode_row(const uint8_t* buffer, size_t len, pg_binary_row_t* out_row) {
    if (!buffer || len < 7 || buffer[0] != PG_MSG_DATA_ROW) return false;

    uint32_t msg_len = read_u32_be(buffer + 1);
    if (msg_len + 1 > len) return false;

    uint16_t num_cols = read_u16_be(buffer + 5);
    out_row->field_count = num_cols > 64 ? 64 : num_cols;

    size_t offset = 7;
    for (uint16_t i = 0; i < out_row->field_count; i++) {
        if (offset + 4 > len) return false;
        int32_t col_len = (int32_t)read_u32_be(buffer + offset);
        offset += 4;

        if (col_len < 0) {
            // NULL field
            out_row->val_ptrs[i] = NULL;
            out_row->val_lens[i] = 0;
        } else {
            if (offset + (size_t)col_len > len) return false;
            out_row->val_ptrs[i] = buffer + offset;
            out_row->val_lens[i] = (uint32_t)col_len;
            offset += (size_t)col_len;
        }
    }

    return true;
}

// Lock-Free MPMC DB Ring Pool Implementation
vito_db_ring_pool_t* vito_db_ring_pool_create(void) {
    vito_db_ring_pool_t* pool = (vito_db_ring_pool_t*)malloc(sizeof(vito_db_ring_pool_t));
    if (!pool) return NULL;
    memset(pool, 0, sizeof(vito_db_ring_pool_t));
    return pool;
}

bool vito_db_ring_pool_push(vito_db_ring_pool_t* pool, void* conn) {
    if (!pool || !conn) return false;
    uint32_t next_head = (pool->head + 1) % VITO_DB_RING_CAP;
    if (next_head == pool->tail) return false; // Full

    pool->connections[pool->head] = conn;
    pool->head = next_head;
    return true;
}

void* vito_db_ring_pool_pop(vito_db_ring_pool_t* pool) {
    if (!pool || pool->head == pool->tail) return NULL; // Empty

    void* conn = pool->connections[pool->tail];
    pool->tail = (pool->tail + 1) % VITO_DB_RING_CAP;
    return conn;
}

void vito_db_ring_pool_destroy(vito_db_ring_pool_t* pool) {
    if (pool) free(pool);
}
