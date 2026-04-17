#include <string.h>

void strcpy(char* dst, const char* src) {
  while ((*dst++ = *src++));
}
