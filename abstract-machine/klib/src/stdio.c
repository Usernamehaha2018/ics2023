#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
#define is_digit(c)    ((c) >= '0' && (c) <= '9') //判读一个字符是否在0~9范围内
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

static int skip_atoi(const char **s) 
{
    int i=0;
 
    while (is_digit(**s))
        i = i*10 + *((*s)++) - '0';
    return i;
}

static char * number(char * str, int num, int base, int size, int precision,int type)
{
    char c,sign,tmp[36];
    const char *digits="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i;
 
    if (type&SMALL) digits="0123456789abcdefghijklmnopqrstuvwxyz";
    if (type&LEFT) type &= ~ZEROPAD;
    if (base<2 || base>36) 
        return 0;
    c = (type & ZEROPAD) ? '0' : ' ' ; 
    if (type&SIGN && num<0) { 
        sign='-';
        num = -num; 
    } else
        sign=(type&PLUS) ? '+' : ((type&SPACE) ? ' ' : 0); 
    if (sign) size--;
    if (type&SPECIAL)
        {if (base==16) size -= 2;
        else if (base==8) size--;}
    i=0;
    if (num==0) 
        tmp[i++]='0';
    else while (num!=0) 
        tmp[i++]=digits[do_div(num,base)];
    if (i>precision)precision=i;
    size -= precision;
    if (!(type&(ZEROPAD+LEFT))) 
        {while(size-->0)
            *str++ = ' ';}
    if (sign) 
        *str++ = sign;
    if (type&SPECIAL){
        if (base==8)
            *str++ = '0';
        else if (base==16) {
            *str++ = '0';
            *str++ = digits[33];
        }
    }
    if (!(type&LEFT)) 
        while(size-->0)
            *str++ = c;
    while(i<precision--)
        *str++ = '0';
    while(i-->0) 
        *str++ = tmp[i];
    while(size-->0)
        *str++ = ' ';
    return str;
}

    int vsprintf(char *buf, const char *fmt, va_list args) //fmt是格式说明符号，buf是待写入数据的buffer
    {
        int len;
        int i;
        char * str;
        char *s;
        int *ip;
     
        int flags;        /* flags to number() */
     
        int field_width;    /* width of output field */
        int precision;        /* min. # of digits for integers; max
                       number of chars for from string */
        
     
        for (str=buf ; *fmt ; ++fmt) { //扫描格式说明符
            if (*fmt != '%') { //如果没有格式说明符，说明是普通的字符，直接写入str指向的buffer即可
                *str++ = *fmt;
                continue;
            }
            
            //到这一步，说明遇到了格式说明符
            /* process flags */
            flags = 0;
            repeat:
                ++fmt;        /* this also skips first '%' */
                switch (*fmt) { //取格式说明符%后面的一个字符，如果是 + - 空格 # 0
                    case '-': flags |= LEFT; goto repeat;  //左对齐
                    case '+': flags |= PLUS; goto repeat;  // 右对齐
                    case ' ': flags |= SPACE; goto repeat; //空格
                    case '#': flags |= SPECIAL; goto repeat; //我printf的时候。。。。这些都用的少。。。。
                    case '0': flags |= ZEROPAD; goto repeat;
                    }
            
            /* get field width */
            field_width = -1; //格式输出宽度
            if (is_digit(*fmt)) //如果是个数字
                field_width = skip_atoi(&fmt); //把格式输出宽度由字符转换成数字
            else if (*fmt == '*') { //如果是个*号
                /* it's the next argument */
                field_width = va_arg(args, int);
                if (field_width < 0) { //如果参数是个负数，那么左对齐
                    field_width = -field_width;
                    flags |= LEFT;
                }
            }
     
            /* get the precision */
            precision = -1;
            if (*fmt == '.') { //如果格式说明符号后面跟了'.' ,说明对输出的精度有要求
                ++fmt;    
                if (is_digit(*fmt))
                    precision = skip_atoi(&fmt);
                else if (*fmt == '*') { //如果是*，取args参数做为进度要求
                    /* it's the next argument */
                    precision = va_arg(args, int);
                }
                if (precision < 0) //精度必须大于0
                    precision = 0;
            }
            if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
                ++fmt;
            }
     
            switch (*fmt) {
            case 'c': //如果格式说明符是个c,把此时的参数当作ASCII码对待,说明将参数以字符的形式打印
                if (!(flags & LEFT)) //没有左对齐进入if
                    while (--field_width > 0)
                        *str++ = ' ';
                *str++ = (unsigned char) va_arg(args, int); //把对应参数写入str指定的buffer
                while (--field_width > 0) //对格式输出宽度有要求的时候，输出补齐空格
                    *str++ = ' ';
                break;
     
            case 's'://说明是个字符串
                s = va_arg(args, char *);
                len = strlen(s);
                if (precision < 0)  //其实我也没搞明白这个精度在字符串输出的时候，有啥用
                    precision = len;
                else if (len > precision)
                    len = precision;
     
                if (!(flags & LEFT)) //没有要求左对齐，就进入if
                    while (len < field_width--)
                        *str++ = ' ';
                for (i = 0; i < len; ++i) //这里把s指向的字符串的字符一个个的copy到str指向的buffer
                    *str++ = *s++;
                while (len < field_width--)
                    *str++ = ' ';
                break;
     
            case 'o': //八进制输出数
                str = number(str, va_arg(args, unsigned long), 8,
                    field_width, precision, flags);
                break;
     
            case 'p': //指针输出
                if (field_width == -1) {
                    field_width = 8;
                    flags |= ZEROPAD;
                }
                str = number(str,
                    (unsigned long) va_arg(args, void *), 16,
                    field_width, precision, flags);
                break;
     
            case 'x': //十六进制输出
                flags |= SMALL;
            case 'X':
                str = number(str, va_arg(args, unsigned long), 16,
                    field_width, precision, flags);
                break;
     
            case 'd': //有符号整形输出
            case 'i':
                flags |= SIGN;
            case 'u': //无符号整形输出
                str = number(str, va_arg(args, unsigned long), 10,
                    field_width, precision, flags);
                break;
     
            case 'n': //这个我真没用过。。。把到目前位置转换输出字符串数字保存到对应参数指针指定的位置
                ip = va_arg(args, int *);
                *ip = (str - buf);
                break;
     
            default:
                if (*fmt != '%') //这里我觉得有问题，if判断条件，不应该是！= 而应该是 ==
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
