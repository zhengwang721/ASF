/**
 * \file
 *
 * \brief DLMS Application Emulator for ATMEL PRIME v.1.3 Service Node
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
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



#include <string.h>
#include <stdio.h>

#include "asf.h"
#include "compiler.h"
#include "dlms_emu.h"



/**
 * \defgroup thirdparty_prime_group Short Cycles Application
 *
 * \par Purpose
 *
 * This application shows how PRIME interfaces should be used.
 *
 */

/* Internal Function declarations */
static void prvProcessDLMSemuApp(void * pvParameters);

/* Tasks handlers */
xTaskHandle xCyclesHnd;
/* Timers handlers */
xTimerHandle xUpdateCyclesTimer;


#define STATUS_432_CONNECTION    PIB_432_CONNECTION_STATE
#define MAX_TIME_BETWEEN_MSGS    10000    // miliseconds

// Serial Number buffer
static uint8_t puc_serial_number[13+1];

enum {
	CH_SEND_CHECK,
	CH_WAIT_ANSWER
} stCheckNode;

static uint8_t uc_mlme_cmd[MLME_DATA_SIZE];
static uint16_t us_base_node_address;

static dl432Buffer_t x_cmd_tx_432;
static dl432Buffer_t x_cmd_rx_432;
static dl432_LlcCallback_t x_rcv_432;

/// variables needed to generate serial number from mac address
typedef struct
{
	uint16_t cfgMagic;
	uint8_t _mac[6];
}MacCfg;


#define LEN_ASSOCIATION_RSP    43
static const uint8_t puc_association_rsp[LEN_ASSOCIATION_RSP] = {0x61, 0x29,
	0xA1, 0x09, 0x06, 0x07, 0x60, 0x85, 0x74, 0x05, 0x08, 0x01, 0x01, 0xA2, 0x03,
	0x02, 0x01, 0x00, 0xA3, 0x05, 0xA1, 0x03, 0x02, 0x01, 0x00, 0xBE, 0x10, 0x04,
	0x0E, 0x08, 0x00, 0x06, 0x5F, 0x1F, 0x04, 0x00, 0x00, 0x10, 0x14, 0x00, 0x00,
	0x00, 0x07};

#define LEN_OBIS_0BJ_0        16
static const uint8_t puc_obis_obj_0[LEN_OBIS_0BJ_0] = {0xC4 , 0x01 , 0x00 ,
	0x00 , 0x09 , 0x0A , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
	0x00 , 0x00 };

#define LEN_OBIS_0BJ_1        12
static const uint8_t puc_obis_obj_1[LEN_OBIS_0BJ_1] = {0xC4 , 0x01 , 0x00 ,
	0x00 , 0x09 , 0x06 , 0x20 , 0x48 , 0x41 , 0x42 , 0x31 , 0x32};

#define LEN_OBIS_0BJ_2        24
static const uint8_t puc_obis_obj_2[LEN_OBIS_0BJ_2] = {0xC4 , 0x01 , 0x00 ,
	0x00 , 0x09 , 0x12 , 0x63 , 0x6F , 0x6E , 0x74 , 0x61 , 0x64 , 0x6F , 0x72 ,
	0x20 , 0x20 , 0x44 , 0x4C , 0x4D , 0x53 , 0x30 , 0x31 , 0x30 , 0x35};

#define LEN_OBIS_0BJ_3        9
static const uint8_t puc_obis_obj_3[LEN_OBIS_0BJ_3] = {0xC4 , 0x01 , 0x00 ,
	0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00};

