#include <Arduino.h>
#include <avr/pgmspace.h>
#include "translate.h"
#include "usbk.h"

static const unsigned int translation_table[] PROGMEM = {
  0,                               // 0x00 -> undefined (above bksp)
  USBK_BACKSPACE,                  // 0x01 -> bksp
  TRX_SHIFTED   | USBK_5,          // 0x02 -> %
  TRX_UNSHIFTED | USBK_SLASH,      // 0x03 -> /
  TRX_UNSHIFTED | USBK_8,          // 0x04 -> 8
  TRX_UNSHIFTED | USBK_0,          // 0x05 -> 0
  USBK_LT_ALT,                     // 0x06 -> alt
  USBK_RETURN,                     // 0x07 -> enter
  USBK_LT_SHIFT,                   // 0x08 -> shift
  0,                               // 0x09 -> undefined (below shift)
  TRX_SHIFTED   | USBK_O,          // 0x0A -> O
  TRX_SHIFTED   | USBK_4,          // 0x0B -> $
  TRX_UNSHIFTED | USBK_7,          // 0x0C -> 7
  TRX_UNSHIFTED | USBK_9,          // 0x0D -> 9
  USBK_LT_CTRL,                    // 0x0E -> ctrl
  TRX_UNSHIFTED | USBK_MINUS,      // 0x0F -> -
  1,                               // 0x10 -> fn
  USBK_LT_META,                    // 0x11 -> meta
  TRX_SHIFTED   | USBK_N,          // 0x12 -> N
  TRX_SHIFTED   | USBK_7,          // 0x13 -> &
  TRX_UNSHIFTED | USBK_6,          // 0x14 -> 6
  TRX_LIGATURE  | USBK_I,          // 0x15 -> fi
  TRX_UNSHIFTED | USBK_COMMA,      // 0x16 -> ,
  TRX_SHIFTED   | USBK_SEMICOLON,  // 0x17 -> :
  USBK_SYSRQ,                      // 0x18 -> prtsc
  USBK_DEL,                        // 0x19 -> del
  TRX_SHIFTED   | USBK_M,          // 0x1A -> M
  TRX_SHIFTED   | USBK_U,          // 0x1B -> U
  TRX_UNSHIFTED | USBK_5,          // 0x1C -> 5
  TRX_LIGATURE  | USBK_F,          // 0x1D -> ff
  TRX_UNSHIFTED | USBK_W,          // 0x1E -> w
  TRX_UNSHIFTED | USBK_PERIOD,     // 0x1F -> .
  USBK_HOME,                       // 0x20 -> home
  USBK_END,                        // 0x21 -> end
  TRX_SHIFTED   | USBK_L,          // 0x22 -> L
  TRX_SHIFTED   | USBK_J,          // 0x23 -> J
  TRX_UNSHIFTED | USBK_4,          // 0x24 -> 4
  TRX_UNSHIFTED | USBK_G,          // 0x25 -> g
  TRX_UNSHIFTED | USBK_P,          // 0x26 -> p
  TRX_UNSHIFTED | USBK_SEMICOLON,  // 0x27 -> ;
  USBK_PGDN,                       // 0x28 -> pgdn
  USBK_RIGHT,                      // 0x29 -> right arrow
  TRX_SHIFTED   | USBK_K,          // 0x2A -> K
  TRX_SHIFTED   | USBK_Z,          // 0x2B -> Z
  TRX_UNSHIFTED | USBK_3,          // 0x2C -> 3
  TRX_UNSHIFTED | USBK_F,          // 0x2D -> f
  TRX_UNSHIFTED | USBK_Y,          // 0x2E -> y
  TRX_UNSHIFTED | USBK_R,          // 0x2F -> r
  USBK_UP,                         // 0x30 -> up arrow
  USBK_DOWN,                       // 0x31 -> down arrow
  TRX_SHIFTED   | USBK_I,          // 0x32 -> I
  TRX_SHIFTED   | USBK_Y,          // 0x33 -> Y
  TRX_UNSHIFTED | USBK_2,          // 0x34 -> 2
  TRX_UNSHIFTED | USBK_S,          // 0x35 -> s
  0,                               // 0x36 -> undefined (between o and y)
  0,                               // 0x37 -> undefined (between a and r)
  USBK_PGUP,                       // 0x38 -> pgup
  USBK_LEFT,                       // 0x39 -> left arrow
  TRX_SHIFTED   | USBK_H,          // 0x3A -> H
  TRX_SHIFTED   | USBK_X,          // 0x3B -> X
  TRX_UNSHIFTED | USBK_1,          // 0x3C -> 1
  TRX_UNSHIFTED | USBK_I,          // 0x3D -> i
  TRX_UNSHIFTED | USBK_O,          // 0x3E -> o
  TRX_UNSHIFTED | USBK_A,          // 0x3F -> a
  TRX_SHIFTED   | USBK_MINUS,      // 0x40 -> _
  TRX_SHIFTED   | USBK_PERIOD,     // 0x41 -> >
  TRX_SHIFTED   | USBK_QUOTE,      // 0x42 -> "
  TRX_UNSHIFTED | USBK_EQUAL,      // 0x43 -> =
  0,                               // 0x44 -> undefined (between e and 1)
  0,                               // 0x45 -> undefined (between e and i)
  0,                               // 0x46 -> undefined (between h and o)
  0,                               // 0x47 -> undefined (between space and a)
  TRX_SHIFTED   | USBK_6,          // 0x48 -> ^
  TRX_SHIFTED   | USBK_COMMA,      // 0x49 -> <
  TRX_SHIFTED   | USBK_G,          // 0x4A -> G
  TRX_SHIFTED   | USBK_W,          // 0x4B -> W
  TRX_UNSHIFTED | USBK_E,          // 0x4C -> e
  0,                               // 0x4D -> undefined (between e and h)
  TRX_UNSHIFTED | USBK_H,          // 0x4E -> h
  USBK_SPACE,                      // 0x4F -> space
  TRX_SHIFTED   | USBK_3,          // 0x50 -> #
  TRX_SHIFTED   | USBK_RT_BRACKET, // 0x51 -> }
  TRX_SHIFTED   | USBK_F,          // 0x52 -> F
  TRX_SHIFTED   | USBK_V,          // 0x53 -> V
  TRX_UNSHIFTED | USBK_K,          // 0x54 -> k
  0,                               // 0x55 -> undefined (between d and e)
  0,                               // 0x56 -> undefined (between n and k)
  0,                               // 0x57 -> undefined (between t and space)
  TRX_SHIFTED   | USBK_2,          // 0x58 -> @
  TRX_SHIFTED   | USBK_LT_BRACKET, // 0x59 -> {
  TRX_SHIFTED   | USBK_E,          // 0x5A -> E
  TRX_SHIFTED   | USBK_T,          // 0x5B -> T
  TRX_UNSHIFTED | USBK_QUOTE,      // 0x5C -> '
  TRX_UNSHIFTED | USBK_D,          // 0x5D -> d
  TRX_UNSHIFTED | USBK_N,          // 0x5E -> n
  TRX_UNSHIFTED | USBK_T,          // 0x5F -> t
  USBK_BACKSLASH,                  // 0x60 -> \ |
  TRX_UNSHIFTED | USBK_RT_BRACKET, // 0x61 -> ]
  TRX_SHIFTED   | USBK_D,          // 0x62 -> D
  TRX_SHIFTED   | USBK_S,          // 0x63 -> S
  TRX_SHIFTED   | USBK_EQUAL,      // 0x64 -> +
  TRX_UNSHIFTED | USBK_C,          // 0x65 -> c
  TRX_UNSHIFTED | USBK_M,          // 0x66 -> m
  TRX_UNSHIFTED | USBK_U,          // 0x67 -> u
  USBK_TILDE,                      // 0x68 -> ` ~
  TRX_UNSHIFTED | USBK_LT_BRACKET, // 0x69 -> [
  TRX_SHIFTED   | USBK_C,          // 0x6A -> C
  TRX_SHIFTED   | USBK_R,          // 0x6B -> R
  TRX_SHIFTED   | USBK_8,          // 0x6C -> *
  TRX_UNSHIFTED | USBK_B,          // 0x6D -> b
  TRX_UNSHIFTED | USBK_L,          // 0x6E -> l
  TRX_UNSHIFTED | USBK_V,          // 0x6F -> v
  0,                               // 0x70 -> undefined (above tab)
  USBK_TAB,                        // 0x71 -> tab
  TRX_SHIFTED   | USBK_B,          // 0x72 -> B
  TRX_SHIFTED   | USBK_Q,          // 0x73 -> Q
  TRX_SHIFTED   | USBK_0,          // 0x74 -> )
  TRX_SHIFTED   | USBK_SLASH,      // 0x75 -> ?
  TRX_UNSHIFTED | USBK_Z,          // 0x76 -> z
  TRX_UNSHIFTED | USBK_Q,          // 0x77 -> q
  USBK_ESC,                        // 0x78 -> esc
  0,                               // 0x79 -> undefined (below esc)
  TRX_SHIFTED   | USBK_A,          // 0x7A -> A
  TRX_SHIFTED   | USBK_P,          // 0x7B -> P
  TRX_SHIFTED   | USBK_9,          // 0x7C -> (
  TRX_UNSHIFTED | USBK_J,          // 0x7D -> j
  TRX_SHIFTED   | USBK_1,          // 0x7E -> !
  TRX_UNSHIFTED | USBK_X,          // 0x7F -> x
};

unsigned int translate_scancode(unsigned char packet) {
  return pgm_read_word(&translation_table[packet]);
}

unsigned int translate_packet(unsigned char packet) {
  if (packet & 0x80) return translate_scancode(packet & 0x7F) | TRX_RELEASED;
  if (packet) return translate_scancode(packet & 0x7F) | TRX_PRESSED;
  return 0;
}
