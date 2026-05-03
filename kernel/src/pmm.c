#include <lanex/pmm.h>

#define PAGE_SIZE 4096
#define BITS_PER_BYTE 8

static uint8_t *bitmap = NULL;
static uint32_t total_pages = 0;
static uint32_t next_free_page = 0;

extern uint32_t kernel_start;
extern uint32_t kernel_end;

static inline void set_page(uint32_t p, uint8_t v) {
  uint32_t byte = p / BITS_PER_BYTE;
  uint8_t bit = p % BITS_PER_BYTE;
  if (v)
    bitmap[byte] |= (1 << bit);
  else
    bitmap[byte] &= ~(1 << bit);
}

void pmm_init(uint32_t mem_kb) {
  total_pages = (mem_kb * 1024) / PAGE_SIZE;
  size_t bitmap_bytes = (total_pages + BITS_PER_BYTE - 1) / BITS_PER_BYTE;
  uint32_t bitmap_addr =
      ((uint32_t)&kernel_end + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
  bitmap = (uint8_t *)bitmap_addr;

  for (size_t i = 0; i < bitmap_bytes; i++)
    bitmap[i] = 0xFF;

  uint32_t bitmap_end = bitmap_addr + bitmap_bytes;
  uint32_t free_start =
      ((bitmap_end + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1)) / PAGE_SIZE;
  for (uint32_t i = free_start; i < total_pages; i++)
    set_page(i, 0);

  for (uint32_t i = 0; i < 0x100000 / PAGE_SIZE; i++)
    set_page(i, 1);

  next_free_page = free_start;
}

void *pmm_alloc_page(void) {
  for (uint32_t i = next_free_page; i < total_pages; i++) {
    uint32_t byte = i / BITS_PER_BYTE;
    uint8_t bit = i % BITS_PER_BYTE;
    if (!(bitmap[byte] & (1 << bit))) {
      bitmap[byte] |= (1 << bit);
      next_free_page = i + 1;
      return (void *)(i * PAGE_SIZE);
    }
  }
  return NULL;
}

void pmm_free_page(void *addr) {
  uint32_t page = (uint32_t)addr / PAGE_SIZE;
  if (page >= total_pages)
    return;
  set_page(page, 0);
  if (page < next_free_page)
    next_free_page = page;
}

uint32_t pmm_free_pages_count(void) {
  uint32_t free_pages = 0;
  for (uint32_t i = 0; i < total_pages; i++) {
    uint32_t byte = i / BITS_PER_BYTE;
    uint8_t bit = i % BITS_PER_BYTE;
    if (!(bitmap[byte] & (1 << bit))) {
      free_pages++;
    }
  }
  return free_pages;
}
