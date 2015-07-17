
/**
 * \file
 *
 * \brief Proximity Profile
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
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
*							        Includes	                                     							*
****************************************************************************************/

#include "pxp_reporter.h"
#include "timer_hw.h"
#include "tx_power.h"
#include "immediate_alert.h"
#include "link_loss.h"
#include "pxp_reporter_app.h"
#include "ble_manager.h"


/****************************************************************************************
*							        Macros	                                     							*
****************************************************************************************/

#define PATHLOSS

/****************************************************************************************
*							        Globals	                                     							*
****************************************************************************************/

gatt_service_handler_t txps_handle;
gatt_service_handler_t lls_handle;
gatt_service_handler_t ias_handle;

uint8_t scan_rsp_data[SCAN_RESP_LEN] = {0x09,0xff, 0x00, 0x06, 0xd6, 0xb2, 0xf0, 0x05, 0xf0, 0xf8};

uint8_t timer_interval = LL_INTERVAL_SLOW;
uint8_t pathloss_alert_value;
uint8_t linkloss_current_alert_level;


/****************************************************************************************
*							        Implementation	                                     							*
****************************************************************************************/

/**
 * \Initialisations of profile services
*/

void pxp_service_init(void)
{
	init_linkloss_service(&lls_handle);
	
	#if defined PATHLOSS
	
	init_immediate_alert_service(&ias_handle);
	init_tx_power_service(&txps_handle);
	
	#endif
}

/**
* \Definition of profile services to the attribute data base
*/

at_ble_status_t pxp_service_define (void)
{
	lls_primary_service_define(&lls_handle);
	
	#if defined PATHLOSS
	// need to define the return type and put it into console
	
	ias_primary_service_define(&ias_handle);
	txps_primary_service_define(&txps_handle);
	#endif
	
	DBG_LOG("The link loss handle is %x",lls_handle.serv_handle);
	
	#if defined PATHLOSS
	
	DBG_LOG("The immediate loss handle is %x",ias_handle.serv_handle);
	DBG_LOG("The tx power handle is %x",txps_handle.serv_handle);
	
	#endif
	
	return AT_BLE_SUCCESS;
}

/**
* \Service Characteristic change handler function 
*/


at_ble_status_t pxp_reporter_char_changed_handler(at_ble_characteristic_changed_t *char_handle)
{
	at_ble_characteristic_changed_t change_params;
	uint32_t i = 0;

	memcpy((uint8_t *)&change_params, char_handle, sizeof(at_ble_characteristic_changed_t));

	DBG_LOG("Characteristic 0x%x changed, new_value = ", change_params.char_handle);
	for(i=0; i<change_params.char_len; i++)
	DBG_LOG("0x%02x ", change_params.char_new_value[i]);



	if (change_params.char_handle == lls_handle.serv_chars.char_val_handle)
	{
		linkloss_current_alert_level = change_params.char_new_value[0];
		DBG_LOG("The current alert level for linkloss is %x",linkloss_current_alert_level);
	}
	
	else if (change_params.char_handle == ias_handle.serv_chars.char_val_handle)
	{
		pathloss_alert_value = change_params.char_new_value[0];
		DBG_LOG("There is pathloss current alert level is %x",pathloss_alert_value);
		if (pathloss_alert_value == 2)
		{
			DBG_LOG("Pathloss : High Alert");
			timer_interval = PL_INTERVAL_FAST;
			LED_On(LED0);
			hw_timer_start(timer_interval);
		}

		else if (pathloss_alert_value == 1)
		{
			DBG_LOG("Pathloss : Mid Alert");
			timer_interval = PL_INTERVAL_MEDIUM;
			LED_On(LED0);
			hw_timer_start(timer_interval);
		}

		else if (pathloss_alert_value == 0)
		{
			DBG_LOG("Pathloss : Low Alert");
			hw_timer_stop();
			LED_Off(LED0);
			led_state = 0;
		}
	}
	return AT_BLE_SUCCESS;
}

/**
* \Pxp reporter connected state handler function called after
*/

