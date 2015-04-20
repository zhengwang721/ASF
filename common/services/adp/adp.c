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

#include <compiler.h>

#include "adp.h"
#include "adp_packet.h"
#include <string.h>


/**
* \brief Initialization of the ADP service
*
* \return None
*/
void adp_init(void)
{
	adp_packet_init();
}

/**
* \brief Send handshake to PC and check for response
*
* \param[in]  protocol_version  ADP version used in this implementation
* \param[out] status            Status returned by PC
*
* \return PC returned valid response to out handshake
* \retval true  If we got valid response
* \retval false If we didn't receive a valid handshake response
*/
bool adp_request_handshake(uint8_t protocol_version, uint8_t options, uint8_t *status)
{
	uint8_t key[8] = ADP_HANDSHAKE_KEY;

	adp_interface_send_start();
	/* Send the packet header */
	adp_packet_send_header(MSG_REQ_HANDSHAKE, MSQ_REQ_HANDSHAKE_LEN);
	/* Send protocol version */
	adp_packet_send_data(&protocol_version, 1);
	/* Send the GPIO enable byte */
	adp_packet_send_data(&options, 1);
	/* Send the handshake key */
	adp_packet_send_data(key, 8);
	//adp_interface_send(key, 8);
	adp_interface_send_stop();

	/* Wait for response */
	return adp_packet_check_for_response(MSG_RES_HANDSHAKE, status, 1);
}


/**
* \brief Send handshake to PC until we get a valid response
*
* \warning Blocking function
*
* \return Handshake status. See /ref something
*/
enum adp_handshake_status adp_wait_for_handshake(void)
{
	uint8_t handshake_status;

	/* Keep sending handshake until we get something back */
	while (adp_request_handshake(ADP_VERSION, 0, &handshake_status) == false) {
	}

	/* Return status */
	return ((enum adp_handshake_status)handshake_status);
}


/**
* \brief Request status from PC
*
* Sends a MSG_REQ_STATUS and waits for response
*
* \return PC Status. See /ref something
*/
enum adp_status_code adp_request_status(void)
{
	uint16_t status;

	/* Send header (no data in this message) */
	adp_interface_send_start();
	adp_packet_send_header(MSG_REQ_STATUS, MSG_REQ_STATUS_LEN);
	adp_interface_send_stop();
	/* Wait for status response from PC */
	adp_packet_wait_for_response(MSG_RES_STATUS, (uint8_t*)&status, 2);
	/* Return status */
	return ((enum adp_status_code)status);
}

/**
* \brief Send MSG_REQ_DATA_READY and wait for response
*
* \param[out] response Data ready response struct
*
* \return None.
*/
void adp_request_data_ready(struct adp_msg_response_data_ready *response)
{
	uint8_t stream_num;
	uint8_t receive_offset;
	uint8_t receive_data[MSG_RES_DATA_READY_MAX_LEN];

	memset(response, 0, sizeof(response));

	/* Send packet header (no data in this message) */
	adp_interface_send_start();
	adp_packet_send_header(MSG_REQ_DATA_READY, MSG_REQ_DATA_READY_LEN);
	adp_interface_send_stop();

	/* Wait for response from PC */
	adp_packet_wait_for_response(MSG_RES_DATA_READY, receive_data, MSG_RES_DATA_READY_MAX_LEN);

	/* Build response struct */
	response->num_streams = receive_data[0];
	Assert(response->num_streams <= ADP_MAX_INCOMMING_STREAMS);

	for (stream_num = 0; stream_num < response->num_streams; stream_num++) {
		receive_offset = (stream_num * MSG_RES_DATA_READY_STREAM_SIZE) + 1;
		response->stream[stream_num].stream_id = receive_data[receive_offset];
		response->stream[stream_num].data_size = receive_data[receive_offset+1] | (receive_data[receive_offset+2] << 8);
	}
}


