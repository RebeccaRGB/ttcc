#include <usb_keyboard.h>
#include "usbk.h"

static unsigned char keyReportBuf[8] = {0,0,0,0,0,0,0,0};

static void usbk_write_report() {
  Keyboard.set_modifier(keyReportBuf[0]);
  Keyboard.set_key1(keyReportBuf[2]);
  Keyboard.set_key2(keyReportBuf[3]);
  Keyboard.set_key3(keyReportBuf[4]);
  Keyboard.set_key4(keyReportBuf[5]);
  Keyboard.set_key5(keyReportBuf[6]);
  Keyboard.set_key6(keyReportBuf[7]);
  Keyboard.send_now();
}

void usbk_key_pressed(unsigned char key) {
  int i;
  if ((key & 0xF8) == 0xE0) {
    keyReportBuf[0] |= (1 << (key & 0x07));
    usbk_write_report();
    return;
  }
  for (i = 2; i < 8; i++) {
    if (keyReportBuf[i] == key) {
      keyReportBuf[i] = 0;
    }
  }
  if (key < 0x04) {
    for (i = 7; i >= 2; i--) {
      if (!keyReportBuf[i]) {
        keyReportBuf[i] = key;
        break;
      }
    }
  } else {
    for (i = 2; i < 8; i++) {
      if (!keyReportBuf[i]) {
        keyReportBuf[i] = key;
        break;
      }
    }
  }
  usbk_write_report();
}

void usbk_key_released(unsigned char key) {
  int i;
  if ((key & 0xF8) == 0xE0) {
    keyReportBuf[0] &=~ (1 << (key & 0x07));
    usbk_write_report();
    return;
  }
  for (i = 2; i < 8; i++) {
    if (keyReportBuf[i] == key) {
      keyReportBuf[i] = 0;
    }
  }
  usbk_write_report();
}

void usbk_set_error_state(unsigned char err) {
  keyReportBuf[2] = err;
  keyReportBuf[3] = err;
  keyReportBuf[4] = err;
  keyReportBuf[5] = err;
  keyReportBuf[6] = err;
  keyReportBuf[7] = err;
  usbk_write_report();
}

unsigned char usbk_get_error_state() {
  for (int i = 2; i < 8; i++) {
    if (keyReportBuf[i] > 0x00 && keyReportBuf[i] < 0x04) {
      return keyReportBuf[i];
    }
  }
  return 0;
}

unsigned char * usbk_output_report() {
  return keyReportBuf;
}

extern volatile unsigned char keyboard_leds;

unsigned char usbk_read_leds() {
  return keyboard_leds;
}
