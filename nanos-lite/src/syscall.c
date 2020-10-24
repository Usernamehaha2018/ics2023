#include <common.h>
#include <syscall.h>

void sys_exit(Context *c) {
  halt(0);
}

void sys_yield(Context *c) {
  yield();
  c->GPRx = 0;
}






void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  printf("c->GPR1:%u\n",c->GPR1);
  printf("c->GPR2:%u\n",c->GPR2);
  printf("c->GPR3:%u\n",c->GPR3);
  printf("c->GPR4:%u\n",c->GPR4);
  printf("c->GPR1:%u\n",c->eax);
  printf("c->GPR2:%u\n",c->ebx);
  printf("c->GPR3:%u\n",c->ecx);
  printf("c->GPR4:%u\n",c->edx);
  printf("c->GPR3:%u\n",c->edi);
  printf("c->GPR4:%u\n",c->esi);
  switch (a[0]) {
    case 0: sys_exit(c); break;
    case 1: sys_yield(c);break; //SYS_yield
    //case 4: sys_write(c);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
