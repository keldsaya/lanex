#include <lanex/mm/kmalloc.h>
#include <lanex/pmm.h>

#define PAGE_SIZE 4096
#define MIN_ALLOC 16

typedef struct block_header {
  size_t size;              
  struct block_header *next;
} block_header_t;

static int initialized = 0;

void kmalloc_init(void) {
  initialized = 1;
}

void *kmalloc(size_t size) {
  if (!initialized || size == 0)
    return NULL;
    
  size_t total = size + sizeof(block_header_t);
    
  if (total < MIN_ALLOC + sizeof(block_header_t))
  total = MIN_ALLOC + sizeof(block_header_t);
    
  size_t pages = (total + PAGE_SIZE - 1) / PAGE_SIZE;
    
  void *mem = pmm_alloc_page();  
  if (!mem)
    return NULL;
    
  for (size_t i = 1; i < pages; i++) {
    pmm_alloc_page();  
  }
    
  block_header_t *hdr = (block_header_t *)mem;
  hdr->size = size;
  hdr->next = NULL;
    
  return (void *)(hdr + 1);
}

void kfree(void *ptr) {
  if (!ptr)
    return;
    
  block_header_t *hdr = ((block_header_t *)ptr) - 1;
    
  pmm_free_page((void *)hdr);
}

void *krealloc(void *ptr, size_t size) {
  if (!ptr)
    return kmalloc(size);
    
  if (size == 0) {
    kfree(ptr);
    return NULL;
  }
    
  block_header_t *hdr = ((block_header_t *)ptr) - 1;
    
  if (size <= hdr->size) {
    hdr->size = size;
    return ptr;
  }
    
  void *new_ptr = kmalloc(size);
  if (!new_ptr)
    return NULL;
    
  char *dst = (char *)new_ptr;
  char *src = (char *)ptr;
  for (size_t i = 0; i < hdr->size; i++)
    dst[i] = src[i];
    
  kfree(ptr);
  return new_ptr;
}
