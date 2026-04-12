#include "idt.h"
#include "pic.h"
#include "tty.h"
#include "keyboard.h"
#include "sh.h"

void kmain();
void _start() { kmain(); }

void kmain() {
  tty_initialize();
  idt_install();
  pic_remap();
  __asm__ __volatile__("sti");

  tty_writestring("Hello!\n");
  sh_main();
  while(1) {
    tty_writestring("KERNEL PANIC!!");
    __asm__ __volatile__ ("cli");
    __asm__ __volatile__ ("hlt");
  }
}
