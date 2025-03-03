// Copyright 2025 Morgan Nilsson

#ifndef SRC_LIBS_INCLUDE_STDLIB_H_
#define SRC_LIBS_INCLUDE_STDLIB_H_

#define HEAP_SIZE_BYTES 4096
#define HEAP_START 0x200000
#define MEM_BLOCK_NODE_SIZE sizeof(mem_block_t)

#include "../../libs/include/stdtypes.h"
#include "../../libs/include/string.h"
#include "../../libs/include/bool.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

// memory manigment struct is a dLL
typedef struct mem_block {
    uint32_t size;
    bool free;
    struct mem_block *next;
    struct mem_block *prev;
} mem_block_t;

void *mem_alloc(uint32_t size);
void mem_free(void *ptr);

void itoa(int n, char *str, int base);
void reverse(char *s);

#endif