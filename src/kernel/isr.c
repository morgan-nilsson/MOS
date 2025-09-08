// Copyright 2025 Morgan Nilsson

#include "kernel/isr.h"
#include "libs/stdio.h"
#include "kernel/idt.h"
#include "driver/vga_driver.h"

isr_t interrupt_handlers[256];

void isr_install() {
    // internal IRSs
    set_idt_gate(0, (uintptr_t)isr0, 0x08, 0x8E);
    set_idt_gate(1, (uintptr_t)isr1, 0x08, 0x8E);
    set_idt_gate(2, (uintptr_t)isr2, 0x08, 0x8E);
    set_idt_gate(3, (uintptr_t)isr3, 0x08, 0x8E);
    set_idt_gate(4, (uintptr_t)isr4, 0x08, 0x8E);
    set_idt_gate(5, (uintptr_t)isr5, 0x08, 0x8E);
    set_idt_gate(6, (uintptr_t)isr6, 0x08, 0x8E);
    set_idt_gate(7, (uintptr_t)isr7, 0x08, 0x8E);
    set_idt_gate(8, (uintptr_t)isr8, 0x08, 0x8E);
    set_idt_gate(9, (uintptr_t)isr9, 0x08, 0x8E);
    set_idt_gate(10, (uintptr_t)isr10, 0x08, 0x8E);
    set_idt_gate(11, (uintptr_t)isr11, 0x08, 0x8E);
    set_idt_gate(12, (uintptr_t)isr12, 0x08, 0x8E);
    set_idt_gate(13, (uintptr_t)isr13, 0x08, 0x8E);
    set_idt_gate(14, (uintptr_t)isr14, 0x08, 0x8E);
    set_idt_gate(15, (uintptr_t)isr15, 0x08, 0x8E);
    set_idt_gate(16, (uintptr_t)isr16, 0x08, 0x8E);
    set_idt_gate(17, (uintptr_t)isr17, 0x08, 0x8E);
    set_idt_gate(18, (uintptr_t)isr18, 0x08, 0x8E);
    set_idt_gate(19, (uintptr_t)isr19, 0x08, 0x8E);
    set_idt_gate(20, (uintptr_t)isr20, 0x08, 0x8E);
    set_idt_gate(21, (uintptr_t)isr21, 0x08, 0x8E);
    set_idt_gate(22, (uintptr_t)isr22, 0x08, 0x8E);
    set_idt_gate(23, (uintptr_t)isr23, 0x08, 0x8E);
    set_idt_gate(24, (uintptr_t)isr24, 0x08, 0x8E);
    set_idt_gate(25, (uintptr_t)isr25, 0x08, 0x8E);
    set_idt_gate(26, (uintptr_t)isr26, 0x08, 0x8E);
    set_idt_gate(27, (uintptr_t)isr27, 0x08, 0x8E);
    set_idt_gate(28, (uintptr_t)isr28, 0x08, 0x8E);
    set_idt_gate(29, (uintptr_t)isr29, 0x08, 0x8E);
    set_idt_gate(30, (uintptr_t)isr30, 0x08, 0x8E);
    set_idt_gate(31, (uintptr_t)isr31, 0x08, 0x8E);

//   PIC remapping

    // ICW1
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);

    // ICW2
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);

    // ICW3
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);

    // ICW4
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);

    // OCW1
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);

    // iqr isrs (primary PIC)
    set_idt_gate(32, (uintptr_t)irq0, 0x08, 0x8E);
    set_idt_gate(33, (uintptr_t)irq1, 0x08, 0x8E);
    set_idt_gate(34, (uintptr_t)irq2, 0x08, 0x8E);
    set_idt_gate(35, (uintptr_t)irq3, 0x08, 0x8E);
    set_idt_gate(36, (uintptr_t)irq4, 0x08, 0x8E);
    set_idt_gate(37, (uintptr_t)irq5, 0x08, 0x8E);
    set_idt_gate(38, (uintptr_t)irq6, 0x08, 0x8E);
    set_idt_gate(39, (uintptr_t)irq7, 0x08, 0x8E);

    // iqr isrs (secondary PIC)
    set_idt_gate(40, (uintptr_t)irq8, 0x08, 0x8E);
    set_idt_gate(41, (uintptr_t)irq9, 0x08, 0x8E);
    set_idt_gate(42, (uintptr_t)irq10, 0x08, 0x8E);
    set_idt_gate(43, (uintptr_t)irq11, 0x08, 0x8E);
    set_idt_gate(44, (uintptr_t)irq12, 0x08, 0x8E);
    set_idt_gate(45, (uintptr_t)irq13, 0x08, 0x8E);
    set_idt_gate(46, (uintptr_t)irq14, 0x08, 0x8E);
    set_idt_gate(47, (uintptr_t)irq15, 0x08, 0x8E);

    load_idt();
}

char *exeception_msgs[] = {
    "Division by zero",
    "Debug",
    "Non-Maskable Interupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Bounds Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 FPU Floating-Point Error",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

void isr_handler(registers_t *r) {
    write_string(exeception_msgs[r->int_no]);
    write_newline();
    dump_registers(r);

    for (;;) {
        __asm__ volatile ("hlt"); // sleep this CPU forever
    }
}

void irq_handler(registers_t *r) {
    if (interrupt_handlers[r->int_no] != 0) {
        isr_t handler = interrupt_handlers[r->int_no];
        handler(r);
    }

    if (r->int_no >= 40) {
        port_byte_out(0xA0, 0x20);
    }
    port_byte_out(0x20, 0x20);
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}