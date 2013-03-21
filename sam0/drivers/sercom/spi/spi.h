/**
 * \file
 *
 * \brief SAMD20 Serial Peripheral Interface Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

/**
 * \defgroup asfdoc_samd20_sercom_spi_group SAMD20 Serial Peripheral Interface Driver (SERCOM SPI)
 *
 * This driver for SAMD20 devices provides an interface for the configuration
 * and management of the SERCOM module in its SPI mode to transfer SPI  data
 * frames. The following driver API modes are covered by this manual:
 *
 * - Polled APIs
 * \if SPI_CALLBACK_MODE
 * - Callback APIs
 * \endif
 *
 * The following peripherals are used by this module:
 *
 * - SERCOM (Serial Communication Interface)
 *
 * The outline of this documentation is as follows:
 * - \ref asfdoc_samd20_sercom_spi_prerequisites
 * - \ref asfdoc_samd20_sercom_spi_module_overview
 * - \ref asfdoc_samd20_sercom_spi_special_considerations
 * - \ref asfdoc_samd20_sercom_spi_extra_info
 * - \ref asfdoc_samd20_sercom_spi_examples
 * - \ref asfdoc_samd20_sercom_spi_api_overview
 *
 * \section asfdoc_samd20_sercom_spi_prerequisites Prerequisites
 * There are no prerequisites.
 *
 *
 * \section asfdoc_samd20_sercom_spi_module_overview Module Overview
 * The Serial Peripheral Interface (SPI) is a high-speed synchronous data
 * transfer interface using three or four pins. It allows fast communication
 * between a master device and one or more peripheral devices.
 *
 * A device connected to the bus must act as a master or a slave. The master
 * initiates and controls all data transactions.
 * The SPI master initiates a communication cycle by pulling low the Slave
 * Select (SS) pin of the desired slave. The Slave Select pin is active low.
 * Master and slave prepare data to be sent in their respective shift
 * registers, and the master generates the required clock pulses on the SCK
 * line to interchange data. Data is always shifted from master to slave on
 * the Master Out - Slave In (MOSI) line, and from slave to master on the
 * Master In - Slave Out (MISO) line. After each data transfer, the master can
 * synchronize to the slave by pulling the SS line high.
 *
 * \subsection asfdoc_samd20_sercom_spi_bus SPI Bus Connection
 * In the figure below, the connection between one master and one slave is
 * shown.
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
 * - \b MOSI Master Input, Slave Output. The line where the data is shifted
 *   out from the master and in to the slave.
 * - \b MISO Master Output Slave Input. The line where the data is shifted
 *   out from the slave and in to the master.
 * - \b SCK Serial Clock. Generated by the master device.
 * - \b SS Slave Select. To initiate a transaction, the master must pull this
 *      line low.
 *
 * If the bus consists of several SPI slaves, they can be connected in parallel
 * and the SPI master can use general I/O pins to control separate SS lines to
 * each slave on the bus.
 *
 * It is also possible to connect all slaves in series. In this configuration,
 * a common SS is provided to \c N slaves, enabling them simultaneously. The
 * MISO from the \c N-1 slaves is connected to the MOSI on the next slave. The
 * \c Nth slave connects its MISO back to the master. For a
 * complete transaction, the master must shift \c N+1 characters.
 *
 * \subsection asfdoc_samd20_sercom_spi_chsize SPI Character Size
 * The SPI character size is configurable to 8 or 9 bits.
 *
 * \subsection asfdoc_samd20_sercom_spi_master_mode Master Mode
 * When configured as a master, the SS pin will be configured as an output..
 *
 * \subsubsection asfdoc_samd20_sercom_spi_master_mode_data_transfer Data Transfer
 * Writing a character will start the SPI clock generator, and
 * the character is transferred to the shift register when the shift
 * register is empty.
 * Once this is done, a new character can be written.
 * As each character is shifted out from the master, a character is shifted in
 * from the slave. If the receiver is enabled, the data is moved to the receive
 * buffer at the completion of the frame and can be read.
 *
 * \subsection asfdoc_samd20_sercom_spi_slave_mode Slave Mode
 * When configured as a slave, the SPI interface will remain inactive with MISO
 * tri-stated as long as the SS pin is driven high.
 *
 * \subsubsection asfdoc_samd20_sercom_spi_slave_mode_data_transfer_slave Data Transfer
 * The data register can be updated at any time.
 * As the SPI slave shift register is clocked by SCK, a minimum of three SCK
 * cycles are needed from the time new data is written, until the character is
 * ready to be shifted out. If the shift register has not been loaded with
 * data, the current contents will be transmitted.
 *
 * If constant transmission of data is needed in SPI slave mode, the system
 * clock should be faster than SCK.
 * If the receiver is enabled, the received character can be read from the.
 * When SS line is driven high, the slave will not receive any additional data.
 *
 * \subsubsection asfdoc_samd20_sercom_spi_slave_mode_addr_recognition Address Recognition
 * When the SPI slave is configured with address recognition, the first
 * character in a transaction is checked for an address match. If there is a
 * match, the MISO output is enabled and the transaction is processed.
 * If the address does not match, the complete transaction is ignored.
 *
 * If the device is asleep, it can be woken up by an address match in order
 * to process the transaction.
 *
 * \note In master mode, an address packet is written by the
 *       \ref spi_select_slave function if the address_enabled configuration is
 *       set in the \ref spi_slave_inst_config struct.
 *
 * \subsection asfdoc_samd20_sercom_spi_data_modes Data Modes
 * There are four combinations of SCK phase and polarity with respect to
 * serial data. The table below shows the clock polarity (CPOL) and
 * clock phase (CPHA) in the different modes.
 * Leading edge is the first clock edge in a clock cycle and trailing edge is
 * the last clock edge in a clock cycle.
 *
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
 * \subsection asfdoc_samd20_sercom_spi_pads SERCOM Pads
 * The SERCOM pads are automatically configured as seen in the table below.
 * If the receiver is disabled, the data input (MISO for master, MOSI for
 * slave) can be used for other purposes.
 *
 * In master mode, the SS pin(s) must be configured using the \ref spi_slave_inst
 * struct.
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
 * \subsection asfdoc_samd20_sercom_spi_sleep_modes Operation in Sleep Modes
 * The SPI module can operate in all sleep modes by setting the run_in_standby
 * option in the \ref spi_config struct. The operation in slave and master mode
 * is shown in the table below.
 * <table>
 *   <tr>
 *      <th> run_in_standby </th>
 *      <th> Slave </th>
 *      <th> Master </th>
 *   </tr>
 *   <tr>
 *      <th> false </th>
 *      <td> Disabled, all reception is dropped </td>
 *      <td> GCLK disabled when master is idle, wake on transmit complete </td>
 *   </tr>
 *   <tr>
 *      <th> true </th>
 *      <td> Wake on reception </td>
 *      <td> GCLK is enabled while in sleep modes, wake on all interrupts </td>
 *   </tr>
 * </table>
 *
 * \subsection asfdoc_samd20_sercom_spi_clock_generation Clock Generation
 * In SPI master mode, the clock (SCK) is generated internally using the
 * SERCOM baud rate generator. In SPI slave mode, the clock is provided by
 * an external master on the SCK pin. This clock is used to directly clock
 * the SPI shift register.
 *
 * \section asfdoc_samd20_sercom_spi_special_considerations Special Considerations
 * \subsection pin_mux Pin MUX Settings
 * The pin MUX settings must be configured properly, as not all settings
 * can be used in different modes of operation.
 *
 * \section asfdoc_samd20_sercom_spi_extra_info Extra Information
 * For extra information see \ref asfdoc_samd20_sercom_spi_extra. This includes:
 * - \ref asfdoc_samd20_sercom_spi_extra_acronyms
 * - \ref asfdoc_samd20_sercom_spi_extra_dependencies
 * - \ref asfdoc_samd20_sercom_spi_extra_workarounds
 * - \ref asfdoc_samd20_sercom_spi_extra_history
 *
 * \section asfdoc_samd20_sercom_spi_examples Examples
 *
 * The following quick start guides and application examples are available for
 * this driver:
 * - \ref asfdoc_samd20_sercom_spi_basic_use_case
 *
 * \section asfdoc_samd20_sercom_spi_api_overview API Overview
 * @{
 */

