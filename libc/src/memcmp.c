#include <string.h>

int memcmp(const void *ptr1, const void *ptr2, size_t num) {
  const unsigned char *p1 = ptr1;
  const unsigned char *p2 = ptr2;
  while (num--) {
    if (*p1 != *p2)
      return *p1 - *p2;
    p1++;
    p2++;
  }
  return 0;
}
