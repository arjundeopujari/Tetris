#include "timer.h"
/*
 * timer.c
 *
 *  Created on: Oct 13, 2020
 *      Author: Arjun
 */

void ConfigureTimer(uint16_t period)
{
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    TIMER_A0->CCR[0] = period;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
                    TIMER_A_CTL_MC__UP;

    NVIC->ISER[0] = 1 << ((TA0_0_IRQn)&31);
}

void StartTimer()
{
    // Set timer in UP mode
    TA0CTL |= TIMER_A_CTL_MC_1;
}

void ChangeMCLKSpeed()
{
    // Unlock CS registers.
    CS->KEY = 0x0000695A;
    // Set source.
    CS->CTL0 &= ~CS_CTL0_DCORSEL0;
    // Change the divider.
    CS->CTL1 |= CS_CTL1_DIVM__128;
}
