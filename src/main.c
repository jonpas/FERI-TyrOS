#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "keyboard.h"

struct multiboot;

int kernel_main(struct multiboot *mboot_ptr) {
    init_descriptor_tables();
    asm volatile("sti"); // Enable interrupts
    init_timer(50);
    init_keyboard();

    monitor_clear();
    monitor_write("Welcome to Tyr (");
    monitor_write_hex(0x547972);
    monitor_write("), the one-handed OS!\n");


    return 0;
}
