SRC_DIR = src
DRIVERS_BUILD_DIR = $(BUILD_DIR)/drivers

-include $(CURDIR)/../.config
-include $(CURDIR)/Kconfig.include

DRIVERS_DIRS = $(DRIVERS-y)

LIB_DRIVERS = $(DRIVERS_BUILD_DIR)/drivers.a
REL_LIB_DRIVERS = $(subst $(abspath ..)/,,$(LIB_DRIVERS))

INTERNAL_CFLAGS = $(CFLAGS) -I../../kernel/include -I$(BUILD_DIR)/kernel

DRIVER_OBJS = $(foreach mod,$(DRIVERS_DIRS), \
    $(wildcard $(DRIVERS_BUILD_DIR)/$(mod)/*.c.o))

.PHONY: all objects archive clean

all: archive

objects: $(DRIVERS_DIRS:%=$(DRIVERS_BUILD_DIR)/%)

archive: $(LIB_DRIVERS)

$(DRIVERS_BUILD_DIR)/%: %/
	@$(MAKE) -C $* \
		MODULE=$* \
		OBJ_DIR=$(abspath $(DRIVERS_BUILD_DIR)/$*) \
		FS_CFLAGS="$(INTERNAL_CFLAGS)" \
		objects

$(LIB_DRIVERS): $(DRIVER_OBJS)
	@echo "  AR      $(REL_LIB_DRIVERS) ($(DRIVERS-y))"
	@mkdir -p $(dir $@)
	@$(AR) rcs $@ $(DRIVER_OBJS)

clean:
	@rm -rf $(DRIVERS_BUILD_DIR)
