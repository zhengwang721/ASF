/**
 * \file
 *
 * \brief AT30TS(E)75x Temperature Sensor Example.
 *
 * Copyright (c) 2013-2015 Atmel Corporation. All rights reserved.
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

/**
 * \mainpage AT30TS(E)75x Temperature Sensor Example
 *
 * \section Purpose
 *
 * The application demonstrates how to access AT30TS(E)75x temperature sensor.
 *
 * \section Requirements
 *
 * This package can be used with:
 * - SAM Xplained Pro with I01 Xplained Pro attached on EXT1.
 * - SAM4C_EK
 * - SAM4CMP-DB and SAM4CMS-DB
 *
 * \section Description
 * There are 2 stages in the example.
 * - In 1st stage, some patterns are written to the specified memory address of
 * the EEPROM in AT30TSE75x. Then the memory is read and checked (note this
 * stage is only for the device which has EEPROM).
 * - In 2nd stage, the temperature sampled by AT30TS(E)75x is read every second.
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR EWARM.
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com/">Atmel</A>.\n
 * Support and FAQ: http://www.atmel.com/design-support/
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <string.h>
#include "at30tse75x.h"

#define NB_PAGE 16
#define NB_BYTE 16

uint8_t rx[NB_BYTE], tx[NB_BYTE];

static void delay(int32_t count)
{
	for (int32_t i = 0; i < count; i++) {
		for (int32_t j = 0; j < 100; j++)
			asm volatile ("nop");
	}
}

int main(void)
{
	volatile double temper;
	volatile bool passed;
	uint32_t i;

	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);

	memset(tx, 0xFF, NB_BYTE);
	
	/* Initialize AT30TS(E)75x */
	at30tse_init();
	
	/* Write pages in EEPROM */
	for (i = 0; i < NB_PAGE; i++) {
		tx[NB_PAGE - 1] = i;
		at30tse_eeprom_write(tx, NB_BYTE, 0, i);
		delay(100);
	}

	passed = true;
	
	/* Read each page in EEPROM and compare them */
	for (i = 0; i < NB_PAGE; i++) {
		memset(rx, 0, NB_BYTE);
		at30tse_eeprom_read(rx, NB_BYTE, 0, i);
		if (memcmp(tx, rx, NB_BYTE - 1) || (rx[NB_PAGE - 1] != i)) {
			passed = false;
			break;
		}
	}
	
	/* Read thigh and tlow */
	volatile uint16_t thigh = 0;
	thigh = at30tse_read_register(AT30TSE_THIGH_REG,
			AT30TSE_NON_VOLATILE_REG, AT30TSE_THIGH_REG_SIZE);

	volatile uint16_t tlow = 0;
	tlow = at30tse_read_register(AT30TSE_TLOW_REG,
			AT30TSE_NON_VOLATILE_REG, AT30TSE_TLOW_REG_SIZE);
			
	/* Set 12-bit resolution mode. */
	at30tse_write_config_register(
			AT30TSE_CONFIG_RES(AT30TSE_CONFIG_RES_12_bit));

	while (1) {
		/* Read current temperature. */
		temper = at30tse_read_temperature();
	}
	UNUSED(passed);
	UNUSED(temper);
	UNUSED(tlow);
	UNUSED(thigh);
}
