#include <am.h>
#include <NDL.h>

bool ioe_init() {
  return true;
}

void ioe_read (int reg, void *buf) { 
  switch (reg)
  {
  case AM_TIMER_CONFIG:
    AM_TIMER_CONFIG_T *cfg = (AM_TIMER_CONFIG_T *)buf;
    cfg->present = true; cfg->has_rtc = true;
    break;
  case AM_TIMER_RTC:
    AM_TIMER_RTC_T *rtc = (AM_TIMER_RTC_T *)buf;
    rtc->second = 0;
    rtc->minute = 0;
    rtc->hour   = 0;
    rtc->day    = 0;
    rtc->month  = 0;
    rtc->year   = 1900;
    break;
  case AM_TIMER_UPTIME:
    int time_u_sec = NDL_GetTicks();
    AM_TIMER_UPTIME_T *uptime = (AM_TIMER_UPTIME_T *)buf;
    uptime->us = time_u_sec*1000;
    break;
  case AM_INPUT_CONFIG:
    AM_INPUT_CONFIG_T *cfg = (AM_INPUT_CONFIG_T *)buf;
    cfg->present = true;
    break;
  case AM_INPUT_KEYBRD:
    break;
  case AM_GPU_CONFIG:
    break;
  case AM_GPU_FBDRAW:
    break;
  case AM_GPU_STATUS:
    break;
  case AM_UART_CONFIG:
    AM_UART_CONFIG_T *cfg = (AM_UART_CONFIG_T*)buf;
    cfg->present = false;
  case AM_DISK_CONFIG:
    AM_DISK_CONFIG_T *cfg = (AM_DISK_CONFIG_T*)buf;
    cfg->present = false;
  case AM_NET_CONFIG:
    AM_NET_CONFIG_T *cfg = (AM_NET_CONFIG_T *)buf;
    cfg->present = false;
    break; 
  default:
  
  printf("shouldnt have this reg\n");
  while (1);
  }
}
void ioe_write(int reg, void *buf) { }