#include <port.h>
#include <sercom.h>
#include <pinmux.h>
#include <string.h>

#if SPI_CALLBACK_MODE == true
#  include <sercom_interrupt.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PINMUX_DEFAULT
#  define PINMUX_DEFAULT 0
#endif

#ifndef SPI_TIMEOUT
#  define SPI_TIMEOUT 10000
#endif

#if SPI_CALLBACK_MODE == true
/**
 * \brief SPI Callback enum
 *
 * Callbacks for SPI callback driver
 *
 */
enum spi_callback {
	/** Callback for buffer transmitted */
	SPI_CALLBACK_BUFFER_TRANSMITTED,
	/** Callback for buffer received */
	SPI_CALLBACK_BUFFER_RECEIVED,
	/** Callback for error */
	SPI_CALLBACK_ERROR,
	/** Callback for transmission complete for slave */
	SPI_CALLBACK_SLAVE_TRANSMISSION_COMPLETE,
#  if !defined(__DOXYGEN__)
	/** Number of available callbacks. */
	SPI_CALLBACK_N,
#  endif
};
#endif

#if SPI_CALLBACK_MODE == true
#  if !defined(__DOXYGEN__)
/**
 * \internal SPI transfer directions
 *
 */
enum spi_direction {
	SPI_DIRECTION_READ,
	SPI_DIRECTION_WRITE,
	SPI_DIRECTION_BOTH,
	SPI_DIRECTION_IDLE,
};
#  endif
#endif

