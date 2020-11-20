/*
 * Professional MultiSwimmers
 * ECE 4440
 * 
 * display.h
 * 
 * Author: Patrick Thomas
 */

#include "msp.h"
#include "tetris.h"

#pragma once

#define WRITE_DISPLAY_LATCH(val) P1->OUT = val ? P1->OUT | BIT6 : P1->OUT & ~BIT6
#define TOGGLE_DISPLAY_LATCH \
    P1->OUT ^= BIT6;         \
    __nop();                 \
    P1->OUT ^= BIT6
#define WRITE_DISPLAY_OE(val) P5->OUT = val ? P5->OUT | BIT6 : P5->OUT & ~BIT6
#define WRITE_DISPLAY_CLK(val) P2->OUT = val ? P2->OUT | BIT7 : P2->OUT & ~BIT7
#define TOGGLE_DISPLAY_CLK \
    P2->OUT ^= BIT7;       \
    __nop();               \
    P2->OUT ^= BIT7
#define WRITE_DISPLAY_ADDR P4->OUT = display_address_union.b
#define WRITE_DISPLAY_DATA P5->OUT = display_data_union.b | BIT6; // Bit 6 is OE, which will always be high when we are writing data.

#define DISPLAY_ROW_OFFSET 1
#define DISPLAY_COL_OFFSET 2

/*
1.6 LAT
2.7 CLK
4.0 A +
4.1 B +
4.2 C +
4.3 D +
5.0 R1 +
5.1 B1
5.2 G1
5.4 R2
5.5 B2
5.6 OE
5.7 G2
*/

struct __attribute__((__packed__)) display_address
{
    unsigned int a : 1;
    unsigned int b : 1;
    unsigned int c : 1;
    unsigned int d : 1;
    unsigned int _unused : 4;
};

union
{
    struct display_address s;
    unsigned char b : 8;
} display_address_union;

struct __attribute__((__packed__)) display_data
{
    unsigned int r1 : 1;
    unsigned int b1 : 1;
    unsigned int g1 : 1;
    unsigned int _unused1 : 1;
    unsigned int r2 : 1;
    unsigned int b2 : 1;
    unsigned int _unused2 : 1;
    unsigned int g2 : 1;
};

union
{
    struct display_data s;
    unsigned char b : 8;
} display_data_union;

void display_init();
void display_write(Tetris *t);
void display_write_array(BoardValue board[64][32]);
