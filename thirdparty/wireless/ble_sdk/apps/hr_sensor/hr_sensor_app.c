/**
 * \file
 *
 * \brief Heart Rate Sensor Application
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
*							        Includes	
*                                       *
****************************************************************************************/
#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "console_serial.h"
#include "timer_hw.h"
#include "conf_extint.h"
#include "ble_manager.h"
#include "ble_utils.h"
#include "hr_sensor_app.h"
#include "hr_sensor.h"

/****************************************************************************************
*							        Globals		
*                                       *
****************************************************************************************/
volatile bool app_state = 0 ; /*!< flag to represent the application state*/

volatile bool start_advertisement = 0; /*!< flag to start advertisement*/

volatile bool notification_flag = false; /*!< flag to start notification*/

volatile bool disconnect_flag = false;	/*!< flag for disconnection*/

volatile bool hr_initializer_flag = 1; /*!< flag for initialization of hr for each category*/

uint8_t db_mem[1024];

uint8_t second_counter = 0;	/*!< second_counter to count the time*/

uint8_t activity = 0; /*!< activiy which will determine the */

uint16_t energy_expended_val = ENERGY_EXP_NORMAL; /*!< to count the energy expended*/

uint16_t energy_incrementor ;	/*!< energy incrementor for various heart rate values*/

uint16_t heart_rate_value = HEART_RATE_MIN_NORM; /*!< to count the heart rate value*/

uint16_t rr_interval_value = RR_VALUE_MIN; /*!< to count the rr interval value*/

bool inc_changer = true;/*!< to alter the direction of increments of hr*/

bool reverse = false;/*!< Used to change the hr zones in reverse order*/

bool notification_confirm_flag = true;
/****************************************************************************************
*							        Functions											*
****************************************************************************************/

/** @brief notification confirmation handler function called by the ble manager
 *	@param[in] at_ble_status_t gives the status notification has been sent in air.
 */
void app_notification_confirmation_handler(uint8_t status)
{
		if (status == AT_BLE_SUCCESS)
		{
			notification_confirm_flag = true;
		} 
}

/** @brief notification handler function called by the profile
 *	@param[in] notification_enable which will tell the state of the
 *application
 */
void app_notification_handler(uint8_t notification_enable)
{
	if (notification_enable == true) {
		DBG_LOG("Notification Enabled");
		hw_timer_start(NOTIFICATION_INTERVAL);
	} else {
		hw_timer_stop();
		notification_flag = false;
		DBG_LOG("Notification Disabled");
	}
}

/** @brief energy expended handler called by profile to reset the energy values
 *
 */

void app_reset_handler(void)
{
	energy_expended_val = 0;
	DBG_LOG("Energy Expended is made '0'on user Reset");
}

/** @brief app_state_handler which will tell the state of the application
 *  @param[in] status of the application
 */
void app_state_handler(bool state)
{
	app_state = state;
	if (app_state == false) {
		hw_timer_stop();
		notification_flag = false;
		LED_Off(LED0);
		DBG_LOG("Press button to advertise");
	} else if (app_state == true) {
		LED_On(LED0);
		DBG_LOG(
				"Enable the notification in app to listen heart rate or press the button to disconnect");
	}
}

/**
 * @brief Button Press Callback
 */
void button_cb(void)
{
	if (app_state) {
		DBG_LOG("Going to disconnect ");
		disconnect_flag = true;
	} else {
		DBG_LOG("Going to advertisement");
		start_advertisement = true;
	}
}

/** @brief hr_measurment_send sends the notifications after adding the hr values
 *	heart rate values starts @60bpm increments by 1 goes upto 255 bpm and
 *restarts @60
 *  Energy Expended will be sent on every 10th notification,it starts @ 0 and
 *increments by 20
 *  rr interval values, two rr interval values will be sent in every
 *notification
 */
