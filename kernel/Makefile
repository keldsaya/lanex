CC = i686-elf-gcc
AS = i686-elf-as
NASM = nasm

PROJECT_ROOT := $(abspath $(CURDIR)/..)
SRC_DIR = src
BUILD_DIR ?= $(PROJECT_ROOT)/build
LIBC_DIR = $(PROJECT_ROOT)/libc

CFLAGS = -I$(LIBC_DIR)/include -Iinclude -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T ../linker.ld -ffreestanding -O2 -nostdlib -lgcc -Wl,--oformat,binary

LIBC_LIB = $(BUILD_DIR)/libc.a

KERNEL_OBJS = entry.o main.o io.o \
							interrupts.o idt.o pic.o \
							vga.o pit.o tty.o \
							pmm.o keyboard.o cursor.o \
							sh.o messages.o ata.o
              
OBJECTS = $(addprefix $(BUILD_DIR)/, $(KERNEL_OBJS))
KERNEL = $(BUILD_DIR)/kernel.bin

.PHONY: all clean

all: $(KERNEL)

$(KERNEL): $(OBJECTS) $(LIBC_LIB)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBC_LIB)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(BUILD_DIR)
	$(AS) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(BUILD_DIR)
	$(NASM) -f elf32 $< -o $@

clean:
	rm -f $(OBJECTS) $(KERNEL)
