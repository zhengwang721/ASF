
/**
* \file
*
* \brief Alert Notification Profile declarations
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
*    Atmel micro controller product.
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

/**
* 
* \section preface Preface
* This is the reference manual for the Time Information Profile
*/

#ifndef __ANP_CLIENT_H__
#define __ANP_CLIENT_H__





#define APP_ANP_FAST_ADV 100 //100 ms
#define APP_ANP_ADV_TIMEOUT 1000 // 100 Secs

#define SCAN_RESP_LEN 10

#define AD_TYPE_16BIT_UUID_LEN       (2)
#define AD_TYPE_32BIT_UUID_LEN       (4)
#define AD_TYPE_128BIT_UUID_LEN      (16)
#define ADV_TYPE_LEN				 (0x01)

#define ANP_ADV_DATA_APPEARANCE_LEN	 (2)
#define ANP_ADV_DATA_APPEARANCE_TYPE (0x19)
#define ANP_ADV_DATA_APPEARANCE_DATA "\x00\x40"

#define ANP_ADV_DATA_NAME_LEN		 (6)
#define ANP_ADV_DATA_NAME_TYPE		 (0x09)
#define ANP_ADV_DATA_NAME_DATA		 "AT-ANP"

#define ANP_SOLICITATION_LEN					   (16)
#define ANP_ADV_DATA_SERVSOLICITATION_16UUID_TYPE  (0x14)
#define ANP_ADV_DATA_SERVSOLICITATION_128UUID_TYPE (0x15)
#define ANP_ANCS_SERVICE_UUID                       ("\xD0\x00\x2D\x12\x1E\x4B\x0F\xA4\x99\x4E\xCE\xB5\x31\xF4\x05\x79")


#define ANCS_SERV_UUID	 "\xD0\x00\x2D\x12\x1E\x4B\x0F\xA4\x99\x4E\xCE\xB5\x31\xF4\x05\x79"
#define ANCS_CHAR_NOTIFICATION_SOURCE_UUID "\xBD\x1D\xA2\x99\xE6\x25\x58\x8C\xD9\x42\x01\x63\x0D\x12\xBF\x9F"
#define ANCS_CHAR_CONTROL_POINT "\xD9\xD9\xAA\xFD\xBD\x9B\x21\x98\xA8\x49\xE1\x45\xF3\xD8\xD1\x69"
#define ANCS_CHAR_DATA_SOURCE "\xFB\x7B\x7C\xCE\x6A\xB3\x44\xBE\xB5\x4B\xD6\x24\xE9\xC6\xEA\x22"

#define UUID_16BIT_LEN       (2)
#define UUID_32BIT_LEN       (4)
#define UUID_128BIT_LEN      (16)

#define ANP_MAX_CHARACTERISTIC (3)
#define ANP_MAX_DESCRIPTOR     (3)

#define FALSE (0)
#define TRUE  (1)

#define NOTIFICATION_ADDED   (0)
#define NOTIFICATION_REMOVED (2)

#define CATEGORY_ID_INCOMINGCALL (1)
#define CATEGORY_ID_MISSEDCALL   (2)

#define START_HANDLE				  0x0001
#define END_HANDLE					  0xffff


/* Typedef for alert notification profile */
typedef struct ancs_prf{
	
	/*Instance for ANCS*/
	at_ble_primary_service_found_t ancs_serv;
	
	/*Instance for notification source characteristic*/
	at_ble_characteristic_found_t notification_source_char;
	
	/*Instance for control point characteristic*/
	at_ble_characteristic_found_t control_point_char;
	
	/*Instance for data source characteristic*/
	at_ble_characteristic_found_t data_source_char;
	
	/*Instance for notification source descriptor*/
	at_ble_descriptor_found_t notification_source_desc;
	
	/*Instance for notification source descriptor*/
	at_ble_descriptor_found_t data_source_desc;
	
}ancs_prf_t;

/* Typedef for alert notification profile */
typedef struct app_anp_data{
	uint8_t devicedb;
	uint8_t discover_role;
	uint8_t dispinfo;
	at_ble_connected_t conn_params;
	at_ble_characteristic_found_t char_info;
	at_ble_descriptor_found_t ancs_desc;
}app_anp_data_t;

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
	
}ble_discovery_role;

typedef struct
{
	uint8_t timeval[8];
	uint8_t dateval[10];
	uint8_t daysval[2];
	uint16_t year;
}time_val_t;

void anp_info_init(void);

void anp_client_init( void *params);

void anp_client_adv(void);

void anp_client_connected_state_handler(at_ble_connected_t *params);

void anp_client_disconnected_event_handler(at_ble_disconnected_t *params);

void anp_client_service_found_handler(at_ble_primary_service_found_t * params);

//void anp_client_discovery_complete_handler(at_ble_primary_service_found_t);

void anp_client_discovery_complete_handler(at_ble_discovery_complete_t *params);

void anp_client_characteristic_found_handler(at_ble_characteristic_found_t *params);

void anp_client_descriptor_found_handler(at_ble_descriptor_found_t *params);

void anp_client_char_changed_handler(at_ble_characteristic_changed_t *params);

void anp_client_write_response_handler(at_ble_characteristic_write_response_t *params);

void anp_client_notification_handler(at_ble_notification_recieved_t *params);

void anp_client_write_notification_handler(void *param);
#endif /* __ANP_CLIENT_H__*/