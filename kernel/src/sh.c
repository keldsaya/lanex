#include <string.h>
#include <stdio.h>
#include "sh.h"
#include "tty.h"
#include "keyboard.h"
#include "pit.h"     
#include "messages.h"
const char prompt[] = "> ";
char line[MAX_LINE];
size_t line_pos = 0;

static char history[HIST_SIZE][MAX_LINE];
static int history_count = 0;
static int history_nav = 0;

void add_history(const char *cmd) {
  if(cmd[0] == 0 || cmd[0] == '\0') return;
  if(history_count > 0 &&
      strcmp(history[(history_count - 1) % HIST_SIZE], cmd) == 0) return;
  strcpy(history[history_count % HIST_SIZE], cmd);
  history_count++; 
}

void execute(const char *cmd) {
  if(strcmp(cmd, "clear") == 0) {
    tty_clear();
  } else if(strcmp(cmd, "uptime") == 0) {
    uint32_t ticks = pit_get_ticks();
    kprintf("uptime: %d seconds\n", ticks / 1000);
  } else if(strncmp(cmd, "echo", 4) == 0) {
    kprintf("%s\n", cmd + 5);
  } else if(strcmp(cmd, "panic") == 0) {
    kpanic("Called by user");
  } else if(strcmp(cmd, "help") == 0) {
    kprintf("  clear - Clear screen\n");
    kprintf("  uptime - Show uptime\n");
    kprintf("  echo - Print\n");
    kprintf("  panic - Show kernel panic\n");
    kprintf("  help - Show this\n");
  } else if(line_pos != 0) {
    kprintf("sh: Unknown command: %s\n", cmd);
  }

  if(tty_last_char() == '\n')
    tty_putchar('\n');
}

void command(char *cmd) {
  add_history(cmd);
  execute(cmd);
    
  for (size_t i = 0; i < MAX_LINE; i++) {
    line[i] = '\0';
  }
  line_pos = 0;
    
  sh_prompt();
}
void sh_init() {
  line_pos = 0;
}
void sh_prompt() {
  kprintf(prompt);
}

void sh_char(const char c) {
  if(c == KEY_UP || c == KEY_DOWN) {  
    if(history_count == 0) return;
    if(c == KEY_UP) {
      if (history_nav == -1) history_nav = history_count - 1;
      else if (history_nav > 0 && history_nav > (history_count - HIST_SIZE)) 
        history_nav--;
    } 
    else {
      if(history_nav == -1) return;
      history_nav++;
      if(history_nav >= history_count) {
        history_nav = -1;
        line[0] = '\0';
        line_pos = 0;
        tty_clear_current_row();
        sh_prompt();
        return;
      }
    }
    strcpy(line, history[history_nav % HIST_SIZE]);
    line_pos = 0;
    while(line[line_pos]) line_pos++;
    tty_clear_current_row();
    sh_prompt();
    kprintf(line);
  } else if(c == '\n') {
    tty_putchar(c);
    line[line_pos] = '\0';
    history_nav = -1;
    command(line);
  } else if(c == '\b') {
    if (line_pos > 0) {
      line_pos--;
      tty_putchar('\b');
    }
  } else {
    if(line_pos < MAX_LINE - 1) {
      line[line_pos++] = c;
      tty_putchar(c);
    }
  }
}

void sh_main() {
  sh_prompt();
  for(;;) {
    asm volatile("hlt");
    char c = keyboard_get_char();
    if(c)
      sh_char(c);
  }
}
