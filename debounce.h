/*
 * debounce.h
 *
 *  Created on: Oct 19, 2020
 *      Author: thomas
 */

#pragma once

#include "types.h"

#define SMCLK_FREQ 187500                      // Frequency of SMCLK.
#define DEBOUNCE_DURATION_MS 20                // Milliseconds to wait until a button settles.
//#define DEBOUNCE_CYCLES 20 * SMCLK_FREQ / 1000 // Number of loops until a button has settled.
#define DEBOUNCE_CYCLES 30 // Number of loops until a button has settled.

typedef enum
{
    STABLE,
    TRANSITION,
} DebounceStates;

typedef struct debounceFSM
{
    DebounceStates state;
    unsigned char current;
    unsigned char prev;

    unsigned int current_cycles;
    bool is_handled;
} DebounceFSM;

/*************************
 * next_state
 * Take inputs to the FSM and perform state transitions on the FSM based on the inputs.
 * Inputs
 *      buttons - All buttons to debounce from one port.
 *      source_is_timer - Whether or not the input is a timer saying that the switch has settled
 *          or not.
 * Called in main.c
 *************************/
void debounce_tick();

/*************************
 * debounce_tick
 * Count the number of cycles between a button changing and accepting that the button has settled.
 * Called in main.c
 *************************/
void next_state(unsigned char buttons, bool source_is_timer);

static DebounceFSM fsm = {STABLE, 0x00, 0x00, 0, false};
