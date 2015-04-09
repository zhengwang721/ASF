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

/** Stream number defines */
/* Status messages stream id */
#define STREAM_ID_STATUS_MESSAGE    0x00
/* Outgoing voltage value stream id */
#define STREAM_ID_LIGHT_SENSOR      0x01
/* Outgoing ADC value stream id */
#define STREAM_ID_LIGHT_SENSOR_ADC  0x02
/* Incoming stream setting high hysteresis */
#define STREAM_ID_HYST_HIGH         0x10
/* Incoming stream setting low hysteresis */
#define STREAM_ID_HYST_LOW          0x11
/* Night mode signal stream id */
#define STREAM_ID_NIGHT_MODE        0x29
/* Led toggle stream id */
#define STREAM_ID_LED_TOGGLE        0x30

/** Dashboard and elements defines */
/* Light sensor example dashboard id */ 
#define DASHBOARD_ID_LIGHT_SENSOR       0x00
#define DASHBOARD_ID_LIGHT_SENSOR_CON   0x01

/* Terminal label */
#define TERMINAL_LABEL "Status terminal"
/* Graph label */
#define GRAPH_LABEL    "Light Sensor Output"
/* Dashboard label */
#define DASHBOARD_DIS_LABEL "Light Sensor Example Dashboard"
#define DASHBOARD_CON_LABEL "Light Sensor Config Dashboard"

/* Terminal configuration. Global, so we can change it later */
struct adp_msg_conf_terminal terminal_config = {
	.terminal_id = 0,
	.width = 80,
	.height = 50,
	.background_color = {ADP_COLOR_WHITE},
	.foreground_color = {ADP_COLOR_GREEN}
};

enum adp_example_element_id {
	/* Dashboard example */
	ELEMENT_ID_LABEL_TITEL,
	ELEMENT_ID_LABEL_VALUE_VOL,
	ELEMENT_ID_LABEL_NIGHT_LIGHT,
	ELEMENT_ID_LABEL_LED_CONTROL,
	ELEMENT_ID_BAR,
	ELEMENT_ID_SIGNAL,
	ELEMENT_ID_BUTTON,
	ELEMENT_ID_GRAPH,
	
	/* Dashboard config */
	ELEMENT_ID_LABEL_HYSTERESIS,
	ELEMENT_ID_LABEL_LOW,
	ELEMENT_ID_LABEL_HIGH,
	ELEMENT_ID_LABEL_VALUE_ADC,
	ELEMENT_ID_SLIDER_LOW,
	ELEMENT_ID_SLIDER_HIGH,
	ELEMENT_ID_SEGMENT,	
};


/**
* \brief Config and add all new streams.
*
*/
static void adp_example_add_terminal(void)
{
	struct adp_msg_configure_stream stream;
	
	/* Add terminal stream */
	stream.stream_id = STREAM_ID_STATUS_MESSAGE;
	stream.type = ADP_STREAM_UINT_8;
	stream.mode = ADP_STREAM_OUT;
	stream.state = ADP_STREAM_ON;
	adp_set_string(stream.label, "Status messages");
	adp_configure_stream(&stream);
	
	adp_configure_terminal(&terminal_config, TERMINAL_LABEL);
	
	/* Connect stream and terminal */
	struct adp_msg_add_stream_to_terminal conf_stream = {
		.terminal_id = 0,
		.stream_id = STREAM_ID_STATUS_MESSAGE,
		.mode = 0xFF,
		.text_color = {ADP_COLOR_BLACK},
		.tag_text = "",
		.tag_text_color = {ADP_COLOR_BLACK}
	};
	adp_add_stream_to_terminal(&conf_stream);
}

