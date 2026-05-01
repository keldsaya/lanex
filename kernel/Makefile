LIBC_DIR = $(abspath ../libc)
DRIVERS_DIR = $(abspath ../drivers)
FS_DIR = $(abspath ../fs)
SRC_DIR  = src

KERNEL_OBJS = entry.asm.o main.c.o io.c.o \
              interrupts.s.o idt.c.o pic.c.o \
              pit.c.o tty.c.o pmm.c.o \
              sh.c.o messages.c.o kprintf.c.o

OBJS = $(addprefix $(BUILD_DIR)/kernel/, $(KERNEL_OBJS))
BIN  = $(BUILD_DIR)/kernel.bin

LIBC = $(BUILD_DIR)/libc/libc.a
DRIVERS = $(BUILD_DIR)/drivers/drivers.a
FS = $(BUILD_DIR)/fs/fs.a

LIBS = $(LIBC) $(DRIVERS) $(FS)

INTERNAL_CFLAGS = $(CFLAGS) -I$(LIBC_DIR)/include -Iinclude -I$(DRIVERS_DIR) -I$(FS_DIR)
INTERNAL_LDFLAGS = $(LDFLAGS) -T ../linker.ld -Wl,--oformat,binary

.PHONY: all objects kernel clean

all: kernel

objects: $(OBJS)

kernel: objects $(LIBS)
	@echo "  LD      $(subst $(abspath ..)/,,$(BIN))"
	@$(CC) $(INTERNAL_LDFLAGS) -o $(BIN) $(OBJS) $(LIBS) -lgcc

$(BUILD_DIR)/kernel/%.c.o: $(SRC_DIR)/%.c $(BUILD_DIR)/kernel/config.h
	@echo "  CC      kernel/$<"
	@mkdir -p $(dir $@)
	@$(CC) $(INTERNAL_CFLAGS) -I$(BUILD_DIR)/kernel -c $< -o $@

$(BUILD_DIR)/kernel/%.s.o: $(SRC_DIR)/%.s
	@echo "  ASM     kernel/$<"
	@mkdir -p $(dir $@)
	@$(AS) $< -o $@

$(BUILD_DIR)/kernel/%.asm.o: $(SRC_DIR)/%.asm
	@echo "  ASM     kernel/$<"
	@mkdir -p $(dir $@)
	@$(NASM) -f elf32 $< -o $@

$(BUILD_DIR)/kernel/config.h: $(CURDIR)/../.config
	@mkdir -p $(dir $@)
	@echo "/* Auto-generated config header */" > $@
	@echo "#ifndef _CONFIG_H_" >> $@
	@echo "#define _CONFIG_H_" >> $@
	@echo "" >> $@
	@echo "/* Enabled configs (y): */" >> $@
	@grep "^CONFIG_.*=y" $< | sed 's/=y$$/ 1/' | sed 's/^/#define /' >> $@
	@echo "" >> $@
	@echo "/* Disabled configs (n): */" >> $@
	@grep "^CONFIG_.*=n" $< | sed 's/=n$$//' | sed 's/^/#undef /' >> $@
	@echo "" >> $@
	@echo "#endif" >> $@

clean:
	@rm -f $(OBJS) $(BIN)
