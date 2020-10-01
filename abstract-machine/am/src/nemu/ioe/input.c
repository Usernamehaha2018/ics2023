#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
      int code = inl(0x60);
    code <<= 24;
    code >>= 24;
  if(code){
    kbd->keydown = 1;
    kbd->keycode = code;
  }
  else{
    kbd->keycode = 0;
    kbd->keydown = 0;
  }
}
