// Copyright 2025 Morgan Nilsson

#ifndef SRC_KERNEL_INCLUDE_SYSTEM_H_
#define SRC_KERNEL_INCLUDE_SYSTEM_H_

#include "libs/stdtypes.h"
#define NUM_SYSCALLS 3


typedef int (*syscall_t)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

int sys_read(int fd, int buf, int count, int unused, int unused2);
int sys_write(int fd, int buf, int count, int unused, int unused2);
int sys_sleep(int ms, int unused, int unused2, int unused3, int unused4);

int syscall(unsigned int num, int arg1, int arg2, int arg3, int arg4, int arg5);
void init_syscalls();

#endif