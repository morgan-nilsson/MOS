// Copyright 2025 Morgan Nilsson
#ifndef SRC_LIBS_INCLUDE_STDTYPES_H
#define SRC_LIBS_INCLUDE_STDTYPES_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

#endif