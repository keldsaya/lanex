#include "vga.h"
#include "cursor.h"
#include "tty.h"

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer = (uint16_t *)VGA_MEMORY;

static size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

void terminal_initialize(void) {
  cursor_enable(14,15);
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}

void terminal_setcolor(uint8_t color) { terminal_color = color; }

void terminal_scroll() {
  for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t src_index = (y + 1) * VGA_WIDTH + x;
      const size_t dst_index = y * VGA_WIDTH + x;
      terminal_buffer[dst_index] = terminal_buffer[src_index];
    }
  }

  const size_t last_row_start = (VGA_HEIGHT - 1) * VGA_WIDTH;
  for (size_t x = 0; x < VGA_WIDTH; x++) {
    terminal_buffer[last_row_start + x] = vga_entry(' ', terminal_color);
  }
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
  cursor_update(x, y);
}

void terminal_putchar(char c) {
  if (c == '\n') {
    terminal_row++;
    terminal_column = 0;
  } else if (c == '\r') {
    terminal_column = 0;
  } else if (c == '\b') {
    if (terminal_column > 0) {
      terminal_column--;
    } else if (terminal_row > 0) {
      terminal_row--;
      terminal_column = VGA_WIDTH - 1;
    }
    terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
    return;
  } else {
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
      terminal_column = 0;
      terminal_row++;
    }
  }

  if(terminal_row >= VGA_HEIGHT) {
    terminal_scroll();
    terminal_row = VGA_HEIGHT - 1;
  }
  cursor_update(terminal_column, terminal_row);
}

void terminal_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
}

void terminal_writestring(const char *data) {
  terminal_write(data, strlen(data));
}
