#include "timer.h"
/*
 * timer.c
 *
 *  Created on: Oct 13, 2020
 *      Author: Arjun
 */

void ConfigureTimer(uint16_t period){
//    // Select SMCLK as clock source for Timer
////    TA0CTL |= TACLR;
//    TA0CTL |= TIMER_A_CTL_TASSEL_2;
//    // Put value in TA0CCR register
//    TA0CCR0 = period;
//    // Enable interrupt on capture/compare
//    TA0CTL |= TIMER_A_CTL_IE;

    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    TIMER_A0->CCR[0] = period;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
            TIMER_A_CTL_MC__UP;

    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
}
void StartTimer(){
    // Set timer in UP mode
    TA0CTL |= TIMER_A_CTL_MC_1;
}
