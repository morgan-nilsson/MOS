#include "../libs/inc/vga_driver.h"
#include "../libs/inc/string.h"
#include "../libs/inc/keyboard_driver.h"
#include "../libs/inc/isr.h"
#include "../libs/inc/stdlib.h"
#include "../libs/inc/timer.h"

void _start(void) { 
    clear_screen();
    write_string("Installing isrs\n");
    isr_install();

    write_string("Enabling external interrupts\n");
    asm volatile("sti");

    write_string("Installing keyboard driver\n");
    init_keyboard();
    write_string("Keyboard driver installed\n");

    write_string("Installing timer\n");
    init_timer(50);

    while (1);
}