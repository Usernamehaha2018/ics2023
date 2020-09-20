#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
int times = 0;
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

uint32_t choose(uint32_t n) {
  uint32_t ans = rand() % n;
  return ans;
}

static void gen_num(){
  times += 2;
  uint32_t num = choose(99);
  while(num==0){
    num = choose(99);
  }
  char num_s[3];
  sprintf(num_s, "%u", num);
  strcat(buf, num_s);
}

static void gen(char s){
  times += 1;
  char ss[2];
  ss[0] = s;
  ss[1] = '\0';
  strcat(buf, ss);
}

static int gen_rand_op(){
  uint32_t num = choose(4);
  switch (num)
  {
  //case 0:gen('&&');break;
  //case 1:gen('==');break;
  //case 2:gen('!=');break;
  case 3:gen('+');break;
  case 0:gen('-');break;
  case 1:gen('*');break;
  case 2:gen('/');return 1;break;
  default:assert(0);break;
  }
  return 0;
}

static inline void gen_rand_expr() {
  if(times>50){
    gen_num();
    return;
  }
  switch (choose(3)) {
    case 0: gen_num(); break;
    case 1: gen('('); gen_rand_expr(); gen(')'); break;
    default: gen_rand_expr(); if(gen_rand_op()){gen_num();}else gen_rand_expr(); break;
  }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    buf[0] = '\0';
    times = 0;
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
