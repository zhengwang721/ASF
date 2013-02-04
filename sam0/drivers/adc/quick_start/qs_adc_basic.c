/**
 * \file
 *
 * \brief SAMD20 ADC Quick Start
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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
#include <asf.h>

int main(void)
{
//! [main]
//! [variable]
	struct adc_dev_inst dev_inst;
	struct adc_conf config;
//! [variable]
//! [res_variable]
	uint16_t result;
//! [res_variable]

	/* Setup ADC module*/
//! [get_conf]
	adc_get_config_defaults(&config);
//! [get_conf]
//! [init_adc]
	adc_init(&dev_inst, ADC, &config);
//! [init_adc]
//! [enable]
	adc_enable(&dev_inst);
//! [enable]

//! [start_conv]
	adc_start_conversion(&dev_inst);
//! [start_conv]

	/* Wait for conversion to be done and read out result */
//! [get_res]
	do {
	} while(adc_read(&dev_inst, &result) == STATUS_ERR_BUSY);
//! [get_res]

//! [inf_loop]
	while (1) {
		/* Infinite loop */
	}
//! [inf_loop]
//! [main]
}
