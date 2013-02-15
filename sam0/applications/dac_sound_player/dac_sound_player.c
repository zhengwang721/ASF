/**
 * \file
 *
 * \brief SAMD20 DAC Sound Player Application
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

const uint32_t sample_rate = 16000;
const uint16_t wav_samples[] = {
	#include "data.x"
};

const uint32_t number_of_samples = (sizeof(wav_samples)/sizeof(wav_samples[0]));

/**
 * \brief Function for configuring the pins
 */
static void configure_pins(void)
{
	struct system_pinmux_config pin_config;
	system_pinmux_get_config_defaults(&pin_config);

	/* Set up the Xplained PRO LED pin to output status info */
	pin_config.mux_position = SYSTEM_PINMUX_GPIO;
	pin_config.direction = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
	pin_config.input_pull = PORT_PIN_PULL_UP;
	system_pinmux_pin_set_config(LED0_PIN, &pin_config);

	pin_config.direction = SYSTEM_PINMUX_PIN_DIR_INPUT;
	system_pinmux_pin_set_config(SW0_PIN, &pin_config);
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
	struct dac_config config;
	struct dac_chan_config channel_config;

	/* Get the DAC default configuration */
	dac_get_config_defaults(&config);

	/* Switch to GCLK generator 0 */
	config.clock_source = GCLK_GENERATOR_0;

	/* Initialize and enable the DAC */
	dac_init(dac_module, DAC, &config);
	dac_enable(dac_module);

	/* Get the default DAC channel config */
	dac_chan_get_config_defaults(&channel_config);

	/* Disable start on event, we want manual trigger */
	channel_config.enable_start_on_event = true;

	/* Set the channel configuration, and enable it */
	dac_chan_set_config(dac_module, DAC_CHANNEL_0, &channel_config);
	dac_chan_enable(dac_module, DAC_CHANNEL_0);
}

static void configure_tc(struct tc_module *tc_module)
{
	struct tc_conf config;
	struct tc_events events;

	tc_get_config_defaults(&config);

	config.clock_source = GCLK_GENERATOR_0;
	config.wave_generation = TC_WAVE_GENERATION_MATCH_FREQ;

	tc_init(tc_module, TC0, &config);

	events.generate_event_on_overflow = true;
	events.enable_incoming_events = false;
	events.generate_event_on_compare_channel[0] = false;
	events.generate_event_on_compare_channel[1] = false;

	tc_enable_events(tc_module, &events);

	tc_set_top_value(tc_module,
			system_gclk_gen_get_hz(GCLK_GENERATOR_0)/sample_rate);

	tc_enable(tc_module);

	/* Turn off the APB clock for the timer, as we do not need to
	 * reconfigure it and it runs from the GCLK */
	system_apb_clock_clear_mask(SYSTEM_CLOCK_APB_APBC, PM_APBCMASK_TC0);
}

static void configure_event_channel(void)
{
	struct events_chan_config events_chan_config;
	events_chan_get_config_defaults(&events_chan_config);

	events_chan_config.generator_id = EVSYS_ID_GEN_TC0_OVF;
	events_chan_config.edge_detection = EVENT_EDGE_RISING;
	events_chan_config.path = EVENT_PATH_RESYNCHRONOUS;
	events_chan_set_config(EVENT_CHANNEL_0, &events_chan_config);

}

static void configure_event_user(void)
{
	struct events_user_config events_user_config;
	events_user_get_config_defaults(&events_user_config);
	events_user_config.event_channel_id = EVENT_CHANNEL_0;
	events_user_set_config(EVSYS_ID_USER_DAC_START, &events_user_config);
}

static void configure_events(void)
{
	events_init();

	configure_event_user();
	configure_event_channel();

	/* Turn off the APB clock for the evsys, as we do not need to
	 * reconfigure it and it runs from the GCLK */
	system_apb_clock_clear_mask(SYSTEM_CLOCK_APB_APBC, PM_APBCMASK_EVSYS);
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
		while (port_pin_get_input_level(SW0_PIN)) {
			/* Wait for the button to be pressed */
		}

		port_pin_toggle_output_level(LED0_PIN);

		for (uint32_t i = 0; i < number_of_samples; i++) {
			dac_chan_write(&dac_module, DAC_CHANNEL_0, wav_samples[i]);

			while (!(DAC->INTFLAG.reg & DAC_INTFLAG_EMPTY)) {
				/* Wait for data buffer to be empty */
			}

		}

		while (!port_pin_get_input_level(SW0_PIN)) {
			/* Wait for the button to be depressed */
		}

	}

}