#define LEN_OBIS_0BJ_4        253
static const uint8_t puc_obis_obj_4[LEN_OBIS_0BJ_4] = {0xC4 , 0x02 , 0x00 ,
	0x00 , 0x00 , 0x00 , 0x00 , 0x01 , 0x00 , 0x81 , 0xF2 , 0x01 , 0x0D , 0x02 ,
	0x08 , 0x09 , 0x0C , 0x07 , 0xDD , 0x08 , 0x05 , 0x01 , 0x14 , 0x00 , 0x00 ,
	0xFF , 0x80 , 0x00 , 0x80 , 0x11 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 ,
	0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 ,
	0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 ,
	0x00 , 0x00 , 0x00 , 0x02 , 0x08 , 0x09 , 0x0C , 0x07 , 0xDD , 0x08 , 0x05 ,
	0x01 , 0x15 , 0x00 , 0x00 , 0xFF , 0x80 , 0x00 , 0x80 , 0x11 , 0x00 , 0x06 ,
	0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 ,
	0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 ,
	0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x02 , 0x08 , 0x09 , 0x0C ,
	0x07 , 0xDD , 0x08 , 0x05 , 0x01 , 0x16 , 0x00 , 0x00 , 0xFF , 0x80 , 0x00 ,
	0x80 , 0x11 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 ,
	0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 ,
	0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 ,
	0x02 , 0x08 , 0x09 , 0x0C , 0x07 , 0xDD , 0x08 , 0x05 , 0x01 , 0x17 , 0x00 ,
	0x00 , 0xFF , 0x80 , 0x00 , 0x80 , 0x11 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 ,
	0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 ,
	0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 ,
	0x00 , 0x00 , 0x00 , 0x00 , 0x02 , 0x08 , 0x09 , 0x0C , 0x07 , 0xDD , 0x08 ,
	0x06 , 0x02 , 0x00 , 0x00 , 0x00 , 0xFF , 0x80 , 0x00 , 0x80 , 0x11 , 0x00 ,
	0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 ,
	0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 ,
	0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00};

#define LEN_OBIS_0BJ_5        114
static const uint8_t puc_obis_obj_5[LEN_OBIS_0BJ_5] = {0xC4 , 0x01 , 0x00 ,
	0x00 , 0x01 , 0x01 , 0x02 , 0x07 , 0x09 , 0x0C , 0x07 , 0xDD , 0x08 , 0x06 ,
	0x02 , 0x12 , 0x00 , 0x01 , 0xFF , 0x80 , 0x00 , 0x80 , 0x06 , 0x00 , 0x00 ,
	0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 ,
	0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x00 ,
	0x06 , 0x00 , 0x00 , 0x00 , 0x00};

#define LEN_OBIS_0BJ_6        18
static const uint8_t puc_obis_obj_6[LEN_OBIS_0BJ_6] = {0xC4 , 0x01 , 0x00 ,
	0x00 , 0x09 , 0x0C , 0x07 , 0xDD , 0x08 , 0x06 , 0x02 , 0x12 , 0x01 , 0x1B ,
	0xFF , 0x80 , 0x00 , 0x80};

#define LEN_OBIS_0BJ_7        251
static const uint8_t puc_obis_obj_7[LEN_OBIS_0BJ_7] = { 0xC4, 0x02, 0xC2, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x00, 0x81, 0xF0, 0x02, 0x08, 0x09, 0x0C, 0x07, 0xDD,
	0x08, 0x06, 0x02, 0x01, 0x00, 0x00, 0xFF, 0x80, 0x00, 0x80, 0x11, 0x00, 0x06,
	0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x08, 0x09, 0x0C, 0x07, 0xDD, 0x08, 0x06, 0x02, 0x02,
	0x00, 0x00, 0xFF, 0x80, 0x00, 0x80, 0x11, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00,
	0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x02,
	0x08, 0x09, 0x0C, 0x07, 0xDD, 0x08, 0x06, 0x02, 0x03, 0x00, 0x00, 0xFF, 0x80,
	0x00, 0x80, 0x11, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
	0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x02, 0x08, 0x09, 0x0C, 0x07,
	0xDD, 0x08, 0x06, 0x02, 0x04, 0x00, 0x00, 0xFF, 0x80, 0x00, 0x80, 0x11, 0x00,
	0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06,
	0x00, 0x00, 0x00, 0x00, 0x02, 0x08, 0x09, 0x0C, 0x07, 0xDD, 0x08, 0x06, 0x02,
	0x05, 0x00, 0x00, 0xFF, 0x80, 0x00, 0x80, 0x11, 0x00, 0x06, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
	0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00};

#define LEN_OBIS_0BJ_8        155
static const uint8_t puc_obis_obj_8[LEN_OBIS_0BJ_8] = { 0xC4, 0x02, 0xC2, 0x01,
	0x00, 0x00, 0x00, 0x03, 0x00, 0x81, 0x90, 0x02, 0x08, 0x09, 0x0C, 0x07, 0xDD,
	0x08, 0x06, 0x02, 0x06, 0x00, 0x00, 0xFF, 0x80, 0x00, 0x80, 0x11, 0x00, 0x06,
	0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x08, 0x09, 0x0C, 0x07, 0xDD, 0x08, 0x06, 0x02, 0x07,
	0x00, 0x00, 0xFF, 0x80, 0x00, 0x80, 0x11, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00,
	0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x02,
	0x08, 0x09, 0x0C, 0x07, 0xDD, 0x08, 0x06, 0x02, 0x08, 0x00, 0x00, 0xFF, 0x80,
	0x00, 0x80, 0x11, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
	0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00};

