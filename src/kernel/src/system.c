// Copyright 2025 Morgan Nilsson

#include "../include/system.h"

#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4

// paging code first 3gb user last 1gb kernel

// page table entry
uint32_t page_directory[1024] __attribute__((aligned(4096)));

// page table
uint32_t page_table[1024][1024] __attribute__((aligned(4096)));

void init_paging() {
    // clear the page directory
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;
    }

    for (int i = 0; i < 1024; i++) {
        for (int j = 0; j < 1024; j++) {
            uint32_t address = (i * 1024 + j) * 0x1000;
            uint32_t flags = PAGE_PRESENT | PAGE_WRITE;

            // if address is less than 3gb, set user flag
            if (address < 0xC0000000) {
                flags |= PAGE_USER;
            }

            page_table[i][j] = address | flags;
        }

        page_directory[i] = (uint32_t)page_table[i] | PAGE_PRESENT | PAGE_WRITE;
    }

    asm volatile("mov %0, %%cr3" : : "r" (page_directory));

    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" : : "r" (cr0));
}