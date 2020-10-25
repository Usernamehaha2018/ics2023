#include <common.h>
#include <syscall.h>

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);
int mm_brk(uintptr_t brk);
void __am_timer_get_time(AM_TIMER_UPTIME_T *uptime);

void sys_exit(Context *c) {
  halt(0);
}

void sys_yield(Context *c) {
  yield();
  c->GPRx = 0;
}

void sys_write(Context *c) {
  c->GPRx = fs_write(c->GPR2, (const void*)c->GPR3, c->GPR4);
}

void sys_brk(Context *c) {
  c->GPRx = mm_brk(c->GPR2+c->GPR3);
}

void sys_read(Context *c) {
  c->GPRx = fs_read(c->GPR2, (void*)c->GPR3, c->GPR4);
}

void sys_open(Context *c) {
  c->GPRx = fs_open((const void*)c->GPR2, c->GPR3, c->GPR4);
}

void sys_close(Context *c) {
  c->GPRx = fs_close(c->GPR2);
}

void sys_lseek(Context *c) {
  c->GPRx = fs_lseek(c->GPR2, c->GPR3, c->GPR4);
}

void sys_get_time(Context *c) {
  AM_TIMER_UPTIME_T upt;
  AM_TIMER_UPTIME_T* uptime = &upt;
  __am_timer_get_time(uptime);
  unsigned int sec = uptime->us / 1000000;
  unsigned int usec = uptime->us % 1000000;
  memcpy((void*)c->GPR2, (const void *)(&sec),4);
  memcpy((void*)c->GPR3, (const void *)(&usec),4);
  c->GPRx = 0;
}

void sys_event(Context *c){
  c->GPRx = fs_read(c->GPR2, (void*)c->GPR3, c->GPR4);
}






void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  switch (a[0]) {
    case 0: sys_exit(c); break;    
    case 1: sys_yield(c);break; //SYS_yield
    case 2: sys_open(c); break;
    case 3: sys_read(c);break;
    case 4: sys_write(c);break;
    case 7: sys_close(c);break;
    case 8: sys_lseek(c);break;
    case 9: sys_brk(c);break;
    case 12: sys_event(c);break;
    case 19: sys_get_time(c);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
