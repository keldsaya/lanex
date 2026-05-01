#ifndef RTC_H
#define RTC_H

#include <stdint.h>

#define RTC_ADDRESS 0x70
#define RTC_DATA 0x71
#define RTC_REG_A 0x0A
#define RTC_REG_B 0x0B
#define RTC_REG_C 0x0C

typedef struct {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t day;
  uint8_t month;
  uint16_t year;
} rtc_datetime_t;

void rtc_init(uint32_t frequency);
uint32_t rtc_get_ticks(void);
void rtc_handler(void);
rtc_datetime_t rtc_get_datetime(void);
void rtc_print_datetime(void);

#endif /* RTC_H */
