MAKEFLAGS += -rR -s

SRC_DIR     = .
OBJ_DIR     ?= $(BUILD_DIR)/fs/$(MODULE)
SRCS        = $(wildcard *.c)
OBJS        = $(SRCS:.c=.c.o)
FULL_OBJS   = $(addprefix $(OBJ_DIR)/,$(OBJS))

MSG_PREFIX  = fs/$(MODULE)
CFLAGS_USER = $(FS_CFLAGS)

include ../../scripts/rules.mk

.PHONY: all objects clean

all: objects

objects: $(FULL_OBJS)

clean:
	@echo "  CLN      fs/$(MODULE)"
	@rm -rf $(BUILD_DIR)/fs/$(MODULE)
