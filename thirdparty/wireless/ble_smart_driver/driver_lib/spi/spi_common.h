#ifndef SPI_COMMON_H_INCLUDED
#define SPI_COMMON_H_INCLUDED
#include "spi.h"
#include "spi_hw.h"

/**
 * \internal Writes an SPI SERCOM configuration to the hardware module.
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
static enum spi_status_code _spi_set_config(
		struct spi_module *const module,
		const struct spi_config *const config)
{
	SPI_CONFIG_Type spi_cfg;
	uint8_t dummy = 0;
	SPI *const spi_module = (module->hw);

	spi_cfg.reg = 0;
	module->mode             = config->mode;
	module->receiver_enabled = config->receiver_enable;

#if CONF_SPI_MASTER_ENABLE == true
	/* Value to write to BAUD register */
	uint16_t baud = 0;
#endif

#if CONF_SPI_MASTER_ENABLE == true
	/* Find baud value and write it */
	if (config->mode == SPI_MODE_MASTER) {
		// This divider value needs to be based on baud rate. For now we will use the 1 for testing.
		//spi_module->CLKDIVIDER.reg = config->clock_divider;
	}
#endif
	/* Set data order */
	spi_cfg.bit.LSB_FIRST = config->data_order;
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
	spi_module->CONFIG.reg = spi_cfg.reg;
	#if 0
	//clear all spi RX status bits before enabling it.
	while(spi_module->RXSTATUS.reg != 0) {
		dummy = spi_module->RXDATA.reg;
	}
	
	// Write a dummy byte
	spi_module->TXDATA.reg = 0x0;
	#endif
	//Enable SPI
	spi_module->ENABLE.bit.SPI_ENABLE = 1;
	return SPI_STATUS_OK;
}

static inline void _spi_disable(SPI *const spi_module)
{
	if(spi_module == NULL)
		return;
	spi_module->ENABLE.reg = 0;
}

static inline void _spi_enable(SPI *const spi_module)
{
	if(spi_module == NULL)
		return;
	spi_module->ENABLE.reg = 1;
}

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
		SPI *const spi_module)
{
	return spi_module->ACTIVE.bit.SPI_ACTIVITY;
}

/**
 * \brief Checks if the SPI in master mode has shifted out last data, or if the master has ended the transfer in slave mode.
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
		SPI *const spi_module)
{
	/* Check interrupt flag */
	return (spi_module->TXSTATUS.bit.TX_FIFO_EMPTY);
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
		SPI *const spi_module)
{
	/* Check interrupt flag */
	return (spi_module->TXSTATUS.bit.TX_FIFO_NOT_FULL);
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
		SPI *const spi_module)
{
	/* Check interrupt flag */
	return (spi_module->RXSTATUS.bit.RX_FIFO_NOT_EMPTY);
}
#endif	//I2C_COMMON_H_INCLUDED