/**
 * \brief SPI Interrupt Flags
 *
 * Interrupt flags for the SPI module
 *
 */
enum spi_interrupt_flag {
	/**
	 * This flag is set when the contents of the data register has been moved
	 * to the shift register and the data register is ready for new data
	 */
	SPI_INTERRUPT_FLAG_DATA_REGISTER_EMPTY = SERCOM_SPI_INTFLAG_DREIF,
	/**
	 * This flag is set when the contents of the shift register has been
	 * shifted out
	 */
	SPI_INTERRUPT_FLAG_TX_COMPLETE       = SERCOM_SPI_INTFLAG_TXCIF,
	/** This flag is set when data has been shifted into the data register */
	SPI_INTERRUPT_FLAG_RX_COMPLETE       = SERCOM_SPI_INTFLAG_RXCIF,
};

/**
 * \brief SPI transfer modes enum
 *
 * SPI transfer mode.
 */
enum spi_transfer_mode {
	/** Mode 0. Leading edge: rising, sample. Trailing edge: falling, setup */
	SPI_TRANSFER_MODE_0 = 0,
	/** Mode 1. Leading edge: rising, setup. Trailing edge: falling, sample */
	SPI_TRANSFER_MODE_1 = SERCOM_SPI_CTRLA_CPHA,
	/** Mode 2. Leading edge: falling, sample. Trailing edge: rising, setup */
	SPI_TRANSFER_MODE_2 = SERCOM_SPI_CTRLA_CPOL,
	/** Mode 3. Leading edge: falling, setup. Trailing edge: rising, sample */
	SPI_TRANSFER_MODE_3 = SERCOM_SPI_CTRLA_CPHA | SERCOM_SPI_CTRLA_CPOL,
};

/**
 * \brief SPI frame format enum
 *
 * Frame format for slave mode.
 */
