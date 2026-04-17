#ifndef H_PMM
#define H_PMM
#include <stdint.h>

#define PMM_PAGE_SIZE 4096
#define PMM_BITMAP_FREE 0
#define PMM_BITMAP_USED 1

extern uint32_t* pmm_bitmap;
extern uint32_t pmm_total_pages;
extern uint32_t pmm_bitmap_size;

void pmm_set_bit(uint32_t page);
void pmm_clear_bit(uint32_t page);
int pmm_test_bit(uint32_t page);

#endif /* H_PMM */
