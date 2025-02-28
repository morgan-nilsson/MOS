// Copyright 2025 Morgan Nilsson

#include "../include/idt.h"

idt_gate_t idt[256];
idt_register_t idt_reg;
void set_idt_gate(int n, uint32_t handler, uint16_t sel, uint8_t flags) {
    idt[n].low_offset = low_16(handler);
    idt[n].selector = sel;
    idt[n].zero = 0;

    idt[n].flags = flags;
    idt[n].high_offset = high_16(handler);
}

void load_idt(void) {
    idt_reg.base = (uint32_t) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    asm volatile("lidtw (%0)" : : "r" (&idt_reg));
}