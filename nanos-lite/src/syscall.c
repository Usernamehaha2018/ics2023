#include <common.h>
#include <syscall.h>
size_t fs_write(int fd, void* buf, size_t len);

void sys_exit(Context *c) {
  halt(0);
}

void sys_yield(Context *c) {
  yield();
  c->GPRx = 0;
}

void sys_write(Context *c) {
  c->GPRx = fs_write(c->GPR2, (void*)c->GPR3, c->GPR4);
}

void sys_brk(Context *c) {
  c->GPRx = 0;
}






void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  switch (a[0]) {
    case 0: sys_exit(c); break;
    case 1: sys_yield(c);break; //SYS_yield
    case 4: sys_write(c);break;
    case 9: sys_brk(c);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
