#include <NDL.h>
#include <SDL.h>
#include <stdio.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  printf("unhandled pushevent\n");
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  printf("unhandled pollevent\n");
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  char s[128];
  int flag = 1;
  int ans = NDL_PollEvent(s,128);
  if(ans){
    if(s[1]=='d'){
      printf("%s,s.\n",s);
      event->type = 0;
    }
    else if (s[1]=='u')event->type = 1;
    char *kbd = &s[3];    
    for(uint8_t i= 0;i<83;i++){
      if(strncmp(keyname[i],(const char*)kbd,strlen(keyname[i]))==0&&strlen((const char*)kbd)-1==strlen(keyname[i])){
       event->key.keysym.sym = i;
           printf("%s\n",s);
          printf("key:%d,%d\n",event->type,event->key.keysym.sym);
      }
    }  
  }
  else{
    event->type = 2; //user event
    event->key.keysym.sym = 0;
  }


  return ans;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  printf("unhandled peepevent\n");
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  printf("unhandled getketstate\n");
  return NULL;
}
