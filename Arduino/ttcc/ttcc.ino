#include <Arduino.h>
#include <avr/pgmspace.h>
#include <usb_keyboard.h>
#include <util/delay.h>
#include "matrix.h"
#include "translate.h"
#include "usbk.h"

static unsigned char shiftState[MTX_STATE_SIZE * 8];

static void init_shift_state() {
  for (unsigned int i = 0; i < (MTX_STATE_SIZE * 8); i++) shiftState[i] = 0;
}

static void push_shift_up(unsigned char p) {
  // if shift is down, release shift
  unsigned char m = shiftState[p & 0x7F] = usbk_output_report()[0];
  if (m & USBK_MOD_LT_SHIFT) usbk_key_released(USBK_LT_SHIFT);
  if (m & USBK_MOD_RT_SHIFT) usbk_key_released(USBK_RT_SHIFT);
}

static void pop_shift_up(unsigned char p) {
  // if shift was down, press shift
  unsigned char m = shiftState[p & 0x7F];
  if (m & USBK_MOD_LT_SHIFT) usbk_key_pressed(USBK_LT_SHIFT);
  if (m & USBK_MOD_RT_SHIFT) usbk_key_pressed(USBK_RT_SHIFT);
}

static void push_shift_down(unsigned char p) {
  // if shift is up, press shift
  unsigned char m = shiftState[p & 0x7F] = usbk_output_report()[0];
  if (!(m & USBK_MOD_ANY_SHIFT)) usbk_key_pressed(USBK_LT_SHIFT);
}

static void pop_shift_down(unsigned char p) {
  // if shift was up, release shift
  unsigned char m = shiftState[p & 0x7F];
  if (!(m & USBK_MOD_ANY_SHIFT)) usbk_key_released(USBK_LT_SHIFT);
}

void setup() {
  init_shift_state();
  matrix_init();
}

void loop() {
  unsigned char p = matrix_read_packet();
  if (p) {
    unsigned int q = translate_packet(p);
    if (q) {
      unsigned char r = q;
      if (q & TRX_LIGATURE) {
        // ligature key
        if (q & TRX_PRESSED) {
          push_shift_up(p);
          usbk_key_pressed(USBK_F);
          usbk_key_released(USBK_F);
          usbk_key_pressed(r);
          usbk_key_released(r);
          pop_shift_up(p);
        }
      } else if (q & TRX_SHIFTED) {
        // shifted alphanumeric or symbol key
        if (q & TRX_PRESSED) {
          push_shift_down(p);
          usbk_key_pressed(r);
        }
        if (q & TRX_RELEASED) {
          usbk_key_released(r);
          pop_shift_down(p);
        }
      } else if (q & TRX_UNSHIFTED) {
        // unshifted alphanumeric or symbol key
        if (q & TRX_PRESSED) {
          push_shift_up(p);
          usbk_key_pressed(r);
        }
        if (q & TRX_RELEASED) {
          usbk_key_released(r);
          pop_shift_up(p);
        }
      } else {
        // function or modifier key
        if (q & TRX_PRESSED) usbk_key_pressed(r);
        if (q & TRX_RELEASED) usbk_key_released(r);
      }
    }
  }
}
