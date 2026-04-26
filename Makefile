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

DEFCONFIG_FILE = user/defconfig

export CC AS AR NASM CFLAGS LDFLAGS BUILD_DIR
export CONFIG_FILE=$(CURDIR)/.config

ifeq ($(wildcard .config),)
  NEED_DEFCONFIG = 1
endif

.PHONY: all clean run bootloader libc kernel drivers defconfig menuconfig

ifeq ($(NEED_DEFCONFIG),1)
all: defconfig
	@$(MAKE) --no-print-directory all_real
else
all: all_real
endif

all_real: $(IMG)

$(IMG): bootloader libc drivers kernel
	@echo "  IMG     $(REL_IMG)"
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

run: all
	@echo "  RUN     $(REL_IMG)"
	@qemu-system-i386 -m $(MEM) -drive file=$(IMG),format=raw,index=0,media=disk

defconfig:
	@echo "  DEFCONFIG"
	@cp $(DEFCONFIG_FILE) $(CONFIG_FILE)

menuconfig:
	@if [ ! -f .config ]; then \
		echo "No .config found, running defconfig first"; \
		$(MAKE) defconfig; \
	fi
	@echo "========================================"
	@echo "Manual config: edit .config file"
	@echo "========================================"
	@echo "Available options:"
	@echo "  CONFIG_DRIVER_VGA     - VGA text mode (y/n)"
	@echo "  CONFIG_DRIVER_CURSOR  - Hardware cursor (depends on VGA)"
	@echo "  CONFIG_DRIVER_KEYBOARD - PS/2 keyboard"
	@echo "  CONFIG_DRIVER_ATA     - ATA/IDE disk driver"
	@echo "  CONFIG_DRIVER_POWER   - Shutdown/reboot"
	@echo "  CONFIG_DRIVER_RTC     - Real Time Clock"
	@echo ""
	@echo "Current configuration:"
	@if [ -f .config ]; then \
		cat .config | grep "CONFIG_" || echo "  (empty)"; \
	else \
		echo "  (no config)"; \
	fi
	@echo ""
	@echo "To change: vim .config"
	@echo "To reset: make defconfig"

clean:
	@echo "  CLN     $(REL_BUILD_DIR)"
	@rm -rf $(BUILD_DIR)

clean-drivers:
	@echo "  CLN     $(REL_BUILD_DIR)/drivers"
	@rm -rf $(BUILD_DIR)/drivers
