const stdtypes = @import("libs/stdtypes");
const idt = @import("kernel/idt");

pub const int_registers_t = struct { ds: c_uint, edi: c_uint, esi: c_uint, ebp: c_uint, esp: c_uint, ebx: c_uint, edx: c_uint, ecx: c_uint, eax: c_uint, int_no: c_uint, err_code: c_uint, eip: c_uint, cs: c_uint, eflags: c_uint, user_esp: c_uint, ss: c_uint };
const isr_t = fn (int_registers_t) void;

const IRQ0 = 32;

extern fn isr0() void;
extern fn isr1() void;
extern fn isr2() void;
extern fn isr3() void;
extern fn isr4() void;
extern fn isr5() void;
extern fn isr6() void;
extern fn isr7() void;
extern fn isr8() void;
extern fn isr9() void;
extern fn isr10() void;
extern fn isr11() void;
extern fn isr12() void;
extern fn isr13() void;
extern fn isr14() void;
extern fn isr15() void;
extern fn isr16() void;
extern fn isr17() void;
extern fn isr18() void;
extern fn isr19() void;
extern fn isr20() void;
extern fn isr21() void;
extern fn isr22() void;
extern fn isr23() void;
extern fn isr24() void;
extern fn isr25() void;
extern fn isr26() void;
extern fn isr27() void;
extern fn isr28() void;
extern fn isr29() void;
extern fn isr30() void;
extern fn isr31() void;

extern fn irq0() void;
extern fn irq1() void;
extern fn irq2() void;
extern fn irq3() void;
extern fn irq4() void;
extern fn irq5() void;
extern fn irq6() void;
extern fn irq7() void;
extern fn irq8() void;
extern fn irq9() void;
extern fn irq10() void;
extern fn irq11() void;
extern fn irq12() void;
extern fn irq13() void;
extern fn irq14() void;
extern fn irq15() void;

pub fn isr_install() void {}

export fn isr_handler(r: *int_registers_t) callconv(.C) void {
    _ = r;
}
export fn irq_handler(r: *int_registers_t) callconv(.C) void {
    _ = r;
}
pub fn register_interrupt_handler(n: u8, handler: isr_t) void {
    _ = n;
    _ = handler;
}
