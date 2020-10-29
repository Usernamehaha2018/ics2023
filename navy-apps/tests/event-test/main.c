#include <stdio.h>
#include <NDL.h>

int main() {
  NDL_Init(0);
  while (1) {
    char buf[64]="emm";
    NDL_PollEvent(buf, sizeof(buf));
    if (1) {
      printf("receive event: %s\n", buf);
    }
  }
  return 0;
}
