MEM           = 4M
BUILD_DIR     = $(CURDIR)/build
SCRIPTS_DIR   = $(CURDIR)/scripts

IMG           = $(BUILD_DIR)/lanex.img
BOOT_BIN      = $(BUILD_DIR)/boot.bin
KERNEL_BIN    = $(BUILD_DIR)/kernel.bin

CC   = i686-elf-gcc
AS   = i686-elf-as
AR   = i686-elf-ar
NASM = nasm

CFLAGS      = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS     = -ffreestanding -O2 -nostdlib
MAKEFLAGS  += -s --no-print-directory

ifeq ($(deb), 1)
  CC      = gcc -m32
  AS      = as --32
  AR      = ar
  CFLAGS  += -fno-stack-protector -fno-pic
  LDFLAGS += -m32
endif

DEFCONFIG_FILE = user/defconfig

export CC AS AR NASM CFLAGS LDFLAGS BUILD_DIR
export CONFIG_FILE=$(CURDIR)/.config

ifeq ($(wildcard $(CONFIG_FILE)),)
$(shell $(SCRIPTS_DIR)/kconfig.sh def)
endif

.PHONY: all clean run bootloader libc kernel drivers fs defconfig menuconfig scripts-executable

all: $(IMG)

$(IMG): bootloader libc drivers fs kernel
	@echo "  IMG     build/lanex.img"
	@cat $(BOOT_BIN) $(KERNEL_BIN) > $(IMG)
	@truncate -s 1440k $(IMG)

libc:
	@$(MAKE) -C libc

kernel:
	@$(MAKE) -C kernel

bootloader:
	@$(MAKE) -C bootloader

drivers:
	@$(MAKE) -C drivers

fs:
	@$(MAKE) -C fs

run: all
	@echo "  RUN     build/lanex.img"
	@$(SCRIPTS_DIR)/qemu.sh $(MEM) $(IMG)

defconfig:
	@echo "  DEFCONFIG"
	@$(SCRIPTS_DIR)/kconfig.sh def

menuconfig:
	@echo "  MENUCONFIG"
	@$(SCRIPTS_DIR)/kconfig.sh menu

clean:
	@echo "  CLN     build"
	@rm -rf $(BUILD_DIR)
