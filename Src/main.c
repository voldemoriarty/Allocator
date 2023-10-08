#include "allocator.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

extern clist_t alloc_chunks;
extern clist_t free_chunks;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    for (int i = 0; i < 3; ++i)
        allocate(3 + i);

    dump_chunks(&alloc_chunks, "Alloc");
    dump_chunks(&free_chunks, "Free");

    deallocate(NULL);

    exit(EXIT_SUCCESS);
}