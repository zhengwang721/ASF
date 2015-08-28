/**
****************************************************************************************
*
* @file uart.h
*
* @brief SAMB11 UART API for Applications
*
* This module contains the public API and the necessary enumerations and structures that are required for 
* SAMB11 Application Developers using UART module
*
*
*  Copyright (c) 2015 Atmel Corporation. All rights reserved.
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*
*  1. Redistributions of source code must retain the above copyright notice, this
*  list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright notice,
*  this list of conditions and the following disclaimer in the documentation
*  and/or other materials provided with the distribution.
*
*  3. The name of Atmel may not be used to endorse or promote products derived from this software 
*  without specific prior written permission.
*
*  4. This software may only be redistributed and used in connection with an Atmel microcontroller product.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
*  THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
*  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
*  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
*  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
****************************************************************************************
*/
#ifndef __UART_H__
#define __UART_H__


#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief UART Driver Status codes.
 *
 * Status codes of UART drivers.
 */
enum uart_status_code {
	UART_STATUS_OK = 0,
	UART_STATUS_BUSY,
	UART_STATUS_ERR_IO,
	UART_STATUS_ERR_OVERFLOW,
	UART_STATUS_ERR_PROTOCOL,
	UART_STATUS_ERR_NOT_INITIALIZED,
	UART_STATUS_ERR_DENIED,
	UART_STATUS_ERR_INVALID_ARG,
	UART_STATUS_ERR_UNSUPPORTED_DEV,
	UART_STATUS_ERR_GPIO_NOT_AVAILABLE,
};

/** \brief UART Core index
 *
 * Some version of chip has multiple UART modules.
 */
enum uart_hw_module{
	UART_HW_MODULE_UART0 = 0,
	UART_HW_MODULE_UART1
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
	/** Spacke parity */
	UART_SPACE_PARITY,
	/** Mark parity */
	UART_MARK_PARITY
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
	/** Parity type */
	enum uart_parity_selection parity;
	/** flow control type */
	uint8_t flow_control;
	/** Number of stop bits */
	uint8_t stop_bits;
	/** Number of data bits */
	uint8_t data_bits;
	/** PAD0 (RXD) pinmux. */
	/** PAD1 (TXD) pinmux. */
	uint16_t pinmux_pad[2];	
};

/**
@defgroup uart-drv UART Driver API

@{
*/

/**
 * \brief Gets the UART default configurations
 *
 * Use to initialize the configuration structure to known default values.
 *
 * The default configuration is as follows:
 * - Baudrate 115200
 * - parity   UART_NO_PARITY
 * - flow_control 0 - No Flow control
 * - stop_bits 1 - 1 stop bit
 * - pinmux_pad[] - Pinmux for UART_RXD and UART_TXD of CORE 1.
 *									Core 0 is by default not available.
 *
 * \param[out] config  Pointer to configuration structure to be initiated
 */
void uart_get_config_defaults(
		struct uart_config *const config);

/**
* \brief Initializes the device
*
* Initializes the UART device based on the setting specified in the
* configuration struct.
*
* \param[in]  module  enumeration UART hw module
* \param[in]  config  Pointer to configuration struct
*
* \return Status of the initialization.
*
* \retval UART_STATUS_OK                       The initialization was successful
* \retval UART_STATUS_ERR_GPIO_NOT_AVAILABLE   Requested GPIO for UART-RXD and UART-TXD is not available.
*/
enum uart_status_code uart_init(enum uart_hw_module module, const struct uart_config *const config);


/**
* \brief Transmit a character via the UART
*
* This blocking function will transmit a single character via the
* UART.
*
* \param[in]  module  enumeration UART hw module
* \param[in]  tx_data  Data to transfer
*
* \return Status of the operation.
* \retval UART_STATUS_OK         If the operation was completed
*/
enum uart_status_code uart_write_wait(enum uart_hw_module module, const uint8_t tx_data);

/**
* \brief Receive a character via the UART
*
* This blocking function will receive a character via the UART.
*
* \param[in]   module  enumeration UART hw module
* \param[out]  rx_data  Pointer to received data
*
* \return Status of the operation.
* \retval UART_STATUS_OK                If the operation was completed
*/
enum uart_status_code uart_read_wait(enum uart_hw_module module, uint8_t *const rx_data);


/**
* \brief Transmit a buffer of characters via the UART
*
* This blocking function will transmit a block of \c length characters
* via the UART.
*
* \note Using this function in combination with the interrupt (\c _job) functions is
*       not recommended as it has no functionality to check if there is an
*       ongoing interrupt driven operation running or not.
*
* \param[in]  module  enumeration UART hw module
* \param[in]  tx_data  Pointer to data to transmit
* \param[in]  length   Number of characters to transmit
* 
* \return Status of the operation.
* \retval UART_STATUS_OK              If operation was completed
*/
enum uart_status_code uart_write_buffer_wait(enum uart_hw_module module, const uint8_t *tx_data, uint32_t length);

/**
 * \brief Receive a buffer of \c length characters via the UART
 *
 * This blocking function will receive a block of \c length characters
 * via the UART.
 *
 * \note Using this function in combination with the interrupt (\c *_job)
 *       functions is not recommended as it has no functionality to check if
 *       there is an ongoing interrupt driven operation running or not.
 *
 * \param[in]  module  enumeration UART hw module
 * \param[out] rx_data  Pointer to receive buffer
 * \param[in]  length   Number of characters to receive
 *
 * \return Status of the operation.
 * \retval UART_STATUS_OK                If operation was completed
 */
enum uart_status_code uart_read_buffer_wait(enum uart_hw_module module, uint8_t *rx_data, uint16_t length);


/**
 * \brief Printf function for logging using UART
 *
 * This blocking function will send a block of characters
 * via the UART.
 *
 * \note Using this function in combination with the interrupt (\c *_job)
 *       functions is not recommended as it has no functionality to check if
 *       there is an ongoing interrupt driven operation running or not.
 *
 * \param[in]  module  enumeration UART hw module
 * \param[out] format  Pointer to data to be printed
 * \...[in]  args   arguments for print message.
 *
 * \return None.
 */
 void uart_printf(enum uart_hw_module module, char* format, ...);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif
