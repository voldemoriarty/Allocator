#include "allocator.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

extern clist_t alloc_chunks;
extern clist_t free_chunks;
extern clist_t defrag_temp;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    void* p_arr[10];

    p_arr[0] = allocate(1);
    p_arr[1] = allocate(3);
    p_arr[2] = allocate(5);
    p_arr[3] = allocate(1);

    deallocate(p_arr[2]);
    deallocate(p_arr[3]);
    deallocate(p_arr[0]);

    dump_chunks(&alloc_chunks, "Alloc");
    dump_chunks(&free_chunks, "Free");

    defragment();

    dump_chunks(&free_chunks, "Free");

    deallocate(p_arr[1]);

    dump_chunks(&free_chunks, "Free");

    defragment();

    dump_chunks(&free_chunks, "Free");

    exit(EXIT_SUCCESS);
}