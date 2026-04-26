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

$(BUILD_DIR)/kernel/%.c.o: $(SRC_DIR)/%.c
	@echo "  CC      $<"
	@mkdir -p $(dir $@)
	@$(CC) $(INTERNAL_CFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel/%.s.o: $(SRC_DIR)/%.s
	@echo "  ASM     $<"
	@mkdir -p $(dir $@)
	@$(AS) $< -o $@

$(BUILD_DIR)/kernel/%.asm.o: $(SRC_DIR)/%.asm
	@echo "  ASM     $<"
	@mkdir -p $(dir $@)
	@$(NASM) -f elf32 $< -o $@

clean:
	@rm -f $(OBJS) $(BIN)
