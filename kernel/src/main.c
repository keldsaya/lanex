#include "main.h"
#include "tty.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "sh.h"
#include "messages.h"

extern uint32_t kernel_start;
extern uint32_t kernel_end;

void kmain() {
  tty_initialize();
  
  idt_install();
  pic_remap();
  pit_init(1000);

  asm volatile("sti"); 

  welcome();
  sh_main(); 
  
  for(;;) {
    asm volatile("hlt");
  }
}
