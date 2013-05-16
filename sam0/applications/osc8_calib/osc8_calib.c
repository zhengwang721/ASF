/**
 * \file
 *
 * \brief SAM0+ OSC8MHz Calibration Application
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

#define RES 4096
#define CAL_CLOCK_HZ 32768

static struct usart_module usart_edbg;

/* Write string to usart. */
static void debug_write_string(struct usart_module *const dev, uint8_t const *string)

{
	do {
		while (usart_write_wait(dev, *string) != STATUS_OK) {
		}
	} while (*(++string) != 0);
}

/* Convert number to string */
static void debug_int_to_string(uint8_t *ret_val, uint8_t size, uint32_t integer)
{
	uint8_t i = 0;
	uint32_t temp_int = integer;

	while (temp_int) {
		temp_int /= 10;
		i++;
	}

	while (size--) {
		if (size < i || size == 0) {
			ret_val[size] = integer % 10 + 48;
			integer /= 10;
		} else {
			ret_val[size] = 0;
		}
	}
}

/* Set up tc and events */
static void setup_tc_channels(struct tc_module *const calib_chan, struct tc_module *const comp_chan)
{
	events_init();

	struct tc_config config;

	tc_get_config_defaults(&config);

	config.counter_size = TC_COUNTER_SIZE_32BIT;
	config.clock_prescaler = TC_CLOCK_PRESCALER_DIV1;
	config.wave_generation = TC_WAVE_GENERATION_NORMAL_FREQ;
	config.enable_capture_on_channel[0] = true;

	tc_init(calib_chan, TC0, &config);

	config.enable_capture_on_channel[0] = false;
	config.counter_size = TC_COUNTER_SIZE_16BIT;
	config.clock_source = GCLK_GENERATOR_3;
	config.size_specific.size_16_bit.compare_capture_channel[0] = RES;
	tc_init(comp_chan, TC2, &config);

	struct tc_events events;
	tc_get_events_config_default(&events);

	events.generate_event_on_compare_channel[0] = true;

	tc_enable_events(comp_chan, &events);

	events.generate_event_on_compare_channel[0] = false;
	events.enable_incoming_events = true;

	tc_enable_events(calib_chan, &events);

	tc_enable(calib_chan);
	tc_enable(comp_chan);

	struct events_chan_config evch_conf;
	events_chan_get_config_defaults(&evch_conf);

	evch_conf.edge_detection = EVENT_EDGE_RISING;
	evch_conf.path = EVENT_PATH_SYNCHRONOUS;
	evch_conf.generator_id = EVSYS_ID_GEN_TC2_MCX_0;

	struct events_user_config evus_conf;
	evus_conf.event_channel_id = EVENT_CHANNEL_0;

	events_user_set_config(EVSYS_ID_USER_TC0_EVU, &evus_conf);
	events_chan_set_config(EVENT_CHANNEL_0, &evch_conf);

}

/* Setup and initialize USART device. */
static void setup_usart_channel(void)
{
	struct usart_config config_struct;

	usart_get_config_defaults(&config_struct);
	config_struct.mux_settings = USART_RX_3_TX_2_XCK_3;
	config_struct.pinout_pad3 = EDBG_CDC_RX_PINMUX;
	config_struct.pinout_pad2 = EDBG_CDC_TX_PINMUX;

	while (usart_init(&usart_edbg, EDBG_CDC_MODULE,
			&config_struct) != STATUS_OK) {
	}

	usart_enable(&usart_edbg);

	usart_enable_transceiver(&usart_edbg, USART_TRANSCEIVER_TX);
	usart_enable_transceiver(&usart_edbg, USART_TRANSCEIVER_RX);
}

