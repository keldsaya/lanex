#ifndef H_STDLIB
#define H_STDLIB
#include <stddef.h>

void* malloc(size_t size);
void free(void* ptr);
void* calloc(size_t num, size_t size);
void* realloc(void* ptr, size_t size);

#endif /* H_STDLIB */
