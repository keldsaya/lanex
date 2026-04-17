#include <string.h>

int strncmp(const char* a, const char* b, int n) {
  while (n-- && *a && *b && *a == *b) { a++; b++; }
  return n < 0 ? 0 : *a - *b;
}
