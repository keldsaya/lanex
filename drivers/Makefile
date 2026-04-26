SRC_DIR = src
D_BUILD_DIR = $(BUILD_DIR)/drivers

DRIVERS_OBJS = ata.c.o keyboard.c.o vga.c.o cursor.c.o rtc.c.o power.c.o

OBJS = $(addprefix $(D_BUILD_DIR)/, $(DRIVERS_OBJS))
LIB_DRIVERS = $(D_BUILD_DIR)/drivers.a

INTERNAL_CFLAGS = $(CFLAGS) -Iinclude -I../kernel/include

.PHONY: all clean

all: $(LIB_DRIVERS)

$(LIB_DRIVERS): $(OBJS)
	@echo "  AR      drivers.a"
	@mkdir -p $(dir $@)
	@$(AR) rcs $@ $(OBJS)

$(D_BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@echo "  CC      drivers/$<"
	@mkdir -p $(D_BUILD_DIR)
	@$(CC) $(INTERNAL_CFLAGS) -c $< -o $@

clean:
	@rm -rf $(D_BUILD_DIR)
