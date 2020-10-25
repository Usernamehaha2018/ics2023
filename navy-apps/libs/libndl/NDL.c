#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

int _gettimeofday(struct timeval *tv, struct timezone *tz);
int _signal(void *buf, size_t len);
int _get_screen_size(void *buf, size_t len);
static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;



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
    if(*buf > '0' && *buf < '9'){
      ans_w = ans_w *10 + (*buf - '0');
    }
    buf ++;
  }
  buf ++;
  while(*buf != '\n'){
    if(*buf > '0' && *buf < '9'){
      ans_h = ans_h *10 + (*buf - '0');
    }
    buf ++;
  }
  *width = ans_w;
  *height = ans_h;
  return;
}

void NDL_OpenCanvas(int *w, int *h) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    _get_screen_size(buf, sizeof(buf));
    //int len = sprintf(buf, "%d %d", screen_w, screen_h);
    char *cur = &buf[0];
    int s_width=0,s_height = 0;
    get_width(cur,&s_width,&s_height);
    if(!*w||!*h){
      screen_h = s_height;
      screen_w = s_width;
    }
    printf("%d-----%d\n",s_height,s_width);
    while(1){

    }
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

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
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
