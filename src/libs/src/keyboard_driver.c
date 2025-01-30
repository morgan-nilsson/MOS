#include "../inc/keyboard_driver.h"
#include "../inc/vga_driver.h"
#include "../inc/isr.h"
#include "../inc/idt.h"
#include "../inc/stdio.h"

static void keyboard_callback(registers_t *r) {
    uint8_t scancode = port_byte_in(0x60);
    print_letter(scancode);
    write_newline();
}

void init_keyboard(void) {
    register_interrupt_handler(IRQ1, keyboard_callback);
}