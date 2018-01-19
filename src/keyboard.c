#include "keyboard.h"
#include "isr.h"
#include "monitor.h"

// Scancode table used to layout a standard US keyboard (KBDUS = Keyboard US)
static uchar kbdus[128][2] = {
    {   0, 0   },
    { ESC, ESC },
    { '1', '!' },
    { '2', '@' },
    { '3', '#' },
    { '4', '$' },
    { '5', '%' },
    { '6', '^' },
    { '7', '&' },
    { '8', '*' },
    { '9', '(' },
    { '0', ')' },
    { '-', '_' },
    { '=', '+' },
    {BKSP, BKSP},
    { TAB, TAB },
    { 'q', 'Q' },
    { 'w', 'W' },
    { 'e', 'E' },
    { 'r', 'R' },
    { 't', 'T' },
    { 'y', 'Y' },
    { 'u', 'U' },
    { 'i', 'I' },
    { 'o', 'O' },
    { 'p', 'P' },
    { '[', '{' },
    { ']', '}' },
    { ENT, ENT },
    {   0, 0   },
    { 'a', 'A' },
    { 's', 'S' },
    { 'd', 'D' },
    { 'f', 'F' },
    { 'g', 'G' },
    { 'h', 'H' },
    { 'j', 'J' },
    { 'k', 'K' },
    { 'l', 'L' },
    { ';', ':' },
    { SQT, DQT },
    { '`', '~' },
    {   0, 0   },
    { '\\' ,'|'},
    { 'z', 'Z' },
    { 'x', 'X' },
    { 'c', 'C' },
    { 'v', 'V' },
    { 'b', 'B' },
    { 'n', 'N' },
    { 'm', 'M' },
    { ',', '<' },
    { '.', '>' },
    { '/', '?' },
    {   0, 0   },
    { '*', '*' },
    {   0, 0   },
    { ' ', ' ' },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    { AUP, AUP },
    {   0, 0   },
    { '-', '-' },
    {ALEF, ALEF},
    {   0, 0   },
    {ARIG, ARIG},
    { '+', '+' },
    {   0, 0   },
    {ADOW, ADOW},
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
    {   0, 0   },
};

static uchar shift = 0;
static uchar lalt = 0;
static uchar ralt = 0;
static uchar ctrl = 0;
static uchar capslock = 0;
static char input_buffer[INPUT_BUFFER_LEN] = {0};

static void keyboard_handler(registers_t *regs) {
    // Read from keyboard's data buffer
    uchar scancode = inb(0x60);

#ifdef DEBUG_KEYBOARD
    monitor_write_hex(scancode);
    monitor_put(' ');
#endif

    // Top bit of the byte is set, meaning a key has just been released
    if (scancode & 0x80) {
        // Released key
        if (scancode == LSHIFT_RELEASED || scancode == RSHIFT_RELEASED) {
            shift = 0;
        } else if (scancode == LALT_RELEASED) {
            lalt = 0;
        } else if (scancode == RALT_RELEASED) {
            ralt = 0;
        } else if (scancode == CTRL_RELEASED) {
            ctrl = 0;
        }
    } else {
        // Pressed key
        if (scancode == LSHIFT_PRESSED || scancode == RSHIFT_PRESSED) {
            shift = 1;
        } else if (scancode == CAPSLOCK_PRESSED) {
            capslock = !capslock;
        } else if (scancode == LALT_PRESSED) {
            lalt = 1;
        } else if (scancode == RALT_PRESSED) {
            ralt = 1;
        } else if (scancode == CTRL_PRESSED) {
            ctrl = 1;
        } else {
            uchar ascii = kbdus[scancode][shift|capslock];
            monitor_put(ascii);

            // Interpret current input
            if (ascii == ENT) {
                // Interpret commands
                if (strcmp(input_buffer, "sysinfo") == 0) {
                    monitor_write("OS: ");
                    monitor_write(OS_NAME);
                    monitor_write(" (");
                    monitor_write_hex(0x547972);
                    monitor_write(")\nArch: x86\n");
                } else if (strcmp(input_buffer, "clear") == 0) {
                    monitor_clear();
                }
                monitor_write("$ ");

                // Clear buffer for next input
                memset(input_buffer, 0, sizeof(input_buffer));
            } else if (ascii == BKSP) {
                // Try to delete last inputted character
                if (monitor_delete() != -1) {
                    // Delete last character from input buffer
                    char tmp[INPUT_BUFFER_LEN] = {0};
                    memcpy(tmp, input_buffer, strlen(input_buffer) - 1);
                    memset(input_buffer, 0, sizeof(input_buffer));
                    strcpy(input_buffer, tmp);
                }
            } else {
                // Buffer ASCII codes for command interpetation on Enter
                char asciistr[2] = {ascii, '\0'};
                strcat(input_buffer, asciistr);
            }
        }
    }

#ifdef DEBUG_KEYBOARD
    monitor_put('\n');
#endif
}

void init_keyboard() {
    // Register keyboard handler
    register_interrupt_handler(IRQ1, &keyboard_handler);
}
