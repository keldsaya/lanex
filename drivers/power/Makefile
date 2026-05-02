SRC_DIR = .
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.c.o)
FULL_OBJS = $(addprefix $(OBJ_DIR)/,$(OBJS))

.PHONY: all objects clean

all: objects

objects: $(FULL_OBJS)

$(OBJ_DIR)/%.c.o: %.c $(CONFIG_HEADER)
	@echo "  CC       drivers/$(MODULE)/$<"
	@mkdir -p $(dir $@)
	@$(CC) $(DRIVERS_CFLAGS) -c $< -o $@

clean:
	@echo "  CLN      drivers/$(MODULE)"
	rm -rf $(BUILD_DIR)/drivers/$(MODULE)
