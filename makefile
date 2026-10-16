ASM=nasm
ASM_FLAGS=-g
ASM32_FLAGS=-f elf32
ASM64_FLAGS=-f elf64

CC=i686-elf-gcc
CFLAGS=-ffreestanding -nostdlib -m32 -g -Iinclude -fno-pic -fno-pie -fno-stack-protector -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes -Wconversion -Wshadow -fno-pic -fno-pie -fno-stack-protector -fno-omit-frame-pointer

LD = i686-elf-ld
LD_FLAGS = -m elf_i386 --oformat binary

QEMU_FLAGS=-cpu qemu32 -m 4G

SRC_DIR=src

BOOTLOADER_DIR=$(SRC_DIR)/bootloader
KERNEL_DIR=$(SRC_DIR)/kernel
DRIVERS_DIR=$(SRC_DIR)/driver
LIBS_DIR=$(SRC_DIR)/libs

INCLUDE_DIR=include
MEMORY_LAYOUT_INC=$(BUILD_DIR)/kernel/memory_layout.inc

BUILD_DIR=build

BOOT_IMG=$(BUILD_DIR)/boot.img
KERNEL_BIN=$(KERNEL_BUILD_DIR)/kernel.bin

BUILD_TOOLS_DIR=buildTools
LD_SRC_FILE=$(BUILD_TOOLS_DIR)/linker.ld
LD_FILE=$(BUILD_DIR)/linker.ld


BOOTLOADER_BUILD_DIR=$(BUILD_DIR)/bootloader
BOOTLOADER_1_BIN=$(BOOTLOADER_BUILD_DIR)/boot1.bin
BOOTLOADER_2_BIN=$(BOOTLOADER_BUILD_DIR)/boot2.bin

KERNEL_BUILD_DIR=$(BUILD_DIR)/kernel

LIBS_BUILD_DIR=$(BUILD_DIR)/libs

DRIVER_BUILD_DIR=$(BUILD_DIR)/driver

sectors = $(shell echo $$(( ($$(wc -c < $(1)) + 511) / 512 )))

KERNEL_SECTORS = $(call sectors,$(KERNEL_BIN))
BOOTLOADER_2_SECTORS = $(call sectors,$(BOOTLOADER_2_BIN))
KERNEL_LBA     = $(shell echo $$(( 1 + $(BOOTLOADER_2_SECTORS) )))
TOTAL_SECTORS  = $(shell echo $$(( $(KERNEL_LBA) + $(KERNEL_SECTORS) )))
# keep the image at least 8MB so the BIOS sees the same disk geometry
MIN_IMG_SECTORS = 16384
IMG_SECTORS    = $(shell echo $$(( $(TOTAL_SECTORS) > $(MIN_IMG_SECTORS) ? $(TOTAL_SECTORS) : $(MIN_IMG_SECTORS) )))

