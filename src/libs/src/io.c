#include "../inc/io.h"

unsigned int x = 0;

void putc(const unsigned char c) {
    unsigned short *video_memory = (unsigned short*)0xB8000;

    video_memory[x++] = (c | 0x0700);
    if (x >= 80 * 25) {
        x = 0;
    }
    return;
}

void puts(const unsigned char *s) {
    if (*s == '\0') putc('!');
    while(*s != '\0') {
        putc(*s);
        ++s;
    }
    return;
}