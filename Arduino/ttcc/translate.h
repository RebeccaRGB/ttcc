#ifndef __TRANSLATE_H__
#define __TRANSLATE_H__

#define TRX_UNSHIFTED  0x0100
#define TRX_SHIFTED    0x0200
#define TRX_LIGATURE   0x0400
#define TRX_PRESSED    0x1000
#define TRX_RELEASED   0x2000

unsigned int translate_scancode(unsigned char packet);
unsigned int translate_packet(unsigned char packet);

#endif
