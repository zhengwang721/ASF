/**
* \file
*
* \brief Alert Notification Application
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
* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
*/

/****************************************************************************************
*							        Includes	                                     	*
****************************************************************************************/


#include <asf.h>
#include "console_serial.h"
#include "at_ble_api.h"
#include "platform.h"
#include "timer_hw.h"
#include "ble_utils.h"
#include "ble_manager.h"
#include "alert_notification.h"
#include "alert_notification_profile.h"
#include "button.h"
#include "led.h"

bool user_request = false;

bool notification_enable = false;

/***********************************************************************************
 *									Implementations                               *
 **********************************************************************************/
/**
 * @brief Button Press Callback 
 */
static void button_cb(void)
{
	user_request = true;
	send_plf_int_msg_ind(USER_TIMER_CALLBACK,TIMER_EXPIRED_CALLBACK_TYPE_DETECT,NULL,0);
}

/**
 * @brief Timer Callback
 */
static void timer_callback_handler(void)
{
	/* Free to use for User Application */
}

/**
 * @brief app_connected_state profile notifies the application about state
 * @param[in] connected
 */
static void app_connected_state(bool connected)
{
	if (connected) {
		DBG_LOG("App connected");	
	}		
}

int main(void)
{
	#if SAMG55
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
	#elif SAM0
	system_init();
	#endif
	
	user_request = false;
	notification_enable = false;
	
	/* Initialize serial console */
	serial_console_init();

	DBG_LOG("Alert Notification Profile Application");
	
	/* Initializing the hardware timer */
	hw_timer_init();

	/*Registration of timer callback*/
	hw_timer_register_callback(timer_callback_handler);
	
	/* initialize the ble chip  and Set the device mac address */
	ble_device_init(NULL);

	/* Initializing the button */
	/* Caution, button_init func has to be called after ble_device_init func */
	button_init(button_cb);

	led_init();

	/* Register the connected call back with the profile */
	register_connected_callback(app_connected_state);
	
	/* Capturing the events  */
	while(1) {
		/* BLE Event Task */
		ble_event_task();

		if (user_request) {
			if (notification_enable) {
				anp_client_write_notification_handler();
				notification_enable = false;
			} else {
				anp_client_disable_notification();
				notification_enable = true;
			}
			user_request = false;
		}
	}
}
