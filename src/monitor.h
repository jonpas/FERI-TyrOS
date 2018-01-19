// Interface for monitor

#pragma once

#include "common.h"

// Enables the cursor and clears the screen
void init_monitor();

// Clears the screen
void monitor_clear();

// Writes a single character to the monitor
void monitor_put(const char c);

// Outputs a null-terminated ASCII string to the monitor
void monitor_write(const char *str);

// Outputs a hexadecimal number to the monitor
void monitor_write_hex(int n);

// Outputs a decimal number to the monitor
void monitor_write_dec(int n);

// Deletes a character from the monitor, preventing deletion of first input line characters ('$ ')
// Returns -1 on blocked deletion, 0 otherwise
int monitor_delete();

// Sets the foreground color and background color
void monitor_set_color(uchar fgcolor, uchar bgcolor);
