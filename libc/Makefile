MAKEFLAGS += -rR -s

SRC_DIR     = src
OBJ_DIR     = $(BUILD_DIR)/libc

LIBC_OBJS = strlen.c.o strcpy.c.o strcmp.c.o \
            strncmp.c.o itoa.c.o utoa.c.o \
            memset.c.o memcpy.c.o memcmp.c.o

OBJS    = $(addprefix $(OBJ_DIR)/, $(LIBC_OBJS))
LIB     = $(OBJ_DIR)/libc.a
REL_LIB = $(subst $(abspath ..)/,,$(LIB))

INTERNAL_CFLAGS = $(CFLAGS) -Iinclude

MSG_PREFIX  = libc
CFLAGS_USER = $(INTERNAL_CFLAGS)
EXTRA_DEP   =

include ../scripts/rules.mk

.PHONY: all objects archive clean single

all: archive

objects: $(OBJS)

archive: $(LIB)

$(LIB): $(OBJS)
	@echo "  AR       $(REL_LIB)"
	@mkdir -p $(dir $@)
	@$(AR) rcs $@ $(OBJS)

ifdef FILE
single: $(OBJ_DIR)/$(FILE).c.o
else
single:
endif

clean:
	@rm -rf $(OBJ_DIR)
