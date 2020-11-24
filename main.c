/*
 * Professional MultiSwimmers
 * ECE 4440
 * 
 * main.c
 * 
 * Author: Arjun, Nick, Daniel, Patrick
 */

#include "debounce.h"
#include "debug.h"
#include "display.h"
#include "init.h"
#include "msp.h"
#include "tetris.h"
#include "timer.h"

// Test defines.
// #define DISPLAY_ADDRESS_OUTPUT_TEST
// #define DISPLAY_TEST1
// #define DISPLAY_TEST2
// #define DISPLAY_TEST3
// #define TEST_POINTS

#define GAME_CYCLES 500 * SMCLK_FREQ / 1000

extern DebounceFSM fsm;

static bool clock_interrupt_flag = false;

/**
 * main.c
 */
void main(void)
{
    /**
     * tetris contains all of the game logic and game state for the Tetris game.
     */
    static Tetris tetris;

    static unsigned int game_clock = 0;
    static unsigned char old_buttons = 0xff;
    static unsigned char is_paused = false;

    /* System initializing code */
    StopWatchdogTimer();
    ConfigureButtonsAsInput();

    EnablePinsAsOutput();
    DivideSMCLK();

    /* Initialize Timer */
    ConfigureTimer(0xffff);

    // /* Initialize debug UART */
    // debug_uart_config();

    /* Enable global interrupts */
    __enable_irq();
    StartTimer();

    // Initialize the display outputs to their starting states.
    display_init();

    /* Initialize game logic */
    tetris_init(&tetris);
    tetris_spawn_piece(&tetris);

#ifdef DISPLAY_ADDRESS_OUTPUT_TEST
    while (1)
    {
        // Increment the display address byte, which should appear like a counter on the output pins for the display address:
        // 4.0 A
        // 4.1 B
        // 4.2 C
        // 4.3 D
        display_address_union.b += 1;
        P4->OUT = display_address_union.b;
    }
#endif

#ifdef DISPLAY_TEST1
    display_test_1();
#endif

#ifdef DISPLAY_TEST2
    display_test_2();
#endif

#ifdef DISPLAY_TEST3
    display_test_3();
#endif

#ifdef TEST_POINTS
    while (1)
    {
        int rotate = P6->IN & ROTATE_MASK;

        display_data_union.s.r1 = 0;
        display_data_union.s.r2 = 0;
        WRITE_DISPLAY_DATA;
        display_data_union.s.r1 = 1;
        display_data_union.s.r2 = 0;
        WRITE_DISPLAY_DATA;
        display_data_union.s.r1 = 0;
        display_data_union.s.r2 = 1;
        WRITE_DISPLAY_DATA;
        display_data_union.s.r1 = 1;
        display_data_union.s.r2 = 1;
        WRITE_DISPLAY_DATA;

        WRITE_DISPLAY_CLK(0);
        WRITE_DISPLAY_CLK(1);
    }
#endif

    while (true)
    {
        tetris_visualize(&tetris);
        /*
         * HANDLE BUTTON INPUTS
         */

        /*
         * HANDLE CLOCK INTERRUPTS
         */
        if (clock_interrupt_flag)
        {
            clock_interrupt_flag = false;
            game_clock++;

            // Runs with frequency 48000000 / 2 / 128 Hz = 187500 Hz ~= 5.3E-6 sec
            // Alternatively: about 187.5 loops per millisecond.\

            // Only do things if the buttons are new.
            unsigned char buttons = (old_buttons ^ P6->IN) & P6->IN;
            old_buttons = P6->IN;
            if (buttons)
            {
                // Mark buttons as handled already.
                fsm.is_handled = true;

                // Mask off only the buttons that have changed.
                // unsigned char buttons = (fsm.current ^ fsm.prev) & fsm.current;
                if (buttons & PAUSE_MASK)
                {
                    is_paused = !is_paused;
                }
                else if (buttons & NEWGAME_MASK)
                {
                    // Completely reinitialize the game.
                    tetris_init(&tetris);
                    tetris_spawn_piece(&tetris);
                }
                else if (!is_paused && !tetris.end_game)
                {
                    if (buttons & ROTATE_MASK)
                    {
                        piece_rotate(tetris_queue_get(&tetris), &tetris);
                    }
                    else if (buttons & DOWN_MASK)
                    {
                        while (tetris_shift_down(&tetris))
                        {
                            // Do nothing.
                        }
                    }
                    else if (buttons & LEFT_MASK)
                    {
                        tetris_move_left(&tetris);
                    }
                    else if (buttons & RIGHT_MASK)
                    {
                        tetris_move_right(&tetris);
                    }
                }
            }

            // Update the display if necessary.
            if (!tetris.end_game)
            {
                if (game_clock >= 10)
                {
                    game_clock = 0;
                    if (!is_paused)
                    {
                        tetris_shift_down(&tetris);
                    }
                }
            }
        }
    }
}

/* Timer A0 ISR*/
void TA0_0_IRQHandler(void)
{
    // No need to display interrupts temporarily; the only interrupt is a timer one.
    // Remove the interrupt flag on the IF.
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    // Set another boolean flag to be handled in the main loop.
    clock_interrupt_flag = true;
}
