/**
* \file
*
* \brief The functions of UART driver
*
* Copyright (c) 2012-2013 Pervasive Displays Inc. All rights reserved.
*
*  Authors: Pervasive Displays Inc.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*  1. Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "EPD_Kit_Tool_Process.h"
#include "Uart_Driver.h"


static receive_event_handler _RxEventHandle;
static bool my_flag_autorize_cdc_transfert = false;
static uint8_t activeport=0;

/** \brief Initialize the Rx event and start USB Device stack 
 */
void data_interface_init(receive_event_handler OnRxEventHandle) {
	_RxEventHandle=OnRxEventHandle;
	//udc_start();
}

/** \brief Transmit data to UART 
 */
void data_transmit (uint8_t *s,uint8_t len) {
	//udi_cdc_multi_write_buf(activeport,s,len);
}

bool my_callback_cdc_enable(uint8_t port) {
	my_flag_autorize_cdc_transfert = true;
	activeport=port;
	return true;
}
void my_callback_cdc_disable(uint8_t port) {
	my_flag_autorize_cdc_transfert = false;
	activeport=port;
}
void my_callback_rx_notify(uint8_t port) {
	//uint32_t len=udi_cdc_multi_get_nb_received_data(port);
	//uint8_t *buf;
	//if(len>0) {
		//if(_RxEventHandle!=NULL) {
			//buf=malloc(len);
			//udi_cdc_multi_read_buf(port,buf,len);
			//_RxEventHandle(buf,len);
			//free(buf);
		//}
	//}
}
