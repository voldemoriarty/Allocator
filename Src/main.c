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

    void *p = allocate(2);

    dump_chunks(&alloc_chunks, "Alloc");
    dump_chunks(&free_chunks, "Free");

    deallocate(p);

    exit(EXIT_SUCCESS);
}