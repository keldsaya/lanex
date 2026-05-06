MAKEFLAGS += -rR -s

USER_BUILD_DIR = $(BUILD_DIR)/user
CONFIG_HEADER  = $(abspath ../include/lanex/config.h)

USER_DIRS = sh

LIB_USER     = $(USER_BUILD_DIR)/user.a
REL_LIB_USER = $(subst $(abspath ..)/,,$(LIB_USER))

INTERNAL_CFLAGS = $(CFLAGS) -I$(abspath ../include) -I$(abspath ../drivers) -I$(abspath ../fs)

USER_OBJS = $(foreach mod,$(USER_DIRS), \
    $(wildcard $(USER_BUILD_DIR)/$(mod)/*.c.o))

.PHONY: all objects archive clean

all: archive

objects: $(CONFIG_HEADER) $(USER_DIRS:%=$(USER_BUILD_DIR)/%)

archive: $(LIB_USER)

$(USER_BUILD_DIR)/%: %/
	@$(MAKE) -C $* \
		MODULE=$* \
		OBJ_DIR=$(abspath $(USER_BUILD_DIR)/$*) \
		USER_CFLAGS="$(INTERNAL_CFLAGS)" \
		CONFIG_HEADER=$(CONFIG_HEADER) \
		objects

$(LIB_USER): $(USER_OBJS)
	@echo "  AR       $(REL_LIB_USER) ($(USER_DIRS))"
	@mkdir -p $(dir $@)
	@$(AR) rcs $@ $(USER_OBJS)

clean:
	@rm -rf $(USER_BUILD_DIR)
