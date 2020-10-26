/*
 * debug.c
 *
 *  Created on: Oct 26, 2020
 *      Author: thomas
 */

#include "debug.h"
#include "msp.h"

void debug_uart_config()
{
#ifdef DEBUG
    //    // From https://embedded-work.com/msp432-uart-communication-with-example/
    //    EUSCI_A2->CTLW0 |= 1;     // put in reset mode for config
    //    EUSCI_A2->MCTLW = 0;      // disable oversampling
    //    EUSCI_A2->CTLW0 = 0x0081; // 1 stop bit, no parity, SMCLK, 8-bit data
    //    EUSCI_A2->BRW = 26;       // Set Baudrate 3,000,000 / 115200 = 26
    //    P3->SEL0 |= 0x0C;         // Config GPIO P3.3, P3.2 for UART
    //    P3->SEL1 &= ~0x0C;
    //    EUSCI_A2->CTLW0 &= ~1; //take UART out of reset mode

    UCA0CTLW0 &= ~UCSWRST;
    UCA0CTLW0 = UCMODE_3 & UCSSEL_3;
    P3MAP01 |= PM_UCA0RXD;
    P3MAP23 |= PM_UCA0TXD;
    UCA0CTLW0 |= UCSWRST;
#endif
}

void debug_print(char value[])
{
#ifdef DEBUG
    int i;
    for (i = 0; i < sizeof(value) - 1; i++)
    {
        // Wait for transmit buffer empty.
        while (!(EUSCI_A2->IFG & 0x02))
        {
            __no_operation();
        }
        // Send a single character.
        UCA0TXBUF = value[i];
    }

    // Sleep between characters.
    for (i = 0; i < 1000; i++)
    {
        __no_operation();
    }
#endif
}

void debug_println(char value[])
{
#ifdef DEBUG
    int i;
    for (i = 0; i < sizeof(value) - 1; i++)
    {
        // Wait for transmit buffer empty.
        while (!(EUSCI_A2->IFG & 0x02))
        {
            __no_operation();
        }
        // Send a single character.
        UCA0TXBUF = value[i];
    }

    // Send a newline.
    // Wait for transmit buffer empty.
    while (!(EUSCI_A2->IFG & 0x02))
    {
        __no_operation();
    }
    // Send a single character.
    UCA0TXBUF = '\n';

    // Sleep between characters.
    for (i = 0; i < 1000; i++)
    {
        __no_operation();
    }
#endif
}
