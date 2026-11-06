ASM=nasm
ASM_FLAGS=-g

QEMU_FLAGS=-cpu qemu32 -m 4G

SRC_DIR=src

BOOTLOADER_DIR=$(SRC_DIR)/bootloader
KERNEL_DIR=$(SRC_DIR)/kernel
DRIVERS_DIR=$(SRC_DIR)/driver
LIBS_DIR=$(SRC_DIR)/libs

INCLUDE_DIR=include
MEMORY_LAYOUT_H=$(INCLUDE_DIR)/kernel/memory_layout.h
MEMORY_LAYOUT_INC=$(BUILD_DIR)/kernel/memory_layout.inc

BUILD_DIR=build

BOOT_IMG=$(BUILD_DIR)/boot.img
KERNEL_BIN=$(KERNEL_BUILD_DIR)/kernel.bin

BOOTLOADER_BUILD_DIR=$(BUILD_DIR)/bootloader
BOOTLOADER_1_BIN=$(BOOTLOADER_BUILD_DIR)/boot1.bin
BOOTLOADER_2_BIN=$(BOOTLOADER_BUILD_DIR)/boot2.bin

KERNEL_BUILD_DIR=$(BUILD_DIR)/kernel

CMAKE_BUILD_DIR = build-cmake
KERNEL_BIN = $(CMAKE_BUILD_DIR)/kernel.bin

all: $(BOOT_IMG)

