/*
 * Professional MultiSwimmers
 * ECE 4440
 * 
 * debug.h
 * 
 * Author: Patrick Thomas
 */

#pragma once

#define DEBUG 0

/*************************
 * debug_uart_config
 * Configures UART over the XDS110 debug probe. Requires DEBUG to be set, otherwise this function does nothing.
 * Called in main.c
 *************************/
void debug_uart_config();

/*************************
 * debug_print
 * Print a string over UART to the serial monitor, newline not included. Requires DEBUG to be set, otherwise this function does nothing.
 *      value - C string to print.
 * Called in main.c
 *************************/
void debug_print(char value[]);

/*************************
 * debug_println
 * Print a string over UART to the serial monitor, newline included. Requires DEBUG to be set, otherwise this function does nothing.
 * Inputs
 *      value - C string to print.
 * Called in main.c
 *************************/
void debug_println(char value[]);

#define SLEEP(i, n)         \
    for (i = 0; i < n; i++) \
    {                       \
        __nop();            \
    }
