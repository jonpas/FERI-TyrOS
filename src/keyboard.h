// Interface for keyboard handling

#pragma once

#include "common.h"

//#define DEBUG_KEYBOARD

// Scancodes
#define ENTER_PRESSED       0x1C    // Enter Pressed
#define ENTER_RELEASED      0x9C    // Enter released
#define LSHIFT_PRESSED      0x2A    // Left Shift Pressed
#define LSHIFT_RELEASED     0xAA    // Left Shift Released
#define RSHIFT_PRESSED      0x36    // Right Shift Pressed
#define RSHIFT_RELEASED     0xB6    // Right Shift Released
#define CAPSLOCK_PRESSED    0x3A    // Caps Lock Pressed
#define CAPSLOCK_RELEASED   0xBA    // Caps Lock Released
#define LALT_PRESSED        0x38    // Left Alt Pressed
#define LALT_RELEASED       0xB8    // Left Alt Released
#define RALT_PRESSED        0xE0    // Right Alt Pressed
#define RALT_RELEASED       0xB8    // Right Alt Released
#define CTRL_PRESSED        0x1D    // Control Pressed
#define CTRL_RELEASED       0x9D    // Control Released
#define SCAN_UP             0x48    // Arrow Up
#define SCAN_DOWN           0x50    // Arrow Down
#define SCAN_LEFT           0x4B    // Arrow Left
#define SCAN_RIGHT          0x4D    // Arrow Right
#define F1                  0x3B    // Function 1
#define F2                  0x3C    // Function 2
#define F3                  0x3B    // Function 3
#define F4                  0x3C    // Function 4
#define F5                  0x3C    // Function 5
#define F6                  0x3C    // Function 6
#define F7                  0x3C    // Function 7
#define F8                  0x3C    // Function 8
#define F9                  0x3C    // Function 9
#define F10                 0x3C    // Function 10
#define F11                 0x3C    // Function 11
#define F12                 0x3C    // Function 12

// ASCII codes
#define ESC         27      // Escape
#define BKSP        '\b'    // Backspace
#define TAB         '\t'    // Tab
#define ENT         '\n'    // Enter
#define SQT         39      // Single quote (')
#define DQT         34      // Double quote ("")
#define AUP         '\200'  // Arrow Up
#define ADOW        '\201'  // Arrow Down
#define ALEF        '\202'  // Arrow Left
#define ARIG        '\203'  // Arrow Right

#define INPUT_BUFFER_LEN 256

// Registers keyboard handler
void init_keyboard();
