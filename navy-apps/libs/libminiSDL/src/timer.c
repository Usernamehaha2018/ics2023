#include <NDL.h>
#include <sdl-timer.h>
#include <stdio.h>

SDL_TimerID SDL_AddTimer(uint32_t interval, SDL_NewTimerCallback callback, void *param) {
  return NULL;
}

int SDL_RemoveTimer(SDL_TimerID id) {
  printf("unhandled removetimer\n");
  return 1;
}

uint32_t SDL_GetTicks() {
  printf("unhandled getticks\n");
  return 0;
}

void SDL_Delay(uint32_t ms) {
  printf("unhandled delay\n");
}
