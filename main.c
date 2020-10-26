/*
 * Professional MultiSwimmers
 * ECE 4440
 * 
 * main.c
 * 
 * Author: Arjun, Nick, Daniel, Patrick
 */

#include "debounce.h"
#include "init.h"
#include "msp.h"
#include "tetris.h"
#include "timer.h"
#define GAME_CYCLES 500 * SMCLK_FREQ / 1000

extern DebounceFSM fsm;

/**
 * tetris contains all of the game logic and game state for the Tetris game.
 */
static Tetris tetris;

/**
 * main.c
 */
void main(void)
{
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

    /* Enable global interrupts */
    __enable_irq();
    StartTimer();

    while (1)
    {
        __no_operation();
    }
}

/* Timer A0 ISR*/
void TA0_0_IRQHandler(void)
{
    static unsigned int game_clock = 0;
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
                ;
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
            tetris_visualize(&tetris);
            tetris.changed = false;
        }
    }
}

/* PORT 6 (buttons) ISR  */
void PORT6_IRQHandler(void)
{
    __disable_interrupts();

    // Read all of Port 6.
    next_state(P6OUT, false);

    __enable_interrupts();
}
