#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

#define HEAP_CAP     (2000) // heap capacity in bytes
#define MAX_CHUNKS   (100)  // max number of chunks
#define DEFRAG_THRES (50)

typedef struct {
    uint8_t* base;
    size_t   size;
} chunk_t;

typedef struct {
    chunk_t chunks[MAX_CHUNKS];
    size_t  size;
} clist_t;

void *allocate(size_t bytes);
void  deallocate(void* ptr);