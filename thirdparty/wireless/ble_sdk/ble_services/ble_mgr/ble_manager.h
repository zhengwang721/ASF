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
#include <stddef.h>
#include "at_ble_api.h"
#include "ble_utils.h"
#include "platform.h"

#if defined PROXIMITY_REPORTER
#include "pxp_reporter.h"
#endif //PROXIMITY_REPORTER

#if defined PROXIMITY_MONITOR
#include "pxp_monitor.h"
#endif //PROXIMITY_MONITOR

#if (BLE_DEVICE_ROLE == BLE_OBSERVER)
#include "ble_observer.h"
#endif //(BLE_DEVICE_ROLE == BLE_OBSERVER)

#define BLE_DEVICE_NAME				"ATMEL-DEV"

#define BLE_EVENT_TIMEOUT			(-1)

static inline void ble_dummy_handler(void *param)
{
	UNUSED(param);
}

#if ((BLE_DEVICE_ROLE == BLE_CENTRAL) || (BLE_DEVICE_ROLE == BLE_OBSERVER) || (BLE_DEVICE_ROLE == BLE_CENTRAL_AND_PERIPHERAL)) 
#define MAX_SCAN_DEVICE				(10)			  //Max number of scan device
#define SCAN_INTERVAL				(96)              //Scan interval 30ms in term of 625us
#define SCAN_WINDOW					(96)              //Scan window 30ms values in term of 625ms
#define SCAN_TIMEOUT				(0x0000)          //Timeout  Scan time-out, 0x0000 disables time-out
#endif //((BLE_DEVICE_ROLE == BLE_CENTRAL) || (BLE_DEVICE_ROLE == BLE_OBSERVER) || (BLE_DEVICE_ROLE == BLE_CENTRAL_AND_PERIPHERAL)) 


/* Observer related declarations */
#define LE_LIMITED_DISCOVERABLE_MODE  ((uint8_t) 1 << 0)
#define LE_GENERAL_DISCOVERABLE_MODE  ((uint8_t) 1 << 1)
#define BREDR_NOT_SUPPORTED			  ((uint8_t) 1 << 2)
#define	LE_BREDR_CAPABLE_CONTROLLER   ((uint8_t) 1 << 3)
#define LE_BREDR_CAPABLE_HOST		  ((uint8_t) 1 << 4)

/*Length of Adv data types*/
#define APPEARANCE_SIZE					2
#define TX_POWER_LEVEL_SIZE				1
#define ADV_INTERVAL_SIZE				2

/* Gap Advertisement Types */
typedef enum
{
	FLAGS = 0x01,
	INCOMPLETE_LIST_16BIT_SERV_UUIDS,
	COMPLETE_LIST_16BIT_SERV_UUIDS,
	INCOMPLETE_LIST_32BIT_SERV_UUIDS,
	COMPLETE_LIST_32BIT_SERV_UUIDS,
	INCOMPLETE_LIST_128BIT_SERV_UUIDS,
	COMPLETE_LIST_128BIT_SERV_UUIDS,
	SHORTENED_LOCAL_NAME,
	COMPLETE_LOCAL_NAME,
	TX_POWER_LEVEL,
	CLASS_OF_DEVICE = 0x0D,
	SIMPLE_PAIRING_HASHING,
	SIMPLE_PAIRING_RANDOMIZER,
	DEVICE_ID,
	SECURITY_MANAGER_OOB_FLAGS,
	SLAVE_CONNECTION_INTERVAL_RANGE,
	LIST_16BIT_SERV_SOLICITATION_UUIDS = 0x14,
	LIST_128BIT_SERV_SOLICITATION_UUIDS,
	SERVICE_DATA,
	PUBLIC_TARGET_ADDRESS,
	RANDOM_TARGET_ADDRESS,
	APPEARANCE,
	ADVERTISING_INTERVAL,
	LE_BLUETOOTH_DEVICE_ADDRESS,
	LE_ROLE,
	SIMPLE_PAIRING_HASHING_C256,
	SIMPLE_PAIRING_RANDOMIZER_R256,
	SERVICE_DATA_32BIT = 0x20,
	SERVICE_DATA_128BIT,
	LE_SECURE_CONNECTIONS_CONFIRMATION_VALUE,
	LE_SECURE_CONNECTIONS_RANDOM_VALUE,
	THREED_INFORMATION_DATA = 0x3D,
	MANUFACTURER_SPECIFIC_DATA = 0xFF
}gap_ad_type;

#if (BLE_DEVICE_ROLE == BLE_OBSERVER)
#define BLE_SCAN_REPORT_HANDLER								ble_scan_report_handler
#define BLE_SCAN_INFO_HANDLER(param)						ble_observer_scan_info_handler(param);\
															ble_scan_info_handler(param);
															
#define	BLE_SCAN_DATA_HANDLER								ble_observer_scan_data_handler
#endif //(BLE_DEVICE_ROLE == BLE_OBSERVER)

/* Service UUID's */

/* Link Loss service UUID */
#define LINK_LOSS_SERVICE_UUID			(0x1803)