/**
* \brief Send MSG_REQ_DATA_READY and wait for response
*
* \param[in] request Struct containing request information
* \param[out] response Struct containing response
*
* \return None
*/
void adp_request_data(uint8_t stream_id,uint8_t bytes_to_send, struct adp_msg_response_data *response)
{
	uint8_t receive_data[MSG_RES_DATA_MAX_LEN];
	uint8_t n;

	memset(response, 0, sizeof(response));		

	if (bytes_to_send > ADP_MAX_BYTE_REQUEST) {
		bytes_to_send = ADP_MAX_BYTE_REQUEST;
	}

	/* Send header and data */
	adp_interface_send_start();
	adp_packet_send_header(MSG_REQ_DATA, MSG_REQ_DATA_LEN);
	adp_packet_send_data(&stream_id, 1);
	adp_packet_send_data(&bytes_to_send, 1);
	adp_interface_send_stop();

	/* Wait for response */
	adp_packet_wait_for_response(MSG_RES_DATA, receive_data, MSG_RES_DATA_MAX_LEN);

	/* Build response struct */
	response->stream_id = receive_data[0];
	response->bytes_sent = receive_data[1];
	Assert(response->bytes_sent <= ADP_MAX_BYTE_REQUEST);
	response->remaining_data = receive_data[2] | (receive_data[3] << 8);

	/* Copy data */
	for (n=0; n < response->bytes_sent; n++) {
		response->data[n] = receive_data[4+n];
	}
}

bool adp_receive_packet_data(uint8_t *receive_buf)
{
	return adp_packet_receive_packet_data(receive_buf);
}


/**
* \brief Send MSG_CONF_INFO message and wait for response
*
* \param[in] title       Application title
* \param[in] description Application description
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_configure_info(const char* title, const char* description)
{
	uint16_t title_len;
	uint16_t description_len;
	uint8_t ack;

	/* Add null-termination to length */
	title_len = strlen(title) + 1;
	description_len = strlen(description) + 1;
	/* Make sure the strings are not too long */
	Assert(title_len + description_len <= ADP_MAX_PACKET_DATA_SIZE);

	/* Send packet */
	adp_interface_send_start();
	adp_packet_send_header(MSG_CONF_INFO, title_len + description_len);
	adp_packet_send_data((uint8_t*)title, title_len);
	adp_packet_send_data((uint8_t*)description, description_len);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}

/**
* \brief Send MSG_CONF_STREAM and wait for response
*
* \param[in] config Configuration struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_configure_stream(struct adp_msg_configure_stream *const config)
{
	uint8_t ack;

	/* Send message header */
	adp_interface_send_start();
	adp_packet_send_header(MSG_CONF_STREAM, MSG_CONF_STREAM_LEN);

	/* Send configuration message */
	adp_packet_send_data(&config->stream_id, 1);
	adp_packet_send_data((uint8_t*)&config->type, 1);
	adp_packet_send_data((uint8_t*)&config->mode, 1);
	adp_packet_send_data((uint8_t*)&config->state, 1);
	adp_packet_send_data((uint8_t*)config->label, ADP_CONF_MAX_LABEL);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}


/**
* \brief Send MSG_CONF_TOGGLE_STREAM and wait for response
*
* \param[in] stream_id ID of stream to toggle
* \param[in] state     New state for given stream
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_toggle_stream(uint8_t stream_id, enum adp_stream_state state)
{
	uint8_t ack;

	/* Send message header */
	adp_interface_send_start();
	adp_packet_send_header(MSG_CONF_TOGGLE_STREAM, MSG_CONF_TOGGLE_STREAM_LEN);

	/* Send message data */
	adp_packet_send_data(&stream_id, 1);
	adp_packet_send_data((uint8_t*)&state, 1);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}


