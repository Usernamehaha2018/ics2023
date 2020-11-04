#include <am.h>
#include <NDL.h>

bool ioe_init() {
  return true;
}

void handle_AM_TIMER_CONFIG(void*buf){
    AM_TIMER_CONFIG_T *cfg = (AM_TIMER_CONFIG_T *)buf;
    cfg->present = true; 
    cfg->has_rtc = true;
}
void handle_AM_TIMER_RTC(void*buf){
    AM_TIMER_RTC_T *rtc = (AM_TIMER_RTC_T *)buf;
    rtc->second = 0;
    rtc->minute = 0;
    rtc->hour   = 0;
    rtc->day    = 0;
    rtc->month  = 0;
    rtc->year   = 1900;
}
void handle_AM_TIMER_UPTIME(void*buf){
    int time_u_sec = NDL_GetTicks();
    AM_TIMER_UPTIME_T *uptime = (AM_TIMER_UPTIME_T *)buf;
    uptime->us = time_u_sec*1000;
}
void handle_AM_INPUT_CONFIG(void*buf){
      AM_INPUT_CONFIG_T *cfg = (AM_INPUT_CONFIG_T *)buf;
      cfg->present = true;
}
void handle_AM_INPUT_KEYBRD(void* buf){
  NDL_get_key_direct(buf);
}
void handle_AM_GPU_CONFIG(void*buf){
    AM_GPU_CONFIG_T* cfg = (AM_GPU_CONFIG_T*)buf;
    *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = true,
    .width = 400, .height = 300,
    .vmemsz = 0
  };
}
void handle_AM_GPU_FBDRAW(void*buf){
  NDL_draw_direct(buf);
}
void handle_AM_GPU_STATUS(void*buf){
  AM_GPU_STATUS_T *status = (AM_GPU_STATUS_T *)buf;
  status->ready = true;
}

void handle_AM_UART_CONFIG(void*buf){
    AM_UART_CONFIG_T *cfg = (AM_UART_CONFIG_T*)buf;
    cfg->present = false;
}
void handle_AM_DISK_CONFIG(void*buf){
    AM_DISK_CONFIG_T *cfg = (AM_DISK_CONFIG_T*)buf;
    cfg->present = false;
}
void handle_AM_NET_CONFIG(void*buf){
    AM_NET_CONFIG_T *cfg = (AM_NET_CONFIG_T *)buf;
    cfg->present = false;
}













void ioe_read (int reg, void *buf) { 
  switch (reg)
  {
  case AM_TIMER_CONFIG:
    handle_AM_TIMER_CONFIG(buf);
    break;
  case AM_TIMER_RTC:
    handle_AM_TIMER_RTC(buf);
    break;
  case AM_TIMER_UPTIME:
    handle_AM_TIMER_UPTIME(buf);
    break;
  case AM_INPUT_CONFIG:
    handle_AM_INPUT_CONFIG(buf);
    break;
  case AM_INPUT_KEYBRD:
    handle_AM_INPUT_KEYBRD(buf);
    break;
  case AM_GPU_CONFIG:
    handle_AM_GPU_CONFIG(buf);
    break;
  case AM_GPU_FBDRAW:
    handle_AM_GPU_FBDRAW(buf);
    break;
  case AM_GPU_STATUS:
    handle_AM_GPU_STATUS(buf);
    break;
  case AM_UART_CONFIG:
    handle_AM_UART_CONFIG(buf);
    break;
  case AM_DISK_CONFIG:
    handle_AM_DISK_CONFIG(buf);
    break;
  case AM_NET_CONFIG:
    handle_AM_NET_CONFIG(buf);
    break; 
  default:
    printf("shouldnt have this reg\n");
    while (1);
  }
}
void ioe_write(int reg, void *buf) {
  ioe_read(reg,buf);
 }