/* Immediate Alert service UUID  */
#define IMMEDIATE_ALERT_SERVICE_UUID	(0x1802)

/* Tx Power service UUID */
#define TX_POWER_SERVICE_UUID			(0x1804)


/* Characteristics UUID's */
/* Alert Level Characteristic UUID */
#define ALERT_LEVEL_CHAR_UUID			(0x2A06)

/* Tx Power Level Characteristic UUID */
#define TX_POWER_LEVEL_CHAR_UUID		(0x2A07)

typedef struct adv_element
{
	uint8_t len;
	uint8_t type;
	uint8_t *data;
}adv_element_t;


/**
* GATT services handles
*/
typedef struct gatt_service_handler
{
	at_ble_uuid_t	serv_uuid;
	at_ble_handle_t	serv_handle;
	at_ble_characteristic_t	serv_chars;
}gatt_service_handler_t;


/* All GAP Connection Parameter defined */
#if ((BLE_DEVICE_ROLE == BLE_CENTRAL) || (BLE_DEVICE_ROLE == BLE_CENTRAL_AND_PERIPHERAL) || (BLE_DEVICE_ROLE == BLE_OBSERVER))
#define GAP_CONN_INTERVAL_MIN			(20)        //Connection interval min 20ms
#define GAP_CONN_INTERVAL_MAX			(40)		//Connection interval max 40ms
#define GAP_CONN_SLAVE_LATENCY			(0)
#define GAP_CE_LEN_MIN					(0)
#define GAP_CE_LEN_MAX					(0)
#define GAP_SUPERVISION_TIMOUT			(0x1f4)		// 500 for supervision timeout

#define GAP_CONNECT_PEER_COUNT			(1)
#define GATT_DISCOVERY_STARTING_HANDLE	(0x0001)
#define GATT_DISCOVERY_ENDING_HANDLE	(0xFFFF)
#endif //((BLE_DEVICE_ROLE == BLE_CENTRAL) || (BLE_DEVICE_ROLE == BLE_CENTRAL_AND_PERIPHERAL) || (BLE_DEVICE_ROLE == BLE_OBSERVER))

#define MAX_DEVICE_CONNECTED			(1)




#if (BLE_DEVICE_ROLE == BLE_PERIPHERAL)

#if defined PROXIMITY_REPORTER
#define BLE_PROFILE_INIT							pxp_reporter_init 
#define BLE_ADDITIONAL_CONNECTED_STATE_HANDLER		pxp_reporter_connected_state_handler
#define BLE_ADDITIONAL_DISCONNECTED_STATE_HANDLER	pxp_disconnect_event_handler
#define BLE_CHARACTERISTIC_CHANGED					pxp_reporter_char_changed_handler
#endif	//PROXIMITY_REPORTER	

#define BLE_CONN_PARAM_UPDATE_DONE					ble_conn_param_update
#define	BLE_PAIR_REQUEST							ble_pair_request_handler
#define BLE_PAIR_KEY_REQUEST						ble_pair_key_request_handler
#define BLE_PAIR_DONE								ble_pair_done_handler
#define BLE_ENCRYPTION_REQUEST						ble_encryption_request_handler
#define BLE_ENCRYPTION_STATUS_CHANGED				ble_encryption_status_change_handler
#endif //(BLE_DEVICE_ROLE == BLE_PERIPHERAL)


#if ((BLE_DEVICE_ROLE == BLE_CENTRAL) || (BLE_DEVICE_ROLE == BLE_CENTRAL_AND_PERIPHERAL))
#define BLE_SCAN_REPORT_HANDLER						ble_scan_report_handler
#define BLE_SCAN_INFO_HANDLER						ble_scan_info_handler

#if defined PROXIMITY_MONITOR
#define BLE_PROFILE_INIT							pxp_monitor_init
#define BLE_ADDITIONAL_CONNECTED_STATE_HANDLER		pxp_monitor_connected_state_handler
#define BLE_ADDITIONAL_DISCONNECTED_STATE_HANDLER	pxp_disconnect_event_handler																								
#define BLE_PRIMARY_SERVICE_FOUND_HANDLER			pxp_monitor_service_found_handler
#define BLE_DISCOVERY_COMPLETE_HANDLER				pxp_monitor_discovery_complete_handler
#define	BLE_SCAN_DATA_HANDLER						pxp_monitor_scan_data_handler
#define BLE_CHARACTERISTIC_READ_RESPONSE			pxp_monitor_characteristic_read_response
#define BLE_CHARACTERISTIC_FOUND_HANDLER			pxp_monitor_characteristic_found_handler
#endif //PROXIMITY_MONITOR
#endif //((BLE_DEVICE_ROLE == BLE_CENTRAL) || (BLE_DEVICE_ROLE == BLE_CENTRAL_AND_PERIPHERAL))

/* Common functions */
#define BLE_CONNECTED_STATE_HANDLER(param)			ble_connected_state_handler(param);\
													BLE_ADDITIONAL_CONNECTED_STATE_HANDLER(param);

