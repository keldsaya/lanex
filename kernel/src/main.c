#include <stdio.h>
#include "config.h"
#include "main.h"
#include "tty.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "pmm.h"
#include "sh.h"
#include "messages.h"

#ifdef CONFIG_DRIVER_RTC
#include "rtc.h"
#endif

uint32_t mem_init(uint32_t low, uint32_t high) {
  uint32_t mem = 1024;
  if (high > 0) {
    mem += low + (high * 64);
  } else {
    mem += low;
  }
  return mem;
}

void kmain(uint32_t mem_low, uint32_t mem_high) {
  tty_initialize();

  uint32_t total_memory_kb = mem_init(mem_low, mem_high);

  idt_install();
  pic_remap();
  pit_init(1000);
  pmm_init(total_memory_kb);
  
#ifdef CONFIG_DRIVER_RTC
  rtc_init(1024);
#endif

  asm volatile("sti"); 

  void *p1 = pmm_alloc_page();
  void *p2 = pmm_alloc_page();
  kprintf("Alloc: %p, %p\n", p1, p2);
  pmm_free_page(p1);
  void *p3 = pmm_alloc_page(); 
  kprintf("After free: %p\n", p3);

  welcome();
  sh_main(); 
  
  for(;;) {
    kpanic("Entered in main().loop");
  }
}
