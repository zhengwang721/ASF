/**
* \file
*
* \brief UART Driver
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
#include "stddef.h"     // standard definition
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "uart.h"
#include "uart_hw.h"

static uint32_t calibrate_clk(void)
{
	uint32_t clock;
	uint32_t u32calib;
	uint32_t u32orig_calib;

	REG_WRITE_32(0x4000c000, 0x0);  // GYR
	REG_WRITE_32(0x4000B018, REG_READ_32(0x4000B018) | (1<<2)); // must have two steps since there is a sequence
	REG_WRITE_32(0x4000B018, REG_READ_32(0x4000B018) | (1<<3));


	//REG_WRITE_32(0x4000f404, REG_READ_32(0x4000f404) | (1<<8));
	REG_WRITE_32(0x4000c000, REG_READ_32(0x4000c000) | (1<<7));


	while((REG_READ_32(0x4000c000) & (1<<7))!=0);
	u32orig_calib = REG_READ_32(0x4000c010) & ~(1<<15);

	REG_WRITE_32(0x4000B018, REG_READ_32(0x4000B018) & (~(1<<3))); // must have two steps since there is a sequence
	REG_WRITE_32(0x4000B018, REG_READ_32(0x4000B018) & ~((1<<2)));

	//REG_WRITE_32(0x4000f404, REG_READ_32(0x4000f404) & ~(1<<8));


	/*UART Calibration*/
	REG_WRITE_32(0x4000c000, REG_READ_32(0x4000c000) | (1<<7));

	while((REG_READ_32(0x4000c000) & (1<<7))!=0);

	//REG_WRITE_32(0x4000f044, (REG_READ_32(0x4000c010)));

	REG_WRITE_32(0x4000c010, (REG_READ_32(0x4000c010) & ~(1<<15)));

	//	while((REG_READ_32(0x4000c010) & (1<<15))!=1);
	REG_WRITE_32(0x4000f404, REG_READ_32(0x4000f404) & ~(1<<8));		

	u32calib = REG_READ_32(0x4000c010) & ~(1<<15);
	clock = ((26000000/u32orig_calib)*u32calib);
	return clock;
}

void uart_get_config_defaults(
		struct uart_config *const config)
{
	config->baud_rate = 115200;
	config->data_bits = 8;
	config->flow_control = 0;
	config->parity = UART_NO_PARITY;
	config->pinmux_pad[0] = ((LPGPIO_6 << PINNUM_OFFSET) | (PINMUX_VAL_2));
	config->pinmux_pad[1] = ((LPGPIO_7 << PINNUM_OFFSET) | (PINMUX_VAL_2));
}

enum uart_status_code uart_init(enum uart_hw_module module, const struct uart_config *const config)
{
	volatile UART *uart;
	uint32_t clock;
	uint32_t baudrate;
	uint16_t integerpart = 0;
	uint8_t fractionalpart = 0;
	uint32_t diff;
	uint8_t i = 0;
	uint8_t pinnum = 0;
	uint8_t muxval = 0;
	uint8_t idx;
	uint32_t regaddr,regval;
	
	if(module == UART_HW_MODULE_UART1)
		uart = (UART *) UART0_BASE;
	else if(module == UART_HW_MODULE_UART2)
		uart = (UART *) UART1_BASE;

	//pinmux programming
	// Pin Mux programming
	for(idx=0;idx<2;idx++) {
		pinnum = (uint8_t)((config->pinmux_pad[idx] >> 8) & 0xFF);
		muxval = (uint8_t)(config->pinmux_pad[idx] & 0xFF);
		if(!port_list[pinnum].bit.available) {
			return UART_STATUS_ERR_GPIO_NOT_AVAILABLE;
		} 
		else {
			port_list[pinnum].bit.available = 0;
		}
		if( pinnum <= 7) {
			regaddr = 0x4000b044;
		}
		else if(pinnum <= 15) {
			regaddr = 0x4000b048;
		}
		else if(pinnum <= 23) {
			regaddr = 0x4000b04c;
		}
		regval = REG_RD(regaddr);
		regval &= ~(7 << ((pinnum%8)*4));
		regval |= (muxval << ((pinnum%8)*4));
		REG_WR(regaddr,regval);
	}
	
