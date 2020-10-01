#include <am.h>
#include <nemu.h>
static uint32_t freq_mhz = 100;
static uint64_t uptsc;


void __am_timer_init() {
  uptsc = rdtsc();
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) { 
 uptime->us = (rdtsc() - uptsc) / freq_mhz;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
