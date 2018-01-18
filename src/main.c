#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "keyboard.h"

struct multiboot;

int kernel_main(struct multiboot *mboot_ptr) {
    // Initialize descriptor tables
    init_descriptor_tables();

    // Enable interrupts (requires descriptor tables)
    asm volatile("sti");

    // Initialize system parts
    init_timer(50);
    init_keyboard();
    init_monitor();

    // Print welcome message
    monitor_write("Welcome to ");
    monitor_write(OS_NAME);
    monitor_write(", the one-handed OS!\n$ ");

    return 0;
}
