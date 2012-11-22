/**
 * \file
 *
 * \brief SAM0+ PAC Quick Start
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
#include <asf.h>

void config_port_pins(void);

//! [pin_setup]
void config_port_pins(void)
{
	struct port_pin_conf pin_conf;
	port_pin_get_config_defaults(&pin_conf);

	pin_conf.input.enabled = true;
	pin_conf.output.enabled = false;
	pin_conf.input.pull = PORT_PIN_PULL_UP;
	pin_conf.input.sampling_mode = PORT_PIN_SAMPLING_CONTINUOUS;
	pin_conf.input.edge_detection.wake_if_sleeping = false;
	pin_conf.input.edge_detection.filter_input_signal = true;
	pin_conf.input.edge_detection.mode
		= PORT_EDGE_DETECT_FALLING;
	port_pin_set_config(10, &pin_conf);

	pin_conf.input.enabled = false;
	pin_conf.output.enabled = true;
	port_pin_set_config(11, &pin_conf);
}
//! [pin_setup]

int main (void)
{
//! [main]
//! [init]
	config_port_pins();
//! [init]
//! [init_lock]
	system_peripheral_lock(SYSTEM_PERIPHERAL_PORT, ~SYSTEM_PERIPHERAL_PORT);
//! [init_lock]

//! [set_sanity]
	uint8_t sanity = 0xFA;
//! [set_sanity]

//! [sei]
	//sei();
//! [sei]

//! [button_press]
	while(!port_pin_is_edge_detected(10));
//! [button_press]

//! [disable_interrupts]
	//cli();
//! [disable_interrupts]

//! [unlock_perph]
	system_peripheral_unlock(SYSTEM_PERIPHERAL_PORT,
			~SYSTEM_PERIPHERAL_PORT);
//! [unlock_perph]

//! [sanity_check]
	if (sanity != 0xFA){
		while(1);
	}
//! [sanity_check]

//! [alter_config]
	port_pin_toggle_output_level(11);
	port_pin_clear_edge_detected(10);
//! [alter_config]

//! [lock_perph]
	system_peripheral_lock(SYSTEM_PERIPHERAL_PORT,
			~SYSTEM_PERIPHERAL_PORT);
//! [lock_perph]

//! [clear_sanity]
	sanity = NULL;
//! [clear_sanity]

//! [enable_interrupts]
	//sei();
//! [enable_interrupts]

//! [inf_loop]
	while(1){
	}
//! [inf_loop]
//! [main]
}