bool hr_measurment_send(void)
{
	uint8_t hr_data[HR_CHAR_VALUE_LEN];
	uint8_t idx = 0;

	if (second_counter % 10) {
		/* Flags */
		hr_data[idx++] = (RR_INTERVAL_VALUE_PRESENT);
		DBG_LOG("Heart Rate: %d bpm", heart_rate_value);
		/* Heart Rate Value 8bit*/
		if (inc_changer) {
			hr_data[idx++] = (uint8_t)heart_rate_value++;
		} else {
			hr_data[idx++] = (uint8_t)heart_rate_value--;
		}

		if (energy_expended_val == ENERGY_RESET) {
			hr_data[0] = hr_data[0] | ENERGY_EXPENDED_FIELD_PRESENT;
			memcpy(&hr_data[idx], &energy_expended_val, 2);
			idx += 2;
		}

		/* RR Interval values(2)*/
		if (rr_interval_value < (uint16_t)RR_VALUE_MAX) {
			DBG_LOG_CONT("\tRR Values:(%d,%d)msec",
					rr_interval_value, rr_interval_value +
					200);
			memcpy(&hr_data[idx], &rr_interval_value, 2);
			idx += 2;
			rr_interval_value += 200;
			memcpy(&hr_data[idx], &rr_interval_value, 2);
			idx += 2;
			rr_interval_value += 200;
		} else {
			DBG_LOG_CONT("\tRR Values:(%d,%d) msec",
					rr_interval_value, rr_interval_value +
					200);
			rr_interval_value = (uint8_t)RR_VALUE_MIN;
			memcpy(&hr_data[idx], &rr_interval_value, 2);
			idx += 2;
			rr_interval_value += 200;
			memcpy(&hr_data[idx], &rr_interval_value, 2);
			idx += 2;
			rr_interval_value += 200;
		}

		if (energy_expended_val == ENERGY_RESET) {
			energy_expended_val += energy_incrementor;
			DBG_LOG("Energy Expended : 0 KJ");
		}
	} else {
		/* flags */
		hr_data[idx++]
			= (RR_INTERVAL_VALUE_PRESENT |
				ENERGY_EXPENDED_FIELD_PRESENT);

		/* Heart Rate Value 8bit*/
		DBG_LOG("Heart Rate: %d bpm", heart_rate_value);
		/* Heart Rate Value 8bit*/
		if (inc_changer) {
			hr_data[idx++] = (uint8_t)heart_rate_value++;
		} else {
			hr_data[idx++] = (uint8_t)heart_rate_value--;
		}

		memcpy(&hr_data[idx], &energy_expended_val, 2);
		idx += 2;
		energy_expended_val += energy_incrementor;

		/* RR Interval values(2) */
		if (rr_interval_value < (uint16_t)RR_VALUE_MAX) {
			DBG_LOG_CONT("\tRR Values:(%d,%d) msec",
					rr_interval_value, rr_interval_value +
					200);
			memcpy(&hr_data[idx], &rr_interval_value, 2);
			idx += 2;
			rr_interval_value += 200;
			memcpy(&hr_data[idx], &rr_interval_value, 2);
			idx += 2;
			rr_interval_value += 200;
		} else {
			DBG_LOG_CONT("\tRR Values:(%d,%d) msec",
					rr_interval_value, rr_interval_value +
					200);
			rr_interval_value = (uint8_t)RR_VALUE_MIN;
			memcpy(&hr_data[idx], &rr_interval_value, 2);
			idx += 2;
			rr_interval_value += 200;
			memcpy(&hr_data[idx], &rr_interval_value, 2);
			idx += 2;
			rr_interval_value += 200;
		}

		DBG_LOG("Energy Expended :%d KJ", energy_expended_val);
	}

	if (activity == 0) {
		DBG_LOG_CONT(" User Status:Idle");
	} else if (activity == 1) {
		DBG_LOG_CONT(" User Status:Walking");
	} else if (activity == 2) {
		DBG_LOG_CONT(" User status:Brisk walking");
	} else if (activity == 3) {
		DBG_LOG_CONT(" User status:Running");
	} else if (activity == 4) {
		DBG_LOG_CONT(" User Status:Fast Running");
	}

	if ((second_counter % 10) == 0) {
		DBG_LOG("\n");
	}

	return hr_sensor_send_notification(hr_data, idx);
}

