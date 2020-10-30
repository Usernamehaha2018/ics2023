#include <NDL.h>
#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define keyname(k) #k,
char keystate[256];
static int down = -1;
static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

#define KEY_QUEUE_LEN 1024
struct event_state{
  int key;
  int state;
};
static struct event_state key_queue[KEY_QUEUE_LEN];
static int key_f = 0, key_r = 0;

int SDL_PumpEvent() {
  char s[128];
  int flag = 1;
  int ans = NDL_PollEvent(s,128);
  if(ans){
    if(s[1]=='d'){
      printf("%s,s.\n",s);
      key_queue[key_r].state = 0;
    }
    else if (s[1]=='u')key_queue[key_r].state = 1;
    int flag  = 0;
    char *kbd = &s[3];    
    for(uint8_t i= 0;i<83;i++){
      if(strncmp(keyname[i],(const char*)kbd,strlen(keyname[i]))==0&&strlen((const char*)kbd)-1==strlen(keyname[i])){
       down = i;
       key_queue[key_r].key = i;
       flag = 1;
      }
    }  
    if(!flag){assert(0);}
    key_r = (key_r + 1) % KEY_QUEUE_LEN;
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
  }
  return 1;   
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  printf("unhandled peepevent\n");
  return 0;
}


uint8_t* SDL_GetKeyState(int *numkeys) {
  SDL_PumpEvent();  
  if(key_f!=key_r){
    for(int i=0;i<256;i++)
      keystate[i] = (i==key_queue[key_f].key?1:0);    
  }
  else{
    for(int i=0;i<256;i++)
      keystate[i] = 0;   
  }
  return keystate;   
}
