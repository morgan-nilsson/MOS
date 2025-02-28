// Copyright 2025 Morgan Nilsson

#ifndef SRC_KERNEL_INCLUDE_IDT_H_
#define SRC_KERNEL_INCLUDE_IDT_H_

#include "../../libs/include/stdtypes.h"
#define IDT_ENTRIES 256

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)


typedef struct {
    uint16_t low_offset;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t high_offset;
} __attribute__((packed)) idt_gate_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;

void set_idt_gate(int n, uint32_t handler, uint16_t sel, uint8_t flags);
void load_idt(void);

#endif