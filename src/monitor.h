// Defines the interface for monitor

#pragma once

#include "common.h"

// Writes a single character to the monitor
void monitor_put(char c);

// Clears the screen
void monitor_clear();

// Outputs a null-terminated ASCII string to the monitor
void monitor_write(char *str);

// Outputs a hexadecimal number to the monitor
void monitor_write_hex(int n);

// Outputs a decimal number to the monitor
void monitor_write_dec(int n);
