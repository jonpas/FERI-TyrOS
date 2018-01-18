#include "monitor.h"

struct multiboot;

int kernel_main(struct multiboot *mboot_ptr) {
    monitor_clear();
    monitor_write("Welcome to Tyr (");
    monitor_write_hex(0x547972);
    monitor_write("), the one-handed OS!\n");

    asm volatile("int $0x3");
    //asm volatile("int $0x4");

    return 0;
}
