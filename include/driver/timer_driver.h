// Copyright 2025 Morgan Nilsson
#ifndef SRC_DRIVER_INCLUDE_TIMER_H_
#define SRC_DRIVER_INCLUDE_TIMER_H_

#include "libs/stdtypes.h"
#include "kernel/isr.h"

void init_timer(uint32_t frequency);
uint32_t get_tick_count();
uint8_t get_pic_mask();

#endif