/**
 * \file
 *
 * \brief SAM CAN Quick Start for FD modue
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
/**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <string.h>
#include <conf_can.h>

//! [module_var]

//! [module_inst]
static struct usart_module cdc_instance;
static struct can_module can_instance;
//! [module_inst]


//! [can_filter_setting]
#define CAN_RX_STANDARD_FILTER_INDEX    0
#define CAN_RX_STANDARD_FILTER_ID_0     0x5A
#define CAN_RX_STANDARD_FILTER_ID_1     0x69
#define CAN_RX_EXTENDED_FILTER_INDEX    0
#define CAN_RX_EXTENDED_FILTER_ID_0     0xA5
#define CAN_RX_EXTENDED_FILTER_ID_1     0x96
//! [can_filter_setting]

//! [can_transfer_message_setting]
#define CAN_TX_BUFFER_INDEX    0
static uint8_t tx_message_0[CONF_CAN_ELEMENT_DATA_SIZE];
static uint8_t tx_message_1[CONF_CAN_ELEMENT_DATA_SIZE];
//! [can_transfer_message_setting]

//! [can_receive_message_setting]
static volatile uint32_t standard_receive_index = 0;
static volatile uint32_t extended_receive_index = 0;
static struct can_rx_element_fifo_0 rx_element_fifo_0;
static struct can_rx_element_fifo_1 rx_element_fifo_1;
//! [can_receive_message_setting]

//! [module_var]

//! [setup]

//! [cdc_setup]
static void configure_usart_cdc(void)
{

	struct usart_config config_cdc;
	usart_get_config_defaults(&config_cdc);
	config_cdc.baudrate	 = 38400;
	config_cdc.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	config_cdc.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_cdc.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_cdc.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_cdc.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	stdio_serial_init(&cdc_instance, EDBG_CDC_MODULE, &config_cdc);
	usart_enable(&cdc_instance);
}
//! [cdc_setup]

//! [can_init_setup]
static void configure_can(void)
{
	uint32_t i;
	/* Initialize the memory. */
	for (i = 0; i < CONF_CAN_ELEMENT_DATA_SIZE; i++) {
		tx_message_0[i] = i;
		tx_message_1[i] = i + 0x80;
	}

	/* Set up the CAN TX/RX pins */
	struct system_pinmux_config pin_config;
	system_pinmux_get_config_defaults(&pin_config);
	pin_config.mux_position = CAN_TX_MUX_SETTING;
	system_pinmux_pin_set_config(CAN_TX_PIN, &pin_config);
	pin_config.mux_position = CAN_RX_MUX_SETTING;
	system_pinmux_pin_set_config(CAN_RX_PIN, &pin_config);

	/* Initialize the module. */
	struct can_config config_can;
	can_get_config_defaults(&config_can);
	can_init(&can_instance, CAN_MODULE, &config_can);

	can_switch_operation_mode(&can_instance, CAN_OPERATION_MODE_FD_OPERATION);

	/* Enable interrupts for this CAN module */
	system_interrupt_enable(SYSTEM_INTERRUPT_MODULE_CAN0);
}
//! [can_init_setup]

//! [can_receive_filter_setup]
static void can_set_standard_filter(uint32_t filter_value)
{
	struct can_standard_message_filter_element sd_filter;

	can_get_standard_message_filter_element_default(&sd_filter);
	sd_filter.S0.bit.SFID1 = filter_value;

	can_set_rx_standand_filter(&can_instance, &sd_filter,
			CAN_RX_STANDARD_FILTER_INDEX);
	can_enable_interrupt(&can_instance, CAN_RX_FIFO_0_NEW_MESSAGE);
}

static void can_set_extended_filter(uint32_t filter_value)
{
	struct can_extended_message_filter_element et_filter;

	can_get_extended_message_filter_element_default(&et_filter);
	et_filter.F0.bit.EFID1 = filter_value;

	can_set_rx_extended_filter(&can_instance, &et_filter,
			CAN_RX_STANDARD_FILTER_INDEX);
	can_enable_interrupt(&can_instance, CAN_RX_FIFO_1_NEW_MESSAGE);
}

//! [can_receive_filter_setup]

//! [can_transfer_message_setup]
static void can_send_standard_message(uint32_t id_value, uint8_t *data)
{
	uint32_t i;
	struct can_tx_element tx_element;

	can_get_tx_buffer_element_defaults(&tx_element);
	tx_element.T0.reg |= CAN_TX_ELEMENT_T0_ID(id_value << 18);
	tx_element.T1.reg = CAN_TX_ELEMENT_T1_FDF | CAN_TX_ELEMENT_T1_BRS |
			CAN_TX_ELEMENT_T1_DLC(CAN_TX_ELEMENT_T1_DLC_DATA64_Val);
	for (i = 0; i < CONF_CAN_ELEMENT_DATA_SIZE; i++) {
		tx_element.data[i] = *data;
		data++;
	}

	can_set_tx_buffer_element(&can_instance, &tx_element,
			CAN_TX_BUFFER_INDEX);
	can_tx_transfer_request(&can_instance, 1 << CAN_TX_BUFFER_INDEX);
}

