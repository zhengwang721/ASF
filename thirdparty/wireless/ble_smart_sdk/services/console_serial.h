
#ifndef COSOLE_SERIAL_H
#define COSOLE_SERIAL_H

#include <uart.h>

void serial_console_init();

int getchar_b11();

void getchar_aysnc(uart_callback_t callback_func, uint8_t * input);

//int printf_b11( const char* format, ... );

#endif /* COSOLE_SERIAL_H */