/* Get current frequency */
static uint32_t debug_get_freq(struct tc_module *calib_chan, struct tc_module *comp_chan)
{
	uint64_t tmp;

	tc_clear_status(comp_chan, TC_STATUS_CHANNEL_0_MATCH);

	tc_start_counter(calib_chan);
	tc_start_counter(comp_chan);

	while (!(tc_get_status(comp_chan) & TC_STATUS_CHANNEL_0_MATCH)) {
		/* Wait for channel 0 match */
	}

	tmp = (uint64_t)tc_get_capture_value(calib_chan, TC_COMPARE_CAPTURE_CHANNEL_0);
	tmp = tmp * CAL_CLOCK_HZ;

	return (uint32_t)(tmp / RES);
}

/* Wait loop */
static void debug_wait(uint16_t ticks) {
	while (ticks--) {
		__asm__("NOP");
	}
}

int main(void)
{
	uint8_t string[10];

	system_init();
	//setup_usart_channel();

	uint32_t tmp;

	/* Set up to clock out osc 8 on pin */
	struct system_pinmux_config pin_mux;
	system_pinmux_get_config_defaults(&pin_mux);
	pin_mux.mux_position = MUX_PA30H_GCLK_IO0;
	system_pinmux_pin_set_config(PIN_PA30H_GCLK_IO0, &pin_mux);

	/* Structures for config and software device instance */
	struct tc_module calib_chan;
	struct tc_module comp_chan;

	/* Set up tc modules to be used. */
	setup_tc_channels(&calib_chan, &comp_chan);

	/* Values for calculating current frequency of osc. */
	uint16_t comm_cal;
	uint8_t frange_cal;
	uint16_t comm_best=0;
	uint8_t frange_best=0;
	uint32_t freq_best = 0xffffffff;
	uint32_t freq_before = debug_get_freq(&calib_chan, &comp_chan);
	uint32_t gen_freq = system_gclk_chan_get_hz(SERCOM_GCLK_ID);

	/* Run calibration */
	for (frange_cal = 0; frange_cal < 4; frange_cal++) {
		for (comm_cal = 0; comm_cal < 128; comm_cal++) {
			system_clock_source_write_calibration(SYSTEM_CLOCK_SOURCE_OSC8M, comm_cal | 8 << 7, frange_cal);

			debug_wait(1000);
			tmp = debug_get_freq(&calib_chan, &comp_chan);

			if (abs(tmp - gen_freq) < abs(freq_best - gen_freq))
			{
				freq_best = tmp;
				comm_best = comm_cal;
				frange_best = frange_cal;
			}
		}
	}
	/* Set the found best calibration. */
	system_clock_source_write_calibration(SYSTEM_CLOCK_SOURCE_OSC8M, comm_best | 8 << 7, frange_best);

	/* Setup usart module to give information back. */
	setup_usart_channel();

	/* Get calibrated value. */
	debug_wait(1000);

	/* Write freq. before and now, together with calibration values to usart. */
	debug_int_to_string(string, 10, freq_before);
	debug_write_string(&usart_edbg, (uint8_t*)"Before: ");
	debug_write_string(&usart_edbg, string);
	debug_write_string(&usart_edbg, (uint8_t*)"\r\n");

	debug_int_to_string(string, 10, freq_best);
	debug_write_string(&usart_edbg, (uint8_t*)"Now: ");
	debug_write_string(&usart_edbg, string);
	debug_write_string(&usart_edbg, (uint8_t*)"\r\n");

	debug_int_to_string(string, 10, frange_best);
	debug_write_string(&usart_edbg, (uint8_t*)"Frequency range: ");
	debug_write_string(&usart_edbg, string);
	debug_write_string(&usart_edbg, (uint8_t*)"\r\n");

	debug_int_to_string(string, 10, comm_best  | 8 << 7);
	debug_write_string(&usart_edbg, (uint8_t*)"Calibration value: ");
	debug_write_string(&usart_edbg, string);
	debug_write_string(&usart_edbg, (uint8_t*)"\r\n");

	/* Deactivate tc modules. */
	tc_disable(&calib_chan);
	tc_disable(&comp_chan);


	while (1) {
		/* Inf loop */
	}
}
