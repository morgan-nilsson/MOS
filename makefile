ASM=nasm
ASM32_FLAGS=-f elf32

LD = i686-elf-ld
LD_FLAGS = -m elf_i386 -T $(LD_FILE) --oformat binary

ZIG=zig build-obj
ZIG_FLAGS= -target x86-freestanding
RELEASE=-O Debug

QEMU_FLAGS=-cpu qemu32 -m 4G

BUILD_DIR=zig-out/bin
SRC_DIR=src
BOOTLOADER_DIR=$(SRC_DIR)/bootloader
KERNEL_DIR=$(SRC_DIR)/kernel
DRIVERS_DIR=$(SRC_DIR)/driver
LIBS_DIR=$(SRC_DIR)/libs

BOOT_IMG=$(BUILD_DIR)/boot.img
KERNEL_BIN=$(BUILD_DIR)/kernel.bin

LD_FILE=$(SRC_DIR)/buildTools/linker.ld

BOOTLOADER_BUILD_DIR=$(BUILD_DIR)/bootloader
BOOTLOADER_1_BIN=$(BOOTLOADER_BUILD_DIR)/boot1.bin
BOOTLOADER_2_BIN=$(BOOTLOADER_BUILD_DIR)/boot2.bin

KERNEL_BUILD_DIR=$(BUILD_DIR)/kernel

LIBS_BUILD_DIR=$(BUILD_DIR)/libs

DRIVER_BUILD_DIR=$(BUILD_DIR)/driver

KERNEL_ASM_FILES=$(KERNEL_DIR)/interrupt.asm $(KERNEL_DIR)/kernel-entry.asm

KERNEL_OBJ_ASM_FILES=$(patsubst $(KERNEL_DIR)/%.asm,$(KERNEL_BUILD_DIR)/%.o,$(KERNEL_ASM_FILES))

# write the boot image together
$(BOOT_IMG): $(BOOTLOADER_1_BIN) $(BOOTLOADER_2_BIN) $(KERNEL_BIN)
	# 8MB boot image
	dd if=/dev/zero of=$(BOOT_IMG) bs=8M count=1

	# write stage 1
	dd if=$(BOOTLOADER_1_BIN) of=$(BOOT_IMG) bs=512 seek=0 conv=notrunc

	# write stage 2
	dd if=$(BOOTLOADER_2_BIN) of=$(BOOT_IMG) bs=512 seek=1 conv=notrunc count=2

	# write kernel
	dd if=$(BUILD_DIR)/kernel.out of=$(BOOT_IMG) bs=512 seek=3 conv=notrunc count=2048

$(BOOTLOADER_1_BIN): $(BOOTLOADER_DIR)/boot1.asm | $(BUILD_DIR) $(BOOTLOADER_BUILD_DIR)
	$(ASM) $(BOOTLOADER_DIR)/boot1.asm -f bin -o $(BOOTLOADER_1_BIN)

$(BOOTLOADER_2_BIN): $(BOOTLOADER_DIR)/boot2.asm | $(BUILD_DIR) $(BOOTLOADER_BUILD_DIR)
	$(ASM) $(BOOTLOADER_DIR)/boot2.asm -f bin -o $(BOOTLOADER_2_BIN)

$(KERNEL_BIN): $(KERNEL_OBJ_ASM_FILES) | $(BUILD_DIR) $(KERNEL_BUILD_DIR)
	zig build

$(KERNEL_OBJ_ASM_FILES): $(KERNEL_BUILD_DIR) $(KERNEL_ASM_FILES)
	$(foreach file, $(KERNEL_ASM_FILES), $(ASM) $(ASM32_FLAGS) $(file) -o $(KERNEL_BUILD_DIR)/$(notdir $(file:.asm=.o));)

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

clean:
	rm -r $(BUILD_DIR)/*