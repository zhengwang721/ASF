/**
 * \file
 *
 * \brief Serial Peripheral Interface Driver for SAMB11
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
#include "spi.h"

/**
 * \brief Determines if the SPI module is currently synchronizing to the bus.
 *
 * This function will check if the underlying hardware peripheral module is
 * currently synchronizing across multiple clock domains to the hardware bus.
 * This function can be used to delay further operations on the module until it
 * is ready.
 *
 * \param[in]  module  SPI hardware module
 *
 * \return Synchronization status of the underlying hardware module
 * \retval true   Module synchronization is ongoing
 * \retval false  Module synchronization is not ongoing
 *
 */
static inline bool _spi_is_active(
		Spi *const spi_module)
{
	return spi_module->SPI_BUS_STATUS.bit.SPI_ACTIVE;
}

/**
 * \internal Writes an SPI configuration to the hardware module.
 *
 * This function will write out a given configuration to the hardware module.
 * Can only be done when the module is disabled.
 *
 * \param[in]  module  Pointer to the software instance struct
 * \param[in]  config  Pointer to the configuration struct
 *
 * \return The status of the configuration
 * \retval STATUS_ERR_INVALID_ARG  If invalid argument(s) were provided.
 * \retval STATUS_OK               If the configuration was written
 */
static enum status_code _spi_set_config(
		struct spi_module *const module,
		const struct spi_config *const config)
{
	SPI_SPI_CONFIGURATION_Type spi_cfg;
	Spi *const spi_module = (module->hw);

	spi_cfg.reg = 0;
	module->mode  = config->mode;
	module->receiver_enabled = config->receiver_enable;

#if CONF_SPI_MASTER_ENABLE == true
	/* Find baud value and write it */
	if (config->mode == SPI_MODE_MASTER) {
		// This divider value needs to be based on baud rate. For now we will use the 1 for testing.
		spi_module->SPI_CLK_DIVIDER.reg = config->clock_divider;
	}
#endif
	/* Set data order */
	spi_cfg.bit.LSB_FIRST_ENABLE = config->data_order;
	/* Set clock polarity and clock phase */
	switch(config->transfer_mode)
	{
		case SPI_TRANSFER_MODE_0:
			spi_cfg.bit.SCK_POLARITY = 0;
			spi_cfg.bit.SCK_PHASE = 0;
			break;
		case SPI_TRANSFER_MODE_1:
			spi_cfg.bit.SCK_POLARITY = 0;
			spi_cfg.bit.SCK_PHASE = 1;
			break;
		case SPI_TRANSFER_MODE_2:
			spi_cfg.bit.SCK_POLARITY = 1;
			spi_cfg.bit.SCK_PHASE = 0;
			break;
		case SPI_TRANSFER_MODE_3:
			spi_cfg.bit.SCK_POLARITY = 1;
			spi_cfg.bit.SCK_PHASE = 1;
			break;
		default:
			break;
	}
	spi_module->SPI_CONFIGURATION.reg = spi_cfg.reg;
#if 0
	//clear all spi RX status bits before enabling it.
	while(spi_module->RECEIVE_STATUS.reg != 0) {
		dummy = spi_module->RECEIVE_DATA.reg;
	}

	// Write a dummy byte
	spi_module->TRANSMIT_DATA.reg = 0x0;
#endif
	//Enable SPI
	spi_module->SPI_MODULE_ENABLE.reg = SPI_SPI_MODULE_ENABLE_ENABLE;
	return STATUS_OK;
}

/**
 * \brief Checks if the SPI in master mode has shifted out last data, or if the
 * master has ended the transfer in slave mode.
 *
 * This function will check if the SPI master module has shifted out last data,
 * or if the slave select pin has been drawn high by the master for the SPI
 * slave module.
 *
 * \param[in]  module  Pointer to the software instance struct
 *
 * \return Indication of whether any writes are ongoing
 * \retval true   If the SPI master module has shifted out data, or slave select
 *                has been drawn high for SPI slave
 * \retval false  If the SPI master module has not shifted out data
 */
static inline bool _spi_is_write_complete(
		Spi *const spi_module)
{
	/* Check interrupt flag */
	return (spi_module->TRANSMIT_STATUS.bit.TX_FIFO_EMPTY);
}


 /**
 * \brief Checks if the SPI module is ready to write data
 *
 * This function will check if the SPI module is ready to write data.
 *
 * \param[in]  module  Pointer to the software instance struct
 *
 * \return Indication of whether the module is ready to read data or not
 * \retval true   If the SPI module is ready to write data
 * \retval false  If the SPI module is not ready to write data
 */
