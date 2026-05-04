#include <lanex/main.h>
#include <lanex/config.h>
#include <lanex/idt.h>
#include <lanex/messages.h>
#include <lanex/pic.h>
#include <lanex/pit.h>
#include <lanex/pmm.h>
#include <lanex/sh.h>
#include <lanex/tty.h>
#include <stdio.h>

#ifdef CONFIG_DRIVER_RTC
#include <rtc/rtc.h>
#endif

static uint32_t mem_l;
static uint32_t mem_h;

uint32_t k_get_mem() {
  uint32_t mem = 1024;
  if (mem_h > 0) {
    mem += mem_l + (mem_h * 64);
  } else {
    mem += mem_l;
  }
  return mem;
}

void kmain(uint32_t mem_low, uint32_t mem_high) {
  mem_l = mem_low;
  mem_h = mem_high;

  tty_initialize();

  uint32_t total_memory_kb = k_get_mem();

  idt_install();
  pic_remap();
  pit_init(1000);
  pmm_init(total_memory_kb);

#ifdef CONFIG_DRIVER_RTC
  rtc_init(1024);
#endif

  asm volatile("sti");

  welcome();
  sh_main();

  for (;;) {
    kpanic("Entered in main().loop");
  }
}
