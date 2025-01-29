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
LIBS_OBJ_FILE_NAMES=string.o vga_driver.o ctype.o keyboard_driver.o stdlib.o math.o
LIBS_OBJ_FILES=$(addprefix $(BUILD_DIR)/, $(LIBS_OBJ_FILE_NAMES))

BUILD_TOOLS=$(SRC_DIR)/buildTools
LD_FILE=$(BUILD_TOOLS)/linker.ld

$(BOOT_IMG): $(BUILD_DIR)/boot1.bin $(BUILD_DIR)/boot2.bin $(BUILD_DIR)/kernel.bin
	# 8MB boot image
	dd if=/dev/zero of=$(BOOT_IMG) bs=8M count=1

	# write stage 1
	dd if=$(STAGE1_BOOT) of=$(BOOT_IMG) bs=512 seek=0 conv=notrunc

	# write stage 2
	dd if=$(STAGE2_BOOT) of=$(BOOT_IMG) bs=512 seek=1 conv=notrunc

	# write kernel
	dd if=$(KERNEL_BIN) of=$(BOOT_IMG) bs=512 seek=3 conv=notrunc count=100
	
$(STAGE1_BOOT): $(BOOTLOADER_DIR)/boot1.asm | $(BUILD_DIR)
	$(ASM) $(ASM_FLAGS) $(BOOTLOADER_DIR)/boot1.asm -f bin -o $(STAGE1_BOOT)
	
$(STAGE2_BOOT): $(BOOTLOADER_DIR)/boot2.asm | $(BUILD_DIR)
	$(ASM) $(ASM_FLAGS) $(BOOTLOADER_DIR)/boot2.asm -f bin -o $(STAGE2_BOOT)

$(KERNEL_BIN): $(KERNEL_DIR)/kernel.c $(LIBS_OBJ_FILES) $(KERNEL_DIR)/kernel-entry.asm | $(BUILD_DIR) 
	nasm $(ASM32_FLAGS) $(KERNEL_DIR)/kernel-entry.asm -o $(BUILD_DIR)/kernel-entry.o
	$(CC) $(CFLAGS) -c $(KERNEL_DIR)/kernel.c -o $(BUILD_DIR)/kernel.o
	$(LD) -m elf_i386 -T $(LD_FILE) -o $(KERNEL_BIN) $(BUILD_DIR)/kernel-entry.o $(BUILD_DIR)/kernel.o $(LIBS_OBJ_FILES) --oformat binary

$(BUILD_DIR)/string.o: $(LIBS_SRC_DIR)/string.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(LIBS_SRC_DIR)/string.c -o $(BUILD_DIR)/string.o

$(BUILD_DIR)/vga_driver.o: $(LIBS_SRC_DIR)/vga_driver.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(LIBS_SRC_DIR)/vga_driver.c -o $(BUILD_DIR)/vga_driver.o

$(BUILD_DIR)/ctype.o: $(LIBS_SRC_DIR)/ctype.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(LIBS_SRC_DIR)/ctype.c -o $(BUILD_DIR)/ctype.o

$(BUILD_DIR)/keyboard_driver.o: $(LIBS_SRC_DIR)/keyboard_driver.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(LIBS_SRC_DIR)/keyboard_driver.c -o $(BUILD_DIR)/keyboard_driver.o

$(BUILD_DIR)/stdlib.o: $(LIBS_SRC_DIR)/stdlib.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(LIBS_SRC_DIR)/stdlib.c -o $(BUILD_DIR)/stdlib.o

$(BUILD_DIR)/math.o: $(LIBS_SRC_DIR)/math.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(LIBS_SRC_DIR)/math.c -o $(BUILD_DIR)/math.o

run:
	qemu-system-i386 -cpu qemu32 -drive file=$(BOOT_IMG),format=raw

debug:
	qemu-system-i386 -cpu qemu32 -drive file=$(BOOT_IMG),format=raw -s -S

clean:
	rm $(BUILD_DIR)/*
