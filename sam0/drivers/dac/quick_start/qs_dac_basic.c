/**
 * \file
 *
 * \brief SAMD20 DAC Quick Start
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
#include <asf.h>

int main(void)
{
//! [main]
//! [variable]
	struct dac_module dev_inst;
	struct dac_config config;
	struct dac_chan_conf chan_config;
//! [variable]

	/* Setup DAC module*/
//! [get_conf]
	dac_get_config_defaults(&config);
//! [get_conf]
//! [init_dac]
	dac_init(&dev_inst, DAC, &config);
//! [init_dac]
//! [enable]
	dac_enable(&dev_inst);
//! [enable]

	/* Setup channel 0*/
//! [get_ch_conf]
	dac_chan_get_config_defaults(&chan_config);
//! [get_ch_conf]
//! [edit_ch_conf]
	chan_config.enable_start_on_event = false;
	//! [edit_ch_conf]
//! [set_ch_conf]
	dac_chan_set_config(&dev_inst, DAC_CHANNEL_0, &chan_config);
//! [set_ch_conf]
//! [enable_ch]
	dac_chan_enable(&dev_inst, DAC_CHANNEL_0);
	//! [enable_ch]

	/* Convert a value */
	//! [write]
	dac_chan_write(&dev_inst, DAC_CHANNEL_0, 0x44);
//! [write]

	/* Disable DAC*/
//! [disable]
	dac_disable(&dev_inst);
//! [disable]

//! [inf_loop]
	while (1) {
	}
//! [inf_loop]
//! [main]
}
