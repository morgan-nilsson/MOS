#include "kernel/paging.h"

#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4

#define PDE_INDEX(va) ((va) >> 22)
#define PTE_INDEX(va) (((va) >> 12) & 0x03FF)

// paging code first 3gb user last 1gb kernel
#define PDE_ENTRIES 1024
#define PTE_ENTRIES 1024

// page table entry
static uint32_t page_directory[PDE_ENTRIES] __attribute__((aligned(4096)));

// page table
static uint32_t page_table[PDE_ENTRIES][PTE_ENTRIES] __attribute__((aligned(4096)));

static inline void _map_page(uint32_t pde_index, uint32_t pte_index, physical_addr_t paddr, uint32_t flags) {
    page_table[pde_index][pte_index] = (paddr & ~0xFFF) | (flags & 0xFFF);
}

static inline void _map_directory(uint32_t pde_index, uint32_t *page_table, uint32_t flags) {
    page_directory[pde_index] = (uint32_t)page_table | flags;
}

void init_paging() {

    for (unsigned int i = 0; i < PDE_ENTRIES; i++) {

        // select flag type
        uint32_t pde_flags = PAGE_PRESENT | PAGE_WRITE;
        if (i < 768) pde_flags |= PAGE_USER;

        for (unsigned int j = 0; j < PTE_ENTRIES; j++) {
            uint32_t paddr = (i * PTE_ENTRIES + j) * 0x1000;

            
            page_table[i][j] = (paddr & ~0xFFF) | (pde_flags & 0xFFF);
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
