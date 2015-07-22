/**
 * \file
 *
 * \brief Time Information Profile declarations
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

#ifndef __TIME_INFO_H__
#define __TIME_INFO_H__

/***********************************************************************************
 *									Includes		                               *
 **********************************************************************************/
#define TIME_INFO

/***********************************************************************************
 *									Macros			                               *
 **********************************************************************************/
/**@brief Advertisement Interval*/
#define APP_TP_FAST_ADV								(100)		//100 ms
/**@brief Advertisement Timeout*/
#define APP_TP_ADV_TIMEOUT							(1000)		// 100 Secs
/**@brief Scan Response length*/
#define SCAN_RESP_LEN								(10)

/**@brief ADV UUID Type & Length*/
#define AD_TYPE_16BIT_UUID_LEN						 (2)
#define AD_TYPE_32BIT_UUID_LEN						 (4)
#define AD_TYPE_128BIT_UUID_LEN						 (16)

/**@brief Time Profile type, UUid & Length */
#define ADV_TYPE_LEN								(0x01)
#define TP_ADV_DATA_UUID_LEN						(2)
#define TP_ADV_DATA_16BIT_UUID_LEN					(2)
#define TP_ADV_DATA_128BIT_UUID_LEN					(16)
#define TP_GAP_UUID_CTS_DATA						("\x00\x18")

/**@brief TIP Service Solicitation Info*/
#define TP_ADV_DATA_SERVSOLICITATION_16UUID_TYPE	(0x14)
#define TP_ADV_DATA_SERVSOLICITATION_128UUID_TYPE	(0x15)

/**@brief Current Time Service UUId  */
#define TP_ADV_DATA_UUID_TYPE						(0x03)
#define TP_ADV_DATA_UUID_CTS_DATA					("\x05\x18")

/**@brief Appearance type & Length */
#define TP_ADV_DATA_APPEARANCE_LEN					(2)
#define TP_ADV_DATA_APPEARANCE_TYPE					(0x19)
#define TP_ADV_DATA_APPEARANCE_DATA					("\x00\x01")

/**@brief Advertisement Name Type Length & data */
#define TP_ADV_DATA_NAME_LEN						 (9)
#define TP_ADV_DATA_NAME_TYPE						 (0x09)
#define TP_ADV_DATA_NAME_DATA						 ("ATMEL-TIP")

/***********************************************************************************
 *									Types			                               *
 **********************************************************************************/
/** @brief Type of attribute discovery */
typedef enum
{
	/// Discovery Disable
	DISCOVER_IDLE = 0,
	/// Discover Service
	DISCOVER_SERVICE,
	/// Discover Include Service
	DISCOVER_INCLUDE_SERVICE,
	
	/// Discover Characteristic
	DISCOVER_CHARACTERISTIC,
	
	/// Discover Descriptor
	DISCOVER_DESCRIPTOR,
	
}at_ble_discovery_role;

/***********************************************************************************
 *									Fucntions		                               *
 **********************************************************************************/
/**
 * @brief Initialize the Time information Profile
 * 
 */
void time_info_init(void *param);

/**
 * @brief Discovering the services of time server
 * @param[in] connection parameter which includes the connection handle device address
 */
at_ble_status_t time_info_service_discover(at_ble_connected_t *conn_params);

/**
 * @brief Handler for scan info data
 * @param[in] scan info data data buffer containing scan data of all the scanned device 
 * @param[in] Device count
 */
at_ble_status_t time_info_scan_data_handler(at_ble_scan_info_t *scan_buffer,
uint8_t scanned_dev_count);

/**
 * @brief Handler for connection event 
 * @param[in] connected event parameter containing details like handle
 * \note Called by the ble_manager after receiving connection event
 */
at_ble_status_t time_info_connected_state_handler(at_ble_connected_t * conn_params);

/**
 * @brief Handler for discovery complete event
 * @param[in] discovery complete event which contains result of discovery event
 * \note Called by the ble_manager after receiving discovery complete event
 */
void time_info_discovery_complete_handler(at_ble_discovery_complete_t *discover_status);

/**
 * @brief Handler for characteristic read
 * @param[in] read response param which gives the status of read and value
 * \note Called by the ble_manager after receiving read response event
 */
void time_info_characteristic_read_response(at_ble_characteristic_read_response_t *char_read_resp);
/**
 * @brief Handler for characteristic found event
 * @param[in] characteristic found event parameter containing details like characteristic handle,uuid
 * \note Called by the ble_manager after receiving characteristic found event
 */
void time_info_characteristic_found_handler(at_ble_characteristic_found_t *characteristic_found);

/**
 * @brief Handler for service found event
 * @param[in] service found event parameter containing details like service handle,uuid
 * \note Called by the ble_manager after receiving service found event
 */
void time_info_service_found_handler(at_ble_primary_service_found_t * primary_service_params);
/**
 * @brief Handler for disconnection event
 * @param[in] disconnected event parameter containing details like handle
 * \note Called by the ble_manager after receiving disconnection event
 */
void time_info_disconnected_event_handler(at_ble_disconnected_t *disconnect);

/**
 * @brief Handler for char changed handler 
 * @param[in] characteristic changed event parameter containing details like characteristic handle,value
 * \note Called by the ble_manager after receiving characteristic change event
 */
void time_info_char_changed_handler(at_ble_characteristic_changed_t *characteristic_changed);

/**
 * @brief setting time profile advertisement data and triggering of advertisment
 */
void time_info_adv(void);


#endif /* __TIME_INFO_H__ */