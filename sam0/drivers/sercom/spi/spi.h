/**
 * \file
 *
 * \brief SAM0+ Serial Peripheral Interface Driver
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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

#ifndef _SPI_H_INCLUDED_
#define _SPI_H_INCLUDED_

#include <stdbool.h>
#include <stdint.h>
#include "../sercom.h"
#ifdef __cplusplus
extern "C" {
#endif

//! \todo: SYNCBUSY: ENABLE, SWRST, RXEN
/**
 * \defgroup sam0_spi_group SAM0+ Serial Peripheral Interface Driver (SPI)
 *
 * Driver for the SAM0+ architecture devices.
 * This driver encompasses the following module within the SAM0+ devices:
 * \li \b SPI \b (Serial Peripheral Interface)
 *
 * \section module_introduction Introduction
 *
 * \subsection module_overview SPI Overview
 * The Serial Peripheral Interface (SPI) is a high-speed synchronous data
 * transfer interface using three or four pins. It allows fast communication
 * between an AVR device and a peripheral device or between several AVR
 * devices. SPI supports both full and half duplex communication.
 * \n\n
 * A device connected to the bus must act as a master or a slave. The master
 * initiates and controls all data transactions. 
  * \dot
 * digraph spi_slaves_par {
 *   subgraph cluster_spi_master {
 *    shift_reg [label="Shift register", shape=box];
 *    mosi_m [label="MOSI", shape=none];
 *    miso_m [label="MISO", shape=none];
 *    sck_m [label="SCK", shape=none];
 *    ss_m [label="_SS", shape=none];
 *    {rank=same; mosi_m miso_m sck_m ss_m}
 *    label="SPI Master";
 *   }
 *   subgraph cluster_spi_slave {
 *    mosi_s [label="MOSI", shape=none];
 *    miso_s [label="MISO", shape=none];
 *    sck_s [label="SCK", shape=none];
 *    ss_s [label="_SS", shape=none];
 *    shift_reg_s [label="Shift register", shape=box];
 *    {rank=same; mosi_s miso_s sck_s ss_s}
 *    label="SPI Slave";
 *    rankdir=LR;
 *   }
 *   shift_reg:e -> mosi_m:w [label=""];
 *   mosi_m:e -> mosi_s:w [label=""];
 *   mosi_s:e -> shift_reg_s:w [label=""];
 *   miso_s:w -> miso_m:e [label=""];
 *   sck_m -> sck_s;
 *   ss_m -> ss_s;
 *   shift_reg_s:se -> miso_s:e [label=""];
 *   miso_m:w -> shift_reg:sw [label=""];
 *   rankdir=LR;
 * }
 * \enddot
 * \subsection master_mode Master Mode
 * When configured as a master, the _SS pin must be configured as an output
 * and controlled by user software.
 * \n\n
 * If the bus consists of several SPI slaves,
 * an SPI master can use general I/O pins to control separate _SS lines to
 * each slave on the bus. In this configuration, the selected slave will drive
 * the tri-state MISO line.
 * \n\n
 * It is also possible to connect all slaves in series. In this configuration,
 * a common _SS is provided to \c N slaves, enabling them simultaneously. For a
 * complete transaction, the master must shift \c N+1 characters.
 * \subsubsection data_transfer Data Transfer
 * Writing a character to the DATA register starts the SPI clock generator, and
 * the DATA register is transferred to the shift register when the shift
 * register is empty.
 * Once the contents of the DATA register have been trasferred to the shift
 * register, a new character can be written to the DATA register.
 * As each character is shiftet out from the master, a character is shifted in
 * from the slave. If the receiver is enabled, the data is moved to the receive
 * bufer at the completion of the frame and can be read from the DATA register.
 * \subsection slave_mode Slave Mode
 * When configured as a slave, the SPI interface will remain inactive with MISO
 * tristated as long as the _SS pin is driven high. The data register can be
 * updated at any time.
 * As the SPI slave shift register is clocked by SCK, a minimum of three SCK
 * cycles are needed from the time DATA is written, until the character is
 * ready to be shifted out. If the shift register has not been loaded with
 * data, the curren contents will be transmitted.
 * \n\n
 * If constant transmission of data is needen in SPI slave mode, the system
 * clock should be faster than SCK.
 * If the receiver is enabled, the recieved character can be read from the
 * DATA register. When _SS is driven high, the slave will not receive any
 * additional data.
 * \subsubsection addr_recognition Address Recognition
 * When the SPI is configured with address recognition, the first character in
 * a transaction is checked for an address match. If there is a match, the MISO
 * output is enabled and the transaction is processed.
 * If the address does not match, the complete transaction is ignored.
 * \n\n
 * If the device is asleep, it can be woken up by an address match in order
 * to process the transaction.
 *
 * \subsection data_modes Data Modes
 * There are four combinations of SCK phase and polarity with respect to
 * serial data.
 * <table>
 *   <tr>
 *      <th>Mode</th>
 *      <th>CPOL</th>
 *      <th>CPHA</th>
 *      <th>Leading Edge</th>
 *      <th>Trailing Edge</th>
 *   </tr>
 *   <tr>
 *      <td> 0 </td>
 *      <td> 0 </td>
 *      <td> 0 </td>
 *      <td> Rising, Sample </td>
 *      <td> Falling, Setup </td>
 *   </tr>
 *   <tr>
 *      <td> 1 </td>
 *      <td> 0 </td>
 *      <td> 1 </td>
 *      <td> Rising, Setup </td>
 *      <td> Falling, Sample </td>
 *   </tr>
 *   <tr>
 *      <td> 2 </td>
 *      <td> 1 </td>
 *      <td> 0 </td>
 *      <td> Falling, Sample </td>
 *      <td> Rising, Setup </td>
 *   </tr>
 *   <tr>
 *      <td> 3 </td>
 *      <td> 1 </td>
 *      <td> 1 </td>
 *      <td> Falling, Setup </td>
 *      <td> Rising, Sample </td>
 *   </tr>
 * </table>
 *
 *
 * \subsection full_duplex Full-Duplex
 * When enabled as full-duplex SPI, the pins are automatically configured
 * as seen in the table below. If the receiver is disabled, the data input
 * (MISO for master, MOSI for slave) can be used for other purposes.
 * In master mode, the SS pins must be configured through port configuration.
 *
 * <table>
 *   <tr>
 *      <th> Pin </th>
 *      <th> Master SPI </th>
 *      <th> Slave SPI </th>
 *   </tr>
 *   <tr>
 *      <td> MOSI </td>
 *      <td> Output </td>
 *      <td> Input </td>
 *   </tr>
 *   <tr>
 *      <td> MISO </td>
 *      <td> Input </td>
 *      <td> Output </td>
 *   </tr>
 *   <tr>
 *      <td> SCK </td>
 *      <td> Output </td>
 *      <td> Input </td>
 *   </tr>
 *   <tr>
 *      <td> _SS </td>
 *      <td> User defined output enable </td>
 *      <td> Input </td>
 *   </tr>
 * </table>
 *
 * \subsection half_duplex Half-Duplex
 * Half-duplex operation is functionally the same as full-duplex operation,
 * with the exception of the data output enable and data modes.
 * For single wire mode, it is up to the software to place MISO and MOSI on the
 * same pin.
 * A half-duplex SPI transaction always begins with the transmission of /c n
 * characters from master to slave. This can optionally be followed by /c n
 * characters from slave to master. In this mode, a bus keeped or pullup should
 * be used to hold the DIO line in a known state between transactions.
 *
 * <table>
 *   <tr>
 *      <th> Pin </th>
 *      <th> Master SPI </th>
 *      <th> Slave SPI </th>
 *   </tr>
 *   <tr>
 *      <td> DIO </td>
 *      <td> Hardware controlled I/O </td>
 *      <td> Hardware controlled I/O </td>
 *   </tr>
 *   <tr>
 *      <td> SCK </td>
 *      <td> Output </td>
 *      <td> Input </td>
 *   </tr>
 *   <tr>
 *      <td> _SS </td>
 *      <td> User defined output enable </td>
 *      <td> Input </td>
 *   </tr>
 * </table>
 *
 * Even though DIO is hardware controlled, the port output must be software
 * enabled so that it can be overridden by the SPI.
 *
 * \subsection clock_generation Clock Generation
 * In SPI master mode, the clock (SCK) is generated internally using the
 * SERCOM Baud Rate Generator. In SPI slave mode, the clock is provided by
 * an external master on the SCK pin. This clock is used to directly clock
 * the SPI shift register.
 *
 * \section dependencies Dependencies
 * The SPI driver has the following dependencies:
 *
 * \section special_cons Special Considerations
 *
 * \subsection extra_info Extra Information
 * For extra information see \ref spi_extra_info.
 *
 * \section module_examples Examples
 * - \ref spi_quickstart
 *
 * \section api_overview API Overview
 * @{
 */

