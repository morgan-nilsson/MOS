#include "../inc/stdtypes.h"
#include "../inc/bool.h"

#ifndef _STDLIB_H
#define _STDLIB_H

#define NULL ((void *)0)

// memory manigment struct is a dLL

typedef struct mem_block {
    uint32_t size;
    bool free;
    struct mem_block *next;
    struct mem_block *prev;
} mem_block_t;

#endif