#include "kernel/gdt.h"
#include "libs/stdlib.h"
#include "driver/vga_driver.h"

static gdt_ptr_t gdt_ptr;

#define GDT_ENTRIES 6
static gdt_entry_t gdt_entries[GDT_ENTRIES];


static tss_t tss_entry;

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}

void init_tss() {

    // clear tss
    memset(&tss_entry, 0, sizeof(tss_t));

    tss_entry.ss0 = KDATA;
    tss_entry.esp0 = 0;
    tss_entry.iomap_base = sizeof(tss_t);

    asm volatile ("ltr %0" : : "r" (TSS_SEL));

}

/**
 * Initialize the GDT
 * Replacing the temperary GDT set up in the bootloader
 */
void init_gdt() {

    asm volatile ("cli");

    // Set null descriptor
    gdt_set_gate(0, 0, 0, 0, 0);

    // Set kernel code segment
    gdt_set_gate(1, 0, 0xFFFFF, 0x9A, 0xCF);

    // Set kernel data segment
    gdt_set_gate(2, 0, 0xFFFFF, 0x92, 0xCF);

    // Set user code segment
    gdt_set_gate(3, 0, 0xFFFFF, 0xFA, 0xCF);

    // Set user data segment
    gdt_set_gate(4, 0, 0xFFFFF, 0xF2, 0xCF);

    // Set TSS segment
    gdt_set_gate(5, (uint32_t)&tss_entry, sizeof(tss_t) - 1, 0x89, 0);

    gdt_ptr.limit = (sizeof(gdt_entry_t) * GDT_ENTRIES) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    asm volatile ("lgdt (%0)" : : "r" (&gdt_ptr));

    init_tss();

    asm volatile(
        "pushl %[kcode]\n"
        "pushl $1f\n"
        "lret\n"
        "1:\n"
        "mov %[kdata], %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        :
        : [kcode]"i"(KCODE), [kdata]"i"(KDATA)
        : "ax", "memory");

    asm volatile ("sti");
}

void gdt_set_tss(uint32_t esp0) {
    tss_entry.esp0 = esp0;
}