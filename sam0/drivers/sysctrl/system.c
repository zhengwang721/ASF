/**
 * \file
 *
 * \brief SAM0+ System related functionality
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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

#include "system.h"

/**
 * \brief configure BOD
 *
 * This function will configure the BOD33 or BOD12 module based on the
 * configuration in the configuration struct. The BOD will be enabled when this
 * function returns.
 *
 * \param[in] conf pointer to the struct containing configuration
 * \param[in] bod which BOD module to configure
 *
 * \retval STATUS_ERR_INVALID_ARG Invalid BOD
 * \retval STATUS_ERR_INVALID_OPTION The configured level is outside the acceptable range
 * \retval STATUS_OK Operation completed successfully
 */
enum status_code system_bod_set_config(struct system_bod_config *conf,
		enum system_bod bod)
{
	Assert(conf);

	uint16_t temp;

	temp = conf->action << SYSCTRL_BOD33CTRL_ACTION_gp |
			conf->mode << SYSCTRL_BOD33CTRL_MODE_bp;

	if (conf->mode) {
	/* Enable sampling clock if sampled mode */
		temp |= SYSCTRL_BOD33CTRL_CEN_bm;
	}
	if (conf->hysteresis) {
		temp |= SYSCTRL_BOD33CTRL_HYST_bm;
	}

	temp |= SYSCTRL_BOD33CTRL_ENABLE_bm;
	switch (bod) {
		case SYSTEM_BOD33:
			if (conf->level > 0x3F) {
				return STATUS_ERR_INVALID_ARG;
			}
			SYSCTRL.BOD33LEVEL = conf->level; // 6 bits
			SYSCTRL.BOD33CTRL = temp;
			break;
		case SYSTEM_BOD12:
			if (conf->level > 0x1F) {
				return STATUS_ERR_INVALID_ARG;
			}
			SYSCTRL.BOD12LEVEL = conf->level; // 5 bits
			SYSCTRL.BOD12CTRL = temp;
			break;
		default:
			return STATUS_ERR_INVALID_ARG;
	}
	return STATUS_OK;
}


