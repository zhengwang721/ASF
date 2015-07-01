/**
 * \file
 *
 * \brief SAM Serial Peripheral Interface Driver
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

#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

#include <stdint.h>
#include <string.h>
#include <compiler.h>
#include <conf_spi.h>

#  if SPI_CALLBACK_MODE == true
//#  include <_interrupt.h>
#  endif

#ifdef __cplusplus
extern "C" {
#endif

#if (CONF_SPI_MASTER_ENABLE == false) && (CONF_SPI_SLAVE_ENABLE == false)
#error "Not possible compile SPI driver, invalid driver configuration. Make sure that either/both CONF_SPI_MASTER_ENABLE/CONF_SPI_SLAVE_ENABLE is set to true."
#endif

#  ifndef SPI_TIMEOUT
/** SPI timeout value */
#  define SPI_TIMEOUT 10000
#  endif

/** \brief SPI Core index
 *
 * Index to select spcific SPI module from available list.
 */
enum spi_core_idx {
	SPI_CORE1 = 1,
	SPI_CORE2,
	SPI_CORE_MAX,
};

/** Prototype for the device instance */
struct spi_module;

#  if SPI_CALLBACK_MODE == true

/** Type of the callback functions */
typedef void (*spi_callback_t)(const struct spi_module *const module);

/**
 * \brief SPI Callback enum
 *
 * Callbacks for SPI callback driver.
 *
 * \note For slave mode, these callbacks will be called when a transaction
 * is ended by the master pulling Slave Select high.
 *
 */
enum spi_callback {
	/** Callback for buffer transmitted */
	SPI_CALLBACK_BUFFER_TRANSMITTED,
	/** Callback for buffer received */
	SPI_CALLBACK_BUFFER_RECEIVED,
	/** Callback for buffers transceived */
	SPI_CALLBACK_BUFFER_TRANSCEIVED,
	/** Callback for error */
	SPI_CALLBACK_ERROR,
#  if !defined(__DOXYGEN__)
	/** Number of available callbacks. */
	SPI_CALLBACK_N,
#  endif
};

#  if !defined(__DOXYGEN__)
/** Prototype for the interrupt handler */
extern void _spi_interrupt_handler(uint8_t instance);

/**
 * \brief SPI transfer directions
 */
enum _spi_direction {
	/** Transfer direction is read. */
	SPI_DIRECTION_READ,
	/** Transfer direction is write. */
	SPI_DIRECTION_WRITE,
	/** Transfer direction is read and write. */
	SPI_DIRECTION_BOTH,
	/** No transfer. */
	SPI_DIRECTION_IDLE,
};
#  endif
#  endif

/**
 * \brief SPI transfer modes enum
 *
 * SPI transfer mode.
 */
enum spi_transfer_mode {
	/** Mode 0. Leading edge: rising, sample. Trailing edge: falling, setup */
	SPI_TRANSFER_MODE_0 = 0,
	/** Mode 1. Leading edge: rising, setup. Trailing edge: falling, sample */
	SPI_TRANSFER_MODE_1 = SPI_SPI_CONFIGURATION_SCK_PHASE_1,
	/** Mode 2. Leading edge: falling, sample. Trailing edge: rising, setup */
	SPI_TRANSFER_MODE_2 = SPI_SPI_CONFIGURATION_SCK_POLARITY_1,
	/** Mode 3. Leading edge: falling, setup. Trailing edge: rising, sample */
	SPI_TRANSFER_MODE_3 = SPI_SPI_CONFIGURATION_SCK_PHASE_1 | \
						SPI_SPI_CONFIGURATION_SCK_POLARITY_1,
};

/**
 * \brief SPI signal mux settings
 *
 * Set the functionality of the GPIO pins for SPI.
 * As not all settings can be used in different modes of operation, proper
 * settings must be chosen according to the rest of the configuration.
 *
 */
enum spi_signal_mux_setting {
	/** SPI MUX setting A */
	SPI_SIGNAL_MUX_SETTING_A = ((2 << 8) | (2 << 12) | (2 << 16) | (2 << 20)),
};

/**
 * \brief SPI modes enum
 *
 * SPI mode selection.
 */
enum spi_mode {
	/** Master mode */
	SPI_MODE_MASTER         = 1,
	/** Slave mode */
	SPI_MODE_SLAVE          = 0,
};

/**
 * \brief SPI data order enum
 *
 * SPI data order.
 *
 */
enum spi_data_order {
	/** The MSB of the data is transmitted first */
	SPI_DATA_ORDER_MSB      = 0,
	/** The LSB of the data is transmitted first */
	SPI_DATA_ORDER_LSB      = SPI_SPI_CONFIGURATION_LSB_FIRST_ENABLE_1,
};

/**
 * \brief SPI module clock input
 *
 * SPI module clock.
 *
 */
enum spi_clock_input {
	/** source from clock input 0 26MHz*/
	SPI_CLK_INPUT_0	= 0,
	/** source from clock input 1 13MHz */
	SPI_CLK_INPUT_1,
	/** source from clock input 2 6.5MHz*/
	SPI_CLK_INPUT_2,
	/** source from clock input 3 3MHz*/
	SPI_CLK_INPUT_3,
};

