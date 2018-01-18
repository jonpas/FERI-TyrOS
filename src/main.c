#include "monitor.h"

struct multiboot;

int main(struct multiboot *mboot_ptr) {
    monitor_clear();
    monitor_write("Welcome to Tyr (");
    monitor_write_hex(0x547972);
    monitor_write("), the one-handed OS!");

    return 0;
}