/**
* \brief Send MSG_CONF_GRAPH and wait for response
*
* \param[in] config Graph configuration struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_configure_graph(struct adp_msg_configure_graph *const config, const char* label)
{
	uint8_t ack;
	uint16_t label_len;

	/* Add 0-termination to label string length */
	label_len = strlen(label) + 1;

	/* Make sure label isn't too big */
	Assert(MSG_CONF_GRAPH_LEN + label_len <= ADP_MAX_PACKET_DATA_SIZE);


	/* Send message header */
	adp_interface_send_start();
	adp_packet_send_header(MSG_CONF_GRAPH, MSG_CONF_GRAPH_LEN + label_len);

	/* Send message data */
	adp_packet_send_data(&config->graph_id, 1);
	//adp_packet_send_data((uint8_t*)config->label, ADP_CONF_MAX_LABEL);
	adp_packet_send_data((uint8_t*)label, label_len);
	adp_packet_send_data((uint8_t*)&config->x_min, 4);
	adp_packet_send_data((uint8_t*)&config->x_max, 4);
	adp_packet_send_data((uint8_t*)config->x_label, ADP_CONF_MAX_LABEL);
	adp_packet_send_data((uint8_t*)&config->x_scale_numerator, 4);
	adp_packet_send_data((uint8_t*)&config->x_scale_denominator, 4);
	adp_packet_send_data((uint8_t*)&config->scale_mode, 1);
	adp_packet_send_data((uint8_t*)&config->background_color, 3);
	adp_packet_send_data((uint8_t*)&config->scroll_mode, 1);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}


/**
* \brief Send MSG_CONF_TERMINAL and wait for response
*
* \param[in] config Terminal configuration struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_configure_terminal(struct adp_msg_conf_terminal *const config, const char* label)
{
	uint8_t ack;
	uint16_t label_len;

	/* Add 0-termination to label string length */
	label_len = strlen(label) + 1;

	/* Make sure label isn't too big */
	Assert(MSG_CONF_TERMINAL_LEN + label_len <= ADP_MAX_PACKET_DATA_SIZE);

	/* Send message header */
	adp_interface_send_start();
	adp_packet_send_header(MSG_CONF_TERMINAL, MSG_CONF_TERMINAL_LEN + label_len);

	/* Send message data */
	adp_packet_send_data(&config->terminal_id, 1);
	adp_packet_send_data((uint8_t*)label, label_len);
	adp_packet_send_data(&config->width, 1);
	adp_packet_send_data(&config->height, 1);
	adp_packet_send_data((uint8_t*)&config->background_color, 3);
	adp_packet_send_data((uint8_t*)&config->foreground_color, 3);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}

bool adp_transceive_terminal(struct adp_msg_conf_terminal *const config, const char* label, uint8_t* rx_buf)
{
	//uint8_t ack;
	uint16_t label_len;
	uint8_t status = false;

	/* Add 0-termination to label string length */
	label_len = strlen(label) + 1;

	/* Make sure label isn't too big */
	Assert(MSG_CONF_TERMINAL_LEN + label_len <= ADP_MAX_PACKET_DATA_SIZE);

	/* Send message header */
	adp_interface_send_start();
	status |= adp_packet_transceive_header(MSG_CONF_TERMINAL, MSG_CONF_TERMINAL_LEN + label_len, rx_buf);

	/* Send message data */
	status |= adp_packet_transceive_data(&config->terminal_id, 1, rx_buf);
	status |= adp_packet_transceive_data((uint8_t*)label, label_len, rx_buf);
	status |= adp_packet_transceive_data(&config->width, 1, rx_buf);
	status |= adp_packet_transceive_data(&config->height, 1, rx_buf);
	status |= adp_packet_transceive_data((uint8_t*)&config->background_color, 3, rx_buf);
	status |= adp_packet_transceive_data((uint8_t*)&config->foreground_color, 3, rx_buf);
	
	adp_interface_send_stop();

	return status;
}


/**
* \brief Send MSG_CONF_ADD_TO_TERMINAL and wait for response
*
* \param[in] config Stream-to-terminal configuration struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_add_stream_to_terminal(struct adp_msg_add_stream_to_terminal *const config)
{
	uint8_t ack;

	/* Send message header */
	adp_interface_send_start();
	adp_packet_send_header(MSG_CONF_ADD_TO_TERMINAL, MSG_CONF_ADD_TO_TERMINAL_LEN);

	/* Send message data */
	adp_packet_send_data(&config->terminal_id, 1);
	adp_packet_send_data(&config->stream_id, 1);
	adp_packet_send_data(&config->mode, 1);
	adp_packet_send_data((uint8_t*)&config->text_color, 3);
	adp_packet_send_data((uint8_t*)config->tag_text, ADP_CONF_MAX_LABEL);
	adp_packet_send_data((uint8_t*)&config->tag_text_color, 3);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}