static inline bool _spi_is_ready_to_write(
		Spi *const spi_module)
{
	/* Check interrupt flag */
	return (spi_module->TRANSMIT_STATUS.bit.TX_FIFO_NOT_FULL);
}

/**
 * \brief Checks if the SPI module is ready to read data
 *
 * This function will check if the SPI module is ready to read data.
 *
 * \param[in]  module Pointer to the software instance struct
 *
 * \return Indication of whether the module is ready to read data or not
 * \retval true   If the SPI module is ready to read data
 * \retval false  If the SPI module is not ready to read data
 */
static inline bool _spi_is_ready_to_read(
		Spi *const spi_module)
{
	/* Check interrupt flag */
	return (spi_module->RECEIVE_STATUS.bit.RX_FIFO_NOT_EMPTY);
}

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
		struct spi_slave_inst_config *const config)
{
	Assert(config);

	config->ss_pin          = PIN_LP_GPIO_12;
	config->address_enabled = false;
	config->address         = 0;
}

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
 *  \li Baudrate 1300000
 *  \li Default pinmux settings for all pads
 *  \li Clock source 0 (26MHz)
 *  \li	Clock divider 4 (Formula: baud_rate = ((clock input freq/clock_divider+1)/4))
 *                                  (For Example: if clock source is CLOCK_INPUT_0 then
 *                                  ((26000000/(4+1))/4) = 1300000 bps)
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
void spi_get_config_defaults(
		struct spi_config *const config)
{
	Spi *spi_module = (Spi *)(SPI0);
	/* Default configuration values */
	if((spi_module->SPI_MODULE_ENABLE.reg == 0) && (spi_module->SPI_CLK_DIVIDER.reg == 0)) {
		config->core_idx = SPI_CORE1;
	} else {
		spi_module = (Spi *)(SPI1);
		if((spi_module->SPI_MODULE_ENABLE.reg == 0) && (spi_module->SPI_CLK_DIVIDER.reg == 0)) {
			config->core_idx = SPI_CORE2;
		} else {
			config->core_idx = SPI_CORE_MAX;
			return;
		}
	}
	config->mode             = SPI_MODE_MASTER;
	config->data_order       = SPI_DATA_ORDER_MSB;
	config->transfer_mode    = SPI_TRANSFER_MODE_0;
	//config->run_in_standby   = 0;
	config->receiver_enable  = true;
	config->clock_source     = SPI_CLK_INPUT_0;
	config->clock_divider    = 4;

	/* Clear mode specific config */
	memset(&(config->mode_specific), 0, sizeof(config->mode_specific));

	/* Master config defaults */
	config->mode_specific.master.baudrate = ((260000000/config->clock_divider)/4);

	/* pinmux config defaults  Upper 16 bits inform the GPIO number and lower 16 bits for pinmux selection*/
	if(config->core_idx == SPI_CORE1){
		config->pinmux_pad[0] = PINMUX_LP_GPIO_10_MUX2_SPI0_SCK;
		config->pinmux_pad[1] = PINMUX_LP_GPIO_11_MUX2_SPI0_MOSI;
		config->pinmux_pad[2] = PINMUX_LP_GPIO_12_MUX2_SPI0_SSN;
		config->pinmux_pad[3] = PINMUX_LP_GPIO_13_MUX2_SPI0_MISO;
	}
	else {
		config->pinmux_pad[0] = PINMUX_LP_GPIO_2_MUX4_SPI0_SCK;
		config->pinmux_pad[1] = PINMUX_LP_GPIO_3_MUX4_SPI0_MOSI;
		config->pinmux_pad[2] = PINMUX_LP_GPIO_4_MUX4_SPI0_SSN;
		config->pinmux_pad[3] = PINMUX_LP_GPIO_5_MUX4_SPI0_MISO;
	}
};

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
 */
void spi_attach_slave(
		struct spi_slave_inst *const slave,
		struct spi_slave_inst_config *const config)
{
	Assert(slave);
	Assert(config);

	slave->ss_pin          = config->ss_pin;
	slave->address_enabled = config->address_enabled;
	slave->address         = config->address;

	gpio_pin_set_output_level(slave->ss_pin, true);
}

/**
 * \brief Resets the SPI module
 *
 * This function will reset the SPI module to its power on default values and
 * disable it.
 *
 * \param[in,out] module Pointer to the software instance struct
 */
