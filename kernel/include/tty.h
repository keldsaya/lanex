#ifndef H_TTY
#define H_TTY
#include <stdint.h>
#include <stddef.h>

extern size_t tty_row;
extern size_t tty_column;
extern uint8_t tty_color;
extern uint16_t *tty_buffer;

void tty_clear();
void tty_clear_current_row();
void tty_initialize(void);
void tty_setcolor(uint8_t color); 
void tty_putentryat(char c, uint8_t color, size_t x, size_t y);
void tty_putchar(char c);
void tty_write(const char *data, size_t size);
char tty_last_char();

#endif
