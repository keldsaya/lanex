SRC_DIR = .
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.c.o)
FULL_OBJS = $(addprefix $(OBJ_DIR)/,$(OBJS))

.PHONY: all objects

all: objects

objects: $(FULL_OBJS)

$(OBJ_DIR)/%.c.o: %.c
	@echo "  CC       fs/$(MODULE)/$<"
	@mkdir -p $(dir $@)
	@$(CC) $(FS_CFLAGS) -c $< -o $@
