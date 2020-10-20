#include <monitor/difftest.h>
#include <isa.h>
// mychange
#include<cpu/exec.h>

static inline def_EHelper(lidt) {
  if (s->isa.is_operand_size_16){   
    cpu.idtr.limit = vaddr_read(*s->isa.mbase, 2);
    cpu.idtr.base = vaddr_read(*s->isa.mbase+2, 3);
  }
  else{
    cpu.idtr.limit = vaddr_read(*s->isa.mbase, 2);
    cpu.idtr.base = vaddr_read(*s->isa.mbase+2, 4);
  }
  print_asm_template1(lidt);
}

static inline def_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src1->reg, 4), id_dest->reg);
}

static inline def_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src1->reg, reg_name(id_dest->reg, 4));

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}

static inline def_EHelper(sys_int) {
  printf("%x,pc\n",s->seq_pc);
  raise_intr(s,*ddest,s->seq_pc);
  print_asm("int %s", id_dest->str);

#ifndef __DIFF_REF_NEMU__
  difftest_skip_dut(1, 2);
#endif
}

static inline def_EHelper(iret) {
  rtl_pop(s, &s->jmp_pc);
  rtl_pop(s, &cpu.cs);
  rtl_pop(s, &cpu.eflags.eflag);
  rtl_j(s, s->jmp_pc);
  print_asm("iret");

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

static inline def_EHelper(in) {
    switch (id_dest->width)
  {
  case 1:
    *id_dest->preg = pio_read_b(*id_src1->preg);
    break;
  case 2: 
    *id_dest->preg = pio_read_w(*id_src1->preg);
    break;
  case 4:
    *id_dest->preg = pio_read_l(*id_src1->preg);
    break;
  default:
    assert(0);
  }

  print_asm_template2(in);
}

static inline def_EHelper(out) {

  switch (id_dest->width)
  {
  case 1:
    pio_write_b(*id_dest->preg,*id_src1->preg);
    break;
  case 2: 
    pio_write_w(*id_dest->preg,*id_src1->preg);
    break;
  case 4:
    pio_write_l(*id_dest->preg,*id_src1->preg);
    break;
  default:
    assert(0);
  }
  print_asm_template2(out);
}
