/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include "platform.h"
#include "conf_serialdrv.h"
#include "serial_drv.h"

static uint16_t rx_data;
volatile uint32_t ntext, txdtext;

void serial_rx_callback(void)
{
	platform_interface_callback((uint8_t *)&rx_data, 1);
	serial_read_byte((uint8_t *)&rx_data);
	ntext++;
}

void serial_tx_callback(void)
{
	port_pin_set_output_level(PIN_PB07, false);
	txdtext++;
}


at_ble_status_t platform_init(void* platform_params)
{
	struct port_config pin_conf;
	
	configure_serial_drv();
	serial_read_byte((uint8_t *)&rx_data);
	
	port_get_config_defaults(&pin_conf);

	/* Configure LEDs as outputs, turn them off */
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(PIN_PB07, &pin_conf);
	port_pin_set_output_level(PIN_PB07, false);	
	
	return AT_BLE_SUCCESS;
}

void platform_interface_send(uint8_t* data, uint32_t len)
{
	port_pin_set_output_level(PIN_PB07, true);
	serial_drv_send(data, len);
	
}

static volatile uint32_t cmd_cmpl_flag = 0;
static volatile uint32_t event_flag = 0;

void platform_cmd_cmpl_signal()
{
	cmd_cmpl_flag = 1;
}

void platform_cmd_cmpl_wait()
{
	while(cmd_cmpl_flag != 1);
	cmd_cmpl_flag = 0;
}

void platform_event_signal()
{
	event_flag = 1;
}

void platform_event_wait()
{
	while(event_flag != 1);
	event_flag = 0;
}

