#include <cpu/exec.h>
#include "../local-include/decode.h"
#include "all-instr.h"
#include <isa.h>

static inline void set_width(DecodeExecState *s, int width) {
  if (width == -1) return;
  if (width == 0) {
    width = s->isa.is_operand_size_16 ? 2 : 4;
  }
  s->src1.width = s->dest.width = s->src2.width = width;
}
//8d60ad19153200e7c --git log
/* 0x80, 0x81, 0x83 */
static inline def_EHelper(gp1) {
  switch (s->isa.ext_opcode) {
    IDEXW(0x0,empt,add,-1) IDEXW(0x1, empt, or,-1) IDEXW(0x2,empt,adc,-1) IDEXW(0x3,empt,sbb,-1)
    IDEXW(0x4,empt, and,-1) IDEXW(0x5,empt,sub,-1) IDEXW(0x6,empt,xor,-1) IDEXW(0x7,empt,cmp,-1)
  }
}

/* 0xc0, 0xc1, 0xd0, 0xd1, 0xd2, 0xd3 */
static inline def_EHelper(gp2) {
  switch (s->isa.ext_opcode) {
    IDEXW(0,empt,rol,-1) IDEXW(0x1,empt,ror,-1) EMPTY(2) EMPTY(3)
    IDEXW(0x4,empt,shl,-1) IDEXW(0x5,empt, shr, -1) EMPTY(6) IDEXW(0x7, empt, sar, -1)
  }
}

/* 0xf6, 0xf7 */
static inline def_EHelper(gp3) {
  switch (s->isa.ext_opcode) {
    IDEXW (0x0, test_I, test, -1) EMPTY(1) IDEXW(0x2,empt,not,-1) IDEXW(0x3,empt,neg,-1)
    IDEXW(0x4,empt, mul,-1) IDEXW(0x5,empt,imul1,-1) IDEXW(0x6,empt,div,-1) IDEXW(0x7,empt,idiv,-1)
  }
}

/* 0xfe */
static inline def_EHelper(gp4) {
  switch (s->isa.ext_opcode) {
    IDEXW(0x0, empt,inc, -1) IDEXW(0x1, empt,dec, -1) EMPTY(2) EMPTY(3)
    EMPTY(4) EMPTY(5) EMPTY(6) EMPTY(7)
  }
}

/* 0xff */
static inline def_EHelper(gp5) {
  switch (s->isa.ext_opcode) {
    IDEXW(0x0,empt,inc,-1) IDEXW(0x1,empt,dec,-1) IDEXW(0x2,empt,call_rm,-1) EMPTY(3)
    IDEXW(0x4,empt,jmp_rm,-1) EMPTY(5) IDEXW(0x6,empt,push,-1) EMPTY(7)
  }
}

/* 0x0f 0x01*/
static inline def_EHelper(gp7) {
  switch (s->isa.ext_opcode) {
    EMPTY(0) EMPTY(1) EMPTY(2) EMPTY(3)
    EMPTY(4) EMPTY(5) EMPTY(6) EMPTY(7)
  }
}

static inline def_EHelper(2byte_esc) {
  uint8_t opcode = instr_fetch(&s->seq_pc, 1);
  s->opcode = opcode;
  switch (opcode) {
  /* TODO: Add more instructions!!! */
    IDEX (0x01, gp7_E, gp7)
    IDEX (0x82, J, jcc)
    IDEX (0x83, J, jcc)
    IDEX (0x84, J, jcc)
    IDEX (0x85, J, jcc)
    IDEX (0x86, J, jcc)
    IDEX (0x87, J, jcc)
    IDEX (0x88, J, jcc)
    IDEX (0x8c, J, jcc)
    IDEX (0x8d, J, jcc)
    IDEX (0x8e, J, jcc)
    IDEX (0x8f, J, jcc)
    IDEXW(0x92, setcc_E, setcc, 1)
    IDEXW(0x94, setcc_E, setcc, 1)
    IDEXW(0x95, setcc_E, setcc, 1)
    IDEXW(0x9f, setcc_E, setcc, 1)
    IDEXW(0x9d, setcc_E, setcc, 1)
    IDEXW(0x9e, setcc_E, setcc, 1)
    IDEX (0xa5, cl_G2E, shld)
    IDEX (0xaf, E2G, imul2)
    IDEX (0xbd, E2G, bsr)
    IDEXW(0xbe, E2G, movsx, 1)
    IDEXW(0xbf, E2G, movsx, 2)
    IDEXW(0xb6, mov_E2G, movzx, 1)
    IDEXW(0xb7, mov_E2G, movzx, 2)
    default: exec_inv(s);
  }
}