/**
 * \brief SPI transfer modes enum
 */
 enum spi_transfer_mode {
	/** Mode 0. Leading edge: rising, sample. Trailing edge: falling, setup */
	SPI_TRANSFER_MODE_0 = 0,
	/** Mode 1. Leading edge: rising, setup. Trailing edge: falling, sample */
	SPI_TRANSFER_MODE_1 = SPI_CPHA_bm,
	/** Mode 2. Leading edge: falling, sample. Trailing edge: rising, setup */
	SPI_TRANSFER_MODE_2 = SPI_CPOL_bm,
	/** Mode 3. Leading edge: falling, setup. Trailing edge: rising, sample */
	SPI_TRANSFER_MODE_3 = SPI_CPHA_bm | SPI_CPOL_bm,
};

/**
 * \brief SPI frame format enum
 *
 */
 enum spi_frame_format {
	/** SPI frame */
	SPI_FRAME_FORMAT_SPI_FRAME = 0,
	/** SPI frame with address */
	SPI_FRAME_FORMAT_SPI_FRAME_ADDR = SPI_FORM_WITH_ADDR_bm,
	/** Half duplex SPI frame */
	SPI_FRAME_FORMAT_HALF_DUPLEX = SPI_FORM_HALF_DUP_bm,
};

/**
 * \brief SPI pin configurations
 * DO: Data Out
 * DI: Data in
 * SLAVE_SS: Slave Select when operating in slave mode
 * SCK: Clock
 */
