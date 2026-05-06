MAKEFLAGS += -rR -s

SRC_DIR     = .
OBJ_DIR     ?= $(BUILD_DIR)/kernel/mm
BUILT_IN    ?= $(OBJ_DIR)/../mm.built-in.o

SRCS        = $(wildcard $(SRC_DIR)/*.c)
OBJS        = $(SRCS:$(SRC_DIR)/%.c=%.c.o)
FULL_OBJS   = $(addprefix $(OBJ_DIR)/,$(OBJS))

MSG_PREFIX  = kernel/mm
CFLAGS_USER = $(KERNEL_CFLAGS)
EXTRA_DEP   = $(CONFIG_HEADER)

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c $(EXTRA_DEP)
	@echo "  CC       $(MSG_PREFIX)/$(subst $(SRC_DIR)/,,$<)"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS_USER) -c $< -o $@

.PHONY: all objects clean

objects: $(FULL_OBJS) $(BUILT_IN)

all: objects

$(BUILT_IN): $(FULL_OBJS)
	@echo "  LD       build/kernel/mm.built-in.o"
	@$(CC) -r -nostdlib -o $@ $^

clean:
	@echo "  CLN      kernel/mm"
	@rm -rf $(OBJ_DIR) $(BUILT_IN)
