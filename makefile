ASM=nasm
ASM_FLAGS=-g 
#-f elf32
SRC_DIR=src
BUILD_DIR=build
BOOTLOADER_DIR=$(SRC_DIR)/bootloader
KERNEL_DIR=$(SRC_DIR)/kernel

BOOT_IMG=$(BUILD_DIR)/boot.img
STAGE1_BOOT=$(BUILD_DIR)/boot1.bin
STAGE2_BOOT=$(BUILD_DIR)/boot2.bin

$(BOOT_IMG): $(BUILD_DIR)/boot1.bin $(BUILD_DIR)/boot2.bin
	# 1MB boot image
	dd if=/dev/zero of=$(BOOT_IMG) bs=1M count=1

	# write stage 1
	dd if=$(STAGE1_BOOT) of=$(BOOT_IMG) bs=512 seek=0 conv=notrunc

	# write stage 2
	dd if=$(STAGE2_BOOT) of=$(BOOT_IMG) bs=512 seek=1 conv=notrunc
	
$(STAGE1_BOOT): $(BOOTLOADER_DIR)/boot1.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) $(ASM_FLAGS) $(BOOTLOADER_DIR)/boot1.asm -f bin -o $(STAGE1_BOOT)
	
$(STAGE2_BOOT): $(BOOTLOADER_DIR)/boot2.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) $(ASM_FLAGS) $(BOOTLOADER_DIR)/boot2.asm -f bin -o $(STAGE2_BOOT)

run:
	qemu-system-i386 -cpu qemu32 -drive file=$(BOOT_IMG),format=raw

debug:
	qemu-system-i386 -cpu qemu32 -drive file=$(BOOT_IMG),format=raw -s -S

clean:
	rm $(BUILD_DIR)/*
