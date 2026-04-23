SRC_DIR = src
L_BUILD_DIR = $(BUILD_DIR)/libc

LIBC_OBJS = strlen.o strcpy.o strcmp.o \
            strncmp.o itoa.o utoa.o

OBJS = $(addprefix $(L_BUILD_DIR)/, $(LIBC_OBJS))
LIB  = $(L_BUILD_DIR)/libc.a

INTERNAL_CFLAGS = $(CFLAGS) -Iinclude

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJS)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $(OBJS) 

$(L_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(L_BUILD_DIR)
	$(CC) $(INTERNAL_CFLAGS) -c $< -o $@

clean:
	rm -rf $(L_BUILD_DIR)
