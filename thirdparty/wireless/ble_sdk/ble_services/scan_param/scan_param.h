
/**
 * \file
 *
 * \brief Scan Param Service declarations
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


#ifndef __SCAN_PARAM_H__
#define __SCAN_PARAM_H__

#include "at_ble_api.h"
#include "ble_manager.h"

/** scan interval value characteristic value initial and maximum length */
#define SPS_CHAR_SCAN_INT_VALUE_INIT_VALUE		0x00
#define SPS_CHAR_SCAN_INT_VALUE_MAX_VALUE		0x00

/** scan refresh value characteristic value initial and maximum length */
#define SPS_CHAR_SCAN_REFRESH_INIT_VALUE		0x00
#define SPS_CHAR_SCAN_REFRESH_MAX_VALUE			0x00

#define SPS_MAX_CHARACTER				(2)


typedef struct sps_gatt_service_handler
{
	at_ble_uuid_t	serv_uuid;
	at_ble_handle_t	serv_handle;
	at_ble_characteristic_t	serv_chars[SPS_MAX_CHARACTER];
	at_ble_handle_t conn_handle;
}sps_gatt_service_handler_t;

/** The type of the information*/
typedef enum {
	/* scan interval characteristic */
	SPS_SCAN_INTERVAL,
	/* scan refresh characteristic */
	SPS_SCAN_REFRESH
}sps_info_type;

/** Configurable Client characteristic data for a given sps info type*/
typedef struct{
	uint16_t data_len;
	uint8_t *info_data;
}sps_info_data;

/**@brief  Generic Function used to update the SPS info type during connection
 *
 * @param[in] sps_serv gatt service information
 * @param[in] info_type @ref sps_info_type, specifies the characteristic
 * @param[in] info_data @ref sps_info_data, holds the new data information
 * @return none
 */
at_ble_status_t sps_info_update(sps_gatt_service_handler_t *dis_serv , sps_info_type info_type, sps_info_data* info_data);

/**@brief Initialize the service with its included service, characteristics, and descriptors
 *
 * @param[in] sps_serv gatt service information
 *
 * @return none
 */
void sps_init_service(sps_gatt_service_handler_t *sps_serv);

/**@brief defining a initialized service 
 *
 * @param[in] sps_service gatt service information
 *
 * @return @ref AT_BLE_SUCCESS operation completed successfully
 * @return @ref AT_BLE_FAILURE Generic error.
 */
at_ble_status_t sps_primary_service_define(sps_gatt_service_handler_t *sps_service);

at_ble_status_t sps_update_char_value (sps_gatt_service_handler_t *scan_param , uint8_t *char_data, at_ble_handle_t conn_handle);


#endif /* __SCAN_PARAM_H__ */