void spi_reset(struct spi_module *const module)
{
	/* Sanity check arguments */
	Spi *const spi_module = (module->hw);

	/* Disable the module */
	spi_disable(module);

	/* Software reset the module */
	/* Assert Reset to SPI0 cores. */
	if(spi_module == (void *)SPI0) {
		LPMCU_MISC_REGS0->LPMCU_GLOBAL_RESET_0.reg &= \
								~(LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_CORTUS_SPI0_CORE_RSTN | \
								LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_CORTUS_SPI0_IF_RSTN);
	}
	/* Assert Reset to SPI1 cores. */
	if(spi_module == (void *)SPI1) {
		LPMCU_MISC_REGS0->LPMCU_GLOBAL_RESET_0.reg &= \
								~(LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_CORTUS_SPI1_CORE_RSTN | \
								LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_CORTUS_SPI1_IF_RSTN);
	}
	/* Clear Reset to SPI0 cores. */
	if(spi_module == (void *)SPI0) {
		LPMCU_MISC_REGS0->LPMCU_GLOBAL_RESET_0.reg |= \
								(LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_CORTUS_SPI0_CORE_RSTN | \
								LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_CORTUS_SPI0_IF_RSTN);
	}
	/* Clear Reset to SPI1 cores. */
	if(spi_module == (void *)SPI1) {
		LPMCU_MISC_REGS0->LPMCU_GLOBAL_RESET_0.reg |= \
								(LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_CORTUS_SPI1_CORE_RSTN | \
								LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_CORTUS_SPI1_IF_RSTN);
	}
}

/**
 * \brief Initializes the SPI module
 *
 * This function will initialize the SPI module, based on the values
 * of the config struct.
 *
 * \param[out]  module  Pointer to the software instance struct
 * \param[in]   hw      Pointer to hardware instance
 * \param[in]   config  Pointer to the config struct
 *
 * \return Status of the initialization
 * \retval STATUS_OK               Module initiated correctly.
 * \retval STATUS_ERR_DENIED       If module is enabled.
 * \retval STATUS_BUSY             If module is busy resetting.
 * \retval STATUS_ERR_INVALID_ARG  If invalid argument(s) were provided.
 */
enum status_code spi_init(
		struct spi_module *const module,
		const struct spi_config *const config)
{

	uint8_t idx;
	uint8_t pinnum = 0;

	/* Initialize device instance */
	if(config->core_idx == SPI_CORE1) {
		module->hw = (void *)SPI0;
	} else if(config->core_idx == SPI_CORE2) {
		module->hw = (void *)SPI1;
	} else {
		return STATUS_ERR_INVALID_ARG;
	}

	Spi *const spi_module = (module->hw);

	/* Check if module is enabled. */
	if (spi_module->SPI_MODULE_ENABLE.reg & SPI_SPI_MODULE_ENABLE_MASK) {
		spi_module->SPI_MODULE_ENABLE.reg = (0x0ul << SPI_SPI_MODULE_ENABLE_ENABLE_Pos);
	}

	spi_reset(module);

	//Program the pinmux.

	/* Set the pinmux for this spi module. */
	for(idx = 0; idx < 4; idx++) {
		gpio_pinmux_cofiguration(config->pinmux_pad[idx]>>16, \
								(uint8_t)(config->pinmux_pad[idx] & 0xFF));
	}

	/* Setting the default value for SS- PIN. */
	pinnum = (config->pinmux_pad[2] >> 16) & 0xFF;
	gpio_pin_set_output_level(pinnum, true);

	/* Set up the input clock & divider for the module */
	spi_module->CLOCK_SOURCE_SELECT.reg = config->clock_source;
	spi_module->SPI_CLK_DIVIDER.reg = config->clock_divider;

#  if CONF_SPI_MASTER_ENABLE == true
	if (config->mode == SPI_MODE_MASTER) {
		/* Set the mode in SPI master mode */
		spi_module->SPI_MASTER_MODE.reg = SPI_MODE_MASTER;
	}
#  endif

#  if CONF_SPI_SLAVE_ENABLE == true
	if (config->mode == SPI_MODE_SLAVE) {
		/* Set the mode in SPI slave mode */
		spi_module->SPI_MASTER_MODE.reg = SPI_MODE_SLAVE;
	}
#  endif

#if SPI_CALLBACK_MODE == true
	/* Temporary variables */
	uint8_t i;
	uint8_t instance_index;

	/* Initialize parameters */
	for (i = 0; i < SPI_CALLBACK_N; i++) {
		module->callback[i]        = NULL;
	}
	module->tx_buffer_ptr              = NULL;
	module->rx_buffer_ptr              = NULL;
	module->remaining_tx_buffer_length = 0x0000;
	module->remaining_rx_buffer_length = 0x0000;
	module->registered_callback        = 0x00;
	module->enabled_callback           = 0x00;
	module->status                     = STATUS_OK;
	module->dir                        = SPI_DIRECTION_IDLE;
	module->locked                     = 0;
	/*
	 * Set interrupt handler and register SPI software module struct in
	 * look-up table
	 */
	if ((config->mode == SPI_MODE_SLAVE) || (config->receiver_enable)) {
		//Enable RX interrupt for this SPI module. Todo.
	}
#endif

	/* Write configuration to module and return status code */
	return _spi_set_config(module, config);
}

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
void spi_enable(struct spi_module *const module)
{
	Spi *const spi_module = (module->hw);

#if SPI_CALLBACK_MODE == true
	if((uint32_t)spi_module == SPI0)
		NVIC_EnableIRQ(SPIRX0_IRQn);
	else if((uint32_t)spi_module == SPI1)
		NVIC_EnableIRQ(SPIRX1_IRQn);
#endif

	/* Enable SPI */
	spi_module->SPI_MODULE_ENABLE.reg = SPI_SPI_MODULE_ENABLE_ENABLE;
}

