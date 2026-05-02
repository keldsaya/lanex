MAKEFLAGS += -rR -s

PROJECT_ROOT = $(abspath ..)
KCONFIG_SH    = kconfig.sh
GEN_CONFIG_SH = gen_config.sh
QEMU_SH       = qemu.sh
FORMAT_SH     = format.sh

.PHONY: defconfig menuconfig gen_config run_qemu format

defconfig:
	@./$(KCONFIG_SH) def

menuconfig:
	@./$(KCONFIG_SH) menu

gen_config:
	@./$(GEN_CONFIG_SH) $(CONFIG_FILE) $(CONFIG_HEADER)

run_qemu:
	@./$(QEMU_SH) $(MEM) $(IMG)

format:
	@./$(FORMAT_SH)