#define LEN_OBIS_0BJ_9        11
static const uint8_t puc_obis_obj_9[LEN_OBIS_0BJ_9] = {0xC4, 0x01, 0xC1, 0x00,
	0x09, 0x05, 0x56, 0x39, 0x33, 0x30, 0x30};

#define LEN_OBIS_0BJ_10        22
static const uint8_t puc_obis_obj_10[LEN_OBIS_0BJ_10] = {0xC4, 0x01, 0xC2, 0x00,
	0x09, 0x10, 0x30, 0x31, 0x30, 0x33, 0x30, 0x38, 0x30, 0x31, 0x44, 0x42, 0x55,
	0x00, 0x00, 0x00, 0x00, 0x00};

#define LEN_OBIS_0BJ_11        4
static const uint8_t puc_obis_obj_11[LEN_OBIS_0BJ_11] = {0xC5, 0x01, 0xC1, 0x02};

#define LEN_OBIS_0BJ_12        66
static const uint8_t puc_obis_obj_12[LEN_OBIS_0BJ_12] = {0xC4, 0x01, 0xC1, 0x00,
	0x01, 0x01, 0x02, 0x0D, 0x09, 0x0C, 0x07, 0xDE, 0x04, 0x08, 0x02, 0x11, 0x0D,
	0x05, 0xFF, 0x00, 0x78, 0x80, 0x12, 0x00, 0xFD, 0x12, 0x00, 0x00, 0x12, 0x00,
	0x00, 0x12, 0x00, 0x00, 0x12, 0x00, 0x00, 0x12, 0x00, 0x00, 0x12, 0x00, 0x00,
	0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00};

#define LEN_OBIS_0BJ_13        52
static const uint8_t puc_obis_obj_13[LEN_OBIS_0BJ_13] = {0xC4, 0x01, 0xC2, 0x00,
	0x01, 0x01, 0x02, 0x07, 0x09, 0x0C, 0x07, 0xDE, 0x04, 0x08, 0x02, 0x10, 0x0A,
	0x32, 0xFF, 0x00, 0x78, 0x80, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06,
	0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00};

#define LEN_RELEASE_RSP        5
static const uint8_t puc_release_rsp[LEN_RELEASE_RSP] = { 0x63, 0x03, 0x80,
	0x01, 0x00};

/***************************************************************************
* Convert functions
***************************************************************************/
static uint8_t _convertHEX2STR (uint8_t num)
{
	if (num>9){
		return (num+0x37);
	}else{
		return (num+0x30);
	}
}

static uint8_t _convertSTR2HEX ( uint8_t  *num )
{
	uint8_t value;
	uint8_t num1,num2;

	num1 = *num;
	num2 = *(num+1);

	num1 = (num1>0x39)? num1 - 0x37: num1 - 0x30;
	num2 = (num2>0x39)? num2 - 0x37: num2 - 0x30;

	value = (num1 * 16 ) | num2;
	return value;
}

static void _convertstringtohex ( uint8_t * target, const uint8_t * source, const int len)
{
	int16_t i;
	uint8_t bytecodevalue;

	for ( i=0 ; i<len ; i=i+2) {
		bytecodevalue = _convertSTR2HEX ((uint8_t *) (source+i));
		*(target+i/2) = bytecodevalue;
	}
}

static void _generate_serial(uint8_t * stringSerialBoard)
{
	uint8_t i ;
	uint8_t num,num1;
	MacCfg mac;

	hal_read_mac_data_cfg(&mac, sizeof(mac));

	*stringSerialBoard = 'A';
	*(stringSerialBoard+1) = 'T';
	*(stringSerialBoard+2) = 'M';
	// convert hex to ascii
	for (i=1;i<6; i++){
		num  = ((mac._mac[i] & 0xf0)>>4);
		num1 = (mac._mac[i] &0x0f);
		stringSerialBoard[2*(i)+1] = _convertHEX2STR(num);
		stringSerialBoard[2*(i)+2] =_convertHEX2STR(num1);
	}
	*(stringSerialBoard+13)=0x00;
}

