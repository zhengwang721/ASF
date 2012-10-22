/**
 * \file
 *
 * \brief SAM0+ Driver Example
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
 
 /**
 * \mainpage
 *
 * \section intro Introduction
 * Simple example using the ready SAM0+ drivers.
 *
 * \section files Main Files
 * - driver_example.c
 *
 * \section device_info Device Info
 *
 * \section description Description of the example
 * Initialize clock system, set internal RC oscillator as main clock,
 * configure SW0 as input and LED0 as output, generate a software event.
 * Put the device to sleep and wakeup from SW0, toggle LED0.
 *
 * \section dependencies Dependencies
 * This example depends on the following modules:
 * - \ref sam0_port_group
 * - \ref system_group
 * - \ref sam0_events_group
 * - \ref clock_group
 *
 * \section compinfo Compilation info
 *
 * \section contactinfo Contact Information
 * For further information, visit <a href="http://www.atmel.com/">Atmel</a>.\n
 */

 #include <asf.h>

#define EXAMPLE_EVENT_GENERATOR    0
#define EXAMPLE_EVENT_CHANNEL      0
#define EXAMPLE_EVENT_USER         0

void configure_clock_sources(void)
{
	struct clock_source_config cs_conf;
	clock_source_get_default_config(&cs_conf);

	/* 8MHz RC oscillator */
	cs_conf.rc8mhz_prescaler = 4;

	clock_source_set_config(&cs_conf, CLOCK_SOURCE_RC8MHZ);

	/* XOSC */
	cs_conf.external_clock = CLOCK_EXTERNAL_CRYSTAL;

	clock_source_set_config(&cs_conf, CLOCK_SOURCE_XOSC);

	/* DFLL */
	cs_conf.dfll_coarse_value = 42;
	cs_conf.dfll_fine_value = 42;

	clock_source_set_config(&cs_conf, CLOCK_SOURCE_DFLL);
}


void configure_pins(void)
{
	struct port_pin_conf pin_conf;
	port_pin_get_config_defaults(&pin_conf);
	
	/* SW0 config */
	pin_conf.input.enabled  = true;
	pin_conf.output.enabled = false;
	/* Wake up the microcontroller if the channel interrupt fires during
		sleep mode. */
	pin_conf.input.edge_detection.wake_if_sleeping = true;
	/** Detect rising signal edges. */
	pin_conf.input.edge_detection.mode = PORT_EDGE_DETECT_RISING;
	/* Pin input buffer should continuously sample the pin state. */
	pin_conf.input.sampling_mode = PORT_PIN_SAMPLING_CONTINUOUS;
	port_pin_set_config(SW0, &pin_conf);

	/* LED0 config */
	pin_conf.input.enabled  = false;
	pin_conf.output.enabled = true;
	port_pin_set_config(LED0, &pin_conf);
}

void configure_event_channel(void)
{
	struct events_ch_conf events_ch_conf;
	events_ch_get_config_defaults(&events_ch_conf);
	events_ch_conf.generator_id   = EXAMPLE_EVENT_GENERATOR;
	events_ch_conf.edge_detection = EVENT_EDGE_RISING;
	events_ch_conf.path           = EVENT_PATH_SYNCHRONOUS;
	events_ch_set_config(EXAMPLE_EVENT_CHANNEL, &events_ch_conf);
}

void configure_event_user(void)
{
	struct events_user_conf events_user_conf;
	events_user_get_config_defaults(&events_user_conf);
	events_user_conf.event_channel_id = EXAMPLE_EVENT_CHANNEL;
	events_user_set_config(EXAMPLE_EVENT_USER, &events_user_conf);
}

void configure_bod(void)
{
	struct system_bod_config system_bod_config;
	//system_bod_get_config_defaults(&system_bod_config)??
	system_bod_get_default_config(&system_bod_config);
	
	/* BOD will use the BOD sampling clock (1kHz) to sample the supply line */
	system_bod_config.mode = SYSTEM_BOD_MODE_SAMPLED;
	system_bod_set_config(&system_bod_config, SYSTEM_BOD12);
	// Set config not consistant between drivers (set_config(*conf, xx) vs set_config(xx, *conf) )
	
}

int main(void)
{
	//	board_init();
	
	// Init clock system from conf_clocks.h
	clocks_init();
	
	// Configure clock sources
	configure_clock_sources();
	
	// Set internal RC oscillator as main clock source
	clock_main_clock_set_clocksource(CLOCK_CPU_RC8);
	
	// Configure BOD
	configure_bod();
	
	// Configure SW0 as input and LED0 as output.
	configure_pins();
	
	// Init and configure events
	events_init();
	configure_event_channel();
	configure_event_user();
	
	while (events_ch_is_ready(EXAMPLE_EVENT_CHANNEL) == false) {
			/* Wait for channel */
	};
	// Trigger software event
	events_ch_software_trigger(EXAMPLE_EVENT_CHANNEL);

	
	// Set sleepmode SYSTEM_SLEEPMODE_IDLE_0
	if (!(system_set_sleepmode(SYSTEM_SLEEPMODE_IDLE_0) == STATUS_OK)) {
		return;
	}
		
	while(1) {
		// Go to sleep		
		system_sleep();

		// Toggle LED on wakeup.
		port_pin_toggle_output_level(LED0);
	}
}
