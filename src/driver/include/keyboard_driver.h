// Copyright 2025 Morgan Nilsson
#ifndef SRC_DRIVER_INCLUDE_KEYBOARD_DRIVER_H_
#define SRC_DRIVER_INCLUDE_KEYBOARD_DRIVER_H_

#include "../../libs/include/stdtypes.h"
#include "../../driver/include/keyboard.h"


#define IRQ1 33

void init_keyboard(void);

VirtualKey translate_PS2_scancode_to_Vkey(uint8_t scancode);
VirtualKey get_Vkey(void);
char get_char(void);

#endif