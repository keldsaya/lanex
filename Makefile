LINKER := ./linker.ld
SRC_DIR := ./src
INCLUDE_DIR := ./include
BUILD_DIR := ./build
ISO_DIR := ./isodir
OUT_BIN := $(BUILD_DIR)/kernel.bin
ISO := ./lanex.iso

OBJECTS := $(BUILD_DIR)/boot.o $(BUILD_DIR)/main.o $(BUILD_DIR)/vga.o $(BUILD_DIR)/tty.o

all: $(OUT_BIN)
	cp $(OUT_BIN) $(ISO_DIR)/boot/kernel.bin
	grub-mkrescue -o $(ISO) isodir

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	i686-elf-as $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	i686-elf-gcc -c $< -o $@ -I$(INCLUDE_DIR) -std=gnu99 -ffreestanding -O2 -Wall -Wextra

$(OUT_BIN): $(OBJECTS)
	i686-elf-gcc -T $(LINKER) -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc

run:
	qemu-system-x86_64 -cdrom $(ISO)

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/*.bin
	rm -f $(ISO)

.PHONY: all run clean
