#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t code = inl(0x60) & 0xff;
  kbd->keydown = code < 128;
  if(kbd->keydown){
    kbd->keycode = code | KEYDOWN_MASK;
  }
  else kbd->keycode = AM_KEY_NONE;
}
