CC = i686-elf-gcc
AR = i686-elf-ar
CFLAGS_LOCAL = -Iinclude -ffreestanding -O2 -Wall -Wextra

SRC_DIR = src
BUILD_DIR ?= ../build
LIBC_BUILD_DIR = $(BUILD_DIR)/libc

LIBC_OBJS = strlen.o strcpy.o strcmp.o \
						strncmp.o itoa.o utoa.o
OBJECTS = $(addprefix $(LIBC_BUILD_DIR)/, $(LIBC_OBJS))
LIBRARY = $(BUILD_DIR)/libc.a

.PHONY: all clean

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $(OBJECTS)

$(LIBC_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(LIBC_BUILD_DIR)
	$(CC) $(CFLAGS_LOCAL) -c $< -o $@

clean:
	rm -rf $(LIBC_BUILD_DIR) $(LIBRARY)
