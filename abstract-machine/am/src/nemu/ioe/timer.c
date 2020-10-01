#include <am.h>
#include <nemu.h>
static uint64_t uptsc;


void __am_timer_init() {
  uint32_t lo = inl(0x48);
  uptsc = ((uint64_t)0 << 32) | lo;
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) { 
  //uint32_t lo = inl(0x48);
  //uint64_t new_time = ((uint64_t)0 << 32) | lo;
  uptime->us = 4294000000;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
