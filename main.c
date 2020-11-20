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
 #define DISPLAY_TEST3

#define GAME_CYCLES 500 * SMCLK_FREQ / 1000

extern DebounceFSM fsm;

static bool clock_interrupt_flag = false;
// static bool button_interrupt_flag = false;

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
    static unsigned char old_buttons = 0x00;

    /* System initializing code */
    StopWatchdogTimer();
    ConfigureButtonsAsInput();

    EnablePinsAsOutput();
    DivideSMCLK();

    /* Initialize Timer */
    ConfigureTimer(0xffff);

    /* Initialize game logic */
    tetris_init(&tetris);
    tetris_spawn_piece(&tetris);

    /* Initialize debug UART */
    debug_uart_config();

    /* Enable global interrupts */
    __enable_irq();
    StartTimer();

    // Initialize the display outputs to their starting states.
    display_init();

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
    int i, j;

    while (1)
    {
        // Randomize the colors of the board.
        for (i = 0; i < GAME_HEIGHT; i++)
        {
            for (j = 0; j < GAME_WIDTH; j++)
            {
                // Make each piece of the board a random color.
                int r = (rand() % 3) + 1;
                tetris.board[i][j] = r;
            }
        }

        // Now write it to the board.
        display_write(&tetris);
    }
#endif

#ifdef DISPLAY_TEST2
    BoardValue board[64][32];

    while (1)
    {
//        int k;
//        for (k = 0; k < 100; k++) {
//        // Randomize the colors of the board.
//        for (i = 0; i < GAME_WIDTH; i++)
//        {
//            last_i = i;
//            for (j = 0; j < GAME_HEIGHT; j++)
//            {
//                tetris.board[j][last_i] = EMPTY;
//                tetris.board[j][i] = RED;
//            }
//
//            // Now write it to the board.
//            display_write(&tetris);
//        }
//        }
        int i, j, k, l;
        k = 0;
        for (i=0; i<64; i++) {
            for (j=0; j<32; j++) {
                if (k==j) {
                    board[i][j] = GREEN;
                } else {
                    board[i][j] = EMPTY;
                }
            }
        }

        k = (k+1) % 32;

        for (l=0; l<100; l++) {
            display_write_array(board);
        }
    }
#endif

