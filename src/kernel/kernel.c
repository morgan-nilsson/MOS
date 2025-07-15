// Copyright 2025 Morgan Nilsson

#include "driver/keyboard_driver.h"
#include "driver/vga_driver.h"
#include "driver/timer_driver.h"
#include "driver/vbe_driver.h"
#include "libs/math.h"
#include "libs/stdlib.h"
#include "kernel/system.h"
#include "kernel/scheduler.h"

void task1(void) {
    write_string("1");
    while (1) {
    }
}

void task2(void) {
    write_string("2");
    while (1) {
    }
}

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
    init_timer(100);
    write_string("Done\n");

    write_string("Init syscalls: ");
    init_syscalls();
    write_string("Done\n");

    write_string("Starting mem test\n");

    write_string("Init memAlloc: ");
    init_mem_alloc();
    write_string("Done\n");

    write_string("Adding tasks: ");
    add_task(task1);
    add_task(task2);
    write_string("Done\n");

    write_string("Activating scheduler: ");
    activate_scheduler();
    write_string("Done\n");

    while (1);
}
