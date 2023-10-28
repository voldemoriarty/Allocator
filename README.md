# Allocator

A toy memory allocator suitable for embedded environments which can also reuse freed 
memory. I made this to learn about the internals of `malloc` and `free`. 
 - Can allocate arbitrary number of bytes instead of fixed chunks
 - Can reuse the deallocated memory for reallocation
 - Standalone. Uses a statically allocated array as the memory chunk and can work without stdlib
 - Is suitable for managing tightly coupled memory in embedded devices

## Usage
 - Copy the following files to your project
    - `allocator.c`
    - `allocator.h`
    - `allocator_types.h`

   By default, a static buffer is used as heap. If you want to change it, 
     - In `allocator.c`, change the line 
        ```C
        uint8_t heap[HEAP_CAP] = { 0 };
        ```
        Make `heap` point to region of memory you wish to use as heap
     - In `allocator.h` update `HEAP_CAP` to size of memory you want to use as heap
     - In `allocator.h` update `MAX_CHUNKS` according to your need

## Limitations
 1. Can allocate a fixed maximum number of chunks. Enough space for tracking these chunks have to be statically allocated.
 2. `allocate` is `O(n)` where `n` is number of free chunks
 3. `deallocate` is `O(n)` where `n` is number of allocated chunks + number of free chunks

### Defragmentation
The free chunks of memory available for allocation are tracked in an array sorted by 
address. Every time memory is allocated, a suitable free chunk is removed from the list.
Each time memory is deallocated, the chunk is appended to the free list. A consequence 
of this method is that a single contiguous block may appear as multiple smaller chunks.
To remedy this, an extra defragmentation step is applied on each deallocation. Since 
free chunks are sorted by address, it is trivial to merge any contiguous blocks as memory
is freed. If the freed memory can be merged with any existing free chunk, it is merged.
Hence the free list is never fragmented. 