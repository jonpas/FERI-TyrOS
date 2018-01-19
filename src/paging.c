#include "paging.h"
#include "kheap.h"

page_directory_t *kernel_directory = 0; // Kernel's page directory
page_directory_t *current_directory = 0; // Current page directory

// Bitset of frames (used or free)
uint *frames;
uint nframes;

// Defined in kheap.c
extern uint placement_address;

static void paging_handler(registers_t regs) {
    // Page fault has occurred
    // Read faulting address from CR2 register
    uint faulting_address;
    asm("mov %%cr2, %0" : "=r" (faulting_address));

    // Get error details
    int present     = !(regs.err_code & 0x1);   // Page not present
    int rw          = regs.err_code & 0x2;      // Write operation
    int us          = regs.err_code & 0x4;      // Processor was in user-mode
    int reserved    = regs.err_code & 0x8;      // Overwritten CPU-reserved bits of page entry
    //int id          = regs.err_code & 0x10;     // Caused by an instruction fetch

    // Print an error message
    monitor_write("Page fault! ( ");
    if (present) {
        monitor_write("present ");
    }
    if (rw) {
        monitor_write("read-only ");
    }
    if (us) {
        monitor_write("user-mode ");
    }
    if (reserved) {
        monitor_write("reserved ");
    }
    monitor_write(") at ");
    monitor_write_hex(faulting_address);
    monitor_write("\n");
    PANIC("Page fault");
}

// Sets a bit in the frame's bitset
static void set_frame(uint frame_addr) {
    uint frame = frame_addr / 0x1000;
    uint idx = INDEX_FROM_BIT(frame);
    uint off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

// Clears a bit in the frame's bitset
static void clear_frame(uint frame_addr) {
    uint frame = frame_addr / 0x1000;
    uint idx = INDEX_FROM_BIT(frame);
    uint off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

// Tests if a bit is set
static uint UNUSED_FUNCTION(test_frame)(uint frame_addr) {
    uint frame = frame_addr / 0x1000;
    uint idx = INDEX_FROM_BIT(frame);
    uint off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}

// Finds the first free frame
// Returns -1 on failure to find, frame otherwise
static uint first_frame() {
    for (uint i = 0; i < INDEX_FROM_BIT(nframes); i++) {
        // Exit early if nothing free
        if (frames[i] != 0xFFFFFFFF) {
            // At least one bit is free in current frame
            for (uint j = 0; j < 32; j++) {
                uint toTest = 0x1 << j;
                if (!(frames[i]&toTest)) {
                    return i * 4 * 8 + j;
                }
            }
        }
    }

    return -1;
}

void alloc_frame(page_t *page, int is_kernel, int is_writeable) {
    if (page->frame != 0) {
        return;
    } else {
        uint idx = first_frame();
        if (idx == (uint)-1) {
            PANIC("No free frames!");
        }
        set_frame(idx*0x1000);
        page->present = 1;
        page->rw = (is_writeable) ? 1 : 0;
        page->user = (is_kernel) ? 0 : 1;
        page->frame = idx;
    }
}

void free_frame(page_t *page) {
    uint frame;
    if (!(frame=page->frame)) {
        return;
    } else {
        clear_frame(frame);
        page->frame = 0x0;
    }
}

void init_paging() {
    // Size of physical memory (for now assume it huge, because we get a reboot otherwise)
    uint mem_end_page = 0x80000000;

    nframes = mem_end_page / 0x1000;
    frames = (uint*)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));

    // Make a page directory
    kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    memset(kernel_directory, 0, sizeof(page_directory_t));
    current_directory = kernel_directory;

    // Identity map (phys addr = virt addr) from 0x0 to the end of used memory, for transparent access, as if paging wasn't enabled
    // Using a while loop deliberately, inside the loop body placement_address is changed by calling 'kmalloc()',
    // a while loop causes this to be computed on-the-fly rather than once at start
    int i = 0;
    while (i < placement_address) {
        // Kernel code is readable but not writeable from user-space
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }

    // Register page fault handler
    register_interrupt_handler(14, paging_handler);

    // Enable paging
    switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t *dir) {
    current_directory = dir;

    // Load physical page directory into CR3
    asm volatile("mov %0, %%cr3":: "r" (&dir->tables_physical));

    // Enable 4 MB paging by setting PSE bit in CR4
    uint cr4;
    asm volatile("mov %%cr4, %0": "=r"(cr4));
    cr4 |= 0x00000010;
    cr4 &= 0xffffffef;
    asm volatile("mov %0, %%cr4":: "r"(cr4));

    // Enable paging by setting PG bit in CR0
    uint cr0;
    asm volatile("mov %%cr0, %0": "=r" (cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0":: "r" (cr0));
}

page_t *get_page(uint address, int make, page_directory_t *dir) {
    // Turn the address into an index
    address /= 0x1000;

    // Find the page table containing this address
    uint table_idx = address / 1024;
    if (dir->tables[table_idx]) {
        // If this table is already assigned
        return &dir->tables[table_idx]->pages[address % 1024];
    } else if (make) {
        uint tmp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
        dir->tables_physical[table_idx] = tmp | 0x7; // Present, rw, us
        return &dir->tables[table_idx]->pages[address % 1024];
    } else {
        return 0;
    }
}

extern void panic(const char *message, const char *file, uint line) {
    // Encountered a massive problem and have to stop, disable interrupts
    asm volatile("cli");

    monitor_write("PANIC(");
    monitor_write(message);
    monitor_write(") at ");
    monitor_write(file);
    monitor_write(":");
    monitor_write_dec(line);
    monitor_write("\n");

    while (1); // Halt
}

extern void panic_assert(const char *file, uint line, const char *desc) {
    // Assertion failed, disable interrupts
    asm volatile("cli");

    monitor_write("ASSERTION-FAILED(");
    monitor_write(desc);
    monitor_write(") at ");
    monitor_write(file);
    monitor_write(":");
    monitor_write_dec(line);
    monitor_write("\n");

    while (1); // Halt
}
