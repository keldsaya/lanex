#ifndef H_KEYBOARD
#define H_KEYBOARD

extern const char lower_scancodes[128];
extern const char upper_scancodes[128];

#define KEY_ESC 0x05
#define KEY_UP 0x01
#define KEY_DOWN 0x02
#define KEY_LEFT 0x03
#define KEY_RIGHT 0x04

void keyboard_handler();
char keyboard_get_char();

#endif /* H_KEYBOARD */
