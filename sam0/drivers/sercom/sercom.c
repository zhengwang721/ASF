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
#include "sercom.h"

#if !defined(__DOXYGEN__)
/**
 * \internal Configuration structure to save current gclk status.
 */
struct _sercom_conf {
	/* Status of gclk generator initialization. */
	bool generator_is_set;
	/* Sercom gclk generator used. */
	//enum gclk_generator generator_source;
	/* Will generator be operational in standby. */
	bool run_in_standby;
};

static struct _sercom_conf _sercom_config;

/**
 * \internal Calculate synchronous baudrate value (SPI/UART)
 */
enum status_code _sercom_get_sync_baud_val(uint32_t baudrate,
		uint32_t external_clock, uint16_t *baudvalue)
{
	/* Baud value variable */
	uint16_t baud_calculated = 0;

	/* Check if baudrate is outside of valid range. */
	if (baudrate > (external_clock / 2)) {
		/* Return with error code */
		return STATUS_ERR_BAUDRATE_UNAVAILABLE;
	}

	/* Calculate BAUD value from clock frequency and baudrate */
	baud_calculated = (external_clock / (2 * baudrate)) - 1;

	/* Check if BAUD value is more than 255, which is maximum
	 * for synchronous mode */
	if (baud_calculated > 0xFF) {
		/* Return with an error code */
		return STATUS_ERR_BAUDRATE_UNAVAILABLE;
	} else {
		*baudvalue = baud_calculated;
		return STATUS_OK;
	}
}

/**
 * \internal Calculate asynchronous baudrate value (UART)
*/
enum status_code _sercom_get_async_baud_val(uint32_t baudrate,
		uint32_t peripheral_clock, uint16_t *baudval)
{
	/* Temporary variables  */
	uint64_t ratio = 0;
	uint64_t scale = 0;
	uint64_t baud_calculated = 0;

	/* Check if the baudrate is outside of valid range */
	if ((baudrate * 16) >= peripheral_clock) {
		/* Return with error code */
		return STATUS_ERR_BAUDRATE_UNAVAILABLE;
	}

	/* Calculate the BAUD value */
	ratio = ((16 * (uint64_t)baudrate) << SHIFT) / peripheral_clock;
	scale = ((uint64_t)1 << SHIFT) - ratio;
	baud_calculated = (65536 * scale) >> SHIFT;

	*baudval = baud_calculated;

	return STATUS_OK;
}
#endif


#ifdef TODO
enum status_code sercom_set_gclk_generator(
		enum gclk_generator generator_source,
		bool run_in_standby,
		bool force_change)
{
	/* Configuration structure for the GCLK channel. */
	struct system_gclk_ch_conf gclk_ch_conf;

	/* Pointer to internal SERCOM configuration. */
	struct _sercom_conf *sercom_config_ptr = &_sercom_config;

	/* Return argument. */
	enum status_code ret_val;

	/* Check if valid option. */
	if(!sercom_config_ptr->generator_is_set || force_change) {
		sercom_config_ptr->generator_is_set = true;

		/* Configure GCLK channel and enable clock */
		gclk_ch_conf.source_generator = generator_source;
#if defined (REVB)
		/* Set the GCLK channel to run in standby mode */
		gclk_ch_conf.run_in_standby = run_in_standby;
#else
		/* Set the GCLK channel sleep enable mode */
		gclk_ch_conf.enable_during_sleep = run_in_standby;
#endif
		/* Apply configuration and enable the GCLK channel */
		system_gclk_ch_set_config(SERCOM_GCLK_ID, &gclk_ch_conf);
		system_gclk_ch_enable(SERCOM_GCLK_ID);

		/* Save config. */
		sercom_config_ptr->generator_source = generator_source;
		sercom_config_ptr->run_in_standby = run_in_standby;

		ret_val = STATUS_OK;

	} else if (generator_source == sercom_config_ptr->generator_source &&
			run_in_standby == sercom_config_ptr->run_in_standby) {
		/* Return status OK if same config. */
		ret_val = STATUS_OK;

	} else {
		/* Return invalid config to already initialized GCLK. */
		ret_val = STATUS_ERR_ALREADY_INITIALIZED;
	}

	return ret_val;
}
#endif

