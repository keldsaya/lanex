#include "vga.h"
#include "cursor.h"
#include "tty.h"

size_t tty_row;
size_t tty_column;
uint8_t tty_color;
uint16_t *tty_buffer = (uint16_t *)VGA_MEMORY;

static size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

void tty_clear() {
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      tty_buffer[index] = vga_entry(' ', tty_color);
    }
  }
  tty_row = 0;
  tty_column = 0;
}
void tty_clear_current_row() {
  const size_t row_start = tty_row * VGA_WIDTH;
  for (size_t x = 0; x < VGA_WIDTH; x++) {
    tty_buffer[row_start + x] = vga_entry(' ', tty_color);
  }
  tty_column = 0; 
  cursor_update(tty_column, tty_row);}

void tty_initialize(void) {
  cursor_enable(14,15);
  tty_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  tty_clear();
}

void tty_setcolor(uint8_t color) { tty_color = color; }

void tty_scroll() {
  for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t src_index = (y + 1) * VGA_WIDTH + x;
      const size_t dst_index = y * VGA_WIDTH + x;
      tty_buffer[dst_index] = tty_buffer[src_index];
    }
  }

  const size_t last_row_start = (VGA_HEIGHT - 1) * VGA_WIDTH;
  for (size_t x = 0; x < VGA_WIDTH; x++) {
    tty_buffer[last_row_start + x] = vga_entry(' ', tty_color);
  }
}

void tty_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  tty_buffer[index] = vga_entry(c, color);
  cursor_update(x, y);
}

void tty_putchar(char c) {
  if (c == '\n') {
    tty_row++;
    tty_column = 0;
  } else if (c == '\r') {
    tty_column = 0;
  } else if (c == '\b') {
    if (tty_column > 0) {
      tty_column--;
    } else if (tty_row > 0) {
      tty_row--;
      tty_column = VGA_WIDTH - 1;
    }
    tty_putentryat(' ', tty_color, tty_column, tty_row);
    return;
  } else {
    tty_putentryat(c, tty_color, tty_column, tty_row);
    if (++tty_column == VGA_WIDTH) {
      tty_column = 0;
      tty_row++;
    }
  }

  if(tty_row >= VGA_HEIGHT) {
    tty_scroll();
    tty_row = VGA_HEIGHT - 1;
  }
  cursor_update(tty_column, tty_row);
}

void tty_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    tty_putchar(data[i]);
}

void tty_writestring(const char *data) {
  tty_write(data, strlen(data));
}
char tty_last_char() {
  size_t x = tty_column;
  size_t y = tty_row;

  if (x == 0) {
    if (y == 0) {
      return ' '; 
    }
    x = VGA_WIDTH - 1;
    y--;
  } else {
    x--;
  }

  const size_t index = y * VGA_WIDTH + x;
  return (char)(tty_buffer[index] & 0xFF);
}
