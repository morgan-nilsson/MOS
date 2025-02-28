// Copyright 2025 Morgan Nilsson

#include "../../driver/include/keyboard_driver.h"
#include "../../driver/include/vga_driver.h"
#include "../../driver/include/timer_driver.h"
#include "../../driver/include/vbe_driver.h"
#include "../../libs/include/math.h"
#include "../include/system.h"

void _start(void) {
    init_paging();

    int i = abs(-5);

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

    write_string("End");

    while (1);
}