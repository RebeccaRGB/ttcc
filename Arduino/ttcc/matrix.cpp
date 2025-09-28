#include <Arduino.h>
#include <util/delay.h>
#include "matrix.h"

void pinModeRows(unsigned char mode) {
  pinMode(ROW_0, mode);
  pinMode(ROW_1, mode);
  pinMode(ROW_2, mode);
  pinMode(ROW_3, mode);
  pinMode(ROW_4, mode);
  pinMode(ROW_5, mode);
  pinMode(ROW_6, mode);
  pinMode(ROW_7, mode);
}

void pinModeCols(unsigned char mode) {
  pinMode(COL_0, mode);
  pinMode(COL_1, mode);
  pinMode(COL_2, mode);
  pinMode(COL_3, mode);
  pinMode(COL_4, mode);
  pinMode(COL_5, mode);
  pinMode(COL_6, mode);
  pinMode(COL_7, mode);
  pinMode(COL_8, mode);
  pinMode(COL_9, mode);
  pinMode(COL_10, mode);
  pinMode(COL_11, mode);
  pinMode(COL_12, mode);
  pinMode(COL_13, mode);
  pinMode(COL_14, mode);
  pinMode(COL_15, mode);
}

unsigned char digitalReadRows() {
  unsigned char i = 0;
  if (digitalRead(ROW_0)) i |= 0x01;
  if (digitalRead(ROW_1)) i |= 0x02;
  if (digitalRead(ROW_2)) i |= 0x04;
  if (digitalRead(ROW_3)) i |= 0x08;
  if (digitalRead(ROW_4)) i |= 0x10;
  if (digitalRead(ROW_5)) i |= 0x20;
  if (digitalRead(ROW_6)) i |= 0x40;
  if (digitalRead(ROW_7)) i |= 0x80;
  return i;
}

unsigned int digitalReadCols() {
  unsigned int i = 0;
  if (digitalRead(COL_0)) i |= 0x0001;
  if (digitalRead(COL_1)) i |= 0x0002;
  if (digitalRead(COL_2)) i |= 0x0004;
  if (digitalRead(COL_3)) i |= 0x0008;
  if (digitalRead(COL_4)) i |= 0x0010;
  if (digitalRead(COL_5)) i |= 0x0020;
  if (digitalRead(COL_6)) i |= 0x0040;
  if (digitalRead(COL_7)) i |= 0x0080;
  if (digitalRead(COL_8)) i |= 0x0100;
  if (digitalRead(COL_9)) i |= 0x0200;
  if (digitalRead(COL_10)) i |= 0x0400;
  if (digitalRead(COL_11)) i |= 0x0800;
  if (digitalRead(COL_12)) i |= 0x1000;
  if (digitalRead(COL_13)) i |= 0x2000;
  if (digitalRead(COL_14)) i |= 0x4000;
  if (digitalRead(COL_15)) i |= 0x8000;
  return i;
}

void digitalWriteRows(unsigned char i) {
  digitalWrite(ROW_0, (i & 0x01) ? HIGH : LOW);
  digitalWrite(ROW_1, (i & 0x02) ? HIGH : LOW);
  digitalWrite(ROW_2, (i & 0x04) ? HIGH : LOW);
  digitalWrite(ROW_3, (i & 0x08) ? HIGH : LOW);
  digitalWrite(ROW_4, (i & 0x10) ? HIGH : LOW);
  digitalWrite(ROW_5, (i & 0x20) ? HIGH : LOW);
  digitalWrite(ROW_6, (i & 0x40) ? HIGH : LOW);
  digitalWrite(ROW_7, (i & 0x80) ? HIGH : LOW);
}

