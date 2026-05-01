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

.PHONY: all clean run bootloader libc drivers fs kernel objects archives

all: $(IMG)

objects:
	$(MAKE) -C libc objects
	$(MAKE) -C drivers objects
	$(MAKE) -C fs objects
	$(MAKE) -C kernel objects

archives: objects
	$(MAKE) -C libc archive
	$(MAKE) -C drivers archive
	$(MAKE) -C fs archive

kernel: archives
	$(MAKE) -C kernel kernel

bootloader:
	$(MAKE) -C bootloader

$(IMG): bootloader kernel
	@echo "  IMG     build/lanex.img"
	@cat $(BOOT_BIN) $(KERNEL_BIN) > $(IMG)
	@truncate -s 1440k $(IMG)

run: all
	@echo "  RUN     build/lanex.img"
	@$(SCRIPTS_DIR)/qemu.sh $(MEM) $(IMG)

defconfig:
	@echo "  DEFCONFIG"
	@$(SCRIPTS_DIR)/kconfig.sh def

menuconfig:
	@echo "  MENUCONFIG"
	@$(SCRIPLES_DIR)/kconfig.sh menu

format:
	@echo "  FORMAT"
	@$(SCRIPTS_DIR)/format.sh

clean:
	@echo "  CLN     build"
	@rm -rf $(BUILD_DIR)
