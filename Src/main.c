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

    void* p_arr[10];

    for (int i = 0; i < 10; ++i) {
        p_arr[i] = allocate(i);

        if (p_arr[i] == NULL) {
            printf("Size %d allocation failed\n", i);
        }

        if (i % 2 == 0)
            deallocate(p_arr[i]);
    }

    dump_chunks(&alloc_chunks, "Alloc");
    dump_chunks(&free_chunks, "Free");

    printf("=====================\n");
    printf("=====================\n");
    
    for (int i = 0; i < 10; ++i) {
        p_arr[i] = allocate(i);
        
        if (p_arr[i] == NULL) {
            printf("Size %d allocation failed\n", i);
        }
        
        if (i % 2 == 0)
            deallocate(p_arr[i]);
    }

    dump_chunks(&alloc_chunks, "Alloc");
    dump_chunks(&free_chunks, "Free");


    exit(EXIT_SUCCESS);
}