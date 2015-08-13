/**
 * \file
 *
 * \brief SAM TIMER Driver Quick Start for SAMB11
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
#include <asf.h>
#include <stdio_serial.h>
#include <string.h>
#include "conf_dualtimer.h"

#define PRINTF(s)   uart_write_buffer_wait(&uart_instance, (const uint8_t *)s, strlen(s))

//! [module_inst]
struct uart_module uart_instance;
//! [module_inst]

//! [callback_funcs]
static void dualtimer_callback1(void)
{
	printf("Timer1 trigger\r\n");
}

static void dualtimer_callback2(void)
{
	printf("Timer2 trigger\r\n");
}
//! [callback_funcs]

//! [setup]
static void configure_uart(void)
{
//! [setup_uart_1]
	struct uart_config config_uart;
//! [setup_uart_1]
//! [setup_uart_2]
	uart_get_config_defaults(&config_uart);
//! [setup_uart_2]
//! [setup_uart_3]
	config_uart.baud_rate = 38400;
	config_uart.pinmux_pad[0] = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_uart.pinmux_pad[1] = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_uart.pinmux_pad[2] = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_uart.pinmux_pad[3] = EDBG_CDC_SERCOM_PINMUX_PAD3;
//! [setup_uart_3]
//! [setup_uart_4]
	stdio_serial_init(&uart_instance, EDBG_CDC_MODULE, &config_uart);
//! [setup_uart_4]
//! [setup_uart_5]
	while (uart_init(&uart_instance,
			EDBG_CDC_MODULE, &config_uart) != STATUS_OK) {
	}
//! [setup_uart_5]

}

static void configure_dualtimer(void)
{
//! [setup_dualtimer_1]
	struct dualtimer_config config_dualtimer;
//! [setup_dualtimer_1]

//! [setup_dualtimer_2]
	dualtimer_get_config_defaults(&config_dualtimer);
//! [setup_dualtimer_2]
//! [setup_dualtimer_3]
	config_dualtimer.timer1.load_value = CONF_DUALTIMER_TIMER1_LOAD;
//! [setup_dualtimer_3]
//! [setup_dualtimer_4]
	config_dualtimer.timer2.load_value = CONF_DUALTIMER_TIMER2_LOAD;
//! [setup_dualtimer_4]

//! [setup_dualtimer_5]
	dualtimer_init(&config_dualtimer);
//! [setup_dualtimer_5]

	if (config_dualtimer.timer1.timer_enable)
//! [setup_dualtimer_6]
		dualtimer_enable(DUALTIMER_TIMER1);
//! [setup_dualtimer_6]

	if (config_dualtimer.timer2.timer_enable)
//! [setup_dualtimer_7]
		dualtimer_enable(DUALTIMER_TIMER2);
//! [setup_dualtimer_7]
}

static void configure_dualtimer_callback(void)
{
	//! [setup_register_callback]
	dualtimer_register_callback(DUALTIMER_TIMER1, dualtimer_callback1);
	dualtimer_register_callback(DUALTIMER_TIMER2, dualtimer_callback2);
	//! [setup_register_callback]
	//! [setup_register_isr]
	system_register_isr(RAM_ISR_TABLE_DUALTIMER_INDEX, (uint32_t)dualtimer_isr_handler);
	//! [setup_register_isr]
	
	/* For A4, timer0 IRQ is 14 */
	//! [enable_IRQ]
	NVIC_EnableIRQ(10);
	//! [enable_IRQ]
}
//! [setup]

int main(void)
{
//! [setup_init]
	system_clock_config(CLOCK_RESOURCE_RC_26_MHZ, CLOCK_FREQ_26_MHZ);
	
	configure_uart();

	configure_dualtimer();

	configure_dualtimer_callback();
//! [setup_init]
	
//! [main_imp]
//! [main_loop]
	while (true) {
		#if 0
//! [main_loop]
//! [timer1_interrupt]
		if (dualtimer_get_status(DUALTIMER_TIMER1)) {
//! [timer1_interrupt]
//! [timer1_interrupt_clr]
			dualtimer_clear_interrupt_status(DUALTIMER_TIMER1);
//! [timer1_interrupt_clr]
//! [print_timer1]
			PRINTF("Timer1 trigger\r\n");
//! [print_timer1]
		}
//! [timer2_interrupt]
		if (dualtimer_get_status(DUALTIMER_TIMER2)) {
//! [timer2_interrupt]
//! [timer2_interrupt_clr]
			dualtimer_clear_interrupt_status(DUALTIMER_TIMER2);
//! [timer2_interrupt_clr]
//! [print_timer2]
			PRINTF("Timer2 trigger\r\n");
//! [print_timer2]
		}
		#endif
	}
//! [main_imp]
}
