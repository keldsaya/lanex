#include <string.h>

char* utoa(unsigned int value, char* str, int base) {
  char* rc;
  char* ptr;
  char* low;

  rc = ptr = str;

  if (base < 2 || base > 36) {
    *str = '\0';
    return str;
  }

  do {
    *ptr++ = "0123456789abcdef"[value % base];
    value /= base;
  } while (value);

  low = str;
  ptr--;

  while (low < ptr) {
    char tmp = *low;
    *low++ = *ptr;
    *ptr-- = tmp;
  }

  return rc;
}