enum spi_frame_format {
	/** SPI frame */
	SPI_FRAME_FORMAT_SPI_FRAME      = SERCOM_SPI_CTRLA_FORM(0),
	/** SPI frame with address */
	SPI_FRAME_FORMAT_SPI_FRAME_ADDR = SERCOM_SPI_CTRLA_FORM(2),
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
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_a
	 */
	SPI_SIGNAL_MUX_SETTING_A =
			(0x0 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x0 << SERCOM_SPI_CTRLA_DIPO_Pos),
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_b
	 */
	SPI_SIGNAL_MUX_SETTING_B =
			(0x0 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x1 << SERCOM_SPI_CTRLA_DIPO_Pos),
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_c
	 */
	SPI_SIGNAL_MUX_SETTING_C =
			(0x0 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x2 << SERCOM_SPI_CTRLA_DIPO_Pos),
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_d
	 */
	SPI_SIGNAL_MUX_SETTING_D =
			(0x0 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x3 << SERCOM_SPI_CTRLA_DIPO_Pos),
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_e
	 */
	SPI_SIGNAL_MUX_SETTING_E =
			(0x1 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x0 << SERCOM_SPI_CTRLA_DIPO_Pos),
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_f
	 */
	SPI_SIGNAL_MUX_SETTING_F =
			(0x1 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x1 << SERCOM_SPI_CTRLA_DIPO_Pos),
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_g
	 */
	SPI_SIGNAL_MUX_SETTING_G =
			(0x1 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x2 << SERCOM_SPI_CTRLA_DIPO_Pos),
	/**
	 * See \ref asfdoc_samd20_sercom_spi_mux_setting_h
	 */
	SPI_SIGNAL_MUX_SETTING_H =
			(0x1 << SERCOM_SPI_CTRLA_DOPO_Pos) |
			(0x3 << SERCOM_SPI_CTRLA_DIPO_Pos),
};

/**
 * \brief SPI address modes enum
 *
 * For slave mode when using the SPI frame with address format.
 *
 */
enum spi_addr_mode {
	/**
	 * addrmask in the \ref spi_config struct is used as a mask to the register.
	 */
	SPI_ADDR_MODE_MASK   = SERCOM_SPI_CTRLB_AMODE(0),
	/**
	 * The slave responds to the two unique addresses in addr and addrmask in
	 * the \ref spi_config struct.
	 */
	SPI_ADDR_MODE_UNIQUE = SERCOM_SPI_CTRLB_AMODE(1),
	/**
	 * The slave responds to the range of addresses between and including addr
	 * and addrmask in the \ref spi_config struct. addr is the upper limit.
	 */
	SPI_ADDR_MODE_RANGE  = SERCOM_SPI_CTRLB_AMODE(2),
};

/**
 * \brief SPI modes enum
 */
enum spi_mode {
	/** Master mode */
	SPI_MODE_MASTER = SERCOM_SPI_CTRLA_MASTER,
	/** Slave mode */
	SPI_MODE_SLAVE = 0,
};

/**
 * \brief SPI data order enum
 */
enum spi_data_order {
	/** The LSB of the data is transmitted first */
	SPI_DATA_ORDER_LSB = SERCOM_SPI_CTRLA_DORD,
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
	SPI_CHARACTER_SIZE_9BIT = SERCOM_SPI_CTRLB_CHSIZE,
};

#if SPI_CALLBACK_MODE == true
/** Prototype for the device instance */
struct spi_module;

/** Type of the callback functions */
typedef void (*spi_callback_t)(const struct spi_module *const module);

#  if !defined(__DOXYGEN__)
/** Prototype for the interrupt handler */
extern void _spi_interrupt_handler(uint8_t instance);
#  endif
#endif

/**
 * \brief SERCOM SPI driver software device instance structure.
 *
 * SERCOM SPI driver software instance structure, used to retain software state
 * information of an associated hardware module instance.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct spi_module {
#if !defined(__DOXYGEN__)
	/** SERCOM hardware module */
	Sercom *hw;
	/** SPI mode */
	enum spi_mode mode;
	/** SPI character size */
	enum spi_character_size character_size;
#  if SPI_CALLBACK_MODE == true
	volatile enum spi_direction dir;
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
	/** Holds the status of the ongoing or last read operation */
	volatile enum status_code rx_status;
	/** Holds the status of the ongoing or last write operation */
	volatile enum status_code tx_status;
