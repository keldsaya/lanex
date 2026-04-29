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

void* memset(void* ptr, int value, size_t num);
void* memcpy(void* dest, const void* src, size_t num);
int memcmp(const void* ptr1, const void* ptr2, size_t num);

#endif /* H_STRING */
