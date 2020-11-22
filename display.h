/*
 * Professional MultiSwimmers
 * ECE 4440
 * 
 * display.h
 * 
 * Author: Patrick Thomas
 */

#include "debug.h"
#include "msp.h"
#include "tetris.h"

#pragma once

#define SMALL_SLEEP \
    __nop();        \
    __nop();        \
    __nop();        \
    __nop();        \
    __nop();        \
    __nop();        \
    __nop();        \
    __nop();        \
    __nop();        \
    __nop();        \
    __nop()
#define WRITE_DISPLAY_LATCH(val)                      \
    P1->OUT = val ? P1->OUT | BIT6 : P1->OUT & ~BIT6; \
    SMALL_SLEEP
#define TOGGLE_DISPLAY_LATCH \
    P1->OUT ^= BIT6;         \
    SMALL_SLEEP;             \
    P1->OUT ^= BIT6;         \
    SMALL_SLEEP;
#define WRITE_DISPLAY_OE(val)                         \
    P5->OUT = val ? P5->OUT | BIT6 : P5->OUT & ~BIT6; \
    SMALL_SLEEP
#define WRITE_DISPLAY_CLK(val)                        \
    P2->OUT = val ? P2->OUT | BIT7 : P2->OUT & ~BIT7; \
    SMALL_SLEEP
#define TOGGLE_DISPLAY_CLK \
    P2->OUT ^= BIT7;       \
    SMALL_SLEEP;           \
    P2->OUT ^= BIT7;       \
    SMALL_SLEEP;
#define WRITE_DISPLAY_ADDR             \
    SMALL_SLEEP;                       \
    P4->OUT = display_address_union.b; \
    SMALL_SLEEP;
#define WRITE_DISPLAY_DATA                 \
    SMALL_SLEEP;                           \
    P5->OUT = display_data_union.b | BIT6; \
    SMALL_SLEEP;

#define DISPLAY_ROW_OFFSET 1
#define DISPLAY_COL_OFFSET 2

#define DISPLAY_WIDTH 32
#define DISPLAY_HEIGHT 64

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

struct
{
    BoardValue buffer[32][64];
} display;

/**
 * @brief Initialize the display pins.
 * 
 */
void display_init();

/**
 * @brief Translate a 10x20 board to a 32x64 one.
 * 
 * @param t The Tetris game object.
 */
void display_translate(Tetris *t);

/**
 * @brief Clear the entire buffer.
 */
void display_clear();

/**
 * @brief Write the display buffer to the display.
 *  
 */
void display_write_buffer();

///**
// * @brief Move a pixel around.
// *
// */
//void display_test_1();
//
///**
// * @brief Move a column around.
// *
// */
//void display_test_2();
//
///**
// * @brief Show random pixels from a Tetris board..
// *
// */
//void display_test_3();