static inline void fetch_decode_exec(DecodeExecState *s) {
  uint8_t opcode;
again:
  opcode = instr_fetch(&s->seq_pc, 1); // 取指
  s->opcode = opcode;  //操作码
  //printf("%x\n",cpu.pc);
//   if (opcode == 0xd3) printf("op:%x,pc:%x\n",opcode,cpu.pc);
// if(opcode==0xd3){
//     isa_reg_display();
//   }
//   if(opcode==0x90){
    
//   }
  /*
   * I:imm
   * r:reg
   * G:register
   * E:r/m
   * a:al ax
   * 0:moff
   */
  switch (opcode) {
    IDEXW (0x00, G2E, add, 1)
    IDEXW (0x02, E2G, add, 1)
    IDEX  (0x05, I2a, add)
    IDEXW (0x08, G2E, or, 1)
    IDEXW (0x0a, E2G, or, 1)
    IDEX (0x0d, I2a, or)
    IDEX (0x01, G2E, add)
    IDEX (0x03, E2G, add)
    IDEX (0x09, G2E, or)
    IDEX (0x0b, E2G, or)
    IDEX (0x11, G2E, adc)
    IDEX (0x13, E2G, adc)
    IDEX (0x19, G2E, sbb)
    IDEXW(0x20, G2E, and, 1)
    IDEX (0x1b, E2G, sbb)
    IDEX (0x21, G2E, and)
    IDEXW(0x22, E2G, and, 1)
    IDEX (0x23, E2G, and)
    IDEXW(0x24, I2a, and, 1)
    IDEX (0x25, I2a, and)
    IDEX (0x29, G2E, sub)
    IDEX (0x2b, E2G, sub)
    IDEXW(0x2c, I2a, sub, 1)
    IDEX (0x2d, I2a, sub)
    
    IDEXW(0x30, G2E, xor, 1)
    IDEX (0x31, G2E, xor)
    IDEXW(0x32, E2G, xor, 1)
    IDEX (0x33, E2G, xor)
    IDEX (0x35, I2a, xor)
    IDEXW(0x38, G2E, cmp, 1)
    IDEX (0x39, G2E, cmp)
    IDEXW(0x3a, E2G, cmp, 1)
    IDEXW(0x3c, I2a, cmp, 1)
    IDEX (0x3b, E2G, cmp)
    IDEX (0x3d, I2a, cmp)
    IDEX (0x40, r, inc)
    IDEX (0x41, r, inc)
    IDEX (0x42, r, inc)
    IDEX (0x43, r, inc)
    IDEX (0x44, r, inc)
    IDEX (0x45, r, inc)
    IDEX (0x46, r, inc)
    IDEX (0x47, r, inc)
    IDEX (0x48, r, dec)
    IDEX (0x49, r, dec)
    IDEX (0x4a, r, dec)
    IDEX (0x4b, r, dec)
    IDEX (0x4e, r, dec)
    IDEX (0x50, r, push)
    IDEX (0x51, r, push)
    IDEX (0x52, r, push)
    IDEX (0x53, r, push)
    IDEX (0x54, r, push)
    IDEX (0x55, r, push)
    IDEX (0x56, r, push)
    IDEX (0x57, r, push)
    IDEX (0x58, r, pop)
    IDEX (0x59, r, pop)
    IDEX (0x5a, r, pop)
    IDEX (0x5b, r, pop)
    IDEX (0x5d, r, pop)
    IDEX (0x5e, r, pop)
    IDEX (0x5f, r, pop)
    IDEX (0x68, I, push)
    IDEX (0x69, I2r, imul1)
    IDEX (0x6b, SI_E2G, imul3)
    IDEXW(0x6a, I, push, 1)
    IDEXW(0x72, J, jcc, 1)
    IDEXW(0x73, J, jcc, 1)
    IDEXW(0x74, J, jcc, 1)
    IDEXW(0x75, J, jcc, 1)
    IDEXW(0x76, J, jcc, 1)
    IDEXW(0x77, J, jcc, 1)
    IDEXW(0x78, J, jcc, 1)
    IDEXW(0x79, J, jcc, 1)
    IDEXW(0x7c, J, jcc, 1)
    IDEXW(0x7d, J, jcc, 1)
    IDEXW(0x7e, J, jcc, 1)
    IDEXW(0x7f, J, jcc, 1)
    EX   (0x0f, 2byte_esc)
    IDEXW(0x80, I2E, gp1, 1)
    IDEX (0x81, I2E, gp1)
    IDEX (0x83, SI2E, gp1)
    IDEXW(0x84, G2E, test, 1)
    IDEX (0x85, G2E, test)
    IDEXW(0x88, mov_G2E, mov, 1)
    IDEX (0x89, mov_G2E, mov)
    IDEXW(0x8a, mov_E2G, mov, 1)
    IDEX (0x8b, mov_E2G, mov)
    IDEX (0x8d, lea_M2G, lea)
    EX   (0x90, nop)
    EX   (0x98, cwtl)
    EX   (0x99, cltd)
    IDEXW(0xa0, O2a, mov, 1)
    IDEX (0xa1, O2a, mov)
    IDEXW(0xa2, a2O, mov, 1)
    IDEX (0xa3, a2O, mov)
    IDEXW(0xa4, empt, movsb, 1)
    IDEXW(0xa8, I2a, test, 1)
    IDEXW(0xb0, mov_I2r, mov, 1)
    IDEXW(0xb1, mov_I2r, mov, 1)
    IDEXW(0xb2, mov_I2r, mov, 1)
    IDEXW(0xb3, mov_I2r, mov, 1)
    IDEXW(0xb4, mov_I2r, mov, 1)
    IDEXW(0xb5, mov_I2r, mov, 1)
    IDEXW(0xb6, mov_I2r, mov, 1)
    IDEXW(0xb7, mov_I2r, mov, 1)
    IDEX (0xb8, mov_I2r, mov)
    IDEX (0xb9, mov_I2r, mov)
    IDEX (0xba, mov_I2r, mov)
    IDEX (0xbb, mov_I2r, mov)
    IDEX (0xbc, mov_I2r, mov)
    IDEX (0xbd, mov_I2r, mov)
    IDEX (0xbe, mov_I2r, mov)
    IDEX (0xbf, mov_I2r, mov)
    IDEXW(0xc0, gp2_Ib2E, gp2, 1)
    IDEX (0xc1, gp2_Ib2E, gp2)
    EX   (0xc3, ret)
    IDEXW(0xc6, mov_I2E, mov, 1)
    IDEX (0xc7, mov_I2E, mov)
    EX   (0xc9, leave)
    IDEXW(0xd0, gp2_1_E, gp2, 1)
    IDEX (0xd1, gp2_1_E, gp2)
    IDEXW(0xd2, gp2_cl2E, gp2, 1)
    IDEX (0xd3, gp2_cl2E, gp2)
    EX   (0xd6, nemu_trap)
    IDEX (0xe8, J, call)
<<<<<<< HEAD
=======
    IDEX (0xe9, J, jmp)
    IDEXW(0xeb, J, jmp, 1)
    IDEXW(0xec, in_dx2a, in, 1)
    IDEX (0xed, in_dx2a, in)
    IDEXW(0xee, out_a2dx, out, 1)
    IDEX (0xef, out_a2dx, out)
>>>>>>> pa2
    IDEXW(0xf6, E, gp3, 1)
    IDEX (0xf7, E, gp3)
    IDEXW(0xfe, E, gp4, 1)
    IDEX (0xff, E, gp5)
  case 0x3e: goto again;
  case 0xf3: s->seq_pc += 3;break;
  case 0x66: s->isa.is_operand_size_16 = true; goto again;
  default: exec_inv(s);
  }
}

vaddr_t isa_exec_once() {
  DecodeExecState s;
  s.is_jmp = 0;
  s.isa = (ISADecodeInfo) { 0 };
  s.seq_pc = cpu.pc;
  fetch_decode_exec(&s);
  update_pc(&s);

  return s.seq_pc;
}
