/**
 *
 * \file
 *
 * \brief SAM UART Driver for SAMB11
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

#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#include <compiler.h>
#include <system_samb11.h>
#include <gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \brief UART byte bit selection
 *
 * Number of bit per byte selection for UART communication.
 */
enum uart_number_of_bit_selection{
	/** 8 bit per byte*/
	UART_8_BITS = UART_UART_CONFIGURATION_NUMBER_OF_BITS_0,
	/** 7 bit per byte*/
	UART_7_BITS = UART_UART_CONFIGURATION_NUMBER_OF_BITS_1,
};

/** \brief UART stop bit selection
 *
 * Number of stop bit selection for UART communication.
 */
enum uart_stop_bit_selection{
	/** 1 stop bit per byte*/
	UART_1_STOP_BIT = UART_UART_CONFIGURATION_STOP_BITS_0,
	/** 2 stop bit per byte*/
	UART_2_STOP_BITS = UART_UART_CONFIGURATION_STOP_BITS_1,
};


/** \brief UART Parity selection
 *
 * Parity type selection for UART communication.
 */
enum uart_parity_selection{
	/** No parity bit */
	UART_NO_PARITY = 0,
	/** Even parity */
	UART_EVEN_PARITY,
	/** Odd parity */
	UART_ODD_PARITY,
	/** Space parity */
	UART_SPACE_PARITY,
	/** Mark parity */
	UART_MARK_PARITY
};

/**
 * \brief UART module instance
 *
 * Forward Declaration for the device instance.
 */
struct uart_module;

/**
 * \brief UART callback type
 *
 * Type of the callback functions.
 */
typedef void (*uart_callback_t)(struct uart_module *const module);

/**
 * \brief UART Callback enum
 *
 * Callbacks for the UART driver.
 */
enum uart_callback {
	/** Callback for TX FIFO not full. */
	UART_TX_FIFO_NOT_FULL,
	/** Callback for TX FIFO empty. */
	UART_TX_FIFO_EMPTY,
	/** Callback for RX FIFO not empty. */
	UART_RX_FIFO_NOT_EMPTY,
	/** Callback for RX FIFO overrun. */
	UART_RX_FIFO_OVERRUN,
	/** Callback for CTS active. */
	UART_CTS_ACTIVE,
	/** Number of available callbacks. */
	UART_CALLBACK_N,
};


/**
 * \brief Configuration structure for the UART module
 *
 * This is the configuration structure for the UART Module in SAMB11. It
 * is used as an argument for \ref uart_init to provide the desired
 * configurations for the module. The structure should be initialized using the
 * \ref uart_get_config_defaults .
 */
struct uart_config{
	/** Baud rate */
	uint32_t baud_rate;
	/** Number of data bits */
	enum uart_number_of_bit_selection data_bits;
	/** Number of stop bits */
	enum uart_stop_bit_selection stop_bits;
	/** Parity type */
	enum uart_parity_selection parity;
	/** flow control type */
	bool flow_control;
	/** UART PAD pinmux */
	uint32_t pinmux_pad[4];
};

/**
 * \brief UART driver software device instance structure.
 *
 * UART driver software instance structure, used to retain software
 * state information of an associated hardware module instance.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct uart_module {
#if !defined(__DOXYGEN__)
	/** Pointer to the hardware instance. */
	Uart *hw;
	/** Array to store callback function pointers in. */
	uart_callback_t callback[UART_CALLBACK_N];
	/** Bit mask for callbacks registered. */
	uint8_t callback_reg_mask;
	/** Bit mask for callbacks enabled. */
	uint8_t callback_enable_mask;
#endif
};

/**
@defgroup uart-drv UART Driver API

@{
*/

void uart_get_config_defaults(struct uart_config *const config);
enum status_code uart_init(struct uart_module *const module, Uart * const hw,
		const struct uart_config *const config);
enum status_code uart_write_wait(struct uart_module *const module,
		const uint8_t tx_data);
enum status_code uart_read_wait(struct uart_module *const module,
		uint8_t *const rx_data);
enum status_code uart_write_buffer_wait(struct uart_module *const module,
		const uint8_t *tx_data, uint32_t length);
enum status_code uart_read_buffer_wait(struct uart_module *const module,
		uint8_t *rx_data, uint16_t length);
void uart_register_callback(struct uart_module *const module,
		uart_callback_t callback_func,
		enum uart_callback callback_type);
void uart_unregister_callback(struct uart_module *module,
		enum uart_callback callback_type);
void uart_enable_callback(struct uart_module *const module,
		enum uart_callback callback_type);
void uart_disable_callback(struct uart_module *const module,
		enum uart_callback callback_type);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif
