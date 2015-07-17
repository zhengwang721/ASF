/**
 * \file
 *
 * \brief Simple Broadcaster Application declarations
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
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel
 * Support</a>
 */

#ifndef __SIMPLE_BROADCASTER_APP_H__
#define __SIMPLE_BROADCASTER_APP_H__

#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "profiles.h"
#include "console_serial.h"
#include "timer_hw.h"
#include "conf_extint.h"
#include "conf_serialdrv.h"
#include "ble_manager.h"
#include "ble_utils.h"

/* 100 ms */
#define APP_BROADCAST_FAST_ADV                  (200)

/* 100 Secs */
#define APP_BROADCAST_ADV_TIMEOUT               (1000)

/* maximum length of advertisement data */
#define MAX_ADV_LEN								(28)

/* maximum length of scan response data */
#define MAX_SCAN_LEN MAX_ADV_LEN

/* /Advertisement types */
#define ADV_TYPE_SCANNABLE_UNDIRECTED AT_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED
#define ADV_TYPE_NONCONN_UNDIRECTED AT_BLE_ADV_TYPE_NONCONN_UNDIRECTED

/* /Advertisement data type length */
#define ADV_TYPE_LEN							(0x01)

/** @brief Return status to app from the APIs provided
 */
typedef enum {
	/* /Success */
	STATUS_SUCCESS = 0,
	/* /adv_data or scan_rsp_data have reached maximum length */
	STATUS_MAX_LENGTH_REACHED,
	/* /Input parameter invalid */
	STATUS_INVALID_PARAM,
	/* /Trying to send incomplete data on scan response data */
	STATUS_OPERATION_NOT_SUPPORTED,
	/* /Failure */
	STATUS_FAILED
} status_t;

/** @brief data type, configurable by app to specify which part of the data(scan
 * response or advertisement)
 *                 should contain the data given by app.
 */
typedef enum {
	ADVERTISEMENT_DATA = 0,
	SCAN_RESP_DATA
} data_type_t;

/** Advertisement data types
 */
typedef enum {
	ADV_DATA_TYPE_INCOMPLETE_SERVICE_UUID16 = 2,
	ADV_DATA_TYPE_COMPLETE_SERVICE_UUID16,
	ADV_DATA_TYPE_INCOMPLETE_SERVICE_UUID32,
	ADV_DATA_TYPE_COMPLETE_SERVICE_UUID32,
	ADV_DATA_TYPE_INCOMPLETE_SERVICE_UUID128,
	ADV_DATA_TYPE_COMPLETE_SERVICE_UUID128,
	ADV_DATA_TYPE_SHORTENED_LOCAL_NAME,
	ADV_DATA_TYPE_COMPLETE_LOCAL_NAME,
	ADV_DATA_TYPE_APPEARANCE = 0x019,
	ADV_DATA_TYPE_MANUFACTURER_DATA = 0xff
} adv_data_type_t;

#define BRD_ADV_DATA_UUID_LEN				(2)
#define BRD_ADV_DATA_UUID_DATA              "\x00\x18"

#define BRD_ADV_DATA_APPEARANCE_LEN			(2)
#define BRD_ADV_DATA_APPEARANCE_DATA		"\x80\x00"

#define BRD_ADV_DATA_NAME_LEN               (0x0b)
#define BRD_ADV_DATA_NAME_DATA              "ATMEL-BROADCAST"

/** @brief Starts advertising
 *
 *@param[in] none
 *@return none
 *
 */
void brd_start_broadcast(void);

/* APIs for app to set the advertisement data */

/** @brief Set advertisement data with incomplete list of service uuid of 16bits
 *
 * @param[in] list_uuid16 list of service uuid of 16bits
 * @param[in] length length of the list
 * @return @ref STATUS_SUCCESS adding the list to advertisement data success
 * @return @ref STATUS_MAX_LENGTH_REACHED advertisement data does not have
 * enough space to hold the list
 * @return @ref STATUS_INVALID_PARAM parameters passed are invalid
 * @return @ref STATUS_OPERATION_NOT_SUPPORTED trying to send incomplete list on
 * scan response data.
 *
 */
status_t brd_adv_incomp_list_service_uuid16(uint8_t *list_uuid16,
		uint8_t length);

/** @brief Set advertisement data with complete list of service uuid of 16bits
 *
 * @param[in] list_uuid16 list of service uuid of 16bits
 * @param[in] length length of the list
 * @return @ref STATUS_SUCCESS adding the list to advertisement data success
 * @return @ref STATUS_MAX_LENGTH_REACHED advertisement data does not have
 * enough space to hold the list
 * @return @ref STATUS_INVALID_PARAM parameters passed are invalid
 *
 */
status_t brd_adv_comp_list_service_uuid16(uint8_t *list_uuid16, uint8_t length);

/** @brief Set advertisement data with incomplete list of service uuid of 32bits
 *
 * @param[in] list_uuid32 list of service uuid of 32bits
 * @param[in] length length of the list
 * @return @ref STATUS_SUCCESS adding the list to advertisement data success
 * @return @ref STATUS_MAX_LENGTH_REACHED advertisement data does not have
 * enough space to hold the list
 * @return @ref STATUS_INVALID_PARAM parameters passed are invalid
 * @return @ref STATUS_OPERATION_NOT_SUPPORTED trying to send incomplete list on
 * scan response data.
 *
 */
status_t brd_adv_incomp_list_service_uuid32(uint8_t *list_uuid32,
		uint8_t length);

