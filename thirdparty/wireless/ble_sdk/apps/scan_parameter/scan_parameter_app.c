/**
* \file
*
* \brief Scan Information Service - Application
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
#include "scan_param.h"
#include "ble_manager.h"
#include "scan_parameter_app.h"


/* === GLOBALS ============================================================ */

#define SCAN_PRAM_UPDATE_INTERVAL	(3) //1 second

/** @brief Scan response data*/
uint8_t scan_rsp_data[SCAN_RESP_LEN] = {0x09,0xff, 0x00, 0x06, 0xd6, 0xb2, 0xf0, 0x05, 0xf0, 0xf8};
sps_gatt_service_handler_t sps_service_handler;
uint16_t scan_interval_window;
uint8_t scan_refresh;


bool volatile timer_cb_done = false;
uint8_t scan_interval_value = 0;
at_ble_handle_t device_conn_handle; 

/**
* \Timer callback handler called on timer expiry
*/

void timer_callback_handler(void)
{
	//Timer call back
	timer_cb_done = true;
}

/**
* \Scan Parameter Application main function
*/

int main(void)
{
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
	
	/* initialize the ble chip  and Set the device mac address */
	ble_device_init(NULL);
	
	/* Initialize the battery service */
	sps_init_service(&sps_service_handler, &scan_interval_window, &scan_refresh);
	
	/* Define the primary service in the GATT server database */
	sps_primary_service_define(&sps_service_handler);
	
	DBG_LOG("Initializing Scan Parameter Application");
	
	sps_service_advertise();
	
	/* Register callback for paired event */
	register_ble_paired_event_cb(ble_paired_app_event);
	
	/* Register callback for disconnected event */
	register_ble_disconnected_event_cb(ble_disconnected_app_event);
	
	/* Capturing the events  */ 
	while (1) {
		/* BLE Event Task */
		ble_event_task();
		if (timer_cb_done)
		{
			timer_cb_done = false;
			LED_Toggle(LED0);
			scan_interval_value = (scan_interval_value % MAX_SPS_SCAN_REFRESH );
			/* send the notification and Update the scan parameter */	
			
			if(sps_scan_refresh_char_update(&sps_service_handler, scan_interval_value) == AT_BLE_SUCCESS)
			{
				DBG_LOG("Scan Interval Updated :%d", scan_interval_value);
			}
			scan_interval_value++;
		}
	}	
	return 0;
}

at_ble_status_t sps_service_advertise(void)
{
	uint8_t idx = 0;
	uint8_t adv_data [ SPS_ADV_DATA_NAME_LEN + SPS_ADV_DATA_UUID_LEN   + (2*2)];
	
	adv_data[idx++] = SPS_ADV_DATA_UUID_LEN + ADV_TYPE_LEN;
	adv_data[idx++] = SPS_ADV_DATA_UUID_TYPE;

	/* Appending the UUID */
	adv_data[idx++] = (uint8_t)SPS_SERVICE_UUID;
	adv_data[idx++] = (uint8_t)(SPS_SERVICE_UUID >> 8);
	
	//Appending the complete name to the Ad packet
	adv_data[idx++] = SPS_ADV_DATA_NAME_LEN + ADV_TYPE_LEN;
	adv_data[idx++] = SPS_ADV_DATA_NAME_TYPE;
	
	memcpy(&adv_data[idx], SPS_ADV_DATA_NAME_DATA, SPS_ADV_DATA_NAME_LEN );
	idx += SPS_ADV_DATA_NAME_LEN;
	
	/* Adding the advertisement data and scan response data */
	if(!(at_ble_adv_data_set(adv_data, idx, scan_rsp_data, SCAN_RESP_LEN) == AT_BLE_SUCCESS) )
	{
		DBG_LOG("Failed to set adv data");
	}
	
	/* Start of advertisement */
	if(at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY, APP_BAS_FAST_ADV, APP_BAS_ADV_TIMEOUT, 0) == AT_BLE_SUCCESS)
	{
		DBG_LOG("BLE Started Adv");
		return AT_BLE_SUCCESS;
	}
	else
	{
		DBG_LOG("BLE Adv start Failed");
	}
	return AT_BLE_FAILURE;
}

void ble_paired_app_event(at_ble_handle_t conn_handle)
{
	timer_cb_done = false;
	hw_timer_start(SCAN_PRAM_UPDATE_INTERVAL);
	device_conn_handle=conn_handle;
	LED_On(LED0);
}

void ble_disconnected_app_event(at_ble_handle_t conn_handle)
{
	timer_cb_done = false;
	hw_timer_stop();
	sps_service_advertise();
	LED_Off(LED0);
}