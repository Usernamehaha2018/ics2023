#include <am.h>
#include <nemu.h>
uint32_t init_time = 0;
void __am_timer_init() {
  init_time = *(volatile uint32_t *)0x48;
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) { 
  uptime->us = *(volatile uint32_t *)0x48-init_time;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
