#include <am.h>
#include <nemu.h>
static uint64_t uptsc;


void __am_timer_init() {
  uint32_t lo = inl(0x48);
  uptsc = ((uint64_t)0 << 32) | lo;
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) { 
  uint32_t lo = inl(0x48);
  uint64_t old_time = uptime->us % 1000000;
  uint64_t new_time = ((uint64_t)0 << 32) | lo;
  if (old_time < new_time) uptime->us += (new_time - old_time);
  else uptime->us += (1000000 - old_time + new_time);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
