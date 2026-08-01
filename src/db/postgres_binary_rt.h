#ifndef VITO_POSTGRES_BINARY_RT_H
#define VITO_POSTGRES_BINARY_RT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Postgres Binary Protocol Message Tags
typedef enum {
    PG_MSG_AUTHENTICATION_OK = 'R',
    PG_MSG_READY_FOR_QUERY  = 'Z',
    PG_MSG_ROW_DESCRIPTION  = 'T',
    PG_MSG_DATA_ROW         = 'D',
    PG_MSG_COMMAND_COMPLETE = 'C',
    PG_MSG_ERROR_RESPONSE   = 'E'
} pg_msg_type_t;

typedef struct {
    uint16_t field_count;
    const uint8_t* val_ptrs[64];
    uint32_t val_lens[64];
} pg_binary_row_t;

// Zero-allocation row decoder from SIMD buffer
bool pg_binary_decode_row(const uint8_t* buffer, size_t len, pg_binary_row_t* out_row);

// Lock-Free MPMC DB Connection Ring Buffer
#define VITO_DB_RING_CAP 128

typedef struct {
    void* connections[VITO_DB_RING_CAP];
    volatile uint32_t head;
    volatile uint32_t tail;
} vito_db_ring_pool_t;

vito_db_ring_pool_t* vito_db_ring_pool_create(void);
bool vito_db_ring_pool_push(vito_db_ring_pool_t* pool, void* conn);
void* vito_db_ring_pool_pop(vito_db_ring_pool_t* pool);
void vito_db_ring_pool_destroy(vito_db_ring_pool_t* pool);

#ifdef __cplusplus
}
#endif

#endif // VITO_POSTGRES_BINARY_RT_H
