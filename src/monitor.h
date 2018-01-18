// Defines the interface for monitor

#pragma once

#include "common.h"

// Clears the screen
void monitor_clear();

// Writes a single character to the monitor
void monitor_put(char c);

// Outputs a null-terminated ASCII string to the monitor
void monitor_write(char *str);

// Outputs a hexadecimal number to the monitor
void monitor_write_hex(int n);

// Outputs a decimal number to the monitor
void monitor_write_dec(int n);

// Sets the foreground color and background color
void monitor_set_color(uchar fgcolor, uchar bgcolor);
