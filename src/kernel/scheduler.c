// Copyright 2025 Morgan Nilsson

#include "kernel/scheduler.h"

#include "libs/stdtypes.h"
#include "libs/stdlib.h"
#include "driver/vga_driver.h"
#include "libs/assert.h"
#include "kernel/isr.h"

uint32_t process_id = 0;

process_queue_t process_queue;

void activate_scheduler() {

    if (process_queue.current_task == NULL) {

        write_string("No tasks to schedule this call will crash\n");
        while (1);

    }

    process_queue.time_share_remaining = TIME_SEGMENT_MS;

    process_queue.active = true;
    process_queue.first_run = true;

}

void switch_task(registers_t *src_regs) {

    asm volatile (
        "cli\n" // disable interrupts
    );

    // send EOI to the PIC
    asm volatile (
        "movb $0x20, %al\n"
        "outb %al, $0x20\n"
    );

    // copy over the stored return registers
    memcpy(src_regs, &process_queue.current_task->regs, sizeof(registers_t));

    asm volatile (
        "sti\n" // enable interrupts
    );

}

void scheduler_timer_callback(registers_t *src_regs) {

    if (process_queue.active == false) {

        return;

    }

    --(process_queue.time_share_remaining);

    // time has run out thus switch to next task
    if (process_queue.time_share_remaining < 1) {

        if (process_queue.current_task == process_queue.current_task->next) {

            return;

        }

        if (process_queue.first_run == false) {
            // save the current task
            memcpy(&process_queue.current_task->regs, src_regs, sizeof(registers_t));
        } else {
            process_queue.first_run = false;
        }

        process_queue.time_share_remaining = TIME_SEGMENT_MS;

        process_queue.current_task = process_queue.current_task->next;

        switch_task(src_regs);

    }

}

void add_task(task_entry_t entry) {

    task_t *task = mem_alloc(sizeof(task_t));
    if (task == NULL) {

        write_string("Error allocating memeory from heap\n");
        return;

    }

    registers_t *regs = &task->regs;

    uint32_t *stack_base = mem_alloc(TASK_STACK_SIZE);
    if (stack_base == NULL) {

        write_string("Error allocating memeory from heap\n");
        return;

    }

    regs->esp = (uint32_t)stack_base + TASK_STACK_SIZE - 4;
    regs->ebp = (uint32_t)stack_base;

    regs->edi = 0;
    regs->esi = 0;
    regs->ebx = 0;
    regs->edx = 0;
    regs->ecx = 0;
    regs->eax = 0;

    regs->cs = 0x08; // kernel code segment
    regs->ds = 0x10; // kernel data segment

    regs->eip = (uint32_t)entry;
    regs->eflags = 0x202;


    task->entry = entry;

    if (process_queue.current_task == NULL) {

        task->next = task;
        
        process_queue.head = task;
        process_queue.tail = task;
        process_queue.current_task = task;

    } else {
        // put the new task at the end
        task->next = process_queue.head;

        process_queue.tail->next = task;
        process_queue.tail = task;

    }
}
