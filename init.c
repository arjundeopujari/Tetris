#include "init.h"
/*
 * init.c
 *
 *  Created on: Oct 13, 2020
 *      Author: Arjun
 */

void StopWatchdogTimer(){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
}

void ConfigureButtonsAsInput(){
    // Set buttons are inputs with pull up resistors
    P6DIR &= ~(BIT7 | BIT6 | BIT5 | BIT4 | BIT1 | BIT0);
//    P6REN |= (BIT7 | BIT6 | BIT5 | BIT4 | BIT1 | BIT0);
//    P6OUT |= (BIT7 | BIT6 | BIT5 | BIT4 | BIT1 | BIT0);
//    // Enable interrupts on P6.{0,1,4,5,6,7}
//    P6IE = (BIT7 | BIT6 | BIT5 | BIT4 | BIT1 | BIT0);
//    // All button interrupts should happen on falling edge of input
//    P6IES = (BIT7 | BIT6 | BIT5 | BIT4 | BIT1 | BIT0);
}

void EnablePinsAsOutput(){
    // Set Ports 1,2,4,5 as outputs
    P1DIR = (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
    P2DIR = (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
    P4DIR = (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
    P5DIR = (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
}

void DivideSMCLK(){
    //Divide Subsystem Master Clock by 2^7 (Clock used for timerA)
    CS->CTL1 |= CS_CTL1_DIVS__128;
}



