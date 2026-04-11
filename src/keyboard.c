#include "io.h"
#include "tty.h"
#include "keyboard.h"
static int shift_pressed = 0;
char lower_scancodes[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
  '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
};

char upper_scancodes[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
  '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0,  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0,
  '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0
};

void keyboard_handler() {
  uint8_t scancode = inb(0x60);

  if(scancode & 0x80) {
    uint8_t released_scancode = scancode & 0x7F;
    if(released_scancode == 0x2A || released_scancode == 0x36) {
      shift_pressed = 0;
    }
  } else {
    if(scancode == 0x2A || scancode == 0x36) {
      shift_pressed = 1;
    } else if(scancode == 0x1C) {
      terminal_putchar('\n');
    } else if(scancode == 0x39) {
      terminal_putchar(' ');
    } else if(scancode == 0x0E) {
      terminal_putchar('\b');
    } else if(scancode < 128) {
      char c = shift_pressed ? upper_scancodes[scancode] : lower_scancodes[scancode];
      if(c > 0) { terminal_putchar(c); }
    }
  }

  outb(0x20, 0x20);
}
