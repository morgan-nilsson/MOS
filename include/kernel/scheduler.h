// Copyright 2025 Morgan Nilsson

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "libs/stdtypes.h"
#include "libs/stdlib.h"

typedef void (*task_entry_t)(void);

#define TIME_SEGMENT_MS 100
#define TASK_STACK_SIZE 4096

typedef struct task {
    uint32_t pid;
    uint32_t state;
    uint32_t priority;
    uint32_t time;
    uint32_t stack_start;
    task_entry_t entry;
    registers_t regs;
    struct task *next;
} task_t;

typedef struct process_queue {
    task_t *current_task;
    task_t *head;
    task_t *tail;
    bool active;
    uint32_t time_share_remaining;
    bool first_run;
} process_queue_t;

typedef enum {
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_TERMINATED
} task_state_t;

void activate_scheduler();
void scheduler_timer_callback(registers_t *regs);
void add_task(task_entry_t entry);

#endif
