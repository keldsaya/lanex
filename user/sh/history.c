#include "history.h"
#include <string.h>

static char history[HIST_SIZE][MAX_LINE];
static int history_count = 0;

void history_init(void) {
  history_count = 0;
  memset(history, 0, sizeof(history));
}

void history_add(const char *cmd) {
  if (cmd[0] == '\0')
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

const char* history_get_up(int *nav) {
  if (history_count == 0)
    return NULL;

  if (*nav == -1) {
    *nav = history_count - 1;
  } else if (*nav > 0) {
    (*nav)--;
  }

  return history[*nav];
}

const char* history_get_down(int *nav) {
  if (history_count == 0 || *nav == -1)
    return NULL;

  (*nav)++;

  if (*nav >= history_count) {
    *nav = -1;
    return NULL;
  }

  return history[*nav];
}

void history_reset_navigation(void) {
}

int history_get_count(void) {
  return history_count;
}

const char* history_get_entry(int index) {
  if (index < 0 || index >= history_count)
    return NULL;
  return history[index];
}
