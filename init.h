#include "msp.h"
/*
 * init.h
 *
 *  Created on: Oct 13, 2020
 *      Author: Arjun
 */

#ifndef INIT_H_
#define INIT_H_

// Macros to turn various outputs on or off
#define TurnOn(port,pin)  port |= pin
#define TurnOff(port,pin) port &= ~pin

#define R1    TurnOn(P5->OUT,BIT0)
#define B1    TurnOn(P5->OUT,BIT1)
#define G1    TurnOn(P5->OUT,BIT2)
#define R2    TurnOn(P5->OUT,BIT4)
#define B2    TurnOn(P5->OUT,BIT5)
#define G2    TurnOn(P5->OUT,BIT7)

#define A     TurnOn(P4->OUT,BIT0)
#define B     TurnOn(P4->OUT,BIT1)
#define C     TurnOn(P4->OUT,BIT2)
#define D     TurnOn(P4->OUT,BIT3)

#define OE     TurnOn(P5->OUT,BIT6)
#define CLK    TurnOn(P2->OUT,BIT7)
#define Latch  TurnOn(P1->OUT,BIT6)

#define NotR1    TurnOff(P5->OUT,BIT0)
#define NotB1    TurnOff(P5->OUT,BIT1)
#define NotG1    TurnOff(P5->OUT,BIT2)
#define NotR2    TurnOff(P5->OUT,BIT4)
#define NotB2    TurnOff(P5->OUT,BIT5)
#define NotG2    TurnOff(P5->OUT,BIT7)

#define NotA     TurnOff(P4->OUT,BIT0)
#define NotB     TurnOff(P4->OUT,BIT1)
#define NotC     TurnOff(P4->OUT,BIT2)
#define NotD     TurnOff(P4->OUT,BIT3)

#define NotOE     TurnOff(P5->OUT,BIT6)
#define NotCLK    TurnOff(P2->OUT,BIT7)
#define NotLatch  TurnOff(P1->OUT,BIT6)

// Prototypes
void StopWatchdogTimer();
void ConfigureButtonsAsInput();
void EnablePinsAsOutput();
void DivideSMCLK();


#endif /* INIT_H_ */
