#ifndef H_SH
#define H_SH
#include <stddef.h>

#define MAX_CWD_SIZE 256

extern int history_nav;
extern size_t line_pos;
extern char line[];

char *get_cwd();
void sh_cd(const char *path);
void command(char *cmd);
void sh_init();
void sh_prompt();
void sh_main();

#endif /* H_SH */
