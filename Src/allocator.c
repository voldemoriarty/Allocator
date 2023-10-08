#include "allocator.h"
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

clist_t defrag_temp = { 0 };

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

static int defrag_cmp(const void* left, const void* right)
{
    const chunk_t* c_left = left;
    const chunk_t* c_right = right;

    if (c_left->base < c_right->base)
        return -1;
    else if (c_left->base > c_right->base)
        return 1;
    else
        return 0;
}

void* allocate(size_t bytes)
{
    size_t  i_free;
    chunk_t to_allocate, free_chunk;

    if (bytes == 0)
        return NULL;

    assert(alloc_chunks.size < MAX_CHUNKS);

    // find the free chunk that fits our size
    i_free = clist_find_size(&free_chunks, bytes);

    // we could not find a chunk big enough
    if (i_free == free_chunks.size) {
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
    size_t  i_alloc;
    chunk_t freed_chunk;

    if (ptr == NULL)
        return;

    // find ptr in allocated list
    i_alloc = clist_find_ptr(&alloc_chunks, ptr);

    // check for illegal deallocation
    assert(i_alloc < alloc_chunks.size);

    // remove from allocated list and add to free list
    freed_chunk = alloc_chunks.chunks[i_alloc];

    clist_remove(&alloc_chunks, i_alloc);
    clist_insert(&free_chunks, &freed_chunk);
}

void defragment()
{
    size_t i, i_free;

    if (free_chunks.size == 1)
        return;

    defrag_temp.size = free_chunks.size;
    memcpy(defrag_temp.chunks, free_chunks.chunks, free_chunks.size * sizeof(chunk_t));

    qsort(defrag_temp.chunks, defrag_temp.size, sizeof(chunk_t), defrag_cmp);

    free_chunks.chunks[0] = defrag_temp.chunks[0];
    i_free = 0;

    for (i = 1; i < defrag_temp.size; ++i) {
        chunk_t* c_free = &free_chunks.chunks[i_free];
        chunk_t* c_temp = &defrag_temp.chunks[i];

        // check if can merge
        if (c_free->base + c_free->size == c_temp->base) {
            c_free->size += c_temp->size;
        }
        else {
            // add new member
            free_chunks.chunks[++i_free] = *c_temp;
        }
    }

    free_chunks.size = i_free + 1;
}