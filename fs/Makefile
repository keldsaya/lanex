SRC_DIR = src
FS_BUILD_DIR = $(BUILD_DIR)/fs

-include $(CURDIR)/../.config
-include $(CURDIR)/Kconfig.include

FS_DIRS = $(FS-y)

LIB_FS = $(FS_BUILD_DIR)/fs.a
REL_LIB_FS = $(subst $(abspath ..)/,,$(LIB_FS))

INTERNAL_CFLAGS = $(CFLAGS) -Iinclude -I../kernel/include -I$(BUILD_DIR)/kernel

FS_OBJS = $(foreach mod,$(FS_DIRS), \
    $(wildcard $(FS_BUILD_DIR)/$(mod)/*.c.o))

.PHONY: all objects archive clean

all: archive

objects: $(FS_DIRS:%=$(FS_BUILD_DIR)/%)

archive: $(LIB_FS)

$(FS_BUILD_DIR)/%: %/
	@$(MAKE) -C $* \
		MODULE=$* \
		OBJ_DIR=$(abspath $(FS_BUILD_DIR)/$*) \
		FS_CFLAGS="$(INTERNAL_CFLAGS)" \
		objects

$(LIB_FS): $(FS_OBJS)
	@echo "  AR      $(REL_LIB_FS) ($(FS-y))"
	@mkdir -p $(dir $@)
	@$(AR) rcs $@ $(FS_OBJS)

clean:
	@rm -rf $(FS_BUILD_DIR)
