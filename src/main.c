#include "monitor.h"

struct multiboot;

int main(struct multiboot *mboot_ptr) {
    monitor_clear();
    monitor_write("Welcome to Tyr, the one-handed OS!");

    return 0;
}