static void can_send_extended_message(uint32_t id_value, uint8_t *data)
{
	uint32_t i;
	struct can_tx_element tx_element;

	can_get_tx_buffer_element_defaults(&tx_element);
	tx_element.T0.reg |= CAN_TX_ELEMENT_T0_ID(id_value) |
			CAN_TX_ELEMENT_T0_XTD;
	tx_element.T1.reg = CAN_TX_ELEMENT_T1_EFC | CAN_TX_ELEMENT_T1_FDF |
			CAN_TX_ELEMENT_T1_BRS |
			CAN_TX_ELEMENT_T1_DLC(CAN_TX_ELEMENT_T1_DLC_DATA64_Val);
	for (i = 0; i < CONF_CAN_ELEMENT_DATA_SIZE; i++) {
		tx_element.data[i] = *data;
		data++;
	}

	can_set_tx_buffer_element(&can_instance, &tx_element,
			CAN_TX_BUFFER_INDEX);
	can_tx_transfer_request(&can_instance, 1 << CAN_TX_BUFFER_INDEX);
}

//! [can_transfer_message_setup]

//! [can_interrupt_handler]
void CAN0_Handler(void)
{
	uint32_t status, i;
	status = can_read_interrupt_status(&can_instance);

	if (status & CAN_RX_FIFO_0_NEW_MESSAGE) {
		can_clear_interrupt_status(&can_instance, CAN_RX_FIFO_0_NEW_MESSAGE);
		can_get_rx_fifo_0_element(&can_instance, &rx_element_fifo_0,
				standard_receive_index);
		can_rx_fifo_acknowledge(&can_instance, 0,
				standard_receive_index);
		standard_receive_index++;
		if (standard_receive_index == CONF_CAN0_RX_FIFO_0_NUM) {
			standard_receive_index = 0;
		}

		printf("\n\r Standard message received. The received data is: \r\n");
		for (i = 0; i < CONF_CAN_ELEMENT_DATA_SIZE; i++) {
			printf("  %d",rx_element_fifo_0.data[i]);
		}
		printf("\r\n\r\n");
	}

	if (status & CAN_RX_FIFO_1_NEW_MESSAGE) {
		can_clear_interrupt_status(&can_instance, CAN_RX_FIFO_1_NEW_MESSAGE);
		can_get_rx_fifo_1_element(&can_instance, &rx_element_fifo_1,
				extended_receive_index);
		can_rx_fifo_acknowledge(&can_instance, 0,
				extended_receive_index);
		extended_receive_index++;
		if (extended_receive_index == CONF_CAN0_RX_FIFO_1_NUM) {
			extended_receive_index = 0;
		}

		printf("\n\r Extended message received. The received data is: \r\n");
		for (i = 0; i < CONF_CAN_ELEMENT_DATA_SIZE; i++) {
			printf("  %d",rx_element_fifo_1.data[i]);
		}
		printf("\r\n\r\n");
	}

}
//! [can_interrupt_handler]

//! [user_menu]
static void display_menu(void)
{
	printf("Menu :\r\n"
			"  -- Select the action:\r\n"
			"  0: Set standard filter ID 0: 0x5A. \r\n"
			"  1: Set standard filter ID 0: 0x69. \r\n"
			"  2: Send standard message with ID: 0x5A and 8 byte data 0 to 7. \r\n"
			"  3: Send standard message with ID: 0x69 and 8 byte data 8 to 15. \r\n"
			"  4: Set extended filter ID 0: 0xA5. \r\n"
			"  5: Set extended filter ID 0: 0x96. \r\n"
			"  6: Send extended message with ID: 0xA5 and 8 byte data 0 to 7. \r\n"
			"  7: Send extended message with ID: 0x96 and 8 byte data 8 to 15. \r\n"
			"  h: Display menu \r\n\r\n");
}
//! [user_menu]

//! [setup]

int main(void)
{
	uint8_t key;

//! [setup_init]
	system_init();
	configure_usart_cdc();
//! [setup_init]

//! [main_setup]

//! [configure_can]
	configure_can();
//! [configure_cab]

//! [display_user_menu]
	display_menu();
//! [display_user_menu]

//! [main_loop]
	while(1) {
		scanf("%c", (char *)&key);

		switch (key) {
		case 'h':
			display_menu();
			break;

		case '0':
			printf("  0: Set standard filter ID 0: 0x5A. \r\n");
			can_set_standard_filter(CAN_RX_STANDARD_FILTER_ID_0);
			break;

		case '1':
			printf("  1: Set standard filter ID 0: 0x69. \r\n");
			can_set_standard_filter(CAN_RX_STANDARD_FILTER_ID_1);
			break;

		case '2':
			printf("  2: Send standard message with ID: 0x5A and 8 byte data 0 to 7. \r\n");
			can_send_standard_message(CAN_RX_STANDARD_FILTER_ID_0, tx_message_0);
			break;

		case '3':
			printf("  3: Send standard message with ID: 0x69 and 8 byte data 8 to 15. \r\n");
			can_send_standard_message(CAN_RX_STANDARD_FILTER_ID_1, tx_message_1);
			break;

		case '4':
			printf("  4: Set extended filter ID 0: 0xA5. \r\n");
			can_set_extended_filter(CAN_RX_STANDARD_FILTER_ID_0);
			break;

		case '5':
			printf("  5: Set extended filter ID 0: 0x96. \r\n");
			can_set_extended_filter(CAN_RX_STANDARD_FILTER_ID_1);
			break;

		case '6':
			printf("  6: Send extended message with ID: 0xA5 and 8 byte data 0 to 7. \r\n");
			can_send_extended_message(CAN_RX_STANDARD_FILTER_ID_0, tx_message_0);
			break;

		case '7':
			printf("  7: Send extended message with ID: 0x96 and 8 byte data 8 to 15. \r\n");
			can_send_extended_message(CAN_RX_STANDARD_FILTER_ID_1, tx_message_1);
			break;

		default:
			break;
		}
	}
//! [main_loop]

//! [main_setup]
}
