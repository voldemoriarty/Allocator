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

static size_t clist_find(clist_t* list, size_t min_size)
{
    size_t i;

    for (i = 0; i < list->size; ++i) {
        if (list->chunks[i].size >= min_size)
            break;
    }

    return i;
}

static void clist_sort_from_idx(clist_t* list, size_t i_sort)
{
    size_t i;

    for (i = i_sort; i > 0; --i) {
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
}

static void clist_insert(clist_t* list, chunk_t* chunk)
{
    // make sure we have space
    assert(list->size < MAX_CHUNKS);
    
    // insert at the end of the list
    list->chunks[list->size] = *chunk;

    // re-sort the list
    clist_sort_from_idx(list, list->size);

    list->size++;
}

static void clist_remove(clist_t* list, size_t i_rem)
{
    size_t i;

    if (i_rem + 1 == list->size) {
        list->size--;
        return;
    }

    for (i = i_rem; i < (list->size - 1); ++i) {
        list->chunks[i] = list->chunks[i + 1];
    }

    list->size--;
}

static void clist_resize_or_remove(clist_t* list, size_t i_rsz, chunk_t* new_chunk)
{
    size_t new_sz;

    new_sz = new_chunk->size;

    if (new_sz == 0) {
        clist_remove(list, i_rsz);
    }
    else {
        list->chunks[i_rsz] = *new_chunk;

        clist_sort_from_idx(list, i_rsz);
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
    i_free = clist_find(&free_chunks, bytes);

    // we could not find a chunk big enough
    if (i_free == free_chunks.size) {
        errno = ENOMEM;
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

    // insert allocated chunk in list and update free_chunks
    clist_insert(&alloc_chunks, &to_allocate);
    clist_resize_or_remove(&free_chunks, i_free, &free_chunk);
    
    return to_allocate.base;
}

void deallocate(void* ptr)
{
    (void)ptr;
    UNIMPLEMENTED;
}
