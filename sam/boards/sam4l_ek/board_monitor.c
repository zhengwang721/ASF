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
#include "delay.h"

/* USART baudrate. */
#define USART_BAUDRATE      115200

#define MSG_START_PATTERN                0x75 //!< Start pattern
#define MSG_STOP_PATTERN                 0xa3 //!< Stop pattern
#define MSG_LENGTH_DEFAULT               9    //!< Number of bytes to transfer
#define MSG_ID_DEFAULT                   0    //!< Default ID Message

#define BM_POINTER_CTRL    0x01
#define BM_POINTER_MOVE    0x02
#define BM_CTRL            0x03
#define BM_LED_SET         0x04
#define BM_LED_CLR         0x05
#define BM_LED_TGL         0x06
#define BM_MCU_STATUS      0x07
#define BM_PULLUP_TWI      0x08 // Set Pull-up Pin
#define BM_PICOUART_SEND   0x09 // Ask to Board monitor to send PicoUart Frame
#define BM_MCU_GET_CURRENT 0x0A // Request Current Consumption of SAM4L
#define BM_TGL_BUTTON      0x0B // Generate Toggle on Button Line 
#define BM_MCU_RET_CURRENT 0x0C // Return Current Consumption of SAM4L [NOT YET USED]
#define BM_MCU_GET_FIFO_FREE_SIZE  0x0D // Request Current free space of the BM fifo.
#define BM_MCU_RET_FIFO_FREE_SIZE  0x0E // Return Current free space of the BM fifo.
#define BM_PRINT_TEXT      0x0F // Print Text Message on Oled Screen
#define BM_PRINT_CLEAR     0x10 // Clear Print Text Area
#define BM_GET_FIRMWARE_VERSION     0x11 // Get Firmware Version
#define BM_RET_FIRMWARE_VERSION     0x12 // Ret Firmware Version

#define BM_PRINT_TEXT_SIZE        21
#define BM_PRINT_TEXT_DEEP        6
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

void bm_send_mcu_status(uint32_t power_scaling, uint32_t sleep_mode,
		uint32_t cpu_freq, uint32_t cpu_src)
{
	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 10 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_MCU_STATUS);
	usart_putchar(REMOTE_TASK_USART, power_scaling);
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

bool bm_get_mcu_current(uint32_t* sleep_mode, float* current)
{
	uint32_t current_d;
        uint32_t c;
	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 4 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_MCU_GET_CURRENT);
	usart_putchar(REMOTE_TASK_USART, *sleep_mode);
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);
	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
	usart_enable_rx(REMOTE_TASK_USART);
	// Check first caracter is start pattern
	usart_getchar(REMOTE_TASK_USART, &c);
	if (c == MSG_STOP_PATTERN) {
		usart_getchar(REMOTE_TASK_USART, &c);
	}
	if (c != MSG_START_PATTERN) {
		return false;
	}
	// Check second caracter is length
	usart_getchar(REMOTE_TASK_USART, &c);
	if (c != 8) {
		return false;
	}
	// Check third caracter is Current Command
	usart_getchar(REMOTE_TASK_USART, &c);
	if (c != BM_MCU_RET_CURRENT) {
		return false;
	}
	// Check third caracter is sleepmode
	usart_getchar(REMOTE_TASK_USART, &c);
	*sleep_mode = c;
	// Then read current
	usart_getchar(REMOTE_TASK_USART, &c);
	current_d = c<<24;
	usart_getchar(REMOTE_TASK_USART, &c);
	current_d |= c<<16;
	usart_getchar(REMOTE_TASK_USART, &c);
	current_d |= c<<8;
	usart_getchar(REMOTE_TASK_USART, &c);
	current_d |= c;
	*current = *(float*)& current_d;
	// Check last caracter is stop pattern
	usart_getchar(REMOTE_TASK_USART, &c);
	if (c != MSG_STOP_PATTERN) {
		return false;
	}
	usart_disable_rx(REMOTE_TASK_USART);
	return true;
}

bool bm_get_fifo_free_size(uint16_t* free_size)
{
        uint32_t start, length, stop, cmd_id, c;

	// Wait for some microseconds in order to avoid fifo overrun
	//
	delay_ms(20);

	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 3 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_MCU_GET_FIFO_FREE_SIZE);
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);

	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
	usart_enable_rx(REMOTE_TASK_USART);
	
	// Check first character is start pattern
	usart_getchar(REMOTE_TASK_USART, &start);
	if (start == MSG_STOP_PATTERN) {
		usart_getchar(REMOTE_TASK_USART, &start);
	}
	if (start != MSG_START_PATTERN) {
		return false;
	}
	// Check second character is length
	usart_getchar(REMOTE_TASK_USART, &length);
	if (length != 5) {
		return false;
	}
	// Check third character is Current Command
	usart_getchar(REMOTE_TASK_USART, &cmd_id);
	if (cmd_id != BM_MCU_RET_FIFO_FREE_SIZE) {
		return false;
	}

	// Get Fifo free size
	usart_getchar(REMOTE_TASK_USART, &c);
	*free_size = c << 8;
	usart_getchar(REMOTE_TASK_USART, &c);
	*free_size |= c;

	// Check last character is stop pattern
	usart_getchar(REMOTE_TASK_USART, &stop);
	if (stop != MSG_STOP_PATTERN) {
		return false;
	}
	usart_disable_rx(REMOTE_TASK_USART);
	return true;
}

