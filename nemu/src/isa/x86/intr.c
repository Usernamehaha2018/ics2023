#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_push(s,&cpu.eflags.eflag);
  // printf("eflagsaddr:%x,%d\n",cpu.esp,ret_addr);
  rtl_push(s,&cpu.cs);
  rtl_push(s,&ret_addr);

  uint32_t addr = cpu.idtr.base + 8*NO;
  uint32_t offset_lo = vaddr_read(addr,4);
  offset_lo &= 0x0000ffff;
  uint32_t offset_hi = vaddr_read(addr+4,4);
  offset_hi &= 0xffff0000;
  offset_hi |= offset_lo;

  rtl_j(s,offset_hi);
}

void query_intr(DecodeExecState *s) {
  TODO();
}
