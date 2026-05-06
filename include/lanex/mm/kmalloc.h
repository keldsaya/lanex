#ifndef H_KMALLOC
#define H_KMALLOC

#include <stddef.h>
#include <stdint.h>

void  kmalloc_init(void);
void *kmalloc(size_t size);
void  kfree(void *ptr);
void *krealloc(void *ptr, size_t size);

#endif
