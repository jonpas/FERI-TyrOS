#include "isr.h"
#include "monitor.h"

static char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

isr_t interrupt_handlers[256];

// Called from ASM interrupt handler stub
void isr_handler(registers_t regs) {
#ifdef DEBUG_ISR
    monitor_write("recieved interrupt: ");
    monitor_write_dec(regs.int_no);
    monitor_put('\n');
#endif

    // Call interrupt handler if one is registered
    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }

    // Send reset signal in case of IRQ, halt system in case of fault
    if (regs.int_no >= IRQ_MASTER_0) {
        // Send reset signal to slave in case of IRQs 8-15
        if (regs.int_no >= IRQ_SLAVE_0) {
            outb(PIC_SLAVE_CMD, PIC_CMD_RESET);
        }

        // Send reset signal to master
        outb(PIC_MASTER_CMD, PIC_CMD_RESET);
    } else {
        //
        monitor_write(exception_messages[regs.int_no]);
        monitor_write("Exception! System halted!\n");
        while (1); // Halt
    }
}

void register_interrupt_handler(uchar n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void deregister_interrupt_handler(uchar n) {
    interrupt_handlers[n] = 0;
}
