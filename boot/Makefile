MAKEFLAGS += -rR -s

SRC_DIR = src
BUILD_DIR ?= ../build
BOOT_BIN = $(BUILD_DIR)/boot.bin

BOOT_SRC = $(SRC_DIR)/boot.asm
DEPS     = $(SRC_DIR)/memory.asm $(SRC_DIR)/disk.asm $(SRC_DIR)/gdt.asm

.PHONY: all clean

all: $(BOOT_BIN)

$(BOOT_BIN): $(BOOT_SRC) $(DEPS)
	@echo "  ASM      boot/src/boot.asm"
	@mkdir -p $(BUILD_DIR)
	@$(NASM) -f bin $< -o $@

clean:
	@echo "  CLN      boot"
	@rm -f $(BOOT_BIN)