/** @brief heart_rate_value_init will initializes the heart rate values
 *	 for simulation.
 *	 Based on the time different heart rate values are chosen to indicate
 *different activity.
 */
void heart_rate_value_init(void )
{
	if (second_counter < TIME_NORMAL_LIMIT) {
		if (hr_initializer_flag) {
			/** For incremental heart rate values*/
			if (!reverse) {
				activity = ACTIVITY_NORMAL;
				heart_rate_value = HEART_RATE_MIN_NORM;
				energy_incrementor = ENERGY_EXP_NORMAL;
				inc_changer = 1; /*ascending */
				hr_initializer_flag = false;
			} else {
				/** For Decremental heart rate values*/
				activity = ACTIVITY_FAST_RUNNING;
				heart_rate_value = HEART_RATE_MAX_FAST_RUNNING;
				energy_incrementor = ENERGY_EXP_FAST_RUNNING;
				inc_changer = false;        /* decending */
				hr_initializer_flag = false;
			}
		}

		if (second_counter == (TIME_NORMAL_LIMIT - 20)) {
			inc_changer = false;
			if (reverse) {
				inc_changer = true;
			}
		}

		if (second_counter == TIME_NORMAL_LIMIT - 1) {
			hr_initializer_flag = true;
		}
	} else if (second_counter >= TIME_NORMAL_LIMIT && second_counter <
			TIME_WALKING_LIMIT) {
		if (hr_initializer_flag) {
			if (!reverse) {
				activity = ACTIVITY_WALKING;
				heart_rate_value = HEART_RATE_MIN_WALKING;
				energy_incrementor = ENERGY_EXP_WALKING;
				hr_initializer_flag = false;
				inc_changer = 1;
			} else {
				activity = ACTIVITY_RUNNING;
				heart_rate_value = HEART_RATE_MAX_RUNNING;
				energy_incrementor = ENERGY_EXP_RUNNING;
				hr_initializer_flag = false;
				inc_changer = false;
			}
		}

		if (second_counter == TIME_WALKING_LIMIT - 20) {
			inc_changer = false;
			if (reverse) {
				inc_changer = true;
			}
		}

		if (second_counter == TIME_WALKING_LIMIT - 1) {
			hr_initializer_flag = true;
		}
	} else if (second_counter >= TIME_WALKING_LIMIT && second_counter <
			TIME_BRISK_WALK_LIMIT) {
		if (hr_initializer_flag) {
			if (!reverse) {
				activity = ACTIVITY_BRISK_WALKING;
				heart_rate_value = HEART_RATE_MIN_BRISK_WALK;
				energy_incrementor = ENERGY_EXP_BRISK_WALKING;
				hr_initializer_flag = false;
				inc_changer = 1;
			} else {
				activity = ACTIVITY_BRISK_WALKING;
				heart_rate_value = HEART_RATE_MIN_BRISK_WALK;
				energy_incrementor = ENERGY_EXP_BRISK_WALKING;
				hr_initializer_flag = false;
				inc_changer = false;
			}
		}

		if (second_counter == TIME_BRISK_WALK_LIMIT - 20) {
			inc_changer = false;
			if (reverse) {
				inc_changer = true;
			}
		}

		if (second_counter == TIME_BRISK_WALK_LIMIT - 1) {
			hr_initializer_flag = true;
		}
	} else if (second_counter >= TIME_BRISK_WALK_LIMIT && second_counter <
			TIME_RUNNING_LIMIT) {
		if (hr_initializer_flag) {
			if (!reverse) {
				activity = ACTIVITY_RUNNING;
				heart_rate_value = HEART_RATE_MIN_RUNNING;
				/* DBG_LOG("The user started Running"); */
				energy_incrementor = ENERGY_EXP_RUNNING;
				hr_initializer_flag = false;
				inc_changer = true;
			} else {
				activity = ACTIVITY_WALKING;
				heart_rate_value = HEART_RATE_MAX_WALKING;
				energy_incrementor = ENERGY_EXP_WALKING;
				hr_initializer_flag = false;
				inc_changer = false;
			}
		}

		if (second_counter == TIME_RUNNING_LIMIT - 20) {
			inc_changer = false;
			if (reverse) {
				inc_changer = true;
			}
		}

		if (second_counter == TIME_RUNNING_LIMIT - 1) {
			hr_initializer_flag = 1;
		}
	} else if (second_counter >= TIME_RUNNING_LIMIT && second_counter <
			TIME_FAST_RUNNING_LIMIT) {
		if (hr_initializer_flag) {
			if (!reverse) {
				activity = ACTIVITY_FAST_RUNNING;
				heart_rate_value = HEART_RATE_MIN_FAST_RUNNING;
				energy_incrementor = ENERGY_EXP_FAST_RUNNING;
				hr_initializer_flag = 0;
				inc_changer = 1;
			} else {
				activity = ACTIVITY_NORMAL;
				heart_rate_value = HEART_RATE_MAX_NORM;
				energy_incrementor = ENERGY_EXP_NORMAL;
				hr_initializer_flag = false;
				inc_changer = false;
			}
		}

		if (second_counter == TIME_FAST_RUNNING_LIMIT - 20) {
			inc_changer = false;
			if (reverse) {
				inc_changer = true;
			}
		}

		if (second_counter == TIME_FAST_RUNNING_LIMIT - 1) {
			hr_initializer_flag = true;
		}
	} else if (second_counter == TIME_FAST_RUNNING_LIMIT) {
		second_counter = true;
		hr_initializer_flag = true;
		reverse = !reverse;
	}
}

