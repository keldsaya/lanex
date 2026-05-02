MAKEFLAGS += -rR -s --no-print-directory

BUILD_DIR     = $(CURDIR)/build
SCRIPTS_DIR   = $(CURDIR)/scripts

MEM           = 4M
IMG           = $(BUILD_DIR)/lanex.img
BOOT_BIN      = $(BUILD_DIR)/boot.bin
KERNEL_BIN    = $(BUILD_DIR)/kernel.bin
LIB_DRIVERS   = $(BUILD_DIR)/drivers/drivers.a
LIB_FS        = $(BUILD_DIR)/fs/fs.a
LIB_LIBC      = $(BUILD_DIR)/libc/libc.a

CROSS_CC := $(shell which i686-elf-gcc 2>/dev/null)
ifneq ($(CROSS_CC),)
  CC       = i686-elf-gcc
  AS       = i686-elf-as
  AR       = i686-elf-ar
  ASFLAGS  =
else
  CC       = gcc
  AS       = as
  AR       = ar
  CFLAGS   += -m32 -fno-stack-protector -fno-pic
  LDFLAGS  += -m32
  ASFLAGS  = --32
endif

NASM = nasm

CFLAGS  += -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS += -ffreestanding -O2 -nostdlib

DRIVERS_CFLAGS = $(CFLAGS) -I$(CURDIR)/include -I$(CURDIR)/kernel/include
FS_CFLAGS      = $(CFLAGS) -I$(CURDIR)/include -I$(CURDIR)/kernel/include

CONFIG_FILE   = $(CURDIR)/.config
CONFIG_HEADER = $(CURDIR)/include/lanex/config.h

export CC AS AR NASM CFLAGS LDFLAGS ASFLAGS BUILD_DIR
export CONFIG_FILE CONFIG_HEADER MEM IMG

ifeq ($(wildcard $(CONFIG_FILE)),)
  $(shell $(SCRIPTS_DIR)/kconfig.sh def)
endif

.PHONY: all clean run boot libc drivers fs kernel objects archives prepare
.PHONY: defconfig menuconfig format
.PHONY: drivers/% fs/% libc/% clean-drivers/% clean-fs/% clean-libc/%

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

boot:
	$(MAKE) -C boot

$(IMG): boot prepare kernel
	@echo "  IMG      build/lanex.img"
	@cat $(BOOT_BIN) $(KERNEL_BIN) > $(IMG)
	@truncate -s 1440k $(IMG)


prepare: $(CONFIG_HEADER)


$(CONFIG_HEADER): $(CONFIG_FILE)
	$(MAKE) -C scripts gen_config

defconfig:
	$(MAKE) -C scripts defconfig

menuconfig:
	$(MAKE) -C scripts menuconfig

genconfig:
	$(MAKE) -C scripts gen_config 

run: all
	@echo "  RUN      build/lanex.img"
	$(MAKE) -C scripts run_qemu

format:
	$(MAKE) -C scripts format


drivers/%: prepare
	@$(MAKE) -C drivers/$* \
		MODULE=$* \
		OBJ_DIR=$(BUILD_DIR)/drivers/$* \
		DRIVERS_CFLAGS="$(DRIVERS_CFLAGS)" \
		CONFIG_HEADER=$(CONFIG_HEADER) \
		objects
	@$(AR) rcs $(LIB_DRIVERS) $(BUILD_DIR)/drivers/$*/*.c.o 2>/dev/null || true

fs/%: prepare
	@$(MAKE) -C fs/$* \
		MODULE=$* \
		OBJ_DIR=$(BUILD_DIR)/fs/$* \
		FS_CFLAGS="$(FS_CFLAGS)" \
		CONFIG_HEADER=$(CONFIG_HEADER) \
		objects
	@$(AR) rcs $(LIB_FS) $(BUILD_DIR)/fs/$*/*.c.o 2>/dev/null || true

libc/%: prepare
	@$(MAKE) -C libc \
		FILE=$* \
		OBJ=$(BUILD_DIR)/libc/$*.c.o \
		single
	@$(AR) rcs $(LIB_LIBC) $(BUILD_DIR)/libc/$*.c.o


clean:
	@rm -rf $(BUILD_DIR)

clean-drivers/%:
	@$(MAKE) -C drivers/$* clean \
		MODULE=$* \
		OBJ_DIR=$(BUILD_DIR)/drivers/$* \
		BUILD_DIR=$(BUILD_DIR)
	@$(AR) d $(LIB_DRIVERS) $(BUILD_DIR)/drivers/$*/*.c.o 2>/dev/null || true

clean-fs/%:
	@$(MAKE) -C fs/$* clean \
		MODULE=$* \
		OBJ_DIR=$(BUILD_DIR)/fs/$* \
		BUILD_DIR=$(BUILD_DIR)
	@$(AR) d $(LIB_FS) $(BUILD_DIR)/fs/$*/*.c.o 2>/dev/null || true

clean-libc/%:
	@rm -f $(BUILD_DIR)/libc/$*.c.o
	@$(AR) d $(LIB_LIBC) $(BUILD_DIR)/libc/$*.c.o 2>/dev/null || true