/**
* \brief Send MSG_CONF_AXIS and wait for response
*
* \param[in] config Axis configuration struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_add_axis_to_graph(struct adp_msg_conf_axis *const config)
{
	uint8_t ack;

	/* Send message header */
	adp_interface_send_start();
	adp_packet_send_header(MSG_CONF_AXIS, MSG_CONF_AXIS_LEN);

	/* Send message data */
	adp_packet_send_data(&config->axis_id, 1);
	adp_packet_send_data(&config->graph_id, 1);
	adp_packet_send_data((uint8_t*)(config->label), ADP_CONF_MAX_LABEL);
	adp_packet_send_data((uint8_t*)&config->y_min, 4);
	adp_packet_send_data((uint8_t*)&config->y_max, 4);
	adp_packet_send_data((uint8_t*)&config->x_scale_numerator, 4);
	adp_packet_send_data((uint8_t*)&config->x_scale_denominator, 4);
	adp_packet_send_data((uint8_t*)&config->mode, 1);
	adp_packet_send_data((uint8_t*)&config->color, 3);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}


/**
* \brief Send MSG_CONF_ADD_STREAM_TO_AXIS and wait for response
*
* \param[in] config Axis configuration struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_add_stream_to_axis(struct adp_msg_add_stream_to_axis *const config)
{
	uint8_t ack;

	/* Send message header */
	adp_interface_send_start();
	adp_packet_send_header(MSG_CONF_ADD_STREAM_TO_AXIS, MSG_CONF_ADD_STREAM_TO_AXIS_LEN);

	/* Send message data */
	adp_packet_send_data(&config->graph_id, 1);
	adp_packet_send_data(&config->axis_id, 1);
	adp_packet_send_data(&config->stream_id, 1);
	adp_packet_send_data((uint8_t*)&config->sample_rate_numerator, 4);
	adp_packet_send_data((uint8_t*)&config->sample_rate_denominator, 4);
	adp_packet_send_data((uint8_t*)&config->y_scale_numerator, 4);
	adp_packet_send_data((uint8_t*)&config->y_scale_denominator, 4);
	adp_packet_send_data((uint8_t*)&config->y_offset, 4);
	adp_packet_send_data((uint8_t*)&config->transparency, 1);
	adp_packet_send_data((uint8_t*)&config->mode, 1);
	adp_packet_send_data((uint8_t*)&config->line_thickness, 1);
	adp_packet_send_data((uint8_t*)&config->line_color, 3);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}


/**
* \brief Send MSG_CONF_CURSOR_TO_GRAPH and wait for response
*
* \param[in] config Cursor configuration struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_add_cursor_to_graph(struct adp_msg_add_cursor_to_graph *const config)
{
	uint8_t ack;

	/* Send message header */
	adp_interface_send_start();
	adp_packet_send_header(MSG_CONF_CURSOR_TO_GRAPH, MSG_CONF_CURSOR_TO_GRAPH_LEN);

	/* Send message data */
	adp_packet_send_data(&config->stream_id, 1);
	adp_packet_send_data(&config->graph_id, 1);
	adp_packet_send_data(&config->axis_id, 1);
	adp_packet_send_data((uint8_t*)(config->label), ADP_CONF_MAX_LABEL);
	adp_packet_send_data(&config->thickness, 1);
	adp_packet_send_data((uint8_t*)&config->color, 3);
	adp_packet_send_data((uint8_t*)&config->initial_value, 4);
	adp_packet_send_data((uint8_t*)&config->minimum_value, 4);
	adp_packet_send_data((uint8_t*)&config->maximum_value, 4);
	adp_packet_send_data((uint8_t*)&config->scale_numerator, 4);
	adp_packet_send_data((uint8_t*)&config->scale_denominator, 4);
	adp_packet_send_data((uint8_t*)&config->scale_offset, 4);
	adp_packet_send_data(&config->line_style, 1);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}


