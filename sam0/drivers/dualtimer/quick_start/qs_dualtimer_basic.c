/**
 * \file
 *
 * \brief SAM TIMER Driver Quick Start for SAMB11
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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
#include <asf.h>

uint32_t dualtimer_counter1, dualtimer_counter2;

//! [setup]
//! [setup_dualtimer_init]
void configure_dualtimer(void);
//! [setup_dualtimer_init]


void configure_dualtimer(void)
{
//! [setup_dualtimer_1]
	struct dualtimer_config config_dualtimer;
//! [setup_dualtimer_1]
//! [setup_dualtimer_2]
	dualtimer_get_config_defaults(&config_dualtimer);
//! [setup_dualtimer_2]
//! [setup_dualtimer_3]
	config_dualtimer.load_value = CONF_DUALTIMER_TIMER1_LOAD;
//! [setup_dualtimer_3]
//! [setup_dualtimer_4]
	dualtimer_init(DUALTIMER_TIMER1, &config_dualtimer);
//! [setup_dualtimer_4]
//! [setup_dualtimer_5]
	config_dualtimer.load_value = CONF_DUALTIMER_TIMER2_LOAD;
//! [setup_dualtimer_5]
//! [setup_dualtimer_6]
	dualtimer_init(DUALTIMER_TIMER2, &config_dualtimer);
//! [setup_dualtimer_6]


//! [setup_dualtimer_7]
	dualtimer_enable(DUALTIMER_TIMER1);
//! [setup_dualtimer_7]
//! [setup_dualtimer_8]
	dualtimer_enable(DUALTIMER_TIMER2);
//! [setup_dualtimer_8]
}
//! [setup]

int main(void)
{
//! [setup_init]

	//system_init();
	
//! [dualtimer_config]
	configure_dualtimer();
//! [dualtimer_config]

//! [setup_init]
	
//! [main_imp]
//! [main_loop]
	while (true) {
//! [main_loop]
//! [timer1_interrupt]
		if (dualtimer_get_interrupt_status_raw(DUALTIMER_TIMER1)) {
//! [timer1_interrupt]
//! [timer1_interrupt_clr]
			dualtimer_clear_interrupt_status(DUALTIMER_TIMER1);
//! [timer1_interrupt_clr]
//! [counter1]
			dualtimer_counter1++;
//! [counter1]
		}
//! [timer2_interrupt]
		if (dualtimer_get_interrupt_status_raw(DUALTIMER_TIMER2)) {
//! [timer2_interrupt]
//! [timer2_interrupt_clr]
			dualtimer_clear_interrupt_status(DUALTIMER_TIMER2);
//! [timer2_interrupt_clr]
//! [counter2]
			dualtimer_counter2++;
//! [counter2]
		}
	}
//! [main_imp]
}
