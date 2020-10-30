#include <NDL.h>
#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define keyname(k) #k,
char keystate[83];
static int down = -1;
static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

// i,key:0,73
// i,key:1,74
// i,key:2,75
// i,key:3,76
// i,key:4,1
// i,key:5,77
// i,key:6,69
// i,key:7,71
// i,key:8,54
// i,key:9,70
// i,key:10,67
// i,key:11,81
// i,key:12,82
// i,key:13,79
// i,key:14,80
// i,key:15,32
// i,key:16,43
// i,key:17,45
// i,key:18,31
// i,key:19,30
// i,key:20,29
// i,key:21,46
// i,key:22,44

#define KEY_QUEUE_LEN 1024
struct event_state{
  int key;
  int state;
};
static struct event_state key_queue[KEY_QUEUE_LEN];
static int key_f = 0, key_r = 0, tmp = 0;

int SDL_PumpEvent() {
  char s[128];
  int flag = 1;
  int ans = NDL_PollEvent(s,128);
  if(ans){
    // printf("%s,s.\n",s);
    if(s[1]=='d'){
      key_queue[key_r].state = 0;
    }
    else if (s[1]=='u')key_queue[key_r].state = 1;
    int flag  = 0;
    char *kbd = &s[3];    
    for(uint8_t i= 0;i<83;i++){
      if(strncmp(keyname[i],(const char*)kbd,strlen(keyname[i]))==0&&strlen((const char*)kbd)-1==strlen(keyname[i])){
       down = i;
       key_queue[key_r].key = i;
      //  printf("new_k_r:%d,r:%d\n",key_queue[key_r].key,key_r);
       flag = 1;
      }
    }  
    if(!flag){assert(0);}
    key_r = (key_r + 1) % KEY_QUEUE_LEN;    
    // printf("key_l&r:%d,%d\n",key_f,key_r);
    return 1;
  }
  return 0;
}


int SDL_PushEvent(SDL_Event *ev) {
  printf("unhandled pushevent\n");
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  // printf("poll here\n");  
  SDL_PumpEvent();
  if(key_f == key_r)return 0;
  if(ev != NULL){     
    ev->key.keysym.sym = key_queue[key_f].key;
    ev->type = key_queue[key_f].state;
    key_f = (key_f + 1) % KEY_QUEUE_LEN;
    // printf("eventpollhappen,key_f:%d\n",key_f);
  }
  if(ev==NULL){
  // printf("curkey:%d\n",key_queue[key_f].key);    
  if(!key_queue[key_f].key)assert(0);
  }
  return 1;   
}

int SDL_WaitEvent(SDL_Event *ev) {
  while(key_f == key_r){
    SDL_PumpEvent();
  }
    if(ev != NULL){     
    ev->key.keysym.sym = key_queue[key_f].key;
    ev->type = key_queue[key_f].state;
    key_f = (key_f + 1) % KEY_QUEUE_LEN;
    // printf("eventwaithappen,key_f:%d\n",key_f);
  }
  return 1;   
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  printf("unhandled peepevent\n");
  return 0;
}


uint8_t* SDL_GetKeyState(int *numkeys) {
  SDL_PumpEvent();  
  // printf("key_l&r:%d,%d\n",key_f,key_r);
  for(int i=0;i<83;i++) {
    keystate[i] = (i==key_queue[tmp].key?1:0);    
      // if(keystate[i])printf("i:%d\n",i);
  }
  tmp = (tmp + 1) % KEY_QUEUE_LEN;
  // key_f = (key_f + 1) % KEY_QUEUE_LEN;
    // for (int i = key_f; i != key_r; i = (i + 1) % KEY_QUEUE_LEN) {
    //   if (key_queue[i].state == 0) {
    //     keystate[key_queue[i].key] = 1;
    //   }
    //   else {
    //     keystate[key_queue[i].key] == 0;
    //   }
    // }
  
  return keystate;   
}
