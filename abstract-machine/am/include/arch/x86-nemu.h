#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  void *cr3;  
  uintptr_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  
  int irq;
  uintptr_t cur, eip, cs, eflags;
};

#define GPR1 ebx
#define GPR2 eax
#define GPR3 ecx
#define GPR4 edx
#define GPRx ebx

#endif
