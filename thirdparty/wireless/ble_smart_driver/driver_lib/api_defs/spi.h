/**
 ****************************************************************************************
 *
 * @file spi.h
 *
 * @brief SAMB11 SPI API for Applications
 *
 * This module contains the public API and the necessary enumerations and structures that are required for 
 * SAMB11 Application Developers using SPI module
 *
 *
 *  Copyright (c) 2014 Atmel Corporation. All rights reserved.
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
 
#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#  if SPI_CALLBACK_MODE == true
//#  include <_interrupt.h>
#  endif

#ifdef __cplusplus
extern "C" {
#endif

#  ifndef SPI_TIMEOUT
/** SPI timeout value */
#  define SPI_TIMEOUT 10000
#  endif

#  if SPI_CALLBACK_MODE == true
/** Prototype for the device instance */
struct spi_module;

/** Type of the callback functions */
typedef void (*spi_callback_t)(const struct spi_module *const module);

#  if !defined(__DOXYGEN__)
/** Prototype for the interrupt handler */
extern void _spi_interrupt_handler(uint8_t instance);
#  endif
#  endif

/**
 * \brief SPI Driver Status codes.
 *
 * Status codes of SPI drivers.
 */
enum spi_status_code {
	SPI_STATUS_OK = 0,
	SPI_STATUS_BUSY,
	SPI_STATUS_ERR_IO,
	SPI_STATUS_ERR_TIMEOUT,
	SPI_STATUS_ERR_OVERFLOW,
	SPI_STATUS_ERR_PROTOCOL,
	SPI_STATUS_ERR_NOT_INITIALIZED,
	SPI_STATUS_ERR_DENIED,
	SPI_STATUS_ERR_INVALID_ARG,
	SPI_STATUS_ERR_UNSUPPORTED_DEV,
	SPI_STATUS_ERR_GPIO_NOT_AVAILABLE,
};


/** \brief SPI Core index
 *
 * Index to select spcific SPI module from available list.
 */
enum spi_core_idx {
	SPI_CORE1	= 1,
	SPI_CORE2,
	SPI_CORE_MAX,
};


#  if SPI_CALLBACK_MODE == true
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
#  endif

#if SPI_CALLBACK_MODE == true
#  if !defined(__DOXYGEN__)

/**
 * \brief SPI transfer directions
 */
enum _spi_direction {
	SPI_DIRECTION_READ,
	SPI_DIRECTION_WRITE,
	SPI_DIRECTION_BOTH,
	SPI_DIRECTION_IDLE,
};
#  endif
#endif

/**
 * \brief SPI transfer modes enum
 *
 * SPI transfer mode.
 */
