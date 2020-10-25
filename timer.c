#include "timer.h"
/*
 * timer.c
 *
 *  Created on: Oct 13, 2020
 *      Author: Arjun
 */

void ConfigureTimer(uint16_t period){
    // Select SMCLK as clock source for Timer
    TA0CTL |= TIMER_A_CTL_TASSEL_2;
    // Put value in TA0CCR register
    TA0CCR0 = period;
    // Enable interrupt on capture/compare
    TA0CCTL0 |= TIMER_A_CCTLN_CCIE;

}
void StartTimer(){
    // Set timer in UP mode
    TA0CTL |= TIMER_A_CTL_MC_1;
}
