#ifndef H_TTY
#define H_TTY

#include <stddef.h>
#include <stdint.h>

void tty_initialize(void);
void tty_setcolor(uint8_t color);
void tty_putchar(char c);
void tty_write(const char *data, size_t size);
void tty_clear(void);
void tty_clear_current_row(void);
char tty_last_char(void);

#endif
