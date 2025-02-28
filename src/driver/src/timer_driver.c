// Copyright 2025 Morgan Nilsson

#include "../include/timer_driver.h"
#include "../../libs/include/stdio.h"
#include "../../driver/include/vga_driver.h"

uint32_t tick = 0;

#define UNUSED(x) (void)(x)

static void timer_callback(registers_t *regs) {
    tick++;
    UNUSED(regs);
}

void init_timer(uint32_t frequency) {

    register_interrupt_handler(IRQ0, timer_callback);

    uint32_t divisor = 1193180 / frequency;

    port_byte_out(0x43, 0x36);

    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

    port_byte_out(0x40, l);
    port_byte_out(0x40, h);
}