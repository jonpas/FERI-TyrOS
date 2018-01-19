// Interface for kernel heap functions
// Additionally provides a placement 'malloc()' for use before the heap is initialised

#pragma once

#include "common.h"
#include "ordered_array.h"

#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000

#define HEAP_INDEX_SIZE 0x20000
#define HEAP_MAGIC      0x123890AB
#define HEAP_MIN_SIZE   0x70000

// Size information for a hole/block
typedef struct {
    uint magic;     // Magic number, used for error checking and identification
    uchar is_hole;  // 1 if hole, 0 if block
    uint size;      // Size of the block, including the end footer
} header_t;

typedef struct {
    uint magic;         // Magic number, same as in 'header_t'
    header_t *header;   // Pointer to the block header
} footer_t;

typedef struct {
    ordered_array_t index;
    uint start_address;     // Start of allocated space
    uint end_address;       // End of allocated space (may be expanded up to 'max_address')
    uint max_address;       // Maximum address the heap can be expanded to
    uchar supervisor;       // Should extra pages requested by us be mapped as supervisor-only
    uchar readonly;         // Should extra pages requested by us be mapped as read-only
} heap_t;

// Creates a new heap
heap_t *create_heap(uint start, uint end, uint max, uchar supervisor, uchar readonly);

// Allocates a contiguous region of memory size in size
// If page_align == 1, it creates that block starting on a page boundary
void *alloc(uint size, uchar page_align, heap_t *heap);

// Releases a block allocated with 'alloc'
void free(void *p, heap_t *heap);


// Allocates a chunk of memory, sz in size
// The chunk must be page aligned
uint kmalloc_a(uint sz);

// Allocates a chunk of memory, sz in size
// Returns physical address in phys, phys MUST be a valid pointer to 'uint'!
uint kmalloc_p(uint sz, uint *phys);

// Allocates a chunk of memory, sz in size, must be page-aligned
// Returns physical address in phys
uint kmalloc_ap(uint sz, uint *phys);

// General allocation
uint kmalloc(uint sz);

// General deallocation
void kfree(void *p);
