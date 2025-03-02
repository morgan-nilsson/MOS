// Copyright 2025 Morgan Nilsson

#ifndef SRC_KERNEL_INCLUDE_SYSTEM_H_
#define SRC_KERNEL_INCLUDE_SYSTEM_H_

#include "../../libs/include/stdtypes.h"
#define NUM_SYSCALLS 2

typedef int (*syscall_t)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

void init_paging();

int sys_read(int fd, char *buf, int count, char unused, char unused2);
int sys_write(int fd, const char *buf, int count, char unused, char unused2);
int sys_sleep(int ms, char unused, char unused2, char unused3, char unused4);

int syscall(unsigned int num, int arg1, int arg2, int arg3, int arg4, int arg5);
void init_syscalls();

#endif