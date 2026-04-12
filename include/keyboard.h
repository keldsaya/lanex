#ifndef H_KEYBOARD
#define H_KEYBOARD

extern const char lower_scancodes[128];
extern const char upper_scancodes[128];

void keyboard_handler();
char keyboard_get_char();

#endif /* H_KEYBOARD */
