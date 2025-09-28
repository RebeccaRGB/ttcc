#ifndef __MATRIX_H__
#define __MATRIX_H__

#define ROW_0    8
#define ROW_1    9
#define ROW_2   10
#define ROW_3   11
#define ROW_4   12
#define ROW_5   13
#define ROW_6   14
#define ROW_7   15

#define COL_0    0
#define COL_1    1
#define COL_2    2
#define COL_3    3
#define COL_4    4
#define COL_5    5
#define COL_6    6
#define COL_7    7
#define COL_8   16
#define COL_9   17
#define COL_10  18
#define COL_11  19
#define COL_12  20
#define COL_13  21
#define COL_14  22
#define COL_15  23

#define MTX_STATE_SIZE     16
#define MTX_PROP_DELAY    100
#define MTX_BUFFER_SIZE    64
#define MTX_DEBOUNCE_TIME  20

#define MTX_PRESSED      0x00
#define MTX_RELEASED     0x80

void pinModeRows(unsigned char mode);
void pinModeCols(unsigned char mode);
unsigned char digitalReadRows();
unsigned int digitalReadCols();
void digitalWriteRows(unsigned char i);
void digitalWriteCols(unsigned int i);

void matrix_init();
void matrix_read(unsigned char * state);
unsigned char matrix_read_packet();

#endif
