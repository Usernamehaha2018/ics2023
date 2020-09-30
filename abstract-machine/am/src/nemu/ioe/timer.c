#include <am.h>
#include <nemu.h>
static int time_passed = 0;

void __am_timer_init() {
  time_passed += 100;
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = time_passed;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