# kernel-entry must be first in the list or the kernel will not have the correct entry point
KERNEL_ASM_FILES=$(KERNEL_DIR)/kernel-entry.asm $(KERNEL_DIR)/interrupt.asm
KERNEL_C_FILES=$(wildcard $(KERNEL_DIR)/*.c)
DRIVER_FILES=$(wildcard $(DRIVERS_DIR)/*.c)
LIBS_FILES=$(wildcard $(LIBS_DIR)/*.c)

KERNEL_OBJ_ASM_FILES=$(patsubst $(KERNEL_DIR)/%.asm,$(KERNEL_BUILD_DIR)/%.o,$(KERNEL_ASM_FILES))
KERNEL_OBJ_C_FILES=$(patsubst $(KERNEL_DIR)/%.c,$(KERNEL_BUILD_DIR)/%.o,$(KERNEL_C_FILES))
DRIVER_OBJ_FILES=$(patsubst $(DRIVERS_DIR)/%.c,$(DRIVER_BUILD_DIR)/%.o,$(DRIVER_FILES))
LIBS_OBJ_FILES=$(patsubst $(LIBS_DIR)/%.c,$(LIBS_BUILD_DIR)/%.o,$(LIBS_FILES))

# write the boot image together
$(BOOT_IMG): $(BOOTLOADER_1_BIN) $(BOOTLOADER_2_BIN) $(KERNEL_BIN)
	# boot image large enough for stage 1, stage 2 and the kernel
	dd if=/dev/zero of=$(BOOT_IMG) bs=512 count=$(IMG_SECTORS)

	# write stage 1
	dd if=$(BOOTLOADER_1_BIN) of=$(BOOT_IMG) bs=512 seek=0 conv=notrunc

	# write stage 2
	dd if=$(BOOTLOADER_2_BIN) of=$(BOOT_IMG) bs=512 seek=1 conv=notrunc

	# write kernel
	dd if=$(KERNEL_BIN) of=$(BOOT_IMG) bs=512 seek=$(KERNEL_LBA) conv=notrunc

# boot1 embeds the size of boot2, so boot2 must be built first
$(BOOTLOADER_1_BIN): $(BOOTLOADER_DIR)/boot1.asm $(BOOTLOADER_2_BIN) | $(BUILD_DIR) $(BOOTLOADER_BUILD_DIR)
	$(ASM) $(ASM_FLAGS) -DBOOT2_SECTORS=$(BOOTLOADER_2_SECTORS) $< -f bin -o $@

$(MEMORY_LAYOUT_INC): $(INCLUDE_DIR)/kernel/memory_layout.h | $(BUILD_DIR) $(KERNEL_BUILD_DIR)
			{ echo "; GENERATED FILE - DO NOT EDIT. Source: $<"; \
	  sed -nE 's/^[[:space:]]*#[[:space:]]*define[[:space:]]+([A-Za-z_][A-Za-z0-9_]*)[[:space:]]+(.+)$$/%define \1 \2/p' $<; } > $@
	
# boot2 embeds the size of the kernel, so the kernel must be built first
$(BOOTLOADER_2_BIN): $(BOOTLOADER_DIR)/boot2.asm $(MEMORY_LAYOUT_INC) $(KERNEL_BIN) | $(BUILD_DIR) $(BOOTLOADER_BUILD_DIR)
	$(ASM) $(ASM_FLAGS) -DKERNEL_SECTORS=$(KERNEL_SECTORS) $< -f bin -o $@

$(LD_FILE): $(LD_SRC_FILE) | $(BUILD_DIR)
	$(CC) -E -P -x c -include include/kernel/memory_layout.h $< -o $@

$(KERNEL_BIN): $(KERNEL_OBJ_ASM_FILES) $(KERNEL_OBJ_C_FILES) $(DRIVER_OBJ_FILES) $(LIBS_OBJ_FILES) $(LD_FILE) | $(BUILD_DIR) $(KERNEL_BUILD_DIR)
	$(LD) $(LD_FLAGS) -T $(LD_FILE) -o $(KERNEL_BIN) $(KERNEL_OBJ_ASM_FILES) $(KERNEL_OBJ_C_FILES) $(DRIVER_OBJ_FILES) $(LIBS_OBJ_FILES)

# build the kernel asm files
$(KERNEL_OBJ_ASM_FILES): $(KERNEL_BUILD_DIR) $(KERNEL_ASM_FILES)
	$(foreach file, $(KERNEL_ASM_FILES), $(ASM) $(ASM32_FLAGS) $(file) -o $(KERNEL_BUILD_DIR)/$(notdir $(file:.asm=.o));)

# build the kernel c files
$(KERNEL_OBJ_C_FILES): $(KERNEL_BUILD_DIR) $(KERNEL_C_FILES)
	$(foreach file, $(KERNEL_C_FILES), $(CC) $(CFLAGS) -c $(file) -o $(KERNEL_BUILD_DIR)/$(notdir $(file:.c=.o));)

# build the driver c files
$(DRIVER_OBJ_FILES): $(DRIVER_BUILD_DIR) $(DRIVER_FILES)
	$(foreach file, $(DRIVER_FILES), $(CC) $(CFLAGS) -c $(file) -o $(DRIVER_BUILD_DIR)/$(notdir $(file:.c=.o));)

# build the libs c files
$(LIBS_OBJ_FILES): $(LIBS_BUILD_DIR) $(LIBS_FILES)
	$(foreach file, $(LIBS_FILES), $(CC) $(CFLAGS) -c $(file) -o $(LIBS_BUILD_DIR)/$(notdir $(file:.c=.o));)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOTLOADER_BUILD_DIR): $(BUILD_DIR)
	mkdir -p $(BOOTLOADER_BUILD_DIR)

$(KERNEL_BUILD_DIR): $(BUILD_DIR)
	mkdir -p $(KERNEL_BUILD_DIR)

$(LIBS_BUILD_DIR): $(BUILD_DIR)
	mkdir -p $(LIBS_BUILD_DIR)

$(DRIVER_BUILD_DIR): $(BUILD_DIR)
	mkdir -p $(DRIVER_BUILD_DIR)

run:
	qemu-system-i386 $(QEMU_FLAGS) -drive file=$(BOOT_IMG),format=raw

debug:
	qemu-system-i386 $(QEMU_FLAGS) -drive file=$(BOOT_IMG),format=raw -s -S

check:
	cppcheck --enable=all --inconclusive --std=c99 --language=c --platform=unix32 --suppress=missingIncludeSystem $(SRC_DIR)
	cpplint --recursive $(SRC_DIR)

clean:
	rm -r $(BUILD_DIR)/*
