LIBC_DIR = $(abspath ../libc)
DRIVERS_DIR = $(abspath ../drivers)
SRC_DIR  = src

KERNEL_OBJS = entry.asm.o main.c.o io.c.o \
              interrupts.s.o idt.c.o pic.c.o \
              pit.c.o tty.c.o pmm.c.o \
              sh.c.o messages.c.o kprintf.c.o

OBJS = $(addprefix $(BUILD_DIR)/kernel/, $(KERNEL_OBJS))
BIN  = $(BUILD_DIR)/kernel.bin
LIBC = $(BUILD_DIR)/libc/libc.a
DRIVERS = $(BUILD_DIR)/drivers/drivers.a

INTERNAL_CFLAGS = $(CFLAGS) -I$(LIBC_DIR)/include -Iinclude -I$(DRIVERS_DIR)/include
INTERNAL_LDFLAGS = $(LDFLAGS) -T ../linker.ld -Wl,--oformat,binary

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS) $(LIBC) $(DRIVERS)
	@echo "  LD      $(subst $(abspath ..)/,,$@)"
	@$(CC) $(INTERNAL_LDFLAGS) -o $@ $(OBJS) $(DRIVERS) $(LIBC) -lgcc

$(BUILD_DIR)/kernel/%.c.o: $(SRC_DIR)/%.c $(BUILD_DIR)/kernel/config.h
	@echo "  CC      $<"
	@mkdir -p $(dir $@)
	@$(CC) $(INTERNAL_CFLAGS) -I$(BUILD_DIR)/kernel -c $< -o $@

$(BUILD_DIR)/kernel/%.s.o: $(SRC_DIR)/%.s 
	@echo "  ASM     $<"
	@mkdir -p $(dir $@)
	@$(AS) $< -o $@

$(BUILD_DIR)/kernel/%.asm.o: $(SRC_DIR)/%.asm
	@echo "  ASM     $<"
	@mkdir -p $(dir $@)
	@$(NASM) -f elf32 $< -o $@

$(BUILD_DIR)/kernel/config.h: $(CURDIR)/../.config
	@mkdir -p $(dir $@)
	@echo "/* Auto-generated config header */" > $@
	@echo "#ifndef _CONFIG_H_" >> $@
	@echo "#define _CONFIG_H_" >> $@
	@echo "" >> $@
	@echo "/* Enabled drivers (y): */" >> $@
	@grep "^CONFIG_.*=y" $< | sed 's/=y$$/ 1/' | sed 's/^/#define /' >> $@
	@echo "" >> $@
	@echo "/* Disabled drivers (n): */" >> $@
	@grep "^CONFIG_.*=n" $< | sed 's/=n$$//' | sed 's/^/#undef /' >> $@
	@echo "" >> $@
	@echo "#endif" >> $@
clean:
	@rm -f $(OBJS) $(BIN)
