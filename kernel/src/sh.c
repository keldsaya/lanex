#include "sh.h"
#include "config.h"
#include "keyboard/keyboard.h"
#include "main.h"
#include "messages.h"
#include "pit.h"
#include "pmm.h"
#include "power/power.h"
#include "rtc/rtc.h"
#include "tty.h"
#include <stdio.h>
#include <string.h>

const char prompt[] = "> ";
char line[MAX_LINE];
size_t line_pos = 0;

static char history[HIST_SIZE][MAX_LINE];
static int history_count = 0;
static int history_nav = -1;

void add_history(const char *cmd) {
  if (cmd[0] == 0 || cmd[0] == '\0')
    return;
  if (history_count > 0 &&
      strcmp(history[(history_count - 1) % HIST_SIZE], cmd) == 0)
    return;

  strcpy(history[history_count % HIST_SIZE], cmd);
  history_count++;

  if (history_count > HIST_SIZE) {
    history_count = HIST_SIZE;
  }
}

void execute(const char *cmd) {
  if (strcmp(cmd, "clear") == 0) {
    tty_clear();
  }
#ifdef CONFIG_DRIVER_POWER
  else if (strcmp(cmd, "shutdown") == 0) {
    power_shutdown();
  } else if (strcmp(cmd, "reboot") == 0) {
    power_reboot();
  }
#endif
#ifdef CONFIG_DRIVER_RTC
  else if (strcmp(cmd, "date") == 0) {
    rtc_print_datetime();
  }
#endif
  else if (strcmp(cmd, "free") == 0) {
    uint32_t total = k_get_mem();
    uint32_t free_pages = pmm_free_pages_count();
    uint32_t free_kb = free_pages * 4;
    kprintf("Total: %uKB, Free: %uKB, Used: %uKB\n", total, free_kb,
            total - free_kb);
  } else if (strcmp(cmd, "uptime") == 0) {
    uint32_t ticks = pit_get_ticks();
    kprintf("uptime: %d seconds\n", ticks / 1000);
  } else if (strncmp(cmd, "echo", 4) == 0) {
    if (strlen(cmd) > 5)
      kprintf("%s\n", cmd + 5);
    else
      kprintf("\n");
  } else if (strcmp(cmd, "panic") == 0) {
    kpanic("Called by user");
  } else if (strcmp(cmd, "help") == 0) {
    kprintf("  clear - Clear screen\n");
#ifdef CONFIG_DRIVER_POWER
    kprintf("  shutdown - Shutdown system\n");
    kprintf("  reboot - Reboot system\n");
#endif
#ifdef CONFIG_DRIVER_RTC
    kprintf("  date - Show time\n");
#endif
    kprintf("  free - Show memory\n");
    kprintf("  uptime - Show uptime\n");
    kprintf("  echo - Print\n");
    kprintf("  panic - Show kernel panic\n");
    kprintf("  help - Show this\n");
  } else if (cmd[0] != '\0') {
    kprintf("sh: Unknown command: %s\n", cmd);
  }
}

void command(char *cmd) {
  if (strlen(cmd) > 0) {
    add_history(cmd);
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
  history_count = 0;
}

void sh_prompt() { kprintf(prompt); }

void sh_char(const char c) {
  if (c == KEY_UP || c == KEY_DOWN) {
    if (history_count == 0)
      return;

    if (c == KEY_UP) {
      if (history_nav == -1) {
        history_nav = history_count - 1;
      } else if (history_nav > 0) {
        history_nav--;
      }
    } else {
      if (history_nav == -1)
        return;
      history_nav++;
      if (history_nav >= history_count) {
        history_nav = -1;
        memset(line, 0, MAX_LINE);
        line_pos = 0;
        tty_clear_current_row();
        sh_prompt();
        return;
      }
    }

    if (history_nav >= 0 && history_nav < history_count) {
      strcpy(line, history[history_nav]);
      line_pos = strlen(line);
      tty_clear_current_row();
      sh_prompt();
      kprintf("%s", line);
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