enum spi_pinout {
	// TODO: DO = DI half duplex?
	/**
	 * DO: SERCOM_PIN0 SCK: SERCOM_PIN1 SLAVE_SS: SERCOM_PIN2 DI: SERCOM_PIN0
	 * Half-duplex
	 */
	SPI_PINOUT_A = (SPI_DOPO_PIN0_PIN1_PIN2 | SPI_DIPO_PIN0),
	// TODO: SS = DI -> Master mode
	/** 
	 * DO: SERCOM_PIN0 SCK: SERCOM_PIN1 SLAVE_SS: SERCOM_PIN2 DI: SERCOM_PIN1
	 * Master Mode
	 */
	SPI_PINOUT_B = (SPI_DOPO_PIN0_PIN1_PIN2 | SPI_DIPO_PIN1),
	// TODO SS = DI -> Master mode
	/**
	 * DO: SERCOM_PIN0 SCK: SERCOM_PIN1 SLAVE_SS: SERCOM_PIN2 DI: SERCOM_PIN2
	 * Master Mode
	 */
	SPI_PINOUT_C = (SPI_DOPO_PIN0_PIN1_PIN2 | SPI_DIPO_PIN2),
	/**
	 * DO: SERCOM_PIN0 SCK: SERCOM_PIN1 SLAVE_SS: SERCOM_PIN2 DI: SERCOM_PIN3
	 */
	SPI_PINOUT_D = (SPI_DOPO_PIN0_PIN1_PIN2 | SPI_DIPO_PIN3),
	/**
	 * DO: SERCOM_PIN2 SCK: SERCOM_PIN3 SLAVE_SS: SERCOM_PIN1 DI: SERCOM_PIN0
	 */
	SPI_PINOUT_E = (SPI_DOPO_PIN2_PIN3_PIN1 | SPI_DIPO_PIN0),
	// TODO: SS = DI -> Master mode
	/**
	 * DO: SERCOM_PIN2 SCK: SERCOM_PIN3 SLAVE_SS: SERCOM_PIN1 DI: SERCOM_PIN1
	 * Master Mode
	 */
	SPI_PINOUT_F = (SPI_DOPO_PIN2_PIN3_PIN1 | SPI_DIPO_PIN1),
	// TODO: SCK = DI
	/**
	 * DO: SERCOM_PIN2 SCK: SERCOM_PIN3 SLAVE_SS: SERCOM_PIN1 DI: SERCOM_PIN2
	 * Receiver turned off
	 */
	SPI_PINOUT_G = (SPI_DOPO_PIN2_PIN3_PIN1 | SPI_DIPO_PIN2),
	// TODO: SCK = DI
	/**
	* DO: SERCOM_PIN2 SCK: SERCOM_PIN3 SLAVE_SS: SERCOM_PIN1 DI: SERCOM_PIN3
	 * Receiver turned off
	 */
	SPI_PINOUT_H = (SPI_DOPO_PIN2_PIN3_PIN1 | SPI_DIPO_PIN3),
};

