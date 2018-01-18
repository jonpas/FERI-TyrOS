#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "keyboard.h"

struct multiboot;

int kernel_main(struct multiboot *mboot_ptr) {
    init_descriptor_tables();
    asm volatile("sti"); // Enable interrupts

    monitor_clear();
    monitor_write("Welcome to Tyr (");
    monitor_write_hex(0x547972);
    monitor_write("), the one-handed OS!\n");

    asm volatile("int $0x3");
    asm volatile("int $0x4");

    init_timer(50);
    init_keyboard();

    return 0;
}