/**
* \brief Add all labels to the dashboard.
*
*/
static void adp_example_add_label(void)
{
	/* Add all elements to DASHBOARD_ID_LIGHT_SENSOR */
	/* Add a label: light sensor example */
	struct adp_msg_conf_dashboard_element_label label1 = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR,
		.element_id = ELEMENT_ID_LABEL_TITEL,
		.z_index = 0,
		.x = 5,
		.y = 5,
		.width = 300,
		.height = 35,
		.element_type = ADP_ELEMENT_TYPE_LABEL,
		.font_size = 24,
		.attribute = BOLD_OFF_ITALIC_OFF,
		.horisontal_alignment = HORISONTAL_ALIGNMENT_CENTER,
		.vertical_alignment = VERTICAL_ALIGNMENT_CENTER,
		.background_transparency = 0,
		.background_color = {ADP_COLOR_WHITE},
		.foreground_transparency = 255,
		.foreground_color = {ADP_COLOR_BLACK},
		.default_text = "LightSensorExample"
	};
	adp_add_label_to_dashboard(&label1);
	
	/* Add a label: Light Sensor Value */
	struct adp_msg_conf_dashboard_element_label label2 = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR,
		.element_id = ELEMENT_ID_LABEL_VALUE_VOL,
		.z_index = 0,
		.x = 5,
		.y = 60,
		.width = 129,
		.height = 25,
		.element_type = ADP_ELEMENT_TYPE_LABEL,
		.font_size = 14,
		.attribute = BOLD_ON_ITALIC_OFF,
		.horisontal_alignment = HORISONTAL_ALIGNMENT_LEFT,
		.vertical_alignment = VERTICAL_ALIGNMENT_CENTER,
		.background_transparency = 0,
		.background_color = {ADP_COLOR_WHITE},
		.foreground_transparency = 255,
		.foreground_color = {ADP_COLOR_BLACK},
		.default_text = "Light Sensor Value"
	};
	adp_add_label_to_dashboard(&label2);
	
	/* Add a label: Night Light */
	struct adp_msg_conf_dashboard_element_label label3 = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR,
		.element_id = ELEMENT_ID_LABEL_NIGHT_LIGHT,
		.z_index = 0,
		.x = 5,
		.y = 100,
		.width = 82,
		.height = 25,
		.element_type = ADP_ELEMENT_TYPE_LABEL,
		.font_size = 14,
		.attribute = BOLD_ON_ITALIC_OFF,
		.horisontal_alignment = HORISONTAL_ALIGNMENT_LEFT,
		.vertical_alignment = VERTICAL_ALIGNMENT_CENTER,
		.background_transparency = 0,
		.background_color = {ADP_COLOR_WHITE},
		.foreground_transparency = 255,
		.foreground_color = {ADP_COLOR_BLACK},
		.default_text = "Night Light"
	};
	adp_add_label_to_dashboard(&label3);
	
	/* Add a label: LED Control */
	struct adp_msg_conf_dashboard_element_label label4 = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR,
		.element_id = ELEMENT_ID_LABEL_LED_CONTROL,
		.z_index = 0,
		.x = 5,
		.y = 230,
		.width = 80,
		.height = 25,
		.element_type = ADP_ELEMENT_TYPE_LABEL,
		.font_size = 14,
		.attribute = BOLD_ON_ITALIC_OFF,
		.horisontal_alignment = HORISONTAL_ALIGNMENT_LEFT,
		.vertical_alignment = VERTICAL_ALIGNMENT_CENTER,
		.background_transparency = 0,
		.background_color = {ADP_COLOR_WHITE},
		.foreground_transparency = 255,
		.foreground_color = {ADP_COLOR_BLACK},
		.default_text = "LED Control"
	};
	adp_add_label_to_dashboard(&label4);
	
	
	/* Add all elements to DASHBOARD_ID_LIGHT_SENSOR */
	/* Add a label: Hysteresis Values: */
	struct adp_msg_conf_dashboard_element_label label5 = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR_CON,
		.element_id = ELEMENT_ID_LABEL_HYSTERESIS,
		.z_index = 0,
		.x = 5,
		.y = 20,
		.width = 128,
		.height = 25,
		.element_type = ADP_ELEMENT_TYPE_LABEL,
		.font_size = 14,
		.attribute = BOLD_ON_ITALIC_OFF,
		.horisontal_alignment = HORISONTAL_ALIGNMENT_LEFT,
		.vertical_alignment = VERTICAL_ALIGNMENT_CENTER,
		.background_transparency = 0,
		.background_color = {ADP_COLOR_WHITE},
		.foreground_transparency = 255,
		.foreground_color = {ADP_COLOR_BLACK},
		.default_text = "Hysteresis Values:"
	};
	adp_add_label_to_dashboard(&label5);
	
	/* Add a label: High */
	struct adp_msg_conf_dashboard_element_label label6 = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR_CON,
		.element_id = ELEMENT_ID_LABEL_HIGH,
		.z_index = 0,
		.x = 25,
		.y = 100,
		.width = 30,
		.height = 25,
		.element_type = ADP_ELEMENT_TYPE_LABEL,
		.font_size = 14,
		.attribute = BOLD_OFF_ITALIC_ON,
		.horisontal_alignment = HORISONTAL_ALIGNMENT_LEFT,
		.vertical_alignment = VERTICAL_ALIGNMENT_CENTER,
		.background_transparency = 0,
		.background_color = {ADP_COLOR_WHITE},
		.foreground_transparency = 255,
		.foreground_color = {ADP_COLOR_BLACK},
		.default_text = "High"
	};
	adp_add_label_to_dashboard(&label6);
	
	/* Add a label: Low */
	struct adp_msg_conf_dashboard_element_label label7 = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR_CON,
		.element_id = ELEMENT_ID_LABEL_LOW,
		.z_index = 0,
		.x = 25,
		.y = 60,
		.width = 30,
		.height = 25,
		.element_type = ADP_ELEMENT_TYPE_LABEL,
		.font_size = 14,
		.attribute = BOLD_OFF_ITALIC_ON,
		.horisontal_alignment = HORISONTAL_ALIGNMENT_LEFT,
		.vertical_alignment = VERTICAL_ALIGNMENT_CENTER,
		.background_transparency = 0,
		.background_color = {ADP_COLOR_WHITE},
		.foreground_transparency = 255,
		.foreground_color = {ADP_COLOR_BLACK},
		.default_text = "Low"
	};
	adp_add_label_to_dashboard(&label7);
	
	/* Add a label: ADC sample value */
	struct adp_msg_conf_dashboard_element_label label8 = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR_CON,
		.element_id = ELEMENT_ID_LABEL_VALUE_ADC,
		.z_index = 0,
		.x = 350,
		.y = 20,
		.width = 130,
		.height = 25,
		.element_type = ADP_ELEMENT_TYPE_LABEL,
		.font_size = 14,
		.attribute = BOLD_ON_ITALIC_OFF,
		.horisontal_alignment = HORISONTAL_ALIGNMENT_LEFT,
		.vertical_alignment = VERTICAL_ALIGNMENT_CENTER,
		.background_transparency = 0,
		.background_color = {ADP_COLOR_WHITE},
		.foreground_transparency = 255,
		.foreground_color = {ADP_COLOR_BLACK},
		.default_text = "ADC Sample Value:"
	};
	adp_add_label_to_dashboard(&label8);
}

