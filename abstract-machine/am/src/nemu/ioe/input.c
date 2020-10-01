#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

static int keylut[128] = {
  [0x01] = AM_KEY_ESCAPE,               [0x02] = AM_KEY_1, [0x03] = AM_KEY_2,
  [0x04] = AM_KEY_3, [0x05] = AM_KEY_4, [0x06] = AM_KEY_5, [0x07] = AM_KEY_6,
  [0x08] = AM_KEY_7, [0x09] = AM_KEY_8, [0x0a] = AM_KEY_9, [0x0b] = AM_KEY_0,
  [0x0c] = AM_KEY_MINUS,                [0x0d] = AM_KEY_EQUALS,
  [0x0e] = AM_KEY_BACKSPACE,            [0x0f] = AM_KEY_TAB,
  [0x10] = AM_KEY_Q, [0x11] = AM_KEY_W, [0x12] = AM_KEY_E, [0x13] = AM_KEY_R,
  [0x14] = AM_KEY_T, [0x15] = AM_KEY_Y, [0x16] = AM_KEY_U, [0x17] = AM_KEY_I,
  [0x18] = AM_KEY_O, [0x19] = AM_KEY_P, [0x1a] = AM_KEY_LEFTBRACKET,
  [0x1b] = AM_KEY_RIGHTBRACKET,         [0x1c] = AM_KEY_RETURN,
  [0x1d] = AM_KEY_LCTRL,                [0x1e] = AM_KEY_A, [0x1f] = AM_KEY_S,
  [0x20] = AM_KEY_D, [0x21] = AM_KEY_F, [0x22] = AM_KEY_G, [0x23] = AM_KEY_H,
  [0x24] = AM_KEY_J, [0x25] = AM_KEY_K, [0x26] = AM_KEY_L,
  [0x27] = AM_KEY_SEMICOLON,            [0x28] = AM_KEY_APOSTROPHE,
  [0x29] = AM_KEY_GRAVE,                [0x2a] = AM_KEY_LSHIFT,
  [0x2b] = AM_KEY_BACKSLASH,            [0x2c] = AM_KEY_Z, [0x2d] = AM_KEY_X,
  [0x2e] = AM_KEY_C, [0x2f] = AM_KEY_V, [0x30] = AM_KEY_B, [0x31] = AM_KEY_N,
  [0x32] = AM_KEY_M,     [0x33] = AM_KEY_COMMA,  [0x34] = AM_KEY_PERIOD,
  [0x35] = AM_KEY_SLASH, [0x36] = AM_KEY_RSHIFT, [0x38] = AM_KEY_LALT,
  [0x38] = AM_KEY_RALT,  [0x39] = AM_KEY_SPACE,  [0x3a] = AM_KEY_CAPSLOCK,
  [0x3b] = AM_KEY_F1,    [0x3c] = AM_KEY_F2,     [0x3d] = AM_KEY_F3,
  [0x3e] = AM_KEY_F4,    [0x3f] = AM_KEY_F5,     [0x40] = AM_KEY_F6,
  [0x41] = AM_KEY_F7,    [0x42] = AM_KEY_F8,     [0x43] = AM_KEY_F9,
  [0x44] = AM_KEY_F10,   [0x48] = AM_KEY_INSERT,
  [0x4b] = AM_KEY_HOME,  [0x4d] = AM_KEY_END,    [0x50] = AM_KEY_DELETE,
  [0x57] = AM_KEY_F11,   [0x58] = AM_KEY_F12,    [0x5b] = AM_KEY_APPLICATION,
};

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
if (inb(0x64) & 0x1) {
    int code = inb(0x60) & 0xff;
    kbd->keydown = code < 128;
    kbd->keycode = keylut[code & 0x7f];
  } else {
    kbd->keydown = false;
    kbd->keycode = AM_KEY_NONE;
  }
}
