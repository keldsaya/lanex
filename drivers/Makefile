SRC_DIR = src
DRIVERS_BUILD_DIR = $(BUILD_DIR)/drivers

-include $(CURDIR)/../.config
-include $(CURDIR)/Kconfig.include

DRIVERS_DIRS = $(DRIVERS-y)

LIB_DRIVERS = $(DRIVERS_BUILD_DIR)/drivers.a
REL_LIB_DRIVERS = $(subst $(abspath ..)/,,$(LIB_DRIVERS))

INTERNAL_CFLAGS = $(CFLAGS) -I../../kernel/include -I$(BUILD_DIR)/kernel

DRIVER_STAMPS = $(addprefix $(DRIVERS_BUILD_DIR)/, $(DRIVERS_DIRS:=.stamp))

.PHONY: all clean

all: $(LIB_DRIVERS)

OBJS_ALL = $(foreach mod,$(DRIVERS_DIRS),$(wildcard $(DRIVERS_BUILD_DIR)/$(mod)/*.c.o))

$(LIB_DRIVERS): $(DRIVER_STAMPS)
	@echo "  AR      $(REL_LIB_DRIVERS) ($(DRIVERS-y))"
	@mkdir -p $(dir $@)
	@$(AR) rcs $@ $(OBJS_ALL)

$(DRIVERS_BUILD_DIR)/%.stamp:
	@$(MAKE) -C $* \
		MODULE=$* \
		OBJ_DIR=$(abspath $(DRIVERS_BUILD_DIR)/$*) \
		FS_CFLAGS="$(INTERNAL_CFLAGS)"
	@touch $@

clean:
	@rm -rf $(DRIVERS_BUILD_DIR)