#ifdef DISPLAY_TEST3
    while (1)
    {
        /*
         * HANDLE BUTTON INPUTS
         */
        if (old_buttons ^ P6->IN)
        {
            old_buttons = P6->IN;
            next_state(P6->IN, false);
        }
        debounce_tick();


        /*
         * HANDLE CLOCK INTERRUPTS
         */
        if (clock_interrupt_flag)
        {
            clock_interrupt_flag = false;
            game_clock++;
            // Runs with frequency 48000000 / 2 / 128 Hz = 187500 Hz ~= 5.3E-6 sec
            // Alternatively: about 187.5 loops per millisecond.

            // Tick the button FSM.

            // Only do things if the buttons are new.
//            if (!fsm.is_handled)
//            {
//                // Mark buttons as handled already.
//                fsm.is_handled = true;
//
//                // Mask off only the buttons that have changed.
            unsigned char buttons = ~P6->IN;

            if (buttons & ROTATE_MASK)
            {
                int i;
                for (i = 0; i < GAME_HEIGHT; i++) {
                    tetris.board[i][0] = RED;
                    tetris.board[i][1] = EMPTY;
                    tetris.board[i][2] = EMPTY;
                    tetris.board[i][3] = EMPTY;
                    tetris.board[i][4] = EMPTY;
                    tetris.board[i][5] = EMPTY;
                }
            }
            else if (buttons & DOWN_MASK)
            {
                int i;
                for (i = 0; i < GAME_HEIGHT; i++) {
                    tetris.board[i][0] = EMPTY;
                    tetris.board[i][1] = GREEN;
                    tetris.board[i][2] = EMPTY;
                    tetris.board[i][3] = EMPTY;
                    tetris.board[i][4] = EMPTY;
                    tetris.board[i][5] = EMPTY;
                }
            }
            else if (buttons & LEFT_MASK)
            {
                int i;
                for (i = 0; i < GAME_HEIGHT; i++) {
                    tetris.board[i][0] = EMPTY;
                    tetris.board[i][1] = EMPTY;
                    tetris.board[i][2] = BLUE;
                    tetris.board[i][3] = EMPTY;
                    tetris.board[i][4] = EMPTY;
                    tetris.board[i][5] = EMPTY;
                }
            }
            else if (buttons & RIGHT_MASK)
            {
                int i;
                for (i = 0; i < GAME_HEIGHT; i++) {
                    tetris.board[i][0] = EMPTY;
                    tetris.board[i][1] = EMPTY;
                    tetris.board[i][2] = EMPTY;
                    tetris.board[i][3] = RED;
                    tetris.board[i][4] = EMPTY;
                    tetris.board[i][5] = EMPTY;
                }
            }
            else if (buttons & NEWGAME_MASK)
            {
                int i;
                for (i = 0; i < GAME_HEIGHT; i++) {
                    tetris.board[i][0] = EMPTY;
                    tetris.board[i][1] = EMPTY;
                    tetris.board[i][2] = EMPTY;
                    tetris.board[i][3] = EMPTY;
                    tetris.board[i][4] = GREEN;
                    tetris.board[i][5] = EMPTY;
                }
            }
            else if (buttons & PAUSE_MASK)
            {
                int i;
                for (i = 0; i < GAME_HEIGHT; i++) {
                    tetris.board[i][0] = EMPTY;
                    tetris.board[i][1] = EMPTY;
                    tetris.board[i][2] = EMPTY;
                    tetris.board[i][3] = EMPTY;
                    tetris.board[i][4] = EMPTY;
                    tetris.board[i][5] = BLUE;
                }
            }
        }

            // Update the display if necessary.
//            if (!tetris.end_game)
//            {
//                if (game_clock >= GAME_CYCLES)
//                {
//                    game_clock = 0;
//                    tetris_shift_down(&tetris);
//                }
//                if (tetris.changed)
//                {
//                    // TODO: Update the display here?
//                    tetris_visualize(&tetris);
//                    tetris.changed = false;
//                }
            // }
        // }

        display_write(&tetris);
    }
#endif

    while (true)
    {
        /*
         * HANDLE BUTTON INPUTS
         */
        if (old_buttons ^ P6->OUT)
        {
            old_buttons = P6->OUT;
            next_state(P6->OUT, false);
        }

        /*
         * HANDLE CLOCK INTERRUPTS
         */
        if (clock_interrupt_flag)
        {
            clock_interrupt_flag = false;
            game_clock++;
            // Runs with frequency 48000000 / 2 / 128 Hz = 187500 Hz ~= 5.3E-6 sec
            // Alternatively: about 187.5 loops per millisecond.

            // Tick the button FSM.
            debounce_tick();

            // Only do things if the buttons are new.
            if (!fsm.is_handled)
            {
                // Mark buttons as handled already.
                fsm.is_handled = true;

                // Mask off only the buttons that have changed.
                unsigned char buttons = (fsm.current ^ fsm.prev) & fsm.current;

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
                else if (buttons & NEWGAME_MASK)
                {
                    // Completely reinitialize the game.
                    tetris_init(&tetris);
                    tetris_spawn_piece(&tetris);
                }
            }

            // Update the display if necessary.
            if (!tetris.end_game)
            {
                if (game_clock >= GAME_CYCLES)
                {
                    game_clock = 0;
                    tetris_shift_down(&tetris);
                }
                if (tetris.changed)
                {
                    // TODO: Update the display here?
                    tetris_visualize(&tetris);
                    tetris.changed = false;
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

// /* PORT 6 (buttons) ISR  */
// void PORT6_IRQHandler(void)
// {
//     debug_println("[PORT6_IRQHandler] Handling button interrupt.");
//     __disable_interrupts();
//     button_interrupt_flag = true;
//     __enable_interrupts();
// }
