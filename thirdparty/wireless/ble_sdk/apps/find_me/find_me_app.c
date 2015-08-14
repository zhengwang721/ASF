/**
 * \file
 *
 * \brief Find Me Profile Application
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel
 *Support</a>
 */

/****************************************************************************************
*							        Includes	                                     	*
****************************************************************************************/

#include "console_serial.h"
#include "platform.h"
#include "timer_hw.h"

#include "ble_manager.h"
#include "immediate_alert.h"
#include "find_me_app.h"

/* === GLOBALS ============================================================ */

/* Timer application task flag */

volatile bool app_timer_done = false;

/** @brief Timer interval variable for applicaition task */
static uint8_t timer_interval = INIT_TIMER_INTERVAL;

uint8_t db_mem[1024];

/**
 * \brief Timer callback handler
 */
void timer_callback_handler(void)
{
	/* Stop the timer */
	hw_timer_stop();

	/* Enable the flag the serve the task */
	app_timer_done = true;
}

/**
 * @brief Alerting function on immediate alert
 *
 * @param[in] alert level of alert level characteristic of immediate alert
 * service
 *
 */
void app_immediate_alert(uint8_t alert_val)
{
	if (alert_val == IAS_HIGH_ALERT) {
		DBG_LOG("Find Me : High Alert");
		LED_On(LED0);
		timer_interval = LED_FAST_INTERVAL;
		hw_timer_start(timer_interval);
	} else if (alert_val == IAS_MID_ALERT) {
		DBG_LOG("Find Me : Mild Alert");
		LED_On(LED0);
		timer_interval = LED_MILD_INTERVAL;
		hw_timer_start(timer_interval);
	} else if (alert_val == IAS_NO_ALERT) {
		DBG_LOG("Find Me : No Alert");
		hw_timer_stop();
		LED_Off(LED0);
	}
}

/**
 * \brief Find Me Application main function
 */
int main(void)
{
		at_ble_init_config_t pf_cfg;
		platform_config busConfig;

		/*Memory allocation required by GATT Server DB*/
		pf_cfg.memPool.memSize = sizeof(db_mem);
		pf_cfg.memPool.memStartAdd = (uint8_t *)db_mem;
		/*Bus configuration*/
		busConfig.bus_type = UART;
		pf_cfg.plf_config = &busConfig;
		
	#if SAMG55
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
	#elif SAM0
	system_init();
	#endif

	/* Initialize serial console */
	serial_console_init();

	/* Initialize the hardware timer */
	hw_timer_init();

	/* Register the callback */
	hw_timer_register_callback(timer_callback_handler);

	DBG_LOG("Initializing Find Me Application");

	/* initialize the ble chip  and Set the device mac address */
	ble_device_init(NULL);

	/* callback registration for immediate alert value*/
	register_find_me_handler(app_immediate_alert);

	/* Capturing the events  */
	while (1) {
		/* BLE Event Task */
		ble_event_task();

		/* Application Task */
		if (app_timer_done) {
			LED_Toggle(LED0);
			hw_timer_start(timer_interval);
			app_timer_done = false;
		}
	}
}