$(CMAKE_BUILD_DIR)/CMakeCache.txt:
	cmake -S . -B $(CMAKE_BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=cmake/i686-elf.cmake

sectors = $(shell echo $$(( ($$(wc -c < $(1)) + 511) / 512 )))

# read a #define value from memory_layout.h, failing if it is missing
layout = $(or $(shell sed -nE 's/^[[:space:]]*\#[[:space:]]*define[[:space:]]+$(1)[[:space:]]+([^[:space:]]+).*/\1/p' $(MEMORY_LAYOUT_H)),$(error $(1) not found in $(MEMORY_LAYOUT_H)))

# layout values the build checks against
PHY_BOOTLOADER_1_START            := $(call layout,PHY_BOOTLOADER_1_START)
PHY_BOOTLOADER_1_END              := $(call layout,PHY_BOOTLOADER_1_END)
PHY_BOOTLOADER_2_START            := $(call layout,PHY_BOOTLOADER_2_START)
PHY_BOOTLOADER_2_END              := $(call layout,PHY_BOOTLOADER_2_END)
PHY_KERNEL_START                  := $(call layout,PHY_KERNEL_START)
PHY_EXTENDED_BIOS_DATA_AREA_START := $(call layout,PHY_EXTENDED_BIOS_DATA_AREA_START)

BOOTLOADER_1_SIZE        := $(shell echo $$(( $(PHY_BOOTLOADER_1_END) - $(PHY_BOOTLOADER_1_START) + 1 )))
BOOTLOADER_2_MAX_SECTORS := $(shell echo $$(( ($(PHY_BOOTLOADER_2_END) - $(PHY_BOOTLOADER_2_START) + 1) / 512 )))
KERNEL_MAX_BYTES         := $(shell echo $$(( $(PHY_KERNEL_START) < $(PHY_EXTENDED_BIOS_DATA_AREA_START) ? \
                                $(PHY_EXTENDED_BIOS_DATA_AREA_START) - $(PHY_KERNEL_START) : 0 )))

# fail and delete $(1) if it is larger than $(2) bytes
check_size = @s=$$(wc -c < $(1) | tr -d ' '); \
	if [ $(2) -ne 0 ] && [ $$s -gt $(2) ]; then \
		echo "Error: $(1) is $$s bytes, max $(2) ($(3))"; rm -f $(1); exit 1; \
	fi

KERNEL_SECTORS = $(call sectors,$(KERNEL_BIN))
BOOTLOADER_2_SECTORS = $(call sectors,$(BOOTLOADER_2_BIN))
KERNEL_LBA     = $(shell echo $$(( 1 + $(BOOTLOADER_2_SECTORS) )))
TOTAL_SECTORS  = $(shell echo $$(( $(KERNEL_LBA) + $(KERNEL_SECTORS) )))
# keep the image at least 8MB so the BIOS sees the same disk geometry
MIN_IMG_SECTORS = 16384
IMG_SECTORS    = $(shell echo $$(( $(TOTAL_SECTORS) > $(MIN_IMG_SECTORS) ? $(TOTAL_SECTORS) : $(MIN_IMG_SECTORS) )))

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
$(BOOTLOADER_1_BIN): $(BOOTLOADER_DIR)/boot1.asm $(BOOTLOADER_2_BIN) $(MEMORY_LAYOUT_INC) | $(BUILD_DIR) $(BOOTLOADER_BUILD_DIR)
	$(ASM) $(ASM_FLAGS) -DBOOT2_SECTORS=$(BOOTLOADER_2_SECTORS) $< -f bin -o $@
	$(call check_size,$@,$(BOOTLOADER_1_SIZE),PHY_BOOTLOADER_1_START..PHY_BOOTLOADER_1_END)

$(MEMORY_LAYOUT_INC): $(MEMORY_LAYOUT_H) | $(BUILD_DIR) $(KERNEL_BUILD_DIR)
			{ echo "; GENERATED FILE - DO NOT EDIT. Source: $<"; \
	  sed -nE 's/^[[:space:]]*#[[:space:]]*define[[:space:]]+([A-Za-z_][A-Za-z0-9_]*)[[:space:]]+(.+)$$/%define \1 \2/p' $<; } > $@
	
# boot2 embeds the size of the kernel, so the kernel must be built first
$(BOOTLOADER_2_BIN): $(BOOTLOADER_DIR)/boot2.asm $(MEMORY_LAYOUT_INC) $(KERNEL_BIN) | $(BUILD_DIR) $(BOOTLOADER_BUILD_DIR)
	$(ASM) $(ASM_FLAGS) -DKERNEL_SECTORS=$(KERNEL_SECTORS) $< -f bin -o $@
	$(call check_size,$@,$$(( $(BOOTLOADER_2_MAX_SECTORS) * 512 )),PHY_BOOTLOADER_2_START..PHY_BOOTLOADER_2_END)

$(KERNEL_BIN): FORCE | $(CMAKE_BUILD_DIR)/CMakeCache.txt
	cmake --build $(CMAKE_BUILD_DIR) --target kernel
	$(call check_size,$@,$(KERNEL_MAX_BYTES),PHY_KERNEL_START..PHY_EXTENDED_BIOS_DATA_AREA_START)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOTLOADER_BUILD_DIR): $(BUILD_DIR)
	mkdir -p $(BOOTLOADER_BUILD_DIR)

$(KERNEL_BUILD_DIR): $(BUILD_DIR)
	mkdir -p $(KERNEL_BUILD_DIR)

FORCE:

.PHONY: all FORCE run debug check clean gdb

run: $(BOOT_IMG)
	qemu-system-i386 $(QEMU_FLAGS) -drive file=$(BOOT_IMG),format=raw

debug: $(BOOT_IMG)
	qemu-system-i386 $(QEMU_FLAGS) -drive file=$(BOOT_IMG),format=raw -s -S

gdb:
	gdb $(CMAKE_BUILD_DIR)/kernel.elf -ex "target remote :1234" -ex "break _start" -ex "continue"

check:
	cppcheck --enable=all --inconclusive --std=c23 --language=c --platform=unix32 --suppress=missingIncludeSystem $(SRC_DIR)
	cpplint --recursive $(SRC_DIR)

clean:
	rm -rf $(BUILD_DIR)/* $(CMAKE_BUILD_DIR)
