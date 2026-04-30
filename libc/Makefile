SRC_DIR = src
L_BUILD_DIR = $(BUILD_DIR)/libc

LIBC_OBJS = strlen.c.o strcpy.c.o strcmp.c.o \
            strncmp.c.o itoa.c.o utoa.c.o \
						memset.c.o memcpy.c.o memcmp.c.o

OBJS = $(addprefix $(L_BUILD_DIR)/, $(LIBC_OBJS))
LIB  = $(L_BUILD_DIR)/libc.a

INTERNAL_CFLAGS = $(CFLAGS) -Iinclude

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJS)
	@mkdir -p $(dir $@)
	@$(AR) rcs $@ $(OBJS) 

$(L_BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@echo "  CC      libc/$<"
	@mkdir -p $(L_BUILD_DIR)
	@$(CC) $(INTERNAL_CFLAGS) -c $< -o $@