#  endif
#endif
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
	bool address_enabled;
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
	/** Frame format */
	enum spi_frame_format frame_format;
	/** Address mode */
	enum spi_addr_mode address_mode;
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
	/** SPI mode */
	enum spi_mode mode;
	/** Data order */
	enum spi_data_order data_order;
	/** Transfer mode */
	enum spi_transfer_mode transfer_mode;
	/** Mux setting */
	enum spi_signal_mux_setting mux_setting;
	/** SPI character size */
	enum spi_character_size character_size;
	/** Enabled in sleep modes */
	bool run_in_standby;
	/** Enable receiver */
	bool receiver_enable;
	/** Union for slave or master specific configuration */
	union {
		/** Slave specific configuration */
		struct spi_slave_config slave;
		/** Master specific configuration */
		struct spi_master_config master;
	}; /**< Union for slave or master specific configuration */
	/** GCLK generator to use as clock source. */
	enum gclk_generator generator_source;
	/** PAD0 pinmux */
	uint32_t pinmux_pad0;
	/** PAD1 pinmux */
	uint32_t pinmux_pad1;
	/** PAD2 pinmux */
	uint32_t pinmux_pad2;
	/** PAD3 pinmux */
	uint32_t pinmux_pad3;
};

#if !defined (__DOXYGEN__)
/**
 * \internal Wait until the synchronization is complete
 */
static inline void _spi_wait_for_sync(
		struct spi_module *const module)
{
	SercomSpi *const spi_module = &(module->hw->SPI);

	/* Wait until the synchronization is complete */
	while (spi_module->STATUS.reg & SERCOM_SPI_STATUS_SYNCBUSY);
}
#endif

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
static inline bool spi_is_syncing(
		struct spi_module *const module)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(module->hw);

	SercomSpi *const spi_module = &(module->hw->SPI);

	/* Return synchronization status */
	return (spi_module->STATUS.reg & SERCOM_SPI_STATUS_SYNCBUSY);
}

/**
 * \name Driver initialization and configuration
 * @{
 */

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
 *  \li \ref asfdoc_samd20_sercom_spi_mux_setting_d
 *  \li Character size 8 bit
 *  \li Not enabled in sleep mode
 *  \li Receiver enabled
 *  \li Baudrate 9600
 *  \li Default pinmux settings for all pads
 *  \li GCLK generator 0
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
static inline void spi_get_config_defaults(
		struct spi_config *const config)
{
	/* Sanity check arguments */
	Assert(config);

	memset(&(config->slave), 0, sizeof(struct spi_slave_config));

	/* Default configuration values */
	config->mode             = SPI_MODE_MASTER;
	config->data_order       = SPI_DATA_ORDER_MSB;
	config->transfer_mode    = SPI_TRANSFER_MODE_0;
	config->mux_setting      = SPI_SIGNAL_MUX_SETTING_D;
	config->character_size   = SPI_CHARACTER_SIZE_8BIT;
	config->run_in_standby   = false;
	config->receiver_enable  = true;
	config->generator_source = GCLK_GENERATOR_0;

	/* Master config defaults */
	config->master.baudrate = 9600;

	/* pinmux config defaults */
	config->pinmux_pad0 = PINMUX_DEFAULT;
	config->pinmux_pad1 = PINMUX_DEFAULT;
	config->pinmux_pad2 = PINMUX_DEFAULT;
	config->pinmux_pad3 = PINMUX_DEFAULT;

};

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
static inline void spi_slave_inst_get_config_defaults(
		struct spi_slave_inst_config *const config)
{
	Assert(config);

	config->ss_pin          = 10;
	config->address_enabled = false;
	config->address         = 0;
}

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
static inline void spi_attach_slave(
		struct spi_slave_inst *const slave,
		struct spi_slave_inst_config *const config)
{
	Assert(slave);
	Assert(config);

	slave->ss_pin          = config->ss_pin;
	slave->address_enabled = config->address_enabled;
	slave->address         = config->address;

	/* Get default config for pin */
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);

	/* Edit config to set the pin as output */
	pin_conf.direction = PORT_PIN_DIR_OUTPUT;

	/* Set config on Slave Select pin */
	port_pin_set_config(slave->ss_pin, &pin_conf);
	port_pin_set_output_level(slave->ss_pin, true);
}