/**
 * \brief Disables the SPI module
 *
 * This function will disable the SPI module.
 *
 * \param[in,out] module  Pointer to the software instance struct
 */
void spi_disable(struct spi_module *const module)
{
	Spi *const spi_module = (module->hw);

#  if SPI_CALLBACK_MODE == true
	if((uint32_t)spi_module == SPI0)
		NVIC_DisableIRQ(SPIRX0_IRQn);
	else if((uint32_t)spi_module == SPI1)
		NVIC_DisableIRQ(SPIRX1_IRQn);
#  endif

	/* Disable SPI */
	spi_module->SPI_MODULE_ENABLE.reg = (0x0ul << SPI_SPI_MODULE_ENABLE_ENABLE_Pos);
}

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
 * \retval STATUS_OK if the module was locked.
 * \retval STATUS_BUSY if the module was already locked.
 */
enum status_code spi_lock(struct spi_module *const module)
{
	enum status_code status;

	//DISABLE_GLOBAL INTERRUPTS
	//system_interrupt_enter_critical_section();

	if (module->locked) {
		status = STATUS_BUSY;
	} else {
		module->locked = true;
		status = STATUS_OK;
	}

	//system_interrupt_leave_critical_section();

	return status;
}

/**
 * \brief Unlock driver instance
 *
 * This function clears the instance lock, indicating that it is available for
 * use.
 *
 * \param[in,out] module Pointer to the driver instance to lock.
 *
 * \retval STATUS_OK if the module was locked.
 * \retval STATUS_BUSY if the module was already locked.
 */
void spi_unlock(struct spi_module *const module)
{
	module->locked = false;
}

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
 * \retval STATUS_OK    If the data was written
 * \retval STATUS_BUSY  If the last write was not completed
 */
enum status_code spi_write(struct spi_module *module, uint8_t tx_data)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(module->hw);

	Spi *const spi_module = (module->hw);

	/* Check if the data register has been copied to the shift register */
	if (!_spi_is_ready_to_write(spi_module)) {
		/* Data register has not been copied to the shift register, return */
		return STATUS_BUSY;
	}

	/* Write the character to the DATA register */
	spi_module->TRANSMIT_DATA.reg = tx_data & SPI_TRANSMIT_DATA_MASK;

	return STATUS_OK;
}

/**
 * \brief Reads last received SPI character
 *
 * This function will return the last SPI character shifted into the receive
 * register by the \ref spi_write function
 *
 * \note Receiver must be enabled in the configuration
 *
 * \param[in] module    Pointer to the software instance struct
 * \param[out] rx_data  Pointer to store the received data
 *
 * \returns Status of the read operation.
 * \retval STATUS_OK            If data was read
 * \retval STATUS_ERR_IO        If no data is available
 * \retval STATUS_ERR_OVERFLOW  If the data is overflown
 */
enum status_code spi_read(
		struct spi_module *const module,
		uint8_t *rx_data)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(module->hw);

	Spi *const spi_module = (module->hw);

	/* Check if data is ready to be read */
	if (!_spi_is_ready_to_read(spi_module)) {
		/* No data has been received, return */
		return STATUS_ERR_IO;
	}

	/* Return value */
	enum status_code retval = STATUS_OK;

	/* Check if data is overflown */
	if (spi_module->RECEIVE_STATUS.bit.FIFO_OVERRUN) {
		retval = STATUS_ERR_OVERFLOW;
	}

	/* Read the character from the DATA register */
	*rx_data = ((uint8_t)spi_module->RECEIVE_DATA.reg & SPI_RECEIVE_DATA_MASK);

	return retval;
}

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
 * \param[out] rx_data  Pointer to store the received data
 * \param[in] length    length of data to be read.
 *
 * \returns Status of the read operation.
 * \retval STATUS_OK            If data was read
 * \retval STATUS_ERR_IO        If no data is available
 * \retval STATUS_ERR_OVERFLOW  If the data is overflown
 */
