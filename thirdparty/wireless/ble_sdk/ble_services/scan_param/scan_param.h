
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

/**@brief Scan parameter service UUID */
#define SPS_SERVICE_UUID 0x1813

/**@brief Scan interval characteristic uuid */
#define SPS_CHAR_SCAN_INT_VALUE_UUID 0x2a4f

/**@brief Scan refresh characteristic uuid */
#define SPS_CHAR_SCAN_REFRESH_UUID 0x2a31

/**@brief Function used to update the scan refresh characteristic value during connection
 *
 * @param[in] sps_serv gatt service information
 * @param[in] info_data @ref sps_info_data, holds the new data information
 * @return none
 */
at_ble_status_t sps_scan_refresh_char_update(sps_gatt_service_handler_t *sps_serv, uint8_t scan_refresh_value);

/**@brief Initialize the service with its included service, characteristics, and descriptors
 *
 * @param[in] sps_serv gatt service information
 * @param[in] scan_interval_window used to know the gatt client scan interval window
 * @param[in] scan_refresh application uses to get the updated scan interval window of the gatt client
 *
 * @return none
 */
void sps_init_service(sps_gatt_service_handler_t *sps_serv, uint16_t *scan_interval_window, uint8_t *scan_refresh);


/**@brief defining a initialized service 
 *
 * @param[in] sps_service gatt service information
 *
 * @return @ref AT_BLE_SUCCESS operation completed successfully
 * @return @ref AT_BLE_FAILURE Generic error.
 */
at_ble_status_t sps_primary_service_define(sps_gatt_service_handler_t *sps_service);


#endif /* __SCAN_PARAM_H__ */