#define BLE_DISCONNECTED_STATE_HANDLER(param)		ble_disconnected_state_handler(param);\
													BLE_ADDITIONAL_DISCONNECTED_STATE_HANDLER(param);
													


#ifndef BLE_PROFILE_INIT
#define BLE_PROFILE_INIT									ble_dummy_handler
#endif

#ifndef BLE_ADDITIONAL_CONNECTED_STATE_HANDLER
#define BLE_ADDITIONAL_CONNECTED_STATE_HANDLER				ble_dummy_handler
#endif

#ifndef BLE_ADDITIONAL_DISCONNECTED_STATE_HANDLER
#define BLE_ADDITIONAL_DISCONNECTED_STATE_HANDLER			ble_dummy_handler
#endif

#ifndef BLE_CHARACTERISTIC_CHANGED
#define BLE_CHARACTERISTIC_CHANGED							ble_dummy_handler
#endif

#ifndef BLE_CONN_PARAM_UPDATE_DONE
#define BLE_CONN_PARAM_UPDATE_DONE							ble_dummy_handler
#endif

#ifndef BLE_PAIR_REQUEST
#define	BLE_PAIR_REQUEST									ble_dummy_handler
#endif

#ifndef BLE_PAIR_KEY_REQUEST
#define BLE_PAIR_KEY_REQUEST								ble_dummy_handler
#endif

#ifndef BLE_PAIR_DONE
#define BLE_PAIR_DONE										ble_dummy_handler
#endif
 
#ifndef BLE_ENCRYPTION_REQUEST
#define BLE_ENCRYPTION_REQUEST								ble_dummy_handler
#endif

#ifndef BLE_ENCRYPTION_STATUS_CHANGED
#define BLE_ENCRYPTION_STATUS_CHANGED						ble_dummy_handler
#endif
													
#ifndef BLE_CONN_PARAM_UPDATE_DONE
#define BLE_CONN_PARAM_UPDATE_DONE							ble_dummy_handler
#endif

#ifndef	BLE_PROFILE_INIT
#define BLE_PROFILE_INIT									ble_dummy_handler
#endif

#ifndef BLE_ADDITIONAL_CONNECTED_STATE_HANDLER
#define BLE_ADDITIONAL_CONNECTED_STATE_HANDLER				ble_dummy_handler
#endif

#ifndef BLE_ADDITIONAL_DISCONNECTED_STATE_HANDLER
#define BLE_ADDITIONAL_DISCONNECTED_STATE_HANDLER			ble_dummy_handler
#endif

#ifndef BLE_CHARACTERISTIC_CHANGED
#define BLE_CHARACTERISTIC_CHANGED							ble_dummy_handler
#endif

#ifndef BLE_CONN_PARAM_UPDATE_DONE
#define BLE_CONN_PARAM_UPDATE_DONE							ble_dummy_handler
#endif

#ifndef BLE_PAIR_REQUEST
#define	BLE_PAIR_REQUEST									ble_dummy_handler
#endif

#ifndef BLE_PAIR_KEY_REQUEST
#define BLE_PAIR_KEY_REQUEST								ble_dummy_handler
#endif

#ifndef BLE_PAIR_DONE
#define BLE_PAIR_DONE										ble_dummy_handler
#endif

#ifndef BLE_ENCRYPTION_REQUEST
#define BLE_ENCRYPTION_REQUEST								ble_dummy_handler
#endif

#ifndef BLE_ENCRYPTION_STATUS_CHANGED
#define BLE_ENCRYPTION_STATUS_CHANGED						ble_dummy_handler
#endif

#ifndef BLE_SCAN_REPORT_HANDLER
#define BLE_SCAN_REPORT_HANDLER								ble_dummy_handler
#endif

#ifndef BLE_SCAN_INFO_HANDLER
#define BLE_SCAN_INFO_HANDLER								ble_dummy_handler
#endif

#ifndef BLE_SCAN_DATA_HANDLER
#define	BLE_SCAN_DATA_HANDLER								ble_dummy_handler
#endif

#ifndef BLE_CHARACTERISTIC_WRITE_RESPONSE
#define BLE_CHARACTERISTIC_WRITE_RESPONSE					ble_dummy_handler
#endif


at_ble_status_t ble_set_device_name(uint8_t *name, uint8_t name_len);
void ble_conn_param_update(at_ble_conn_param_update_done_t * conn_param_update);
void ble_pair_request_handler(at_ble_pair_request_t *at_ble_pair_req);
void ble_pair_key_request_handler(at_ble_pair_key_request_t *pair_key);
at_ble_status_t ble_pair_done_handler(at_ble_pair_done_t *pairing_params);
void ble_encryption_request_handler (at_ble_encryption_request_t *encry_req);
void ble_encryption_status_change_handler(at_ble_encryption_status_changed_t *encry_status);

#if ((BLE_DEVICE_ROLE == BLE_CENTRAL) || (BLE_DEVICE_ROLE == BLE_CENTRAL_AND_PERIPHERAL) || (BLE_DEVICE_ROLE == BLE_OBSERVER))
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