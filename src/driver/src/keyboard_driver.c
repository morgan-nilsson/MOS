// Copyright 2025 Morgan Nilsson

#include "../include/keyboard_driver.h"
#include "../include/vga_driver.h"
#include "../../libs/include/stdio.h"
#include "../../kernel/include/isr.h"

static void keyboard_callback(registers_t *r) {
    uint8_t scancode = port_byte_in(0x60);
    print_letter(scancode);
    write_newline();
}

void init_keyboard(void) {
    register_interrupt_handler(IRQ1, keyboard_callback);
}