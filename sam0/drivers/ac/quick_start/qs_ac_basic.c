/**
 * \file
 *
 * \brief SAM0+ Analog Comparator Driver Quick Start
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

#include <ac.h>

//! [setup]
/* AC module software instance (must not go out of scope while in use) */
static struct ac_dev_inst ac_dev;

/* Comparator channel that will be used */
#define AC_COMPARATOR_CHANNEL   0

void configure_ac(void)
{
	/* Create a new configuration structure for the Analog Comparator settings
	 * and fill with the default module settings. */
	//! [setup_1]
	struct ac_conf ac_conf;
	//! [setup_1]
	//! [setup_2]
	ac_get_config_defaults(&ac_conf);
	//! [setup_2]

	/* Set the Analog Comparator configuration settings */
	//! [setup_3]
	ac_conf.enable_pair_during_sleep[0] = true;
	//! [setup_3]

	/* Initialize and enable the Analog Comparator with the user settings */
	//! [setup_4]
	ac_init(&ac_dev, &AC, &ac_conf);
	//! [setup_4]
	//! [setup_5]
	ac_enable(&ac_dev);
	//! [setup_5]

}

void configure_ac_channel(void)
{
	/* Create a new configuration structure for the Analog Comparator channel
	 * settings and fill with the default module channel settings. */
	//! [setup_6]
	struct ac_ch_conf ac_ch_conf;
	//! [setup_6]
	//! [setup_7]
	ac_ch_get_config_defaults(&ac_ch_conf);
	//! [setup_7]

	/* Set the Analog Comparator channel configuration settings */
	//! [setup_8]
	ac_ch_conf.sample_mode      = AC_CH_MODE_SINGLE_SHOT;
	ac_ch_conf.positive         = AC_CH_POS_MUX_PIN0;
	ac_ch_conf.negative         = AC_CH_NEG_MUX_SCALED_VCC;
	ac_ch_conf.vcc_scale_factor = 32;
	//! [setup_8]

	/* Initialize and enable the Analog Comparator channel with the user
	 * settings */
	//! [setup_9]
	ac_ch_set_config(&ac_dev, &ac_ch_conf, AC_COMPARATOR_CHANNEL);
	//! [setup_9]
	//! [setup_10]
	ac_ch_enable(&ac_dev, AC_COMPARATOR_CHANNEL);
	//! [setup_10]
}
//! [setup]

int main(void)
{
	//! [setup_init]
	configure_ac();
	configure_ac_channel();
	//! [setup_init]

	//! [main]
	//! [main_1]
	ac_ch_trigger_single_shot(&ac_dev, AC_COMPARATOR_CHANNEL);
	//! [main_1]

	//! [main_2]
	enum ac_ch_state last_comparison = AC_CH_STATE_UNKNOWN;
	//! [main_2]

	//! [main_3]
	while (true) {
	//! [main_3]
	//! [main_4]
		if (ac_ch_is_ready(&ac_dev, AC_COMPARATOR_CHANNEL)) {
	//! [main_4]
			//! [main_5]
			last_comparison = ac_ch_get_state(
					&ac_dev, AC_COMPARATOR_CHANNEL);
			//! [main_5]
		}
	}
	//! [main]
}