at_ble_status_t pxp_reporter_connected_state_handler (at_ble_connected_t *conn_params)
{
	at_ble_status_t status;
	hw_timer_stop();
	LED_Off(LED0);
	led_state = 0;
	
	if ( (status = at_ble_tx_power_set(conn_params->handle,DEFAULT_TX_PWR_VALUE)) != AT_BLE_SUCCESS) 
	{
		DBG_LOG("Setting tx power value failed:reason %x",status);
	}
	
	return AT_BLE_SUCCESS;
}

/**
* \Pxp reporter disconnected state handler function called after
*/

at_ble_status_t pxp_disconnect_event_handler(at_ble_disconnected_t *disconnect)
{
	if(at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY,
	APP_PXP_FAST_ADV, APP_PXP_ADV_TIMEOUT, 0) != AT_BLE_SUCCESS)
	{
		#ifdef DBG_LOG
		DBG_LOG("BLE Adv start Failed");
		#endif
	}
	else
	{
		DBG_LOG("BLE Started Adv");
	}
	
	if (linkloss_current_alert_level == 0)
	{
		DBG_LOG("Link loss : Low Alert  ");
		timer_interval = LL_INTERVAL_SLOW;
		LED_On(LED0);
		hw_timer_start(timer_interval);
	}
	else if (linkloss_current_alert_level == 1)
	{
		DBG_LOG("Link loss : Mid Alert  ");
		timer_interval = LL_INTERVAL_MEDIUM;
		LED_On(LED0);
		hw_timer_start(timer_interval);
	}
	else if (linkloss_current_alert_level == 2)
	{
		DBG_LOG("Link loss : High Alert ");
		timer_interval = LL_INTERVAL_FAST;
		LED_On(LED0);
		hw_timer_start(timer_interval);
	}
	return AT_BLE_SUCCESS;
}

/**
* \Pxp reporter advertisement initialization and adv start 
*/

void pxp_reporter_adv(void)
{
	uint8_t idx = 0;
	uint8_t adv_data [ PXP_ADV_DATA_NAME_LEN + LL_ADV_DATA_UUID_LEN   + (2*2)];
	
	adv_data[idx++] = LL_ADV_DATA_UUID_LEN + ADV_TYPE_LEN +  TXP_ADV_DATA_UUID_LEN + IAL_ADV_DATA_UUID_LEN ;
	adv_data[idx++] = LL_ADV_DATA_UUID_TYPE;
	
	//Appending the UUID
	memcpy(&adv_data[idx], LL_ADV_DATA_UUID_DATA, LL_ADV_DATA_UUID_LEN);
	idx += LL_ADV_DATA_UUID_LEN;
	
	//Prepare ADV Data for TXP Service
	memcpy(&adv_data[idx], TXP_ADV_DATA_UUID_DATA, TXP_ADV_DATA_UUID_LEN);
	idx += TXP_ADV_DATA_UUID_LEN;
	
	//Prepare ADV Data for IAS Service
	memcpy(&adv_data[idx], IAL_ADV_DATA_UUID_DATA, IAL_ADV_DATA_UUID_LEN);
	idx += IAL_ADV_DATA_UUID_LEN;
	
	//Appending the complete name to the Ad packet
	adv_data[idx++] = PXP_ADV_DATA_NAME_LEN + ADV_TYPE_LEN;
	adv_data[idx++] = PXP_ADV_DATA_NAME_TYPE;
	memcpy(&adv_data[idx], PXP_ADV_DATA_NAME_DATA, PXP_ADV_DATA_NAME_LEN );
	idx += PXP_ADV_DATA_NAME_LEN ;
	
	
	if(!(at_ble_adv_data_set(adv_data, idx, scan_rsp_data, SCAN_RESP_LEN) == AT_BLE_SUCCESS) )
	{
		#ifdef DBG_LOG
		DBG_LOG("failed adv data set");
		#endif
	}
	
	if(at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY, APP_PXP_FAST_ADV, APP_PXP_ADV_TIMEOUT, 0) == AT_BLE_SUCCESS)
	{
		#ifdef DBG_LOG
		DBG_LOG("BLE Started Adv");
		#endif
	}
	
	else
	{
		#ifdef DBG_LOG
		DBG_LOG("BLE Adv start Failed");
		#endif
	}
}

/**
* \Pxp reporter Initialization which initializes service,defines and start adv
*/

void pxp_app_init(void)
{
	pxp_service_init();
	
	pxp_service_define();
	
	pxp_reporter_adv();
}
