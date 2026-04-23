SRC_DIR = src
BOOT_BIN = $(BUILD_DIR)/boot.bin

.PHONY: all clean

all: $(BOOT_BIN)

$(BOOT_BIN): $(SRC_DIR)/boot.asm
	@echo "  ASM     $<"
	@mkdir -p $(BUILD_DIR)
	@$(NASM) -f bin $< -o $@
