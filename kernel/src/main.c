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

void kmain() {
  tty_initialize();
  
  idt_install();
  pic_remap();
  pit_init(1000);
  
#ifdef CONFIG_DRIVER_RTC
  rtc_init(1024);
#endif

  asm volatile("sti"); 

  welcome();
  sh_main(); 
  
  for(;;) {
    kpanic("Entered in main().loop");
  }
}
