#include <stdio.h>
#include <stdarg.h>
#include "tty.h"

void kprintf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  
  while (*fmt) {
    if (*fmt == '%') {
      fmt++;
      switch (*fmt) {
        case 's': {
          const char* str = va_arg(args, const char*);
          while (*str) {
            tty_putchar(*str);
            str++;
          }
          break;
        }
        case 'd': {
          int num = va_arg(args, int);
          char buffer[12]; 
          int idx = 0;
      
          if (num < 0) {
            tty_putchar('-');
            num = -num;
          }
          
          if (num == 0) {
            tty_putchar('0');
          } else {
            while (num > 0) {
              buffer[idx++] = '0' + (num % 10);
              num /= 10;
            }
            while (idx > 0) {
              tty_putchar(buffer[--idx]);
            }
          }
          break;
        }
        case '%':
          tty_putchar('%');
          break;
        default:
          tty_putchar('%');
          tty_putchar(*fmt);
          break;
      }
    } else {
      tty_putchar(*fmt);
    }
    fmt++;
  }
  
  va_end(args);
}
