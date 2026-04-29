SRC_DIR = src
BOOT_BIN = $(BUILD_DIR)/boot.bin

OBJS := $(SRC_DIR)/boot.asm $(SRC_DIR)/memory.asm \
				$(SRC_DIR)/disk.asm $(SRC_DIR)/gdt.asm

.PHONY: all clean

all: $(BOOT_BIN)

$(BOOT_BIN): $(OBJS)
	@echo "  ASM     $<"
	@mkdir -p $(BUILD_DIR)
	@$(NASM) -f bin $< -o $@
