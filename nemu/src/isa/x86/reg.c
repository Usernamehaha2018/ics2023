#include <isa.h>
#include <stdlib.h>
#include <time.h>
#include "local-include/reg.h"

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

const char *regsl_[] = {"$eax", "$ecx", "$edx", "$ebx", "$esp", "$ebp", "$esi", "$edi"};
const char *regsw_[] = {"$ax", "$cx", "$dx", "$bx", "$sp", "$bp", "$si", "$di"};
const char *regsb_[] = {"$al", "$cl", "$dl", "$bl", "$ah", "$ch", "$dh", "$bh"};

void reg_test() {
  srand(time(0));
  word_t sample[8];
  word_t pc_sample = rand();
  cpu.pc = pc_sample;

  int i;
  for (i = R_EAX; i <= R_EDI; i ++) {
    sample[i] = rand();
    reg_l(i) = sample[i];
    assert(reg_w(i) == (sample[i] & 0xffff));
  }

  assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
  assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
  assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
  assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
  assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
  assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
  assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
  assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

  assert(sample[R_EAX] == cpu.eax);
  assert(sample[R_ECX] == cpu.ecx);
  assert(sample[R_EDX] == cpu.edx);
  assert(sample[R_EBX] == cpu.ebx);
  assert(sample[R_ESP] == cpu.esp);
  assert(sample[R_EBP] == cpu.ebp);
  assert(sample[R_ESI] == cpu.esi);
  assert(sample[R_EDI] == cpu.edi);

  assert(pc_sample == cpu.pc);
}

void isa_reg_display() {
  int i=0;
  for (i = R_EAX; i <= R_EDI; i ++) {
    printf("%s\t%x\t%u\n", regsl[i], reg_l(i), reg_l(i));   
  }
  printf("cpu.pc:%x\n",cpu.pc);
  // for (i = R_AX; i <= R_DI; i ++) {
  //   printf("%s\t%x\t%u\n", regsw[i], reg_w(i), reg_w(i));   
  // }
  // for (i = R_AL; i <= R_BH; i ++) {
  //   printf("%s\t%x\t%u\n", regsb[i], reg_b(i), reg_b(i));   
  // }
}


/* 
 * kill the spaces in the string, eg. "   $eax    " -> "$eax"
 */

char *ltrim(char *str)
{
    if (str == NULL || *str == '\0')return str;
    int len = 0;
    char *p = str;
    while (*p != '\0' && *p==' ')
    {
        ++p;
        ++len;
    }
    memmove(str, p, strlen(str) - len + 1);
    return str;
}
char *rtrim(char *str)
{
    if (str == NULL || *str == '\0')return str;
    int len = strlen(str);
    char *p = str + len - 1;
    while (p >= str  && (*p==' '))
    {
       *p = '\0';
        --p;
    }
    return str;
}

word_t isa_reg_str2val(const char *s, bool *success) {
  *success = true;
  char ss[32];
  strcpy(ss,s);
  ltrim(ss);
  rtrim(ss);
  for (int i = R_EAX; i <= R_EDI; i ++) {
    if(!strcmp(ss,regsl_[i])){*success=true;return reg_l(i); }
    if(!strcmp(ss,regsw_[i])){*success=true;return reg_w(i); }
    if(!strcmp(ss,regsb_[i])){*success=true;return reg_b(i); }
  }
  if(!strcmp(ss,"$pc")){
    return cpu.pc;
  }
  *success=false;
  return 0;
}