//*************************************************************************
/**  build associate resquest with low security response
***************************************************************************/
static uint16_t MSG_AssociationResponse ( uint8_t * msg,  uint8_t ack,
					uint8_t ACSEserviceUserValue,
					uint8_t NegotiateConformance ,
					uint16_t  maxPDU)
{
	// associate response
	// ack ="00" association accepted
	// ack ="01" association not accepted.
	// conformance = 0x01 must handshake security protocol.
	// ACSEserviceUserValue =0
	// pduVaule

	uint8_t *puc_msg_aux = msg;

	memcpy( puc_msg_aux, puc_association_rsp, LEN_ASSOCIATION_RSP);

	*(puc_msg_aux+ 17) = ack;
	*(puc_msg_aux+ 24) = ACSEserviceUserValue;
	*(puc_msg_aux+ 39) = (uint8_t) ((maxPDU & 0xff00)>>8);
	*(puc_msg_aux+ 40) = (uint8_t) ((maxPDU & 0x00FF));

	if ((NegotiateConformance == 1) && (ACSEserviceUserValue == 1)) {
		// answer to request 601D
		*(puc_msg_aux+ 37) = 0x00;
		*(puc_msg_aux+ 38) = 0x10;
	} else {
		if (NegotiateConformance == 1) {
			// answer to 6021 request
			*(puc_msg_aux+ 17) = 0x01;  //association result
			*(puc_msg_aux+ 24) = 0x0D;  // Authentication not accepted
			*(puc_msg_aux+ 38) = 0x14;
		} else {
			// answer to 6036
			*(puc_msg_aux+ 17) = 00;  //association result
			*(puc_msg_aux+ 24) = 00;  // Authentication accepted
			*(puc_msg_aux+ 38) = 0x1D;
		}
	}

#ifdef DLMS_EMU_DEBUG_ENABLE
	printf ("MSG_AssociationResponse<< 6129\r\n");
#endif

	return (LEN_ASSOCIATION_RSP);
}

