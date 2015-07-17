///**
 //* \file
 //*
 //* \brief TX Service declarations
 //*
 //* Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 //*
 //* \asf_license_start
 //*
 //* \page License
 //*
 //* Redistribution and use in source and binary forms, with or without
 //* modification, are permitted provided that the following conditions are met:
 //*
 //* 1. Redistributions of source code must retain the above copyright notice,
 //*    this list of conditions and the following disclaimer.
 //*
 //* 2. Redistributions in binary form must reproduce the above copyright notice,
 //*    this list of conditions and the following disclaimer in the documentation
 //*    and/or other materials provided with the distribution.
 //*
 //* 3. The name of Atmel may not be used to endorse or promote products derived
 //*    from this software without specific prior written permission.
 //*
 //* 4. This software may only be redistributed and used in connection with an
 //*    Atmel microcontroller product.
 //*
 //* THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 //* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 //* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 //* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 //* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 //* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 //* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 //* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 //* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 //* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 //* POSSIBILITY OF SUCH DAMAGE.
 //*
 //* \asf_license_stop
 //*
 //*/
///*
 //* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 //*/
//
//
#ifndef __TX_POWER_H__
#define __TX_POWER_H__

/****************************************************************************************
*							        Includes	                                     							*
****************************************************************************************/

#include <asf.h>
#include "at_ble_api.h"
#include "ble_manager.h"
#include "at_ble_api.h"

/****************************************************************************************
*							        Macros	                                     							*
****************************************************************************************/

/** @brief  DEFAULT_TX_PWR_VALUE the tx power value*/
#define DEFAULT_TX_PWR_VALUE -3


/****************************************************************************************
*							        Function Declarations	                                     							*
****************************************************************************************/

/** @brief Initialize the tx power service with default values
  * 
  *
  * @param[in] gatt_service_handler_t  the service info which has handle range,uuid and characteristic array fields
  *
  * @pre Must be called before @ref txps_primary_service_define
  *
  * @return void
  */

void init_tx_power_service(gatt_service_handler_t *tx_power_serv);

/** @brief Defining the tx power service to the attribute data base
  * 
  *
  * @param[in] gatt_service_handler_t  the service info which has handle range,uuid and characteristic array fields
  *
  * @pre Must be called after @ref init_tx_power_service
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */


at_ble_status_t txps_primary_service_define(gatt_service_handler_t *txps_primary_service);
//
//
#endif /* __TX_POWER_H__ */