#include <am.h>
#include <nemu.h>
static uint64_t current_time=11 ;
static int flag = 0;
void __am_timer_init() {
  uint32_t addr = 0xa1000048;
  uint64_t t = *(volatile uint64_t *)addr;
  current_time = t;
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  if(!flag){
    
  }
  uint32_t addr = 0xa1000048;
  uint64_t t = *(volatile uint64_t *)addr;
  uptime->us = (t-current_time);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
