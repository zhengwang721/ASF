/**
 * \file
 *
 * \brief SAM0+ Serial Peripheral Interface Driver
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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
#include <sercom.h>


/* TODO: move? */
#define SHIFT 1

/**
 * \brief Calculate synchronous baudrate value (SPI/UART)
 */
enum status_code sercom_get_sync_baud_val(uint32_t baudrate,
		uint32_t external_clock, uint16_t *baudval)
{
	uint16_t ret = 0;

	if (baudrate > (external_clock / 2)) {
		// assert error, outside valid range
	}

	//ret = (external_clock / (2 * baudrate)) - 1;

	if (ret > 0xFF) {
		// assert error; max BAUD value for sync is 255 (8-bit)
	} else {
		return ret;
	}
	return ret;
}

/**
 * \brief Calculate asynchronous baudrate value (UART)
*/
enum status_code sercom_get_async_baud_val(uint32_t baudrate,
		uint32_t peripheral_clock, uint16_t *baudval)
{

	if ((baudrate * 16) >= peripheral_clock) {
		// Unattainable baud rate
		return 4;
	}

	uint64_t ratio = 0;
	uint64_t scale = 0;
	uint64_t retval = 0;

	ratio = ((16 * (uint64_t)baudrate) << SHIFT) / peripheral_clock;
	scale = (1ull << SHIFT) - ratio;
	retval = (65536 * scale) >> SHIFT;

	*baudval = retval;

	return STATUS_OK;
}

