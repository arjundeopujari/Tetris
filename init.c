#include "init.h"
/*
 * init.c
 *
 *  Created on: Oct 13, 2020
 *      Author: Arjun
 */

void StopWatchdogTimer()
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
}

void ConfigureButtonsAsInput()
{
    // Set P6 as input.
    P6->DIR &= ~(BIT7 | BIT6 | BIT5 | BIT4 | BIT1 | BIT0);
}

void EnablePinsAsOutput()
{
    // Set Ports 1,2,4,5 as outputs
    P1->DIR = (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
    P2->DIR = (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
    P4->DIR = (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
    P5->DIR = (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
}

void DivideSMCLK()
{
    //Divide Subsystem Master Clock by 2^7 (Clock used for timerA)
    CS->CTL1 |= CS_CTL1_DIVS__128;
}
