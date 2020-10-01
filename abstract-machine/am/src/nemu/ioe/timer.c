#include <am.h>
#include <nemu.h>
static uint32_t sec, usec;


void __am_timer_init() {
  sec = inl(0x4c);
  usec =  inl(0x48);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) { 
  uint32_t lo = inl(0x48);
  uint32_t hi = inl(0x4c) - sec;
  uptime->us = hi * 1000000 + lo - usec;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
