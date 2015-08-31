/**
 * \file
 *
 * \brief UART HW 
 *
 * Copyright (C) 2012-2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef UART_HW_INCLUDED
#define UART_HW_INCLUDED

#include "gpio.h"
#include "common.h"

#define UART0_BASE 0x40004000
#define UART1_BASE 0x40005000

#ifdef CHIPVERSION_B0
	#define UART1_RX_VECTOR_TABLE_INDEX		16
	#define UART1_TX_VECTOR_TABLE_INDEX		17
	#define UART2_RX_VECTOR_TABLE_INDEX		18
	#define UART2_TX_VECTOR_TABLE_INDEX		19
#else
	#define UART1_RX_VECTOR_TABLE_INDEX		16
	#define UART1_TX_VECTOR_TABLE_INDEX		17
	#define UART2_RX_VECTOR_TABLE_INDEX		18
	#define UART2_TX_VECTOR_TABLE_INDEX		19
#endif	//CHIPVERSION_B0

#define REG_READ_32(addr)					(*(volatile unsigned int*)(addr))
#define REG_WRITE_32(addr,val)				(*(volatile unsigned int*)(addr) = val)


typedef struct _Uart
{
	/* Transmit data fifo */
	volatile unsigned long tx_data;

	/* Tranmsmit status
		5:    cts              1 means tx is clear to send
		4:    empty            1 if fifo is entirely empty
		3:    3/4 empty        1 if at least 3/4 of fifo is empty
		2:    1/2 empty        1 if at least 1/2 of fifo is empty
		1:    1/4 empty        1 if at least 1/4 of fifo is empty
		0:    !full            1 if fifo is not full */
	volatile unsigned long tx_status;

	/* Transmit interrupt mask */
	volatile unsigned long tx_mask;

	unsigned long __fill0x0c;

	/* Receive data fifo */
	volatile unsigned long rx_data;

	/* Receive status
		7:    framing error    1 if missing stop bit
		6:    overrun          1 if fifo overrun has occured
		:       -
		3:    3/4 full         1 if fifo at least 3/4 full
		2:    1/2 full         1 if fifo at least 1/2 full
		1:    1/4 full         1 if fifo at least 1/4 full
		0:    !empty           1 if some data is available */
	volatile unsigned long rx_status;

	/* Receive interrupt mask */
	volatile unsigned long rx_mask;

	/* Timeout value */
	volatile unsigned long rx_timeout;

	/*
	* The following registers are only present in the full version of the
	* uart implementation.
	*/

	/* UART configuration register (default 0)
		5:     cts_en          cts enable for TX
		4:     nstop   	0 => 1 stop bit, 1 => 2 stop bits
		[3:2]:     parity          parity mode
		1:     parity_en       enable parity
		0:     nbits           0 => 8 bit data, 1 => 7 bits data */
	volatile unsigned long config;

	/* parity mode = parity check mode
		0:   even
		1:   odd
		2:   space
		3:   mark */

	/* Clock divider, this is a 16 bits register with
	13 bits for the integer part, 3 bits for the fractional part.
	e.g. 32768Hz clock needs to be divided by 3 3/8 for 9600 (9709) bauds
	The register is 3 3/8 per default. i.e. 0x001b == 3*8+3 */
	volatile unsigned long divider;

	/* Clock source selection
			0: 3.375 Mhz,
			1: 6.75 Mhz,
			2: 13.5 Mhz,
			3: 26 Mhz.  
	*/
	volatile unsigned long selclk;			

} UART;



#endif

