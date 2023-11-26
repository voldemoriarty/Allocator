#pragma once

#include <stddef.h>
#include <stdint.h>

#ifndef MAX_CHUNKS
#define MAX_CHUNKS   (100)  // max number of chunks
#endif 

typedef struct {
    uint8_t* base;
    size_t   size;
} chunk_t;

typedef struct {
    chunk_t chunks[MAX_CHUNKS];
    size_t  size;
} clist_t;

typedef struct 
{
    uint8_t *bytes;
    clist_t alloc_chunks;    
    clist_t free_chunks;
} heap;

#ifdef __cplusplus
extern "C" {
#endif

void init_heap(heap *h, uint8_t *heap_base, size_t heap_size);
void *allocate(heap *h, size_t bytes);
void  deallocate(heap *h, void* ptr);

#ifdef __cplusplus
}
#endif