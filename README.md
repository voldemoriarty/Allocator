# Allocator

A toy memory allocator suitable for embedded environments which can also reuse freed 
memory. I made this to learn about the internals of `malloc` and `free`. 
 - Can allocate arbitrary number of bytes instead of fixed chunks
 - Can reuse the deallocated memory for reallocation

## Limitations
 1. Can allocate a fixed maximum number of chunks
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