static uint16_t MSG_GetResponse( uint8_t * msg,  uint8_t  InvokeIDandPriority,
				uint8_t * bufferOBIS, uint8_t * string_MAC)
{
	uint8_t OBIS[6];

#ifdef DLMS_EMU_DEBUG_ENABLE
	printf ("MSG_GetResponse<< C402 \r\n");
#endif

	memcpy(OBIS, bufferOBIS, 6);

	/// obis version dlms
	if ((OBIS[0]==0x01) && (OBIS[1]==0) && (OBIS[2]==0X00)  && (OBIS[3]==0X02) && (OBIS[4]==0X00) ){
		memcpy(msg, puc_obis_obj_9, LEN_OBIS_0BJ_9);
		return LEN_OBIS_0BJ_9;
	}
	// obis version prime
	if ((OBIS[0]==0x00) && (OBIS[1]==0x00) && (OBIS[2]==0X1c)  && (OBIS[3]==0X07) && (OBIS[4]==0X00)){
		memcpy(msg, puc_obis_obj_10, LEN_OBIS_0BJ_10);
		return LEN_OBIS_0BJ_10;
	}
	// obis meter details
	if ((OBIS[0]==0x00) && (OBIS[1]==0x00) && (OBIS[2]==0X15)  && (OBIS[3]==0X00) && (OBIS[4]==0X05)){
		memcpy(msg, puc_obis_obj_12, LEN_OBIS_0BJ_12);
		return LEN_OBIS_0BJ_12;
	}
	// obis meter details
	if ((OBIS[0]==0x00) && (OBIS[1]==0x00) && (OBIS[2]==0X15)  && (OBIS[3]==0X00) && (OBIS[4]==0X06)){
		memcpy(msg, puc_obis_obj_13, LEN_OBIS_0BJ_13);
		return LEN_OBIS_0BJ_13;
	}

	if ((OBIS[0]==0) && (OBIS[1]==0) && (OBIS[2]==0X60)  && (OBIS[3]==0X01) ){
		if (OBIS[4] == 0X00) {
			memcpy(msg, puc_obis_obj_0, LEN_OBIS_0BJ_0);
			_convertstringtohex(&msg[6], string_MAC, sizeof(string_MAC));
			msg[2] = InvokeIDandPriority;
			return LEN_OBIS_0BJ_0;
		} else if (OBIS[4]==0X01) {
			memcpy(msg, puc_obis_obj_1, LEN_OBIS_0BJ_1);
			msg[2] = InvokeIDandPriority;
			return LEN_OBIS_0BJ_1;
		} else if (OBIS[4]==0X02) {
			memcpy(msg, puc_obis_obj_2, LEN_OBIS_0BJ_2);
			msg[2] = InvokeIDandPriority;
			return LEN_OBIS_0BJ_2;
		}
	}

	if ((OBIS[0]==01) && (OBIS[1]==00) && (OBIS[2]==0X01)  && (OBIS[3]==0X08)
		&& (OBIS[4]==0X0A)  ){
		memcpy(msg, puc_obis_obj_3, LEN_OBIS_0BJ_3);
		msg[2] = InvokeIDandPriority;
		return LEN_OBIS_0BJ_3;
	}

	if ((OBIS[0]==01) && (OBIS[1]==00) && (OBIS[2]==0X63)  && (OBIS[3]==0X01)  && (OBIS[4]==0X00)  ){
		memcpy(msg, puc_obis_obj_4, LEN_OBIS_0BJ_4);
		msg[2] = InvokeIDandPriority;
		return LEN_OBIS_0BJ_4;
	}

	if ((OBIS[0]==00) && (OBIS[1]==00) && (OBIS[2]==0X15)  && (OBIS[3]==0X00)  && (OBIS[4]==0X06)  ){
		memcpy(msg, puc_obis_obj_5, LEN_OBIS_0BJ_5);
		msg[2] = InvokeIDandPriority;
		return LEN_OBIS_0BJ_5;
	}

	if ((OBIS[0]==00) && (OBIS[1]==00) && (OBIS[2]==0X01)  && (OBIS[3]==0X00)  && (OBIS[4]==0X00)  ){
		memcpy(msg, puc_obis_obj_6, LEN_OBIS_0BJ_6);
		msg[2] = InvokeIDandPriority;
		return LEN_OBIS_0BJ_6;
	}

	return (0); // obis not found
}

static uint16_t MSG_GetResponseLCNextBlock ( uint8_t * msg, uint8_t lastblock)
{
#ifdef DLMS_EMU_DEBUG_ENABLE
	printf ("MSG_GetResponseLCNextBlock>> C402\r\n");
#endif

	if (!lastblock){
		memcpy(msg, puc_obis_obj_7, LEN_OBIS_0BJ_7);
		return LEN_OBIS_0BJ_7;
	} else {
		memcpy(msg, puc_obis_obj_8, LEN_OBIS_0BJ_8);
		return LEN_OBIS_0BJ_8;
	}

}

//***********************************************************************************
/**  build release response
*************************************************************************************/
static uint16_t MSG_ReleaseResponse (uint8_t * msg, uint8_t  value)
{
	memcpy(msg, puc_release_rsp, LEN_RELEASE_RSP);
	msg[4] = value ;

#ifdef DLMS_EMU_DEBUG_ENABLE
	printf ("MSG_ReleaseResponse<< 6308\r\n");
#endif

	return LEN_RELEASE_RSP;
}


