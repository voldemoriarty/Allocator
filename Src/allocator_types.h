#pragma once

#include "allocator.h"
#include <stdint.h>

typedef struct {
    uint8_t* base;
    size_t   size;
} chunk_t;

typedef struct {
    chunk_t chunks[MAX_CHUNKS];
    size_t  size;
} clist_t;
