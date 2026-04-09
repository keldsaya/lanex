#include "tty.h"

void kmain();
void _start() { kmain(); }

void kmain() {
  /* Initialize terminal interface */
  terminal_initialize();

  terminal_writestring("Hello, kernel World!\n");
  while(1) {
  }
}
