#include "task.h"

void switch_to_user_mode() {
    // Set up a stack structure for switching to user mode
    // Critical section, disable interrupts
    // Set registers to user mode data selector
    // Save stack pointer in 'eax' for later reference
    // Push segment selector and wanted value of stack pointer after 'iret'
    // '$1f' - address of the next label '1:', searching forward
    // Upon return, code exeuction shall be at '1:' line with the same stack, in user mode
    asm volatile("  \
        cli; \
        mov $0x23, %ax; \
        mov %ax, %ds; \
        mov %ax, %es; \
        mov %ax, %fs; \
        mov %ax, %gs; \
        mov %esp, %eax; \
        pushl $0x23; \
        pushl %esp; \
        pushf; \
        pushl $0x1B; \
        push $1f; \
        iret; \
        1: \
    ");
}
