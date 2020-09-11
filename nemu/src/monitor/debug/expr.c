#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <memory/paddr.h>

enum { 
  TK_SPACE, //0
  TK_AND,  //1
  TK_EQ,   //2
  /* TODO: Add more token types and dont change the order*/
  TK_UEQ,  //3
  TK_ADD,  //4
  TK_MINUS, //5
  TK_MULTIPLE,  //6
  TK_DIVIDE,  //7

  NUM,
  TK_HEX,
  TK_REG,
  DEREF,  //11
  TK_LEFT_BRACKET,
  TK_RIGHT_BRACKET,
};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {" *0x[0-9a-fA-F]+ *", TK_HEX},  // hexadecimal_number 
  {" *[0-9]+ *", NUM},   // int
  {" *\\$[a-z]+ *", TK_REG},  // reg_name
  {" *\\( *", TK_LEFT_BRACKET},   // left bracket
  {" *\\) *", TK_RIGHT_BRACKET},   // right bracket

  {" *\\+ *", TK_ADD},   // plus
  {" *\\- *", TK_MINUS},   // minus
  {" *\\* *", TK_MULTIPLE},   // multiple
  {" *\\/ *", TK_DIVIDE},   //devide
  {" *== *", TK_EQ},   // equal
  {" *!= *", TK_UEQ},  // not equal
  {" *&& *", TK_AND}  // logical and
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);    
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
    else{
      printf("true\n");
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
        char res[32];
        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_AND: tokens[nr_token++].type=TK_AND;break;
          case TK_EQ: tokens[nr_token++].type=TK_EQ;break;
          case TK_HEX: tokens[nr_token++].type=TK_HEX;break;
          case TK_REG: tokens[nr_token].type=TK_REG;
              memcpy(res, substr_start, substr_len);
              strcpy(tokens[nr_token++].str,res);
              break;
          case TK_UEQ: tokens[nr_token++].type=TK_UEQ;break;
          case NUM:tokens[nr_token].type=NUM;
          		memcpy(res, substr_start, substr_len);
              strcpy(tokens[nr_token++].str,res);
              break;
          case TK_ADD:tokens[nr_token++].type=TK_ADD;break;
          case TK_MINUS:tokens[nr_token++].type=TK_MINUS;break;
          case TK_MULTIPLE:tokens[nr_token++].type=TK_MULTIPLE;break;
          case TK_DIVIDE:tokens[nr_token++].type=TK_DIVIDE;break;
          case TK_LEFT_BRACKET:tokens[nr_token++].type=TK_LEFT_BRACKET;break;
          case TK_RIGHT_BRACKET:tokens[nr_token++].type=TK_RIGHT_BRACKET;break;


         default: TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


bool check_parentheses(word_t p, word_t q){
  if(tokens[p].type==TK_LEFT_BRACKET&&tokens[q].type==TK_RIGHT_BRACKET){
    return true;
  }
  else return false;
}


int find_main_opt(word_t p, word_t q){
  word_t left_bracket_num = 0, right_bracket_num = 0;
  int main_opt=-1;
  bool in_bracket_state = false;
  for(int i=p;i<q;i++){
    switch (tokens[i].type) {
      case TK_LEFT_BRACKET: left_bracket_num +=1;
      if(left_bracket_num > right_bracket_num ){
        in_bracket_state = true;
        break;
      }
      else return -1;
      case TK_RIGHT_BRACKET: 
      right_bracket_num += 1;
      if(left_bracket_num == right_bracket_num ){
        in_bracket_state = false;
      }
      else if (left_bracket_num < right_bracket_num )return -1;
      break;
      case TK_EQ: case TK_UEQ: case TK_AND: case TK_ADD: case TK_MINUS: case TK_MULTIPLE: case TK_DIVIDE: case DEREF:
      if(!in_bracket_state){
        if(main_opt==-1){
          main_opt = i;
        }
        else if(tokens[main_opt].type/2>=tokens[i].type){
          main_opt = i;
        }
      }
      break;
      default: continue;
    }
  }
  return main_opt;
}

word_t eval(word_t p, word_t q, char *s){
  if (p > q) {
    if(tokens[p].type == DEREF){
      return 0;
    }
    else assert(0);
  }
  else if (p == q) {
    if(tokens[p].type==TK_HEX){
      word_t word_t_val = 0;          
      sscanf(tokens[p].str, "%x", &word_t_val); 
      return word_t_val;    
    }
    else if(tokens[p].type==NUM){
      word_t word_t_val = 0; 
      sscanf(tokens[p].str, "%d", &word_t_val); 
      return word_t_val;
    }
    else if(tokens[p].type==TK_REG){
      bool valid_reg = true;
      vaddr_t pos = isa_reg_str2val(tokens[p].str, &valid_reg);
      if(valid_reg)return pos;
      else assert(0);
    }
    else{
      return 0;
    }
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1, s);
  }
  else {
      int pos = find_main_opt(p,q);
      if(pos==-1)assert(0);
      else{
        int left_val = eval(p, pos-1, s);
        int right_val = eval(pos+1, q, s);
        switch(tokens[pos].type){
          case TK_MULTIPLE:return left_val*right_val;
          case TK_DIVIDE:return left_val/right_val;
          case TK_ADD: return left_val+right_val;
          case TK_MINUS: return left_val-right_val;
          case TK_AND: return left_val!=0 && right_val!=0;
          case TK_EQ: return left_val==right_val;
          case TK_UEQ: return left_val!=right_val;
          case DEREF: return paddr_read(right_val, 4);
          default:assert(0);
        }
      }
    /* We should do more things here. */
  }
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  //TODO();
  for(int i=0;i<nr_token-1;i++){
    if(i==0&&tokens[i].type==TK_MULTIPLE){
      tokens[i].type = DEREF;
    }
    else{
      if((tokens[i].type==TK_MULTIPLE)&&((tokens[i-1].type==TK_ADD)||
      (tokens[i-1].type==TK_MINUS)||(tokens[i-1].type==TK_MULTIPLE)||
      (tokens[i-1].type==TK_DIVIDE))){
        tokens[i].type = DEREF;
      }
    }    
  }
  for(int j=0;j<nr_token-1;j++){
      printf("%d, %s\n",tokens[j].type,tokens[j].str);
    }
  return 0;
  return eval(0, strlen(e), e);
}