//***********************************************************************************
/**  send DLMS answer
*************************************************************************************/
static void DLMSMessageSend ( uint16_t lengthmsg)
{
	if(dl432_dl_data_request(x_cmd_rx_432.dl.lsap, x_cmd_rx_432.dl.dsap, us_base_node_address, 
		&x_cmd_tx_432, lengthmsg, 0)==-1){
#ifdef DLMS_EMU_DEBUG_ENABLE
		printf("Error. Indication Not response\r\n");
	}else{
		printf("Indication Response OK. Length: %d\r\n", lengthmsg);
#endif
	}
}
//***********************************************************************************
/**
*************************************************************************************/
static uint16_t _checkReceivedData(uint8_t* data)
{
	uint8_t value;
	uint16_t AnswerCode = 0x0000;
	uint8_t InvokeAndPriority;
	uint16_t blocklenght;
	uint8_t byteCode1,byteCode2;

	byteCode1 = *(data);
	byteCode2 = *(data+1);

	AnswerCode = byteCode1*256 + byteCode2;
#ifdef DLMS_EMU_DEBUG_ENABLE
	printf("received  %X\r\n", AnswerCode);
#endif

	switch (byteCode1){
		case 0x60:
			/// association request... check type association request with bytecode2
			switch (byteCode2){
				case 0x21: // association without security refused
#ifdef DLMS_EMU_DEBUG_ENABLE
				printf("association request received>> 6021\r\n");
#endif
				blocklenght = MSG_AssociationResponse ( x_cmd_tx_432.dl.buff, 0, 0,
								 1, 0x0100);
				DLMSMessageSend(blocklenght);
				break;

				case 0x1D:
#ifdef DLMS_EMU_DEBUG_ENABLE
				printf("registration association request>> 601D\r\n");
#endif
				blocklenght = MSG_AssociationResponse ( x_cmd_tx_432.dl.buff, 0, 1,
								 1,0x01F4);
				DLMSMessageSend(blocklenght);
				break;

				case 0x36: // association with low security accepted
#ifdef DLMS_EMU_DEBUG_ENABLE
				printf("registration association request>> 6036\r\n");
#endif
				blocklenght = MSG_AssociationResponse ( x_cmd_tx_432.dl.buff, 0, 1,
								 0, 0x0100);
				DLMSMessageSend(blocklenght);
				break;

				default:
				//error unexpected message.
				// no actions
				break;
			}
			break;

		case 0x62:  // return release response.
#ifdef DLMS_EMU_DEBUG_ENABLE
			printf("release request received>> 6200\r\n");
#endif
			blocklenght = MSG_ReleaseResponse ((uint8_t *)x_cmd_tx_432.dl.buff, 0x00);
			DLMSMessageSend(blocklenght);
			break;

		case 0xc0:
#ifdef DLMS_EMU_DEBUG_ENABLE
			printf("get request >> %X\r\n",AnswerCode);
	#endif

			switch (byteCode2) {
				case 0x01:
					//** get request order number *//
					InvokeAndPriority = *(data+2);
					blocklenght = MSG_GetResponse ( x_cmd_tx_432.dl.buff,
								InvokeAndPriority, data+5,
								&puc_serial_number[3]);
					if (blocklenght) {
						DLMSMessageSend(blocklenght);
					}
#ifdef DLMS_EMU_DEBUG_ENABLE
					else{
						printf("instance ID desconocido >%s\r\n",x_cmd_rx_432.dl.buff);
					}
#endif
					break;

				case 0x02:
					value =*(data+6); // next block
					switch ( value) {
						case 0x01:
							blocklenght = MSG_GetResponseLCNextBlock(x_cmd_tx_432.dl.buff, 0);
							DLMSMessageSend(blocklenght);
							break;

						case 0x02:
							blocklenght = MSG_GetResponseLCNextBlock(x_cmd_tx_432.dl.buff, 1);
							DLMSMessageSend(blocklenght);
							break;

						default: // no more blocks to send
	#ifdef DLMS_EMU_DEBUG_ENABLE
						printf(" no more block to send...why?\r\n");
	#endif
						break;
					}
					break;

				default :
#ifdef DLMS_EMU_DEBUG_ENABLE
				printf(" not ready to this get request message\r\n");
#endif
				break;
			}
			break;

		case 0xC1:
			if (byteCode2 == 0x01) {
				uint32_t * obis = (uint32_t *)(data + 5);
				if (*obis == 0x00010000) {
				memcpy(x_cmd_tx_432.dl.buff, puc_obis_obj_11, LEN_OBIS_0BJ_11);
				DLMSMessageSend(LEN_OBIS_0BJ_11);
				}
			}
			break;

		default:
		//unexpected bytecode
#ifdef DLMS_EMU_DEBUG_ENABLE
		printf(" not ready to this bytecode :%d\r\n",AnswerCode );
#endif
		break;
	}

	return AnswerCode;
}

