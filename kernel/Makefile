LIBC_DIR = $(abspath ../libc)
SRC_DIR  = src

KERNEL_OBJS = entry.o kprintf.o main.o io.o \
              interrupts.o idt.o pic.o \
              vga.o pit.o tty.o \
              pmm.o keyboard.o cursor.o \
              sh.o messages.o ata.o

OBJS = $(addprefix $(BUILD_DIR)/kernel/, $(KERNEL_OBJS))
BIN  = $(BUILD_DIR)/kernel.bin
LIBC = $(BUILD_DIR)/libc/libc.a

INTERNAL_CFLAGS = $(CFLAGS) -I$(LIBC_DIR)/include -Iinclude
INTERNAL_LDFLAGS = $(LDFLAGS) -T ../linker.ld -Wl,--oformat,binary

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(INTERNAL_LDFLAGS) -o $@ $(OBJS) $(LIBC) -lgcc

$(BUILD_DIR)/kernel/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(INTERNAL_CFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $< -o $@

$(BUILD_DIR)/kernel/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(NASM) -f elf32 $< -o $@

clean:
	rm -rf $(BUILD_DIR)/kernel
