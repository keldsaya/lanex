#include "messages.h"
#include "tty.h"

void kpanic(const char *msg) {
  tty_clear();
  tty_setcolor(VGA_COLOR_LIGHT_MAGENTA);
  tty_writestring("\n  (\\ /)\n");
  tty_writestring("  ( o o)\n");
  tty_writestring("  c(*)(*)\n\n");
  tty_setcolor(VGA_COLOR_LIGHT_GREY);
  tty_writestring("Kernel panic!: ");
  tty_writestring(msg);
  tty_putchar('\n');
  tty_writestring("\n\nPlease reboot computer");
  asm volatile ("cli; hlt");
}

void welcome() {
  tty_setcolor(VGA_COLOR_LIGHT_MAGENTA);
  tty_writestring("\n  (\\ /)\n");
  tty_writestring("  ( . .)\n");
  tty_writestring("  c(*)(*)\n\n");
  tty_setcolor(VGA_COLOR_WHITE);
  tty_writestring("Welcome to ");
  tty_setcolor(VGA_COLOR_LIGHT_CYAN);
  tty_writestring("Lanex\n");
  tty_setcolor(VGA_COLOR_LIGHT_GREY);
}
