#include "monitor.h"

// VGA framebuffer starts at 0xB8000
ushort *video_memory = (ushort *)0xB8000;

// Cursor attributes
uchar cursor_attr = 0x0F; // Background black (0), foreground white (15)

// Cursor position storage
uchar cursor_x = 0;
uchar cursor_y = 0;

// Updates the hardware cursor
static void move_cursor() {
    // The screen is 80 characters wide
    ushort cursor_loc = cursor_y * 80 + cursor_x;
    outb(0x3D4, 14);                // Tell the VGA board we are setting the high cursor byte
    outb(0x3D5, cursor_loc >> 8);   // Send the high cursor byte
    outb(0x3D4, 15);                // Tell the VGA board we are setting the low cursor byte
    outb(0x3D5, cursor_loc);        // Send the low cursor byte
}

// Scrolls the text on the screen up by one line
static void scroll() {
    // Get a space character with the default colour attributes
    uchar attributes = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    ushort blank = 0x20 /* space */ | (attributes << 8);

    // Row 25 is the end, this means we need to scroll up
    if(cursor_y >= 25) {
        // Move the current text chunk that makes up the screen back in the buffer by a line
        for (int i = 0*80; i < 24*80; ++i) {
            video_memory[i] = video_memory[i+80];
        }

        // The last line should now be blank, do this by writing 80 spaces to it
        for (int i = 24*80; i < 25*80; ++i) {
            video_memory[i] = blank;
        }
        // Cursor should now be on the last line
        cursor_y = 24;
    }
}

void init_monitor() {
    // If timeout in GRUB is disabled, cursor will also be disabled
    // Enable the cursor either way for consistency
    outb(0x3D4, 0x0A);  // Tell the VGA board to enable high scanline
    outb(0x3D5, 14);    // Send the high scanline byte
    outb(0x3D4, 0x0B);  // Tell the VGA board to enable low scanline
    outb(0x3D5, 15);    // Send the low scanline byte

    monitor_clear();
}

// Copies spaces to framebuffer to clear the monitor
void monitor_clear() {
    // Make an attribute byte for the default colours
    uchar attribute = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    ushort blank = 0x20 /* space */ | (attribute << 8);

    for (int i = 0; i < 80*25; ++i) {
        video_memory[i] = blank;
    }

    // Move the hardware cursor back to the start.
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void monitor_put(char c) {
    // Attribute byte is the top 8 bits of the word we have to send to the VGA board
    ushort attribute = cursor_attr << 8;
    ushort *location;

    if (c == 0x08 && cursor_x) {
        // Handle a backspace, by moving the cursor back one space
        cursor_x--;
    } else if (c == 0x09) {
        // Handle a tab by increasing the cursor's X, but only to a point where it is divisible by 8
        cursor_x = (cursor_x+8) & ~(8-1);
    } else if (c == '\r') {
        // Handle carriage return
        cursor_x = 0;
    } else if (c == '\n') {
        // Handle newline by moving cursor back to left and increasing the row
        cursor_x = 0;
        cursor_y++;
    } else if(c >= ' ') {
        // Handle any other printable character
        location = video_memory + (cursor_y*80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    // Check if we need to insert a new line because we have reached the end of the screen
    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y ++;
    }

    // Scroll the screen if needed
    scroll();
    // Move the hardware cursor
    move_cursor();
}

void monitor_write(char *str) {
    int i = 0;
    while (str[i]) {
        monitor_put(str[i++]);
    }
}

void monitor_write_hex(int n) {
    int tmp;
    monitor_write("0x");

    char no_zeroes = 1;
    for (int i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && no_zeroes != 0) {
            continue;
        }

        no_zeroes = 0;
        if (tmp >= 0xA) {
            monitor_put(tmp - 0xA + 'A');
        } else {
            monitor_put(tmp + '0');
        }
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) {
        monitor_put(tmp - 0xA + 'A');
    } else {
        monitor_put(tmp + '0');
    }
}

void monitor_write_dec(int n) {
    if (n == 0) {
        monitor_put('0');
        return;
    }

    char c[32];

    int acc = n;
    int i = 0;
    while (acc > 0) {
        c[i] = '0' + acc%10;
        acc /= 10;
        ++i;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0) {
        c2[i--] = c[j++];
    }

    monitor_write(c2);
}

void monitor_set_color(uchar fgcolor, uchar bgcolor) {
    // Attribute byte is made up of two nibbles
    // Lower being the foreground colour, and the upper the background colour
    cursor_attr = (bgcolor << 4) | (fgcolor & 0x0F);
}
