#include <am.h>
#include <nemu.h>
#include <string.h>
int get_vga_screen_width();
int get_vga_screen_height();
//#define SYNC_ADDR (SCREEN_ADDR + 4)
#define SYNC_ADDR (VGACTL_ADDR + 4)
void __am_gpu_init() {
  int i;
  int w = 400;  // TODO: get the correct width
  int h = 300;  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 0);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = true,
    .width = 400, .height = 300,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
    int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
    uint32_t *pixels = ctl->pixels;
    uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
    int min_w;
    if(w<400-x) min_w = w;
    else min_w = 400-x;
    int cp_bytes = 4 * min_w;
    for (int j = 0; j < h && y + j < 300; j ++) {
        
        memcpy(&fb[(y + j) * 400 + x], (void*)pixels, cp_bytes);
        pixels += w;
    }
  if (ctl->sync) {
   // outl(SYNC_ADDR, 0);
   outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}


