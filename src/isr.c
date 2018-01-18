#include "isr.h"

#ifdef DEBUG_ISR
#include "monitor.h"
#endif

isr_t interrupt_handlers[256];

// Called from ASM interrupt handler stub
void isr_handler(registers_t regs) {
#ifdef DEBUG_ISR
    monitor_write("recieved interrupt: ");
    monitor_write_dec(regs.int_no);
    monitor_put('\n');
#endif

    if (regs.int_no >= IRQ_MASTER_0) {
        if (regs.int_no >= IRQ_SLAVE_0) {
            // Send reset signal to slave
            outb(PIC_SLAVE_CMD, PIC_CMD_RESET);
        }

        // Send reset signal to master
        outb(PIC_MASTER_CMD, PIC_CMD_RESET);
    }

    // Call interrupt handler if one is registered
    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}

void register_interrupt_handler(u8int n, isr_t handler) {
    interrupt_handlers[n] = handler;
}
