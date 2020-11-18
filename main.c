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

#define GAME_CYCLES 500 * SMCLK_FREQ / 1000

#define UNUSED_PINS(p) p->DIR = 0xFF; p->REN = 0xFF

extern DebounceFSM fsm;

/**
 * tetris contains all of the game logic and game state for the Tetris game.
 */
static Tetris tetris;

static bool clock_interrupt_flag, button_interrupt_flag = false;

/**
 * main.c
 */
void main(void)
{

    /* System initializing code */
    StopWatchdogTimer();
    ConfigureButtonsAsInput();

//    CS->KEY = 0x0000695A;
//    CS->CTL0 &= ~CS_CTL0_DCORSEL0;
//    CS->CTL1 |= CS_CTL1_DIVM__128;

//    UNUSED_PINS(P1);
//    UNUSED_PINS(P2);
//    UNUSED_PINS(P3);
//    UNUSED_PINS(P4);
//    UNUSED_PINS(P5);
//    UNUSED_PINS(P6);
//    UNUSED_PINS(P7);
//    UNUSED_PINS(P8);
//    UNUSED_PINS(P9);
//    UNUSED_PINS(P10);

    EnablePinsAsOutput();
    DivideSMCLK();

    while (1) {
        display_address_union.b += 1;
        P4->OUT = display_address_union.b;
    }

    /* Initialize Timer */
    ConfigureTimer(10);

    /* Initialize game logic */
    tetris_init(&tetris);
    tetris_spawn_piece(&tetris);

    /* Initialize debug UART */
    debug_uart_config();

    /* Enable global interrupts */
    __enable_irq();
    StartTimer();

    debug_println("[MAIN LOOP] Starting main loop");

    static unsigned int game_clock = 0;
    while (true)
    {
        /*
         * HANDLE BUTTON INTERRUPTS
         */
        if (button_interrupt_flag)
        {
            button_interrupt_flag = false;
            debug_println("[MAIN LOOP] Handling button flag.");
            // Read all of Port 6.
            next_state(P6OUT, false);
        }

        /*
         * HANDLE CLOCK INTERRUPTS
         */
        if (clock_interrupt_flag)
        {
            clock_interrupt_flag = false;
            debug_println("[MAIN LOOP] Handling clock flag.");
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
                    // TODO: Update the display here?
                    tetris_visualize(&tetris);
                    tetris.changed = false;
                }
            }
        }
    }
}

/* Timer A0 ISR*/
void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    clock_interrupt_flag = true;
//    P1->OUT ^= BIT0;
//    TIMER_A0->CCR[0] += 50000;              // Add Offset to TACCR0
}
//void TA0_N_IRQHandler(void)
//{
//    debug_println("[TA0_0_IRQHandler] Handling clock interrupt.");
//    __disable_interrupts();
//    clock_interrupt_flag = true;
//    __enable_interrupts();
//}

/* PORT 6 (buttons) ISR  */
void PORT6_IRQHandler(void)
{
    debug_println("[PORT6_IRQHandler] Handling button interrupt.");
    __disable_interrupts();
    button_interrupt_flag = true;
    __enable_interrupts();
}
