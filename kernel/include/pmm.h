#ifndef H_PMM
#define H_PMM
#include <stdint.h>
#include <stddef.h>

void pmm_init(uint32_t mem_kb);
void *pmm_alloc_page(void);
void pmm_free_page(void *addr);

#endif /* H_PMM */
