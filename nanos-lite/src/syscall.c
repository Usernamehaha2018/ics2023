#include <common.h>
#include <syscall.h>

void sys_yield(Context *c) {
  yield();
  c->GPRx = 0;
}


void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  printf("debug:%u \n", a[0]);
  switch (a[0]) {
    case 1: sys_yield(c);break; //SYS_yield
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