enum status_code spi_init(
		struct spi_module *const module,
		Sercom *const hw,
		const struct spi_config *const config);

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
 * \param[in,out] module  Pointer to the software instance struct
 */
static inline void spi_enable(
		struct spi_module *const module)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(module->hw);

	SercomSpi *const spi_module = &(module->hw->SPI);

#if SPI_CALLBACK_MODE == true
	system_interrupt_enable(_sercom_get_interrupt_vector(module->hw));
#endif

	while (spi_is_syncing(module)) {
		/* Wait until the synchronization is complete */
	}

	/* Enable SPI */
	spi_module->CTRLA.reg |= SERCOM_SPI_CTRLA_ENABLE;
}

/**
 * \brief Disables the SERCOM SPI module
 *
 * This function will disable the SERCOM SPI module.
 *
 * \param[in,out] module  Pointer to the software instance struct
 */
static inline void spi_disable(
		struct spi_module *const module)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(module->hw);

	SercomSpi *const spi_module = &(module->hw->SPI);

#if SPI_CALLBACK_MODE == true
	system_interrupt_disable(_sercom_get_interrupt_vector(module->hw));
#endif

	while (spi_is_syncing(module)) {
		/* Wait until the synchronization is complete */
	}

	/* Disable SPI */
	spi_module->CTRLA.reg &= ~SERCOM_SPI_CTRLA_ENABLE;
}

void spi_reset(
		struct spi_module *const module);

/** @} */

/**
 * \name Ready to write/read
 * @{
 */

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
static inline bool spi_is_write_complete(
		struct spi_module *const module)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(module->hw);

	SercomSpi *const spi_module = &(module->hw->SPI);

	/* Check interrupt flag */
	return (spi_module->INTFLAG.reg & SERCOM_SPI_INTFLAG_TXCIF);
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
static inline bool spi_is_ready_to_write(
		struct spi_module *const module)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(module->hw);

	SercomSpi *const spi_module = &(module->hw->SPI);

	/* Check interrupt flag */
	return (spi_module->INTFLAG.reg & SERCOM_SPI_INTFLAG_DREIF);
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
static inline bool spi_is_ready_to_read(
		struct spi_module *const module)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(module->hw);

	SercomSpi *const spi_module = &(module->hw->SPI);

	/* Check interrupt flag */
	return (spi_module->INTFLAG.reg & SERCOM_SPI_INTFLAG_RXCIF);
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
 * \ref spi_tranceive_wait function. The \ref spi_is_ready_to_write function
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
 * \retval STATUS_OK    If the data was written
 * \retval STATUS_BUSY  If the last write was not completed
 */
static inline enum status_code spi_write(
		struct spi_module *module,
		uint16_t tx_data)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(module->hw);

	SercomSpi *const spi_module = &(module->hw->SPI);

	/* Check if the data register has been copied to the shift register */
	if (!spi_is_ready_to_write(module)) {
		/* Data register has not been copied to the shift register, return */
		return STATUS_BUSY;
	}

	/* Write the character to the DATA register */
	spi_module->DATA.reg = tx_data & SERCOM_SPI_DATA_MASK;

	return STATUS_OK;
}

enum status_code spi_write_buffer_wait(
		struct spi_module *const module,
		const uint8_t *tx_data,
		uint8_t length);

/**
 * \brief Reads last received SPI character
 *
 * This function will return the last SPI character shifted into the receive
 * register.
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
 * \retval STATUS_OK            If data was read
 * \retval STATUS_ERR_IO        If no data is available
 * \retval STATUS_ERR_OVERFLOW  If the data is overflown
 */
static inline enum status_code spi_read(
		struct spi_module *const module,
		uint16_t *rx_data)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(module->hw);

	SercomSpi *const spi_module = &(module->hw->SPI);

	/* Return value */
	enum status_code retval = STATUS_OK;
	/* Check if data is ready to be read */
	if (!spi_is_ready_to_read(module)) {
		/* No data has been received, return */
		return STATUS_ERR_IO;
	}

	/* Check if data is overflown */
	if (spi_module->STATUS.reg & SERCOM_SPI_STATUS_BUFOVF) {
		retval = STATUS_ERR_OVERFLOW;
	}
	/* Read the character from the DATA register */
	if (module->character_size == SPI_CHARACTER_SIZE_9BIT) {
		*rx_data = (spi_module->DATA.reg & SERCOM_SPI_DATA_MASK);
	} else {
		*rx_data = (uint8_t)spi_module->DATA.reg;
	}

	return retval;
}

