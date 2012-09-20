/**
 * \file
 *
 * \brief SAM4L-EK Board Monitor Control.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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

#include "board_monitor.h"

/* USART baudrate. */
#define USART_BAUDRATE      115200

#define MSG_START_PATTERN                0x75 //!< Start pattern
#define MSG_STOP_PATTERN                 0xa3 //!< Stop pattern
#define MSG_LENGTH_DEFAULT               9    //!< Number of bytes to transfer
#define MSG_ID_DEFAULT                   0    //!< Default ID Message

#define BM_POINTER_CTRL 0x01
#define BM_POINTER_MOVE 0x02
#define BM_CTRL         0x03
#define BM_LED_SET      0x04
#define BM_LED_CLR      0x05
#define BM_LED_TGL      0x06
#define BM_MCU_STATUS   0x07

/**
 *  \brief Configure the USART.
 */
static void configure_usart(void)
{
	static uint32_t ul_sysclk;
	const sam_usart_opt_t usart_console_settings = {
		USART_BAUDRATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		0
	};

	/* Get system clock. */
	ul_sysclk = sysclk_get_cpu_hz();

	/* Configure sysclk. */
	sysclk_enable_peripheral_clock(REMOTE_TASK_USART);

	/* Configure UART. */
	usart_init_rs232(REMOTE_TASK_USART, &usart_console_settings, ul_sysclk);

}

void bm_init(void)
{
	configure_usart();
}

void bm_ctrl(bool state)
{
	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 4 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_CTRL);
	usart_putchar(REMOTE_TASK_USART, state);
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);
	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
}
void bm_mouse_pointer_ctrl(bool state)
{
	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 4 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_POINTER_CTRL);
	usart_putchar(REMOTE_TASK_USART, state);
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);
	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
}
void bm_mouse_pointer_move(uint32_t x, uint32_t y)
{
	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 5 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_POINTER_MOVE);
	usart_putchar(REMOTE_TASK_USART, x);
	usart_putchar(REMOTE_TASK_USART, y);
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);
	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
}
void bm_led_set(uint32_t led)
{
	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 4 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_LED_SET);
	usart_putchar(REMOTE_TASK_USART, led);
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);
	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
}
void bm_led_clr(uint32_t led)
{
	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 4 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_LED_CLR);
	usart_putchar(REMOTE_TASK_USART, led);
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);
	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
}
void bm_led_tgl(uint32_t led)
{
	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 4 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_LED_TGL);
	usart_putchar(REMOTE_TASK_USART, led);
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);
	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
}

void bm_send_mcu_status(uint32_t voltage_scaling, uint32_t sleep_mode,
		uint32_t cpu_freq, uint32_t cpu_src)
{
	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 10 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_MCU_STATUS);
	usart_putchar(REMOTE_TASK_USART, voltage_scaling);
	usart_putchar(REMOTE_TASK_USART, sleep_mode);
	usart_putchar(REMOTE_TASK_USART, cpu_freq >> 24);
	usart_putchar(REMOTE_TASK_USART, (cpu_freq >> 16) & 0xff);
	usart_putchar(REMOTE_TASK_USART, (cpu_freq >> 8 ) & 0xff);
	usart_putchar(REMOTE_TASK_USART, cpu_freq & 0xff);
	usart_putchar(REMOTE_TASK_USART, cpu_src);
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);
	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
}