//***********************************************************************************
/**
*************************************************************************************/
static Bool _checkNodestatus(uint32_t *status)
{
	uint16_t pibAttrib;
	Bool end = false;

	switch (stCheckNode){
		case CH_SEND_CHECK:
			prime_MLME_GET_request(STATUS_432_CONNECTION);
			stCheckNode = CH_WAIT_ANSWER;
			break;

		case CH_WAIT_ANSWER:
			if (prime_MLME_callback(uc_mlme_cmd)) {
				pibAttrib = (uint16_t)(uc_mlme_cmd[2] << 8) + (uint16_t)uc_mlme_cmd[3];
				if ((uc_mlme_cmd[0] == PRIME_MLME_GET_CONFIRM) &&
					(pibAttrib == STATUS_432_CONNECTION)) {
					if (uc_mlme_cmd[1] == PRIME_MLME_RESULT_SUCCESS) {
						// Get pibValue
						*status = (uint16_t)(uc_mlme_cmd[4] << 24) +
						(uint16_t)(uc_mlme_cmd[5] << 16) +
						(uint16_t)(uc_mlme_cmd[6] << 8) +
						(uint16_t)uc_mlme_cmd[7];
					} else {
						*status = 0; // node disconnected
					}
					stCheckNode = CH_SEND_CHECK;
					end = true;
				}
			}
			break;

		default:
			stCheckNode = CH_SEND_CHECK;
			break;
	}
	return end;
}

//***********************************************************************************
/**
*************************************************************************************/
static void _checkReceivedMessages(void)
{
	sscs432_NotService_t newNot;
	uint16_t DLMSstatus;

	if (sscs432_poll_establish_release(&newNot)) {
		if (newNot.cmd == CL_432_RELEASE_CONFIRM) {
#ifdef DLMS_EMU_DEBUG_ENABLE
			printf("4-32 Connection CLOSED %s \r\n",puc_serial_number);
#endif
		} else {
			us_base_node_address = newNot.baseAddress;
#ifdef DLMS_EMU_DEBUG_ENABLE
			printf("4-32 Connection Opened %s\r\n",puc_serial_number);
#endif
		}
	}

	dl432_dl_callback(&x_rcv_432, &x_cmd_rx_432, MAX_LENGTH_432_DATA);
	if (x_rcv_432.cmd == DL432_DATA_INDICATION) {
		DLMSstatus = _checkReceivedData(x_cmd_rx_432.dl.buff);
		switch (DLMSstatus) {
			case 0x0000:
#ifdef DLMS_EMU_DEBUG_ENABLE
				printf("Error. Bad Data integrity\r\n");
#endif
				break;

			case 0x6021:
			case 0x6036:
			case 0x601D:
			case 0xc001:
			case 0xc002:
			case 0x6200:
				break;

			default:
#ifdef DLMS_EMU_DEBUG_ENABLE
				printf("Error unexpected message  %X\r\n ", DLMSstatus);
#endif
				break;
		}
	}
}

/**
* \brief Create main Cycles Application task and create timer to update internal counters.
*
*/
void vDLMSemuTask(void)
{
	_generate_serial(puc_serial_number);
#ifdef DLMS_EMU_DEBUG_ENABLE
	printf("DLMS EMU Application: Service Node...\r\n");
#endif

	/* Create new task to Cycles Application */
	xTaskCreate(prvProcessDLMSemuApp, (const signed char * const)"DLMSEmuTask",
	TASK_DLMSEMU_LAYER_STACK, NULL, TASK_DLMSEMU_LAYER_PRIO, &xCyclesHnd);
}

/**
* \brief Periodic task to process Cycles App. Initialize and start Cycles Application and launch timer
* to update internal counters.
*
*/
static void prvProcessDLMSemuApp(void * pvParameters)
{
	static portTickType xLastWakeTime;
	static portTickType xPeriod;
	static uint8_t uc_counter_chk_conn;
	uint32_t nodeConnected;

	UNUSED(pvParameters);

	/* Init variables and status */
	stCheckNode = CH_SEND_CHECK;
	uc_counter_chk_conn = PRIME_CHECK_CONNECTION_RATE;

	xPeriod = PRIME_DLMSEMU_APP_TIMER_RATE;
	xLastWakeTime = xTaskGetTickCount();
	for(;;){
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

		taskENTER_CRITICAL();
		_checkReceivedMessages();
		if(!uc_counter_chk_conn--){
			uc_counter_chk_conn = PRIME_CHECK_CONNECTION_RATE;
			if (_checkNodestatus(&nodeConnected)){
				if (!nodeConnected){ // launch connection-
					sscs432_establish_request((uint8_t*) puc_serial_number, strlen((const char *) puc_serial_number));
				}
			}
		}
		taskEXIT_CRITICAL();
	}
}
