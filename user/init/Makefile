MAKEFLAGS += -rR -s

SRC_DIR     = .
OBJ_DIR     ?= $(BUILD_DIR)/user/$(MODULE)

SRCS        = $(wildcard *.c)
OBJS        = $(SRCS:.c=.c.o)
FULL_OBJS   = $(addprefix $(OBJ_DIR)/,$(OBJS))

MSG_PREFIX  = user/$(MODULE)
CFLAGS_USER = $(USER_CFLAGS) -I$(abspath .) -I$(abspath ../../include)
EXTRA_DEP   = $(CONFIG_HEADER)

include ../../scripts/rules.mk

.PHONY: all objects clean

all: objects

objects: $(FULL_OBJS)

clean:
	@echo "  CLN      user/$(MODULE)"
	@rm -rf $(OBJ_DIR)