/**
 * \brief  SPI driver software device instance structure.
 *
 *  SPI driver software instance structure, used to retain software state
 * information of an associated hardware module instance.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct spi_module {
#  if !defined(__DOXYGEN__)
	/** Hardware module */
	void *hw;
	/** Module lock */
	volatile uint8_t locked;
	/** SPI mode */
	enum spi_mode mode;
	/** Transmit dummy data when receiving*/
	uint8_t tx_dummy_byte;
	/** Receiver enabled */
	uint8_t receiver_enabled;
#if SPI_CALLBACK_MODE == true
	/** Direction of transaction */
	volatile enum _spi_direction dir;
	/** Array to store callback function pointers in */
	spi_callback_t callback[SPI_CALLBACK_N];
	/** Buffer pointer to where the next received character will be put */
	volatile uint8_t *rx_buffer_ptr;
	/** Buffer pointer to where the next character will be transmitted from
	**/
	volatile uint8_t *tx_buffer_ptr;
	/** Remaining characters to receive */
	volatile uint16_t remaining_rx_buffer_length;
	/** Remaining dummy characters to send when reading */
	volatile uint16_t remaining_dummy_buffer_length;
	/** Remaining characters to transmit */
	volatile uint16_t remaining_tx_buffer_length;
	/** Bit mask for callbacks registered */
	uint8_t registered_callback;
	/** Bit mask for callbacks enabled */
	uint8_t enabled_callback;
	/** Holds the status of the ongoing or last operation */
	volatile enum status_code status;
#  endif
#  endif
};

/**
 * \brief SPI peripheral slave instance structure
 *
 * SPI peripheral slave software instance structure, used to configure the
 * correct SPI transfer mode settings for an attached slave. See
 * \ref spi_select_slave.
 */
struct spi_slave_inst {
	/** Pin to use as Slave Select */
	uint8_t ss_pin;
	/** Address recognition enabled in slave device */
	uint8_t address_enabled;
	/** Address of slave device */
	uint8_t address;
};

/**
 * \brief SPI peripheral slave configuration structure
 *
 * SPI Peripheral slave configuration structure
 */
struct spi_slave_inst_config {
	/** Pin to use as Slave Select */
	uint8_t ss_pin;
	/** Enable address */
	bool address_enabled;
	/** Address of slave */
	uint8_t address;
};

/**
 * \brief SPI Master configuration structure
 *
 * SPI Master configuration structure
 */
struct spi_master_config {
	/** Baud rate */
	uint32_t baudrate;
};

/**
 * \brief SPI slave configuration structure
 *
 * SPI slave configuration structure
 */
struct spi_slave_config {
	/** Address */
	uint8_t address;
	/** Address mask */
	uint8_t address_mask;
	/** Preload data to the shift register while SS is high */
	bool preload_enable;
};

/**
 * \brief SPI configuration structure
 *
 * Configuration structure for an SPI instance. This structure should be
 * initialized by the \ref spi_get_config_defaults function before being
 * modified by the user application.
 */
struct spi_config {
	/** Core Index */
	enum spi_core_idx core_idx;
	/** SPI mode */
	enum spi_mode mode;
	/** Data order */
	enum spi_data_order data_order;
	/** Transfer mode */
	enum spi_transfer_mode transfer_mode;
	/** Mux setting */
	enum spi_signal_mux_setting mux_setting;
	/** Enabled in sleep modes */
	//uint8_t run_in_standby;
	/** Enable receiver */
	bool receiver_enable;
	/** Union for slave or master specific configuration */
	union {
		/** Slave specific configuration */
		struct spi_slave_config slave;
		/** Master specific configuration */
		struct spi_master_config master;
	} mode_specific;
	/** clock source to use */
	enum spi_clock_input clock_source;
	/** clock divider value to use*/
	uint8_t clock_divider;
	/** SPI PAD pinmux */
	uint16_t pinmux_pad[4];
};

/**
@defgroup spi-drv SPI Driver API

@{
*/


/**
 * \name Initialization functions
 * @{
 */

enum status_code spi_init(
		struct spi_module *const module,
		const struct spi_config *const config);

void spi_reset(struct spi_module *const module);

void spi_slave_inst_get_config_defaults(
		struct spi_slave_inst_config *const config);

void spi_get_config_defaults(struct spi_config *const config);

void spi_attach_slave(
		struct spi_slave_inst *const slave,
		struct spi_slave_inst_config *const config);

/** @} */

/**
 * \name Enable/Disable
 * @{
 */

void spi_enable(struct spi_module *const module);
void spi_disable(struct spi_module *const module);

/** @} */

/**
 * \name Lock/Unlock
 * @{
 */

enum status_code spi_lock(struct spi_module *const module);
void spi_unlock(struct spi_module *const module);

/** @} */


/**
 * \name Read/Write
 * @{
 */

enum status_code spi_write(
		struct spi_module *module,
		uint8_t tx_data);

enum status_code spi_write_buffer_wait(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint16_t length);

enum status_code spi_read(
		struct spi_module *const module,
		uint8_t *rx_data);

enum status_code spi_read_buffer_wait(
		struct spi_module *const module,
		uint8_t *rx_data,
		uint16_t length,
		uint8_t dummy);

enum status_code spi_transceive_wait(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint8_t *rx_data);

enum status_code spi_transceive_buffer_wait(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint8_t *rx_data,
		uint16_t length);

enum status_code spi_select_slave(
		struct spi_module *const module,
		struct spi_slave_inst *const slave,
		bool select);

/** @} */

/** @}*/

#ifdef __cplusplus
}
#endif

#endif	//SPI_H_INCLUDED