/**
* \brief Add all labels to the dashboard.
*
*/
static void adp_example_add_element_stream(void)
{
	struct adp_msg_configure_stream stream;
	struct adp_conf_add_stream_to_element element_stream = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR,
	};
	
	/* Add progress bar and graph stream(light sensor value) */
	stream.stream_id = STREAM_ID_LIGHT_SENSOR;
	stream.type = ADP_STREAM_FLOAT;
	stream.mode = ADP_STREAM_OUT;
	stream.state = ADP_STREAM_ON;
	adp_set_string(stream.label, "Light Sensor");
	adp_configure_stream(&stream);
	
	struct adp_msg_conf_dashboard_element_progress progress_bar_value = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR,
		.element_id = ELEMENT_ID_BAR,
		.z_index = 0,
		.x = 140,
		.y = 60,
		.width = 145,
		.height = 25,
		.element_type = ADP_ELEMENT_TYPE_PROGRESS,
		.minimum_value = 0,
		.maximum_value = 4,
		.initial_value = 0,
		.color = {ADP_COLOR_GREEN},
	};
	adp_add_progress_to_dashboard(&progress_bar_value);
	
	element_stream.element_id = ELEMENT_ID_BAR;
	element_stream.stream_id = STREAM_ID_LIGHT_SENSOR;
	adp_add_stream_to_element(&element_stream);
	
	struct adp_msg_conf_dashboard_element_graph graph_value = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR,
		.element_id = ELEMENT_ID_GRAPH,
		.z_index = 0,
		.x = 310,
		.y = 10,
		.width = 320,
		.height = 240,
		.element_type = ADP_ELEMENT_TYPE_GRAPH,
		.title_color = {ADP_COLOR_WHITE},
		.background_color = {ADP_COLOR_BLACK},
		.graph_background_color = {ADP_COLOR_BLACK},
		.plot_color = {ADP_COLOR_RED},
		.title_text = "Light level",
	};
	adp_add_graph_to_dashboard(&graph_value);
	
	element_stream.element_id = ELEMENT_ID_GRAPH;
	element_stream.stream_id = STREAM_ID_LIGHT_SENSOR;
	adp_add_stream_to_element(&element_stream);
	
	/* Add signal stream(night mode) */
	stream.stream_id = STREAM_ID_NIGHT_MODE;
	stream.type = ADP_STREAM_UINT_8;
	stream.mode = ADP_STREAM_OUT;
	stream.state = ADP_STREAM_ON;
	adp_set_string(stream.label, "Night Mode");
	adp_configure_stream(&stream);
	
	struct adp_msg_conf_dashboard_element_signal signal_mode = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR,
		.element_id = ELEMENT_ID_SIGNAL,
		.z_index = 0,
		.x = 140,
		.y = 100,
		.width = 25,
		.height = 25,
		.element_type = ADP_ELEMENT_TYPE_SIGNAL,
		.on_transparency = 0xFF,
		.on_color = {ADP_COLOR_GREEN},
		.off_transparency = 0xFF,
		.off_color = {ADP_COLOR_BLACK},
	};
	adp_add_signal_to_dashboard(&signal_mode);
	
	element_stream.element_id = ELEMENT_ID_SIGNAL;
	element_stream.stream_id = STREAM_ID_NIGHT_MODE;
	adp_add_stream_to_element(&element_stream);
	
	/* Add button stream(LED Toggle) */
	stream.stream_id = STREAM_ID_LED_TOGGLE;
	stream.type = ADP_STREAM_UINT_8;
	stream.mode = ADP_STREAM_IN;
	stream.state = ADP_STREAM_ON;
	adp_set_string(stream.label, "LED Toggle");
	adp_configure_stream(&stream);
	
	struct adp_msg_conf_dashboard_element_button button = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR,
		.element_id = ELEMENT_ID_BUTTON,
		.z_index = 0,
		.x = 110,
		.y = 230,
		.width = 75,
		.height = 50,
		.element_type = ADP_ELEMENT_TYPE_BUTTON,
		.font_size = 10,
		.button_label = "LED Toggle",
	};
	adp_add_button_to_dashboard(&button);
	
	element_stream.element_id = ELEMENT_ID_BUTTON;
	element_stream.stream_id = STREAM_ID_LED_TOGGLE;
	adp_add_stream_to_element(&element_stream);	
	
	/* Add stream to the DASHBOARD_ID_LIGHT_SENSOR_CON dashboard */
	element_stream.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR_CON;
	
	/* Add slider stream(slider_high) */
	stream.stream_id = STREAM_ID_HYST_HIGH;
	stream.type = ADP_STREAM_UINT_16;
	stream.mode = ADP_STREAM_IN;
	stream.state = ADP_STREAM_ON;
	adp_set_string(stream.label, "Hyst. High");
	adp_configure_stream(&stream);
	
	struct adp_msg_conf_dashboard_element_slider slider_high = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR_CON,
		.element_id = ELEMENT_ID_SLIDER_HIGH,
		.z_index = 0,
		.x = 75,
		.y = 100,
		.width = 156,
		.height = 25,
		.element_type = ADP_ELEMENT_TYPE_SLIDER,
		.minimum_value = 2500,
		.maximum_value = 4000,
		.initial_value = 3000,
	};
	adp_add_slider_to_dashboard(&slider_high);
	
	element_stream.element_id = ELEMENT_ID_SLIDER_HIGH;
	element_stream.stream_id = STREAM_ID_HYST_HIGH;
	adp_add_stream_to_element(&element_stream);
	
	/* Add slider stream(slider_low) */
	stream.stream_id = STREAM_ID_HYST_LOW;
	stream.type = ADP_STREAM_UINT_16;
	stream.mode = ADP_STREAM_IN;
	stream.state = ADP_STREAM_ON;
	adp_set_string(stream.label, "Hyst. Low");
	adp_configure_stream(&stream);
	
	struct adp_msg_conf_dashboard_element_slider slider_low = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR_CON,
		.element_id = ELEMENT_ID_SLIDER_LOW,
		.z_index = 0,
		.x = 75,
		.y = 60,
		.width = 156,
		.height = 25,
		.element_type = ADP_ELEMENT_TYPE_SLIDER,
		.minimum_value = 1000,
		.maximum_value = 2400,
		.initial_value = 2000,
	};
	adp_add_slider_to_dashboard(&slider_low);
	
	element_stream.element_id = ELEMENT_ID_SLIDER_LOW;
	element_stream.stream_id = STREAM_ID_HYST_LOW;
	adp_add_stream_to_element(&element_stream);
	
	/* Add segment to dashboard, use light sensor stream */
	/* Add segment stream(light sensor ADC value) */
	stream.stream_id = STREAM_ID_LIGHT_SENSOR_ADC;
	stream.type = ADP_STREAM_UINT_16;
	stream.mode = ADP_STREAM_OUT;
	stream.state = ADP_STREAM_ON;
	adp_set_string(stream.label, "Light Sensor ADC");
	adp_configure_stream(&stream);
	
	struct adp_msg_conf_dashboard_element_segment segment = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR_CON,
		.element_id = ELEMENT_ID_SEGMENT,
		.z_index = 0,
		.x = 500,
		.y = 20,
		.width = 150,
		.height = 50,
		.element_type = (uint8_t)ADP_ELEMENT_TYPE_SEGMENT,
		.segment_count = 4,
		.base = 10,
		.transparency = 255,
		.color = {ADP_COLOR_RED}
	};
	adp_add_segment_to_dashboard(&segment);
	
	element_stream.element_id = ELEMENT_ID_SEGMENT;
	element_stream.stream_id = STREAM_ID_LIGHT_SENSOR_ADC;
	adp_add_stream_to_element(&element_stream);
}

