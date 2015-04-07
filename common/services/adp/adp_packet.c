/**
 * \file
 *
 * \brief ADP service implementation
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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

#include <compiler.h>

#include "adp_packet.h"
#include "adp_interface.h"
#include "adp.h"
#include "status_codes.h"

/** ID of last received message */
static uint8_t last_received_message_id = 0xff;
/** true if valid message has been received */
static bool packet_received = false;
/** Number of bytes received */
static uint16_t bytes_received;

/** States in receive state machine */
enum rx_state_e {
	/** We are idle, waiting for a new packet */
	RX_STATE_IDLE,
	/** Start symbol received, waiting for Message ID */
	RX_STATE_WAIT_MSG_ID,
	/** Message ID received, waiting for data length */
	RX_STATE_WAIT_LENGTH,
	/** Length received; we are receiving packet data */
	RX_STATE_GET_DATA,
	/** Start symbol received */
	RX_STATE_GOT_SYMBOL,
};

/** Current state */
static enum rx_state_e rx_state;


/**
* \brief Initialize ADP packet handling
*
*/
void adp_packet_init(void)
{
	adp_interface_init();
}


/**
* \internal Handle incomming data byte
*
* [More detailed description of the function, \note \warning and so on can
* be used to emphasize important information.]
*
* \param[in] data New data byte to handle
*
* \return
* \retval true Given data byte is part of message data
* \retval false Given data byte is not part of message data
*/
static bool adp_packet_add_byte(uint8_t data)
{
	static uint8_t rx_data_len;
	static uint8_t message_id;

	if ((rx_state == RX_STATE_GOT_SYMBOL) && (data != ADP_TOKEN)) {
		/* Abort packet reception, new packet incoming */
		rx_state = RX_STATE_WAIT_LENGTH;
	}

	switch (rx_state) {

	case RX_STATE_IDLE:

		packet_received = false;
		last_received_message_id = 0xFF;
		/* We are waiting for a new packet. */
		if (data != ADP_TOKEN) {
			return false;
		}
		/* Got start symbol, wait for message ID */
		rx_state = RX_STATE_WAIT_MSG_ID;
		return false;

	case RX_STATE_WAIT_MSG_ID:
		if (data == ADP_TOKEN) {
			/* Restart. Don't change state. Wait for new message ID */
			return false;
		}
		message_id = data;
		rx_state = RX_STATE_WAIT_LENGTH;
		return false;

	case RX_STATE_WAIT_LENGTH:
		if (data == ADP_TOKEN) {
			/* Restart packet reception. Wait for new message ID */
			rx_state = RX_STATE_WAIT_MSG_ID;
			return false;
		}
		/* Got valid length, do we expect data? */
		if (data == 0) {
			/* No data here, wait for next packet */
			rx_state = RX_STATE_IDLE;
			packet_received = true;
			last_received_message_id = message_id;
			return false;
		}

		/* Wait for packet data */
		bytes_received = 0;
		rx_state = RX_STATE_GET_DATA;
		rx_data_len = data;
		return false;

	case RX_STATE_GET_DATA:
	case RX_STATE_GOT_SYMBOL:
		if ((data == ADP_TOKEN) && (rx_state == RX_STATE_GET_DATA)) {
			rx_state = RX_STATE_GOT_SYMBOL;
			return false;
		}
		/* Add new data to rx buffer */
		bytes_received++;
		/* Are we done yet? */
		if (rx_data_len == bytes_received) {
			/* Yes we are! */
			packet_received = true;
			rx_state = RX_STATE_IDLE;
			last_received_message_id = message_id;
			return true;
		}
		/* Not done yet.. keep on receiving */
		rx_state = RX_STATE_GET_DATA;
		return true;
	}
	return false;
}


/**
* \brief [Brief description of the function]
*
* [More detailed description of the function, \note \warning and so on can
* be used to emphasize important information.]
*
* \param[out] [param 1 name] [param description]
* \param[in] [param 2 name] [param description]
* \param[in,out] [param n name] [param description]
*
* \return [description of what the function returns]
* \retval [actual return value] [and what they mean]
* \retval [actual return value] [and what they mean]
* \retval [actual return value] [and what they mean]
*/
bool adp_packet_is_received(void)
{
	if (bytes_received == 0) {
		return false;
	}
	return packet_received;
}

uint8_t adp_packet_received_get_id(void)
{

	return last_received_message_id;
}


