#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  if(inl(0x60)){
    int code = inl(0x60) & 0x7f;
    kbd->keydown = code < 128? 1 : 0;
    kbd->keycode = code;
  }
  else{
    kbd->keycode = 0;
    kbd->keydown = 0;
  }
}
