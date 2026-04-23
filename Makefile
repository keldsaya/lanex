MEM           = 4M
BUILD_DIR     = $(CURDIR)/build

REL_BUILD_DIR = build
REL_IMG       = $(REL_BUILD_DIR)/lanex.img

IMG           = $(BUILD_DIR)/lanex.img
BOOT_BIN      = $(BUILD_DIR)/boot.bin
KERNEL_BIN    = $(BUILD_DIR)/kernel.bin

CC   = i686-elf-gcc
AS   = i686-elf-as
AR   = i686-elf-ar
NASM = nasm

CFLAGS      = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS     = -ffreestanding -O2 -nostdlib
MAKEFLAGS += -s --no-print-directory

ifeq ($(deb), 1)
    CC      = gcc -m32
    AS      = as --32
    AR      = ar
    CFLAGS  += -fno-stack-protector -fno-pic
    LDFLAGS += -m32
endif

export CC AS AR NASM CFLAGS LDFLAGS BUILD_DIR

.PHONY: all clean run libc kernel bootloader

all: $(IMG)

$(IMG): libc kernel bootloader
	@echo "  IMG     $(REL_IMG)"
	@cat $(BOOT_BIN) $(KERNEL_BIN) > $(IMG)
	@truncate -s 1440k $(IMG)

libc:
	@$(MAKE) -C libc

kernel:
	@$(MAKE) -C kernel

bootloader:
	@$(MAKE) -C bootloader

run: all
	@echo "  RUN     $(REL_IMG)"
	@qemu-system-i386 -m $(MEM) -drive file=$(IMG),format=raw,index=0,media=disk

clean:
	@echo "  CLN     $(REL_BUILD_DIR)"
	@rm -rf $(BUILD_DIR)
