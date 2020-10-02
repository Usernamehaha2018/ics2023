#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
    int code = inl(0x60);
    int code_cur = inl(0x60) & 0xff;
    code <<= 24;
    code >>= 24;
  if(code){
    kbd->keydown = (code_cur == 0);
    kbd->keycode = code;
  }
  else{
    kbd->keycode = 0;
    kbd->keydown = 0;
  }
}
