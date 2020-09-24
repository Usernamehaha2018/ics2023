#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {\
  if(s==NULL){return 0;}
  unsigned long i = 0;
	while((*s++) != '\0')
		++i;
	return i;
}

size_t strnlen(const char *s, size_t count)
{
    const char *sc;
    for (sc = s; count-- && *sc != '\0'; ++sc);
    return sc - s;
}

char *strcpy(char* dst,const char* src) {
  if(dst==NULL||src==NULL)
        return NULL;
  char *address = dst;  
	while((*dst++ = *src++) != '\0');  
	return address;
}

char* strncpy(char* dst, const char* src, size_t n) {
  return NULL;
}

char* strcat(char* dst, const char* src) {
  char *tmp = dst;

    while (*dst)
        dst++;
    while ((*dst++ = *src++) != '\0');
    return tmp;
}

int strcmp(const char* s1, const char* s2) {
  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  return 0;
}

void* memset(void* v,int c,size_t n) {
  if(v == NULL)return NULL;
  char* pDest = (char*)v;
  while (n-->0)*pDest++ = c;
  return v; 
}

void* memmove(void* dst,const void* src,size_t n) {
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) {
  if(out==NULL||in==NULL)return NULL;
  char* pDest = (char*)out;
  const char* pSrc = (const char*)in;
  while (n-- > 0){
  *pDest++ = *pSrc++; }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n) {
  if(s1==NULL||s2==NULL) assert(0);
  const char* pSrc1 = (char*)s1;
  const char* pSrc2 = (char*)s2;
  while (n-- > 0){
    if (*pSrc1++ != *pSrc2++){
    return *pSrc1 < *pSrc2 ? -1 : 1;
    }
  }
  return 0;  
}

#endif
