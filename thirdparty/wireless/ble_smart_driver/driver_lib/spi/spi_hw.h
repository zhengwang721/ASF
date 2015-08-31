/**
 * \file
 *
 * \brief SPI HW 
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

#ifndef SPI_HW_INCLUDED
#define SPI_HW_INCLUDED

#include <stdint.h>
//#include "lpmcu_cortex_misc_regs_c.h"
#include "gpio.h"

#define SPI1_BASE 0x40006000
#define SPI2_BASE 0x40007000

#ifdef CHIPVERSION_B0
	#define SPI1_RX_VECTOR_TABLE_INDEX		20
	#define SPI1_TX_VECTOR_TABLE_INDEX		21
	#define SPI2_RX_VECTOR_TABLE_INDEX		22
	#define SPI2_TX_VECTOR_TABLE_INDEX		23
#else
	#define SPI1_RX_VECTOR_TABLE_INDEX		20
	#define SPI1_TX_VECTOR_TABLE_INDEX		21
	#define SPI2_RX_VECTOR_TABLE_INDEX		27
	#define SPI2_TX_VECTOR_TABLE_INDEX		28
#endif	//CHIPVERSION_B0

#define CONF_SPI_MASTER_ENABLE	(1)
#define CONF_SPI_SLAVE_ENABLE	(1)

typedef union {
	struct {
		uint32_t	DATA:8;
		uint32_t	:24;
	}bit;
	uint32_t reg;
}SPI_DATA_Type;

typedef union  {
	struct {
		uint32_t TX_FIFO_NOT_FULL:1;
		uint32_t TX_FIFO_1_4_EMPTY:1;
		uint32_t TX_FIFO_1_2_EMPTY:1;
		uint32_t TX_FIFO_3_4_EMPTY:1;
		uint32_t TX_FIFO_EMPTY:1;
		uint32_t :27;
	}bit;
	uint32_t reg;
}TX_STATUS_Type;

typedef union {
	struct {
		uint32_t RX_FIFO_NOT_EMPTY:1;
		uint32_t RX_FIFO_1_4_FULL:1;
		uint32_t RX_FIFO_1_2_FULL:1;
		uint32_t RX_FIFO_3_4_FULL:1;
		uint32_t FAULT:1;
		uint32_t RX_FIFO_OVERRUN:1;
		uint32_t :26;
	}bit;
	uint32_t reg;
}RX_STATUS_Type;

typedef union {
	struct {
		uint32_t SCK_POLARITY:1;
		uint32_t SCK_PHASE:1;
		uint32_t LSB_FIRST:1;
		uint32_t BIDIRECTIONAL:1;
		uint32_t DIRECTION:1;
		uint32_t :27;
	}bit;
	uint32_t reg;
}SPI_CONFIG_Type;

typedef union  {
	struct {
		uint32_t TX_FIFO_NOT_FULL:1;
		uint32_t TX_FIFO_1_4_EMPTY:1;
		uint32_t TX_FIFO_1_2_EMPTY:1;
		uint32_t TX_FIFO_3_4_EMPTY:1;
		uint32_t TX_FIFO_EMPTY:1;
		uint32_t :27;
	}bit;
	uint32_t reg;
}TX_MASK_Type;
#define SPI_INTFLAG_TX_FIFO_NOT_FULL_Pos	0
#define SPI_INTFLAG_TX_FIFO_NOT_FULL	(0x1 << SPI_INTFLAG_TX_FIFO_NOT_FULL_Pos)
#define SPI_INTFLAG_TX_FIFO_1_4_EMPTY_Pos	1
#define SPI_INTFLAG_TX_FIFO_1_4_EMPTY	(0x1 << SPI_INTFLAG_TX_FIFO_1_4_EMPTY_Pos)
#define SPI_INTFLAG_TX_FIFO_1_2_EMPTY_Pos	2
#define SPI_INTFLAG_TX_FIFO_1_2_EMPTY	(0x1 << SPI_INTFLAG_TX_FIFO_1_2_EMPTY_Pos)
#define SPI_INTFLAG_TX_FIFO_3_4_EMPTY_Pos	3
#define SPI_INTFLAG_TX_FIFO_3_4_EMPTY	(0x1 << SPI_INTFLAG_TX_FIFO_3_4_EMPTY_Pos)
#define SPI_INTFLAG_TX_FIFO_EMPTY_Pos	4
#define SPI_INTFLAG_TX_FIFO_EMPTY	(0x1 << SPI_INTFLAG_TX_FIFO_EMPTY_Pos)

typedef union  {
	struct {
		uint32_t RX_FIFO_NOT_EMPTY:1;
		uint32_t RX_FIFO_1_4_FULL:1;
		uint32_t RX_FIFO_1_2_FULL:1;
		uint32_t RX_FIFO_3_4_FULL:1;
		uint32_t FAULT:1;
		uint32_t RX_FIFO_OVERRUN:1;
		uint32_t :26;
	}bit;
	uint32_t reg;
}RX_MASK_Type;

#define SPI_INTFLAG_RX_FIFO_NOT_EMPTY_Pos	0
#define SPI_INTFLAG_RX_FIFO_NOT_EMPTY (0x1 << SPI_INTFLAG_RX_FIFO_NOT_EMPTY_Pos)
#define SPI_INTFLAG_RX_FIFO_1_4_FULL_Pos	1
#define SPI_INTFLAG_RX_FIFO_1_4_FULL (0x1 << SPI_INTFLAG_RX_FIFO_1_4_FULL_Pos)
#define SPI_INTFLAG_RX_FIFO_1_2_FULL_Pos	2
#define SPI_INTFLAG_RX_FIFO_1_2_FULL (0x1 << SPI_INTFLAG_RX_FIFO_1_2_FULL_Pos)
#define SPI_INTFLAG_RX_FIFO_3_4_FULL_Pos	3
#define SPI_INTFLAG_RX_FIFO_3_4_FULL (0x1 << SPI_INTFLAG_RX_FIFO_3_4_FULL_Pos)
#define SPI_INTFLAG_RX_FIFO_FAULT_Pos	4
#define SPI_INTFLAG_RX_FIFO_FAULT (0x1 << SPI_INTFLAG_RX_FIFO_FAULT_Pos)
#define SPI_INTFLAG_RX_FIFO_OVERRUN_Pos	5
#define SPI_INTFLAG_RX_FIFO_OVERRUN (0x1 << SPI_INTFLAG_RX_FIFO_OVERRUN_Pos)

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
		uint32_t	SPI_ENABLE:1;
		uint32_t	:31;
	}bit;
	uint32_t reg;
}SPI_ENABLE_Type;

typedef union {
	struct {
		uint32_t	SPI_MASTER:1;
		uint32_t	:31;
	}bit;
	uint32_t reg;
}SPI_MODE_Type;

typedef union {
	struct {
		uint32_t	MODE_FAULT:1;
		uint32_t	:31;
	}bit;
	uint32_t reg;
}SPI_MODE_FAULT_Type;

typedef union {
	struct {
		uint32_t	SPI_ACTIVITY:1;
		uint32_t	:31;
	}bit;
	uint32_t reg;
}SPI_ACTIVE_Type;

typedef struct{
	volatile SPI_DATA_Type TXDATA;																		/* Offset : 0*/
	volatile SPI_DATA_Type RXDATA;																		/* Offset : 4*/
	volatile TX_STATUS_Type TXSTATUS;																	/* Offset : 8*/
	volatile RX_STATUS_Type RXSTATUS;																	/* Offset : 0c*/
	volatile CLK_SELECT_Type CLKSEL;																	/* Offset : 10*/
	/* Dividr should not be ZERO,
	the selected clock signal is divided by n+1 to gebereate sck */
	volatile CLK_RATE_DIVIDER_Type CLKDIVIDER;												/* Offset : 14*/
	volatile SPI_ENABLE_Type ENABLE;																	/* Offset : 18*/
	volatile SPI_MODE_Type MODE;																			/* Offset : 1c*/
	volatile SPI_MODE_FAULT_Type  MODEFAULT;													/* Offset : 20*/
	volatile SPI_CONFIG_Type CONFIG;																	/* Offset : 24*/	
	volatile SPI_ACTIVE_Type ACTIVE;																	/* Offset : 28*/			
	volatile TX_MASK_Type TXMASK;																			/* Offset : 2c*/
	volatile RX_MASK_Type RXMASK;																			/* Offset : 30*/	
}SPI;

#endif	//SPI_HW_INCLUDED