enum status_code spi_read_buffer_wait(
		struct spi_module *const module,
		uint8_t *rx_data,
		uint8_t length,
		uint16_t dummy);

/**
 * \brief Sends and reads a single SPI character
 *
 * This function will transfer a single SPI character via SPI and return the
 * SPI character that is shifted into the shift register.
 *
 * In master mode the SPI character will be sent immediately and the received
 * SPI character will be read as soon as the shifting of the data is
 * complete.
 *
 * In slave mode this function will place the data to be sent into the transmit
 * buffer. It will then block until an SPI master has shifted a complete
 * SPI character, and the received data is available.
 *
 * \note The data to be sent might not be sent before the next transfer, as
 *       loading of the shift register is dependent on SCK.
 * \note If address matching is enabled for the slave, the first character
 *       received and placed in the buffer will be the address.
 *
 * \param[in]  module   Pointer to the software instance struct
 * \param[in]  tx_data  SPI character to transmit
 * \param[out] rx_data  Pointer to store the received SPI character
 *
 * \return Status of the operation.
 * \retval STATUS_OK            If the operation was completed
 * \retval STATUS_ERR_TIMEOUT   If the operation was not completed within the
 *                              timeout in slave mode.
 * \retval STATUS_ERR_OVERFLOW  If the incoming data is overflown
 */
static inline enum status_code spi_tranceive_wait(
		struct spi_module *const module,
		uint16_t tx_data,
		uint16_t *rx_data)
{
	/* Sanity check arguments */
	Assert(module);

	uint16_t j;
	enum status_code retval = STATUS_OK;

	/* Start timeout period for slave */
	if (module->mode == SPI_MODE_SLAVE) {
		for (j = 0; j <= SPI_TIMEOUT; j++) {
			if (spi_is_ready_to_write(module)) {
				break;
			} else if (j == SPI_TIMEOUT) {
				/* Not ready to write data within timeout period */
				return STATUS_ERR_TIMEOUT;
			}
		}
	}

	/* Wait until the module is ready to write the character */
	while (!spi_is_ready_to_write(module)) {
	}

	/* Write data */
	spi_write(module, tx_data);

	/* Start timeout period for slave */
	if (module->mode == SPI_MODE_SLAVE) {
		for (j = 0; j <= SPI_TIMEOUT; j++) {
			if (spi_is_ready_to_read(module)) {
				break;
			} else if (j == SPI_TIMEOUT) {
				/* Not ready to read data within timeout period */
				return STATUS_ERR_TIMEOUT;
			}
		}
	}

	/* Wait until the module is ready to read the character */
	while (!spi_is_ready_to_read(module)) {
	}

	/* Read data */
	retval = spi_read(module, rx_data);

	return retval;
}

enum status_code spi_tranceive_buffer_wait(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint8_t *rx_data,
		uint8_t length);

enum status_code spi_select_slave(
		struct spi_module *const module,
		struct spi_slave_inst *const slave,
		bool select);

/** @} */

#ifdef __cplusplus
}
#endif

/** @} */


