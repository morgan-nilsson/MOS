#include "kernel/paging.h"

#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4

// paging code first 3gb user last 1gb kernel
#define PDE_ENTRIES 1024
#define PTE_ENTRIES 1024

// page table entry
static uint32_t page_directory[PDE_ENTRIES] __attribute__((aligned(4096)));

// page table
static uint32_t page_table[PDE_ENTRIES][PTE_ENTRIES] __attribute__((aligned(4096)));

// flat memory for all
void init_paging() {

    for (unsigned int i = 0; i < 1024; i++) {

        uint32_t pde_flags = PAGE_PRESENT | PAGE_WRITE;

        for (unsigned int j = 0; j < 1024; j++) {
            uint32_t address = (i * 1024 + j) * 0x1000;

            // remove lower 12 bits and add flags
            page_table[i][j] = (address & ~0xFFF) | pde_flags;
        }

        page_directory[i] = (uint32_t)page_table[i] | pde_flags;
    }

    // unmap null page
    page_table[0][0] = 0;

    asm volatile("mov %0, %%cr3" : : "r" (page_directory));

    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" : : "r" (cr0));
}
