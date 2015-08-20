/**
 * \file
 *
 * \brief Getting Started Application.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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
 * \mainpage Getting Started Application
 *
 * \section Purpose
 *
 * The Getting Started example will help new users get familiar with Atmel's
 * SAM family of microcontrollers. This basic application shows the startup
 * sequence of a chip and how to use its core peripherals.
 *
 * \section Requirements
 *
 * This application has been tested on following boards:
 * - SAM B11 Xplained Pro
 *
 * \section Description
 *
 * The program demo how LED,button,interrupt and timer work.
 * It makes the LED on the board blink at a fixed rate.
 * The blinking can be stopped/started by using the push button.
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 38400 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# The LED(s) should start blinking on the board. In the terminal window, the
 *    following text should appear (values depend on the board and chip used):
 *    \code
 *     -- Getting Started Example xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *    \endcode
 * -# Pressing and release button SW0 should make LED0 stop and restart blinking.
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <conf_uart_serial.h>
#include <conf_example.h>


#define STRING_EOL    "\r"
#define STRING_HEADER "-- Getting Started Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --\r\n" \
		"-- Pressing and release button SW0 should make LED0 on and off --"STRING_EOL


#ifdef __cplusplus
extern "C" {
#endif

static struct uart_module cdc_uart_module;

volatile bool g_b_led0_active;

static void delay(uint32_t count)
{
	for (uint32_t i = 0; i < count; i++) {
		for (uint32_t j = 0; j < 100; j++)
			asm volatile ("nop");
	}
}

/**
 *  Configure UART console.
 */
static void configure_console(void)
{
	struct uart_config config_uart;

	uart_get_config_defaults(&config_uart);

	config_uart.baud_rate = CONF_STDIO_BAUDRATE;
	config_uart.pinmux_pad[0] = CONF_STDIO_PINMUX_PAD0;
	config_uart.pinmux_pad[1] = CONF_STDIO_PINMUX_PAD1;
	config_uart.pinmux_pad[2] = CONF_STDIO_PINMUX_PAD2;
	config_uart.pinmux_pad[3] = CONF_STDIO_PINMUX_PAD3;

	stdio_serial_init(&cdc_uart_module, CONF_STDIO_USART_MODULE, &config_uart);
}

/** Callback function for the GPIO driver, called when a GPIO interrupt
 *  detection occurs.
 */
static void gpio_callback(void)
{
	g_b_led0_active = !g_b_led0_active;
}

/** Configures and registers the GPIO callback function with the
 *  driver.
 */
static void configure_gpio_callback(void)
{
	gpio_init();
	gpio_register_callback(BUTTON_0_PIN, gpio_callback,
			GPIO_CALLBACK_FALLING);
	gpio_enable_callback(BUTTON_0_PIN);
}

static void configure_gpio_pins(void)
{
	struct gpio_config config_gpio_pin;

	gpio_get_config_defaults(&config_gpio_pin);

	config_gpio_pin.direction  = GPIO_PIN_DIR_INPUT;
	config_gpio_pin.input_pull = GPIO_PIN_PULL_UP;

	gpio_pin_set_config(BUTTON_0_PIN, &config_gpio_pin);

	config_gpio_pin.direction = GPIO_PIN_DIR_OUTPUT;

	gpio_pin_set_config(LED_0_PIN, &config_gpio_pin);
}


/** Timer Callback function.
 */
static void timer_callback(void)
{
	puts("The output is triggered by Timer \r\n");
}

/** Configures Timer function with the  driver.
 */
static void configure_timer(void)
{
	struct timer_config config_timer;

	timer_get_config_defaults(&config_timer);

	config_timer.reload_value = CONF_TIMER_RELOAD_VALUE;

	timer_init(&config_timer);

	timer_enable();
}

/** Registers Timer callback function with the  driver.
 */
static void configure_timer_callback(void)
{
	//! [setup_register_callback]
	timer_register_callback(timer_callback);
	//! [setup_register_callback]
	
	/* For A4, timer0 IRQ is 9 */
	//! [enable_IRQ]
	NVIC_EnableIRQ(9);
	//! [enable_IRQ]
}

/**
 *  \brief getting-started Application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
*/
int main(void)
{
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);

	/*Configure UART console.*/
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);

	/*Configures GPIO pins */
	configure_gpio_pins();

	/*Configures GPIO callback */
	configure_gpio_callback();

	/* Configures Timer driver */
	configure_timer();

	/* Configures Timer callback */
	configure_timer_callback();

	g_b_led0_active = true;

	/*main loop*/
	while (1) {
		/* Wait for LED to be active */
		while (!g_b_led0_active);

		/* Toggle LED state if active */
		if (g_b_led0_active) {
			gpio_pin_toggle_output_level(LED_0_PIN);
		}

		/* Wait for some time */
		delay(CONF_DELAY_VALUE);
	}

}

#ifdef __cplusplus
}
#endif
