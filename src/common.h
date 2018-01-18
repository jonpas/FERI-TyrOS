// Defines typedefs and some global common functions

#pragma once

// PIC constants
#define IRQ_MASTER_0    31
#define IRQ_SLAVE_0     40
#define PIC_MASTER_CMD  0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD   0xA0
#define PIC_SLAVE_DATA  0xA1
#define PIC_CMD_RESET   0x20

// typedefs to standardise sizes across platforms, written for 32-bit x86
typedef unsigned long   ulong;
typedef unsigned int    uint;
typedef unsigned short  ushort;
typedef unsigned char   uchar;

// Writes a byte out to the specified port
void outb(ushort port, uchar value);

// Reads a byte from the specified port
uchar inb(ushort port);

// Reads a word from the specified port
ushort inw(ushort port);

// Copies len bytes from src to dest
void memcpy(uchar *dest, const uchar *src, int len);

// Writes len copies of val into dest
void memset(void *dest, uchar val, int len);

// Returns length of str
int strlen(const char *str);

// Compares two strings
// Returns -1 if str1 < str2, 0 if they are equal or 1 otherwise
int strcmp(char *str1, char *str2);

// Copies the NULL-terminated string src into dest
// Returns dest
char *strcpy(char *dest, const char *src);

// Concatenates the NULL-terminated string src onto the end of dest
// Returns dest
char *strcat(char *dest, const char *src);
