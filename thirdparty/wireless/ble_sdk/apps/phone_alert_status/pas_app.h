/**
* \file
*
* \brief Phone Alert Status Application declarations
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

/****************************************************************************************
*							        Macros			                                 	*
****************************************************************************************/
#define BIT0_MASK					(0x1 << 0)
#define BIT1_MASK					(0x1 << 1)
#define BIT2_MASK					(0x1 << 2)

#define RINGER_SILENT				(0x0)
#define RINGER_NORMAL				(0x1)

#define DEVICE_SILENT				(1)
#define DEVICE_MUTE					(2)
#define DEVICE_NORMAL				(3)
#define READ_REQUEST				(4)

/****************************************************************************************
*							        Function Prototype                                 	*
****************************************************************************************/

/**
 * @brief Timer Callback
 */
void timer_callback_handler(void);

/**
 * @brief displays alerts status value came in read response
 * @param[in] data
 * @param[in] len  
 */
void app_alert_status_read(uint8_t *data, uint8_t len);

/**
 * @brief displays ringer setting value came in read response
 * @param[in] data
 * @param[in] len  
 */
void app_ringer_setting_read(uint8_t *data, uint8_t len);

/**
 * @brief displays alert status value came in notification
 * @param[in] data
 * @param[in] len  
 */
void app_alert_status_notify(uint8_t *data, uint8_t len);

/**
 * @brief displays ringer setting value came in notification
 * @param[in] data
 * @param[in] len  
 */
void app_ringer_setting_notify(uint8_t *data, uint8_t len);

/**
 * @brief app_connected_state profile notifies the application about state
 * @param[in] connected
 */
void app_connected_state(bool connected);

/**
 * @brief display alert status info notifies the application about state
 * @param[in] data
 */
void display_alert_status_info(uint8_t *data);

/**
 * @brief display alert status info notifies the application about state
 * @param[in] data
 */
void display_ringer_setting_info(uint8_t *data);