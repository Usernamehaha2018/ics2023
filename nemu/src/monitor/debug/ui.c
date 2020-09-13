#include <isa.h>
#include "expr.h"
#include "watchpoint.h"

#include <memory/paddr.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "expr.h"

void cpu_exec(uint64_t);
int is_batch_mode();
word_t expr(char *, bool *);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }
  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

/*
 * check if the str is empty, which means 
 * it only contains '\n', '\t', ' '
 */ 
static bool is_empty_arg(char *args){
  if(args==NULL)return true;
  int arg_len = strlen(args);
     for(int i=0;i<arg_len;i++){
      if(args[i]!=' '&&args[i]!='\0'&&args[i]!='\t'&&args[i]!='\n'){
         return false;
      }
   }
   return true;
}

/*
 * check if the "info " function is valid
 */
static int is_r_arg(char *args){
  if(args==NULL){
    return false;
  }
  bool flag_r = false, flag_w = false;
  for(int i=0;i<strlen(args);i++){
    if(args[i]==' '&&args[i]=='\t'){
        continue;
      }
    else{
      if(args[i]=='r'){
        if(flag_r)return 0;
        flag_r=true;}
      else if(args[i]=='w'){
        if(flag_w)return 0;
        flag_w = true;}
      else return 0;
      if(flag_w&&flag_r)return 0;
    }
  }
  if(flag_r)return 1;
  if(flag_w)return 2;
  return 0;
}



static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_si(char *args){
  if(is_empty_arg(args)){
     cpu_exec(1);
     return 0;
   }
  int sum = atoi(args);
  if(sum <= 0){
    printf("invalid input: %s\n", args);
  }
  else{
    cpu_exec(sum);
  }
  return 0;
}

static int cmd_info(char *args){
  if(is_r_arg(args)==1){
    isa_reg_display();
    return 0;
  }
  else if(is_r_arg(args)==2) {
    info_watchpoints();
    return 0;
  }
  else{
    return -1;
  }
}

static int cmd_x(char *args){
  char *num = strtok(args, " ");
  printf("%s\n",num);
  uint32_t nums;
  sscanf(num, "%d", &nums); 
  uint32_t addr;
  char *address = num + strlen(num) + 1;
  sscanf(address, "%x", &addr); 
  printf("%d\n",addr);
  int i = 0;
  while(i<nums){
    printf("%#x\t", addr);   
	  printf("%#08x\r\n", paddr_read(addr, 4));
    addr += 4;
    i+=1;
  }
  return 0;
}

static int cmd_p(char *args){
  bool success = true;
  int ans = expr(args, &success);
  if(success)printf("%d",ans);
  else assert(0);
  return 0;
}

static int cmd_w(char *args){
  bool success = true;
  int val = expr(args, &success);
  if(!success){
    assert(0);
  }
  else{
    int word_t_val;
    sscanf(args, "%d", &word_t_val); 
    new_wp(word_t_val, val,args);
    return 0;
  }
}
static int cmd_d(char* args){
  int word_t_val;
  sscanf(args, "%d", &word_t_val); 
  bool success = true;
  free_wp(word_t_val, &success);
  if(!success)assert(0);
  return 0;
}

static int cmd_help(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "After displaying several commands, the program stops.", cmd_si},
  { "info", "print values of the registers and the watchpoints.", cmd_info},
  { "x", "cao", cmd_x},
  { "p", "calculate", cmd_p},
  { "watch", "add a watchpoint", cmd_w},
  { "d", "delete a watchpoint", cmd_d},


  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop() {
  if (is_batch_mode()) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
