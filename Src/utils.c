#include "utils.h"
#include "allocator.h"

extern clist_t alloc_chunks;
extern clist_t free_chunks;

void dump_chunks(clist_t* clist, const char *name)
{
    printf("List: %s. Size = %zu\n", name, clist->size);
    printf("============\n");
    for (size_t i = 0; i < clist->size; ++i) {
        chunk_t chunk = clist->chunks[i];
        printf("\t%zu: %p; size = %zu\n", i, chunk.base, chunk.size);
    }
    printf("\n");
}

void dump_free_chunks()
{
    dump_chunks(&free_chunks, "Free");
}

void dump_alloc_chunks()
{
    dump_chunks(&alloc_chunks, "Alloc");
}