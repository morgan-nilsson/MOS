// Copyright 2025 Morgan Nilsson
#ifndef SRC_DRIVER_INCLUDE_VGA_DRIVER_H_
#define SRC_DRIVER_INCLUDE_VGA_DRIVER_H_

#include "../../libs/include/stdtypes.h"

/**
 * Definitions:
 * - VGA_CTRL_REG: The VGA control register port address.
 * - VGA_DATA_REG: The VGA data register port address.
 * - VGA_OFFSET_LOW: The low byte of the VGA cursor position.
 * - VGA_OFFSET_HIGH: The high byte of the VGA cursor position.
 * - VGA_ADDRESS: The starting address of the VGA video memory.
 * - VGA_MAX_ROWS: The maximum number of rows on the VGA screen.
 * - VGA_MAX_COLUMNS: The maximum number of columns on the VGA screen.
 * - WHITE_ON_BLACK: Attribute byte for white text on a black background.
 */
#define VGA_CTRL_REG 0x3d4
#define VGA_DATA_REG 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

#define VGA_ADDRESS 0xB8000
#define VGA_MAX_ROWS 25
#define VGA_MAX_COLUMNS 80

#define WHITE_ON_BLACK 0x0f

/**
 * @brief Sets the cursor position.
 * 
 * @param offset The position to set the cursor to.
 */
void set_cursor(int offset);

/**
 * @brief Gets the current cursor position.
 * 
 * @return The current cursor position.
 */
int get_cursor(void);

/**
 * @brief Writes a single character to the screen.
 * 
 * @param c The character to write.
 */
void write_char(char c);

/**
 * @brief Writes a null-terminated string to the screen.
 * 
 * @param s The string to write.
 */
void write_string(const char* s);

/**
 * @brief Clears the entire screen.
 */
void clear_screen();

/**
 * @brief Writes a newline character to the screen.
 */
void write_newline();

/**
 * @brief Prints a letter corresponding to the given scancode.
 * 
 * @param scancode The scancode of the letter to print.
 */
void print_letter(uint8_t scancode);

/**
 * @brief Writes a 32-bit hexadecimal value to the screen.
 * 
 * @param n The 32-bit hexadecimal value to write.
 */
void write_hex(uint32_t n);

/**
 * @brief Writes a 16-bit hexadecimal value to the screen.
 * 
 * @param n The 16-bit hexadecimal value to write.
 */
void write_hex_16(uint16_t n);

/**
 * @brief Writes an 8-bit hexadecimal value to the screen.
 * 
 * @param n The 8-bit hexadecimal value to write.
 */
void write_hex_8(uint8_t n);


/**
 * @brief Write an 32 bit integer to the screen
 *
 * @param n the 32 bit integer
 */
void write_int(int32_t n);

/**
 * @brief Dumps the contents of the CPU registers to the screen.
 * 
 * @param regs Pointer to the structure containing the register values.
 */
void dump_registers(registers_t *regs);

/**
 * @brief Dumps a specified number of bytes from the stack to the screen.
 * 
 * @param regs Pointer to the structure containing the register values.
 * @param size The number of bytes to dump from the stack.
 */
void dump_from_stack(registers_t *regs, size_t size);

/**
 * @brief Dump the memory from the given address
 * 
 * @param address The address to dump from
 * @param size The size of the memory to dump
 */
void dump_from_memory_address(uint32_t address, size_t size);

#endif
