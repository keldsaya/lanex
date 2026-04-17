#ifndef H_STRING
#define H_STRING
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *str);
int strcmp(const char *s1, const char *s2);
int strncmp(const char* a, const char* b, int n);
void strcpy(char* dst, const char* src);
void itoa(uint32_t n, char* str);
char* utoa(unsigned int value, char* str, int base);

#endif /* H_STRING */
