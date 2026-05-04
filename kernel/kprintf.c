#include <lanex/tty.h>
#include <stdarg.h>
#include <stdio.h>

void kprintf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  while (*fmt) {
    if (*fmt == '%') {
      fmt++;
      switch (*fmt) {
      case 's': {
        const char *str = va_arg(args, const char *);
        while (*str) {
          tty_putchar(*str);
          str++;
        }
        break;
      }
      case 'c': {
        char c = (char)va_arg(args, int);
        tty_putchar(c);
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
      case 'u': {
        unsigned int num = va_arg(args, unsigned int);
        char buffer[12];
        int idx = 0;

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
      case 'x': {
        unsigned int num = va_arg(args, unsigned int);
        char buffer[12];
        int idx = 0;

        if (num == 0) {
          tty_putchar('0');
        } else {
          while (num > 0) {
            uint8_t digit = num % 16;
            buffer[idx++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
            num /= 16;
          }
          while (idx > 0) {
            tty_putchar(buffer[--idx]);
          }
        }
        break;
      }
      case 'X': {
        unsigned int num = va_arg(args, unsigned int);
        char buffer[12];
        int idx = 0;

        if (num == 0) {
          tty_putchar('0');
        } else {
          while (num > 0) {
            uint8_t digit = num % 16;
            buffer[idx++] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
            num /= 16;
          }
          while (idx > 0) {
            tty_putchar(buffer[--idx]);
          }
        }
        break;
      }
      case 'p': {
        uint32_t ptr = va_arg(args, uint32_t);
        tty_putchar('0');
        tty_putchar('x');

        char buffer[8];
        int idx = 0;
        for (int i = 7; i >= 0; i--) {
          uint8_t digit = (ptr >> (i * 4)) & 0x0F;
          buffer[idx++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        }
        for (int i = 0; i < 8; i++) {
          tty_putchar(buffer[i]);
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
