#include <stdio.h>
#include "config.h"
#include "main.h"
#include "tty.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "sh.h"
#include "messages.h"

#ifdef CONFIG_DRIVER_RTC
#include "rtc.h"
#endif

extern uint32_t kernel_start;
extern uint32_t kernel_end;

void kmain(uint32_t mem_low, uint32_t mem_high) {
  tty_initialize();
  
  idt_install();
  pic_remap();
  pit_init(1000);
  
#ifdef CONFIG_DRIVER_RTC
  rtc_init(1024);
#endif

  asm volatile("sti"); 
  uint32_t total_memory_kb = 1024;

  if (mem_high > 0) {
    total_memory_kb += mem_low + (mem_high * 64);
  } else {
    total_memory_kb += mem_low;
  }
    
  kprintf("Total memory: %uKB (%uMB)\n", total_memory_kb, 
      total_memory_kb / 1024);

  welcome();
  sh_main(); 
  
  for(;;) {
    kpanic("Entered in main().loop");
  }
}
