/**
 * \file
 *
 * \brief SAMD20 DAC Sine Function Example
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
 *
 * \license
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

//#include "airplane_chime.h"

const uint32_t sample_rate = 8000;
const uint16_t wav_samples[] = {
	#include "data.x"
};

const uint16_t number_of_samples = (sizeof(wav_samples)/sizeof(wav_samples[0]));
/*
uint8_t sine_wave[256] = {
		0x80, 0x83, 0x86, 0x89, 0x8C, 0x90, 0x93, 0x96,
		0x99, 0x9C, 0x9F, 0xA2, 0xA5, 0xA8, 0xAB, 0xAE,
		0xB1, 0xB3, 0xB6, 0xB9, 0xBC, 0xBF, 0xC1, 0xC4,
		0xC7, 0xC9, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD8,
		0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8,
		0xEA, 0xEB, 0xED, 0xEF, 0xF0, 0xF1, 0xF3, 0xF4,
		0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFA, 0xFB, 0xFC,
		0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFD,
		0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF6,
		0xF5, 0xF4, 0xF3, 0xF1, 0xF0, 0xEF, 0xED, 0xEB,
		0xEA, 0xE8, 0xE6, 0xE4, 0xE2, 0xE0, 0xDE, 0xDC,
		0xDA, 0xD8, 0xD5, 0xD3, 0xD1, 0xCE, 0xCC, 0xC9,
		0xC7, 0xC4, 0xC1, 0xBF, 0xBC, 0xB9, 0xB6, 0xB3,
		0xB1, 0xAE, 0xAB, 0xA8, 0xA5, 0xA2, 0x9F, 0x9C,
		0x99, 0x96, 0x93, 0x90, 0x8C, 0x89, 0x86, 0x83,
		0x80, 0x7D, 0x7A, 0x77, 0x74, 0x70, 0x6D, 0x6A,
		0x67, 0x64, 0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52,
		0x4F, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C,
		0x39, 0x37, 0x34, 0x32, 0x2F, 0x2D, 0x2B, 0x28,
		0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18,
		0x16, 0x15, 0x13, 0x11, 0x10, 0x0F, 0x0D, 0x0C,
		0x0B, 0x0A, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04,
		0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
		0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x0A,
		0x0B, 0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x13, 0x15,
		0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24,
		0x26, 0x28, 0x2B, 0x2D, 0x2F, 0x32, 0x34, 0x37,
		0x39, 0x3C, 0x3F, 0x41, 0x44, 0x47, 0x4A, 0x4D,
		0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x61, 0x64,
		0x67, 0x6A, 0x6D, 0x70, 0x74, 0x77, 0x7A, 0x7D
};
 */
/**
 * \brief Function for configuring the pins
 */
static void configure_pins(void)
{
	struct system_pinmux_conf pin_conf;
	system_pinmux_get_config_defaults(&pin_conf);

	/* Set up the Xplained PRO LED pin to output status info */
	pin_conf.mux_position = SYSTEM_PINMUX_GPIO;
	pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	system_pinmux_pin_set_config(PIN_PB08, &pin_conf);

	pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_INPUT;
	system_pinmux_pin_set_config(PIN_PB09, &pin_conf);
}

/**
 * \brief Function for configuring DAC
 *
 * This function will configure the DAC using the default DAC configuration,
 * except for manual trigger instead of event trigger.
 *
 * \param dev_inst pointer to the module descriptor
 */
static void configure_dac(struct dac_module *dac_module)
{
	struct dac_conf config;
	struct dac_chan_conf ch_config;

	/* Get the DAC default configuration */
	dac_get_config_defaults(&config);

	/* Switch to GCLK generator 3 */
	config.clock_source = GCLK_GENERATOR_0;

	/* Initialize and enable the DAC */
	dac_init(dac_module, DAC, &config);
	dac_enable(dac_module);

	/* Get the default DAC channel config */
	dac_chan_get_config_defaults(&ch_config);

	/* Disable start on event, we want manual trigger */
	ch_config.enable_start_on_event = true;

	/* Set the channel configuration, and enable it */
	dac_chan_set_config(dac_module, DAC_CHANNEL_0, &ch_config);
	dac_chan_enable(dac_module, DAC_CHANNEL_0);
}

static void configure_tc(struct tc_module *tc_module)
{
	struct tc_conf config;
	struct tc_events events;

	tc_get_config_defaults(&config);

	config.clock_source = GCLK_GENERATOR_0;
	config.wave_generation = TC_WAVE_GENERATION_MATCH_FREQ;

	tc_init(TC0, tc_module, &config);

	events.generate_event_on_overflow = true;
	events.on_event_perform_action = false;
	events.generate_event_on_compare_channel[0] = false;
	events.generate_event_on_compare_channel[1] = false;

	tc_enable_events(tc_module, &events);

	tc_set_top_value(tc_module, 1000);

	tc_enable(tc_module);
}

static void configure_event_channel(void)
{
	struct events_chan_conf events_chan_conf;
	events_chan_get_config_defaults(&events_chan_conf);

	events_chan_conf.generator_id = EVSYS_ID_GEN_TC0_OVF;
	events_chan_conf.edge_detection = EVENT_EDGE_RISING;
	events_chan_conf.path = EVENT_PATH_RESYNCHRONOUS;
	events_chan_set_config(0, &events_chan_conf);

	//while (!events_chan_is_ready(1));
	//while (!events_user_is_ready(0));
}

static void configure_event_user(void)
{
	struct events_user_conf events_user_conf;
	events_user_get_config_defaults(&events_user_conf);
	events_user_conf.event_channel_id = 1;
	events_user_set_config(EVSYS_ID_USER_DAC_START, &events_user_conf);
}

static void configure_events(void)
{
	struct system_gclk_chan_conf gclk_chan_conf;

	gclk_chan_conf.source_generator = GCLK_GENERATOR_0;
	gclk_chan_conf.run_in_standby   = false;

	system_gclk_chan_set_config(EVSYS_GCLK_ID_0, &gclk_chan_conf);
	system_gclk_chan_enable(EVSYS_GCLK_ID_0);

	events_init();

	configure_event_user();
	configure_event_channel();

}

/**
 * \brief Main application routine
 */
int main(void)
{
	struct dac_module dac_module;
	struct tc_module tc_module;

	configure_pins();

	/* Initialize all the system clocks, pm, gclk... */
	system_init();

	/* Enable the internal bandgap to use as reference to the DAC */
	system_vref_enable(SYSTEM_VOLTAGE_REFERENCE_BANDGAP);

	configure_tc(&tc_module);

	/* Configure the DAC */
	configure_dac(&dac_module);

	tc_start_counter(&tc_module);

	configure_events();

	/* Main application loop that writes a sine wave */
	while (true) {
		while (port_pin_get_input_level(PIN_PB09)) {
			/* Wait */
		}

		port_pin_toggle_output_level(PIN_PB08);

		for (uint16_t i = 0; i < number_of_samples; i++) {
			dac_write(&dac_module, DAC_CHANNEL_0, wav_samples[i], true);

			//events_chan_software_trigger(1);

			while (!(DAC->INTFLAG.reg & DAC_INTFLAG_EMPTY)) {
				/* Wait for data buffer to be empty */
			}

		}

		while (!port_pin_get_input_level(PIN_PB09)) {
			/* Wait */
		}

	}

}