/**
* \brief Send MSG_DATA_STREAM
*
* \param[in] stream_data Data stream struct
*
* \return None
*/
bool adp_send_stream(struct adp_msg_data_stream *const stream_data, uint8_t *receive_buf)
{
	uint8_t stream_num;
	uint8_t packet_size = 0;
	bool status;

	/* find packet size */
	for (stream_num = 0; stream_num < stream_data->number_of_streams; stream_num++) {
		packet_size += stream_data->stream[stream_num].data_size;
	}
	packet_size += (1 + (2*stream_data->number_of_streams));

	adp_interface_send_start();

	/* Send message header */
	adp_packet_send_header(MSG_DATA_STREAM, packet_size);

	/* Send message data */
	adp_packet_send_data(&stream_data->number_of_streams, 1);
	for (stream_num = 0; stream_num < stream_data->number_of_streams; stream_num++) {
		adp_packet_send_data(&stream_data->stream[stream_num].stream_id, 1);
		adp_packet_send_data(&stream_data->stream[stream_num].data_size, 1);
		adp_packet_send_data((uint8_t*)stream_data->stream[stream_num].data, stream_data->stream[stream_num].data_size);
	}
	
	status = adp_packet_receive_packet_data(receive_buf);
	
	adp_interface_send_stop();
	
	return status;
}

bool adp_transceive_stream(struct adp_msg_data_stream *const stream_data, uint8_t *receive_buf)
{
	uint8_t stream_num;
	uint8_t packet_size = 0;
	bool status = false;

	/* find packet size */
	for (stream_num = 0; stream_num < stream_data->number_of_streams; stream_num++) {
		packet_size += stream_data->stream[stream_num].data_size;
	}
	packet_size += (1 + (2*stream_data->number_of_streams));

	adp_interface_send_start();

	/* Send message header */
	status |= adp_packet_transceive_header(MSG_DATA_STREAM, packet_size, receive_buf);

	/* Send message data */
	status |= adp_packet_transceive_data(&stream_data->number_of_streams, 1, \
										receive_buf);
	for (stream_num = 0; stream_num < stream_data->number_of_streams; stream_num++) {
		status |= adp_packet_transceive_data( \
								&stream_data->stream[stream_num].stream_id, \
								1, receive_buf);
		status |= adp_packet_transceive_data( \
								&stream_data->stream[stream_num].data_size, \
								1, receive_buf);
		status |= adp_packet_transceive_data( \
								(uint8_t*)stream_data->stream[stream_num].data, \
								stream_data->stream[stream_num].data_size, receive_buf);
	}	
	adp_interface_send_stop();
	
	return status;
}


/**
* \brief Send data on one single stream
*
* \param[in] stream_id ID of stream
* \param[in] data      Pointer to data to send
* \param[in] data_size Size of data to send
*
* \return None
*/
bool adp_send_single_stream(uint8_t stream_id, uint8_t* data, uint8_t data_size, uint8_t* receive_buf)
{
	struct adp_msg_data_stream data_stream;
	uint8_t status;

	data_stream.number_of_streams = 1;
	data_stream.stream[0].stream_id = stream_id;
	data_stream.stream[0].data_size = data_size;
	data_stream.stream[0].data = data;
	status = adp_send_stream(&data_stream, receive_buf);
	
	return status;
}


bool adp_transceive_single_stream(uint8_t stream_id, uint8_t* data, uint8_t data_size, uint8_t* receive_buf)
{
	struct adp_msg_data_stream data_stream;
	uint8_t status;

	data_stream.number_of_streams = 1;
	data_stream.stream[0].stream_id = stream_id;
	data_stream.stream[0].data_size = data_size;
	data_stream.stream[0].data = data;
	status = adp_transceive_stream(&data_stream, receive_buf);
	
	return status;
}

