/**
 * \file
 *
 * \brief Proximity Reporter Profile Application declarations
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


#ifndef __PXP_APP_H__
#define __PXP_APP_H__

#include "at_ble_api.h"
#include "platform.h"
#include "console_serial.h"


#define SUCCESS 1

#define DBG_LOG printf
#define DBG_LOG_1LVL printf

#define APP_PXP_FAST_ADV 100 //100 ms

#define APP_PXP_ADV_TIMEOUT 1000 // 100 Secs


#define APP_PXP_FAST_ADV 100 //100 ms

#define APP_PXP_ADV_TIMEOUT 1000 // 100 Secs

#define SCAN_RESP_LEN 10
#define ADV_DATA_LEN 18

#define ADV_TYPE_LEN (0x01)

#define LL_ADV_DATA_UUID_LEN	 (2)
#define LL_ADV_DATA_UUID_TYPE	 (0x03)
#define LL_ADV_DATA_UUID_DATA	 "\x03\x18"

#define TXP_ADV_DATA_UUID_LEN	 (2)
#define TXP_ADV_DATA_UUID_TYPE	 (0x03)
#define TXP_ADV_DATA_UUID_DATA	 "\x04\x18"

#define IAL_ADV_DATA_UUID_LEN	 (2)
#define IAL_ADV_DATA_UUID_TYPE	 (0x03)
#define IAL_ADV_DATA_UUID_DATA	 "\x02\x18"


#define PXP_ADV_DATA_NAME_LEN	 (9)
#define PXP_ADV_DATA_NAME_TYPE	 (0x09)
#define PXP_ADV_DATA_NAME_DATA	 "ATMEL-PXP"

extern uint8_t scan_rsp_data[SCAN_RESP_LEN];
#endif 