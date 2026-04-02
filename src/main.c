#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "tty.h"

void kmain(void) {
  /* Initialize terminal interface */
  terminal_initialize();

  terminal_writestring("Hello, kernel World!\n");
}
