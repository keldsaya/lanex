#include <lanex/sh.h>
#include <lanex/config.h>
#include <keyboard/keyboard.h>
#include <lanex/tty.h>
#include <stdio.h>
#include <string.h>

#include "cmd.h"
#include "history.h"
#include "char.h"

const char prompt[] = "> ";
int history_nav = -1;

char cwd[MAX_CWD_SIZE] = "/";

char *get_cwd() {
  return cwd;
}

void sh_cd(const char *path) {
  char temp[1024];
  char *token;

  if (path[0] == '/') {
    strcpy(temp, path);
  } else {
    strcpy(temp, cwd);
    if (temp[strlen(temp) - 1] != '/')
      strcat(temp, "/");
    strcat(temp, path);
  }

  char result[1024] = "";
  char *parts[100];
  int count = 0;

  token = strtok(temp, "/");
  while (token != NULL) {
    if (strcmp(token, "..") == 0) {
      if (count > 0)
        count--;
    } else if (strcmp(token, ".") != 0 && strcmp(token, "") != 0) {
      parts[count++] = token;
    }
    token = strtok(NULL, "/");
  }

  if (count == 0) {
    strcpy(cwd, "/");
  } else {
    strcpy(result, "/");
    for (int i = 0; i < count; i++) {
      if (i > 0)
        strcat(result, "/");
      strcat(result, parts[i]);
    }
    strcpy(cwd, result);
  }
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
}

void sh_prompt() {
  kprintf("%s %s", cwd, prompt);
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
