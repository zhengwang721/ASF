/**
 * \file
 *
 * \brief Uart Serial for SAM.
 *
 * Copyright (c) 2011 - 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
#ifndef _UART_SERIAL_H_
#define _UART_SERIAL_H_

#include "compiler.h"
#include "sysclk.h"
#include "uart.h"

/*! \name Serial Management Configuration
 */
//! @{
#include "conf_uart_serial.h"

//! @}

//! Input parameters when initializing RS232 and similar modes.
typedef struct uart_rs232_options {
	//! Set baud rate of the USART (unused in slave modes).
	uint32_t baudrate;

	//! Number of bits to transmit as a character (5 to 9).
	uint32_t charlength;

	//! Parity type: USART_PMODE_DISABLED_gc, USART_PMODE_EVEN_gc,
	//! USART_PMODE_ODD_gc.
	uint32_t paritytype;

	//! Number of stop bits between two characters:
	//! true: 2 stop bits
	//! false: 1 stop bit
	bool stopbits;

} usart_rs232_options_t;

typedef usart_rs232_options_t usart_serial_options_t;

typedef Uart *usart_if;

/*! \brief Initializes the Usart in master mode.
 *
 * \param usart       Base address of the USART instance.
 * \param opt         Options needed to set up RS232 communication (see
 *                    \ref usart_options_t).
 */
static inline void usart_serial_init(usart_if p_uart,
		usart_serial_options_t *opt)
{
	sam_uart_opt_t uart_settings;
	uart_settings.ul_mck = sysclk_get_peripheral_hz();
	uart_settings.ul_baudrate = opt->baudrate;
	uart_settings.ul_mode = opt->paritytype;

#ifdef UART
	if (UART == p_uart) {
		sysclk_enable_peripheral_clock(ID_UART);
	}
#else
# ifdef UART0
	if (UART0 == p_uart) {
		sysclk_enable_peripheral_clock(ID_UART0);
	}
# endif
# ifdef UART1
	if (UART1 == p_uart) {
		sysclk_enable_peripheral_clock(ID_UART1);
	}
# endif
#endif // ifdef UART

#ifdef USART
	if (USART == (Usart*)p_uart) {
		sysclk_enable_peripheral_clock(ID_USART);
	}
#else
# ifdef USART0
	if (USART0 == (Usart*)p_uart) {
		sysclk_enable_peripheral_clock(ID_USART0);
	}
# endif
# ifdef USART1
	if (USART1 == (Usart*)p_uart) {
		sysclk_enable_peripheral_clock(ID_USART1);
	}
# endif
# ifdef USART2
	if (USART2 == (Usart*)p_uart) {
		sysclk_enable_peripheral_clock(ID_USART2);
	}
# endif
# ifdef USART3
	if (USART3 == (Usart*)p_uart) {
		sysclk_enable_peripheral_clock(ID_USART3);
	}
# endif
# ifdef USART4
	if (USART4 == (Usart*)p_uart) {
		sysclk_enable_peripheral_clock(ID_USART4);
	}
# endif
#endif // ifdef USART


	/* Configure UART */
	uart_init(p_uart, &uart_settings);

}

/*! \brief Sends a character with the USART.
 *
 * \param usart   Base address of the USART instance.
 * \param c       Character to write.
 *
 * \return Status.
 *   \retval 1  The character was written.
 *   \retval 0  The function timed out before the USART transmitter became
 *              ready to send.
 */
static inline int usart_serial_putchar(usart_if p_uart, const uint8_t c)
{
	while (uart_write(p_uart, c)!=0);
	return 1;
}
/*! \brief Waits until a character is received, and returns it.
 *
 * \param usart   Base address of the USART instance.
 * \param data   Data to read
 *
 */
static inline void usart_serial_getchar(usart_if p_uart, uint8_t *data)
{
	while (uart_read(p_uart, data));
}

/**
 * \brief Send a sequence of bytes to a USART device
 *
 * \param usart Base address of the USART instance.
 * \param data   data buffer to write
 * \param len    Length of data
 *
 */
status_code_t usart_serial_write_packet(usart_if usart, const uint8_t *data,
		size_t len);

/**
 * \brief Receive a sequence of bytes to a USART device
 *
 * \param usart Base address of the USART instance.
 * \param data   data buffer to write
 * \param len    Length of data
 *
 */
status_code_t usart_serial_read_packet(usart_if usart, uint8_t *data,
		size_t len);

#endif  // _UART_SERIAL_H_
