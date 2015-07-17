/**
* \file
*
* \brief Proximity Reporter Profile declarations
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


#ifndef __PXP_REPORTER_H__
#define __PXP_REPORTER_H__

#include "ble_manager.h"


/****************************************************************************************
*							        Includes	                                     							*
****************************************************************************************/


/****************************************************************************************
*							        Macros	                                     							*
****************************************************************************************/
#define LL_INTERVAL_SLOW			(3)
#define LL_INTERVAL_MEDIUM			(2)
#define LL_INTERVAL_FAST			(1)


#define PL_INTERVAL_SLOW			(8)
#define PL_INTERVAL_MEDIUM			(6)
#define PL_INTERVAL_FAST			(4)


typedef void (*reporter_callback_t)(uint8_t);


/****************************************************************************************
*							        Function Prototypes	                                     							*
****************************************************************************************/


/** @brief Initialize the profile, includes all initializations and start up routines of the profiles
  * 
  *
  * @param[in] void
  *
  * @pre Must be called after @ref ble_init
  *
  * @return void
  */
void pxp_app_init(void *param);

/** @brief Initialize the services of the profile
  * 
  * @param[in] void
  *
  * @pre Must be called before @ref pxp_service_define
  *
  * @return void
  */
void pxp_service_init(void);


/** @brief profile services definition to the attribute data base
  * 
  * @param[in] void
  *
  * @pre Must be called after ref pxp_service_init
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t pxp_service_define (void);


/** @brief sets the advertisement data and starts advertisement
  * 
  * @param[in] void
  *
  * @pre Must be called after ref pxp_service_init
  *
  * @return void
  */
void pxp_reporter_adv(void);

/** @brief connection state handler
  * 
  * @param[in] at_ble_connected_t which includes the connection parameters like handle
  *
  * @pre Must be called when the device is connected
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t pxp_reporter_connected_state_handler (at_ble_connected_t * conn_params);


/** @brief disconnection event handler
  * 
  * @param[in] at_ble_disconencted_t which includes the connection parameters like handle,reason for disconnection
  *
  * @pre Must be called when the device is disconnected
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t pxp_disconnect_event_handler(at_ble_disconnected_t *disconnect);


/** @brief character changed handler
  * 
  * @param[in] at_ble_characteristic_changed_t which includes handle,new value
  *
  * @pre Must be called when character change event occurred
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
at_ble_status_t pxp_reporter_char_changed_handler(at_ble_characteristic_changed_t *char_handle);

void register_pathloss_handler(reporter_callback_t pathloss_fn);
void register_linkloss_handler(reporter_callback_t linkloss_fn);

#endif


