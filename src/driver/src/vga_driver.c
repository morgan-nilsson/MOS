// Copyright 2025 Morgan Nilsson

#include "../include/vga_driver.h"
#include "../../libs/include/stdio.h"
#include "../../libs/include/string.h"
#include "../../libs/include/stdlib.h"

/**
 * Set the cursor on the screen to the given offset
 * @param offset The offset to set the cursor to
 */
void set_cursor(int offset) {

    offset = offset / 2;

    port_byte_out(VGA_CTRL_REG, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REG, (unsigned char) (offset >> 8));
    port_byte_out(VGA_CTRL_REG, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REG, (unsigned char) (offset & 0xff));

}

/**
 * Get the current cursor position
 * @return The current cursor position
 */
int get_cursor(void) {

    port_byte_out(VGA_CTRL_REG, VGA_OFFSET_HIGH);

    int offset = port_byte_in(VGA_DATA_REG) << 8;
    port_byte_out(VGA_CTRL_REG, VGA_OFFSET_LOW);

    offset += port_byte_in(VGA_DATA_REG);
    return offset * 2;

}

/**
 * Set the character at the given offset in the video memory
 * @param c The character to set
 * @param offset The offset to set the character at
 */
void set_char_at_video_memory(char c, int offset) {

    unsigned char* vidmem = (unsigned char*)VGA_ADDRESS;
    vidmem[offset] = c;
    vidmem[offset + 1] = WHITE_ON_BLACK;

    return;

}

/**
 * Get the row from the offset
 * @param offset The offset to get the row from
 */
int get_row_from_offset(int offset) {

    return offset / (2 * VGA_MAX_COLUMNS);

}

/**
 * Get the offset from the column and row
 * @param column The column
 * @param row The row
 */
int get_offset(int column, int row) {

    return 2 * (row * VGA_MAX_COLUMNS + column);

}

/**
 * Get the offset of the next newline
 * @param offset The offset to get the next newline from
 */
int get_newline_offset(int offset) {

    get_offset(0, get_row_from_offset(offset) + 1);

}

/**
 * Scroll the screen one line upwards
 * @param offset The current offset
 */
int scroll_line(int offset) {

    // write every line upwards one
    memcpy(
        (void*)(get_offset(0, 0) + VGA_ADDRESS), 
        (void*)(get_offset(0, 1) + VGA_ADDRESS),
        VGA_MAX_COLUMNS * (VGA_MAX_ROWS-1) * 2);

    // write the very last line
    for (int column = 0; column < VGA_MAX_COLUMNS; ++column) {
        set_char_at_video_memory(' ', get_offset(column, VGA_MAX_ROWS - 1));
    }

    return offset - 2 * VGA_MAX_COLUMNS;

}

/**
 * Write a character to the screen
 * @param c The character to write
 */
void write_char(char c) {

    int offset = get_cursor();
    set_char_at_video_memory(c, offset);
    set_cursor(offset + 2);

    return;

}

/**
 * Write a 32-bit hex number to the screen
 * @param n The number to write
 */
void write_hex(uint32_t n) {

    char hex[9];

    for (int i = 0; i < 8; i++) {

        uint8_t nibble = (n >> (4 * (7 - i))) & 0xF;
        hex[i] = nibble < 10 ? '0' + nibble : 'A' + nibble - 10;

    }

    hex[8] = '\0';

    write_string("0x");
    write_string(hex);

}

/**
 * Write a 16-bit hex number to the screen
 * @param n The number to write
 */
void write_hex_16(uint16_t n) {

    char hex[5];

    for (int i = 0; i < 4; i++) {

        uint8_t nibble = (n >> (4 * (3 - i))) & 0xF;
        hex[i] = nibble < 10 ? '0' + nibble : 'A' + nibble - 10;

    }

    hex[4] = '\0';

    write_string("0x");
    write_string(hex);

}

/**
 * Write an 8-bit hex number to the screen
 * @param n The number to write
 */
void write_hex_8(uint8_t n) {

    char hex[3];

    for (int i = 0; i < 2; i++) {
        uint8_t nibble = (n >> (4 * (1 - i))) & 0xF;
        hex[i] = nibble < 10 ? '0' + nibble : 'A' + nibble - 10;
    }

    hex[2] = '\0';

    write_string("0x");
    write_string(hex);

}

void write_int(isize n) {

    char number[33];

    itoa(n, number, 10);

    number[32] = '\0';

    write_string(number);
}

/**
 * Clear the screen
 */
void clear_screen() {

    for (int i = 0; i < VGA_MAX_COLUMNS * VGA_MAX_COLUMNS; i++) {

        set_char_at_video_memory(' ', i * 2);

    }

    set_cursor(get_offset(0, 0));

}

/**
 * Write a newline to the screen
 */
void write_newline() {

    int offset = get_cursor();

    offset = get_newline_offset(offset);
    set_cursor(offset);

}

/**
 * write a string to the screen
 * @param s The string to write
 */
