/**
* \file
*
* \brief Phone Alert Status Application
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
#include "at_ble_trace.h"
#include "at_ble_errno.h"
#include "platform.h"
#include "console_serial.h"
#include "timer_hw.h"
#include "conf_extint.h"
#include "ble_manager.h"
#include "ble_utils.h"
#include "pas_client.h"
#include "pas_app.h"


volatile bool button_pressed = false;

volatile uint8_t press;

volatile bool flag;

volatile bool app_state;

/***********************************************************************************
 *									Implementations                               *
 **********************************************************************************/

/**
 * @brief app_connected_state profile notifies the application about state
 * @param[in] connected
 */
void app_connected_state(bool connected)
{
	app_state = connected;
	if (app_state == false)
	{
		DBG_LOG("App disconnected");
		press = 0;
	}
}

/**
 * @brief displays alerts status value came in read response
 * @param[in] data
 * @param[in] len  
 */
void app_alert_status_read(uint8_t *data, uint8_t len)
{
	uint8_t idx;
	
	DBG_LOG("read alert status value is");
	for (idx = 0; idx < len; idx++)
	{
		DBG_LOG_CONT("%d ",data[idx]);
	}
}

/**
 * @brief displays ringer setting value came in read response
 * @param[in] data
 * @param[in] len  
 */
void app_ringer_setting_read(uint8_t *data, uint8_t len)
{
	uint8_t idx;
	DBG_LOG("read ringer setting value is");
	for (idx = 0; idx < len; idx++)
	{
		DBG_LOG_CONT("%d ",data[idx]);
	}
}

/**
 * @brief displays alert status value came in notification
 * @param[in] data
 * @param[in] len  
 */
void app_alert_status_notify(uint8_t *data, uint8_t len)
{
	uint8_t idx;
	DBG_LOG("notified alert status value is");
	for (idx = 0; idx < len; idx++)
	{
		DBG_LOG_CONT("%d ",data[idx]);
	}
}

/**
 * @brief displays ringer setting value came in notification
 * @param[in] data
 * @param[in] len  
 */
void app_ringer_setting_notify(uint8_t *data, uint8_t len)
{
	uint8_t idx;
	DBG_LOG("notified alert status value is");
	for (idx = 0; idx < len; idx++)
	{
		DBG_LOG_CONT("%d ",data[idx]);
	}
}

/**
 * @brief Button Press Callback 
 */
void button_cb(void)
{
	if (button_pressed == false)
	{
		 button_pressed = true;
		 return;
	}
	if (app_state)
	{
		DBG_LOG("button Pressed");
		flag = 1;
		press += 1;
	}
}

/**
 * @brief Timer Callback
 */
void timer_callback_handler(void)
{
	/* Free to use for User Application */
}


int main(void)
{	
	at_ble_status_t status;
	#if SAMG55
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
	#elif SAM0
	system_init();
	#endif
	
	/* Initialize serial console */
	serial_console_init();
	
	/* Initializing the button */
	button_init();
	
	/* Initializing the hardware timer */
	hw_timer_init();
	
	/*Registration of timer callback*/
	hw_timer_register_callback(timer_callback_handler);
	
	DBG_LOG("Initializing Phone Alert Status Profile Application");
	
	register_alert_status_read_callback(app_alert_status_read);
	
	register_ringer_setting_read_callback(app_ringer_setting_read);
	
	register_alert_status_notification_callback(app_alert_status_notify);
	
	register_ringer_setting_notification_callback(app_ringer_setting_notify);
	
	register_connected_callback(app_connected_state);
	
	/* initialize the ble chip  and Set the device mac address */
	ble_device_init(NULL);
	
	/* Capturing the events  */
	while(1)
	{
		/* BLE Event Task */
		ble_event_task();
		
		if (press == 1)
		{
			if (flag)
			{
				DBG_LOG("Device to silent");
				pas_client_write_ringer_control_point(1);
				flag = 0;
			}
			
		} else if (press == 2) {
			if (flag)
			{
				DBG_LOG("Device to Mute Once");
				pas_client_write_ringer_control_point(2);
				flag = 0;
			}
		} else if (press == 3) {
			if (flag)
			{
				DBG_LOG("Device to cancel mute");
				pas_client_write_ringer_control_point(3);
				flag = 0;
			}
		} else if (press == 4) {
			if (flag)
			{
				DBG_LOG("reading the alert status and ringer setting");			
				if ((status = pas_client_read_ringer_setting_char()) != AT_BLE_SUCCESS)
					{
						DBG_LOG("reading ringer control point invocation failed");
					}
						
				if ((status = pas_client_read_alert_status_char()) != AT_BLE_SUCCESS)
					{
						DBG_LOG("reading alert status invocation failed");
					}
					press = 0;
			}
		}
	}
	return 0;
}
