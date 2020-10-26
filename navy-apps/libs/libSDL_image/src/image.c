#define SDL_malloc  malloc
#define SDL_free    free
#define SDL_realloc realloc

#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

SDL_Surface* IMG_Load_RW(SDL_RWops *src, int freesrc) {
  assert(src->type == RW_TYPE_MEM);
  return NULL;
}

SDL_Surface* IMG_Load(const char *filename) {
  int fd = open(filename,1);
  struct stat pstat;
  fstat(fd,&pstat);
  printf("size: %d\n",pstat.st_size);
  void * buf = malloc(pstat.st_size);
  read(fd,(void*)buf,pstat.st_size);
  printf("readover,size: %d\n",pstat.st_size);
  SDL_Surface* su = STBIMG_LoadFromMemory((const unsigned char*)buf, pstat.st_size);
  close(fd);
  free(buf);
  return su;
}

int IMG_isPNG(SDL_RWops *src) {
  return 0;
}

SDL_Surface* IMG_LoadJPG_RW(SDL_RWops *src) {
  return IMG_Load_RW(src, 0);
}

char *IMG_GetError() {
  return "Navy does not support IMG_GetError()";
}
