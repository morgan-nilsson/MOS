# Builds the kernel
cmake_minimum_required(VERSION 3.20)
project(MOS LANGUAGES C ASM_NASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_program(ELF_LD i686-elf-ld REQUIRED)
set(LAYOUT_H ${CMAKE_SOURCE_DIR}/include/kernel/memory_layout.h) # Memory layout file

add_custom_command(OUTPUT linker.ld
    COMMAND ${CMAKE_C_COMPILER} -E -P -x c -include ${LAYOUT_H}
        ${CMAKE_SOURCE_DIR}/buildTools/linker.ld -o linker.ld
    DEPENDS buildTools/linker.ld ${LAYOUT_H})
add_custom_target(linker_script DEPENDS linker.ld)

file(GLOB KERNEL_C CONFIGURE_DEPENDS src/kernel/*.c src/driver/*.c src/libs/*.c)
add_executable(kernel
    src/kernel/kernel-entry.asm
    src/kernel/interrupt.asm
    ${KERNEL_C})
target_include_directories(kernel PRIVATE include)
set(KERNEL_CFLAGS
    -ffreestanding -nostdlib -m32 -g -fno-pic -fno-pie -fno-stack-protector
    -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes -Wconversion -Wshadow
    -fno-omit-frame-pointer)
target_compile_options(kernel PRIVATE "$<$<COMPILE_LANGUAGE:C>:${KERNEL_CFLAGS}>")
# DWARF debug info for the kernel asm too, so gdb can step through interrupt.asm
target_compile_options(kernel PRIVATE "$<$<COMPILE_LANGUAGE:ASM_NASM>:-g;-Fdwarf>")

# link to an ELF (keeps symbols for gdb), then strip to the flat binary the bootloader loads
set_target_properties(kernel PROPERTIES SUFFIX .elf
    LINK_DEPENDS ${CMAKE_BINARY_DIR}/linker.ld)
add_dependencies(kernel linker_script)
set(CMAKE_C_LINK_EXECUTABLE
    "${ELF_LD} -m elf_i386 -T ${CMAKE_BINARY_DIR}/linker.ld <OBJECTS> -o <TARGET>")

find_program(ELF_OBJCOPY i686-elf-objcopy REQUIRED)
add_custom_command(TARGET kernel POST_BUILD
    COMMAND ${ELF_OBJCOPY} -O binary $<TARGET_FILE:kernel> ${CMAKE_BINARY_DIR}/kernel.bin
    BYPRODUCTS ${CMAKE_BINARY_DIR}/kernel.bin)