/**
* \brief Send packet header
*
* \param[in] message_id ID of message
* \param[in] message_length Length of message
*/
void adp_packet_send_header(uint8_t message_id, uint8_t message_length)
{
	uint8_t header[3];

	/* Build header */
	header[0] = ADP_TOKEN;
	header[1] = message_id;
	header[2] = message_length;
	/* Send header */
	adp_interface_send(header, 3);
}



/**
* \brief Wait for response from PC
*
* \warning This function is blocking until we receive the packet we expect
*
* \param[in]  received_id         Expected message ID of packet to receive
* \param[out] receive_buf         Pointer to received message data
* \param[in]  receive_buffer_size Maximum size of message data
*/
void adp_packet_wait_for_response(uint8_t received_id, uint8_t *receive_buf, uint8_t receive_buffer_size)
{
	uint8_t rx_byte;
	uint8_t receive_offset = 0;

	packet_received = false;

	adp_interface_send_start();
	while (adp_packet_is_received() == false) {
		/* lalala, let's wait.. lala */

		adp_interface_read(&rx_byte, 1);
		if (adp_packet_add_byte(rx_byte) == true) {
			/* This is a data byte */
			receive_buf[receive_offset] = rx_byte;
			if (receive_offset < receive_buffer_size) {
				receive_offset++;
			} // else: something is wrong.
		}
	}
	adp_interface_send_stop();
	/* Packet received, check content */
	if (adp_packet_received_get_id() != received_id) {
		/* Error. Invalid message received */
	}

}


/**
* \brief Wait for response from PC
*
* Non-blocking function. Will receive n bytes before it gives up
*
* \param[in]  received_id         Expected message ID of packet to receive
* \param[out] receive_buf         Pointer to received message data
* \param[in]  receive_buffer_size Maximum size of message data
*/
bool adp_packet_check_for_response(uint8_t received_id, uint8_t *receive_buf, uint8_t receive_buffer_size)
{
	uint8_t rx_byte;
	uint8_t retry;
	uint8_t receive_offset = 0;

	packet_received = false;

	retry = 50;

	adp_interface_send_start();
	while ( (adp_packet_is_received() == false) & (retry-- > 0) ) {
		/* lalala, let's wait.. lala */
		adp_interface_read(&rx_byte, 1);
		if (adp_packet_add_byte(rx_byte) == true) {
			/* This is a data byte */
			receive_buf[receive_offset] = rx_byte;
			if (receive_offset < receive_buffer_size) {
				receive_offset++;
			} // else: something is wrong.
		}
	}
	adp_interface_send_stop();

	if (adp_packet_is_received() & (adp_packet_received_get_id() == received_id)) {
		return true;
	}
	return false;
}

/**
* \brief Receive data packet from PC
*
* Non-blocking function. Will receive n bytes before it gives up
*
* \param[in]  received_id         Expected message ID of packet to receive
* \param[out] receive_buf         Pointer to received message data
* \param[in]  receive_buffer_size Maximum size of message data
*/
bool adp_packet_receive_packet_data(uint8_t *receive_buf)
{
	uint8_t rx_byte;
	uint8_t retry;
	uint8_t receive_offset = 0;

	packet_received = false;

	retry = MSG_RES_PACKET_DATA_MAX_LEN;

	adp_interface_send_start();
	while ( (adp_packet_is_received() == false) & (retry-- > 0) ) {
		/* lalala, let's wait.. lala */
		adp_interface_read(&rx_byte, 1);
		if (adp_packet_add_byte(rx_byte) == true) {
			/* This is a data byte */
			receive_buf[receive_offset] = rx_byte;
			if (receive_offset < MSG_RES_PACKET_DATA_MAX_LEN) {
				receive_offset++;
			} // else: something is wrong.
		}
	}
	adp_interface_send_stop();

	if (adp_packet_is_received() & (adp_packet_received_get_id() == MSG_RES_DATA)) {
		return true;
	}
	return false;
}

/**
* \brief Send message data
*
* \param[in] data Pointer to message data to send
* \param[in] data_length Length of message data to send
*/
//static inline void adp_packet_send_data(uint8_t* data, uint8_t data_length)
void adp_packet_send_data(uint8_t* data, uint8_t data_length)
{
	while (data_length--) {
		if (*data == ADP_TOKEN) {
			/* Pad tokens */
			adp_interface_send_single(*data);
		}
		adp_interface_send_single(*data++);
	}
}
