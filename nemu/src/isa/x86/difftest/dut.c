#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  printf("%d\n",ref_r->eflags.eflag); 
  printf("%d\n",cpu.eflags.eflag); 
  if(cpu.eax == ref_r->eax&&cpu.ebx == ref_r->ebx
  &&cpu.ecx == ref_r->ecx&&cpu.edx == ref_r->edx
  &&cpu.esp == ref_r->esp&&cpu.ebp == ref_r->ebp
  &&cpu.esi == ref_r->esi&&cpu.edi == ref_r->edi/*&&cpu.pc == ref_r->pc*/&&cpu.eflags.eflag==ref_r->eflags.eflag){  
    //printf("pc:%x,%x\n",cpu.pc,ref_r->pc);
    return true;
  }
  printf("reg:\n");
  for(int i=0;i<8;i++){
    printf("%d:%x\n",i,ref_r->gpr[i]._32); 
  }
  printf("pc:%x\n",ref_r->pc);
  return false;
}

void isa_difftest_attach() {
}
