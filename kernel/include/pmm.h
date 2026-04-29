#ifndef H_PMM
#define H_PMM
#include <stdint.h>
#include <stddef.h>

void pmm_init(uint32_t mem_kb);
void *pmm_alloc_page(void);
void pmm_free_page(void *addr);
uint32_t pmm_free_pages_count(void);

#endif /* H_PMM */
