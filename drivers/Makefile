SRC_DIR = src
D_BUILD_DIR = $(BUILD_DIR)/drivers

-include $(CURDIR)/../.config
-include $(CURDIR)/Kconfig.include

OBJS = $(addprefix $(D_BUILD_DIR)/, $(DRIVER_OBJS))
LIB_DRIVERS = $(D_BUILD_DIR)/drivers.a

INTERNAL_CFLAGS = $(CFLAGS) -Iinclude -I../kernel/include -I$(BUILD_DIR)/kernel

all: $(LIB_DRIVERS)

$(LIB_DRIVERS): $(OBJS)
	@echo "  AR      drivers.a ($(DRIVERS-y))"
	@mkdir -p $(dir $@)
	@rm -f $@
	@$(AR) rcs $@ $(OBJS)

$(D_BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@echo "  CC      drivers/$<"
	@mkdir -p $(D_BUILD_DIR)
	@$(CC) $(INTERNAL_CFLAGS) -c $< -o $@

clean:
	@rm -rf $(D_BUILD_DIR)
