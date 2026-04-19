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
          char buffer[12];  //足够存储32位整数 (-2147483648 到 2147483647)
          int idx = 0;
          
          // 处理负数
          if (num < 0) {
            tty_putchar('-');
            num = -num;
          }
          
          // 特殊情况: num == 0
          if (num == 0) {
            tty_putchar('0');
          } else {
            // 将数字转换为字符串 (反向存储)
            while (num > 0) {
              buffer[idx++] = '0' + (num % 10);
              num /= 10;
            }
            // 反向输出
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
