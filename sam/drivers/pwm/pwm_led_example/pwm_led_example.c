/**
 * \file
 *
 * \brief PWM LED example for SAM.
 *
 * Copyright (c) 2011 - 2012 Atmel Corporation. All rights reserved.
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
 * \mainpage PWM LED Example
 *
 * \par Purpose
 *
 * This example demonstrates a simple configuration of 2 PWM channels to
 * generate variable duty cycle signals.
 * The 2 LEDs on the evaluation kit will glow repeatedly.
 *
 * \par Usage
 *
 * -# Initialize system clock and pins setting on board
 * -# Initialize PWM clock
 * -# Configure PWM_CHANNEL_LED_0
 * -# Configure PWM_CHANNEL_LED_1
 * -# Enable interrupt of counter event and PWM_CHANNEL_LED_0 & PWM_CHANNEL_LED_1
 * -# Change duty cycle in ISR
 *
 */

#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"

/** Console baud rate */
#define CONSOLE_BAUD_RATE  115200
/** PWM frequency in Hz */
#define PWM_FREQUENCY      1000
/** Period value of PWM output waveform */
#define PERIOD_VALUE       100
/** Initial duty cycle value */
#define INIT_DUTY_VALUE    0

#define STRING_EOL    "\r"
#define STRING_HEADER "-- PWM LED Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/** PWM channel instance for LEDs */
pwm_channel_t g_pwm_channel_led;

/**
 * \brief Interrupt handler for the PWM controller.
 */
void PWM_Handler(void)
{
	static uint32_t ul_count = 0;  /* PWM counter value */
	static uint32_t ul_duty = INIT_DUTY_VALUE;  /* PWM duty cycle rate */
	static uint8_t fade_in = 1;  /* LED fade in flag */
	uint32_t events = pwm_channel_get_interrupt_status(PWM);

	/* Interrupt on PWM_CHANNEL_LED_0 */
	if ((events & PWM_CHANNEL_LED_0) == PWM_CHANNEL_LED_0) {

		ul_count++;

		/* Fade in/out */
		if (ul_count == (PWM_FREQUENCY / (PERIOD_VALUE - INIT_DUTY_VALUE))) {

			/* Fade in */
			if (fade_in) {

				ul_duty++;
				if (ul_duty == PERIOD_VALUE) {

					fade_in = 0;
				}
			}
			/* Fade out */
			else {

				ul_duty--;
				if (ul_duty == INIT_DUTY_VALUE) {

					fade_in = 1;
				}
			}

			/* Set new duty cycle */
			ul_count = 0;
			g_pwm_channel_led.channel = PWM_CHANNEL_LED_0;
			pwm_channel_update_duty(PWM, &g_pwm_channel_led, ul_duty);
			g_pwm_channel_led.channel = PWM_CHANNEL_LED_1;
			pwm_channel_update_duty(PWM, &g_pwm_channel_led, ul_duty);
		}
	}
}

/**
 *  \brief Configure the Console UART.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};
	
	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief Application entry point for PWM with LED example.
 * Output PWM waves on LEDs to make them fade in and out.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	/* Initialize the SAM system */
	sysclk_init();
	board_init();

	/* Configure the console uart for debug information */
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);

	/* Enable PWM peripheral clock */
	pmc_enable_periph_clk(ID_PWM);

	/* Disable PWM channels for LEDs */
	pwm_channel_disable(PWM, PWM_CHANNEL_LED_0 | PWM_CHANNEL_LED_1);

	/* Set PWM clock A as PWM_FREQUENCY * PERIOD_VALUE (clock B is not used) */
	pwm_clock_t clock_setting = {
		.ul_clka = PWM_FREQUENCY * PERIOD_VALUE,
		.ul_clkb = 0,
		.ul_mck = sysclk_get_cpu_hz()
	};
	pwm_init(PWM, &clock_setting);

	/* Initialize PWM channel for LED0 (period is left-aligned and output waveform starts at a low level) */
	g_pwm_channel_led.ul_prescaler = PWM_CMR_CPRE_CLKA;  /* Use PWM clock A as source clock */
	g_pwm_channel_led.ul_period = PERIOD_VALUE;  /* Period value of output waveform */
	g_pwm_channel_led.ul_duty = INIT_DUTY_VALUE;  /* Duty cycle value of output waveform */
	g_pwm_channel_led.channel = PWM_CHANNEL_LED_0;
	pwm_channel_init(PWM, &g_pwm_channel_led);

	/* Enable channel counter event interrupt */
	pwm_channel_enable_interrupt(PWM, PWM_CHANNEL_LED_0, 0);

	/* Initialize PWM channel for LED1 */
	g_pwm_channel_led.alignment = PWM_ALIGN_CENTER;  /* Period is center-aligned */
	g_pwm_channel_led.polarity = PWM_HIGH;  /* Output waveform starts at a high level */
	g_pwm_channel_led.channel = PWM_CHANNEL_LED_1;
	pwm_channel_init(PWM, &g_pwm_channel_led);

	/* Disable channel counter event interrupt */
	pwm_channel_disable_interrupt(PWM, PWM_CHANNEL_LED_1, 0);

	/* Configure interrupt and enable PWM interrupt */
	NVIC_DisableIRQ(PWM_IRQn);
	NVIC_ClearPendingIRQ(PWM_IRQn);
	NVIC_SetPriority(PWM_IRQn, 0);
	NVIC_EnableIRQ(PWM_IRQn);

	/* Enable PWM channels for LEDs */
	pwm_channel_enable(PWM, PWM_CHANNEL_LED_0 | PWM_CHANNEL_LED_1);

	/* Infinite loop */
	while (1) {
	}
}
