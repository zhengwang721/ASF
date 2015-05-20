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
#include <asf.h>
#include <string.h>
#include "adp_example_adc.h"
#include "adp_example_tc.h"
#include "adp_window.h"

/* Low hysteresis value */
uint16_t hyst_low = 2300;
/* High hysteresis value */
uint16_t hyst_high = 3000;
bool isLight = false;

/* Length of terminal string */
#define TERMINAL_STRING_LEN 50
/* String holding terminal text */
char terminal_string[TERMINAL_STRING_LEN];
/* Received the packet data buffer */
uint8_t receive_packet_data[MSG_RES_PACKET_DATA_MAX_LEN] = {0,};

/**
* \brief Initiate extension board ioport.
*
*/
static void adp_example_ioport_init(void)
{
	/* Configure LED*/
	ioport_set_pin_dir(USER_LED_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(USER_LED_PIN, USER_LED_INACTIVE);	
}

/**
* \brief Initiate example.
*
*/
static void adp_example_init(void)
{
	adp_init();
	/* Extension board ioport initial */
	adp_example_ioport_init();
	adp_example_adc_init();
	
	while (adp_wait_for_handshake() != ADP_HANDSHAKE_ACCEPTED) {
	}
		
	adp_window_init();
	adp_example_tc_init();
}

/**
* \brief Executive command form PC to the target.
*
*/
static void executive_command(uint8_t* receive_buf)
{
	struct adp_msg_packet_data *receive_buf_s = receive_buf;
	
	switch(receive_buf_s->stream_id) {
		case STREAM_ID_LED_TOGGLE:
			ioport_toggle_pin_level(USER_LED_PIN);
			break;
			
		case STREAM_ID_HYST_LOW:
			if(receive_buf_s->bytes_sent >= 2) {
				hyst_low = (uint16_t)receive_buf_s->data[0] | ((uint16_t)receive_buf_s->data[1] << 8);
			}
			break;
		
		case STREAM_ID_HYST_HIGH:
			if(receive_buf_s->bytes_sent >= 2) {
				hyst_high = (uint16_t)receive_buf_s->data[0] | ((uint16_t)receive_buf_s->data[1] << 8);
			}
			break;
		
		default:
			break;
	}
}

/**
* \brief Send single stream to PC.
*
*/
static void adp_example_transceive_single_stream(uint16_t stream_id, uint8_t* data, \
										uint8_t data_size, uint8_t* receive_buf)
{
	if(adp_transceive_single_stream(stream_id, data, data_size, receive_buf) == true) {
		executive_command(receive_buf);
	}
}

/**
* \brief Send terminal configure to PC.
*
*/
static void adp_example_transceive_terminal(struct adp_msg_conf_terminal *const config, \
										const char* label, uint8_t* receive_buf)
{
	if(adp_configure_terminal(config, label) == true) {
		executive_command(receive_buf);
	}
}

/**
* \brief Read new light sensor value from ADC and handle it.
*
*/
static void transceive_data(void)
{
	uint16_t adc_value;
	float voltage;
	uint8_t status;

	//struct adp_msg_data_stream data_stream;

	/* Get light sensor value */
	adc_value = adp_example_adc_get_value();
	voltage = ((float)adc_value*3.3f)/4096.0f;
	//uint8_t* volt_value = &voltage;
	
	/* Send ADC value to graph and progress bar */
	adp_example_transceive_single_stream(STREAM_ID_LIGHT_SENSOR, (uint8_t*)&voltage, \
										4, receive_packet_data);
	adp_example_transceive_single_stream(STREAM_ID_LIGHT_SENSOR_ADC, \
										(uint8_t *)&adc_value, 2, receive_packet_data);
	
	if((adc_value > hyst_high) & isLight) {		
		status = 0;
		adp_example_transceive_single_stream(STREAM_ID_NIGHT_MODE, &status, 1, \
											receive_packet_data);
		isLight = false;
		
		/* Send string to terminal */
		snprintf(terminal_string, TERMINAL_STRING_LEN,
				"%s", "It got dark... Entered night mode!\r\n");
		adp_example_transceive_single_stream(STREAM_ID_STATUS_MESSAGE, \
											(uint8_t *)&terminal_string, \
											strlen(terminal_string), receive_packet_data);
		/* Set new terminal background color */
		adp_set_color(terminal_config.background_color, ADP_COLOR_BLACK);
		adp_example_transceive_terminal(&terminal_config, TERMINAL_LABEL, receive_packet_data);
	} 
	else if ((adc_value < hyst_low) & (!isLight)) {
		status = 1;
		adp_example_transceive_single_stream(STREAM_ID_NIGHT_MODE, &status, \
											1, receive_packet_data);
		isLight = true;
		
		/* Send string to terminal */
		snprintf(terminal_string, TERMINAL_STRING_LEN,
				"%s", "It's bright again... Entered day mode!\r\n");
		adp_example_transceive_single_stream(STREAM_ID_STATUS_MESSAGE, \
											(uint8_t*)&terminal_string, \
											strlen(terminal_string), receive_packet_data);
		/* Set new terminal background color */
		adp_set_color(terminal_config.background_color, ADP_COLOR_WHITE);
		adp_example_transceive_terminal(&terminal_config, TERMINAL_LABEL, receive_packet_data);
	}	
}

int main (void)
{
	/* Initialize system */
	board_init();
	adp_example_init();

	while (1) {
		if(time_out) {
			/* Receive and send forever */
			transceive_data();
			time_out = false;
		}
		
		/* Add user application here */
	}
}
