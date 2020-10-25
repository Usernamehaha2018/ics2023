#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
int _gettimeofday(struct timeval *tv, struct timezone *tz);
int main() {
  int i = 0;
  int j = 0;
  int count = 0;
  struct timeval tv;
  struct timezone tz;
  while (1) {
  	_gettimeofday(&tv, &tz);
  	i = tv.tv_sec;
  	if(j==0) j = i;
    if (i-j==1) {
      printf(" %dth time!\n", count ++);
      j = i;
    }
  }
  return 0;
}