	/* empty UART FIFO */
	while (uart->rx_status & (0x01))	
	{
		i = uart->rx_data;
	}
	
	// reset configuration register
	uart->config = 0;

	baudrate = config->baud_rate;
	clock = calibrate_clk();
	integerpart = clock / baudrate;

	diff = clock - (baudrate * integerpart);

	i = 0;
	while(diff > (baudrate/16))
	{
		i++;
		diff -= (baudrate/16);
	}

	fractionalpart = (i+1)/2;

	/**
	 program the uart configuration
	**/
	if(config->flow_control)
	{
		uart->config |= (1<<5);		//flow control
	}
	
	if(config->stop_bits == 2)
	{
		uart->config |= (1<<4);		//number of stop bits
	}
	
	if(config->parity == UART_EVEN_PARITY)
	{
		//enable parity
		uart->config |= (1<<1);
	}
	else if(config->parity == UART_ODD_PARITY)
	{
		//enable parity
		uart->config |= (1<<1);
		// select parity type
		uart->config |= (1<<2);
		
	}
	else if(config->parity == UART_SPACE_PARITY)
	{
		//enable parity
		uart->config |= (1<<1);
		// select parity type
		uart->config |= (2<<2);
	}
	else if(config->parity == UART_MARK_PARITY)
	{
		//enable parity
		uart->config |= (1<<1);
		// select parity type
		uart->config |= (13<<2);
	}
	else
	{
		uart->config &= ~(1<<1);
	}
	
	if(config->data_bits == 7)
	{
		// select 7bit data width
		uart->config |= (1<<0);
	}
	
	uart->selclk = 0;
	uart->divider = ((uint32_t)integerpart << 3) | ((uint32_t)fractionalpart << 0);


	uart->rx_mask = 0;	// disable int at initialization, enable it at read time
	uart->tx_mask = 0;	// disable int at initialization, enable it at write time
	
	return UART_STATUS_OK;
}


enum uart_status_code uart_write_wait(enum uart_hw_module module, const uint8_t tx_data)
{
	volatile UART *uart;
	
	if(module == UART_HW_MODULE_UART1)
		uart = (UART *) UART0_BASE;
	else if(module == UART_HW_MODULE_UART2)
		uart = (UART *) UART1_BASE;
	
	while (!(uart->tx_status & 1));
	uart->tx_data = tx_data;
	
	return UART_STATUS_OK;
}

enum uart_status_code uart_read_wait(enum uart_hw_module module, uint8_t *const rx_data)
{
	volatile UART *uart;
	
	if(module == UART_HW_MODULE_UART1)
		uart = (UART *) UART0_BASE;
	else if(module == UART_HW_MODULE_UART2)
		uart = (UART *) UART1_BASE;
	
	while (!(uart->rx_status & 1));
	*rx_data = uart->rx_data;
	
	return UART_STATUS_OK;
}


enum uart_status_code uart_write_buffer_wait(enum uart_hw_module module, 
										const uint8_t *tx_data, uint32_t length)
{
	while(length--)
		uart_write_wait(module, *tx_data++);
	
	return UART_STATUS_OK;
}

enum uart_status_code uart_read_buffer_wait(enum uart_hw_module module, uint8_t *rx_data, uint16_t length)
{
	while(length--)
		uart_read_wait(module, rx_data++);
	
	return UART_STATUS_OK;
}

static char printf_buff[80];
void uart_printf(enum uart_hw_module module, char* format, ...)
{
	int size;
		
	__va_list args;
	va_start(args, format);

	size = vsprintf(printf_buff, format, args);
	uart_write_buffer_wait(module, (uint8_t *)printf_buff, size);
}


