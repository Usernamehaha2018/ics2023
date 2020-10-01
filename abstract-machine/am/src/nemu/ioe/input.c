#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
    int code = inl(0x60);
    bool cur_code = code & 0x80;
    code <<= 24;
    code >>= 24;
  if(code){
    kbd->keydown = cur_code;
    kbd->keycode = code;
  }
  else{
    kbd->keycode = 0;
    kbd->keydown = 0;
  }
}
