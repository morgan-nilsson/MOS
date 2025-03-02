// Copyright 2025 Morgan Nilsson

#include "../include/keyboard_driver.h"
#include "../include/vga_driver.h"
#include "../../libs/include/stdio.h"
#include "../../kernel/include/isr.h"
#include "../../driver/include/keyboard.h"

static void keyboard_callback(registers_t *r) {
    uint8_t scancode = port_byte_in(0x60);
    print_letter(scancode);
    write_newline();
}

void init_keyboard(void) {
    //register_interrupt_handler(IRQ1, keyboard_callback);
}

uint8_t get_scancode() {
    while (!(port_byte_in(0x64) & 1)); // Wait until output buffer is full

    uint8_t scancode = port_byte_in(0x60);
    
    if (scancode == 0xF0) { 
        while (!(port_byte_in(0x64) & 1));
        port_byte_in(0x60);
        return 0;
    }

    return scancode;
}

VirtualKey get_Vkey(void) {
    uint8_t scancode = 0;

    while (1) {
        scancode = get_scancode();
        
        if (scancode != 0) {  // Ensure we don’t return invalid keys
            return translate_PS2_scancode_to_Vkey(scancode);
        }
    }
}

/**
 * Blocking function that returns only characters from the keyboard.
 * Will ignore key-up events.
 */
char get_char(void) {
    uint8_t scancode = 0;

    while (1) {
        // Wait for a key event
        while (!(port_byte_in(0x64) & 1));

        scancode = port_byte_in(0x60);

        // Ignore Key Up events (0x80 offset)
        if (scancode & 0x80) {
            continue;
        }

        // Convert the scancode to a character
        char c = Virtual_key_to_char(translate_PS2_scancode_to_Vkey(scancode));

        if (c) return c; // Only return valid characters
    }
}



VirtualKey translate_PS2_scancode_to_Vkey(uint8_t scancode) {
    switch(scancode) {
        case 0x01: return VK_ESCAPE;
        case 0x02: return VK_1;
        case 0x03: return VK_2;
        case 0x04: return VK_3;
        case 0x05: return VK_4;
        case 0x06: return VK_5;
        case 0x07: return VK_6;
        case 0x08: return VK_7;
        case 0x09: return VK_8;
        case 0x0A: return VK_9;
        case 0x0B: return VK_0;
        case 0x0C: return VK_OEM_MINUS;
        case 0x0D: return VK_OEM_PLUS;
        case 0x0E: return VK_BACK;
        case 0x0F: return VK_TAB;
        case 0x10: return VK_Q;
        case 0x11: return VK_W;
        case 0x12: return VK_E;
        case 0x13: return VK_R;
        case 0x14: return VK_T;
        case 0x15: return VK_Y;
        case 0x16: return VK_U;
        case 0x17: return VK_I;
        case 0x18: return VK_O;
        case 0x19: return VK_P;
        case 0x1A: return VK_OEM_4;
        case 0x1B: return VK_OEM_6;
        case 0x1C: return VK_RETURN;
        case 0x1D: return VK_LCONTROL;
        case 0x1E: return VK_A;
        case 0x1F: return VK_S;
        case 0x20: return VK_D;
        case 0x21: return VK_F;
        case 0x22: return VK_G;
        case 0x23: return VK_H;
        case 0x24: return VK_J;
        case 0x25: return VK_K;
        case 0x26: return VK_L;
        case 0x27: return VK_OEM_1;
        case 0x28: return VK_OEM_7;
        case 0x29: return VK_OEM_3;
        case 0x2A: return VK_LSHIFT;
        case 0x2B: return VK_OEM_5;
        case 0x2C: return VK_Z;
        case 0x2D: return VK_X;
        case 0x2E: return VK_C;
        case 0x2F: return VK_V;
        case 0x30: return VK_B;
        case 0x31: return VK_N;
        case 0x32: return VK_M;
        case 0x33: return VK_OEM_COMMA;
        case 0x34: return VK_OEM_PERIOD;
        case 0x35: return VK_OEM_2;
        case 0x36: return VK_RSHIFT;
        case 0x37: return VK_MULTIPLY;
        case 0x38: return VK_LMENU;
        case 0x39: return VK_SPACE;
        case 0x3A: return VK_CAPITAL;
        case 0x3B: return VK_F1;
        case 0x3C: return VK_F2;
        case 0x3D: return VK_F3;
        case 0x3E: return VK_F4;
        case 0x3F: return VK_F5;
        case 0x40: return VK_F6;
        case 0x41: return VK_F7;
        case 0x42: return VK_F8;
        case 0x43: return VK_F9;
        case 0x44: return VK_F10;
        case 0x45: return VK_NUMLOCK;
        case 0x46: return VK_SCROLL;
        case 0x47: return VK_HOME;
        case 0x48: return VK_UP;
        case 0x49: return VK_PRIOR;
        case 0x4B: return VK_LEFT;
        case 0x4D: return VK_RIGHT;
        case 0x4F: return VK_END;
        case 0x50: return VK_DOWN;
        case 0x51: return VK_NEXT;
        case 0x52: return VK_INSERT;
        case 0x53: return VK_DELETE;
        case 0x54: return VK_OEM_102;
        case 0x56: return VK_F11;
        case 0x57: return VK_F12;
        case 0x58: return VK_F13;
        case 0x59: return VK_F14;
        case 0x5A: return VK_F15;
        case 0x5B: return VK_F16;
        case 0x5C: return VK_F17;
        case 0x5D: return VK_F18;
        case 0x5E: return VK_F19;
        case 0x5F: return VK_F20;
        case 0x60: return VK_F21;
        case 0x61: return VK_F22;
        case 0x62: return VK_F23;
        case 0x63: return VK_F24;
        case 0x6B: return VK_OEM_4;
        case 0x6C: return VK_OEM_6;
        case 0x6D: return VK_OEM_1;
        case 0x6E: return VK_OEM_7;
        case 0x6F: return VK_OEM_3;
        case 0x70: return VK_OEM_5;
        case 0x71: return VK_OEM_COMMA;
        case 0x72: return VK_OEM_PERIOD;
        case 0x73: return VK_OEM_2;
        case 0x74: return VK_OEM_102;
        case 0x75: return VK_PROCESSKEY;
    }
}