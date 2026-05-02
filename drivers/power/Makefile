MAKEFLAGS += -rR -s

SRC_DIR     = .
OBJ_DIR     ?= $(BUILD_DIR)/drivers/$(MODULE)
SRCS        = $(wildcard *.c)
OBJS        = $(SRCS:.c=.c.o)
FULL_OBJS   = $(addprefix $(OBJ_DIR)/,$(OBJS))

MSG_PREFIX  = drivers/$(MODULE)
CFLAGS_USER = $(DRIVERS_CFLAGS)
EXTRA_DEP   = $(CONFIG_HEADER)

include ../../scripts/rules.mk

.PHONY: all objects clean

all: objects

objects: $(FULL_OBJS)

clean:
	@echo "  CLN      drivers/$(MODULE)"
	@rm -rf $(BUILD_DIR)/drivers/$(MODULE)
