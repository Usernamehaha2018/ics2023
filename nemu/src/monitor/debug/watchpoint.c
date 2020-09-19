#include "watchpoint.h"
#include "expr.h"
#include <memory/paddr.h>

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL, *tail = NULL, *free_tail = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
  free_tail = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
/* add a watchpoint */
WP* new_wp(int val, char *reg){
  /* if there is no free wp 
   * for convenience 
   * assert(0)
   */
  if(free_!=NULL){
    if(head==NULL){
      head=free_;
      tail = free_;  
      head->NO = 1; 
      head->cur_val = val;
      strcpy(head->s, reg);
      
    }
    else{
      int num = tail->NO + 1;
      tail->next = free_;
      tail = free_;
      tail->NO = num; 
      tail->cur_val = val;
      strcpy(tail->s, reg);   
    }
    if(free_->next)free_ = free_->next;
      else free_ = NULL; 
  }
  else assert(0);
  info_watchpoints();
  return tail;
}

void free_wp(int num, bool *success){
  if(head==tail&&head!=NULL){
    if(head->NO!=num){
      *success = false;
      return;
    }
    free_tail->next = head;
    free_tail = free_tail->next;
    head = NULL;
    tail =  NULL;
  } 
  else{
    WP* i = head;
    if(head==NULL){
      *success = false;
      return;
    }
    if(i->NO==num){
      head = i->next;
      free_tail->next = i;
      free_tail = i;
      return;
    }
    WP* j = head->next;
    for(;j->NO!=num&&j!=tail;j++);
    if(j->NO!=num){
      *success = false;
      return;
    }
    i->next = j->next;
    if(j == tail)tail = i;
    free_tail->next = j;
    free_tail = j;
  }
}

bool check_watchpoint(){
  if(head==NULL)return false;
  bool flag = false;
  for(WP* i = head;;i++){
    bool  success = true;
    int val = expr(i->s, &success);
    if(val!=i->cur_val){
      flag = true;
      printf("Hardware watchpoint %d: %s\n",i->NO,i->s);
      printf("Old value:%d\n",i->cur_val);
      printf("New value:%d\n",val);
      i->cur_val = val;
    }
    if(i==tail)break;
  }
  return flag;
}

void info_watchpoints(){
  if(head==NULL){
    printf("\n");
    return;
  }
  for(WP* i = head;;i++){
    printf("Num\tType\tWhat\n");
    printf("%d\thw watchpoint\t%s\n",i->NO,i->s);
    if(i==tail)return;
  }
}