enum spi_transfer_mode {
	/** Mode 0. Leading edge: rising, sample. Trailing edge: falling, setup */
	SPI_TRANSFER_MODE_0 = 0,
	/** Mode 1. Leading edge: rising, setup. Trailing edge: falling, sample */
	SPI_TRANSFER_MODE_1,
	/** Mode 2. Leading edge: falling, sample. Trailing edge: rising, setup */
	SPI_TRANSFER_MODE_2,
	/** Mode 3. Leading edge: falling, setup. Trailing edge: rising, sample */
	SPI_TRANSFER_MODE_3,
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
	SPI_DATA_ORDER_LSB,
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
	/**  hardware module */
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
	volatile enum spi_status_code status;
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
	uint8_t address_enabled;
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
	uint8_t preload_enable;
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
	uint8_t receiver_enable;
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

/**
 * \brief Initializes the  SPI module
 *
 * This function will initialize the  SPI module, based on the values
 * of the config struct.
 *
 * \param[out]  module  Pointer to the software instance struct
 * \param[in]   config  Pointer to the config struct
 *
 * \return Status of the initialization
 * \retval SPI_STATUS_OK              				 Module initiated correctly.
 * \retval SPI_STATUS_ERR_GPIO_NOT_AVAILABLE   If requested GPIO for SPI is not available.
 * \retval SPI_STATUS_ERR_INVALID_ARG  				 If invalid argument(s) were provided.
 */
enum spi_status_code spi_init(
		struct spi_module *const module,
		const struct spi_config *const config);

/**
 * \brief Resets the SPI module
 *
 * This function will reset the SPI module to its power on default values and
 * disable it.
 *
 * \param[in,out] module Pointer to the software instance struct
 */

void spi_reset(
		struct spi_module *const module);


/**
 * \brief Initializes an SPI peripheral slave device configuration structure to default values
 *
 * This function will initialize a given SPI slave device configuration
 * structure to a set of known default values. This function should be called
 * on any new instance of the configuration structures before being modified by
 * the user application.
 *
 * The default configuration is as follows:
 *  \li Slave Select on GPIO pin 12
 *  \li Addressing not enabled
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
void spi_slave_inst_get_config_defaults(
		struct spi_slave_inst_config *const config);

/**
 * \brief Initializes an SPI configuration structure to default values
 *
 * This function will initialize a given SPI configuration structure to a set
 * of known default values. This function should be called on any new
 * instance of the configuration structures before being modified by the
 * user application.
 *
 * The default configuration is as follows:
 *  \li Master mode enabled
 *  \li MSB of the data is transmitted first
 *  \li Transfer mode 0
 *  \li MUX Setting 0
 *  \li Character size 8 bit
 *  \li Not enabled in sleep mode
 *  \li Receiver enabled
 *  \li Baudrate 2000000
 *  \li Default pinmux settings for all pads
 *  \li Clock source 0 (26MHz)
 *	\li	Clock divider 2 (Formula: baud_rate = ((clock input freq/clock_divider+1)/4))
 *											(For Example: if clock source is CLOCK_INPUT_0 then
 *												((26000000/(2+1))/4) ~= 2000000 bps)
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
void spi_get_config_defaults(
		struct spi_config *const config);

/**
 * \brief Attaches an SPI peripheral slave
 *
 * This function will initialize the software SPI peripheral slave, based on
 * the values of the config struct. The slave can then be selected and
 * optionally addressed by the \ref spi_select_slave function.
 *
 * \param[out] slave   Pointer to the software slave instance struct
 * \param[in]  config  Pointer to the config struct
 *
 * \retval SPI_STATUS_OK 					if the requested SS pin is available and configured.
 * \retval SPI_STATUS_ERR_DENIED 	if the requested SS pin is not available.
 *
 */
enum spi_status_code spi_attach_slave(
		struct spi_slave_inst *const slave,
		struct spi_slave_inst_config *const config);

/** @} */
		
/**
 * \name Enable/Disable
 * @{
 */

/**
 * \brief Enables the SPI module
 *
 * This function will enable the SPI module.
 *
 * \param[in,out] module  Pointer to the software instance struct
 */
void spi_enable(
		struct spi_module *const module);



/**
 * \brief Disables the  SPI module
 *
 * This function will disable the  SPI module.
 *
 * \param[in,out] module  Pointer to the software instance struct
 */
void spi_disable(
		struct spi_module *const module);

/** @} */

/**
 * \name Lock/Unlock
 * @{
 */

/**
 * \brief Attempt to get lock on driver instance
 *
 * This function checks the instance's lock, which indicates whether or not it
 * is currently in use, and sets the lock if it was not already set.
 *
 * The purpose of this is to enable exclusive access to driver instances, so
 * that, e.g., transactions by different services will not interfere with each
 * other.
 *
 * \param[in,out] module Pointer to the driver instance to lock.
 *
 * \retval SPI_STATUS_OK if the module was locked.
 * \retval SPI_STATUS_BUSY if the module was already locked.
 */
enum spi_status_code spi_lock(struct spi_module *const module);

/**
 * \brief Unlock driver instance
 *
 * This function clears the instance lock, indicating that it is available for
 * use.
 *
 * \param[in,out] module Pointer to the driver instance to lock.
 *
 */
void spi_unlock(struct spi_module *const module);

/** @} */


/**
 * \name Read/Write
 * @{
 */

/**
 * \brief Transfers a single SPI character
 *
 * This function will send a single SPI character via SPI and ignore any data
 * shifted in by the connected device. To both send and receive data, use the
 * \ref spi_transceive_wait function or use the \ref spi_read function after
 * writing a character. 
 *
 * Note that this function does not handle the SS (Slave Select)
 * pin(s) in master mode; this must be handled from the user application.
 *
 * \note In slave mode, the data will not be transferred before a master
 *       initiates a transaction.
 *
 * \param[in] module   Pointer to the software instance struct
 * \param[in] tx_data  Data to transmit
 *
 * \return Status of the procedure
 * \retval SPI_STATUS_OK    If the data was written
 * \retval SPI_STATUS_BUSY  If the last write was not completed
 */
enum spi_status_code spi_write(
		struct spi_module *module,
		uint8_t tx_data);
		
/**
 * \brief Writes requested amount of data to the SPI.
 *
 * This function will return after writing requested amount of data
 *
 * \note The \ref spi_select_slave function should be called before calling
 *       this function.
 *
 * \param[in] module    Pointer to the software instance struct
 * \param[out] tx_data  Pointer to buffer to be transmitted
 * \param[in] length		length of data to be read
 *
 * \returns Status of the write operation.
 * \retval SPI_STATUS_OK            				If data was written successfully
 * \retval SPI_STATUS_ERR_NOT_INITIALIZED   Module data structure is not initialized.
 * \retval SPI_STATUS_ERR_INVALID_ARG  			If the requested length is equal zero.
 */ 
enum spi_status_code spi_write_buffer_wait(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint16_t length);

/**
 * \brief Reads last received SPI character
 *
 * This function will return the last SPI character shifted into the receive
 * register by the \ref spi_write function
 *
 *
 * \note Receiver must be enabled in the configuration
 *
 * \param[in] module    Pointer to the software instance struct
 * \param[out] rx_data  Pointer to store the received data
 *
 * \returns Status of the read operation.
 * \retval SPI_STATUS_OK            If data was read
 * \retval SPI_STATUS_ERR_IO        If no data is available
 * \retval SPI_STATUS_ERR_OVERFLOW  If the data is overflown
 */
enum spi_status_code spi_read(
		struct spi_module *const module,
		uint8_t *rx_data);

/**
 * \brief Reads requested amount of data from the SPI.
 *
 * This function will return after reading requested amount of data
 *
 * \note Receiver must be enabled in the configuration
 * \note The \ref spi_select_slave function should be called before calling
 *       this function.
 *
 * \param[in] module    Pointer to the software instance struct
 * \param[out] rx_data  Pointer to store the received data
 * \param[in] length		length of data to be read.
 * \param[in] dummy			dummy byte to be sent on bus when reading data.
 *
 * \returns Status of the read operation.
 * \retval SPI_STATUS_OK            				If data was read successfully
 * \retval SPI_STATUS_ERR_NOT_INITIALIZED   Module data structure is not initialized.
 * \retval SPI_STATUS_ERR_INVALID_ARG  			If the requested length is equal zero.
 */ 
enum spi_status_code spi_read_buffer_wait(
		struct spi_module *const module,
		uint8_t *rx_data,
		uint16_t length,
		uint8_t dummy);

/**
 * \brief Transceive single byte of data to and from the SPI.
 *
 * This function will return after single byte of data transceived.
 *
 * \note Receiver must be enabled in the configuration
 * \note The \ref spi_select_slave function should be called before calling
 *       this function.
 *
 * \param[in] module    Pointer to the software instance struct
 * \param[in] tx_data   Pointer containing the data to be transmitted.
 * \param[out] rx_data	Pointer to store the received data
 *
 * \returns Status of the read operation.
 * \retval SPI_STATUS_OK            				If data was transmitted and received successfully
 * \retval SPI_STATUS_ERR_NOT_INITIALIZED   Module data structure is not initialized.
 * \retval SPI_STATUS_ERR_INVALID_ARG  			If the requested length is equal zero.
 */ 
enum spi_status_code spi_transceive_wait(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint8_t *rx_data);

/**
 * \brief Transceive requested amount of data to and from the SPI.
 *
 * This function will return after sending and receiving requested amount of data
 *
 * \note Receiver must be enabled in the configuration
 * \note The \ref spi_select_slave function should be called before calling
 *       this function.
 *
 * \param[in] module    Pointer to the software instance struct
 * \param[in] tx_data   Pointer containing the data to be transmitted.
 * \param[out] rx_data	Pointer to store the received data
 * \param[in] length		length of data to be read.
 *
 * \returns Status of the read operation.
 * \retval SPI_STATUS_OK            				If data was transmitted and received successfully
 * \retval SPI_STATUS_ERR_NOT_INITIALIZED   Module data structure is not initialized.
 * \retval SPI_STATUS_ERR_INVALID_ARG  			If the requested length is equal zero.
 */ 
enum spi_status_code spi_transceive_buffer_wait(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint8_t *rx_data,
		uint16_t length);

/**
 * \brief Asserting/Deasserting the slave select for the corresponding slave.
 *
 * This function will assert or deassert the SS of the requested slave device.
 *
 * \param[in] module    Pointer to the software instance struct
 * \param[in] slave   	Pointer containing slave instance.
 * \param[in] select		bool to select the salve or deselect.
 *
 * \returns Status of the slave select operation.
 *		
 * \retval SPI_STATUS_OK            	If SS pin is a valid one and selected/deselected.
 * \retval SPI_STATUS_ERR_INVALID_ARG Invalid SS pin.
 */ 
enum spi_status_code spi_select_slave(
		struct spi_module *const module,
		struct spi_slave_inst *const slave,
		bool select);

/** @} */

/** @}*/		
		
#ifdef __cplusplus
}
#endif

#endif	//SPI_H_INCLUDED

