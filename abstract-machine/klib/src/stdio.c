#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
#define is_digit(c)    ((c) >= '0' && (c) <= '9') 
#define ZEROPAD    1        /* pad with zero */
#define SIGN    2        /* unsigned/signed long */
#define PLUS    4        /* show plus */
#define SPACE    8        /* space if plus */
#define LEFT    16        /* left justified */
#define SPECIAL    32        /* 0x */
#define SMALL    64 
#define do_div(n,base) ({ \
    int __res; \
    __asm__("divl %4":"=a" (n),"=d" (__res):"0" (n),"1" (0),"r" (base)); \
    __res; })

/* change all str(digit) to number
 * until meet a char which is not a digit
 */ 
 static int skip_atoi(const char **s) 
 {
     int i=0;
 
     while (is_digit(**s))
         i = i*10 + *((*s)++) - '0';
     return i;
 }

static char * number(char * str, int num, int base, int size, int precision,int type)
{
    char tmp[36];
    const char *digits="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i;
    i=0;
    if (num==0) 
        tmp[i++]='0';
    else while (num!=0) 
        tmp[i++]=digits[do_div(num,base)];
    while(i-->0) 
        *str++ = tmp[i];
    while(size-->0)
        *str++ = ' ';
    return str;
}

    int vsprintf(char *buf, const char *fmt, va_list args) 
    {
        int len;
        char *str=buf;
        char *s;    
        int flags;          
        int field_width;   
        
     
        for (; *fmt; ++fmt) { 
             if (*fmt != '%') { 
                 *str++ = *fmt;
                 continue;
             }
            flags = 0;
            repeat:
                ++fmt;        
                switch (*fmt) { 
                    case '#': flags |= SPECIAL; goto repeat; 
                    case '0': flags |= ZEROPAD; goto repeat;
                    }
            
            /* get field width */
            field_width = -1; 
            if (is_digit(*fmt))field_width = skip_atoi(&fmt); 
            
            if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
                ++fmt;
            }
     
            switch (*fmt) {
     
            case 's':
                s = va_arg(args, char *);
                len = strlen(s);     
                while (len < field_width--)
                        *str++ = ' ';
                for (int i = 0; i < len; ++i) 
                    *str++ = *s++;
                while (len < field_width--)
                    *str++ = ' ';
                break;          
     
            case 'd': 
            case 'i':
                flags |= SIGN;
            case 'u': 
                str = number(str, va_arg(args, unsigned long), 10,
                    field_width, 0, flags);
                break;
     
            default:
                if (*fmt != '%') 
                    *str++ = '%';
                if (*fmt)
                    *str++ = *fmt;
                else
                    --fmt;
                break;
            }
        }
        *str = '\0'; //最后填上空字符
        return str-buf;
}

 
int printf(const char *fmt, ...) {
  char s[256];
  va_list args;
  va_start(args, fmt);
  int val = vsprintf(s, fmt, args);
  va_end(args);
  for(int i=0;s[i]!='\0';i++)
    putch(s[i]);
  return val;
}


int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int val = vsprintf(out, fmt, args);
  va_end(args);
  return val;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  return 0;
}

#endif
