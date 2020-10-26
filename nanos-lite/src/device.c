#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};
void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd);
void __am_gpu_config(AM_GPU_CONFIG_T *cfg);
void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl);
// its a file
static char dispinfo[128] = {};
static AM_GPU_CONFIG_T cfg;
static AM_GPU_FBDRAW_T ctl;

size_t serial_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T kbd;
  __am_input_keybrd(&kbd);
  if (kbd.keycode == AM_KEY_NONE) {
    return 0;
  }
  else {
    if(strlen(keyname[kbd.keycode])+3+1 >= len)
    sprintf(buf, "%s %s\n", kbd.keydown?"kd":"ku", keyname[kbd.keycode]);
    else{
      char a[32];
      sprintf(a, "%s %s\n", kbd.keydown?"kd":"ku", keyname[kbd.keycode]);
      memcpy(buf,a,len);
      return len;
    }
  }
  size_t ret_len = strlen(buf);
  return ret_len;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  memcpy(buf, dispinfo, len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  ctl.x = (offset) / cfg.width;
  ctl.y = (offset) % cfg.width;
  ctl.w = len/4;
  ctl.h = 1;
  ctl.pixels = (void*)buf;
  printf("%d,%d,%d,%d\n",ctl.x,ctl.y,ctl.w,ctl.h);
  ctl.sync = 1;
  __am_gpu_fbdraw(&ctl);
  return len;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
  __am_gpu_config(&cfg);
  sprintf(dispinfo, "WIDTH: %d\nHEIGHT:%d\n", cfg.width, cfg.height);
}
