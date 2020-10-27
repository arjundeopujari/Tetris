
#pragma once

#define DEBUG 1

void debug_uart_config();
void debug_print(char value[]);
void debug_println(char value[]);

#define SLEEP(i, n)         \
    for (i = 0; i < n; i++) \
    {                       \
        __nop();            \
    }
