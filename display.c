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
    WRITE_DISPLAY_LATCH(1);
    WRITE_DISPLAY_OE(1);
    WRITE_DISPLAY_CLK(1);

    display_address_union.b = 0;
    display_data_union.b = 0;

    WRITE_DISPLAY_ADDR;
    WRITE_DISPLAY_DATA;
}

void display_translate(Tetris *t)
{
    int i, j, p_i, p_j, x, y;

    for (i = 0; i < GAME_WIDTH; i++)
    {
        for (j = 0; j < GAME_HEIGHT; j++)
        {
            x = i * 3 + 1;
            y = j * 3 + 2;

            display.buffer[31 - (x + 0)][y + 0] = t->board[j][i];
            display.buffer[31 - (x + 1)][y + 0] = t->board[j][i];
            display.buffer[31 - (x + 2)][y + 0] = t->board[j][i];
            display.buffer[31 - (x + 0)][y + 1] = t->board[j][i];
            display.buffer[31 - (x + 1)][y + 1] = t->board[j][i];
            display.buffer[31 - (x + 2)][y + 1] = t->board[j][i];
            display.buffer[31 - (x + 0)][y + 2] = t->board[j][i];
            display.buffer[31 - (x + 1)][y + 2] = t->board[j][i];
            display.buffer[31 - (x + 2)][y + 2] = t->board[j][i];
        }
    }

    // Clear the borders.
    for (i = 0; i < DISPLAY_HEIGHT; i++)
    {
        display.buffer[0][i] = BLUE;
        display.buffer[31][i] = BLUE;
    }
    int score_bit = 0;
    for (i = 0; i < DISPLAY_WIDTH; i++)
    {
        score_bit = 1 << (i / 2);

        if (t->score & score_bit)
        {
            display.buffer[i][0] = WHITE;
            display.buffer[i][1] = WHITE;
        }
        else
        {
            if (i % 2 == 1)
            {
                display.buffer[i][0] = PURPLE;
                display.buffer[i][1] = PURPLE;
            }
            else
            {
                display.buffer[i][0] = BLUE;
                display.buffer[i][1] = BLUE;
            }
        }
        display.buffer[i][63] = BLUE;
        display.buffer[i][62] = BLUE;
    }
}

void display_clear()
{
    static int i, j;
    for (i = 0; i < DISPLAY_WIDTH; i++)
    {
        for (j = 0; j < DISPLAY_HEIGHT; j++)
        {
            display.buffer[i][j] = EMPTY;
        }
    }
}

void display_write_buffer()
{
    // Set all outputs to 0.
    display_init();

    int i, i1, i2, j;
    BoardValue val1, val2;
    for (i = 0; i < DISPLAY_WIDTH / 2; i++)
    {
        i1 = i;
        i2 = i + 16;

        display_address_union.b = i;
        WRITE_DISPLAY_ADDR;

        for (j = 0; j < DISPLAY_HEIGHT; j++)
        {
            // Zero the display data.
            display_data_union.b = 0;

            val1 = display.buffer[i1][j];
            val2 = display.buffer[i2][j];

            switch (val1)
            {
            case RED:
                display_data_union.s.r1 = 1;
                break;
            case GREEN:
                display_data_union.s.g1 = 1;
                break;
            case BLUE:
                display_data_union.s.b1 = 1;
                break;
            case YELLOW:
                display_data_union.s.r1 = 1;
                display_data_union.s.g1 = 1;
                break;
            case TEAL:
                display_data_union.s.g1 = 1;
                display_data_union.s.b1 = 1;
                break;
            case PURPLE:
                display_data_union.s.r1 = 1;
                display_data_union.s.b1 = 1;
                break;
            case WHITE:
                display_data_union.s.r1 = 1;
                display_data_union.s.g1 = 1;
                display_data_union.s.b1 = 1;
                break;
            }

            switch (val2)
            {
            case RED:
                display_data_union.s.r2 = 1;
                break;
            case GREEN:
                display_data_union.s.g2 = 1;
                break;
            case BLUE:
                display_data_union.s.b2 = 1;
                break;
            case YELLOW:
                display_data_union.s.r2 = 1;
                display_data_union.s.g2 = 1;
                break;
            case TEAL:
                display_data_union.s.g2 = 1;
                display_data_union.s.b2 = 1;
                break;
            case PURPLE:
                display_data_union.s.r2 = 1;
                display_data_union.s.b2 = 1;
                break;
            case WHITE:
                display_data_union.s.r2 = 1;
                display_data_union.s.g2 = 1;
                display_data_union.s.b2 = 1;
                break;
            }

            WRITE_DISPLAY_DATA;
            TOGGLE_DISPLAY_CLK;
        }
        WRITE_DISPLAY_OE(0);
        TOGGLE_DISPLAY_LATCH;
        WRITE_DISPLAY_OE(1);
    }
}

void display_test_1()
{
    int x, y;
    x = 4;
    y = 4;

    while (1)
    {
        display_clear();
        display.buffer[x][y] = GREEN;
        display.buffer[x][y + 1] = BLUE;
        display.buffer[x + 1][y] = RED;
        display.buffer[x + 1][y + 1] = GREEN;
        display_write_buffer();

        x = (x + 1) % 31;
        y = (y + 1) % 63;
    }
}

void display_test_2()
{
    Tetris t;
    tetris_init(&t);
    t.board[0][0] = GREEN;
    t.board[0][1] = GREEN;
    t.board[1][2] = GREEN;
    t.board[0][2] = GREEN;

    while (1)
    {
        // display.buffer[x][y] = RED;
        display_clear();
        display_translate(&t);
        display_write_buffer();
    }
}

void display_test_3()
{
    display_clear();
    int i, x, y, color;
    for (i = 0; i < 60; i++)
    {
        x = rand() % 32;
        y = rand() % 64;
        color = rand() % 4;

        display.buffer[x][y] = color;
    }

    while (1)
    {
        display_write_buffer();
    }
}
