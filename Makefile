CC = i686-elf-gcc
AS = i686-elf-as
NASM = nasm
CFLAGS = -Iinclude -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib -lgcc -Wl,--oformat,binary

BUILD_DIR = build
SRC_DIR = src

OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/io.o $(BUILD_DIR)/vga.o $(BUILD_DIR)/tty.o $(BUILD_DIR)/cursor.o
OUT_BIN = $(BUILD_DIR)/kernel.bin
IMG = lanex.img

all: $(IMG)

$(IMG): $(OUT_BIN)
	$(NASM) -f bin $(SRC_DIR)/boot.asm -o $(BUILD_DIR)/boot.bin
	cat $(BUILD_DIR)/boot.bin $(OUT_BIN) > $(IMG)
	truncate -s 1440k $(IMG)

$(OUT_BIN): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	mkdir -p $(BUILD_DIR)
	$(AS) $< -o $@

run:
	qemu-system-i386 -drive file=$(IMG),format=raw,index=0,media=disk

clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.bin $(IMG)

.PHONY: all clean run
