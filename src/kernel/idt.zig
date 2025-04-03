// Copyright 2025 Morgan Nilsson

const idt_gate_t = packed struct { low_offset: u32, selector: u16, zero: u8, flags: u8, high_offset: u16 };
const idt_register_t = packed struct { limit: u16, base: u32 };

const idt_entries = 256;

const idt = [idt_entries]idt_gate_t;
const idt_reg = idt_register_t;

fn low_16(address: u32) u16 {
    return @as(u16, address & 0xFFFF);
}

fn high_16(address: u32) u16 {
    return @as(u16, (address >> 16) & 0xFFFF);
}

pub fn set_idt_gate(n: i32, handler: u32, sel: u16, flags: u8) void {
    idt[n].low_offset = low_16(handler);
    idt[n].selector = sel;
    idt[n].zero = 0;

    idt[n].flags = flags;
    idt[n].high_offset = high_16(handler);
}

pub fn load_idt() void {
    idt_reg.base = @as(u32, &idt);
    idt_reg.limit = idt_entries * @sizeOf(idt_gate_t) - 1;
    asm volatile ("lidt (%[idt])"
        :
        : [idt] "r" (idt_reg),
        : "memory"
    );
}
