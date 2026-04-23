MEM = 4M
BUILD_DIR = $(CURDIR)/build
IMG = lanex.img

CC   = i686-elf-gcc
AS   = i686-elf-as
AR   = i686-elf-ar
NASM = nasm

CFLAGS  = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -ffreestanding -O2 -nostdlib

ifeq ($(deb), 1)
    CC      = gcc -m32
    AS      = as --32
    AR      = ar
    CFLAGS  += -fno-stack-protector -fno-pic
    LDFLAGS += -m32
endif

export CC AS AR NASM CFLAGS LDFLAGS BUILD_DIR

.PHONY: all clean run libc kernel

all: libc kernel
	@mkdir -p $(BUILD_DIR)
	$(NASM) -f bin kernel/src/boot.asm -o $(BUILD_DIR)/boot.bin
	cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin > $(IMG)
	truncate -s 1440k $(IMG)

libc:
	@$(MAKE) -C libc

kernel: libc
	@$(MAKE) -C kernel

run: all
	qemu-system-i386 -m $(MEM) -drive file=$(IMG),format=raw,index=0,media=disk

clean:
	@$(MAKE) -C libc clean
	@$(MAKE) -C kernel clean
	rm -rf $(BUILD_DIR) $(IMG)