/**
 * \brief SPI address modes enum
 *
 * For slave mode when using the SPI frame with address format.
 *
 */
enum spi_addr_mode {
	/** ADDRMASK is used as a mask to the ADDR register */
	SPI_ADDRMODE_MASK  = 0,
	/** The slave responds to the two unique addresses in ADDR and ADDRMASK */
	SPI_ADDRMODE_UNIQUE= SPI_ADDRMODE_UNIQUE_bm,
	/**
	 * The slave responds to the range of addresses between and including ADDR
	 * and ADDRMASK. ADDR is the upper limit
	 */
	SPI_ADDRMODE_RANGE = SPI_ADDRMODE_RANGE_bm,
};


/**
 * \brief SPI data order enum
 */
enum spi_dord {
	/** The LSB of the data byte is transmitted first */
	SPI_DORD_LSB = SPI_DORD_bm,
	/** The MSB of the data byte is transmitted first */
	SPI_DORD_MSB = 0,
};

/**
 * \brief SPI software device instance structure
 */
struct spi_dev_inst {
/** \todo SERCOM_t or SPI_t */
	SERCOM_t *hw_dev;
	bool master_mode;
};

/**
 * \brief SPI master conf
 *
 */
struct spi_master_conf {
	uint32_t baudrate;
};

/**
 * \brief SPI slave conf
 * \todo Frame format in slave conf -> half duplex only available in slave?
 */
struct spi_slave_conf {
	/** Frame format */
	enum spi_frame_format frame_format;
	/** Address mode */
	enum spi_addr_mode addr_mode;
	/** Address */
	uint8_t address;
	/** Address mask */
	uint8_t address_mask;
};

/**
 * \brief SPI configuration structure
 *
 * Configuration structure for an SPI instance. This structure should be
 * initialized by the \ref spi_get_config_defaults()
 * function before being modified by the user application.
 */
struct spi_conf {
	/** Enable master mode */
	bool master_mode;
	/** 
	 * The LSB of the data is transmitted first. If false, MSB is transmitted
	 * first.
	 */
	bool data_order_lsb;
	/** Transfer mode */
	enum spi_transfer_mode transfer_mode;
	/** Data pinout */
	enum spi_pinout pinout;
	/** Enable in sleep modes */
	bool sleep_enable;
	//TODO:??
	/** Union for slave or master specific config */
	union {
		/** Slave specific configuration*/
		struct spi_slave_conf slave_config;
			//addrmode, addr, addrmask, frame_format
		/** Master specific configuration*/
		struct spi_master_conf master_config;
			// baud,
	};
};

/**
 * \name Driver initialization and configuration
 * @{
 */

/**
 * \internal Wait until the synchronization is complete
 */
static inline void _spi_wait_for_sync(struct spi_dev_inst *const dev_inst)
{
	SERCOM_t *const spi_module = dev_inst->hw_dev;

	while (spi_module->SPI->STATUS & SPI_SYNCBUSY_bm);
}

