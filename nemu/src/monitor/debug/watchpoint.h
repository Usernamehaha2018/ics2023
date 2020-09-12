#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO; //the number of watchpoint
  struct watchpoint *next;
  /* TODO: Add more members if necessary */
  int cur_val;
  int address;
  char s[32];
} WP;
WP* new_wp(int,int,char*);
void free_wp(int, bool *);
bool check_watchpoint();
void info_watchpoints();
#endif
