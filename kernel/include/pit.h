#ifndef PIT_H
#define PIT_H

#include <stdint.h>

#define PIT_FREQUENCY 1193182  

#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40

void pit_init(uint32_t frequency);
void pit_sleep(uint32_t milliseconds);
uint32_t pit_get_ticks(void);

#endif /* H_PIT */
