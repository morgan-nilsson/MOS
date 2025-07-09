// Copyright 2025 Morgan Nilsson

#ifndef SRC_LIBS_INCLUDE_STDTYPES_H
#define SRC_LIBS_INCLUDE_STDTYPES_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

typedef uint32_t usize;
typedef uint32_t size_t;
typedef int32_t isize;

typedef uint32_t uintptr_t;
typedef int32_t intptr_t;

typedef struct {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;


#endif