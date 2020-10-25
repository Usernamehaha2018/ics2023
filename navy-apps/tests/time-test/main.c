#include <stdio.h>
#include <NDL.h>

uint32_t NDL_GetTicks();
int main() {
  int i = 0;
  int j = 0;
  int count = 0;
  while (1) {
  	i = NDL_GetTicks()/1000;
  	if(j==0) j = i;
    if (i-j==1) {
      printf(" %dth time!\n", count ++);
      j = i;
    }
  }
  return 0;
}