/**
 * \page asfdoc_samd20_sercom_spi_extra Extra Information
 *
 * \section asfdoc_samd20_sercom_spi_extra_acronyms Acronyms
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
 * \section asfdoc_samd20_sercom_spi_extra_dependencies Dependencies
 * The SPI driver has the following dependencies:
 * \li \ref asfdoc_samd20_system_pinmux_group "System Pin Multiplexer Driver"
 *
 *
 * \section asfdoc_samd20_sercom_spi_extra_workarounds Workarounds Implemented by Driver
 * No workarounds in driver.
 *
 * \section asfdoc_samd20_sercom_spi_extra_history Module History
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in the table.
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
 * \page asfdoc_samd20_sercom_spi_quickstart Quick Start Guides for the SPI module
 *
 * This is the quick start guide list for the \ref asfdoc_samd20_sercom_spi_group
 * module, with step-by-step instructions on how to configure and use the driver
 * in a selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function of the
 * user application and run at system startup, while the steps for usage can be
 * copied into the normal user application program flow.
 *
 * \see General list of module \ref asfdoc_samd20_sercom_spi_examples "examples".
 *
 * - \subpage asfdoc_samd20_sercom_spi_basic_use_case
 */

 /**
 * \page asfdoc_samd20_sercom_spi_mux_settings Mux Settings
 * The different options for functionality of the SERCOM pads.
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
 * \section asfdoc_samd20_sercom_spi_mux_setting_a Mux Setting A
 *
 * \li Master mode: Receiver turned off
 * \li Slave mode: Receiver turned off
 * \li Enum: SPI_SIGNAL_MUX_SETTING_A
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pad0 </th>
 *      <th> Pad1 </th>
 *      <th> Pad2 </th>
 *      <th> Pad3 </th>
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
 * \section asfdoc_samd20_sercom_spi_mux_setting_b Mux Setting B
 *
 * \li Master mode: Receiver turned off
 * \li Slave mode: Not applicable
 * \li Enum: SPI_SIGNAL_MUX_SETTING_B
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pad0 </th>
 *      <th> Pad1 </th>
 *      <th> Pad2 </th>
 *      <th> Pad3 </th>
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
 * \section asfdoc_samd20_sercom_spi_mux_setting_c Mux Setting C
 *
 * \li Master mode: No restrictions
 * \li Slave mode: Not applicable
 * \li Enum: SPI_SIGNAL_MUX_SETTING_C
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pad0 </th>
 *      <th> Pad1 </th>
 *      <th> Pad2 </th>
 *      <th> Pad3 </th>
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
 * \section asfdoc_samd20_sercom_spi_mux_setting_d Mux Setting D
 *
 * \li Master mode: No restrictions
 * \li Slave mode: No restrictions
 * \li Enum: SPI_SIGNAL_MUX_SETTING_D
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pad0 </th>
 *      <th> Pad1 </th>
 *      <th> Pad2 </th>
 *      <th> Pad3 </th>
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
 * \section asfdoc_samd20_sercom_spi_mux_setting_e Mux Setting E
 *
 * \li Master mode: No restrictions
 * \li Slave mode: No restrictions
 * \li Enum: SPI_SIGNAL_MUX_SETTING_E
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pad0 </th>
 *      <th> Pad1 </th>
 *      <th> Pad2 </th>
 *      <th> Pad3 </th>
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
 * \section asfdoc_samd20_sercom_spi_mux_setting_f Mux Setting F
 *
 * \li Master mode: No restrictions
 * \li Slave mode: Not applicable
 * \li Enum: SPI_SIGNAL_MUX_SETTING_F
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pad0 </th>
 *      <th> Pad1 </th>
 *      <th> Pad2 </th>
 *      <th> Pad3 </th>
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
 * \section asfdoc_samd20_sercom_spi_mux_setting_g Mux Setting G
 *
 * \li Master mode: Receiver turned off
 * \li Slave mode: Receiver turned off
 * \li Enum: SPI_SIGNAL_MUX_SETTING_G
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pad0 </th>
 *      <th> Pad1 </th>
 *      <th> Pad2 </th>
 *      <th> Pad3 </th>
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
 * \section asfdoc_samd20_sercom_spi_mux_setting_h Mux Setting H
 *
 * \li Master mode: Receiver turned off
 * \li Slave mode: Not applicable
 * \li Enum: SPI_SIGNAL_MUX_SETTING_H
 *
 * <table>
 *   <tr>
 *      <th> Function </th>
 *      <th> Pad0 </th>
 *      <th> Pad1 </th>
 *      <th> Pad2 </th>
 *      <th> Pad3 </th>
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
 */

#endif /* SPI_H_INCLUDED */
