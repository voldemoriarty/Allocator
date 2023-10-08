# Allocator

A toy memory allocator suitable for embedded environments. I made this to learn 
about the internals of `malloc` and `free`. 
 - Can allocate arbitrary number of bytes (`allocate(size_t bytes)`)
 - Can deallocate previously allocated memory (`deallocate(void *ptr)`)
 - Can reuse the deallocated memory for reallocation
 - Auto defragmentation when free chunks hit a fixed threshold

## Limitations
 1. Can allocate a fixed maximum number of chunks
 2. Can fail to allocate even if space is available (due to fragmentation, more on this below)
 3. `allocate` is `O(n)` where `n` is number of free chunks + allocated chunks
 4. `deallocate` is `O(n)` without `defragment()` and `O(n logn)` with `defragment()` where n 
    is number of free chunks

### Fragmentation
The free chunks of memory available for allocation are tracked in an array. Every 
time memory is allocated, a suitable free chunk is removed from the list.
Each time memory is deallocated, the chunk is appended to the free list. A consequence 
of this method is that a single contiguous block may appear as multiple smaller chunks.
This can cause allocation to fail if the requested size is greater than the size of individual
chunks even if it is smaller than the total size of the contiguous block. To fix this, an
extra defragmentation step is needed. Defragmentation is a relatively expensive operation
so in my implementation, defragmentation occurs only if number of free chunks are greater
than a fixed threshold. 

## Possible Future Direction
Rethink the deallocation as to avoid fragmenting the free list.