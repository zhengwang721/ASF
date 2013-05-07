/**
 * \file
 *
 * \brief SAM D20 ADC Quick Start
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

struct adc_module module_inst;

/* Flag to indicate if a low-voltage situation has occured */
volatile bool low_voltage = false;

/* ADC Window callback; called whenever the supply voltage drops
 * below ~1.7 Volts */
static void adc_window_callback(
	const struct adc_module *const module)
{
	/* Signal the application that the voltage has gone below the
	threshold */
	low_voltage = true;
}

/* Setup RTC as ADC sample timer */
static void rtc_setup(void) {
	struct rtc_count_config conf;
	
	rtc_count_get_config_defaults(&conf);

	conf.mode              = RTC_COUNT_MODE_32BIT;
	conf.clear_on_match    = true;
	conf.compare_values[0] = 1000;
	
	struct rtc_count_events evconfig; 
	evconfig.generate_event_on_compare[0] = true;
	
	rtc_count_init(&conf);
	rtc_count_enable_events(&evconfig);
	rtc_count_enable();
	
}

/* Setup the event system to route RTC events to the ADC */
static void event_setup(void) {
	struct events_chan_config chan_config;
	struct events_user_config user_config;
	
	events_chan_get_config_defaults(&chan_config);
	events_user_get_config_defaults(&user_config);
	
	events_init();
	
	
	/* Setup event channel 0 with RTC compare 0 as input */
	chan_config.generator_id = EVSYS_ID_GEN_RTC_CMP_0;
	chan_config.path = EVENT_PATH_ASYNCHRONOUS;
	events_chan_set_config(EVENT_CHANNEL_0, &chan_config);
	
	
	/* Setup ADC to listen to event channel 0 */
	user_config.event_channel_id = EVENT_CHANNEL_0;
	events_user_set_config(EVSYS_ID_USER_ADC_START, &user_config);
		
}

/* Setup the ADC to sample the internal scaled VCC */
static void adc_setup(void) {
	struct adc_config config;

	adc_get_config_defaults(&config);

	config.gain_factor        = ADC_GAIN_FACTOR_1X;
	/* Use GCLK generator 4 as clock source */
	config.clock_source       = GCLK_GENERATOR_4;
	/* Divide input clock by 4 (8MHz / 4 = 2MHz) */
	config.clock_prescaler    = ADC_CLOCK_PRESCALER_DIV4;
	/* Use internal 1V bandgap reference */
	config.reference          = ADC_REFERENCE_INT1V;
	/* Use VCC divided by 4 as positive input */
	config.positive_input     = ADC_POSITIVE_INPUT_SCALEDIOVCC;
	/* Start new conversion on event */
	config.event.event_action = ADC_EVENT_ACTION_START_CONV;

	/* Enable the ADC to run in standby sleep mode */
	config.run_in_standby     = true;
	
	/* Average 16 samples in hardware (accmulate 16 samples and divide by 16) */
	config.resolution         = ADC_RESOLUTION_CUSTOM;
	config.accumulate_samples = ADC_ACCUMULATE_SAMPLES_16;
	config.divide_result      = ADC_DIVIDE_RESULT_16;
	
	/* Configure window */
	config.window.window_mode        = ADC_WINDOW_MODE_BELOW_UPPER;
	/* (1.0V / 4095 * 1740) * 4 = 1.7V */ 
	config.window.window_upper_value = 1740;
	
	
	adc_init(&module_inst, ADC, &config);

	adc_enable(&module_inst);

	/* Register and enable the window callback */
	adc_register_callback(&module_inst, adc_window_callback, ADC_CALLBACK_WINDOW);
	adc_enable_callback(&module_inst, ADC_CALLBACK_WINDOW);

}


static void gclk_output(void)
{
	struct system_pinmux_config pin_conf;
	system_pinmux_get_config_defaults(&pin_conf);
	pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
	pin_conf.mux_position = MUX_PA10H_GCLK_IO4;
	system_pinmux_pin_set_config(PIN_PA10H_GCLK_IO4, &pin_conf);
	
}


int main(void)
{
	volatile uint32_t delay = 0xFFF;
	
	system_init();
	
	rtc_setup();
	event_setup();
	adc_setup();
	gclk_output();
	
	system_interrupt_enable_global();
	system_set_sleepmode(SYSTEM_SLEEPMODE_STANDBY);

	system_sleep();	
	
	while (true) {
		if(low_voltage == true) {
			while(1) {
				port_pin_toggle_output_level(LED0_PIN);
				while(delay--){};
				delay = 0xFFFF;
			}
		}
		/* Infinite loop */
	}

}
