// Interface and structures for high level interrupt service routines

#include "common.h"

typedef struct registers {
    // Data segment selector
    u32int ds;

    // Pushed by pusha
    // 'useless_value' instead of 'esp', because it has to do with the current stack context, not what was interrupted (proper 'esp' below)
    u32int edi, esi, ebp, useless_value, ebx, edx, ecx, eax;

    // Interrupt number and error code (if applicable)
    u32int int_no, err_code;

    // Pushed by the processor automatically
    // Proper 'esp' here
    u32int eip, cs, eflags, esp, ss;
} registers_t;
