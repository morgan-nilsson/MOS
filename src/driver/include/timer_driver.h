// Copyright 2025 Morgan Nilsson
#ifndef SRC_DRIVER_INCLUDE_TIMER_H_
#define SRC_DRIVER_INCLUDE_TIMER_H_

#include "../../libs/include/stdtypes.h"
#include "../../kernel/include/isr.h"

void init_timer(uint32_t frequency);
uint32_t get_tick_count();

#endif