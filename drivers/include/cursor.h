#ifndef H_CURSOR
#define H_CURSOR
#include <stdint.h>
void cursor_enable(uint8_t start, uint8_t end);
void cursor_disable();
void cursor_update(int x, int y);
uint16_t get_cursor_position(void);
#endif /* H_CURSOR */
