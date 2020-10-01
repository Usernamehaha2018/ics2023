#include <am.h>
#include <nemu.h>
//static uint32_t freq_mhz = 2000;
static uint64_t uptsc;


// static inline int read_rtc(int reg) {
//   outb(0x70, reg);
//   int ret = inb(0x71);
//   return (ret & 0xf) + (ret >> 4) * 10;
// }

// static void read_rtc_async(AM_TIMER_RTC_T *rtc) {
//   *rtc = (AM_TIMER_RTC_T) {
//     .second = read_rtc(0),
//     .minute = read_rtc(2),
//     .hour   = read_rtc(4),
//     .day    = read_rtc(7),
//     .month  = read_rtc(8),
//     .year   = read_rtc(9) + 2000,
//   };
// }


// static void wait_sec(AM_TIMER_RTC_T *t1) {
//   AM_TIMER_RTC_T t0;
//   while (1) {
//     read_rtc_async(&t0);
//     for (int volatile i = 0; i < 100000; i++) ;
//     read_rtc_async(t1);
//     if (t0.second != t1->second) {
//       return;
//     }
//   }
// }

// static uint32_t estimate_freq() {
//   AM_TIMER_RTC_T rtc1, rtc2;
//   uint64_t tsc1, tsc2, t1, t2;
//   wait_sec(&rtc1); tsc1 = rdtsc(); t1 = rtc1.minute * 60 + rtc1.second;
//   wait_sec(&rtc2); tsc2 = rdtsc(); t2 = rtc2.minute * 60 + rtc2.second;
//   if (t1 >= t2) return estimate_freq(); // passed an hour; try again
//   return ((tsc2 - tsc1) >> 20) / (t2 - t1);
// }

void __am_timer_init() {
  //freq_mhz = estimate_freq();
  uptsc = rdtsc();
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) { 
 uptime->us = (rdtsc() - uptsc);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
