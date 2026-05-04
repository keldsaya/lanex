#ifndef SH_HISTORY_H
#define SH_HISTORY_H

#include <stddef.h>

#define HIST_SIZE 100
#define MAX_LINE 256

void history_init(void);
void history_add(const char *cmd);
const char* history_get_up(int *nav);
const char* history_get_down(int *nav);
void history_reset_navigation(void);
int history_get_count(void);
const char* history_get_entry(int index);

#endif /* SH_HISTORY_H */
