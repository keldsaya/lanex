#include <string.h>

void itoa(uint32_t n, char* str) {
  char temp[12];
  int i = 0;

  if (n == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return;
  }

  while (n > 0) {
    temp[i++] = (n % 10) + '0';
    n /= 10;
  }

  int len = i;
  for (int j = 0; j < len; j++) {
    str[j] = temp[len - j - 1];
  }
  str[len] = '\0';
}
