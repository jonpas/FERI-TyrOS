#include "common.h"

void outb(u16int port, u8int value) {
    asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port) {
    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

u16int inw(u16int port) {
    u16int ret;
    asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void memcpy(u8int *dest, const u8int *src, u32int len) {
    // TODO Implement
}

void memset(u8int *dest, u8int val, u32int len) {
    // TODO Implement
}

int strcmp(char *str1, char *str2) {
    // TODO Implement
    return 0;
}

char *strcpy(char *dest, const char *src) {
    // TODO Implement
    return dest;
}

char *strcat(char *dest, const char *src) {
    // TODO Implement
    return dest;
}
