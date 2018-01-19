#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "syscall.h"
#include "task.h"
#include "monitor.h"
#include "keyboard.h"

struct multiboot;

int kernel_main(struct multiboot *mboot_ptr) {
    // Initialize descriptor tables
    init_descriptor_tables();

    // Enable interrupts (requires descriptor tables)
    asm volatile("sti");

    // Initialize system parts
    init_timer(50);
    //init_paging(); // TODO Fix paging address (implement heap?)
    init_syscalls();
    init_monitor();
    init_keyboard();

    // Print welcome message
    monitor_write("Welcome to ");
    monitor_write(OS_NAME);
    monitor_write(", the one-handed OS!\n$ ");

    // TODO Multitasking
    //switch_to_user_mode();
    //syscall_monitor_write("Hello, user world!\n");

    // Page fault test
    /*uint *ptr = (uint*)0xA0000000;
    uint do_page_fault = *ptr;
    */

    return 0;
}
