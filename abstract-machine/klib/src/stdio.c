#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int num2str(char *s, int num){
  int len = 1;
  int n = num;
  while(n >= 10){
    n /= 10;
    len ++;
  }
  for(int i=1; i<=len; i++) {
    s[len-i] = num % 10 +'0';
    num /= 10;
  }
  return len;
}

int printf(const char *fmt, ...) {
  char s[256];
  va_list args;

  va_start(args, fmt);
  vsprintf(s, fmt, args);
  va_end(args);

  for(int i=0;s[i]!='\0';i++)
    putch(s[i]);

  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  //int flags;
  //int width;
  //int precision;
  //int length;

  while (*fmt){
    while (*fmt != '%' && *fmt){
      *out = *fmt;
      out ++;
      fmt ++;
    }
    if (!*fmt){
      *out = '\0';
      return 0;
    }
    fmt ++;

    // get flags
    // get width
    // get precision
    // get length
    // get specifier
    char *s = out;
    int num = 0;
    int len = 0;
    switch(*fmt){
      case 's':
        s = va_arg(ap, char*);
        len = strlen(s);
        break;
      case 'd':
        num = va_arg(ap, int);
        len = num2str(s, num);
        break;
      case 'u':
        num = va_arg(ap, int);
        break;
      default:
        break;
    }
    
    for(int i=0; i<len; i++){
      *out = *s;
      out ++;
      s ++;
    }
    fmt ++;
  }

  *out = '\0';
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  vsprintf(out, fmt, args);
  va_end(args);

  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}


#endif
