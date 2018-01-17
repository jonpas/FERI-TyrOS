// Defines typedefs and some global common functions

#pragma once

// typedefs to standardise sizes across platforms, written for 32-bit x86
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

// Writes a byte out to the specified port
void outb(u16int port, u8int value);

// Reads a byte from the specified port
u8int inb(u16int port);

// Reads a word from the specified port
u16int inw(u16int port);

// Copies len bytes from src to dest
void memcpy(u8int *dest, const u8int *src, u32int len);

// Writes len copies of val into dest
void memset(u8int *dest, u8int val, u32int len);

// Compares two strings
// Returns -1 if str1 < str2, 0 if they are equal or 1 otherwise
int strcmp(char *str1, char *str2);

// Copies the NULL-terminated string src into dest
// Returns dest
char *strcpy(char *dest, const char *src);

// Concatenates the NULL-terminated string src onto the end of dest
// Returns dest
char *strcat(char *dest, const char *src);

