#include "cc.h"


static inline def_EHelper(test) {
  rtl_and(s,s0,ddest,dsrc1);
  rtl_update_ZFSF(s,ddest,id_dest->width);
  *s1 = 0;
  rtl_set_CF(s,s1);
  rtl_set_OF(s,s1);
  print_asm_template2(test);
}

static inline def_EHelper(and) {
  rtl_and(s,s0,ddest,dsrc1);
  operand_write(s,id_dest, s0); 
  rtl_update_ZFSF(s,ddest,id_dest->width);
  *s1 = 0;
  rtl_set_CF(s,s1);
  rtl_set_OF(s,s1);
  print_asm_template2(and);
}

static inline def_EHelper(xor) {
  rtl_xor(s, s0, id_dest->preg, id_src1->preg);
  operand_write(s, id_dest, s0);
}

static inline def_EHelper(or) {
  rtl_or(s,s0,ddest,dsrc1);
  operand_write(s,id_dest, s0); 
  rtl_update_ZFSF(s,ddest,id_dest->width);
  *s1 = 0;
  rtl_set_CF(s,s1);
  rtl_set_OF(s,s1);
  print_asm_template2(or);
}

static inline def_EHelper(sar) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

static inline def_EHelper(shl) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

static inline def_EHelper(shr) {
  // unnecessary to update CF and OF in NEMU
  rtl_shr(s,s0,ddest,dsrc1);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s,ddest,id_dest->width);
  
  print_asm_template2(shr);
}


static inline def_EHelper(setcc) {
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, ddest, cc);
  operand_write(s, id_dest, ddest);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

static inline def_EHelper(not) {
  rtl_not(s,s0,ddest);
  operand_write(s,id_dest,s0);
  print_asm_template1(not);
}

