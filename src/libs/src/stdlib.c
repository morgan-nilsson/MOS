#include "../include/stdlib.h"

char* mem[MEM_BLOCK_SIZE];
static mem_block_t *mem_start;

void init_mem_alloc() {
    mem_start = (mem_block_t *)mem;
    mem_start->size = MEM_BLOCK_SIZE - MEM_BLOCK_NODE_SIZE;
    mem_start->next = NULL;
    mem_start->prev = NULL;
}

void *find_best_mem_block(mem_block_t *memLL, uint32_t size) {
    mem_block_t *best_mem_block = NULL;
    uint32_t best_mem_block_size = MEM_BLOCK_SIZE + 1;

    mem_block_t *curr_block = mem_start;
    while (curr_block) {
        if (curr_block->free && curr_block->size >= size + MEM_BLOCK_NODE_SIZE 
            && curr_block->size <= best_mem_block->size) {
                best_mem_block = curr_block;
                best_mem_block_size = curr_block->size;
            }

            curr_block = curr_block->next;
    }

    return best_mem_block;
}

void *mem_alloc(uint32_t size) {
    mem_block_t *best_mem_block = find_best_mem_block(mem_start, size);

    if (best_mem_block != NULL) {
        best_mem_block->size = best_mem_block->size - size - MEM_BLOCK_NODE_SIZE;

        mem_block_t *new_mem_node = (mem_block_t *) (
            ( (uint8_t *) best_mem_block) 
                + MEM_BLOCK_NODE_SIZE 
                + best_mem_block->size);
            
        new_mem_node->size = size;
        new_mem_node->free = false;
        new_mem_node->next = best_mem_block->next;
        new_mem_node->prev = best_mem_block;

        if (best_mem_block->next != NULL) {
            best_mem_block->next->prev = new_mem_node;
        }
        best_mem_block->next = new_mem_node;

        return (uint8_t *)new_mem_node + MEM_BLOCK_NODE_SIZE;
    }

    return NULL;
}

mem_block_t *merge_node_forward(mem_block_t *node) {
    mem_block_t *next_node = node->next;
    if (next_node != NULL && next_node->free) {
        node->size += node->next->size;
        node->size += MEM_BLOCK_NODE_SIZE;

        node->next = node->next->next;
        if (node->next != NULL) {
            node->next->prev = node;
        }
    }
    return node;
}

void *merge_node_previous(mem_block_t *node) {
    mem_block_t *prev_node = node->prev;
    if (prev_node != NULL && prev_node->free) {
        prev_node->size += node->size;
        prev_node->size += MEM_BLOCK_NODE_SIZE;

        prev_node->next = node->next;
        if (node->next != NULL) {
            node->next->prev = prev_node;
        }
    }
}

void mem_free(void *ptr) {
    if (ptr == NULL) return;

    mem_block_t *curr_node = (void*)((uint8_t *) ptr - MEM_BLOCK_NODE_SIZE);

    if (curr_node == NULL) return;

    curr_node->free = true;

    curr_node = merge_node_forward(curr_node);
    merge_node_previous(curr_node);
}

void reverse(char *s) {
    char c;
    int len = strlen(s);
    for (int i = 0; i < len / 2; i++) {
        c = s[i];
        s[i] = s[len - i - 1];
        s[len - i - 1] = c;
    }
}

void itoa(int n, char *str, int base) {
    char buf[128];

    int i = 0;
    bool is_negative = n < 0;
    n = is_negative ? -n : n;

    while (n) {
        int rem = n % base;
        buf[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        n = n / base;
    }

    if (is_negative) {
        buf[i++] = '-';
    }

    buf[i] = '\0';

    reverse(buf);

    strcpy(str, buf);
}