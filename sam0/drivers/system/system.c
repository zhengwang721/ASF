/**
 * \file
 *
 * \brief SAMD20 System related functionality
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

#include <system.h>
#include <conf_bod.h>

/**
 * Handler for the CPU Hard Fault interrupt, fired if an illegal access was
 * attempted to a memory address.
 */
void HardFault_Handler(void)
{
	while (1) {
		/* Infinite loop if CPU exception is detected */
		Assert(false);
	}
}

/**
 * \brief Configure a Brown Out Detector module.
 *
 * Configures a given BOD33 or BOD12 module based on the settings stored in the
 * configuration struct. The BOD will be enabled when this function returns.
 *
 * \param[in] bod    BOD module ID to configure
 * \param[in] conf   Configuration settings for the BOD
 *
 * \retval STATUS_ERR_INVALID_ARG Invalid BOD
 * \retval STATUS_ERR_INVALID_OPTION The configured level is outside the acceptable range
 * \retval STATUS_OK Operation completed successfully
 */
enum status_code system_bod_set_config(
		const enum system_bod bod,
		struct system_bod_config *const conf)
{
	/* Sanity check arguments */
	Assert(conf);

	uint32_t temp;

	/* Convert BOD trigger action and mode to a bitmask */
	temp = (uint32_t)conf->action | (uint32_t)conf->mode;

	if (conf->mode == SYSTEM_BOD_MODE_SAMPLED) {
		/* Enable sampling clock if sampled mode */
		temp |= SYSCTRL_BOD33_CEN;
	}

	if (conf->hysteresis == true) {
		temp |= SYSCTRL_BOD33_HYST;
	}

	switch (bod) {
		case SYSTEM_BOD_BOD33:
			if (conf->level > 0x3F) {
				return STATUS_ERR_INVALID_ARG;
			}

			SYSCTRL->BOD33.reg = SYSCTRL_BOD33_LEVEL(conf->level) |
					temp | SYSCTRL_BOD33_ENABLE;
			break;
		case SYSTEM_BOD_BOD12:
			if (conf->level > 0x1F) {
				return STATUS_ERR_INVALID_ARG;
			}

			SYSCTRL->BOD12.reg = SYSCTRL_BOD12_LEVEL(conf->level) |
					temp | SYSCTRL_BOD12_ENABLE;
			break;
		default:
			return STATUS_ERR_INVALID_ARG;
	}

	return STATUS_OK;
}

/**
 * \brief Initialize BOD12 and BOD33 based on the configuration in conf_bod.h
 *
 */
void system_bod_init(void)
{
#if (BOD33_ENABLED == true) || (BOD12_ENABLED == true)
	struct system_bod_config conf;

#  if BOD33_ENABLED == true
	conf.action       = BOD33_ACTION;
	conf.sampled_mode = BOD33_MODE;
	conf.prescaler    = BOD33_PRESCALER;
	conf.hysteresis   = BOD33_HYSTERESIS;

	system_bod_set_config(SYSTEM_BOD_BOD33, &conf);
#  endif

#  if BOD12_ENABLED == true
	conf.action       = BOD12_ACTION;
	conf.sampled_mode = BOD12_MODE;
	conf.prescaler    = BOD12_PRESCALER;
	conf.hysteresis   = BOD12_HYSTERESIS;

	system_bod_set_config(SYSTEM_BOD_BOD12, &conf);
#  endif

#endif
}

/**
 * \brief Initialize system
 *
 * This function will call the initialization functions for the system namespace
 * in a single function. The functions are weak functions, so if one of the
 * functions are not provided by the ASF project it will just run a no operation
 * function.
 *
 */
void system_init(void)
{
	/* Initialize BOD according to conf_bod.h */
	system_bod_init();

	/* Configure GCLK and clock sources according to conf_clocks.h */
	system_clock_init();

	/* Initialize board hardware */
	system_board_init();
}