void digitalWriteCols(unsigned int i) {
  digitalWrite(COL_0, (i & 0x0001) ? HIGH : LOW);
  digitalWrite(COL_1, (i & 0x0002) ? HIGH : LOW);
  digitalWrite(COL_2, (i & 0x0004) ? HIGH : LOW);
  digitalWrite(COL_3, (i & 0x0008) ? HIGH : LOW);
  digitalWrite(COL_4, (i & 0x0010) ? HIGH : LOW);
  digitalWrite(COL_5, (i & 0x0020) ? HIGH : LOW);
  digitalWrite(COL_6, (i & 0x0040) ? HIGH : LOW);
  digitalWrite(COL_7, (i & 0x0080) ? HIGH : LOW);
  digitalWrite(COL_8, (i & 0x0100) ? HIGH : LOW);
  digitalWrite(COL_9, (i & 0x0200) ? HIGH : LOW);
  digitalWrite(COL_10, (i & 0x0400) ? HIGH : LOW);
  digitalWrite(COL_11, (i & 0x0800) ? HIGH : LOW);
  digitalWrite(COL_12, (i & 0x1000) ? HIGH : LOW);
  digitalWrite(COL_13, (i & 0x2000) ? HIGH : LOW);
  digitalWrite(COL_14, (i & 0x4000) ? HIGH : LOW);
  digitalWrite(COL_15, (i & 0x8000) ? HIGH : LOW);
}

void matrix_init() {
  pinModeRows(OUTPUT);
  pinModeCols(INPUT_PULLUP);
}

static void matrix_read_raw(unsigned char * state) {
  unsigned int i, cs, cm;
  unsigned char rm;

  for (i = 0; i < MTX_STATE_SIZE; i++) state[i] = 0;

  digitalWriteRows(-1);
  _delay_us(MTX_PROP_DELAY);

  for (rm = 0x80; rm; rm >>= 1) {
    digitalWriteRows(~rm);
    _delay_us(MTX_PROP_DELAY);
    cs = ~digitalReadCols();
    for (i = 0, cm = 0x8000; cm; cm >>= 1, i++) {
      if (cs & cm) state[i] |= rm;
    }
  }

  digitalWriteRows(-1);
  _delay_us(MTX_PROP_DELAY);
}

static unsigned char matrix_xor(unsigned char * state1, unsigned char * state2) {
  unsigned char m = 0;
  for (unsigned int i = 0; i < MTX_STATE_SIZE; i++) {
    m |= state1[i] ^ state2[i];
  }
  return m;
}

void matrix_read(unsigned char * state) {
  unsigned char state1[MTX_STATE_SIZE];
  unsigned char state2[MTX_STATE_SIZE];
  matrix_read_raw(state1);
  matrix_read_raw(state2);
  while (matrix_xor(state1, state2)) {
    matrix_read_raw(state1);
    matrix_read_raw(state2);
  }
  for (unsigned int i = 0; i < MTX_STATE_SIZE; i++) {
    state[i] = state1[i];
  }
}

static unsigned char lastState[MTX_STATE_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static unsigned char packetBuffer[MTX_BUFFER_SIZE];
static unsigned int packetBufStart = 0;
static unsigned int packetBufEnd = 0;

static void matrix_write_packet(unsigned char packet) {
  packetBuffer[packetBufEnd] = packet;
  packetBufEnd++;
  packetBufEnd &= (MTX_BUFFER_SIZE-1);
  if (packetBufEnd == packetBufStart) {
    // Buffer overflow. Ignore input.
    packetBufEnd--;
    packetBufEnd &= (MTX_BUFFER_SIZE-1);
  }
}

unsigned char matrix_read_packet() {
  unsigned char nextState[MTX_STATE_SIZE];
  unsigned char released[MTX_STATE_SIZE];
  unsigned char pressed[MTX_STATE_SIZE];
  unsigned char changed = 0;
  unsigned char packet;
  unsigned int i;
  unsigned char m;

  matrix_read(nextState);
  for (i = 0; i < MTX_STATE_SIZE; i++) {
    released[i] = lastState[i] &~ nextState[i];
    pressed[i] = nextState[i] &~ lastState[i];
    changed |= released[i] | pressed[i];
    lastState[i] = nextState[i];
  }

  for (packet = 0, i = 0; i < MTX_STATE_SIZE; i++) {
    for (m = 1; m != 0; m <<= 1, packet++) {
      if (released[i] & m) matrix_write_packet(MTX_RELEASED | packet);
      if (pressed[i] & m) matrix_write_packet(MTX_PRESSED | packet);
    }
  }

  if (changed) delay(MTX_DEBOUNCE_TIME);

  if (packetBufStart == packetBufEnd) return 0;
  packet = packetBuffer[packetBufStart];
  packetBufStart++;
  packetBufStart &= (MTX_BUFFER_SIZE-1);
  return packet;
}
