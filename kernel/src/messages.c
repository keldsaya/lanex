#include "messages.h"
#include "tty.h"
#include "vga/vga.h"
#include <stdio.h>

void kpanic(const char *msg) {
  tty_clear();
  tty_setcolor(VGA_COLOR_LIGHT_MAGENTA);
  kprintf("\n  (\\ /)\n");
  kprintf("  ( o o)\n");
  kprintf("  c(*)(*)\n\n");
  tty_setcolor(VGA_COLOR_LIGHT_GREY);
  kprintf("Kernel panic!: %s\n", msg);
  kprintf("\n\nPlease reboot computer");
  asm volatile("cli; hlt");
}

void welcome() {
  tty_setcolor(VGA_COLOR_LIGHT_MAGENTA);
  kprintf("\n  (\\ /)\n");
  kprintf("  ( . .)\n");
  kprintf("  c(*)(*)\n\n");
  tty_setcolor(VGA_COLOR_WHITE);
  kprintf("Welcome to ");
  tty_setcolor(VGA_COLOR_LIGHT_CYAN);
  kprintf("Lanex\n");
  tty_setcolor(VGA_COLOR_LIGHT_GREY);
}
