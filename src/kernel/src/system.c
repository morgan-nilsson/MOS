// Copyright 2025 Morgan Nilsson

#include "../include/system.h"
#include "../include/isr.h"
#include "../include/idt.h"
#include "../../libs/include/string.h"
#include "../../driver/include/vga_driver.h"
#include "../../libs/include/stdlib.h"
#include "../../driver/include/keyboard_driver.h"
#include "../../driver/include/keyboard.h"

extern void syscall_handler();

#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4


syscall_t syscall_table[NUM_SYSCALLS] = {
    (syscall_t) sys_read,
    (syscall_t) sys_write,
};

// paging code first 3gb user last 1gb kernel

// page table entry
static uint32_t page_directory[1024] __attribute__((aligned(4096)));

// page table
static uint32_t page_table[1024][1024] __attribute__((aligned(4096)));

void init_paging() {

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

int sys_read(int fd, char *buf, int count, char unused, char unused2) {
    if (buf == NULL || count <= 0) {
        return -1;
    }

    switch (fd) {
        case 0: {
            int i = 0;
            while (i < count - 1) {
                buf[i] = get_char();

                if (buf[i] == '\n') {
                    i++;
                    break;
                }

                i++;
            }
            buf[i] = '\0';
            return i;
        }

        case 1:
        case 2:
            return -1;

        default:
            return -1;
    }
}

int sys_write(int fd, const char *buf, int count, char unused, char unused2) {

    switch (fd) {
    case 1:
    case 2:

        for (int i = 0; i < count; i++) {
            if (buf[i] == '\0') {
                return i;
            }
            write_char(buf[i]);
        }

        return count;
    
    default:
        return -1;
    }
}

void init_syscalls() {
    // add syscall interrupt
    set_idt_gate(0x80, (uint32_t)syscall_handler, 0x08, 0x8E);
}

int syscall(unsigned int num, int arg1, int arg2, int arg3, int arg4, int arg5) {
    int ret;

    // pass argumnet via registers to syscall
    asm volatile("int $0x80" : "=a" (ret) : "a" (num), "b" (arg1), "c" (arg2), "d" (arg3), "S" (arg4), "D" (arg5));

    return ret;
}
