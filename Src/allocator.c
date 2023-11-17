#include "allocator_types.h"
#include <assert.h>
#include <stdbool.h>

#define ALLOC_ASSERT assert

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

static size_t clist_find_ptr(clist_t* list, void* ptr)
{
    size_t i;

    for (i = 0; i < list->size; ++i) {
        if (list->chunks[i].base == ptr)
            break;
    }

    return i;
}

static size_t clist_find_size(clist_t* list, size_t min_size)
{
    size_t i, i_min, c_size;
    
    i_min = 0;
    for (i = 0; i < list->size; ++i) {
        c_size = list->chunks[i].size;
        if (c_size >= min_size && (c_size < list->chunks[i_min].size)) {
            i_min = i;
        }
    }

    if (list->chunks[i_min].size >= min_size) {
        return i_min;
    }
    else {
        return list->size;
    }
}

static size_t clist_sort_from_idx(clist_t* list, size_t i_sort)
{
    size_t i;
    bool   is_first, is_last, forward;

    is_first = i_sort == 0;
    is_last  = i_sort == (list->size - 1);
    forward  = false;
    i = i_sort;

    if (!is_last) {
        for (i = i_sort; i < list->size; ++i) {
            chunk_t next = list->chunks[i + 1];
            chunk_t curr = list->chunks[i];

            if (next.base < curr.base) {
                list->chunks[i + 1] = curr;
                list->chunks[i] = next;
                forward = true;
            }
            else {
                break;
            }
        }
    }

    if (!is_first && !forward) {
        for (i = i_sort; i > 0; --i) {
            chunk_t prev = list->chunks[i - 1];
            chunk_t curr = list->chunks[i];

            if (prev.base > curr.base) {
                list->chunks[i - 1] = curr;
                list->chunks[i] = prev;
            }
            else {
                break;
            }
        }
    }

    return i;
}

static size_t clist_insert(clist_t* list, chunk_t* chunk)
{
    size_t i;

    // make sure we have space
    ALLOC_ASSERT(list->size < MAX_CHUNKS);
    
    // insert at the end of the list
    list->chunks[list->size] = *chunk;

    // re-sort the list
    i = clist_sort_from_idx(list, list->size);
    list->size++;

    return i;
}

static void clist_remove(clist_t* list, size_t i_rem)
{
    size_t i;

    for (i = i_rem + 1; i < list->size; ++i) {
        list->chunks[i - 1] = list->chunks[i];
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

static bool chunks_can_merge_ord(chunk_t* a, chunk_t* b)
{
    return ((a->base + a->size) == b->base);
}

static void clist_merge_cont_blks(clist_t* list, size_t i_insert)
{
    bool     can_merge_front, can_merge_back;
    chunk_t* p_chunk, *p_front, *p_back;
    size_t   i;

    p_chunk = &list->chunks[i_insert];
    p_front = p_chunk + 1;
    p_back  = p_chunk - 1;

    can_merge_front = ((i_insert + 1) < list->size) && chunks_can_merge_ord(p_chunk, p_front);
    can_merge_back = (i_insert > 0) && chunks_can_merge_ord(p_back, p_chunk);

    if (can_merge_front && !can_merge_back) {
        p_chunk->size += p_front->size;
        clist_remove(list, i_insert + 1);
    }
    else if (can_merge_back && !can_merge_front) {
        p_back->size += p_chunk->size;
        clist_remove(list, i_insert);
    }
    else if (can_merge_back && can_merge_front) {
        p_back->size += p_chunk->size + p_front->size;
        for (i = i_insert + 2; i < list->size; ++i) {
            list->chunks[i - 2] = list->chunks[i];
        }
        list->size -= 2;
    }
}

void* allocate(size_t bytes)
{
    size_t  i_free;
    chunk_t to_allocate, free_chunk;

    if (bytes == 0)
        return NULL;

    ALLOC_ASSERT(alloc_chunks.size < MAX_CHUNKS);

    // find a free chunk that fits our size
    i_free = clist_find_size(&free_chunks, bytes);

    // we could not find a chunk big enough
    if (i_free == free_chunks.size) {
        return NULL;
    }

    // we found a chunk big enough
    // take bytes from it and insert it to alloc_chunks
    free_chunk = free_chunks.chunks[i_free];
    to_allocate = (chunk_t) { 
        .base = free_chunk.base, 
        .size = bytes 
    };

    // remove bytes from free_chunk
    // bytes <= free_chunk.size
    // so it will at worst create a zero size chunk 
    free_chunk.size -= bytes;
    free_chunk.base += bytes;

    // insert allocated chunk in list
    alloc_chunks.chunks[alloc_chunks.size++] = to_allocate;

    // update free chunks
    clist_resize_or_remove(&free_chunks, i_free, &free_chunk);
    
    return to_allocate.base;
}

void deallocate(void* ptr)
{
    size_t  i_alloc, i_free;
    chunk_t freed_chunk;

    if (ptr == NULL)
        return;

    // find ptr in allocated list
    i_alloc = clist_find_ptr(&alloc_chunks, ptr);

    // check for illegal deallocation
    ALLOC_ASSERT(i_alloc < alloc_chunks.size);

    // remove from allocated list and add to free list
    freed_chunk = alloc_chunks.chunks[i_alloc];

    clist_remove(&alloc_chunks, i_alloc);
    i_free = clist_insert(&free_chunks, &freed_chunk);

    // merge contiguous blocks to keep heap defragmented
    clist_merge_cont_blks(&free_chunks, i_free);
}
