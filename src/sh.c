#include "sh.h"
#include "tty.h"
#include "keyboard.h"

const char prompt[] = "> ";

void sh_prompt() {
  tty_writestring(prompt);
}

void sh_main() {
  sh_prompt();
  while(1) {
    char c = keyboard_get_char();
    if(c == 0) continue;
    tty_putchar(c);
    if(c == '\n') {
      sh_prompt();
    }
  }
}