void write_string(const char *s) {

    int offset = get_cursor();
    int index = 0;

    while (s[index] != '\0') {

        if (offset >= VGA_MAX_COLUMNS * VGA_MAX_ROWS * 2) {

            offset = scroll_line(offset);

        }

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

/**
 * Dump the registers to the screen
 * @param regs The registers to dump
 */
void dump_registers(registers_t *regs) {

    write_string("EAX: ");
    write_hex(regs->eax);

    write_string("\nEBX: ");
    write_hex(regs->ebx);

    write_string("\nECX: ");
    write_hex(regs->ecx);

    write_string("\nEDX: ");
    write_hex(regs->edx);

    write_string("\nESI: ");
    write_hex(regs->esi);

    write_string("\nEDI: ");
    write_hex(regs->edi);

    write_string("\nEBP: ");
    write_hex(regs->ebp);

    write_string("\nESP: ");
    write_hex(regs->esp);

    write_string("\nEIP: ");
    write_hex(regs->eip);

    write_string("\nEFLAGS: ");
    write_hex(regs->eflags);

    write_string("\nCS: ");
    write_hex_16(regs->cs);

    write_string("\nDS: ");
    write_hex_16(regs->ds);
    write_string("\n");

}

/**
 * Dump the stack to the screen
 * @param regs The registers to dump the stack from
 * @param size The size of the stack to dump
 */
void dump_from_stack(registers_t *regs, size_t size) {

    write_string("Address    | Values\n");

    for (size_t i = 0; i < size; ++i) {

        if (i % 4 == 0) {

            write_hex(regs->esp + i);
            write_string(" | ");

        }

        write_hex(*(size_t*)(regs->esp + i * 4));

        if (i % 4 == 3) {

            write_string("\n");

        } else {

            write_string(" ");

        }

    }

}

/**
 * Dump the memory from the given address
 * @param address The address to dump from
 * @param size The size of the memory to dump
 */
void dump_from_memory_address(uint32_t address, size_t size) {

    write_string("Address    | Values\n");

    uint32_t byte_max_offset = size * 4;

    for (uint32_t i = 0; i < size; ++i) {

        if (i % 4 == 0) {

            write_hex(address + i);
            write_string(" | ");

        }

        write_hex(*(size_t*)(address + i * 4));

        if (i % 4 == 3) {

            write_string("\n");

        } else {

            write_string(" ");

        }

    }

}

/**
 * Print a letter to the screen
 * @param scancode The ps2 scancode of the letter to print
 */
void print_letter(uint8_t scancode) {

    switch(scancode) {

        case 0x00:
            write_string("ERROR");
            break;

        case 0x01:
            write_string("ESC");
            break;

        case 0x02:
            write_string("1");
            break;

        case 0x03:
            write_string("2");
            break;

        case 0x04:
            write_string("3");
            break;

        case 0x05:
            write_string("4");
            break;

        case 0x06:
            write_string("5");
            break;

        case 0x07:
            write_string("6");
            break;

        case 0x08:
            write_string("7");
            break;

        case 0x09:
            write_string("8");
            break;

        case 0x0A:
            write_string("9");
            break;

        case 0x0B:
            write_string("0");
            break;

        case 0x0C:
            write_string("-");
            break;

        case 0x0D:
            write_string("=");
            break;

        case 0x0E:
            write_string("BACKSPACE");
            break;

        case 0x0F:
            write_string("TAB");
            break;

        case 0x10:
            write_string("Q");
            break;

        case 0x11:
            write_string("W");
            break;

        case 0x12:
            write_string("E");
            break;

        case 0x13:
            write_string("R");
            break;

        case 0x14:
            write_string("T");
            break;

        case 0x15:
            write_string("Y");
            break;

        case 0x16:
            write_string("U");
            break;

        case 0x17:
            write_string("I");
            break;

        case 0x18:
            write_string("O");
            break;

        case 0x19:
            write_string("P");
            break;

        case 0x1A:
            write_string("[");
            break;

        case 0x1B:
            write_string("]");
            break;

        case 0x1C:
            write_string("ENTER");
            break;

        case 0x1D:
            write_string("LEFT CTRL");
            break;

        case 0x1E:
            write_string("A");
            break;

        case 0x1F:
            write_string("S");
            break;

        case 0x20:
            write_string("D");
            break;

        case 0x21:
            write_string("F");
            break;

        case 0x22:
            write_string("G");
            break;

        case 0x23:
            write_string("H");
            break;

        case 0x24:
            write_string("J");
            break;

        case 0x25:
            write_string("K");
            break;

        case 0x26:
            write_string("L");
            break;

        case 0x27:
            write_string(";");
            break;

        case 0x28:
            write_string("'");
            break;

        case 0x29:
            write_string("`");
            break;

        case 0x2A:
            write_string("LEFT SHIFT");
            break;

        case 0x2B:
            write_string("\\");
            break;

        case 0x2C:
            write_string("Z");
            break;

        case 0x2D:
            write_string("X");
            break;

        case 0x2E:
            write_string("C");
            break;

        case 0x2F:
            write_string("V");
            break;

        case 0x30:
            write_string("B");
            break;

        case 0x31:
            write_string("N");
            break;

        case 0x32:
            write_string("M");
            break;

        case 0x33:
            write_string(",");
            break;

        case 0x34:
            write_string(".");
            break;

        case 0x35:
            write_string("/");
            break;

        case 0x36:
            write_string("RIGHT SHIFT");
            break;

        case 0x37:
            write_string("KP*");
            break;

        case 0x38:
            write_string("LEFT ALT");
            break;

        case 0x39:
            write_string("SPACE");
            break;

        default:

            if (scancode <= 0x7f) {

                write_string("UNKNOWN KEY");

            } else if (scancode <= 0x39 + 0x80) {

                write_string("KEY RELEASED: ");
                print_letter(scancode - 0x80);

            } else {

                write_string("UNKNOWN KEY UP");

            }

            break;
    }

}
