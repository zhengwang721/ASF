/**
 * \file
 *
 * \brief SAM Watchdog Driver Quick Start for SAMB
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

//! [module_inst]
struct wdt_module wdt_instance;
//! [module_inst]

//! [setup]
static void configure_gpio(void)
{
	struct gpio_config config_gpio;

	gpio_get_config_defaults(&config_gpio);
	
	gpio_pin_set_config(BUTTON_0_PIN, &config_gpio);

	config_gpio.direction  = GPIO_PIN_DIR_OUTPUT;
	config_gpio.input_pull = GPIO_PIN_PULL_NONE;
	gpio_pin_set_config(LED_0_PIN, &config_gpio);
}

static void configure_wdt(void)
{
	/* Create a new configuration structure for the Watchdog settings and fill
	 * with the default module settings. */
	//! [setup_1]
	struct wdt_config config_wdt;
	//! [setup_1]
	//! [setup_2]
	wdt_get_config_defaults(&config_wdt);
	//! [setup_2]
	//! [setup_3]
	config_wdt.load_value = CONF_WDT_LOAD_VALUE;
	//! [setup_3]

	/* Initialize and enable the Watchdog with the user settings */
	//! [setup_4]
	wdt_set_config(&wdt_instance, CONF_WDT_MODULE, &config_wdt);
	//! [setup_4]
}
//! [setup]

int main(void)
{
	uint32_t current_value;
	uint16_t i;

	//system_init();

	//! [setup_init]
	configure_gpio();
	configure_wdt();
	//! [setup_init]


	//! [main]
	//! [main_1]
	for(i = 0; i < 5; i++) {
		gpio_pin_toggle_output_level(LED_0_PIN);
	}
	gpio_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
	//! [main_1]
	//! [main_2]
	while (true) {
	//! [main_2]

		//! [main_3]
		if (gpio_pin_get_input_level(BUTTON_0_PIN) == false) {
		//! [main_3]
		//! [main_4]
			wdt_get_current_count(&wdt_instance, &current_value);
		//! [main_4]
		//! [main_5]
			wdt_set_reload_count(&wdt_instance, CONF_WDT_LOAD_VALUE);
		//! [main_5]
		}
	}
	//! [main]
}
