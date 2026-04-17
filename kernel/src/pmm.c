#include "pmm.h"

uint32_t* pmm_bitmap;
uint32_t  pmm_total_pages;
uint32_t  pmm_bitmap_size;

void pmm_set_bit(uint32_t page) {
  pmm_bitmap[page / 32] |= (1 << (page % 32));
}

void pmm_clear_bit(uint32_t page) {
  pmm_bitmap[page / 32] &= ~(1 << (page % 32));
}

int pmm_test_bit(uint32_t page) {
  return pmm_bitmap[page / 32] & (1 << (page % 32));
}
