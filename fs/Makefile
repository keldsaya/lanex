SRC_DIR = src
FS_BUILD_DIR = $(BUILD_DIR)/fs

-include $(CURDIR)/../.config
-include $(CURDIR)/Kconfig.include

OBJS = $(addprefix $(FS_BUILD_DIR)/, $(FS_OBJS))
LIB_FS = $(FS_BUILD_DIR)/fs.a

INTERNAL_CFLAGS = $(CFLAGS) -Iinclude -I../kernel/include -I$(BUILD_DIR)/kernel

all: $(LIB_FS)

$(LIB_FS): $(OBJS)
	@echo "  AR      fs.a ($(FS-y))"
	@mkdir -p $(dir $@)
	@rm -f $@
	@$(AR) rcs $@ $(OBJS)

$(FS_BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@echo "  CC      fs/$<"
	@mkdir -p $(FS_BUILD_DIR)
	@$(CC) $(INTERNAL_CFLAGS) -c $< -o $@

clean:
	@rm -rf $(D_BUILD_DIR)
