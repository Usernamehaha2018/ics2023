#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {\
  // assert(s != NULL);
	int len = 0;
	while((*s++) != '\0')
		++len;
  printf("len%d\n",len);
	return len;
}

size_t strnlen(const char *s, size_t count)
{
    const char *sc;
    for (sc = s; count-- && *sc != '\0'; ++sc);
    return sc - s;
}

char *strcpy(char* dst,const char* src) {
  if(dst==NULL||src==NULL)
        assert(0);
  char *address = dst;  
	while((*dst++ = *src++) != '\0');  
	return address;
}

char* strncpy(char* dst, const char* src, size_t n) {
  return NULL;
}

char* strcat(char* dst, const char* src) {
  assert(dst != NULL && src != NULL);
  
  char *temp = dst;
  while (*temp != '\0')
    temp ++;
  while (*src != '\0'){
    *temp = *src;
    temp ++;
    src ++;
  }

  return dst;
}

int strcmp(const char* s1, const char* s2) {
  	if((s1==NULL) || (s2==NULL)){
      assert(0);
    };
    while(*s1 == *s2)
    {
        if(*s1 == '\0')
            {return 0; }      
        ++s1;
        ++s2;
    }
    if(*s1 - *s2 > 0)return 1;

    else return -1;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    	if((s1==NULL) || (s2==NULL)){
      assert(0);
    };
    int i = 0;
    while(*s1 == *s2 && i<n)
    {
        if(*s1 == '\0')
            {return 0; }      
        ++s1;
        ++s2;
        ++i;
    }
    if(i==n)return 0;
    if(*s1 - *s2 > 0)return 1;

    else return -1;
}

void* memset(void* v, int c,size_t n) {
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
