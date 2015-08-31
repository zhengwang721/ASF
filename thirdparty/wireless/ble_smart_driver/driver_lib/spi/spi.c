/**
 * \file
 *
 * \brief I2C Master Driver
 *
 * Copyright (C) 2012-2015 Atmel Corporation. All rights reserved.
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
#include <stdint.h>
#include <string.h>
#include "CMSDK_CM0.h"
#include "spi.h"
#include "spi_hw.h"
#include "spi_common.h"
#include "dma_if.h"
#include "common.h"
#include "platform_drv.h"

#define CONF_SPI_MASTER_ENABLE	(1)

/**
 * \brief Initializes an SPI peripheral slave device configuration structure to default values
 *
 * This function will initialize a given SPI slave device configuration
 * structure to a set of known default values. This function should be called
 * on any new instance of the configuration structures before being modified by
 * the user application.
 *
 * The default configuration is as follows:
 *  \li Slave Select on GPIO pin 10
 *  \li Addressing not enabled
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
void spi_slave_inst_get_config_defaults( struct spi_slave_inst_config *config)
{
	config->ss_pin          = LPGPIO_12;
	config->address_enabled = 0;
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
 *  \li MUX Setting D
 *  \li Character size 8 bit
 *  \li Not enabled in sleep mode
 *  \li Receiver enabled
 *  \li Baudrate 100000
 *  \li Default pinmux settings for all pads
 *  \li GCLK generator 0
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
void spi_get_config_defaults(
		struct spi_config *const config)
{
	SPI *spi_module = (SPI *)(SPI1_BASE);
	/* Default configuration values */
	if((spi_module->ENABLE.reg == 0) && (spi_module->CLKDIVIDER.reg == 0)) {
		config->core_idx				 = SPI_CORE1;
	}
	else {
		spi_module = (SPI *)(SPI2_BASE);
		if((spi_module->ENABLE.reg == 0) && (spi_module->CLKDIVIDER.reg == 0))
			config->core_idx				 = SPI_CORE2;
		else {
			config->core_idx				 = SPI_CORE_MAX;
			return;
		}
	}
	config->mode             = SPI_MODE_MASTER;
	config->data_order       = SPI_DATA_ORDER_MSB;
	config->transfer_mode    = SPI_TRANSFER_MODE_0;
	config->mux_setting      = SPI_SIGNAL_MUX_SETTING_A;
	//config->run_in_standby   = 0;
	config->receiver_enable  = 1;
	config->clock_source		 = SPI_CLK_INPUT_0;
	config->clock_divider		 = 2;
	
	/* Clear mode specific config */
	memset(&(config->mode_specific), 0, sizeof(config->mode_specific));

	/* Master config defaults */
	config->mode_specific.master.baudrate = ((260000000/config->clock_divider)/4);

	/* pinmux config defaults  Upper 16 bits inform the GPIO number and lower 16 bits for pinmux selection*/
	if(config->core_idx == SPI_CORE1){
		config->pinmux_pad[0] = (LPGPIO_10 << 8)|(PINMUX_VAL_2);
		config->pinmux_pad[1] = (LPGPIO_11 << 8)|(PINMUX_VAL_2);
		// SS will be controlled from application layer so just configure as GPIO
		config->pinmux_pad[2] = (LPGPIO_12 << 8)|(PINMUX_VAL_0);	
		config->pinmux_pad[3] = (LPGPIO_13 << 8)|(PINMUX_VAL_2);
	}
	else {
		config->pinmux_pad[0] = (LPGPIO_2 << 8)|(PINMUX_VAL_4);
		config->pinmux_pad[1] = (LPGPIO_3 << 8)|(PINMUX_VAL_4);
		// SS will be controlled from application layer so just configure as GPIO
		config->pinmux_pad[2] = (LPGPIO_4 << 8)|(PINMUX_VAL_0);
		config->pinmux_pad[3] = (LPGPIO_5 << 8)|(PINMUX_VAL_4);
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
 * \retval SPI_STATUS_OK 					if the requested SS pin is available and configured.
 * \retval SPI_STATUS_ERR_DENIED 	if the requested SS pin is not available.
 *
 */
enum spi_status_code spi_attach_slave(
		struct spi_slave_inst *const slave,
		struct spi_slave_inst_config *const config)
{
	//Assert(slave);
	//Assert(config);

	slave->ss_pin          = config->ss_pin;
	slave->address_enabled = config->address_enabled;
	slave->address         = config->address;

	if(!port_list[config->ss_pin].bit.available) {
		return SPI_STATUS_ERR_DENIED;
	}
	else {
		port_list[config->ss_pin].bit.available = 0;
		/* enable GPIO to be SS */
		CMSDK_GPIO0->OUTENABLESET |= 1 << slave->ss_pin;
		CMSDK_GPIO0->DATAOUT |= 1 << slave->ss_pin;	
	}
	return SPI_STATUS_OK;
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
void spi_enable(
		struct spi_module *const module)
{
	SPI *const spi_module = (module->hw);

#if SPI_CALLBACK_MODE == true
	//system_interrupt_enable(_sercom_get_interrupt_vector(module->hw));
	if((uint32_t)spi_module == SPI1_BASE)
		NVIC_EnableIRQ(SPIRX0_IRQn);
	else if((uint32_t)spi_module == SPI2_BASE)
		NVIC_EnableIRQ(SPIRX1_IRQn);
#endif

	/* Enable SPI */
	spi_module->ENABLE.bit.SPI_ENABLE = 1;
}

/**
 * \brief Disables the SERCOM SPI module
 *
 * This function will disable the SERCOM SPI module.
 *
 * \param[in,out] module  Pointer to the software instance struct
 */
void spi_disable(
		struct spi_module *const module)
{
	SPI *const spi_module = (module->hw);

#  if SPI_CALLBACK_MODE == true
	//system_interrupt_disable(_sercom_get_interrupt_vector(module->hw));
	if((uint32_t)spi_module == SPI1_BASE)
		NVIC_DisableIRQ(SPIRX0_IRQn);
	else if((uint32_t)spi_module == SPI2_BASE)
		NVIC_DisableIRQ(SPIRX1_IRQn);
#  endif

	/* Disable SPI */
	spi_module->ENABLE.bit.SPI_ENABLE = 0;
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
 * \retval SPI_STATUS_OK if the module was locked.
 * \retval SPI_STATUS_BUSY if the module was already locked.
 */
enum spi_status_code spi_lock(struct spi_module *const module)
{
	enum spi_status_code status;

	//DISABLE_GLOBAL INTERRUPTS
	//system_interrupt_enter_critical_section();

	if (module->locked) {
		status = SPI_STATUS_BUSY;
	} else {
		module->locked = 1;
		status = SPI_STATUS_OK;
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
 * \retval SPI_STATUS_OK if the module was locked.
 * \retval SPI_STATUS_BUSY if the module was already locked.
 */
void spi_unlock(struct spi_module *const module)
{
	module->locked = 0;
}

/**
 * \brief Resets the SPI module
 *
 * This function will reset the SPI module to its power on default values and
 * disable it.
 *
 * \param[in,out] module Pointer to the software instance struct
 */
void spi_reset(
		struct spi_module *const module)
{
	/* Sanity check arguments */
	uint32_t regval = 0;
	SPI *const spi_module = (module->hw);

	/* Disable the module */
	//spi_disable(module);

	/* Software reset the module */
	//Reset the SPI cores.
	regval = REG_RD(0x4000B004);
	if((uint32_t)spi_module == SPI1_BASE) {
		regval &= ~((1 << 4) | (1 << 5));		//Assert Reset to SPI cores.
	}
	if((uint32_t)spi_module == SPI2_BASE) {
		regval &= ~((1 << 6) | (1 << 7));		//Assert Reset to SPI cores.
	}
	REG_WR(0x4000B004, regval);
	if((uint32_t)spi_module == SPI1_BASE) {
		regval |= ((1 << 4) | (1 << 5));		//Clear Reset to SPI cores.
	}
	if((uint32_t)spi_module == SPI2_BASE) {
		regval |= ((1 << 6) | (1 << 7));		//Clear Reset to SPI cores.
	}
	REG_WR(0x4000B004, regval);
}

/**
 * \brief Initializes the SERCOM SPI module
 *
 * This function will initialize the SERCOM SPI module, based on the values
 * of the config struct.
 *
 * \param[out]  module  Pointer to the software instance struct
 * \param[in]   hw      Pointer to hardware instance
 * \param[in]   config  Pointer to the config struct
 *
 * \return Status of the initialization
 * \retval SPI_STATUS_OK              				 Module initiated correctly.
 * \retval SPI_STATUS_ERR_GPIO_NOT_AVAILABLE   If requested GPIO for SPI is not available.
 * \retval SPI_STATUS_ERR_INVALID_ARG  				 If invalid argument(s) were provided.
 */
enum spi_status_code spi_init(
		struct spi_module *const module,
		const struct spi_config *const config)
{

	uint8_t idx;
	uint8_t pinnum = 0;
	uint8_t muxval = 0;
	uint32_t regaddr = 0;
	uint32_t regval = 0;
	/* Initialize device instance */
	if(config->core_idx == SPI_CORE1) {
		module->hw = (void *)SPI1_BASE;
#ifdef CHIPVERSION_B0
		platform_unregister_isr(SPI1_RX_VECTOR_TABLE_INDEX);
#endif	//CHIPVERSION_B0
	}
	else if(config->core_idx == SPI_CORE2)
		module->hw = (void *)SPI2_BASE;
	else
		return SPI_STATUS_ERR_INVALID_ARG;

	SPI *const spi_module = (module->hw);

	/* Check if module is enabled. */
	if (spi_module->ENABLE.bit.SPI_ENABLE) {
		spi_module->ENABLE.bit.SPI_ENABLE = 0;
	}

	spi_reset(module);

	//Program the pinmux.

	// Set the pinmux for this spi module.
	for(idx=0;idx<4;idx++) {
		pinnum = (uint8_t)((config->pinmux_pad[idx] >> 8) & 0xFF);
		muxval = (uint8_t)(config->pinmux_pad[idx] & 0xFF);
		if(!port_list[pinnum].bit.available) {
			return SPI_STATUS_ERR_GPIO_NOT_AVAILABLE;
		} 
		else {
			port_list[pinnum].bit.available = 0;
		}
		if( pinnum <= 7) {
			regaddr = 0x4000b044;
		}
		else if(pinnum <= 15) {
			regaddr = 0x4000b048;
		}
		else if(pinnum <= 23) {
			regaddr = 0x4000b04c;
		}
		regval = REG_RD(regaddr);
		regval &= ~(7 << ((pinnum%8)*4));
		regval |= (muxval << ((pinnum%8)*4));
		REG_WR(regaddr,regval);
	}

	//Setting the default value for SS- PIN.
	pinnum = (config->pinmux_pad[2] >> 8) & 0xFF;
	if(pinnum < 16) {
		CMSDK_GPIO0->OUTENABLESET |= (1 << pinnum);
		CMSDK_GPIO0->DATAOUT |= (1 << pinnum);
	}
	else {
		CMSDK_GPIO1->OUTENABLESET |= (1 << (pinnum % 16));
		CMSDK_GPIO1->DATAOUT |= (1 << (pinnum % 16));
	}
	//Only for SS we set it to available and it will be used 
	//when application decides to use it by attaching the slave.
	port_list[pinnum].bit.available = 1;
	
	/* Set up the input clock & divider for the module */
	spi_module->CLKSEL.reg = config->clock_source;
	spi_module->CLKDIVIDER.reg = config->clock_divider;
	
#  if CONF_SPI_MASTER_ENABLE == true
	if (config->mode == SPI_MODE_MASTER) {
		/* Set the SERCOM in SPI master mode */
		spi_module->MODE.reg = SPI_MODE_MASTER;
	}
#  endif

#  if CONF_SPI_SLAVE_ENABLE == true
	if (config->mode == SPI_MODE_SLAVE) {
		/* Set the SERCOM in SPI slave mode */
		spi_module->MODE.reg = SPI_MODE_SLAVE;
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
	module->status                     = SPI_STATUS_OK;
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
 * \brief Transfers a single SPI character
 *
 * This function will send a single SPI character via SPI and ignore any data
 * shifted in by the connected device. To both send and receive data, use the
 * \ref spi_transceive_wait function or use the \ref spi_read function after
 * writing a character. The \ref spi_is_ready_to_write function
 * should be called before calling this function.
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
		uint8_t tx_data)
{
	/* Sanity check arguments */
	//Assert(module);
	//Assert(module->hw);

	SPI *const spi_module = (module->hw);

	/* Check if the data register has been copied to the shift register */
	if (!_spi_is_ready_to_write(spi_module)) {
		/* Data register has not been copied to the shift register, return */
		return SPI_STATUS_BUSY;
	}

	/* Write the character to the DATA register */
	spi_module->TXDATA.bit.DATA = tx_data;

	return SPI_STATUS_OK;
}

/**
 * \brief Reads last received SPI character
 *
 * This function will return the last SPI character shifted into the receive
 * register by the \ref spi_write function
 *
 * \note The \ref spi_is_ready_to_read function should be called before calling
 *       this function.
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
		uint8_t *rx_data)
{
	/* Sanity check arguments */
	//Assert(module);
	//Assert(module->hw);

	SPI *const spi_module = (module->hw);

	/* Check if data is ready to be read */
	if (!_spi_is_ready_to_read(spi_module)) {
		/* No data has been received, return */
		return SPI_STATUS_ERR_IO;
	}

	/* Return value */
	enum spi_status_code retval = SPI_STATUS_OK;

	/* Check if data is overflown */
	if (spi_module->RXSTATUS.bit.RX_FIFO_OVERRUN) {
		retval = SPI_STATUS_ERR_OVERFLOW;
	}

	/* Read the character from the DATA register */
	*rx_data = ((uint8_t)spi_module->RXDATA.reg & 0xFF);

	return retval;
}

enum spi_status_code spi_transceive_buffer_wait(
		struct spi_module *const module,
		uint8_t *pu8tx_data,
		uint8_t *pu8rx_data,
		uint16_t length)
{
	SPI *spi_module = module->hw;
	uint8_t u8dummy = 0;
	uint8_t u8skipmosi = 0;
	uint8_t u8skipmiso = 0;
	uint8_t u8status;
	uint16_t u16transferlen = 0;
	if(spi_module == 0)
		return SPI_STATUS_ERR_NOT_INITIALIZED;
	if(!pu8tx_data) {
		pu8tx_data = &u8dummy;
		*pu8tx_data = module->tx_dummy_byte;
		u8skipmosi = 1;
	}
	else if(!pu8rx_data) {
		pu8rx_data = &u8dummy;
		u8skipmiso = 1;
	}
	else if(length == 0)
		return SPI_STATUS_ERR_INVALID_ARG;
	
	//Check for Idle
	do {
		u8status = _spi_is_active(spi_module);
	}while(u8status);
	
	// Clear all status registers
	spi_module->RXSTATUS.reg;
	spi_module->TXSTATUS.reg;
	
	//Start transfer
	while(u16transferlen < length) {
		//Write data to MOSI
		//tx_data = *pu8tx_data;
		while(!_spi_is_ready_to_write(spi_module));
		spi_module->TXDATA.reg = *pu8tx_data;
		//Read data shifted from MISO
		while(!_spi_is_ready_to_read(spi_module));
		*pu8rx_data = spi_module->RXDATA.reg;
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

	return SPI_STATUS_OK;
	
}

enum spi_status_code spi_transceive_wait(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint8_t *rx_data)
{
	return spi_transceive_buffer_wait(module,tx_data,rx_data,1);
}

enum spi_status_code spi_read_buffer_wait(
		struct spi_module *const module,
		uint8_t *rx_data,
		uint16_t length,
		uint8_t dummy)
{
	module->tx_dummy_byte = dummy;
	return spi_transceive_buffer_wait(module,NULL,rx_data,length);
}

enum spi_status_code spi_write_buffer_wait(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint16_t length)
{
	return spi_transceive_buffer_wait(module,tx_data,NULL,length);
}

enum spi_status_code spi_select_slave(
		struct spi_module *const module,
		struct spi_slave_inst *const slave,
		bool select)
{
	uint8_t gpio_num = slave->ss_pin;
	if(select) { // ASSERT Slave select pin
		if(gpio_num < 16) {
			CMSDK_GPIO0->DATAOUT &= ~(1 << gpio_num);
		}
		else if(gpio_num < 25) {
			CMSDK_GPIO1->DATAOUT &= ~(1 << (gpio_num % 16));
		}
		else {
			return SPI_STATUS_ERR_INVALID_ARG;
		}
	}
	else {			 // DEASSERT Slave select pin
		if(gpio_num < 16) {
			CMSDK_GPIO0->DATAOUT |= (1 << gpio_num);
		}
		else if(gpio_num < 25) {
			CMSDK_GPIO1->DATAOUT |= (1 << (gpio_num % 16));
		}
		else {
			return SPI_STATUS_ERR_INVALID_ARG;
		}
	}
	return SPI_STATUS_OK;
}
