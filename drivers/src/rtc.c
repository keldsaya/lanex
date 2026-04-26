#include <stdio.h>
#include "tty.h"
#include "rtc.h"
#include "io.h"
#include "pit.h" 
#include "messages.h"

static volatile uint32_t rtc_tick_count = 0;
static volatile uint32_t rtc_frequency = 0;

static uint8_t rtc_read(uint8_t reg) {
  outb(RTC_ADDRESS, reg | 0x80);
  return inb(RTC_DATA);
}

static void rtc_write(uint8_t reg, uint8_t value) {
  outb(RTC_ADDRESS, reg | 0x80);
  outb(RTC_DATA, value);
}

/* frequency = 32768 >> (rate-1) => rate = log2(32768/frequency) + 1 */
static uint8_t frequency_to_rate(uint32_t freq) {
  if (freq == 0) return 0;  
  if (freq < 2) freq = 2;
  if (freq > 8192) freq = 8192;
    
  /* frequency = 32768 / (2^(rate-1)) => 2^(rate-1) = 32768/freq */
  /* rate = log2(32768/freq) + 1 */
  uint32_t divisor = 32768 / freq;
  uint8_t rate = 1;
  while (divisor > 1) {
    divisor >>= 1;
    rate++;
  }
  if (rate < 3) rate = 3;   
  if (rate > 15) rate = 15;    
  return rate;
}

static uint8_t bcd_to_bin(uint8_t bcd) {
  return (bcd & 0x0F) + ((bcd >> 4) * 10);
}

void rtc_init(uint32_t frequency) {
  asm volatile("cli");
    
  rtc_frequency = frequency;
  uint8_t rate = frequency_to_rate(frequency);
    
  rtc_read(RTC_REG_C);
    
  uint8_t regB = rtc_read(RTC_REG_B);
  regB |= 0x40;  
  rtc_write(RTC_REG_B, regB);
    
  uint8_t regA = rtc_read(RTC_REG_A);
  regA = (regA & 0xF0) | (rate & 0x0F);
  rtc_write(RTC_REG_A, regA);
    
  outb(0xA1, inb(0xA1) & 0xFE); 
  
  asm volatile("sti");
}

uint32_t rtc_get_ticks(void) {
  return rtc_tick_count;
}

void rtc_handler(void) {
#ifdef CONFIG_DRIVER_RTC
  rtc_tick_count++;
    
  rtc_read(RTC_REG_C);
#endif
    
  outb(0x20, 0x20);  /* Master PIC */
  outb(0xA0, 0x20);  /* Slave PIC */
}

rtc_datetime_t rtc_get_datetime(void) {
  rtc_datetime_t dt;

  asm volatile("cli");

  while (rtc_read(RTC_REG_A) & 0x80);

  dt.second = bcd_to_bin(rtc_read(0x00));
  dt.minute = bcd_to_bin(rtc_read(0x02));
  dt.hour = bcd_to_bin(rtc_read(0x04));
  dt.day = bcd_to_bin(rtc_read(0x07));
  dt.month = bcd_to_bin(rtc_read(0x08));
  dt.year = bcd_to_bin(rtc_read(0x09)) + 2000;

  asm volatile("sti");
  return dt;
}

void rtc_print_datetime(void) {
  rtc_datetime_t dt = rtc_get_datetime();

  if (dt.hour < 10) tty_putchar('0');
  kprintf("%d:", dt.hour);

  if (dt.minute < 10) tty_putchar('0');
  kprintf("%d:", dt.minute);

  if (dt.second < 10) tty_putchar('0');
  kprintf("%d ", dt.second);

  if (dt.day < 10) tty_putchar('0');
  kprintf("%d.", dt.day);

  if (dt.month < 10) tty_putchar('0');
  kprintf("%d.", dt.month);

  kprintf("%d\n", dt.year);
}
