/**
 * \file
 *
 * \brief C8263A LCD Glass component example for SAM.
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
 * \mainpage
 * \section intro Introduction
 * This is the documentation for the data structures, functions, variables,
 * defines, enums, and typedefs for the C8263A LCD Glass component example.
 * It also comes bundled with an application-example of usage.
 *
 * This example demonstrates how to use the C8263A LCD Glass driver with the
 * 32 KHz oscillator crystal.
 *
 * \section files Main Files
 * - c8263a.c: C8263A LCD Glass component driver using SLCD;
 * - c8263a.h: C8263A LCD Glass component driver header file;
 * - c8263a_example.c: C8263A LCD Glass component example application.
 *
 * \section compilinfo Compilation Information
 * This software is written for GNU GCC and IAR Embedded Workbench
 * for Atmel. Other compilers may or may not work.
 *
 * \section deviceinfo Device Information
 * All SAM devices with an SLCD connected to C8263A LCD Glass
 * component and a USART module can be used.
 *
 * \section configinfo Configuration Information
 * - PC terminal settings:
 *   - 115200 bps,
 *   - 8 data bits,
 *   - no parity bit,
 *   - 1 stop bit,
 *   - no flow control.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com">Atmel</A>.\n
 * Support and FAQ: http://www.atmel.com/design-support/
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <string.h>
#include "config_c8263a.h"
struct usart_module usart_instance;

/**
 *  Configure serial console.
 */
static void configure_console(void)
{
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	config_usart.baudrate    = 38400;
	config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	stdio_serial_init(&usart_instance, EDBG_CDC_MODULE, &config_usart);
	usart_enable(&usart_instance);
}

/**
 * \brief Display the user menu on the terminal.
 */
static void display_menu(void)
{
	printf("\n\r");
	printf("===============================================\n\r");
	printf("Menu: press a key to continue.\n\r");
	printf("===============================================\n\r");

	printf("  a : Clear all\n\r");
	printf("  b : Show ICON\n\r");
	printf("  c : Show Text\n\r");
	printf("  d : Show decial num\n\r");
	printf("  e : Show float num\n\r");
	printf("  f : Show temperature \n\r");
	printf("  g : Show Time\n\r");

	printf("-----------------------------------------------\n\r");
	printf(">>");
}


/**
 * \brief main function : do init and loop
 */
int main(void)
{
	uint8_t key;

	struct c8263a_time val;
	system_init();
	configure_console();
	delay_init();

	/* Turn on the backlight. */
	//port_set_pin_level(LED0, 1);

	/* Initialize the C8263A LCD glass component. */
	c8263a_init();


	printf("Show all the components on the glass.\r\n");
	c8263a_show_all();

	printf("Press any key to continue.\r\n");
	scanf("%c", (char *)&key);

	c8263a_clear_all();

	/* Display menu */
	display_menu();

	while (1) {
		scanf("%c", (char *)&key);

		switch (key) {
		case 'a':
			printf("Clear all the screen.\r\n");
			c8263a_clear_all();
			break;
		case 'b':
			c8263a_show_icon(C8263A_ICON_BAT_LEVEL_1);
			delay_s(1);
			c8263a_show_icon(C8263A_ICON_BAT_LEVEL_2);
			delay_s(1);
			c8263a_show_icon(C8263A_ICON_BAT_LEVEL_3);
			c8263a_show_icon(C8263A_ICON_WIRELESS_LEVEL_3);
			delay_s(1);
			c8263a_show_icon(C8263A_ICON_WIRELESS_LEVEL_2);
			delay_s(1);
			
			c8263a_show_icon(C8263A_ICON_WIRELESS_LEVEL_1);
			break;
		case 'c':
			c8263a_show_text((const uint8_t *)"Welcome");
			break;
		case 'd':
			c8263a_show_numeric_dec(199999);
			delay_s(1);
			break;
		case 'e':
			c8263a_show_numeric_float(1.001);
			delay_s(1);
			break;
		case 'f':
			c8263a_show_temperature(32.1,true);
			delay_s(1);
			break;
		case 'g':
			val.hour = 18;
			val.minute = 10;
			val.pm = true;
			c8263a_show_time(val);
			delay_s(1);
			break;
		default:
			break;
		}
	}

}
