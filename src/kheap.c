#include "kheap.h"

// 'end' is defined in the linker
extern uint end;
uint placement_address = (uint)&end;

// Internal version of kmalloc, allocates a chunk of memory, sz in size
// If align == 1, the chunk must be page-aligned
// If phys != 0, the physical location of the allocated chunk will be stored into phys
static uint kmalloc_internal(uint sz, int align, uint *phys) {
    // Will eventually call 'malloc()' on the kernel heap
    // Always useful before the heap is initialised
    if (align == 1 && (placement_address & 0xFFFFF000)) {
        // Align the placement address
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phys) {
        *phys = placement_address;
    }
    uint tmp = placement_address;
    placement_address += sz;
    return tmp;
}

uint kmalloc_a(uint sz) {
    return kmalloc_internal(sz, 1, 0);
}

uint kmalloc_p(uint sz, uint *phys) {
    return kmalloc_internal(sz, 0, phys);
}

uint kmalloc_ap(uint sz, uint *phys) {
    return kmalloc_internal(sz, 1, phys);
}

uint kmalloc(uint sz) {
    return kmalloc_internal(sz, 0, 0);
}
