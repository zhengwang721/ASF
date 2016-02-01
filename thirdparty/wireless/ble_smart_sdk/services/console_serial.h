
#ifndef COSOLE_SERIAL_H
#define COSOLE_SERIAL_H

#include <uart.h>

void serial_console_init(void);
int getchar_b11(void);
int getchar_b11_timeout(uint32_t sec);
void getchar_aysnc(uart_callback_t callback_func, uint8_t * input);

#endif /* COSOLE_SERIAL_H */