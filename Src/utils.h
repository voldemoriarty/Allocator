#pragma once

#include "allocator_types.h"
#include <stdio.h>
#include <string.h>


// Thank you stackoverflow: https://stackoverflow.com/a/8488201
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define UNIMPLEMENTED \
    fprintf(stderr, "%s:%d %s() is not implemented.\n", __FILENAME__, __LINE__, __func__); \
    exit(EXIT_FAILURE);

void dump_chunks(clist_t* clist, const char* name);
void dump_free_chunks(heap *h);
void dump_alloc_chunks(heap *h);