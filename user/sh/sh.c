#include <lanex/sh.h>
#include <lanex/config.h>
#include <keyboard/keyboard.h>
#include <lanex/tty.h>
#include <stdio.h>
#include <string.h>
#include "cmd.h"
#include "history.h"
#include "char.h"
#include "path.h"

const char prompt[] = "> ";
int history_nav = -1;

void sh_cd(const char *path) {
  change_directory(path);
}

void command(char *cmd) {
  if (strlen(cmd) > 0) {
    history_add(cmd);
    execute(cmd);
  }
  memset(line, 0, MAX_LINE);
  line_pos = 0;
  history_nav = -1;
  sh_prompt();
}

void sh_init() {
  line_pos = 0;
  memset(line, 0, MAX_LINE);
  history_nav = -1;
  history_init();
  path_init();
}

void sh_prompt() {
  kprintf("%s %s", get_cwd(), prompt);
}

void sh_main() {
  sh_prompt();
  for (;;) {
    asm volatile("hlt");
#ifdef CONFIG_DRIVER_KEYBOARD
    char c = keyboard_get_char();
#else
    char c = 0;
#endif
    if (c)
      sh_char(c);
  }
}
