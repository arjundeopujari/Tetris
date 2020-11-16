/*
 * Professional MultiSwimmers
 * ECE 4440
 * 
 * display.c
 * 
 * Author: Patrick Thomas
 */

#include "display.h"
#include "msp.h"
#include "tetris.h"
#include "types.h"

void display_init()
{
    // Initialize display logic.
    WRITE_DISPLAY_LATCH(0);
    WRITE_DISPLAY_OE(0);
    WRITE_DISPLAY_CLK(0);

    display_address_union.b = 0;
    display_data_union.b = 0;

    WRITE_DISPLAY_ADDR;
    WRITE_DISPLAY_DATA;

    TOGGLE_DISPLAY_CLK;
}

void display_write(Tetris *t)
{
    /* 
    Board is 64x32, written long ways.
    Tetris is 10x20.
    We write Tetris sideways with a y-offset of 2 and x-offset of 1.
    */

    WRITE_DISPLAY_OE(1);

    int i, j;
    // For each display row
    for (i = 0; i < 16; i++)
    {
        display_address_union.b = i;
        WRITE_DISPLAY_OE(1);
        WRITE_DISPLAY_ADDR;

        // Translate to game col.
        unsigned char col1 = i - 2 / 3;
        unsigned char col2 = (i + 16) - 2 / 3;

        // For each display column
        for (j = 0; j < 64; j++)
        {
            // Translate to game row.
            unsigned char row = j - 4 / 3;

            // Reset the colors.
            display_data_union.b = 0x00;

            // Write black if before/after game region.
            if (!(row < 0 || row >= GAME_HEIGHT))
            {
                if (0 <= col1 && col1 < GAME_WIDTH)
                {
                    switch (t->board[row][col1])
                    {
                    case RED:
                        display_data_union.s.r1 = 1;
                        break;
                    case BLUE:
                        display_data_union.s.g1 = 1;
                        break;
                    case GREEN:
                        display_data_union.s.b1 = 1;
                        break;
                    default:
                        break;
                    }
                }
                if (0 <= col2 && col2 < GAME_WIDTH)
                {
                    switch (t->board[row][col2])
                    {
                    case RED:
                        display_data_union.s.r2 = 1;
                        break;
                    case BLUE:
                        display_data_union.s.g2 = 1;
                        break;
                    case GREEN:
                        display_data_union.s.b2 = 1;
                        break;
                    default:
                        break;
                    }
                }
            }

            // Send data to display.
            WRITE_DISPLAY_DATA;
            TOGGLE_DISPLAY_CLK;
        }
        TOGGLE_DISPLAY_LATCH;
        WRITE_DISPLAY_OE(0);
    }
}
