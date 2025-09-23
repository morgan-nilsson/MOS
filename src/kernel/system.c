// Copyright 2025 Morgan Nilsson

#include "kernel/system.h"
#include "kernel/isr.h"
#include "kernel/idt.h"
#include "libs/string.h"
#include "driver/vga_driver.h"
#include "libs/stdlib.h"
#include "driver/keyboard_driver.h"
#include "driver/keyboard.h"
#include "driver/timer_driver.h"

extern void syscall_handler();

syscall_t syscall_table[NUM_SYSCALLS] = {
    (syscall_t) sys_read,
    (syscall_t) sys_write,
    (syscall_t) sys_sleep
};

int sys_read(int fd, int buf_ptr, int count, int unused, int unused2) {
    UNUSED(unused);
    UNUSED(unused2);
    char *buf = (char *)buf_ptr;

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

int sys_write(int fd, int buf_ptr, int count, int unused, int unused2) {

    UNUSED(unused);
    UNUSED(unused2);
    const char *buf = (char *)buf_ptr;

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

int sys_sleep(int ms, int unused, int unused2, int unused3, int unused4) {

    UNUSED(unused);
    UNUSED(unused2);
    UNUSED(unused3);
    UNUSED(unused4);

    asm ("sti");
    uint32_t end = get_tick_count() + ms;
    while (1) {
        uint32_t now = get_tick_count();
        if (now >= end) {
            break;
        }

    }
    return 0;
}

void init_syscalls() {
    // add syscall interrupt
    set_idt_gate(0x80, (uintptr_t)syscall_handler, 0x08, 0x8E);
}

int syscall(unsigned int num, int arg1, int arg2, int arg3, int arg4, int arg5) {
    int ret;

    // pass argumnet via registers to syscall
    asm volatile("int $0x80" : "=a" (ret) : "a" (num), "b" (arg1), "c" (arg2), "d" (arg3), "S" (arg4), "D" (arg5));

    return ret;
}