/**
 * \brief Timer callback handler called on timer expiry
 */
void timer_callback_handler(void)
{
	second_counter++;
	heart_rate_value_init();
	notification_flag = true;
}

/**
 * \brief Heart Rate Sensor Application main function
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

	/* Initialize the button */
	button_init();

	/* Initialize serial console */
	serial_console_init();

	/* Initialize the hardware timer */
	hw_timer_init();

	/* Register the callback */
	hw_timer_register_callback(timer_callback_handler);

	DBG_LOG("Initializing Heart Rate Sensor Application");

	/* initialize the ble chip  and Set the device mac address */
	ble_device_init(NULL,&pf_cfg);

	/* Registering the app_notification_handler with the profile */
	register_hr_notification_handler(app_notification_handler);

	/* Registering the app_reset_handler with the profile */
	register_hr_reset_handler(app_reset_handler);

	/* Registering the app_state_handler with the profile */
	register_hr_state_handler(app_state_handler);
	
	/* Registering the notification handler with the ble_manager*/
	register_ble_notification_confirmed_cb(app_notification_confirmation_handler);

	/* Capturing the events  */
	while (1) {
		ble_event_task();

		/* Flag to start advertisement */
		if (start_advertisement) {
			hr_sensor_adv();
			start_advertisement = false;
		}

		/* Flag to start notification */
		if (notification_flag) {
			if (notification_confirm_flag)
			{
				LED_Toggle(LED0);
				if (hr_measurment_send())
				{
					notification_confirm_flag = false;
				}
			}
		}

		/* Flag to disconnect with the peer device */
		if (disconnect_flag) {
			hr_sensor_disconnect();
			app_state = false;
			disconnect_flag = false;
		}
	}

	return 0;
}
