#pragma once

#define HEAP_CAP     (2000) // heap capacity in bytes
#define MAX_CHUNKS   (100)  // max number of chunks

void *allocate(size_t bytes);
void  deallocate(void* ptr);