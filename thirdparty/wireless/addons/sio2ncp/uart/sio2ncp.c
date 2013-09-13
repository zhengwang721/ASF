/**
 * \file sio2ncp.c
 *
 * \brief Handles Serial I/O  Functionalities
 *
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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
 */

/* === INCLUDES ============================================================ */

#include "asf.h"
#include "sio2ncp.h"
#include "conf_sio2ncp.h"
/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === PROTOTYPES ========================================================== */

/* === GLOBALS ========================================================== */
static usart_serial_options_t usart_serial_options = {
	.baudrate     = USART_NCP_BAUDRATE,
	.charlength   = USART_NCP_CHAR_LENGTH,
	.paritytype   = USART_NCP_PARITY,
	.stopbits     = USART_NCP_STOP_BITS
};

/**
 * Receive buffer
 * The buffer size is defined in sio2ncp.h
 */
static uint8_t serial_rx_buf[SERIAL_RX_BUF_SIZE_NCP];

/**
 * Receive buffer head
 */
static uint8_t serial_rx_buf_head;

/**
 * Receive buffer tail
 */
static uint8_t serial_rx_buf_tail;

/**
 * Number of bytes in receive buffer
 */
static uint8_t serial_rx_count;

/* === IMPLEMENTATION ====================================================== */

void sio2ncp_init(void)
{
#if defined (NCP_RESET_GPIO)
	ioport_set_pin_dir(NCP_RESET_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(NCP_RESET_GPIO, IOPORT_PIN_LEVEL_HIGH);
#endif //(NCP_RESET_GPIO)

	usart_serial_init(USART_NCP, &usart_serial_options);
	USART_NCP_RX_ISR_ENABLE();
}

uint8_t sio2ncp_tx(uint8_t *data, uint8_t length)
{
	status_code_t status;
	do {
		status = usart_serial_write_packet(USART_NCP,
				(const uint8_t *)data,
				length);
	} while (status != STATUS_OK);
	return length;
}

uint8_t sio2ncp_rx(uint8_t *data, uint8_t max_length)
{
	uint8_t data_received = 0;
	if (0 == serial_rx_count) {
		return 0;
	}

	if (SERIAL_RX_BUF_SIZE_NCP <= serial_rx_count) {
		/*
		 * Bytes between head and tail are overwritten by new data.
		 * The oldest data in buffer is the one to which the tail is
		 * pointing. So reading operation should start from the tail.
		 */
		serial_rx_buf_head = serial_rx_buf_tail;

		/*
		 * This is a buffer overflow case. But still only the number of
		 *bytes equivalent to
		 * full buffer size are useful.
		 */
		serial_rx_count = SERIAL_RX_BUF_SIZE_NCP;

		/* Bytes received is more than or equal to buffer. */
		if (SERIAL_RX_BUF_SIZE_NCP <= max_length) {
			/*
			 * Requested receive length (max_length) is more than
			 *the
			 * max size of receive buffer, but at max the full
			 * buffer can be read.
			 */
			max_length = SERIAL_RX_BUF_SIZE_NCP;
		}
	} else {
		/* Bytes received is less than receive buffer maximum length. */
		if (max_length > serial_rx_count) {
			/*
			 * Requested receive length (max_length) is more than
			 *the data
			 * present in receive buffer. Hence only the number of
			 *bytes
			 * present in receive buffer are read.
			 */
			max_length = serial_rx_count;
		}
	}

	data_received = max_length;
	while (max_length > 0) {
		/* Start to copy from head. */
		*data = serial_rx_buf[serial_rx_buf_head];
		serial_rx_buf_head++;
		serial_rx_count--;
		data++;
		max_length--;
		if ((SERIAL_RX_BUF_SIZE_NCP) == serial_rx_buf_head) {
			serial_rx_buf_head = 0;
		}
	}
	return data_received;
}

uint8_t sio2ncp_getchar(void)
{
	uint8_t c;

	while (0 == sio2ncp_rx(&c, 1)) {
	}
	return c;
}

int sio2ncp_getchar_nowait(void)
{
	uint8_t c;

	int back = sio2ncp_rx(&c, 1);
	if (back >= 1) {
		return c;
	} else {
		return (-1);
	}
}

USART_NCP_ISR_VECT()
{
	uint8_t temp;

	usart_serial_read_packet(USART_NCP, &temp, 1);
	/* Introducing critical section to avoid buffer corruption. */
	cpu_irq_disable();

	/* The number of data in the receive buffer is incremented and the
	 *buffer is updated. */
	serial_rx_count++;

	serial_rx_buf[serial_rx_buf_tail] = temp;

	if ((SERIAL_RX_BUF_SIZE_NCP - 1) == serial_rx_buf_tail) {
		/* Reached the end of buffer, revert back to beginning of
		 *buffer. */
		serial_rx_buf_tail = 0x00;
	} else {
		serial_rx_buf_tail++;
	}

	cpu_irq_enable();
}

/** EOF */
