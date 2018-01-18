#include "isr.h"
#include "monitor.h"

// Called from ASM interrupt handler stub
void isr_handler(registers_t regs) {
    monitor_write("recieved interrupt: ");
    monitor_write_dec(regs.int_no);
    monitor_put('\n');
}