enum status_code spi_transceive_buffer_wait(
		struct spi_module *const module,
		uint8_t *pu8tx_data,
		uint8_t *pu8rx_data,
		uint16_t length)
{
	Spi *spi_module = module->hw;
	uint8_t u8dummy = 0;
	uint8_t u8skipmosi = 0;
	uint8_t u8skipmiso = 0;
	uint8_t u8status;
	uint16_t u16transferlen = 0;

	if(spi_module == 0) {
		return STATUS_ERR_NOT_INITIALIZED;
	}
	if(!pu8tx_data) {
		pu8tx_data = &u8dummy;
		*pu8tx_data = module->tx_dummy_byte;
		u8skipmosi = 1;
	} else if(!pu8rx_data) {
		pu8rx_data = &u8dummy;
		u8skipmiso = 1;
	} else if(length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}

	//Check for Idle
	do {
		u8status = _spi_is_active(spi_module);
	}while(u8status);

	// Clear all status registers
	spi_module->RECEIVE_STATUS.reg;
	spi_module->TRANSMIT_STATUS.reg;

	//Start transfer
	while(u16transferlen < length) {
		//Write data to MOSI
		//tx_data = *pu8tx_data;
		while(!_spi_is_ready_to_write(spi_module));
		spi_module->TRANSMIT_DATA.reg = *pu8tx_data;
		//Read data shifted from MISO
		while(!_spi_is_ready_to_read(spi_module));
		*pu8rx_data = spi_module->RECEIVE_DATA.reg;
		//rx_fifo_not_empty = 0;
		u16transferlen++;
		if(!u8skipmosi)
			pu8tx_data++;
		if(!u8skipmiso)
			pu8rx_data++;
	}
	//check TXFIFO is empty
	do {
		u8status = _spi_is_write_complete(spi_module);
	}while(!u8status);

	return STATUS_OK;

}

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
 * \param[out] rx_data  Pointer to store the received data
 *
 * \returns Status of the read operation.
 * \retval STATUS_OK            If data was read
 * \retval STATUS_ERR_IO        If no data is available
 * \retval STATUS_ERR_OVERFLOW  If the data is overflown
 */
enum status_code spi_transceive_wait(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint8_t *rx_data)
{
	return spi_transceive_buffer_wait(module, tx_data, rx_data, 1);
}

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
 * \param[in] length    length of data to be read.
 * \param[in] dummy     dummy byte to be sent on bus when reading data.
 *
 * \returns Status of the read operation.
 * \retval STATUS_OK            If data was read
 * \retval STATUS_ERR_IO        If no data is available
 * \retval STATUS_ERR_OVERFLOW  If the data is overflown
 */
enum status_code spi_read_buffer_wait(
		struct spi_module *const module,
		uint8_t *rx_data,
		uint16_t length,
		uint8_t dummy)
{
	module->tx_dummy_byte = dummy;
	return spi_transceive_buffer_wait(module, NULL, rx_data, length);
}

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
 * \param[in] length    length of data to be read
 *
 * \returns Status of the read operation.
 * \retval STATUS_OK            If data was read
 * \retval STATUS_ERR_IO        If no data is available
 * \retval STATUS_ERR_OVERFLOW  If the data is overflown
 */
enum status_code spi_write_buffer_wait(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint16_t length)
{
	return spi_transceive_buffer_wait(module, tx_data, NULL, length);
}

/**
 * \brief Asserting/Deasserting the slave select for the corresponding slave.
 *
 * This function will assert or deassert the SS of the requested slave device.
 *
 * \param[in] module    Pointer to the software instance struct
 * \param[in] slave     Pointer containing slave instance.
 * \param[in] select    Bool to select the salve or deselect.
 *
 * \returns Status of the slave select operation.
 *
 * \retval STATUS_OK                If SS pin is a valid one and selected/deselected.
 * \retval STATUS_ERR_INVALID_ARG   Invalid SS pin.
 */
enum status_code spi_select_slave(
		struct spi_module *const module,
		struct spi_slave_inst *const slave,
		bool select)
{
	uint8_t gpio_num = slave->ss_pin;
	if(select) { // ASSERT Slave select pin
		gpio_pin_set_output_level(gpio_num, false);
	} else {			 // DEASSERT Slave select pin
		gpio_pin_set_output_level(gpio_num, true);
	}

	return STATUS_OK;
}
