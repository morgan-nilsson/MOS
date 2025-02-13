#ifndef _STDLIB_H
#define _STDLIB_H

#define MEM_BLOCK_SIZE 4096
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