CC = i686-elf-gcc
AS = i686-elf-as
NASM = nasm
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
MEM = 4M

BUILD_DIR = build
KERNEL_DIR = kernel
LIBC_DIR = libc
IMG = lanex.img

.PHONY: all clean run libc kernel

all: libc kernel
	$(NASM) -f bin $(KERNEL_DIR)/src/boot.asm -o $(BUILD_DIR)/boot.bin
	cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin > $(IMG)
	truncate -s 1440k $(IMG)

libc:
	@$(MAKE) -C $(LIBC_DIR) CC="$(CC)" AS="$(AS)" CFLAGS="$(CFLAGS)"

kernel: libc
	@$(MAKE) -C $(KERNEL_DIR) CC="$(CC)" AS="$(AS)" NASM="$(NASM)" \
		BUILD_DIR="../$(BUILD_DIR)" LIBC_DIR="../$(LIBC_DIR)"
run: all
	qemu-system-i386 -m $(MEM) -drive file=$(IMG),format=raw,index=0,media=disk

clean:
	@$(MAKE) -C $(KERNEL_DIR) clean BUILD_DIR="../$(BUILD_DIR)" || true
	@$(MAKE) -C $(LIBC_DIR) clean BUILD_DIR="../$(BUILD_DIR)" || true
	rm -rf $(BUILD_DIR) $(IMG)
