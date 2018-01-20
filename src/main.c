#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "kheap.h"
#include "syscall.h"
#include "task.h"
#include "monitor.h"
#include "keyboard.h"

struct multiboot;

int kmain(struct multiboot *mboot_ptr) {
    // Initialize descriptor tables
    init_descriptor_tables();

    // Enable interrupts (requires descriptor tables)
    asm volatile("sti");

    // Initialize system parts
    init_timer(50);
    init_paging();
    init_syscalls();
    init_monitor();
    init_keyboard();

    // Print welcome message
    monitor_write("Welcome to ");
    monitor_write(OS_NAME);
    monitor_write(", the one-handed OS!\n$ ");

    // TODO Multitasking for proper user mode switching
    //switch_to_user_mode();

    // System Call test
    /*
    syscall_monitor_write("Hello, user world!\n$ ");
    */

    // Paging and Heap test (comment 'init_paging()' above!)
    /*
    uint a = kmalloc(8);
    init_paging();
    uint b = kmalloc(8);
    uint c = kmalloc(8);
    monitor_write("a: ");
    monitor_write_hex(a);
    monitor_write(", b: ");
    monitor_write_hex(b);
    monitor_write("\nc: ");
    monitor_write_hex(c);
    kfree((void *)(uintptr_t)c);
    kfree((void *)(uintptr_t)b);
    uint d = kmalloc(12);
    monitor_write(", d: ");
    monitor_write_hex(d);
    */

    // Panic test
    /*
    uint *ptr = (uint*)0xA0000000;
    uint do_page_fault = *ptr;
    */

    return 0;
}