/**
* \brief Add a dashboard and add all elements to the dashboard.
*
*/
static void adp_example_add_dashboard(void)
{
	/* Add a dashboard */
	struct adp_msg_conf_dashboard dashboard = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR,
		.color = {ADP_COLOR_WHITE},
		.height = 300,
	};
	adp_add_dashboard(&dashboard, DASHBOARD_DIS_LABEL);
	
	struct adp_msg_conf_dashboard dashboard_con = {
		.dashboard_id = DASHBOARD_ID_LIGHT_SENSOR_CON,
		.color = {ADP_COLOR_WHITE},
		.height = 150,
	};
	adp_add_dashboard(&dashboard_con, DASHBOARD_CON_LABEL);
	
	adp_example_add_label();
	adp_example_add_element_stream();	
}


/**
* \brief Setup streams needed.
*
*/
static void adp_example_setup_streams(void)
{
	adp_example_add_terminal();
	adp_example_add_dashboard();
}

/**
* \brief Initiate extension board ioport.
*
*/
static void ext_ioport_init(void)
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
	ext_ioport_init();
	adp_example_adc_init();
	
	while (adp_wait_for_handshake() != ADP_HANDSHAKE_ACCEPTED) {
	}

	adp_configure_info("Light Sensor Example for Xplained Pro",
		"This example demonstrates light intensity measurements "
		"through the ADC of a Xplained Pro board. You will need the "
		"IO1 Xplained Pro (EXT1).");
	adp_example_setup_streams();
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
static void adp_example_transceive_single_stream(uint8_t stream_id, uint8_t* data, \
										uint8_t data_size, uint8_t* receive_buf)
{
	if(adp_transceive_single_stream(stream_id, data, data_size, receive_buf) == true) {
		executive_command(receive_buf);
	}
}

static void adp_example_transceive_terminal(struct adp_msg_conf_terminal *const config, \
										const char* label, uint8_t* receive_buf)
{
	if(adp_transceive_terminal(config, label, receive_buf) == true) {
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
	uint8_t* volt_value = &voltage;
	
	/* Send ADC value to graph and progress bar */
	adp_example_transceive_single_stream(STREAM_ID_LIGHT_SENSOR, volt_value, \
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
		/* Receive and send forever */
		transceive_data();
	}
}
