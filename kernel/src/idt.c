#include "idt.h"
#include "messages.h" 
#include "config.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void isr_wrapper();
extern void pit_wrapper();
extern void keyboard_wrapper();
extern void rtc_wrapper();

#ifndef CONFIG_DRIVER_KEYBOARD
void keyboard_handler() {}
#endif

#ifndef CONFIG_DRIVER_RTC
void rtc_handler() {}
#endif

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
  idt[num].base_low = (base & 0xFFFF);
  idt[num].base_high = (base >> 16) & 0xFFFF;
  idt[num].selector = sel;
  idt[num].zero = 0;
  idt[num].flags = flags;
}

void idt_install() {
  idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
  idtp.base = (uint32_t)&idt;

  for(int i = 0; i < 32; i++) {
    idt_set_gate(i, (uint32_t)isr_wrapper, 0x08, 0x8E);
  }
  idt_set_gate(32, (uint32_t)pit_wrapper, 0x08, 0x8E);
  idt_set_gate(33, (uint32_t)keyboard_wrapper, 0x08, 0x8E);
  idt_set_gate(40, (uint32_t)rtc_wrapper, 0x08, 0x8E);
  asm volatile("lidt (%0)" : : "r" (&idtp));
}

void isr_handler() {
  kpanic("CPU Exception");
}
