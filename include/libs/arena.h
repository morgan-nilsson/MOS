#ifndef ARENA_H
#define ARENA_H

#include "libs/stdlib.h"
#include "libs/stdtypes.h"

#define ARENA_DEFAULT_CAPACITY 1024
#define ARENA_GROWTH_FACTOR 2

typedef struct {
    void* data;
    size_t size;
    size_t capacity;
} arena_t;

typedef size_t ArenaMark_t;

void arena_init(arena_t* arena);
void arena_init_with_capacity(arena_t* arena, size_t capacity);
void arena_destroy(arena_t* arena);

void arena_push(arena_t* arena, const void* data, size_t size);

void arena_reset(arena_t* arena);
ArenaMark_t arena_mark(arena_t* arena);
void arena_reset_to_mark(arena_t* arena, ArenaMark_t mark);




#endif