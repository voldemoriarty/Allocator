#include "allocator_types.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

extern clist_t alloc_chunks;
extern clist_t free_chunks;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    void* p_arr[10] = { 0 };

    p_arr[0] = allocate(42);
    p_arr[1] = allocate(3);
    p_arr[2] = allocate(5);
    p_arr[3] = allocate(1);
    p_arr[4] = allocate(69);
    p_arr[5] = allocate(69);

    deallocate(p_arr[0]);
    deallocate(p_arr[2]);
    deallocate(p_arr[4]);

    dump_free_chunks();
    dump_alloc_chunks();

    p_arr[6] = allocate(1);
    dump_free_chunks();

    deallocate(p_arr[1]);
    deallocate(p_arr[3]);
    deallocate(p_arr[5]);
    deallocate(p_arr[6]);

    assert(free_chunks.size == 1);
    assert(free_chunks.chunks[0].size == HEAP_CAP);

    exit(EXIT_SUCCESS);
}