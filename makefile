ASM=nasm
ASM_FLAGS=-g 
ASM32_FLAGS=-f elf32
ASM64_FLAGS=-f elf64

CC=i686-elf-gcc
CFLAGS=-ffreestanding -nostdlib -m32 -g
LD = i686-elf-ld

SRC_DIR=src
BUILD_DIR=build
BOOTLOADER_DIR=$(SRC_DIR)/bootloader
KERNEL_DIR=$(SRC_DIR)/kernel
LIBS_DIR=$(SRC_DIR)/libs
LIBS_INC_DIR=$(LIBS_DIR)/inc
LIBS_SRC_DIR=$(LIBS_DIR)/src

BOOT_IMG=$(BUILD_DIR)/boot.img
STAGE1_BOOT=$(BUILD_DIR)/boot1.bin
STAGE2_BOOT=$(BUILD_DIR)/boot2.bin
KERNEL_BIN=$(BUILD_DIR)/kernel.bin
LIBS_OBJ_FILES=$(BUILD_DIR)/io.o $(BUILD_DIR)/string.o

BUILD_TOOLS=$(SRC_DIR)/buildTools
LD_FILE=$(BUILD_TOOLS)/linker.ld

$(BOOT_IMG): $(BUILD_DIR)/boot1.bin $(BUILD_DIR)/boot2.bin $(BUILD_DIR)/kernel.bin
	# 1MB boot image
	dd if=/dev/zero of=$(BOOT_IMG) bs=1M count=1

	# write stage 1
	dd if=$(STAGE1_BOOT) of=$(BOOT_IMG) bs=512 seek=0 conv=notrunc

	# write stage 2
	dd if=$(STAGE2_BOOT) of=$(BOOT_IMG) bs=512 seek=1 conv=notrunc

	# write kernel
	dd if=$(KERNEL_BIN) of=$(BOOT_IMG) bs=512 seek=3 conv=notrunc
	
$(STAGE1_BOOT): $(BOOTLOADER_DIR)/boot1.asm | $(BUILD_DIR)
	$(ASM) $(ASM_FLAGS) $(BOOTLOADER_DIR)/boot1.asm -f bin -o $(STAGE1_BOOT)
	
$(STAGE2_BOOT): $(BOOTLOADER_DIR)/boot2.asm | $(BUILD_DIR)
	$(ASM) $(ASM_FLAGS) $(BOOTLOADER_DIR)/boot2.asm -f bin -o $(STAGE2_BOOT)

$(KERNEL_BIN): $(KERNEL_DIR)/kernel.c $(LIBS_OBJ_FILES) | $(BUILD_DIR) 
	$(CC) $(CFLAGS) -c $(KERNEL_DIR)/kernel.c -o $(BUILD_DIR)/kernel.o
	$(LD) -T $(LD_FILE) -m elf_i386 -o $(KERNEL_BIN) $(BUILD_DIR)/kernel.o $(LIBS_OBJ_FILES) --oformat binary

$(BUILD_DIR)/io.o: $(LIBS_SRC_DIR)/io.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(LIBS_SRC_DIR)/io.c -o $(BUILD_DIR)/io.o

$(BUILD_DIR)/string.o: $(LIBS_SRC_DIR)/string.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(LIBS_SRC_DIR)/string.c -o $(BUILD_DIR)/string.o

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run:
	qemu-system-i386 -cpu qemu32 -drive file=$(BOOT_IMG),format=raw

debug:
	qemu-system-i386 -cpu qemu32 -drive file=$(BOOT_IMG),format=raw -s -S

clean:
	rm $(BUILD_DIR)/*
