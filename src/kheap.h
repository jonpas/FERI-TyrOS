// Interface for kernel heap functions
// Provides a placement 'malloc()' for use before the heap is initialised

#pragma once

#include "common.h"

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
