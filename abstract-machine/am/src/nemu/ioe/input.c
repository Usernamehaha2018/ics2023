#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  if(inb(0x60)){
    int code = inb(0x60) & 0xff;
    kbd->keydown = code < 128? 1: 0;
    kbd->keycode = code;
  }
  else{
    kbd->keycode = 0;
    kbd->keydown = 0;
  }
}
