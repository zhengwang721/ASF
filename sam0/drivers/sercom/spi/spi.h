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

#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

#include "asf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup sam0_spi_group SAM0+ Serial Peripheral Interface Driver (SPI)
 *
 * Driver for the SAM0+ architecture devices. This driver provides an
 * interface for configuration and management of the SERCOM SPI module, as well
 * as data transfer via SPI. This driver encompasses the following module
 * within the SAM0+ devices:
 * \li \b SPI \b (Serial Peripheral Interface)
 *
 * \section module_introduction Introduction
 *
 * \subsection module_overview SPI Overview
 * The Serial Peripheral Interface (SPI) is a high-speed synchronous data
 * transfer interface using three or four pins. It allows fast communication
 * between a Master device and one or more peripheral devices.
 * SPI supports both full and half duplex communication.
 * \n\n
 * A device connected to the bus must act as a Master or a Slave. The Master
 * initiates and controls all data transactions.
 * The SPI Master initiates a communication cycle by pulling low the Slave
 * Select (SS) pin of the desired Slave. The Slave Select pin is active low.
 *  Master and Slave prepare data to be sent in their respective Shift
 * Registers, and the Master generates the required clock pulses on the SCK
 * line to interchange data. Data is always shifted from Master to Slave on
 * the Master Out - Slave In (MOSI) line, and from Slave to Master on the
 * Master In - Slave Out (MISO) line. After each data transfer, the Master can
 * synchronize to the Slave by pulling the SS line high.
 *
 * \subsection spi_bus SPI Bus Connection
 * In the figure below, the connection between one Master and one Slave is
 * shown. The SPI lines are hardware controlled, but the port output must be
 * software enabled so that it can be overridden by the SPI.
 * \todo Which port output to enable?
 *
 * For half-duplex mode, the same pin is used for MOSI and MISO.
 *
 * \dot
 * digraph spi_slaves_par {
 *   subgraph cluster_spi_master {
 *    shift_reg [label="Shift register", shape=box];
 *    mosi_m [label="MOSI", shape=none];
 *    miso_m [label="MISO", shape=none];
 *    sck_m [label="SCK", shape=none];
 *    ss_m [label="GPIO pin", shape=none];
 *    {rank=same; mosi_m miso_m sck_m ss_m}
 *    label="SPI Master";
 *   }
 *   subgraph cluster_spi_slave {
 *    mosi_s [label="MOSI", shape=none];
 *    miso_s [label="MISO", shape=none];
 *    sck_s [label="SCK", shape=none];
 *    ss_s [label="SS", shape=none];
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
 *
 * The different lines are as follows:
 * \li \b MOSI Master Input, Slave Output. The line where the data is shifted
 * out from the Master and in to the Slave.
 * \li \b MISO Master Output Slave Input. The line where the data is shifted
 * out from the Slave and in to the Master.
 * \li \b SCK Serial Clock. Generated by the Master device.
 * \li \b SS Slave Select. To initiate a transaction, the Master must pull this
 * line low.
 *
 * If the bus consists of several SPI slaves, they can be connected in parallel
 * and the SPI Master can use general I/O pins to control separate SS lines to
 * each slave on the bus.
 * \n\n
 * It is also possible to connect all slaves in series. In this configuration,
 * a common SS is provided to \c N slaves, enabling them simultaneously. The
 * MISO from the \c N-1 slaves is connected to the MOSI on the next slave. The
 * \c Nth slave connects its MISO back to the Master. For a
 * complete transaction, the Master must shift \c N+1 characters.
 *
 * \subsection chsize SPI Character Size
 * The SPI character size is configurable to 8 or 9 bits.
 *
 * \subsection master_mode Master Mode
 * When configured as a Master, the SS pin must be configured as an output
 * and controlled by user software.
 *
 * \subsubsection data_transfer Data Transfer
 * Writing a character will start the SPI clock generator, and
 * the character is transferred to the shift register when the shift
 * register is empty.
 * Once this is done, a new character can be written.
 * As each character is shifted out from the Master, a character is shifted in
 * from the Slave. If the receiver is enabled, the data is moved to the receive
 * buffer at the completion of the frame and can be read.
 *
 * \subsection slave_mode Slave Mode
 * When configured as a Slave, the SPI interface will remain inactive with MISO
 * tri-stated as long as the SS pin is driven high.
 *
 * \subsubsection data_transfer_slave Data Transfer
 * The data register can be updated at any time.
 * As the SPI Slave shift register is clocked by SCK, a minimum of three SCK
 * cycles are needed from the time new data is written, until the character is
 * ready to be shifted out. If the shift register has not been loaded with
 * data, the current contents will be transmitted.
 * \n\n
 * If constant transmission of data is needed in SPI Slave Mode, the system
 * clock should be faster than SCK.
 * If the receiver is enabled, the received character can be read from the.
 * When SS line is driven high, the Slave will not receive any additional data.
 *
 * \subsubsection addr_recognition Address Recognition
 * When the SPI Slave is configured with address recognition, the first
 * character in a transaction is checked for an address match. If there is a
 * match, the MISO output is enabled and the transaction is processed.
 * If the address does not match, the complete transaction is ignored.
 * \n\n
 * If the device is asleep, it can be woken up by an address match in order
 * to process the transaction.
 *
 * \note In Master Mode, an address packet is written by the
 * \ref spi_select_slave function if the address_enabled configuration is set
 * in the \ref spi_slave_dev_conf struct.
 *
 * \subsection data_modes Data Modes
 * There are four combinations of SCK phase and polarity with respect to
 * serial data. The table below shows the clock polarity (CPOL) and
 * clock phase (CPHA) in the different modes.
 * Leading edge is the first clock edge in a clock cycle and trailing edge is
 * the last clock edge in a clock cycle.
 * <table>
 *   <tr>
 *      <th>Mode</th>
 *      <th>CPOL</th>
 *      <th>CPHA</th>
 *      <th>Leading Edge</th>
 *      <th>Trailing Edge</th>
 *   </tr>
 *   <tr>
 *      <th> 0 </th>
 *      <td> 0 </td>
 *      <td> 0 </td>
 *      <td> Rising, Sample </td>
 *      <td> Falling, Setup </td>
 *   </tr>
 *   <tr>
 *      <th> 1 </th>
 *      <td> 0 </td>
 *      <td> 1 </td>
 *      <td> Rising, Setup </td>
 *      <td> Falling, Sample </td>
 *   </tr>
 *   <tr>
 *      <th> 2 </th>
 *      <td> 1 </td>
 *      <td> 0 </td>
 *      <td> Falling, Sample </td>
 *      <td> Rising, Setup </td>
 *   </tr>
 *   <tr>
 *      <th> 3 </th>
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
 * (MISO for Master, MOSI for Slave) can be used for other purposes.
 *
 * In Master Mode, the SS pin(s) must be configured through port configuration.
 *
 * <table>
 *   <tr>
 *      <th> Pin </th>
 *      <th> Master SPI </th>
 *      <th> Slave SPI </th>
 *   </tr>
 *   <tr>
 *      <th> MOSI </th>
 *      <td> Output </td>
 *      <td> Input </td>
 *   </tr>
 *   <tr>
 *      <th> MISO </th>
 *      <td> Input </td>
 *      <td> Output </td>
 *   </tr>
 *   <tr>
 *      <th> SCK </th>
 *      <td> Output </td>
 *      <td> Input </td>
 *   </tr>
 *   <tr>
 *      <th> SS </th>
 *      <td> User defined output enable </td>
 *      <td> Input </td>
 *   </tr>
 * </table>
 *
 * \subsection half_duplex Half-Duplex
 * \todo remove? marked as extended config, might not be a part of the finished module
 *
 * When enabled as half-duplex SPI, the pins are automatically configured
 * as seen in the table below. In this driver, half duplex is only available
 * in Slave Mode.
 *
 * For half duplex mode, it is up to the software to place MISO and MOSI on the
 * same pin with the mux_setting in the \ref spi_conf struct, as well as ensure that
 * there are never multiple drivers simultaneously enabled on the data line.
 * \n\n
 * A half-duplex SPI transaction always begins with the transmission of \c n
 * characters from Master to Slave. This can optionally be followed by \c n
 * characters from Slave to Master. In this mode, a bus keeper or pullup should
 * be used to hold the DIO (Data Input Output) line in a known state between
 * transactions.
 *
 * <table>
 *   <tr>
 *      <th> Pin </th>
 *      <th> Master SPI </th>
 *      <th> Slave SPI </th>
 *   </tr>
 *   <tr>
 *      <th> DIO </th>
 *      <td> Hardware controlled I/O </td>
 *      <td> Hardware controlled I/O </td>
 *   </tr>
 *   <tr>
 *      <th> SCK </th>
 *      <td> Output </td>
 *      <td> Input </td>
 *   </tr>
 *   <tr>
 *      <th> SS </th>
 *      <td> User defined output enable </td>
 *      <td> Input </td>
 *   </tr>
 * </table>
 *
 * Even though DIO is hardware controlled, the port output must be software
 * enabled so that it can be overridden by the SPI.
 *
 * \dot
 * digraph spi_slaves_par {
 *   subgraph cluster_spi_master {
 *    shift_reg [label="Shift register", shape=box];
 *    dio_m [label="DIO", shape=none];
 *    sck_m [label="SCK", shape=none];
 *    ss_m [label="SS", shape=none];
 *    {rank=same; dio_m sck_m ss_m}
 *    label="SPI Master Half Duplex";
 *   }
 *   subgraph cluster_spi_slave {
 *    dio_s [label="DIO", shape=none];
 *    sck_s [label="SCK", shape=none];
 *    ss_s [label="SS", shape=none];
 *    shift_reg_s [label="Shift register", shape=box];
 *    {rank=same; dio_s sck_s ss_s}
 *    label="SPI Slave Half Duplex";
 *    rankdir=LR;
 *   }
 *   sck_m -> sck_s;
 *   ss_m -> ss_s;
 *   edge [dir=both]
 *   shift_reg:e -> dio_m:w [label=""];
 *   dio_m:e -> dio_s:w [label=""];
 *   dio_s:e -> shift_reg_s:w [label=""];
 *   rankdir=LR;
 * }
 * \enddot
 *
 * \subsection sleep_modes Operation in Sleep Modes
 * The SPI module can operate in all sleep modes by setting the sleep_enable
 * option in the \ref spi_conf struct. The operation in Slave and Master Mode
 * is shown in the table below.
 * <table>
 *   <tr>
 *      <th> sleep_enable </th>
 *      <th> Slave </th>
 *      <th> Master </th>
 *   </tr>
 *   <tr>
 *      <th> false </th>
 *      <td> Disabled, all reception is dropped </td>
 *      <td> GCLK disabled when Master is idle, wake on transmit complete </td>
 *   </tr>
 *   <tr>
 *      <th> true </th>
 *      <td> Wake on reception </td>
 *      <td> GCLK is enabled while in sleep modes, wake on all interrupts </td>
 *   </tr>
 * </table>
 *
 * \subsection clock_generation Clock Generation
 * In SPI Master Mode, the clock (SCK) is generated internally using the
 * SERCOM Baud Rate Generator. In SPI Slave Mode, the clock is provided by
 * an external Master on the SCK pin. This clock is used to directly clock
 * the SPI shift register.
 *
 * \section dependencies Dependencies
 * The SPI driver has the following dependencies:
 * \li \b PORT
 *
 * \section special_cons Special Considerations
 * \subsection pin_mux Pin Mux Settings
 * The pin mux settings must be configured properly, as not all settings
 * can be used in different modes of operation.
 *
 * \section extra_info Extra Information
 * For extra information see \ref spi_extra_info.
 *
 * \section module_examples Examples
 * - \ref quickstart
 *
 * \section api_overview API Overview
 * @{
 */

// TODO: Move this to a spi_conf.h
#define SPI_TIMEOUT 10000

/**
 * \brief SPI transfer modes enum
 *
 * SPI transfer mode.
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
 * Frame format for Slave Mode.
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
 * \brief SPI signal mux settings
 *
 * Set the functionality of the SERCOM pins.
 * As not all settings can be used in different modes of operation, proper
 * settings must be chosen according to the rest of the configuration.
 *
 */
enum spi_signal_mux_setting {
	/**
	 * See \ref mux_setting_a
	 */
	SPI_SIGNAL_MUX_SETTING_A = (SPI_DOPO_PIN0_PIN1_PIN2 | SPI_DIPO_PIN0),
	/**
	 * See \ref mux_setting_b
	 */
	SPI_SIGNAL_MUX_SETTING_B = (SPI_DOPO_PIN0_PIN1_PIN2 | SPI_DIPO_PIN1),
	/**
	 * See \ref mux_setting_c
	 */
	SPI_SIGNAL_MUX_SETTING_C = (SPI_DOPO_PIN0_PIN1_PIN2 | SPI_DIPO_PIN2),
	/**
	 * See \ref mux_setting_d
	 */
	SPI_SIGNAL_MUX_SETTING_D = (SPI_DOPO_PIN0_PIN1_PIN2 | SPI_DIPO_PIN3),
	/**
	 * See \ref mux_setting_e
	 */
	SPI_SIGNAL_MUX_SETTING_E = (SPI_DOPO_PIN2_PIN3_PIN1 | SPI_DIPO_PIN0),
	/**
	 * See \ref mux_setting_f
	 */
	SPI_SIGNAL_MUX_SETTING_F = (SPI_DOPO_PIN2_PIN3_PIN1 | SPI_DIPO_PIN1),
	/**
	 * See \ref mux_setting_g
	 */
	SPI_SIGNAL_MUX_SETTING_G = (SPI_DOPO_PIN2_PIN3_PIN1 | SPI_DIPO_PIN2),
	/**
	 * See \ref mux_setting_h
	 */
	SPI_SIGNAL_MUX_SETTING_H = (SPI_DOPO_PIN2_PIN3_PIN1 | SPI_DIPO_PIN3),
};

/**
 * \brief SPI address modes enum
 *
 * For Slave Mode when using the SPI frame with address format.
 *
 */
enum spi_addr_mode {
	/**
	 * addrmask in the \ref spi_conf struct is used as a mask to the register.
	 */
	SPI_ADDR_MODE_MASK = 0,
	/**
	 * The Slave responds to the two unique addresses in addr and addrmask in
	 * the \ref spi_conf struct.
	 */
	SPI_ADDR_MODE_UNIQUE = SPI_ADDRMODE_UNIQUE_bm,
	/**
	 * The Slave responds to the range of addresses between and including addr
	 * and addrmask in the \ref spi_conf struct. addr is the upper limit.
	 */
	SPI_ADDR_MODE_RANGE = SPI_ADDRMODE_RANGE_bm,
};

/**
 * \brief SPI modes enum
 */
enum spi_mode {
	/** Master Mode */
	SPI_MODE_MASTER = SPI_MASTER_bm,
	/** Slave Mode */
	SPI_MODE_SLAVE = 0,
};

/**
 * \brief SPI data order enum
 */
enum spi_data_order {
	/** The LSB of the data is transmitted first */
	SPI_DATA_ORDER_LSB = SPI_DORD_bm,
	/** The MSB of the data is transmitted first */
	SPI_DATA_ORDER_MSB = 0,
};

/**
 * \brief SPI character size enum
 */
enum spi_character_size {
	/** 8 bit character */
	SPI_CHARACTER_SIZE_8BIT = 0,
	/** 9 bit character */
	SPI_CHARACTER_SIZE_9BIT = SPI_CHSIZE_bm,
};

/**
 * \brief SPI software device instance structure
 *
 * SPI software device instance structure
 *
 */
struct spi_dev_inst {
	/** SERCOM hardware module */
	SERCOM_t *hw_dev;
	/** SPI mode */
	enum spi_mode mode;
	/** SPI character size */
	enum spi_character_size chsize;
};

/**
 * \brief SPI peripheral slave device structure
 *
 * SPI peripheral slave software device instance structure. To be used as
 * parameter to the \ref spi_select_slave and \ref spi_deselect_slave
 * functions.
 *
 */
struct spi_slave_dev_inst {
	/** Pin to use as Slave Select */
	uint8_t ss_pin;
	/** Address recognition enabled in Slave device */
	bool address_enabled;
	/** Address of Slave device */
	uint8_t address;
};

/**
 * \brief SPI Peripheral Slave configuration structure
 *
 * SPI Peripheral Slave configuration structure
 */
struct spi_slave_dev_conf {
	/** Pin to use as Slave Select */
	uint8_t ss_pin;
	/** Enable address */
	bool address_enabled;
	/** Address of Slave */
	uint8_t address;
};

/**
 * \brief SPI Master configuration structure
 *
 * SPI Master configuration structure
 */
struct spi_master_conf {
	/** Baud rate */
	uint32_t baudrate;
};

/**
 * \brief SPI Slave configuration structure
 *
 * SPI Slave configuration structure
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
struct spi_conf {
	/** SPI mode */
	enum spi_mode mode;
	/** Data order */
	enum spi_data_order data_order;
	/** Transfer mode */
	enum spi_transfer_mode transfer_mode;
	/** Mux setting */
	enum spi_signal_mux_setting mux_setting;
	/** SPI character size */
	enum spi_character_size chsize;
	/** Enabled in sleep modes */
	bool sleep_enable;
	/** Enable receiver */
	bool receiver_enable;
	/** Union for Slave or Master specific configuration */
	union {
		/** Slave specific configuration */
		struct spi_slave_conf slave;
		/** Master specific configuration */
		struct spi_master_conf master;
	}; /**< Union for Slave or Master specific configuration */
};

#if !defined (__DOXYGEN__)
/**
 * \internal Wait until the synchronization is complete
 */
static inline void _spi_wait_for_sync(struct spi_dev_inst *const dev_inst)
{
	SERCOM_t *const spi_module = dev_inst->hw_dev;

	/* Wait until the synchronization is complete */
	while (spi_module->SPI.STATUS & SPI_SYNCBUSY_bm);
}
#endif

/**
 * \name Driver initialization and configuration
 * @{
 */

/**
 * \brief Initializes an SPI configuration structure to default values
 *
 *  This function will initialize a given SPI configuration structure to a set
 * of known default values. This function should be called on any new
 *  instance of the configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *   \li Master Mode enabled
 *   \li MSB of the data is transmitted first
 *   \li Transfer mode 0
 *   \li \ref mux_setting_d
 *   \li Character size 8 bit
 *   \li Not enabled in sleep mode
 *   \li Receiver enabled
 *   \li Baudrate 9600
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
static inline void spi_get_config_defaults(struct spi_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->mode = SPI_MODE_MASTER;
	config->data_order = SPI_DATA_ORDER_MSB;
	config->transfer_mode = SPI_TRANSFER_MODE_0;
	config->mux_setting = SPI_SIGNAL_MUX_SETTING_D;
	config->chsize = SPI_CHARACTER_SIZE_8BIT;
	config->sleep_enable = false;
	config->receiver_enable = true;

	/* Master config defaults */
	config->master.baudrate = 9600;

};

/**
 * \brief Initializes an SPI perhipheral slave device configuration structure
 * to default values
 *
 *  This function will initialize a given SPI slave device configuration
 * structure to a set of known default values. This function should be called
 * on any new instance of the configuration structures before being modified by
 * the user application.
 *
 *  The default configuration is as follows:
 *   \li Slave Select on pin 10
 *   \li Addressing not enabled
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
static inline void spi_slave_dev_get_config_defaults(struct spi_slave_dev_conf
		*const config)
{
	Assert(config);

	config->ss_pin = 10;
	config->address_enabled = false;
	config->address = 0;
}

/**
 * \brief Initializes an SPI peripheral slave struct
 *
 * This function will initialize the software SPI peripheral slave, based on
 * the values of the config struct.
 *
 * \param[out] dev_inst   Pointer to the software instance struct
 * \param[in] config      Pointer to the config struct
 *
 */
static inline void spi_slave_dev_init(struct spi_slave_dev_inst *const dev_inst,
		struct spi_slave_dev_conf *const config) {
	Assert(dev_inst);
	Assert(config);

	dev_inst->ss_pin = config->ss_pin;
	dev_inst->address_enabled = config->address_enabled;
	dev_inst->address = config->address;

	struct port_pin_conf pin_conf;

	/* Get default config for pin */
	port_pin_get_config_defaults(&pin_conf);

	/* Edit config to set the pin as output */
	pin_conf.input.enabled = false;
	pin_conf.output.enabled = true;

	/* Set config on Slave Select pin */
	port_pin_set_config(dev_inst->ss_pin, &pin_conf);
}

enum status_code spi_init(struct spi_dev_inst *const dev_inst, SERCOM_t *module,
		struct spi_conf *config);

/** @} */

/**
 * \name Enable/Disable
 * @{
 */

/**
 * \brief Enables the SERCOM SPI module
 *
 * This function will enable the SERCOM SPI module.
 *
 * \param dev_inst    Pointer to the software instance struct
 */
static inline void spi_enable(struct spi_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_t *const spi_module = dev_inst->hw_dev;

	/* Wait until the synchronization is complete */
	_spi_wait_for_sync(dev_inst);

	/* Enable SPI */
	spi_module->SPI.CTRLA |= SPI_ENABLE_bm;
}

/**
 * \brief Disables the SERCOM SPI module
 *
 * This function will disable the SERCOM SPI module.
 *
 * \param dev_inst    Pointer to the software instance struct
 */
static inline void spi_disable(struct spi_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_t *const spi_module = dev_inst->hw_dev;

	/* Wait until the synchronization is complete */
	_spi_wait_for_sync(dev_inst);

	/* Disable SPI */
	spi_module->SPI.CTRLA &= ~SPI_ENABLE_bm;
}

void spi_reset(struct spi_dev_inst *const dev_inst);

/** @} */

/**
 * \name Ready to write/read
 * @{
 */

 /**
 * \brief Checks if the SPI module is ready to write data
 *
 * This function will check if the SPI module is ready to write data.
 *
 * \param[in] dev_inst      Pointer to the software instance struct
 *
 * \return Boolean value to tell whether the module is ready to read data or
 * not
 * \retval true  If the SPI module is ready to write data
 * \retval false If the SPI module is not ready to write data
 */
static inline bool spi_is_ready_to_write(struct spi_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_t *const spi_module = dev_inst->hw_dev;

	/* Check interrupt flag */
	return (spi_module->SPI.INTFLAGS & SPI_TXCIF_bm);
}

/**
 * \brief Checks if the SPI module is ready to read data
 *
 * This function will check if the SPI module is ready to read data.
 *
 * \param[in] dev_inst      Pointer to the software instance struct
 *
 * \return Boolean value to tell whether the module is ready to read data or
 * not
 * \retval true  If the SPI module is ready to read data
 * \retval false If the SPI module is not ready to read data
 */
static inline bool spi_is_ready_to_read(struct spi_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_t *const spi_module = dev_inst->hw_dev;

	/* Wait for synchronization */
	_spi_wait_for_sync(dev_inst);

	/* Disable receiver */
	return (spi_module->SPI.INTFLAGS & SPI_RXCIF_bm);
}
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
 * \ref spi_tranceive function. The \ref spi_is_ready_to_write function should
 * be called before calling this function.
 * Note that this function does not handle the SS (Slave Select)
 * pin(s) in Master Mode; this must be handled from the user application.
 *
 * \note In Slave Mode, the data will not be transferred before a Master
 * initiates a transaction.
 *
 * \param[in] dev_inst    Pointer to the software instance struct
 * \param[in] tx_data     Data to transmit
 *
 * \return Status of the procedure
 * \retval STATUS_OK       If the data was written
 * \retval STATUS_ERR_BUSY If the last write was not completed
 *
 */
static inline enum status_code spi_write(struct spi_dev_inst *dev_inst,
		uint16_t tx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_t *const spi_module = dev_inst->hw_dev;

	/* Check if the data register has been copied to the shift register */
	if (!spi_is_ready_to_write(dev_inst)) {
		/* Data register has not been copied to the shift register, return */
		return STATUS_ERR_BUSY;
	}

	/* Write the character to the DATA register */
	spi_module->SPI.DATA |= tx_data & SPI_DATA_gm;

	return STATUS_OK;
}

enum status_code spi_write_buffer(struct spi_dev_inst
		*const dev_inst, const uint8_t *tx_data, uint8_t length);

/**
 * \brief Reads last received SPI character
 *
 * This function will return the last SPI character shifted into the receive
 * register.
 * The \ref spi_is_ready_to_read function should
 * be called before calling this funtion.
 *
 * \note Receiver must be enabled in the configuration
 *
 * \param[in] dev_inst   Pointer to the software instance struct
 * \param[out] data      Pointer to store the received data
 *
 * \retval STATUS_OK       If data was read
 * \retval STATUS_ERR_IO   If no data is available
 */
static inline enum status_code spi_read(struct spi_dev_inst *const dev_inst,
		uint16_t *rx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	SERCOM_t *const spi_module = dev_inst->hw_dev;

	/* Check until if data is ready to be read */
	if (!spi_is_ready_to_read(dev_inst)) {
		/* No data has been received, return */
		return STATUS_ERR_IO;
	}

	/* Read the character from the DATA register */
	if (dev_inst->chsize == SPI_CHARACTER_SIZE_9BIT) {
		*rx_data = spi_module->SPI.DATA & SPI_DATA_gm;
	} else {
		*(uint8_t*)rx_data = (uint8_t)spi_module->SPI.DATA_UINT8_T[0];
	}

	return STATUS_OK;
}

enum status_code spi_read_buffer(struct spi_dev_inst *const dev_inst,
		uint8_t *rx_data, uint8_t length, uint16_t dummy);

/**
 * \brief Sends and reads a single SPI character
 *
 * This function will transfer a single SPI character via SPI and return the
 * SPI character that is shifted into the shift register.
 *
 * In Master Mode the SPI character will be sent immediately and the received
 * SPI character will be read as soon as the shifting of the data is
 * complete.
 *
 * In Slave Mode this function will place the data to be sent into the transmit
 * buffer. It will then block until an SPI Master has shifted a complete
 * SPI character, and the received data is available.
 *
 * \param[in] dev_inst    Pointer to the software instance struct
 * \param[in] tx_data     SPI character to transmit
 * \param[out] rx_data    Pointer to store the received SPI character
 *
 * \return Status of the operation
 * \retval STATUS_OK           If the operation was completed
 * \retval STATUS_ERR_TIMEOUT  If the operation was not completed within the
 *                             timeout in slave mode.
 */
static inline enum status_code spi_tranceive(struct spi_dev_inst *const dev_inst,
		uint16_t tx_data, uint16_t *rx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	uint16_t j;

	/* Start timeout period for slave */
	if (dev_inst->mode == SPI_MODE_SLAVE) {
		for (j = 0; j <= 10000; j++) {
			if (spi_is_ready_to_write(dev_inst)) {
				break;
			} else if (j == 10000) {
				/* Not ready to write data within timeout period */
				return STATUS_ERR_TIMEOUT;
			}
		}
	}
	/* Wait until the module is ready to write the character */
	while (!spi_is_ready_to_write(dev_inst)) {
	}
	/* Write data */
	spi_write(dev_inst, tx_data);

	/* Start timeout period for slave */
	if (dev_inst->mode == SPI_MODE_SLAVE) {
		for (j = 0; j <= 10000; j++) {
			if (spi_is_ready_to_read(dev_inst)) {
				break;
			} else if (j == 10000) {
				/* Not ready to read data within timeout period */
				return STATUS_ERR_TIMEOUT;
			}
		}
	}

	/* Wait until the module is ready to read the character */
	while (!spi_is_ready_to_read(dev_inst)) {
	}
	/* Read data */
	spi_read(dev_inst, rx_data);

	return STATUS_OK;
}

enum status_code spi_tranceive_buffer(struct spi_dev_inst *const dev_inst,
		uint8_t *tx_data, uint8_t *rx_data, uint8_t length);

 /**
 * \brief Selects slave device
 *
 * This function will drive the slave select pin of the selected device low.
 * If address recognition is enabled, the address will be sent to the slave,
 * so it is ready to receive data.
 *
 * \return Status of the operation
 * \retval STATUS_OK                  If the slave device was selected
 * \retval STATUS_ERR_UNSUPPORTED_DEV If the SPI module is operating in Slave
 *                                    Mode
 */
static inline enum status_code spi_select_slave(struct spi_dev_inst *dev_inst,
		struct spi_slave_dev_inst *slave_dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(slave_dev_inst);

	/* Check that the SPI module is operating in Master Mode */
	if (dev_inst->mode != SPI_MODE_MASTER) {
		return STATUS_ERR_UNSUPPORTED_DEV;
	}

	/* Drive Slave Select low */
	port_pin_set_output_level(slave_dev_inst->ss_pin, false);

	/* Check if address recognition is enabled */
	if (slave_dev_inst->address_enabled) {
		/* Wait until the module is ready to write the address */
		while (!spi_is_ready_to_write(dev_inst)) {
		}
		/* Write address to slave */
		spi_write(dev_inst, slave_dev_inst->address);
	}

	return STATUS_OK;
}

/**
 * \brief Deselects slave device
 *
 * This function will drive the slave select pin of the selected device high.
 *
 * \return Status of the operation
 * \retval STATUS_OK                  If the slave device was deselected
 * \retval STATUS_ERR_UNSUPPORTED_DEV If the SPI module is operating in Slave
 *                                    Mode
 */
static inline enum status_code spi_deselect_slave(struct spi_dev_inst *dev_inst,
		struct spi_slave_dev_inst *slave_dev_inst)
{
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(slave_dev_inst);

	/* Check that the SPI module is operating in Master Mode */
	if (dev_inst->mode != SPI_MODE_MASTER) {
		return STATUS_ERR_UNSUPPORTED_DEV;
	}

	/* Drive Slave Select high */
	port_pin_set_output_level(slave_dev_inst->ss_pin, true);

	return STATUS_OK;
}

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
 * \section workarounds Workarounds implemented by driver
 * No workarounds in driver.
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
 * \page quickstart Quick Start Guides for the SPI module
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

 /**
 * \page mux_settings Mux Settings
 * The different options for functionality of the SERCOM pins.
 * As not all settings can be used in different modes of operation, proper
 * settings must be chosen according to the rest of the configuration.
 *
 * <table>
 *   <tr>
 *      <th> Pin </th>
 *      <th> Master Description </th>
 *      <th> Slave Description </th>
 *   </tr>
 *   <tr>
 *      <td> DO </td>
 *      <td> MOSI </td>
 *      <td> MISO </td>
 *   </tr>
 *   <tr>
 *      <td> DI </td>
 *      <td> MISO </td>
 *      <td> MOSI </td>
 *   </tr>
 *   <tr>
 *      <td> SLAVE_SS </td>
 *      <td> None </td>
 *      <td> Slave Select </td>
 *   </tr>
 *   <tr>
 *      <td> SCK </td>
 *      <td> Serial Clock </td>
 *      <td> Serial Clock </td>
 *   </tr>
 * </table>
 *
 * \section mux_setting_a Mux Setting A
 *
 * \li Master Mode: Receiver turned off
 * \li Slave Mode: Receiver turned off or half duplex
 * \li Enum: SPI_SIGNAL_MUX_SETTING_A
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pin0 </th>
 *      <th> Pin1 </th>
 *      <th> Pin2 </th>
 *      <th> Pin3 </th>
 *   </tr>
 *   <tr>
 *      <th> SCK </th>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> SLAVE_SS </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DO </th>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DI </th>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 * </table>
 *
 * \section mux_setting_b Mux Setting B
 *
 * \li Master Mode: Receiver turned off
 * \li Slave Mode: Not applicable
 * \li Enum: SPI_SIGNAL_MUX_SETTING_B
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pin0 </th>
 *      <th> Pin1 </th>
 *      <th> Pin2 </th>
 *      <th> Pin3 </th>
 *   </tr>
 *   <tr>
 *      <th> SCK </th>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> SLAVE_SS </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DO </th>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DI </th>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 * </table>
 *
 * \section mux_setting_c Mux Setting C
 *
 * \li Master Mode: No restrictions
 * \li Slave Mode: Not applicable
 * \li Enum: SPI_SIGNAL_MUX_SETTING_C
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pin0 </th>
 *      <th> Pin1 </th>
 *      <th> Pin2 </th>
 *      <th> Pin3 </th>
 *   </tr>
 *   <tr>
 *      <th> SCK </th>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> SLAVE_SS </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DO </th>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DI </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *   </tr>
 * </table>
 *
 * \section mux_setting_d Mux Setting D
 *
 * \li Master Mode: No restrictions
 * \li Slave Mode: No restrictions
 * \li Enum: SPI_SIGNAL_MUX_SETTING_D
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pin0 </th>
 *      <th> Pin1 </th>
 *      <th> Pin2 </th>
 *      <th> Pin3 </th>
 *   </tr>
 *   <tr>
 *      <th> SCK </th>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>

 *   </tr>
 *   <tr>
 *      <th> SLAVE_SS </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DO </th>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DI </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *   </tr>
 * </table>
 *
 * \section mux_setting_e Mux Setting E
 *
 * \li Master Mode: No restrictions
 * \li Slave Mode: No restrictions
 * \li Enum: SPI_SIGNAL_MUX_SETTING_E
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pin0 </th>
 *      <th> Pin1 </th>
 *      <th> Pin2 </th>
 *      <th> Pin3 </th>
 *   </tr>
 *   <tr>
 *      <th> SCK </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *   </tr>
 *   <tr>
 *      <th> SLAVE_SS </th>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DO </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DI </th>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 * </table>
 *
 * \section mux_setting_f Mux Setting F
 *
 * \li Master Mode: No restrictions
 * \li Slave Mode: Not applicable
 * \li Enum: SPI_SIGNAL_MUX_SETTING_F
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pin0 </th>
 *      <th> Pin1 </th>
 *      <th> Pin2 </th>
 *      <th> Pin3 </th>
 *   </tr>
 *   <tr>
 *      <th> SCK </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *   </tr>
 *   <tr>
 *      <th> SLAVE_SS </th>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DO </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DI </th>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 * </table>
 *
 * \section mux_setting_g Mux Setting G
 *
 * \li Master Mode: Receiver turned off
 * \li Slave Mode: Receiver turned off or half duplex
 * \li Enum: SPI_SIGNAL_MUX_SETTING_G
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pin0 </th>
 *      <th> Pin1 </th>
 *      <th> Pin2 </th>
 *      <th> Pin3 </th>
 *   </tr>
 *   <tr>
 *      <th> SCK </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *   </tr>
 *   <tr>
 *      <th> SLAVE_SS </th>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DO </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DI </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *   </tr>
 * </table>
 *
 * \section mux_setting_h Mux Setting H
 *
 * \li Master Mode: Receiver turned off
 * \li Slave Mode: Not applicable
 * \li Enum: SPI_SIGNAL_MUX_SETTING_H
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pin0 </th>
 *      <th> Pin1 </th>
 *      <th> Pin2 </th>
 *      <th> Pin3 </th>
 *   </tr>
 *   <tr>
 *      <th> SCK </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *   </tr>
 *   <tr>
 *      <th> SLAVE_SS </th>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DO </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *      <td>  </td>
 *   </tr>
 *   <tr>
 *      <th> DI </th>
 *      <td>  </td>
 *      <td>  </td>
 *      <td>  </td>
 *      <td> x </td>
 *   </tr>
 * </table>
 *
 * Back to \ref spi_signal_mux_setting.
 */

#endif /* SPI_H_INCLUDED */
