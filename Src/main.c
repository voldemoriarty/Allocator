#include "allocator.h"
#include "allocator_types.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define HEAP_CAP (300)

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    
    uint8_t mem[HEAP_CAP];
    heap _heap = {0};
    init_heap(&_heap, mem, HEAP_CAP);

    void* p_arr[10] = { 0 };

    p_arr[0] = allocate(&_heap, 42);
    p_arr[1] = allocate(&_heap, 3);
    p_arr[2] = allocate(&_heap, 5);
    p_arr[3] = allocate(&_heap, 1);
    p_arr[4] = allocate(&_heap, 69);
    p_arr[5] = allocate(&_heap, 69);

    deallocate(&_heap, p_arr[0]);
    deallocate(&_heap, p_arr[2]);
    deallocate(&_heap, p_arr[4]);

    dump_free_chunks(&_heap);
    dump_alloc_chunks(&_heap);

    p_arr[6] = allocate(&_heap, 1);
    dump_free_chunks(&_heap);

    deallocate(&_heap, p_arr[1]);
    deallocate(&_heap, p_arr[3]);
    deallocate(&_heap, p_arr[5]);
    deallocate(&_heap, p_arr[6]);

    assert(_heap.free_chunks.size == 1);
    assert(_heap.free_chunks.chunks[0].size == HEAP_CAP);

    exit(EXIT_SUCCESS);
}