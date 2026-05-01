#include "pit.h"
#include "io.h"

static volatile uint32_t tick_count = 0;

void pit_handler(void) {
  tick_count++;
  outb(0x20, 0x20);
}

void pit_init(uint32_t frequency) {
  if (frequency == 0)
    frequency = 100;

  uint32_t divisor = PIT_FREQUENCY / frequency;

  asm volatile("cli");

  outb(PIT_COMMAND_PORT, 0x36);
  outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));
  outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF));

  outb(0x21, inb(0x21) & 0xFE);

  asm volatile("sti");
}

uint32_t pit_get_ticks(void) { return tick_count; }