void bm_send_picouart_frame(uint8_t frame, uint32_t timeout_ms)
{
	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 8 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_PICOUART_SEND);
	usart_putchar(REMOTE_TASK_USART, frame);
	usart_putchar(REMOTE_TASK_USART, timeout_ms & 0xff);
	usart_putchar(REMOTE_TASK_USART, (timeout_ms >> 8 ) & 0xff);
	usart_putchar(REMOTE_TASK_USART, (timeout_ms >> 16) & 0xff);
	usart_putchar(REMOTE_TASK_USART, timeout_ms >> 24);
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);
	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
}

void bm_tgl_button(uint32_t timeout_ms)
{
	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 7 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_TGL_BUTTON);
	usart_putchar(REMOTE_TASK_USART, timeout_ms & 0xff);
	usart_putchar(REMOTE_TASK_USART, (timeout_ms >> 8 ) & 0xff);
	usart_putchar(REMOTE_TASK_USART, (timeout_ms >> 16) & 0xff);
	usart_putchar(REMOTE_TASK_USART, timeout_ms >> 24);
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);
	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
}

void bm_pullup_twi(bool state)
{
	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 4 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_PULLUP_TWI);
	usart_putchar(REMOTE_TASK_USART, state);
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);
	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
}

void bm_print_clear(void)
{
	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 3 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_PRINT_CLEAR);
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);
	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
}

void bm_print_txt(uint8_t* str, uint8_t str_length)
{
	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, BM_PRINT_TEXT_SIZE + 3 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_PRINT_TEXT);
	if (str_length < BM_PRINT_TEXT_SIZE) {
		for (uint8_t i=0;i<str_length;i++){
			usart_putchar(REMOTE_TASK_USART, str[i]);
		}
		for (uint8_t i=0;i < BM_PRINT_TEXT_SIZE - str_length;i++){
			usart_putchar(REMOTE_TASK_USART, '\0');
		}
	} else {
		for (uint8_t i=0;i<BM_PRINT_TEXT_SIZE;i++){
			usart_putchar(REMOTE_TASK_USART, str[i]);
		}
	}
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);
	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
}

bool bm_get_firmware_version(uint8_t* fw_minor_version, uint8_t* fw_major_version)
{
        uint32_t start, length, stop, cmd_id, c;

	// Wait for some microseconds in order to avoid fifo overrun
	//
	delay_ms(20);

	usart_enable_tx(REMOTE_TASK_USART);
	usart_putchar(REMOTE_TASK_USART, MSG_START_PATTERN);
	usart_putchar(REMOTE_TASK_USART, 3 /* length */);
	usart_putchar(REMOTE_TASK_USART, BM_GET_FIRMWARE_VERSION);
	usart_putchar(REMOTE_TASK_USART, MSG_STOP_PATTERN);

	while (!usart_is_tx_empty(REMOTE_TASK_USART));
	usart_disable_tx(REMOTE_TASK_USART);
	usart_enable_rx(REMOTE_TASK_USART);
	
	// Check first character is start pattern
	usart_getchar(REMOTE_TASK_USART, &start);
	if (start == MSG_STOP_PATTERN) {
		usart_getchar(REMOTE_TASK_USART, &start);
	}
	if (start != MSG_START_PATTERN) {
		return false;
	}
	// Check second character is length
	usart_getchar(REMOTE_TASK_USART, &length);
	if (length != 5) {
		return false;
	}
	// Check third character is Current Command
	usart_getchar(REMOTE_TASK_USART, &cmd_id);
	if (cmd_id != BM_RET_FIRMWARE_VERSION) {
		return false;
	}

	// Get Fifo free size
	usart_getchar(REMOTE_TASK_USART, &c);
	*fw_major_version = c;
	usart_getchar(REMOTE_TASK_USART, &c);
	*fw_minor_version = c;

	// Check last character is stop pattern
	usart_getchar(REMOTE_TASK_USART, &stop);
	if (stop != MSG_STOP_PATTERN) {
		return false;
	}
	usart_disable_rx(REMOTE_TASK_USART);
	return true;
}
