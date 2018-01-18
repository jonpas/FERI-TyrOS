// Interface and structures for high level interrupt service routines

#pragma once

#include "common.h"

//#define DEBUG_ISR

// IRQ constants
#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef struct registers {
    // Data segment selector
    int ds;

    // Pushed by pusha
    // 'useless_value' instead of 'esp', because it has to do with the current stack context, not what was interrupted (proper 'esp' below)
    int edi, esi, ebp, useless_value, ebx, edx, ecx, eax;

    // Interrupt number and error code (if applicable)
    int int_no, err_code;

    // Pushed by the processor automatically
    // Proper 'esp' here
    int eip, cs, eflags, esp, ss;
} registers_t;

typedef void (*isr_t)(registers_t);

// Registers a handler for interrupts or IRQs
void register_interrupt_handler(uchar n, isr_t handler);

// Deregisters a handler for interrupts or IRQs
void deregister_interrupt_handler(uchar n);
