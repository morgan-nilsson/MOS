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

#define _map_page(pd_index, pte_index, paddr, flags) \
    page_table[pd_index][pte_index] = (paddr & ~0xFFF) | (flags & 0xFFF)

#define _map_directory(pd_index, pt_index, flags) \
    page_directory[pd_index] = (pt_index & ~0xFFF) | (flags & 0xFFF)


void init_paging() {

    // ignore last page directory entry
    for (unsigned int i = 0; i < PDE_ENTRIES - 1; i++) {

        // select flag type
        uint32_t pde_flags = PAGE_PRESENT | PAGE_WRITE;
        if (i < 768) pde_flags |= PAGE_USER;

        for (unsigned int j = 0; j < PTE_ENTRIES; j++) {
            uint32_t paddr = (i * PTE_ENTRIES + j) * 0x1000;

            _map_page(i, j, paddr, pde_flags);
        }

        _map_directory(i, (uint32_t)page_table[i], pde_flags);
    }

    // unmap null page
    page_table[0][0] = 0;

    // map the page directory to itself
    _map_directory(1023, (uint32_t)page_directory, PAGE_PRESENT | PAGE_WRITE);

    asm volatile("mov %0, %%cr3" : : "r" (page_directory));

    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" : : "r" (cr0));
}
