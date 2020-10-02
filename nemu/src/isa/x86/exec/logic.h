#include "cc.h"


static inline def_EHelper(test) {
  rtl_and(s,s0,ddest,dsrc1);
  rtl_update_ZFSF(s,s0,id_dest->width);
  
  *s1 = 0;
  rtl_set_CF(s,s1);
  rtl_set_OF(s,s1);
  print_asm_template2(test);
}

static inline def_EHelper(and) {
  rtl_and(s,s0,ddest,dsrc1);
  operand_write(s,id_dest, s0); 
  rtl_update_ZFSF(s,s0,id_dest->width);
  *s1 = 0;
  rtl_set_CF(s,s1);
  rtl_set_OF(s,s1);
  print_asm_template2(and);
}

static inline def_EHelper(xor) {
  rtl_xor(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s,s0,id_dest->width);
  *s1 = 0;
  rtl_set_CF(s,s1);
  rtl_set_OF(s,s1);
  print_asm_template2(xor);
}

static inline def_EHelper(or) {
  rtl_or(s,s0,ddest,dsrc1);
  operand_write(s,id_dest, s0); 
  rtl_update_ZFSF(s,s0,id_dest->width);
  *s1 = 0;
  rtl_set_CF(s,s1);
  rtl_set_OF(s,s1);
  print_asm_template2(or);
}

uint32_t get_sar(const uint32_t times, const uint32_t width){
 // width is 15 or 7
 uint32_t cur = 1;
 uint32_t w = width,w_=width;
  while(w--){
    cur<<=1;
    cur+=1;
  }
  cur>>=(w_+1-times);
  cur<<=(w_+1-times);
  return cur;
}

static inline def_EHelper(sar) {
  // unnecessary to update CF and OF in NEMU
  *s1 = 0;
  if(id_dest->width==2&&*ddest&(uint32_t)32768){
    *s1 = get_sar(*dsrc1,id_dest->width*8-1);
  }
  if(id_dest->width==1&&*ddest&(uint32_t)128){
    *s1 = get_sar(*dsrc1,id_dest->width*8-1);
  }
   //printf("widthsar:%d,times:%u,des:%u\n",id_dest->width,*dsrc1,*ddest);
   rtl_sar(s, s0, id_dest->preg, id_src1->preg);
   //printf("widthsar:%d,val-s0:%u--%u\n",id_dest->width,*s0,*s1);
  if(*s1)*s0 |= *s1;
  //printf("widthsar:%d,val-s0:%u\n",id_dest->width,*s0);
  operand_write(s, id_dest, s0);
  //printf("widthsar:%d,val:%u\n",id_dest->width,*ddest);

  // unnecessary to update CF and OF in NEMU
  // update ZF and SF
  rtl_update_ZFSF(s, s0, id_dest->width);
  print_asm_template2(sar);
}

static inline def_EHelper(shl) {
  // unnecessary to update CF and OF in NEMU
  rtl_shl(s, s0, id_dest->preg, id_src1->preg);
  operand_write(s, id_dest, s0);

  // unnecessary to update CF and OF in NEMU
  // update ZF and SF
  rtl_update_ZFSF(s, s0, id_dest->width);
  print_asm_template2(shl);
}

static inline def_EHelper(shr) {
  // unnecessary to update CF and OF in NEMU
  //printf("id_dest.reg:%d---%d\n",id_dest->width,*ddest);
  rtl_shr(s,s0,ddest,dsrc1);
  //printf("id_dest.reg:%d---%d\n",id_dest->width,*ddest);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s,s0,id_dest->width);
  
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

static inline def_EHelper(rol) {
  if(id_src1->width == 4) *s0 = (*dsrc1<<24)>>24;
  for(; *s0!=0; (*s0)-=1) {
    rtl_msb(s, s1, ddest, id_dest->width);
    rtl_shli(s, ddest, ddest, 1);
    rtl_addi(s, ddest, ddest, *s1);
  }
  operand_write(s, id_dest, ddest);
  print_asm_template1(rol);
}