/**
 * \brief Initializes an SPI configuration structure to defaults
 *
 *  Initializes a given SPI configuration structure to a set of
 *  known default values. This function should be called on any new
 *  instance of the configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *   \li Master mode enabled
 *   \li Not enabled in sleep mode
 *   \li Transfer mode 0
 *   \li Baudrate 9600
 *   \li MSB of the data is transmitted first
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
static inline void spi_get_config_defaults(struct spi_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->master_mode = true;
	config->sleep_enable = false;
	config->transfer_mode = SPI_TRANSFER_MODE_0;
	config->data_order_lsb = true;

	config->master_config.baudrate = 9600;
};

enum status_code spi_init(struct spi_dev_inst *const dev_inst, SPI_t *module,
		struct spi_conf *config);

/**
 * \brief Enable the SERCOM SPI module
 *
 * Enables the SERCOM SPI module.
 *
 * \param dev_inst    Pointer to the software instance struct
 */
static inline spi_enable(struct spi_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev->SPI);

	SERCOM_t *const spi_module = dev_inst->hw_dev;

	/* Wait until the synchronization is complete */
	_spi_wait_for_sync(dev_inst);

	// TODO: enable sercom mode spi? SERCOM_MODE_SPI_gc = 1,
	/* Enable SPI */
	spi_module->SPI->CTRLA |= SPI_ENABLE_bm;
}

/**
 * \brief Disables the SERCOM SPI module
 *
 * Disables the SERCOM SPI module. Writes only prescaler to register.
 *
 * \param dev_inst    Pointer to the software instance struct
 */
static inline spi_disable(struct spi_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev->SPI);

	SERCOM_t *const spi_module = dev_inst->hw_dev;

	/* Wait until the synchronization is complete */
	_spi_wait_for_sync(dev_inst);
	/* Disable SPI */
	spi_module->SPI->CTRLA &= ~SPI_ENABLE_bm;
}
/** @} */

/**
 * \name Read/Write
 * @{
 */

/**
 * \brief Transfer single byte via SPI
 *
 * This function will send a single byte via SPI and ignore any data shifted
 * in by the connected device. To both send and receive data you must use the
 * \ref spi_tranceive spi_tranceive function. This function will block until
 * any previous data has been moved from the transmit buffer to the shift
 * register, making it safe to repeatedly call the function without causing data
 * corruption. Note that this function does not handle the SS (Slave Select)
 * pin(s) in master mode; this must be handled from the user application.
 *
 * \param[in] dev_inst    Pointer to the software instance struct
 * \param[in] tx_data     Data to transmit
 */
//TODO: Add timeout (for slave mode). Define timeout value?
//TODO: 9 bit character?
static inline void spi_write(struct spi_dev_inst *dev_inst,
		uint8_t tx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_t *const spi_module = dev_inst->hw_dev;

	/* Wait until the data register has been copied to the shift register */
	while (!(spi_module->SPI->INTFLAGS & SPI_DREIF_bm));

	/* Write to the DATA register */
	spi_module->SPI->DATA = tx_data;
}

/**
 * \brief Transfer a block of \ref length length bytes via SPI
 *
 * This function will transfer a block of bytes via the SPI
 * and discard any data that is received. To both send and receive a block of
 * data you must use the \ref spi_tranceive_block function.
 * Note that this functions does not handle the _SS (slave select) pin(s) in
 * master mode; this must be handled by the user application.
 *
 * \param[in] dev_inst  Pointer to the software instance struct
 * \param[in] data      Pointer to the buffer to transmit
 * \param[in] length    Number of bytes to transfer
 *
 * \return
 * \retval STATUS_OK              If the transfer is completed
 * \retval STATUS_ERR_INVALID_ARG If the length is 0
 */
static inline enum status_code spi_write_block(struct spi_dev_inst
		*const dev_inst, const uint8_t *tx_data, uint8_t length)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Write block */
	while (length--) {
		spi_write(dev_inst, *(tx_data++));
	}

	return STATUS_OK;
}