/**
* \brief Add a dashboard
*
* \param[in] config    Pointer to dashboard configuration data struct
* \param[in] label     Dashboard label (0-terminated string)
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_add_dashboard(struct adp_msg_conf_dashboard *const config, const char* label)
{
	uint8_t ack;
	uint16_t label_len;

	/* Add 0-termination to label string length */
	label_len = strlen(label) + 1;

	/* Make sure label isn't too big */
	Assert(MSG_CONF_DASHBOARD_LEN + label_len <= ADP_MAX_PACKET_DATA_SIZE);

	/* Send message header */
	adp_interface_send_start();
	adp_packet_send_header(MSG_CONF_DASHBOARD, MSG_CONF_DASHBOARD_LEN + label_len);

	/* Send message data */
	adp_packet_send_data(&config->dashboard_id, 1);
	adp_packet_send_data((uint8_t*)label, label_len);
	adp_packet_send_data((uint8_t*)&config->color, 3);
	adp_packet_send_data((uint8_t*)&config->height, 2);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}

/**
* \brief Send common data for all dashboard elements
*
* \param[in] config    Pointer to common data struct
* \param[in] data      Total length of message (depending on element_type)
*
* \return None
*/
static void adp_send_dahboard_element_common_data(struct adp_msg_conf_dashboard_element_common *const config, uint8_t length)
{
	/* Send message header */
	adp_interface_send_start();
	adp_packet_send_header(MSG_CONF_DASHBOARD_ELEMENT, length);

	/* Send common message data */
	adp_packet_send_data(&config->dashboard_id, 1);
	adp_packet_send_data(&config->element_id, 1);
	adp_packet_send_data(&config->z_index, 1);
	adp_packet_send_data((uint8_t*)&config->x, 2);
	adp_packet_send_data((uint8_t*)&config->y, 2);
	adp_packet_send_data((uint8_t*)&config->width, 2);
	adp_packet_send_data((uint8_t*)&config->height, 2);
	adp_packet_send_data(&config->element_type, 1);
}

/**
* \brief Add a label to dashboard
*
* \param[in] config    Pointer to dashboard label struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_add_label_to_dashboard(struct adp_msg_conf_dashboard_element_label *const config)
{
	uint8_t ack;

	/* Send common message data */
	adp_send_dahboard_element_common_data((struct adp_msg_conf_dashboard_element_common*)config, ADP_ELEMENT_TYPE_LABEL_LEN);

	/* Send label specific message data */
	adp_packet_send_data(&config->font_size, 1);
	adp_packet_send_data(&config->attribute, 1);
	adp_packet_send_data(&config->horisontal_alignment, 1);
	adp_packet_send_data(&config->vertical_alignment, 1);
	adp_packet_send_data(&config->background_transparency, 1);
	adp_packet_send_data((uint8_t*)&config->background_color, 3);
	adp_packet_send_data(&config->foreground_transparency, 1);
	adp_packet_send_data((uint8_t*)&config->foreground_color, 3);
	adp_packet_send_data((uint8_t*)config->default_text, ADP_CONF_MAX_LABEL);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}

/**
* \brief Add a button to dashboard
*
* \param[in] config    Pointer to dashboard button struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_add_button_to_dashboard(struct adp_msg_conf_dashboard_element_button *const config)
{
	uint8_t ack;

	/* Send common message data */
	adp_send_dahboard_element_common_data((struct adp_msg_conf_dashboard_element_common*)config, ADP_ELEMENT_TYPE_BUTTON_LEN);

	/* Send button specific message data */
	adp_packet_send_data(&config->font_size, 1);
	adp_packet_send_data((uint8_t*)config->button_label, ADP_CONF_MAX_LABEL);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}

/**
* \brief Add a slider to dashboard
*
* \param[in] config    Pointer to dashboard slider struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_add_slider_to_dashboard(struct adp_msg_conf_dashboard_element_slider *const config)
{
	uint8_t ack;

	/* Send common message data */
	adp_send_dahboard_element_common_data((struct adp_msg_conf_dashboard_element_common*)config, ADP_ELEMENT_TYPE_SLIDER_LEN);

	/* Send slider specific message data */
	adp_packet_send_data((uint8_t*)&config->minimum_value, 4);
	adp_packet_send_data((uint8_t*)&config->maximum_value, 4);
	adp_packet_send_data((uint8_t*)&config->initial_value, 4);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}

