#include "../inc/vga_driver.h"

unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
    return;
}

void set_cursor(int offset) {
    offset = offset / 2;
    port_byte_out(VGA_CTRL_REG, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REG, (unsigned char) (offset >> 8));
    port_byte_out(VGA_CTRL_REG, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REG, (unsigned char) (offset & 0xff));
}

int get_cursor(void) {
    port_byte_out(VGA_CTRL_REG, VGA_OFFSET_HIGH);
    int offset = port_byte_in(VGA_DATA_REG) << 8;
    port_byte_out(VGA_CTRL_REG, VGA_OFFSET_LOW);
    offset += port_byte_in(VGA_DATA_REG);
    return offset * 2;
}

void set_char_at_video_memory(char c, int offset) {
    unsigned char* vidmem = (unsigned char*)VGA_ADDRESS;
    vidmem[offset] = c;
    vidmem[offset + 1] = WHITE_ON_BLACK;
    return;
}

void write_char(char c) {
    int offset = get_cursor();
    set_char_at_video_memory(c, offset);
    set_cursor(offset + 2);
    return;
}

int get_row_from_offset(int offset) {
    return offset / (2 * VGA_MAX_COLUMNS);
}

int get_offset(int column, int row) {
    return 2 * (row * VGA_MAX_COLUMNS + column);
}

int get_newline_offset(int offset) {
    get_offset(0, get_row_from_offset(offset) + 1);
}

void write_string(const char *s) {
    int offset = get_cursor();
    int index = 0;
    while (s[index] != '\0') {
        if (s[index] == '\n') {
            offset = get_newline_offset(offset);
        } else {
            set_char_at_video_memory(s[index], offset);
            offset += 2;
        }
        ++index;
    }
    set_cursor(offset);
    int i = 0xff;
    return;
}
