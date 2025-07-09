// Copyright 2025 Morgan Nilsson

#include "driver/keyboard_driver.h"
#include "driver/vga_driver.h"
#include "driver/timer_driver.h"
#include "driver/vbe_driver.h"
#include "libs/math.h"
#include "libs/stdlib.h"
#include "kernel/system.h"

void _start(void) {
    init_paging();

    clear_screen();
    write_string("Installing isrs: ");
    isr_install();
    write_string("Done\n");

    write_string("Enabling external interrupts: ");
    asm volatile("sti");
    write_string("Done\n");

    write_string("Installing keyboard driver: ");
    init_keyboard();
    write_string("Done\n");

    write_string("Installing timer: ");
    init_timer(1000);
    write_string("Done\n");

    write_string("Init syscalls: ");
    init_syscalls();
    write_string("Done\n");

    write_string("Starting mem test\n");

    write_string("Init memAlloc: ");
    init_mem_alloc();
    write_string("Done\n");

    void *ptr = NULL;

    for (uint8_t i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            write_string("Freeing previous allocation\n");
            mem_free(ptr);
        }
        write_string("Allocating 1000 bytes: ");
        ptr = mem_alloc(1000);
        write_string("Done ");
        write_hex((uintptr_t)ptr);
        write_string("\n");
    }

    write_string("\nEnd\n");

    while (1);
}
