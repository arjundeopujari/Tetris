#include "msp.h"
#include "init.h"
#include "timer.h"
#include "test.h"


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

	/* Enable global interrupts */
	__enable_irq();
	StartTimer();

	while (1) {
	    TestLEDMatrix();
	}
}

/*Timer A0 ISR*/
void TA0_0_IRQHandler(void){

}

/*PORT 6 (buttons) ISR  */
void PORT6_IRQHandler(void){

}