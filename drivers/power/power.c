#include "power.h"
#include "io.h"

static inline void local_io_wait() { outb(0x80, 0); }

static inline void wait_kbd_buffer() {
  while (inb(0x64) & 0x02) {
    local_io_wait();
  }
}

void power_shutdown() {
  outw(0x604, 0x2000);
  local_io_wait();

  outw(0xB004, 0x2000);
  local_io_wait();
  outw(0x4004, 0x3400);
  local_io_wait();

  wait_kbd_buffer();
  outb(0x64, 0xFE);

  asm volatile("cli");
  while (1)
    asm volatile("hlt");
}

void power_reboot() {
  wait_kbd_buffer();
  outb(0x64, 0xFE);
  local_io_wait();

  outb(0xCF9, 0x06);
  local_io_wait();

  uint8_t fast_res = inb(0x92);
  outb(0x92, fast_res | 0x01);
  local_io_wait();

  struct {
    uint16_t limit;
    uint32_t base;
  } __attribute__((packed)) idtr = {0, 0};

  asm volatile("lidt %0" : : "m"(idtr));
  asm volatile("int $3");

  asm volatile("cli");
  while (1)
    asm volatile("hlt");
}
