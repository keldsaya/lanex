MAKEFLAGS += -rR -s

DRIVERS_BUILD_DIR = $(BUILD_DIR)/drivers
CONFIG_HEADER     = $(abspath ../include/lanex/config.h)

-include $(CURDIR)/../.config
-include $(CURDIR)/Kconfig.include

DRIVERS_DIRS = $(DRIVERS-y)

LIB_DRIVERS     = $(DRIVERS_BUILD_DIR)/drivers.a
REL_LIB_DRIVERS = $(subst $(abspath ..)/,,$(LIB_DRIVERS))

INTERNAL_CFLAGS = $(CFLAGS) -I$(abspath ../include) -I../../kernel/include

DRIVER_OBJS = $(foreach mod,$(DRIVERS_DIRS), \
    $(wildcard $(DRIVERS_BUILD_DIR)/$(mod)/*.c.o))

.PHONY: all objects archive clean

all: archive

objects: $(CONFIG_HEADER) $(DRIVERS_DIRS:%=$(DRIVERS_BUILD_DIR)/%)

archive: $(LIB_DRIVERS)

$(DRIVERS_BUILD_DIR)/%: %/
	@$(MAKE) -C $* \
		MODULE=$* \
		OBJ_DIR=$(abspath $(DRIVERS_BUILD_DIR)/$*) \
		DRIVERS_CFLAGS="$(INTERNAL_CFLAGS)" \
		CONFIG_HEADER=$(CONFIG_HEADER) \
		objects

$(LIB_DRIVERS): $(DRIVER_OBJS)
	@echo "  AR       $(REL_LIB_DRIVERS) ($(DRIVERS-y))"
	@mkdir -p $(dir $@)
	@$(AR) rcs $@ $(DRIVER_OBJS)

clean:
	@rm -rf $(DRIVERS_BUILD_DIR)
