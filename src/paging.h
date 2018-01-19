// Interface and structures for paging

#pragma once

#include "common.h"
#include "isr.h"
#include "monitor.h"

// Macros for bitset algorithms
#define INDEX_FROM_BIT(a) (a / (8 * 4))
#define OFFSET_FROM_BIT(a) (a % (8 * 4))

typedef struct page {
    uint present  : 1;    // Page present in memory
    uint rw       : 1;    // Read-only if clear, readwrite if set
    uint user     : 1;    // Supervisor level only if clear
    uint accessed : 1;    // Has the page been accessed since last refresh
    uint dirty    : 1;    // Has the page been written to since last refresh
    uint unused   : 7;    // Amalgamation of unused and reserved bits
    uint frame    : 20;   // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table {
    page_t pages[1024];
} page_table_t;

typedef struct page_directory {
    // Array of pointers to pagetables
    page_table_t *tables[1024];

    // Array of pointers to the pagetables above, but gives their *physical* location, for loading into the CR3 register
    uint tables_physical[1024];

    // Physical address of 'tables_physical'
    // When kernel heap gets allocated and the directory may be in a different location in virtual memory
    uint physical_addr;
} __attribute__((aligned(4096))) page_directory_t;

// Allocates a frame
void alloc_frame(page_t *page, int is_kernel, int is_writeable);

// Deallocates a frame
void free_frame(page_t *page);

// Sets up the environment, page directories and enables paging
void init_paging();

// Causes the specified page directory to be loaded into the CR3 register
void switch_page_directory(page_directory_t *new);

// Retrieves a pointer to the page required
// If make == 1 and page-table in which this page should reside isn't created, create it
page_t *get_page(uint address, int make, page_directory_t *dir);

// Panic macros and functions
#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char *message, const char *file, uint line);
extern void panic_assert(const char *file, uint line, const char *desc);
