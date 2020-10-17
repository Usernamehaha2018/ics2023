#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO; //the number of watchpoint
  struct watchpoint *next;
  /* TODO: Add more members if necessary */
  word_t cur_val;
  char s[32];
} WP;
WP* new_wp(word_t,char*);
void free_wp(int, bool *);
bool check_watchpoint();
void info_watchpoints();
#endif
