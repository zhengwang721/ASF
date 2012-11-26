/**
 * \file
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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

#ifndef __SAM0XX_COMPONENT_DAC__
#define __SAM0XX_COMPONENT_DAC__

#include <io.h>

/* DAC module */
typedef struct {
	__REG8(CTRLA, __IO);
	__REG8(CTRLB, __IO);
	__REG8(EVCTRL, __IO);
	__REG8(reserved_0x03, __I);
	__REG8(INTENCLR, __IO);
	__REG8(INTENSET, __IO);
	__REG8(INTFLAG, __IO);
	__REG8(STATUS, __IO);
	__REG16(DATA, __IO);
	__REG8(reserved_0x0A, __I);
	__REG8(reserved_0x0B, __I);
	__REG16(DATABUF, __IO);
	__REG8(reserved_0x0E, __I);
	__REG8(reserved_0x0F, __I);
} DAC_t;

/* Clears all bits in a 8bits register */
#define DAC_CLEAR_bm 0x00

#define DAC_RESET_bm 1

/* Enables the DAC */
#define DAC_ENABLE_bm 0x02
/* Resets the the DAC to its reset state */
#define DAC_SWRST_bm 0x01

/* Leftadjust data and databuf */
#define DAC_LEFTADJ_bm 0x08
/* Enable the internal DAC channel output to be used by the Analog */
/* Comparator or ADC */
#define DAC_IOEN_bm 0x04
/* Enables the output buffer. */
#define DAC_EOEN_bm 0x02
/* Disables the output buffer when the chip enters to STANDBY sleep mode*/
#define DAC_SLEEPEN_bm 0x01

/* Enables the data buffer empty event generation */
#define DAC_EMPTYEO_bm 0x02

/* Triggers a new conversion on an incoming event */
#define DAC_STARTEI_bm 0x01

/* For Interrupt Enable Clear Register: disables the ready interrupt
 * For Interrupt Enable Set Register: enables the ready interrupt */
#define DAC_READY_bm 0x04

/* For Interrupt Enable Clear Register: disable the data register empty
 * interrupt. For Interrupt Enable Set Register: enable interrupt on data
 * register empty*/
#define DAC_EMPTY_bm 0x02

/* For Interrupt Enable Clear Register: disable the underrun interrupt
 * For Interrupt Enable Set Register: enable interrupt on underrun
 * situations */
#define DAC_UNDERRUN_bm 0x01

/* Set by hardware to indicate that the DAC is busy synchronizing registers
 * between the clock domains */
#define DAC_SYNC_BUSY_bm 0x80

#define DAC_REF_INT1V_bm  (1UL << 5)
#define DAC_REF_AVCC_bm   (1UL << 6)
#define DAC_REF_AREF_bm   (1UL << 7)

#define DAC_GCLK_ID       26


#endif /* __SAM0XX_COMPONENT_DAC__ */
