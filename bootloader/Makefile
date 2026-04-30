SRC_DIR = src
BOOT_BIN = $(BUILD_DIR)/boot.bin

BOOT_ASM = $(SRC_DIR)/boot.asm
OBJS := $(SRC_DIR)/boot.asm $(SRC_DIR)/memory.asm \
				$(SRC_DIR)/disk.asm $(SRC_DIR)/gdt.asm

.PHONY: all clean

all: $(BOOT_BIN)

$(BOOT_BIN): $(OBJS)
	@echo "  ASM     bootloader/$<"
	@mkdir -p $(BUILD_DIR)
	@$(NASM) -f bin $(BOOT_ASM) -o $@

clean:
	@echo "  CLN     bootloader"
	@rm -f $(BOOT_BIN)
