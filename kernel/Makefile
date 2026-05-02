MAKEFLAGS += -rR -s

LIBC_DIR     = $(abspath ../libc)
DRIVERS_DIR  = $(abspath ../drivers)
FS_DIR       = $(abspath ../fs)
SRC_DIR      = src
INCLUDE_DIR  = $(abspath ../include)

CONFIG_HEADER = $(INCLUDE_DIR)/lanex/config.h

KERNEL_OBJS = entry.asm.o main.c.o io.c.o \
              interrupts.s.o idt.c.o pic.c.o \
              pit.c.o tty.c.o pmm.c.o \
              sh.c.o messages.c.o kprintf.c.o

OBJ_DIR = $(BUILD_DIR)/kernel
OBJS    = $(addprefix $(OBJ_DIR)/, $(KERNEL_OBJS))
BIN     = $(BUILD_DIR)/kernel.bin

LIBC    = $(BUILD_DIR)/libc/libc.a
DRIVERS = $(BUILD_DIR)/drivers/drivers.a
FS      = $(BUILD_DIR)/fs/fs.a

LIBS = $(LIBC) $(DRIVERS) $(FS)

INTERNAL_CFLAGS  = $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBC_DIR)/include \
                   -Iinclude -I$(DRIVERS_DIR) -I$(FS_DIR)
INTERNAL_LDFLAGS = $(LDFLAGS) -T ../linker.ld -Wl,--oformat,binary

MSG_PREFIX  = kernel
CFLAGS_USER = $(INTERNAL_CFLAGS)
EXTRA_DEP   = $(CONFIG_HEADER)

include ../scripts/rules.mk

.PHONY: all objects kernel clean

all: kernel

objects: $(OBJS)

kernel: objects $(LIBS)
	@echo "  LD       $(subst $(abspath ..)/,,$(BIN))"
	@$(CC) $(INTERNAL_LDFLAGS) -o $(BIN) $(OBJS) $(LIBS) -lgcc

clean:
	@rm -f $(OBJS) $(BIN)
	@rm -f $(CONFIG_HEADER)
