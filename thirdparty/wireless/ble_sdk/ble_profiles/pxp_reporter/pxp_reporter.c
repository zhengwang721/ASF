
/**
 * \file
 *
 * \brief Proximity Reporter Profile
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
*							        Includes	                                     							*
****************************************************************************************/
#include <asf.h>
#include <string.h>
#include "timer_hw.h"
#include "at_ble_api.h"
#include "ble_manager.h"
#include "pxp_reporter_app.h"
#include "pxp_reporter.h"
#include "immediate_alert.h"
#include "link_loss.h"
#include "tx_power.h"

#include "ble_utils.h"

/****************************************************************************************
*							        Macros	                                     		*
****************************************************************************************/

/****************************************************************************************
*							        Globals	                                     		*
****************************************************************************************/
#ifdef LINK_LOSS_SERVICE
gatt_service_handler_t txps_handle;
#endif // LINK_LOSS_SERVICE

#ifdef TX_POWER_SERVICE
gatt_service_handler_t lls_handle;
#endif // TX_POWER_SERVICE

#ifdef IMMEDIATE_ALERT_SERVICE
gatt_service_handler_t ias_handle;
#endif // IMMEDIATE_ALER_SERVICE


/** @brief led_state  the state of the led*/
extern uint8_t pxp_led_state;

/** @brief Scan response data*/
uint8_t scan_rsp_data[SCAN_RESP_LEN] = {0x09,0xff, 0x00, 0x06, 0xd6, 0xb2, 0xf0, 0x05, 0xf0, 0xf8};


/** @brief Alert value used for immediate alert service helps in pathloss */
uint8_t pathloss_alert_value = INVALID_IAS_PARAM ;

/** @brief Alert value used for Linkloss service*/
uint8_t linkloss_current_alert_level ;

reporter_callback_t pathloss_cb;
reporter_callback_t linkloss_cb;

/****************************************************************************************
*							        Implementation	                                     							*
****************************************************************************************/

/**
 * \Initializations of profile services based on pathloss option
*/

void pxp_service_init(void)
{
	init_linkloss_service(&lls_handle);
	
	#if defined PATHLOSS	
	init_immediate_alert_service(&ias_handle);
	init_tx_power_service(&txps_handle);	
	#endif
}

void register_pathloss_handler(reporter_callback_t pathloss_fn)
{
	pathloss_cb = pathloss_fn;
}

void register_linkloss_handler(reporter_callback_t linkloss_fn)
{
	linkloss_cb = linkloss_fn;
}

/**
* \Definition of profile services to the attribute data base based on pathloss
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
	
	uint32_t index = 0;

	memcpy((uint8_t *)&change_params, char_handle, sizeof(at_ble_characteristic_changed_t));

	DBG_LOG("Characteristic 0x%x changed, new_value = ", change_params.char_handle);
	for(index=0; index<change_params.char_len; index++)
	{
		DBG_LOG_CONT("0x%02x ", change_params.char_new_value[index]);
	}	

	linkloss_current_alert_level = lls_set_alert_value(&change_params,&lls_handle);
	
	pathloss_alert_value		 = ias_set_alert_value(&change_params,&ias_handle);
	
	if (pathloss_alert_value != INVALID_IAS_PARAM)
	{
		pathloss_cb(pathloss_alert_value);
		
	}
	
	return AT_BLE_SUCCESS;
}

/**
* \Pxp reporter connected state handler function called after
*/
at_ble_status_t pxp_reporter_connected_state_handler(at_ble_connected_t *conn_params)
{

		at_ble_status_t status;
	hw_timer_stop();
	LED_Off(LED0);
	pxp_led_state = 0;
	if ( (status = at_ble_tx_power_set(conn_params->handle, DEFAULT_TX_PWR_VALUE)) != AT_BLE_SUCCESS) 
	{
		DBG_LOG("Setting tx power value failed:reason %x",status);
		return AT_BLE_FAILURE;
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
	
	//DBG_LOG("The linkloss value is %d",linkloss_current_alert_level);
	linkloss_cb(linkloss_current_alert_level);

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

#ifdef LINK_LOSS_SERVICE	
	/* Appending the UUID */
	adv_data[idx++] = (uint8_t)LINK_LOSS_SERVICE_UUID;
	adv_data[idx++] = (uint8_t)(LINK_LOSS_SERVICE_UUID >> 8);
#endif	//LINK_LOSS_SERVICE

#ifdef TX_POWER_SERVICE
	//Prepare ADV Data for TXP Service
	adv_data[idx++] = (uint8_t)TX_POWER_SERVICE_UUID;
	adv_data[idx++] = (uint8_t)(TX_POWER_SERVICE_UUID >> 8);
#endif // TX_POWER_SERVICE	

#ifdef IMMEDIATE_ALERT_SERVICE
	//Prepare ADV Data for IAS Service
	adv_data[idx++] = (uint8_t)IMMEDIATE_ALERT_SERVICE_UUID;
	adv_data[idx++] = (uint8_t)(IMMEDIATE_ALERT_SERVICE_UUID >> 8);
#endif	
	
	//Appending the complete name to the Ad packet
	adv_data[idx++] = PXP_ADV_DATA_NAME_LEN + ADV_TYPE_LEN;
	adv_data[idx++] = PXP_ADV_DATA_NAME_TYPE;
	
	memcpy(&adv_data[idx], PXP_ADV_DATA_NAME_DATA, PXP_ADV_DATA_NAME_LEN );
	idx += PXP_ADV_DATA_NAME_LEN ;
	
	/* Adding the advertisement data and scan response data */
	if(!(at_ble_adv_data_set(adv_data, idx, scan_rsp_data, SCAN_RESP_LEN) == AT_BLE_SUCCESS) )
	{
		#ifdef DBG_LOG
		DBG_LOG("Failed to set adv data");
		#endif
	}
	
	/* Start of advertisement */
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

void pxp_reporter_init(void *param)
{
	/* pxp services initialization*/
	pxp_service_init();
	
	/* pxp services definition    */
	pxp_service_define();
	
	/* pxp services advertisement */
	pxp_reporter_adv();
}