/**
 * \brief Read last received byte from receive register
 *
 * This function will return the last byte shifted into the receive register
 * in the SPI peripheral. It will poll the RX complete interrupt flag before
 * returning the data. In slave mode this will block until a byte has been
 * shifted in from an external SPI master. In master mode this will return
 * the data that was shifted in by the last master TX as soon as the transfer
 * is complete.
 * \todo receive must be enabled
 *
 * \param[in] dev_inst   Pointer to the software instance struct
 * \param[out] data      Pointer to store the received data
 */
static inline spi_read(struct spi_dev_inst *dev_inst, uint8_t *rx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_t *const spi_module = dev_inst->hw_dev;

	// Wait until the RX complete interrupt flag is set
	while (!spi_module->SPI->INTFLAGS & SPI_RXCIF_bm);

	*rx_data = spi_module->SPI->DATA;
}

static inline spi_read_block(struct spi_dev_inst *const dev_inst,
		uint8_t *rx_data, uint8_t length);

/**
 * \brief Transfer and receive a single byte via SPI
 *
 * This function will transfer a single byte via SPI and return the byte that
 * is shifted into the SPI shift register.
 *
 * In master mode the byte will be sent immediately and the received byte will
 * be read as soon as the shifting of the byte is complete.
 *
 * In slave mode this function will place the data to be sent into the transmit
 * buffer. It will then block until an SPI master has shifted a complete byte,
 * and the received data is available.
 *
 * \param[in] dev_inst    Pointer to the software instance struct
 * \param[in] tx_data     Byte to transmit
 * \param[out] rx_data    Pointer to store the received byte
 */
static inline void spi_tranceive(struct spi_dev_inst *const dev_inst,
		uint8_t tx_data, uint8_t *rx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	/* Write data */
	spi_write(dev_inst, tx_data);
	/* Read data */
	spi_read(dev_inst, rx_data);
}

enum status_code spi_tranceive_block(struct spi_dev_inst *const dev_inst,
		uint8_t *tx_data, uint8_t *rx_data, uint8_t length);

/** @} */


#ifdef __cplusplus
}
#endif

/** @} */


/**
 * \page spi_extra_info Extra Information
 *
 * \section acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *	<tr>
 *		<th>Acronym</th>
 *		<th>Description</th>
 *	</tr>
 *	<tr>
 *		<td>SPI</td>
 *		<td>Serial Peripheral Interface</td>
 *	</tr>
 *	<tr>
 *		<td>SCK</td>
 *		<td>Serial Clock</td>
 *	</tr>
 *	<tr>
 *		<td>MOSI</td>
 *		<td>Master Output Slave Input</td>
 *	</tr>
 *	<tr>
 *		<td>MISO</td>
 *		<td>Master Input Slave Output</td>
 *	</tr>
 *	<tr>
 *		<td>SS</td>
 *		<td>Slave Select</td>
 *	</tr>
 *	<tr>
 *		<td>DIO</td>
 *		<td>Data Input Output</td>
 *	</tr>
 *	<tr>
 *		<td>DO</td>
 *		<td>Data Output</td>
 *	</tr>
 *	<tr>
 *		<td>DI</td>
 *		<td>Data Input</td>
 *	</tr>
 * </table>
 *
 * \section fixed_errata Erratas fixed by driver
 * No errata workarounds in driver. 
 *
 * \section module_history Module History
 * Below is an overview of the module history, detailing enhancements and fixes
 * made to the module since its first release. The current version of this
 * corresponds to the newest version listed in the table below.
 *
 * <table>
 *	<tr>
 *		<th>Changelog</th>
 *	</tr>
 *	<tr>
 *		<td>Initial Release</td>
 *	</tr>
 * </table>
 */

/**
 * \page spi_quickstart Quick Start Guides for the SPI module
 *
 * This is the quick start guide list for the \ref sam0_spi_group module, with
 * step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function of the
 * user application and run at system startup, while the steps for usage can be
 * copied into the normal user application program flow.
 *
 * \see General list of module \ref module_examples "examples".
 *
 * \section spi_use_cases SPI driver use cases
 * - \subpage spi_basic_use_case
 */

#endif /* _SPI_H_INCLUDED_ */
