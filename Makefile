LINKER := ./linker.ld
SRC_DIR := ./src
INCLUDE_DIR := ./include
BUILD_DIR := ./build
ISO_DIR := ./isodir
OUT_BIN := $(BUILD_DIR)/kernel.bin
ISO := ./lanex.iso

all:
	i686-elf-as $(SRC_DIR)/boot.s -o $(BUILD_DIR)/boot.o
	i686-elf-gcc -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o -I$(INCLUDE_DIR) -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	i686-elf-gcc -T $(LINKER) -o $(OUT_BIN) -I$(INCLUDE_DIR) -ffreestanding -O2 -nostdlib $(BUILD_DIR)/boot.o $(BUILD_DIR)/main.o -lgcc

	cp $(OUT_BIN) $(ISO_DIR)/boot/kernel.bin
	grub-mkrescue -o $(ISO) isodir

run:
	qemu-system-x86_64 -cdrom $(ISO)

clean:
	rm -f $(BUILD_DIR)/*
	rm -f $(ISO)
