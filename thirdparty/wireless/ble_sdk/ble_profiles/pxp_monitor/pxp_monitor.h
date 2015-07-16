/**
 * \file
 *
 * \brief Proximity Monitor Profile Application declarations
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

#ifndef __PXP_MONITOR_H__
#define __PXP_MONITOR_H__

#include "ble_manager.h"

#define PROXIMITY_MONITOR

typedef enum
{
	AD_TYPE_FLAGS = 01,
	AD_TYPE_COMPLETE_LIST_UUID = 0x03,
	AD_TYPE_COMPLETE_LOCAL_NAME = 0x09
}AD_TYPE;

#define PXP_RSSI_UPDATE_INTERVAL	(0x01)
#define PXP_LOW_ALERT_RANGE			(-70)
#define PXP_HIGH_ALERT_RANGE		(-90)

void pxp_monitor_init(void);

at_ble_status_t pxp_monitor_scan_data_handler(at_ble_scan_info_t *scan_buffer,
												uint8_t scanned_dev_count);
												
at_ble_status_t pxp_disconnect_event_handler(at_ble_disconnected_t *disconnect);


 /**@brief After the successful connection handler
  *
  * After connecting to the peer device start the GATT primary discovery
  *
  * @param[in] at_ble_connected_t parameters of the established connection
  *
  * @return @ref AT_BLE_SUCCESS operation programmed successfully.
  * @return @ref AT_BLE_INVALID_PARAM if GATT discovery parameter are incorrect parameter.
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t pxp_monitor_connected_state_handler(at_ble_connected_t * conn_params);


 /**@brief Discover all Characteristics supported for Proximity Service in a connected device
 *  and handles discovery complete
 * Search will go from start_handle to end_handle, whenever a characteristic is found
 * After search and discovery completes will initialize the alert level and read the tx power value as defined
 * @ref AT_BLE_CHARACTERISTIC_FOUND event is sent and @ref AT_BLE_DISCOVERY_COMPLETE is sent at end of discover operation.
 *
 * @param[in] discover_status discovery status on each
 * @param[in] start_handle start of the searched range
 * @param[in] end_handle   end of the searched range
 *
 */
void pxp_monitor_discovery_complete_handler(at_ble_discovery_complete_t *discover_status);



void pxp_monitor_characteristic_read_response(at_ble_characteristic_read_response_t *char_read_resp);

void pxp_monitor_characteristic_found_handler(at_ble_characteristic_found_t *characteristic_found);

void pxp_monitor_service_found_handler(at_ble_primary_service_found_t * primary_service_params);

at_ble_status_t pxp_monitor_connect_request(at_ble_scan_info_t * scan_buffer, uint8_t index);

//void rssi_update (at_ble_handle_t conn_handle);

#endif /*__PXP_MONITOR_H__*/