/**
* \brief Add a signal to dashboard
*
* \param[in] config    Pointer to dashboard signal struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_add_signal_to_dashboard(struct adp_msg_conf_dashboard_element_signal *const config)
{
	uint8_t ack;

	/* Send common message data */
	adp_send_dahboard_element_common_data((struct adp_msg_conf_dashboard_element_common*)config, ADP_ELEMENT_TYPE_SIGNAL_LEN);

	/* Send signal specific message data */
	adp_packet_send_data(&config->on_transparency, 1);
	adp_packet_send_data((uint8_t*)&config->on_color, 3);
	adp_packet_send_data(&config->off_transparency, 1);
	adp_packet_send_data((uint8_t*)&config->off_color, 3);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}

/**
* \brief Add a progress bar to dashboard
*
* \param[in] config    Pointer to dashboard progress bar struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_add_progress_to_dashboard(struct adp_msg_conf_dashboard_element_progress *const config)
{
	uint8_t ack;

	/* Send common message data */
	adp_send_dahboard_element_common_data((struct adp_msg_conf_dashboard_element_common*)config, ADP_ELEMENT_TYPE_PROGRESS_LEN);

	/* Send progress specific message data */
	adp_packet_send_data((uint8_t*)&config->minimum_value, 4);
	adp_packet_send_data((uint8_t*)&config->maximum_value, 4);
	adp_packet_send_data((uint8_t*)&config->initial_value, 4);
	adp_packet_send_data((uint8_t*)&config->color, 3);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}

/**
* \brief Add a segment display to dashboard
*
* \param[in] config    Pointer to dashboard segment struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_add_segment_to_dashboard(struct adp_msg_conf_dashboard_element_segment *const config)
{
	uint8_t ack;

	/* Send common message data */
	adp_send_dahboard_element_common_data((struct adp_msg_conf_dashboard_element_common*)config, ADP_ELEMENT_TYPE_SEGMENT_LEN);

	/* Send segment specific message data */
	adp_packet_send_data(&config->segment_count, 1);
	adp_packet_send_data(&config->base, 1);
	adp_packet_send_data(&config->transparency, 1);
	adp_packet_send_data((uint8_t*)&config->color, 3);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}

/**
* \brief Add a graph display to dashboard
*
* \param[in] config    Pointer to dashboard graph struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_add_graph_to_dashboard(struct adp_msg_conf_dashboard_element_graph *const config)
{
	uint8_t ack;

	/* Send common message data */
	adp_send_dahboard_element_common_data((struct adp_msg_conf_dashboard_element_common*)config, ADP_ELEMENT_TYPE_SEGMENT_LEN);

	/* Send segment specific message data */
	adp_packet_send_data((uint8_t*)&config->title_color, 3);
	adp_packet_send_data((uint8_t*)&config->background_color, 3);
	adp_packet_send_data((uint8_t*)&config->graph_background_color, 3);
	adp_packet_send_data((uint8_t*)&config->plot_color, 3);
	adp_packet_send_data((uint8_t*)config->title_text, sizeof(config->title_text));
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}

/**
* \brief Connect a stream to dashboard element
*
* \param[in] config    Pointer to stream element configuration struct
*
* \return Status from PC
* \retval true   Message received and accepted
* \retval false  Message received but not accepted
*/
bool adp_add_stream_to_element(struct adp_conf_add_stream_to_element *const config)
{
	uint8_t ack;

	/* Send message header */
	adp_interface_send_start();
	adp_packet_send_header(MSG_CONF_ADD_STREAM_TO_ELEMENT, MSG_CONF_ADD_STREAM_TO_ELEMENT_LEN);

	/* Send message data */
	adp_packet_send_data(&config->dashboard_id, 1);
	adp_packet_send_data(&config->element_id, 1);
	adp_packet_send_data(&config->stream_id, 1);
	adp_interface_send_stop();

	/* Wait for response and return status */
	adp_packet_wait_for_response(MSG_CONF_ACK, &ack, 1);
	return (ack == ADP_ACK_OK);
}

