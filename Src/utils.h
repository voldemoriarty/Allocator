#pragma once

#include "allocator.h"
#include <stdio.h>
#include <string.h>


// Thank you stackoverflow: https://stackoverflow.com/a/8488201
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define UNIMPLEMENTED \
    fprintf(stderr, "%s:%d %s() is not implemented.\n", __FILENAME__, __LINE__, __func__); \
    exit(EXIT_FAILURE);


void dump_chunks(clist_t* chunks, const char *name);