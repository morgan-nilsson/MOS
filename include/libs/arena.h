#ifndef ARENA_H
#define ARENA_H

#include "libs/stdlib.h"
#include "libs/stdtypes.h"

#define ARENA_DEFAULT_CAPACITY 1024
#define ARENA_GROWTH_FACTOR 2

typedef struct {
    void* data;
    usize size;
    usize capacity;
} arena_t;

typedef usize ArenaMark_t;

void arena_init(arena_t* arena);
void arena_init_with_capacity(arena_t* arena, usize capacity);
void arena_destroy(arena_t* arena);

void arena_push(arena_t* arena, const void* data, usize size);

void arena_reset(arena_t* arena);
ArenaMark_t arena_mark(arena_t* arena);
void arena_reset_to_mark(arena_t* arena, ArenaMark_t mark);




#endif