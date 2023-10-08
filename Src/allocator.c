#include "allocator.h"
#include "utils.h"
#include <assert.h>

uint8_t heap[HEAP_CAP] = { 0 };

clist_t alloc_chunks = {
    .chunks = {0},
    .size = 0
};

clist_t free_chunks = {
    .chunks = {
        [0] = {.base = heap, .size = sizeof heap}
    },
    .size = 1
};

static void clist_insert(clist_t* list, chunk_t* chunk)
{
    size_t i;

    // make sure we have space
    assert(list->size < MAX_CHUNKS);
    
    // insert at the end of the list
    list->chunks[list->size] = *chunk;

    // re-sort the list
    for (i = list->size; i > 0; --i) {
        chunk_t prev = list->chunks[i - 1];
        chunk_t curr = list->chunks[i];

        if (prev.size > curr.size) {
            list->chunks[i - 1] = curr;
            list->chunks[i] = prev;
        }
        else {
            break;
        }
    }

    list->size++;
}

void clist_remove(clist_t* list, size_t i_rem)
{
    size_t i;

    if (i_rem + 1 == list->size) {
        list->size--;
        return;
    }

    for (i = i_rem; i < (list->size - 1); ++i) {
        list->chunks[i] = list->chunks[i + 1];
    }
}

void* allocate(size_t bytes)
{
    size_t  i_free;
    chunk_t to_allocate, free_chunk;

    if (bytes == 0)
        return NULL;

    assert(alloc_chunks.size < MAX_CHUNKS);

    // find the free chunk that fits our size
    for (i_free = 0; i_free < free_chunks.size; ++i_free) {
        if (free_chunks.chunks[i_free].size >= bytes)
            break;
    }

    // we could not find a chunk big enough
    if (i_free == free_chunks.size) {
        errno = -ENOMEM;
        perror("Could not find a chunk big enough");
        return NULL;
    }

    // we found a chunk big enough
    // take bytes from it and insert it to alloc_chunks
    free_chunk = free_chunks.chunks[i_free];
    to_allocate.base = free_chunk.base;
    to_allocate.size = bytes;

    // remove bytes from free_chunk
    // bytes <= free_chunk.size
    // so it will at worst create a zero size chunk 
    free_chunk.size -= bytes;
    free_chunk.base += bytes;

    // insert allocated chunk in list
    clist_insert(&alloc_chunks, &to_allocate);
    clist_remove(&free_chunks, i_free);
    
    if (free_chunk.size > 0) {
        clist_insert(&free_chunks, &free_chunk);
    }

    return to_allocate.base;
}

void deallocate(void* ptr)
{
    (void)ptr;
    UNIMPLEMENTED;
}
