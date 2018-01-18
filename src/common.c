#include "common.h"

void outb(ushort port, uchar value) {
    asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

uchar inb(ushort port) {
    uchar ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

ushort inw(ushort port) {
    ushort ret;
    asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void memcpy(uchar *dest, const uchar *src, int len) {
    const uchar *sp = (const uchar *)src;
    uchar *dp = (uchar *)dest;

    while (len != 0) {
        *dp++ = *sp++;
        --len;
    }
}

void memset(void *dest, uchar val, int len) {
    uchar *temp = (uchar *)dest;

    while (len != 0) {
        *temp++ = val;
        --len;
    }
}

int strlen(const char *str) {
    const char *s;
    for (s = str; *s; ++s);
    return s - str;
}

int strcmp(char *str1, char *str2) {
    int failed = 0;
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if(str1[i] != str2[i]) {
            failed = 1;
            break;
        }
        ++i;
    }

    if ((str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0')) {
        failed = 1;
    }

    return failed;
}

char *strcpy(char *dest, const char *src) {
    do {
      *dest++ = *src++;
    } while (*src != 0);

    return dest;
}

char *strcat(char *dest, const char *src) {
    strcpy(dest + strlen(dest), src);
    return dest;
}
