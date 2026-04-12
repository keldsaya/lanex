#include "io.h"
#include "keyboard.h"
const char lower_scancodes[128] = {
  0, 27, '1', '2', '3', '4', '5', '6',
  '7', '8', '9', '0', '-', '=', '\b', '\t',
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
  'o', 'p', '[', ']', '\n', 0, 'a', 's',
  'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
  '\'', '`', '\n', '\\', 'z', 'x', 'c', 'v',
  'b', 'n', 'm', ',', '.', '/', 0, '*',
  0, ' ', 0, 0, 0, 0, 0, 0
};

const char upper_scancodes[128] = {
  0, 27, '!', '@', '#', '$', '%', '^',
  '&', '*', '(', ')', '_', '+', '\b', '\t',
  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
  'O', 'P', '{', '}', '\n', 0, 'A', 'S',
  'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
  '\"', '~', '\n', '|', 'Z', 'X', 'C', 'V',
  'B', 'N', 'M', '<', '>', '?', 0, '*',
  0, ' ', 0, 0, 0, 0, 0, 0
};


static volatile char last_key = 0;
void keyboard_handler() {
  uint8_t status = inb(0x64);
  if (status & 0x01) {
    uint8_t scancode = inb(0x60);
    if (!(scancode & 0x80)) {
      last_key = lower_scancodes[scancode];
    }
  }
  outb(0x20, 0x20);
}

char keyboard_get_char() {
  char c = last_key;
  last_key = 0;
  return c;
}
