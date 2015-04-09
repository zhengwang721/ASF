/**
 * \file
 *
 * \brief ADP service implementation
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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

#ifndef ADP_PACKET_H_INCLUDED
#define ADP_PACKET_H_INCLUDED

#include <compiler.h>
#include "adp_interface.h"

#define ADP_MAX_PACKET_DATA_SIZE 254
#define ADP_TOKEN 0xFF

void adp_packet_init(void);

bool adp_packet_is_received(void);
uint8_t adp_packet_received_get_id(void);
void adp_packet_send_header(uint8_t message_id, uint8_t message_length);
void adp_packet_send_data(uint8_t* data, uint8_t data_length);
bool adp_packet_transceive_header(uint8_t message_id, uint8_t message_length, uint8_t* rx_buf);
bool adp_packet_transceive_data(uint8_t* data, uint8_t data_length, uint8_t* rx_buf);
void adp_packet_wait_for_response(uint8_t received_id, uint8_t *receive_buf, uint8_t receive_buffer_size);
bool adp_packet_check_for_response(uint8_t received_id, uint8_t *receive_buf, uint8_t receive_buffer_size);
bool adp_packet_receive_packet_data(uint8_t *receive_buf);

#endif
