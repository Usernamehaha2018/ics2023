#include <am.h>
#include <nemu.h>
static uint32_t current_time=11 ;
void __am_timer_init() {
  current_time = (uint32_t)inl(0x48);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  //uint32_t addr = 0xa1000048;
  //uint64_t t = *(volatile uint64_t *)addr;
  if(inl(0x48)<current_time){
    uptime->us = 9999999;
  }
  uptime->us = (uint64_t)((uint32_t)inl(0x48)-current_time);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
