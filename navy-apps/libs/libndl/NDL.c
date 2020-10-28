#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

int _gettimeofday(struct timeval *tv, struct timezone *tz);
int _signal(void *buf, size_t len);
int _get_screen_size(void *buf, size_t len);
int _draw_screen(void *buf, size_t offset, size_t len);
static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int canvas_w = 0, canvas_h = 0;



uint32_t NDL_GetTicks() {
  struct timeval tv;
  struct timezone tz;
  int flag = _gettimeofday(&tv, &tz);
  if(flag){
    return 0;
  }
  else{
    return tv.tv_sec*1000 + tv.tv_usec /1000;
  }
}

int NDL_PollEvent(char *buf, int len) {
  return _signal(buf,len);
}

void get_width(char* buf, int* width, int* height){
  int ans_w=0, ans_h = 0;
  int flag = 0;
  while(*buf != '\n'){
    if(*buf >= '0' && *buf <= '9'){
      ans_w = ans_w *10 + (*buf - '0');
    }
    buf ++;
  }
  buf ++;
  while(*buf != '\n'){
    if(*buf >= '0' && *buf <= '9'){
      ans_h = ans_h *10 + (*buf - '0');
    }
    buf ++;
  }
  *width = ans_w;
  *height = ans_h;
  return;
}

void NDL_OpenCanvas(int *w, int *h) {
    char buf[64];
    _get_screen_size(buf, sizeof(buf));
    char *cur = &buf[0];
    get_width(cur,&screen_w,&screen_h);
    if(!*w || !*h){
    *w = screen_w;
    *h = screen_h;
    }
    *w = *w>screen_w? screen_w:*w;
    *h = *h>screen_h? screen_h:*h;
    canvas_w = *w;
    canvas_h = *h;
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    // canvas_w = *w; canvas_h = *h;
    //int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    //write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  if(w==0||h==0){
    w = screen_w;
    h = screen_h;
  }
  for(int i = 0;i<h; i ++){
    _draw_screen((void *)pixels, screen_w*(x+i)+y, 4*w);
    pixels += w;
  }
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  return 0;
}

void NDL_Quit() {
}
