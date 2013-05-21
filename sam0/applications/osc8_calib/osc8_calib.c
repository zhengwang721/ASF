/**
 * \file
 *
 * \brief SAMD20 OSC8MHz Calibration Application
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

#define CALIBRATION_RESOLUTION 4096
#define REFERENCE_CLOCK_HZ     32768

static struct usart_module usart_edbg;
static struct tc_module tc_calib;
static struct tc_module tc_comp;

static void setup_tc_channels(void)
{
	struct tc_config config;
	tc_get_config_defaults(&config);

	config.counter_size    = TC_COUNTER_SIZE_32BIT;
	config.clock_prescaler = TC_CLOCK_PRESCALER_DIV1;
	config.wave_generation = TC_WAVE_GENERATION_NORMAL_FREQ;
	config.enable_capture_on_channel[0] = true;
	tc_init(&tc_calib, TC0, &config);

	config.enable_capture_on_channel[0] = false;
	config.counter_size = TC_COUNTER_SIZE_16BIT;
	config.clock_source = GCLK_GENERATOR_3;
	config.size_specific.size_16_bit.compare_capture_channel[0] = CALIBRATION_RESOLUTION;
	tc_init(&tc_comp, TC2, &config);
}

static void setup_tc_events(void)
{
	struct tc_events events;
	tc_get_events_config_default(&events);

	events.generate_event_on_compare_channel[0] = true;
	tc_enable_events(&tc_comp, &events);

	events.generate_event_on_compare_channel[0] = false;
	events.enable_incoming_events = true;
	tc_enable_events(&tc_calib, &events);

	tc_enable(&tc_calib);
	tc_enable(&tc_comp);
}

static void setup_events(void)
{
	struct events_chan_config evch_conf;
	events_chan_get_config_defaults(&evch_conf);

	evch_conf.edge_detection   = EVENT_EDGE_RISING;
	evch_conf.path             = EVENT_PATH_SYNCHRONOUS;
	evch_conf.generator_id     = EVSYS_ID_GEN_TC2_MCX_0;
	events_chan_set_config(EVENT_CHANNEL_0, &evch_conf);

	struct events_user_config evus_conf;
	events_user_get_config_defaults(&evus_conf);

	evus_conf.event_channel_id = EVENT_CHANNEL_0;
	events_user_set_config(EVSYS_ID_USER_TC0_EVU, &evus_conf);
}

static void setup_usart_channel(void)
{
	struct usart_config cdc_uart_config;
	usart_get_config_defaults(&cdc_uart_config);

	cdc_uart_config.mux_settings = USART_RX_3_TX_2_XCK_3;
	cdc_uart_config.pinout_pad3  = EDBG_CDC_RX_PINMUX;
	cdc_uart_config.pinout_pad2  = EDBG_CDC_TX_PINMUX;
	cdc_uart_config.baudrate     = 115200;
	stdio_serial_init(&usart_edbg, EDBG_CDC_MODULE, &cdc_uart_config);

	usart_enable(&usart_edbg);
	usart_enable_transceiver(&usart_edbg, USART_TRANSCEIVER_TX);
}

static void setup_clock_out_pin(void)
{
	struct system_pinmux_config pin_mux;
	system_pinmux_get_config_defaults(&pin_mux);

	pin_mux.mux_position = MUX_PA30H_GCLK_IO0;
	system_pinmux_pin_set_config(PIN_PA30H_GCLK_IO0, &pin_mux);
}

static uint32_t get_osc_frequency(void)
{
	tc_clear_status(&tc_comp, TC_STATUS_CHANNEL_0_MATCH);

	tc_start_counter(&tc_calib);
	tc_start_counter(&tc_comp);

	while (!(tc_get_status(&tc_comp) & TC_STATUS_CHANNEL_0_MATCH)) {
		/* Wait for channel 0 match */
	}

	uint64_t tmp = tc_get_capture_value(&tc_calib, TC_COMPARE_CAPTURE_CHANNEL_0);
	return ((tmp * REFERENCE_CLOCK_HZ) / CALIBRATION_RESOLUTION);
}

int main(void)
{
	system_init();
	events_init();
	delay_init();

	setup_tc_channels();
	setup_tc_events();
	setup_events();
	setup_clock_out_pin();

	uint16_t comm_best   = -1;
	uint8_t  frange_best = -1;
	uint32_t freq_best   = -1;
	uint32_t freq_before = get_osc_frequency();

	/* Run calibration */
	for (uint8_t frange_cal = 1; frange_cal <= 2; frange_cal++) {
		delay_ms(1);

		for (uint16_t comm_cal = 0; comm_cal < 128; comm_cal++) {
			system_clock_source_write_calibration(
					SYSTEM_CLOCK_SOURCE_OSC8M, comm_cal | (8 << 7), frange_cal);

			delay_cycles(1000);

			uint32_t freq_current = get_osc_frequency();
			if (abs(freq_current - 8000000UL) < abs(freq_best - 8000000UL))
			{
				freq_best   = freq_current;
				comm_best   = comm_cal;
				frange_best = frange_cal;

				port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
			}
			else
			{
				port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
			}
		}
	}

	/* Set the found best calibration. */
	system_clock_source_write_calibration(
			SYSTEM_CLOCK_SOURCE_OSC8M, comm_best | (8 << 7), frange_best);

	/* Setup USART module to give information back. */
	setup_usart_channel();

	/* Write freq. before and now, together with calibration values to usart. */
	printf("Freq Before: %lu\r\n", freq_before);
	printf("Freq Before: %lu\r\n", freq_best);

	printf("Freq Range: %u\r\n", frange_best);
	printf("Calib Value: %u\r\n", comm_best | 8 << 7);

	tc_disable(&tc_calib);
	tc_disable(&tc_comp);

	while (1) {
		port_pin_toggle_output_level(LED_0_PIN);
		delay_ms(200);
	}
}
