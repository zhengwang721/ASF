/**
 * \file
 *
 * \brief Link Loss Service declarations
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


#ifndef __LINK_LOSS_H__
#define __LINK_LOSS_H__

#include "at_ble_api.h"
#include "ble_utils.h"
#include "ble_manager.h"

/** @brief count of included service in Linkloss service
  * 
  */

#define LLS_INCLUDED_SERVICE_COUNT  (0)

/** @brief count of characteristics in linkloss service
  * 
  */
#define LLS_CHARACTERISTIC_COUNT	(1)

#define INVALID_LLS_PARAM			(0xff)

typedef enum linkloss_alert_level {
	LLS_NO_ALERT,
	LLS_MILD_ALERT,
	LLS_HIGH_ALERT
} linkloss_alert_level_t;

typedef struct gatt_lls_char_handler
{
	at_ble_handle_t start_handle;
	at_ble_handle_t end_handle;
	at_ble_handle_t char_handle;
	at_ble_status_t char_discovery;
	uint8_t *char_data;
}gatt_lls_char_handler_t;

/****************************************************************************************
*							        Function Prototypes	                                     							*
****************************************************************************************/

/** @brief Initialize the linkloss service with default values
  * 
  *
  * @param[in] gatt_service_handler_t  the service info which has handle range,uuid and characteristic array fields
  *
  * @pre Must be called before @ref lls_primary_service_define
  *
  * @return void
  */
void init_linkloss_service(gatt_service_handler_t *linkloss_serv);

/** @brief Defining linkloss service to the attribute data base
  * 
  *
  * @param[in] gatt_service_handler_t  the service info which has handle range,uuid and characteristic array fields
  *
  * @pre Must be called after @ref init_linkloss_service
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */

at_ble_status_t lls_primary_service_define(gatt_service_handler_t *lls_service);


/** @brief sets the alert value of the Linkloss service
  * @param[in] gatt_service_handler_t  the service info which has handle range,uuid and characteristic array fields
  * @param[in] at_ble_characteristic_changed_t  the information of the characteristic changed.
  * @return @ref ias alert value
  */
uint8_t lls_set_alert_value(at_ble_characteristic_changed_t *change_params, gatt_service_handler_t *lls_handler);

#endif /* __LINK_LOSS_H__ */