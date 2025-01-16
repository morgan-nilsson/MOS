#include "../inc/io.h"

void putc(const unsigned char c, const int x, const int y) {
    unsigned short *video_memory = (unsigned short*)0xB8000;
    
    unsigned short offset = (y * VGA_WIDTH + x);
    
    video_memory[offset] = (c | 0x0700);
}


void puts(const unsigned char *s) {
    unsigned short *video_memory = (unsigned short*)0xB8000;

    unsigned int i = 0;
    while(s[i] != '\0') {
        video_memory[i] = (s[i]);
    }
}