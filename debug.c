/*
 * Professional MultiSwimmers
 * ECE 4440
 * 
 * debug.c
 * 
 * Author: Patrick Thomas
 */

#include "debug.h"
#include "msp.h"

void debug_uart_config()
{
#ifdef DEBUG
    // Set UART into reset mode to edit settings.
    UCA0CTLW0 = UCSWRST;
    /*
    Settings:
        Parity disabled
        Odd parity
        LSB first
        8-bit data
        One stop bit
        UART mode with automatic baud-rate detection
        Async mode
        Clock source: SMCLK
        Erroneous characters rejected and UCRXIFG is not set
        Received break characters do not set UCRXIFG
        Not dormant
        Next frame transmitted is data
        Next frame transmitted is not a break
    */
    UCA0CTLW0 |= UCMODE_3 | UCSSEL_2;
    // Take UART out of reset mode.
    UCA0CTLW0 &= ~UCSWRST;
#endif
}

void debug_print(char string[])
{
#ifdef DEBUG
    int i;
    for (i = 0; i < sizeof(string) - 1; i++)
    {
        // Wait for transmit buffer empty.
        while (!(UCA0STATW & UCBUSY))
        {
            __no_operation();
        }
        // Send a single character.
        UCA0TXBUF = string[i];

        // Sleep between characters.
        SLEEP(i, 1000);
    }
#endif
}

void debug_println(char string[])
{
#ifdef DEBUG
    int i;
    for (i = 0; i < sizeof(string) - 1; i++)
    {
        // Wait for transmit buffer empty.
        while (!(UCA0STATW & UCBUSY))
        {
            __no_operation();
        }
        // Send a single character.
        UCA0TXBUF = string[i];

        // Sleep between characters.
        SLEEP(i, 1000);
    }

    // Send a newline.
    // Wait for transmit buffer empty.
    while (!(UCA0STATW & UCBUSY))
    {
        __no_operation();
    }
    // Send a single character.
    UCA0TXBUF = '\n';

    // Sleep between characters.
    SLEEP(i, 1000);
#endif
}
