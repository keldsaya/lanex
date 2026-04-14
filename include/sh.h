#ifndef H_SH
#define H_SH
#define MAX_LINE 128
#define HIST_SIZE 128

void command(char *cmd);
void sh_init();
void sh_prompt();
void sh_char(const char c);
void sh_main();

#endif /* H_SH */
