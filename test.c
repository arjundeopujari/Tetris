/*
 * test.c
 *
 *  Created on: Oct 18, 2020
 *      Author: Nick
 */

#include "init.h"

void TestLEDMatrix() {

    // LED Board Init
    OE;
    NotCLK;
    NotLatch;
    NotR1;
    NotG1;
    NotB1;
    NotR2;
    NotG2;
    NotB2;
    NotA;
    NotB;
    NotC;
    NotD;
    uint16_t y;
    uint16_t x;

    // Testing Writing to All Rows separately

    // test A row
    OE;
    NotLatch;
    A;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 32; x++) {
            NotCLK;
            R1;
            G1;
            B1;
            CLK;
        }
    }
    Latch;
    NotA;
    NotOE;


    // test B row
    OE;
    NotLatch;
    B;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 32; x++) {
            NotCLK;
            R1;
            G1;
            B1;
            CLK;
        }
    }
    Latch;
    NotB;
    NotOE;

    // test C row
    OE;
    NotLatch;
    C;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 32; x++) {
            NotCLK;
            R2;
            G2;
            B2;
            CLK;
        }
    }
    Latch;
    NotC;
    NotOE;

    // test D row
    OE;
    NotLatch;
    D;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 32; x++) {
            NotCLK;
            R2;
            G2;
            B2;
            CLK;
        }
    }
    Latch;
    NotD;
    NotOE;

    __sleep();


    // Clear A row
    NotLatch;
    A;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 32; x++) {
            NotCLK;
            NotR1;
            NotG1;
            NotB1;
            CLK;
        }
    }
    Latch;
    NotA;

    // Moving Pixel Test on Row A
    uint16_t nowX = 0;
    uint16_t nowY = 0;

    NotLatch;
    A;
    for (nowY = 0; nowY < 16; nowY++) {
            for (nowX = 0; nowX < 32; nowX++) {
                for (y = 0; y < 16; y++) {
                        for (x = 0; x < 32; x++) {
                            if (nowX == x && nowY == y) {
                                NotCLK;
                                R1;
                                G1;
                                B1;
                                CLK;
                            }
                            else {
                                NotCLK;
                                NotR1;
                                NotG1;
                                NotB1;
                                CLK;
                            }

                        }
                    }
            }
        }

    Latch;
    NotA;
}


