#include "libs/arena.h"

void arena_init(arena_t* arena) {
    arena->data = mem_alloc(ARENA_DEFAULT_CAPACITY);
    arena->size = 0;
    arena->capacity = ARENA_DEFAULT_CAPACITY;
}

void arena_init_with_capacity(arena_t* arena, usize capacity) {
    arena->data = mem_alloc(capacity);
    arena->size = 0;
    arena->capacity = capacity;
}

void arena_destroy(arena_t* arena) {
    mem_free(arena->data);
    arena->data = NULL;
    arena->size = 0;
    arena->capacity = 0;
}

void arena_push(arena_t* arena, const void* data, usize size) {
    if (arena->size + size > arena->capacity) {
        usize new_capacity = arena->capacity * ARENA_GROWTH_FACTOR;
        while (new_capacity < arena->size + size) {
            new_capacity *= ARENA_GROWTH_FACTOR;
        }
        void* new_data = mem_alloc(new_capacity);
        memcpy(new_data, arena->data, arena->size);
        mem_free(arena->data);
        arena->data = new_data;
        arena->capacity = new_capacity;
    }
    memcpy((uint8_t*)arena->data + arena->size, data, size);
    arena->size += size;
}

void arena_reset(arena_t* arena) {
    arena->size = 0;
}

void arena_reset_to_mark(arena_t* arena, ArenaMark_t mark) {
    if (mark <= arena->size) {
        arena->size = mark;
    }
}