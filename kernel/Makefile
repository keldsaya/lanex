MAKEFLAGS += -rR -s

LIBC_DIR     = $(abspath ../libc)
DRIVERS_DIR  = $(abspath ../drivers)
FS_DIR       = $(abspath ../fs)
SRC_DIR      = .
INCLUDE_DIR  = $(abspath ../include)

CONFIG_HEADER = $(INCLUDE_DIR)/lanex/config.h

KERNEL_OBJS = entry.asm.o main.c.o io.c.o \
              interrupts.s.o idt.c.o pic.c.o \
              pit.c.o tty.c.o pmm.c.o \
              messages.c.o kprintf.c.o

SUBDIRS := $(wildcard */)
SUBDIRS := $(filter-out include/ scripts/., $(SUBDIRS))

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

BUILTINS := $(addprefix $(OBJ_DIR)/, $(SUBDIRS:%/=%.built-in.o))

.PHONY: all objects kernel clean $(SUBDIRS)

all: kernel

objects: $(OBJS) $(BUILTINS)

kernel: objects $(LIBS)
	@echo "  LD       $(subst $(abspath ..)/,,$(BIN))"
	@$(CC) $(INTERNAL_LDFLAGS) -o $(BIN) $(OBJS) $(BUILTINS) $(LIBS) -lgcc

$(OBJ_DIR)/%.built-in.o: %
	@$(MAKE) -C $< \
		OBJ_DIR=$(abspath $(OBJ_DIR)/$<) \
		BUILT_IN=$@ \
		CFLAGS_USER="$(INTERNAL_CFLAGS)" \
		CONFIG_HEADER=$(CONFIG_HEADER) \
		objects

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean OBJ_DIR=$(abspath $(OBJ_DIR)/$$dir); \
	done
	@rm -f $(OBJS) $(BUILTINS) $(BIN)
	@rm -f $(CONFIG_HEADER)
