/**
 * \file
 *
 * \brief I2C Master Driver
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

#ifndef I2C_HW_INCLUDED
#define I2C_HW_INCLUDED

#include <stdint.h>
//#include "lpmcu_cortex_misc_regs_c.h"
#include "gpio.h"

#define I2C1_BASE 0x40003000
#define I2C2_BASE 0x40003400

#ifdef CHIPVERSION_B0
	#define I2C1_RX_VECTOR_TABLE_INDEX		24
	#define I2C1_TX_VECTOR_TABLE_INDEX		25
	#define I2C2_RX_VECTOR_TABLE_INDEX		26
	#define I2C2_TX_VECTOR_TABLE_INDEX		27
#else
	#define I2C1_RX_VECTOR_TABLE_INDEX		29
	#define I2C1_TX_VECTOR_TABLE_INDEX		30
#endif	//CHIPVERSION_B0

#define I2C_DISABLE	0
#define I2C_ENABLE	1

#define I2C_SLAVE	0
#define I2C_MASTER	1

#define I2C_N_ON_BUS	0
#define I2C_ON_BUS	1

#define I2C_IDLE	0
#define I2C_ACTIVE	1

#define ADDRESS_FLAG_DATA	0
#define ADDRESS_FLAG_ADDRESS	1

#define I2C_WRITE_TO_SLAVE	0
#define I2C_READ_FROM_SLAVE	1

typedef union {
	struct {
		uint32_t direction:1;
		uint32_t address:7;
		uint32_t address_flag:1;
		uint32_t pack:23;
	}bit;
	uint32_t reg;
}I2C_TX_DATA_Type;

#define FIFO_NOT_FULL	0
#define FIFO_1_4_EMPTY	1
#define	FIFO_1_2_EMPTY	2
#define	FIFO_3_4_EMPTY	3
#define	FIFO_EMPTY	4

typedef union  {
	struct {
		uint32_t fifo_not_full:1;
		uint32_t fifo_1_4_empty:1;
		uint32_t fifo_1_2_empty:1;
		uint32_t fifo_3_4_empty:1;
		uint32_t fifo_empty:1;
		uint32_t pack:27;
	}bit;
	uint32_t reg;
}I2C_TX_STATUS_Type;

#define FIFO_NOT_EMPTY	0
#define FIFO_1_4_FULL	1
#define	FIFO_1_2_FULL	2
#define	FIFO_3_4_FULL	3
#define	FIFO_OVERRUN	4
#define	FIFO_NAK			5

typedef union {
	struct {
		uint32_t fifo_not_empty:1;
		uint32_t fifo_1_4_full:1;
		uint32_t fifo_1_2_full:1;
		uint32_t fifo_3_4_full:1;
		uint32_t fifo_overrun:1;
		uint32_t nak:1;
		uint32_t pack:26;
	}bit;
	uint32_t reg;
}I2C_RX_STATUS_Type;

typedef union {
	struct {
		uint32_t	CLOCK_SELECT:2;
		uint32_t	:30;
	}bit;
	uint32_t reg;
}CLK_SELECT_Type;

typedef union {
	struct {
		uint32_t	CLK_DIVIDER:16;
		uint32_t	:16;
	}bit;
	uint32_t reg;
}CLK_RATE_DIVIDER_Type;


typedef union {
	struct {
		uint32_t fifo_not_full:1;
		uint32_t fifo_1_4_empty:1;
		uint32_t fifo_1_2_empty:1;
		uint32_t fifo_3_4_empty:1;
		uint32_t fifo_empty:1;
		uint32_t pack:27;
	}bit;
	uint32_t reg;
}I2C_TX_MASK_Type;

typedef union {
	struct {
		uint32_t fifo_not_empty:1;
		uint32_t fifo_1_4_full:1;
		uint32_t fifo_1_2_full:1;
		uint32_t fifo_3_4_full:1;
		uint32_t fifo_overrun:1;
		uint32_t NAK:1;
		uint32_t pack:26;
	}bfields;
	uint32_t reg;
}I2C_RX_MASK_Type;

typedef struct {
	volatile I2C_TX_DATA_Type 		TXDATA;								/* offset: 0x0 */
	volatile uint32_t 						RXDATA;								/* offset: 0x4 */
	volatile I2C_TX_STATUS_Type 	TXSTATUS;							/* offset: 0x8 */
	volatile I2C_RX_STATUS_Type 	RXSTATUS;							/* offset: 0xc */
	volatile CLK_SELECT_Type 			CLKSEL;								/* offset: 0x10 */
	volatile uint32_t 						ENABLE;								/* offset: 0x14 */
	volatile CLK_RATE_DIVIDER_Type CLKDIVIDER;					/* offset: 0x18 */
	volatile uint32_t 						MASTER;								/* offset: 0x1c */
	volatile uint32_t 						ONBUS;								/* offset: 0x20 */
	volatile uint32_t 						SLVADDR;							/* offset: 0x24 */
	volatile uint32_t 						ACTIVITY;							/* offset: 0x28 */
	volatile I2C_TX_MASK_Type 		TXMASK;								/* offset: 0x2c */
	volatile I2C_RX_MASK_Type 		RXMASK;								/* offset: 0x30 */
	volatile uint32_t 						FLUSH;								/* offset: 0x34 */
}I2C;

#endif	//I2C_HW_INCLUDED

