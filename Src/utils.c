#include "utils.h"
#include "allocator.h"

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

void dump_free_chunks(heap *h)
{
    dump_chunks(&h->free_chunks, "Free");
}

void dump_alloc_chunks(heap *h)
{
    dump_chunks(&h->alloc_chunks, "Alloc");
}