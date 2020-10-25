/*
 * debounce.c
 *
 *  Created on: Oct 19, 2020
 *      Author: thomas
 */

#include "debounce.h"
#include "msp.h"
#include "types.h"

void debounce_tick() {
    // Only "tick" if the FSM is awaiting the input to settle.
    if (fsm.state == TRANSITION) {
        fsm.current_cycles++;
        if (fsm.current_cycles >= DEBOUNCE_CYCLES) {
            next_state(P6OUT, true);
        }
    }
}

void next_state(unsigned char buttons, bool source_is_timer)
{
    switch (fsm.state)
    {
    case STABLE:
        // Any input will cause the FSM to try to transition.
        fsm.state = TRANSITION;
        break;
    case TRANSITION:
        if (source_is_timer)
        {
            fsm.state = STABLE;
            fsm.prev = fsm.current;
            fsm.current = ~buttons; // Negate since buttons are active low.
            fsm.is_handled = false;
        }
        // Reset the debounce clock if either the clock times out or if the switch bounces here.
        fsm.current_cycles = 0;
        break;
    default:
        break;
    }
}
