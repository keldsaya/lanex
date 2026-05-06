#include <lanex/sh.h>
#include <lanex/tty.h>
#include <keyboard/keyboard.h>
#include <string.h>
#include <stdio.h>
#include "history.h"

char line[MAX_LINE];
size_t line_pos = 0;

void sh_char(const char c) {
  const char *hist_entry;

  if (c == KEY_UP) {
    hist_entry = history_get_up(&history_nav);
    if (hist_entry) {
      strcpy(line, hist_entry);
      line_pos = strlen(line);
      tty_clear_current_row();
      sh_prompt();
      kprintf("%s", line);
    }
  } else if (c == KEY_DOWN) {
    hist_entry = history_get_down(&history_nav);
    if (hist_entry) {
      strcpy(line, hist_entry);
      line_pos = strlen(line);
      tty_clear_current_row();
      sh_prompt();
      kprintf("%s", line);
    } else {
      memset(line, 0, MAX_LINE);
      line_pos = 0;
      tty_clear_current_row();
      sh_prompt();
    }
  } else if (c == KEY_LEFT) {
    if (line_pos > 0) {
      line_pos--;
      tty_move_cur(-1);
    }
  } else if (c == KEY_RIGHT) {
    if (line_pos < strlen(line)) {
      line_pos++;
      tty_move_cur(1);
    }
  } else if (c == '\n') {
    tty_putchar(c);
    command(line);
  } else if (c == '\b') {
    if (line_pos > 0 && strlen(line) > 0) {
      for (size_t i = line_pos - 1; i < strlen(line); i++) {
        line[i] = line[i + 1];
      }
      line_pos--;
      line[strlen(line)] = '\0';

      tty_clear_current_row();
      sh_prompt();
      kprintf("%s", line);

      for (size_t i = line_pos; i < strlen(line); i++) {
        tty_move_cur(-1);
      }
    }
  } else if (c >= 32 && c <= 126) {
    size_t len = strlen(line);
    if (len < MAX_LINE - 1) {
      for (size_t i = len; i > line_pos; i--) {
        line[i] = line[i - 1];
      }
      line[line_pos] = c;
      line_pos++;

      tty_clear_current_row();
      sh_prompt();
      kprintf("%s", line);

      for (size_t i = line_pos; i < strlen(line); i++) {
        tty_move_cur(-1);
      }
    }
  }
}

