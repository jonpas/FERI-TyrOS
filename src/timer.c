#include "timer.h"
#include "isr.h"

#ifdef DEBUG_TIMER
#include "monitor.h"
#endif

int tick = 0;

static void timer_handler(registers_t regs) {
    tick++;

#ifdef DEBUG_TIMER
    monitor_write("Tick: ");
    monitor_write_dec(tick);
    monitor_write("\n");
#endif
}

void init_timer(int frequency) {
    // Register timer handler
    register_interrupt_handler(IRQ0, &timer_handler);

    // Calue we send to PIT is the value to divide its input clock (1193180 Hz) by, to get our required frequency
    // The divisor must be small enough to fit into 16-bits.
    int divisor = 1193180 * frequency;
    uchar l = (uchar)(divisor & 0xFF);
    uchar h = (uchar)((divisor>>8) & 0xFF);

    // Send command byte
    outb(0x43, 0x36);

    // Send frequency divisor
    outb(0x40, l);
    outb(0x40, h);
}

void timer_wait(int ticks) {
    ulong eticks;
    eticks = tick + ticks;
    while(tick < eticks);
}
