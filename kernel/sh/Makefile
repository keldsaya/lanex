MAKEFLAGS += -rR -s

SRC_DIR     = .
OBJ_DIR     ?= $(BUILD_DIR)/kernel/sh
BUILT_IN    ?= $(OBJ_DIR)/../sh.built-in.o

SRCS        = $(wildcard *.c)
OBJS        = $(SRCS:.c=.c.o)
FULL_OBJS   = $(addprefix $(OBJ_DIR)/, $(OBJS))

MSG_PREFIX  = kernel/sh
CFLAGS_USER ?= $(CFLAGS)
EXTRA_DEP   = $(CONFIG_HEADER)

include ../../scripts/rules.mk

.PHONY: all objects clean

all: objects

objects: $(FULL_OBJS) $(BUILT_IN)

$(BUILT_IN): $(FULL_OBJS)
	@echo "  LD       $(subst $(abspath ../../)/,,$@)"
	@$(CC) -r -nostdlib -o $@ $^

clean:
	@echo "  CLN      kernel/sh"
	@rm -rf $(OBJ_DIR) $(BUILT_IN)
