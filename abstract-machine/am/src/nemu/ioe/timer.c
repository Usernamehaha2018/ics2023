#include <am.h>
#include <nemu.h>
static uint64_t uptsc;


void __am_timer_init() {
  uint32_t lo = inl(0x48);
  uint32_t hi = inl(0x4c);
  uptsc = ((uint64_t)hi << 32) | lo;
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) { 
  uint32_t lo = inl(0x48);
  uint32_t hi = inl(0x4c);
  uptime->us = (((uint64_t)hi << 32) | lo) - uptsc;
  // if(new_time < uptime->us%1000000){
  //   uptime->us = new_time + (1000000-uptime->us%1000000)+ uptime->us;
  // }
  // else uptime->us = new_time ;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
