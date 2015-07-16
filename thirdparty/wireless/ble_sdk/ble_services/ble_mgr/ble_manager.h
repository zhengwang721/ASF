/**
* \file
*
* \brief Ble Manager declarations
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

#ifndef __BLE_MANAGER_H__
#define __BLE_MANAGER_H__

#include <asf.h>
#include <string.h>
#include "stddef.h"
#include "at_ble_api.h"
#include "ble_utils.h"
#include "pxp_monitor.h"
#include "platform.h"

#define TX_POWER_SERVICE
#define LINK_LOSS_SERVICE
#define IMMEDIATE_ALERT_SERVICE

#define PRXIMITY_MONITOR
#define BLE_DEVICE_ROLE BLE_CENTRAL


/* Service UUID's */

/* Link Loss service UUID */
#define IMMEDIATE_ALERT_SERVICE_UUID	(0x1802)

/* Immediate Alert service UUID  */
#define LINK_LOSS_SERVICE_UUID			(0x1803)

/* Tx Power service UUID */
#define TX_POWER_SERVICE_UUID			(0x1804)


/* Characteristics UUID's */

/* Alert Level Characteristic UUID */
#define ALERT_LEVEL_CHAR_UUID			(0x2A06)

/* Tx Power Level Characteristic UUID */
#define TX_POWER_LEVEL_CHAR_UUID		(0x2A07)

#define BLE_CENTRAL						(0x01)
#define BLE_PERIPHERAL					(0x02)
#define BLE_CENTRAL_PERIPHERAL			(0x03)

/* BLE Event Get Timeout Value */
#define AT_BLE_EVENT_TIMEOUT			(-1)

typedef struct adv_element
{
	uint8_t len;
	uint8_t type;
	uint8_t *data;
}adv_element_t;

/**
* GATT characteristics handles
*/
typedef struct gatt_char_handler
{
	at_ble_handle_t start_handle;
	at_ble_handle_t end_handle;
	at_ble_handle_t char_handle;
	at_ble_status_t char_discovery;
	uint8_t *char_data;
}gatt_char_handler_t;


#if (BLE_DEVICE_ROLE == BLE_CENTRAL)

#define GAP_CONN_INTERVAL_MIN		(20)        //Connection interval min 20ms
#define GAP_CONN_INTERVAL_MAX		(40)		//Connection interval max 40ms
#define GAP_CONN_SLAVE_LATENCY		(0)
#define GAP_CE_LEN_MIN				(0)
#define GAP_CE_LEN_MAX				(0)
#define GAP_SUPERVISION_TIMOUT		(0x1f4)			// 500 for supervision timeout

#define MAX_SCAN_DEVICE				(10)			  //Max number of scan device @Todo parse when scan info is available
#define SCAN_INTERVAL				(48)              //Scan interval 30ms in term of 625us
#define SCAN_WINDOW					(48)              //Scan window 30ms values in term of 625ms
#define SCAN_TIMEOUT				(0x0000)          //Timeout  Scan time-out, 0x0000 disables time-out

#define GAP_CONNECT_PEER_COUNT		(1)

#define GATT_DISCOVERY_STARTING_HANDLE	(0x0001)
#define GATT_DISCOVERY_ENDING_HANDLE	(0xFFFF)

#endif

#if (BLE_DEVICE_ROLE == BLE_CENTRAL)
#define MAX_DEVICE_CONNECTED		(1)
#else
#define MAX_DEVICE_CONNECTED		(1)
#endif


static inline void ble_manager_dummy_handler(void *param)
{
	while(0);
}

#if (BLE_DEVICE_ROLE == BLE_CENTRAL)

#define BLE_SCAN_REPORT_HANDLER					ble_scan_report_handler
#define BLE_SCAN_INFO_HANDLER					ble_scan_info_handler

#if defined PROXIMITY_MONITOR
#define BLE_PROFILE_INIT						pxp_monitor_init
#define BLE_CONNECTED_STATE_HANDLER(param)		ble_connected_state_handler(param);\
												pxp_monitor_connected_state_handler(param);
												
#define BLE_DISCONNECTED_STATE_HANDLER(param)	ble_disconnected_state_handler(param);\
												pxp_disconnect_event_handler(param);
																								
#define BLE_PRIMARY_SERVICE_FOUND_HANDLER		pxp_monitor_service_found_handler
#define BLE_DISCOVERY_COMPLETE_HANDLER			pxp_monitor_discovery_complete_handler
#define	BLE_SCAN_DATA_HANDLER					pxp_monitor_scan_data_handler
#define BLE_CHARACTERISTIC_READ_RESPONSE		pxp_monitor_characteristic_read_response
#define BLE_CHARACTERISTIC_FOUND_HANDLER		pxp_monitor_characteristic_found_handler
#endif

#define BLE_CHARACTERISTIC_WRITE_RESPONSE		ble_manager_dummy_handler
#endif

#if (BLE_DEVICE_ROLE == BLE_CENTRAL)
at_ble_status_t gap_dev_connect(at_ble_addr_t dev_addr[]);
at_ble_status_t gap_dev_scan(void);
at_ble_status_t ble_scan_info_handler(at_ble_scan_info_t *scan_param);
at_ble_status_t ble_scan_report_handler(at_ble_scan_report_t *scan_report);
uint8_t scan_info_parse(at_ble_scan_info_t *scan_info_data, at_ble_uuid_t *ble_service_uuid, uint8_t adv_type);
void ble_characteristic_found_handler(at_ble_characteristic_found_t *characteristic_found);
#endif


at_ble_status_t ble_event_task(void);
void ble_device_init(at_ble_addr_t *addr);
void ble_event_manager(at_ble_events_t events , void *event_params);
void ble_discovery_complete_handler(at_ble_discovery_complete_t *discover_status);
void ble_disconnected_state_handler(at_ble_disconnected_t *disconnect);
void ble_connected_state_handler(at_ble_connected_t *conn_params);


#endif /*__BLE_MANAGER_H__*/