/** @brief Set advertisement data with complete list of service uuid of 32bits
 *
 * @param[in] list_uuid32 list of service uuid of 32bits
 * @param[in] length length of the list
 * @return @ref STATUS_SUCCESS adding the list to advertisement data success
 * @return @ref STATUS_MAX_LENGTH_REACHED advertisement data does not have
 * enough space to hold the list
 * @return @ref STATUS_INVALID_PARAM parameters passed are invalid
 *
 */
status_t brd_adv_comp_list_service_uuid32(uint8_t *list_uuid32, uint8_t length);

/** @brief Set advertisement data with incomplete list of service uuid of 32bits
 *
 * @param[in] list_uuid128 list of service uuid of 128bits
 * @param[in] length length of the list
 * @return @ref STATUS_SUCCESS adding the list to advertisement data success
 * @return @ref STATUS_MAX_LENGTH_REACHED advertisement data does not have
 * enough space to hold the list
 * @return @ref STATUS_INVALID_PARAM parameters passed are invalid
 * @return @ref STATUS_OPERATION_NOT_SUPPORTED trying to send incomplete list on
 * scan response data.
 *
 */
status_t brd_adv_incomp_list_service_uuid128(uint8_t *list_uuid128,
		uint8_t length);

/** @brief Set advertisement data with complete list of service uuid of 128bits
 *
 * @param[in] list_uuid128 list of service uuid of 128bits
 * @param[in] length length of the list
 * @return @ref STATUS_SUCCESS adding the list to advertisement data success
 * @return @ref STATUS_MAX_LENGTH_REACHED advertisement data does not have
 * enough space to hold the list
 * @return @ref STATUS_INVALID_PARAM parameters passed are invalid
 *
 */
status_t brd_adv_comp_list_service_uuid128(uint8_t *list_uuid128,
		uint8_t length);

/** @brief Set advertisement data with shortened local name
 *
 * @param[in] local_name local name of the device
 * @param[in] length length of the local name
 * @return @ref STATUS_SUCCESS adding the shortened local name to advertisement
 * data success
 * @return @ref STATUS_MAX_LENGTH_REACHED advertisement data does not have
 * enough space to hold the local name
 * @return @ref STATUS_INVALID_PARAM parameters passed are invalid
 * @return @ref STATUS_OPERATION_NOT_SUPPORTED trying to send shortened local
 * name.
 *
 */
status_t brd_adv_shortened_local_name(uint8_t *local_name, uint8_t length);

/** @brief Set advertisement data with complete local name
 *
 * @param[in] local_name local name of the device
 * @param[in] length length of the local name
 * @return @ref STATUS_SUCCESS adding the complete local name to advertisement
 * data success
 * @return @ref STATUS_MAX_LENGTH_REACHED advertisement data does not have
 * enough space to hold the local name
 * @return @ref STATUS_INVALID_PARAM parameters passed are invalid
 *
 */
status_t brd_adv_comp_local_name(uint8_t *local_name, uint8_t length);

/** @brief Set advertisement data with appearance of the device
 *
 * @param[in] appearance appearance of the device
 * @param[in] length length of the local name
 * @return @ref STATUS_SUCCESS adding the appearance to advertisement data
 * success
 * @return @ref STATUS_MAX_LENGTH_REACHED advertisement data does not have
 * enough space to hold the local name
 * @return @ref STATUS_INVALID_PARAM parameters passed are invalid
 *
 */
status_t brd_adv_appearance(uint8_t *appearance, uint8_t length);

/** @brief Set advertisement data with manufacturer data
 *
 * @param[in] data manufacturer data of the device
 * @param[in] length length of the local name
 * @return @ref STATUS_SUCCESS adding the manufacturer data to advertisement
 * data success
 * @return @ref STATUS_MAX_LENGTH_REACHED advertisement data does not have
 * enough space to hold the local name
 * @return @ref STATUS_INVALID_PARAM parameters passed are invalid
 *
 */
status_t brd_adv_manufacturer_data(uint8_t *data, uint8_t length);

/** @brief Set advertisement type
 *
 * @param[in] type type can be ADV_TYPE_SCANNABLE_UNDIRECTED or
 * ADV_TYPE_NONCONN_UNDIRECTED
 * @return @ref STATUS_SUCCESS successfully set advertisement type
 * @return @ref STATUS_INVALID_PARAM parameters passed are invalid
 *
 */
status_t brd_set_advertisement_type(uint8_t type);

/** @brief Set advertisement data type,data type can be SCAN_RESP_DATA only if
 * advertisement type is ADV_TYPE_SCANNABLE_UNDIRECTED
 *
 * @param[in] type type can be ADVERTISEMENT_DATA or SCAN_RESP_DATA
 * @return @ref STATUS_SUCCESS successfully set the advertisement data type
 * @return @ref STATUS_INVALID_PARAM parameters passed are invalid
 *
 */
status_t brd_set_data_type(uint8_t type);

/** @brief Set advertisement data
 *
 * @param[in] adv_data_type type of the advertisement data type
 * @param[in] adv_usr_data reference to a user data corresponding to the
 * advertisement data type
 * @return @ref STATUS_SUCCESS successfully added user data to the advertisement
 * data
 * @return @ref STATUS_INVALID_PARAM parameters passed are invalid
 * @return @ref STATUS_MAX_LENGTH_REACHED advertisement data does not have
 * enough space to hold the user data
 * @return @ref STATUS_OPERATION_NOT_SUPPORTED trying to add data in unmatched
 * advertisement type
 *
 */
status_t brd_set_advertisement_data(adv_data_type_t adv_data_type,
		uint8_t *adv_usr_data, uint8_t length);

/** @brief initialize the broadcaster event
 * *
 */
status_t brd_adv_init(void);

#endif /* __SIMPLE_BROADCASTER_APP_H__ */
