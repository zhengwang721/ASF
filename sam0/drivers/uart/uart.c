/**
* \file
*
* \brief UART Driver
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

#include "uart.h"

/* The calibration should be removed lately. */
#define REG_READ_32(addr)					(*(volatile unsigned int*)(addr))
#define REG_WRITE_32(addr,val)				(*(volatile unsigned int*)(addr) = val)
static uint32_t calibrate_clk(void)
{
	uint32_t clock;
	uint32_t u32calib;
	uint32_t u32orig_calib;

	REG_WRITE_32(0x4000c000, 0x0);  // GYR
	REG_WRITE_32(0x4000B018, REG_READ_32(0x4000B018) | (1<<2)); // must have two steps since there is a sequence
	REG_WRITE_32(0x4000B018, REG_READ_32(0x4000B018) | (1<<3));


	//REG_WRITE_32(0x4000f404, REG_READ_32(0x4000f404) | (1<<8));
	REG_WRITE_32(0x4000c000, REG_READ_32(0x4000c000) | (1<<7));


	while((REG_READ_32(0x4000c000) & (1<<7))!=0);
	u32orig_calib = REG_READ_32(0x4000c010) & ~(1<<15);

	REG_WRITE_32(0x4000B018, REG_READ_32(0x4000B018) & (~(1<<3))); // must have two steps since there is a sequence
	REG_WRITE_32(0x4000B018, REG_READ_32(0x4000B018) & ~((1<<2)));

	//REG_WRITE_32(0x4000f404, REG_READ_32(0x4000f404) & ~(1<<8));


	/*UART Calibration*/
	REG_WRITE_32(0x4000c000, REG_READ_32(0x4000c000) | (1<<7));

	while((REG_READ_32(0x4000c000) & (1<<7))!=0);

	//REG_WRITE_32(0x4000f044, (REG_READ_32(0x4000c010)));

	REG_WRITE_32(0x4000c010, (REG_READ_32(0x4000c010) & ~(1<<15)));

	//	while((REG_READ_32(0x4000c010) & (1<<15))!=1);
	REG_WRITE_32(0x4000f404, REG_READ_32(0x4000f404) & ~(1<<8));		

	u32calib = REG_READ_32(0x4000c010) & ~(1<<15);
	clock = ((26000000/u32orig_calib)*u32calib);
	return clock;
}

static void uart_set_baudrate(struct uart_module *const module,
		const uint32_t baud_rate)
{
	uint32_t clock;
	uint16_t integerpart = 0;
	uint8_t fractionalpart = 0;
	uint32_t diff;
	uint8_t i = 0;

	clock = calibrate_clk();
	integerpart = clock / baud_rate;
	diff = clock - (baud_rate * integerpart);
	i = 0;
	while(diff > (baud_rate / 16)) {
		i++;
		diff -= (baud_rate / 16);
	}
	fractionalpart = (i + 1) / 2;

	module->hw->UART_CLOCK_SOURCE = 0;
	module->hw->UART_BAUD_RATE = ((uint32_t)integerpart << 3) | ((uint32_t)fractionalpart << 0);
}

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
		struct uart_config *const config)
{
	config->baud_rate = 115200;
	config->data_bits = UART_8_BITS;
	config->stop_bits = UART_1_STOP_BIT;
	config->parity = UART_NO_PARITY;
	config->flow_control = false;
}

/**
 * \brief Initializes the device
 *
 * Initializes the UART device based on the setting specified in the
 * configuration struct.
 *
 * \param[in]  module  enumeration UART hw module
 * \param[in]  hw      Pointer to USART hardware instance
 * \param[in]  config  Pointer to configuration struct
 *
 * \return Status of the initialization.
 *
 * \retval UART_STATUS_OK                       The initialization was successful
 * \retval UART_STATUS_ERR_GPIO_NOT_AVAILABLE   Requested GPIO for UART-RXD and UART-TXD is not available.
 */
enum status_code uart_init(struct uart_module *const module, Uart * const hw,
		const struct uart_config *const config)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(hw);
	Assert(config);

	uint8_t config_temp = 0;
	enum status_code status_code = STATUS_OK;

	/* Assign module pointer to software instance struct */
	module->hw = hw;

	/* Pin Mux programming */
	if(hw == UART0) {
		gpio_pinmux_cofiguration(PIN_LP_GPIO_2_MUX2_UART0_RXD, MUX_LP_GPIO_2_MUX2_UART0_RXD);
	} else if(hw == UART1) {
		gpio_pinmux_cofiguration(PIN_LP_GPIO_6_MUX2_UART1_RXD, MUX_LP_GPIO_6_MUX2_UART1_RXD);
	} else {
		return STATUS_ERR_INVALID_ARG;
	}


	/* empty UART FIFO */
	while (module->hw->RECEIVE_STATUS & UART_RECEIVE_STATUS_RX_FIFO_NOT_EMPTY) {
		i = module->hw->RECEIVE_DATA;
	}
	
	/* reset configuration register */
	module->hw->UART_CONFIGURATION = 0;

	/* program the uart configuration. */
	if(config->flow_control) {
		config_temp |= UART_UART_CONFIGURATION_CTS_ENABLE_1;
	}
	config_temp |= config->data_bits;
	config_temp |= config->stop_bits;
	if(config->parity != UART_NO_PARITY) {
		config_temp |= UART_UART_CONFIGURATION_PARITY_ENABLE_1;
		if(config->parity == UART_ODD_PARITY) {
			config_temp |= UART_UART_CONFIGURATION_PARITY_MODE_1;				
		} else if(config->parity == UART_SPACE_PARITY) {
			config_temp |= UART_UART_CONFIGURATION_PARITY_MODE_2;
		} else if(config->parity == UART_MARK_PARITY) {
			config_temp |= UART_UART_CONFIGURATION_PARITY_MODE_3;
		}
	}	
	module->hw->UART_CONFIGURATION = config_temp;

	/* Calculate the baud rate. */
	uart_set_baudrate(module, config->baud_rate);

	module->hw->RX_INTERRUPT_MASK = 0;	// disable int at initialization, enable it at read time
	module->hw->TX_INTERRUPT_MASK = 0;	// disable int at initialization, enable it at write time
	
	return STATUS_OK;
}

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
enum status_code uart_write_wait(struct uart_module *const module,
		const uint8_t tx_data)
{
	while (!(module->hw->TRANSMIT_STATUS & UART_TRANSMIT_STATUS_TX_FIFO_NOT_FULL));

	module->hw->TRANSMIT_DATA = tx_data;
	
	return STATUS_OK;
}

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
enum status_code uart_read_wait(struct uart_module *const module,
		uint8_t *const rx_data)
{
	while (!(module->hw->RECEIVE_STATUS & UART_RECEIVE_STATUS_RX_FIFO_NOT_EMPTY));

	*rx_data = module->hw->RECEIVE_DATA;
	
	return STATUS_OK;
}

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
enum status_code uart_write_buffer_wait(struct uart_module *const module, 
		const uint8_t *tx_data, uint32_t length)
{
	while(length--)
		uart_write_wait(module, *tx_data++);
	
	return STATUS_OK;
}

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
enum status_code uart_read_buffer_wait(struct uart_module *const module,
		uint8_t *rx_data, uint16_t length)
{
	while(length--)
		uart_read_wait(module, rx_data++);
	
	return STATUS_OK;
}

