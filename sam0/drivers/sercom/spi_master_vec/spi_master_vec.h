/**
 * \file
 *
 * \brief SERCOM SPI master with vectored I/O driver include
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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

#ifndef SPI_MASTER_VEC_H
#define SPI_MASTER_VEC_H

#include <compiler.h>
#include <gclk.h>
#include <port.h>
#include <spi.h>
#include <status_codes.h>
#include <conf_spi_master_vec.h>

/**
 * \defgroup asfdoc_samd20_sercom_spi_master_vec_group SAM D20 Serial Peripheral Interface Master Driver w/ Vectored I/O (SERCOM SPI)
 *
 * This driver for SAM D20 devices provides an interface for the configuration
 * and operation of the SERCOM module in SPI master mode and uses vectored I/O
 * for data transfers.
 *
 * The following peripherals are used by this driver:
 * - SERCOM (Serial Communication Interface)
 *
 *
 * \section asfdoc_samd20_sercom_spi_master_vec_prerequisites Prerequisites
 *
 * This driver uses the \ref asfdoc_samd20_system_clock_group "SYSTEM clock driver"
 * to select the SERCOM's clock source and to configure the SERCOM for the
 * desired baud rate. Ensure that the selected clock source is configured and
 * that the clock system is initialized -- this is typically done indirectly
 * with \ref system_init(), or directly with \ref system_clock_init().
 *
 * According to the datasheet, the minimum and maximum limits for the baud rate
 * is given by: \f$ \frac{1}{2^{17}} \times f_{clk} \le f_{baud}
 * \le \frac{1}{2} \times f_{clk} \f$.
 *
 *
 * \section asfdoc_samd20_sercom_spi_master_vec_module_overview Module Overview
 *
 * This SERCOM SPI master driver supports uni- and bidirectional transfers of
 * 8-bit data with vectored I/O, also know as scatter/gather.
 * It does not implement control of SS or slave addressing since the intended
 * usage is in stacks which usually have their own protocols and handshaking
 * schemes.
 *
 * Vectored I/O enables the transfer of data from/to any number of buffers with
 * arbitrary memory locations without having to do several transfers, i.e., one
 * buffer at a time. This feature is useful in stacks because it allows each
 * layer of the stack to have a dedicated data buffer, thus avoiding the need
 * for a "master" data buffer that the different layers must use in cooperation.
 *
 * The vectored I/O relies on arrays of buffer descriptors which must be passed
 * to the driver to start a transfer. These buffer descriptors specify where in
 * memory each buffer is, and how large they are.
 *
 * \sa spi_master_vec_transceive_buffer_job()
 *
 *
 * \section asfdoc_samd20_sercom_spi_master_vec_special_considerations Special Considerations
 *
 * The SERCOM module has two layers of signal multiplexing in SPI mode:
 * -# SERCOM pad MUX: This routes the SPI signals to internal lines.
 * -# PORT pin MUX: This routes the internal line to a device pin.
 *
 * Both of these layers are configured in the \ref spi_master_vec_config
 * "configuration structure", using the members named \c mux_setting and
 * \c pinmux_padN. These must be set in combination.
 *
 * The driver supplies values for the
 * \ref spi_master_vec_config::mux_setting "SERCOM pad MUX" from the standard
 * ASF SERCOM SPI driver. For the PORT pin MUX configuration, refer to the
 * peripheral include file for the device (\c pio_samd20XNN.h ) and use the
 * macros that are prefixed with \c PINMUX_, such as
 * \c PINMUX_PA04D_SERCOM0_PAD0. It is also possible to use the default pin MUX
 * setting for a SERCOM pad by using the \ref PINMUX_DEFAULT macro. The defaults
 * are defined in the file \ref sercom_pinout.h.
 *
 * Note that for \ref spi_master_vec_init() to function properly with the macro
 * \ref PINMUX_DEFAULT, the order of the values in \c pinmux_padN \e must be
 * correct, i.e., \c pinmux_pad0 must contain the pin MUX setting for
 * multiplexing SERCOM pad 0, and so on.
 *
 *
 * \section asfdoc_samd20_sercom_spi_master_vec_extra_info Extra Information
 *
 * For extra information, see
 * \subpage asfdoc_samd20_sercom_spi_master_vec_extra. This includes:
 * - \ref asfdoc_samd20_sercom_spi_master_vec_extra_acronyms
 * - \ref asfdoc_samd20_sercom_spi_master_vec_extra_dependencies
 * - \ref asfdoc_samd20_sercom_spi_master_vec_extra_errata
 * - \ref asfdoc_samd20_sercom_spi_master_vec_extra_history
 *
 *
 * \section asfdoc_samd20_sercom_spi_master_vec_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_samd20_sercom_spi_master_vec_exqsg.
 *
 *
 * \section asfdoc_samd20_sercom_spi_master_vec_api_overview API Overview
 *
 * @{
 */

/**
 * \brief Driver configuration structure
 *
 * \sa asfdoc_samd20_sercom_spi_master_vec_special_considerations for more
 * information regarding SERCOM pad and pin MUX.
 */
struct spi_master_vec_config {
	/** Baud rate in Hertz. */
	uint32_t baudrate;
	/** GCLK generator to use for the SERCOM. */
	enum gclk_generator gclk_generator;
	/** SERCOM pad MUX setting. */
	enum spi_signal_mux_setting mux_setting;
	/** Transfer mode. */
	enum spi_transfer_mode transfer_mode;
	/** Data order. */
	enum spi_data_order data_order;
	/** Pin MUX setting for SERCOM pad 0. */
	uint32_t pinmux_pad0;
	/** Pin MUX setting for SERCOM pad 1. */
	uint32_t pinmux_pad1;
	/** Pin MUX setting for SERCOM pad 2. */
	uint32_t pinmux_pad2;
	/** Pin MUX setting for SERCOM pad 4. */
	uint32_t pinmux_pad3;
};

/**
 * \name Buffer Description
 * @{
 */

/** Buffer length container. */
typedef uint16_t spi_master_vec_buflen_t;

/** Buffer descriptor structure. */
struct spi_master_vec_bufdesc {
	/** Pointer to buffer start. */
	void *data;
	/** Length of buffer. */
	spi_master_vec_buflen_t length;
};

/** @} */

/** Driver instance. */
struct spi_master_vec_module {
	Sercom *volatile sercom;
	volatile enum _spi_direction direction;
	volatile enum status_code status;
	volatile spi_master_vec_buflen_t rx_length;
	volatile spi_master_vec_buflen_t tx_length;
	uint8_t *volatile rx_head_ptr;
	uint8_t *volatile tx_head_ptr;
	volatile uint_fast8_t tx_lead_on_rx;
	struct spi_master_vec_bufdesc *volatile rx_bufdesc_ptr;
	struct spi_master_vec_bufdesc *volatile tx_bufdesc_ptr;
#ifdef CONF_SPI_MASTER_VEC_OS_SUPPORT
	CONF_SPI_MASTER_VEC_SEMAPHORE_TYPE busy_semaphore;
#endif
};

/**
 * \name Configuration and Initialization
 * @{
 */

/**
 * \brief Initialize configuration with default values.
 *
 * \param[out] config Configuration struct to initialize.
 */
static inline void spi_master_vec_get_config_defaults(
		struct spi_master_vec_config *const config)
{
	config->baudrate = 100000;
	config->gclk_generator = GCLK_GENERATOR_0;
	config->mux_setting = SPI_SIGNAL_MUX_SETTING_D;
	config->transfer_mode = SPI_TRANSFER_MODE_0;
	config->data_order = SPI_DATA_ORDER_MSB;
	config->pinmux_pad0 = PINMUX_DEFAULT;
	config->pinmux_pad1 = PINMUX_DEFAULT;
	config->pinmux_pad2 = PINMUX_DEFAULT;
	config->pinmux_pad3 = PINMUX_DEFAULT;
}

#ifdef __cplusplus
extern "C" {
#endif

enum status_code spi_master_vec_init(struct spi_master_vec_module *const module,
		Sercom *const sercom, const struct spi_master_vec_config *const config);

/** @} */

/**
 * \name Enable/Disable and Reset
 * @{
 */

void spi_master_vec_enable(const struct spi_master_vec_module *const module);
void spi_master_vec_disable(struct spi_master_vec_module *const module);
void spi_master_vec_reset(struct spi_master_vec_module *const module);

/** @} */

/**
 * \name Read/Write and Status
 * @{
 */

enum status_code spi_master_vec_transceive_buffer_job(
		struct spi_master_vec_module *const module,
		struct spi_master_vec_bufdesc tx_bufdescs[],
		struct spi_master_vec_bufdesc rx_bufdescs[]);

/**
 * \brief Get current status of transfer.
 *
 * \param[in] module Driver instance to operate on.
 *
 * \return Current status of driver instance.
 * \retval STATUS_OK if idle and previous transfer succeeded.
 * \retval STATUS_BUSY if a transfer is ongoing.
 * \retval <other> if previous transfer failed.
 */
static inline enum status_code spi_master_vec_get_job_status(
		const struct spi_master_vec_module *const module)
{
	return module->status;
}

/**
 * \brief Get status of transfer upon job end.
 *
 * \param[in]  module Driver instance to operate on.
 *
 * \return Current status of driver instance.
 * \retval STATUS_OK if idle and previous transfer succeeded.
 * \retval <other> if previous transfer failed.
 */
static inline enum status_code spi_master_vec_get_job_status_wait(
		const struct spi_master_vec_module *const module)
{
	enum status_code status;

#ifdef CONF_SPI_MASTER_VEC_OS_SUPPORT
	CONF_SPI_MASTER_VEC_TAKE_SEMAPHORE(module->busy_semaphore);
	status = spi_master_vec_get_job_status(module);
	CONF_SPI_MASTER_VEC_GIVE_SEMAPHORE(module->busy_semaphore);
#else
	do {
		status = spi_master_vec_get_job_status(module);
	} while (status == STATUS_BUSY);
#endif

	return status;
}


/**
 * \brief Start vectored I/O transfer, wait for it to end.
 *
 * \param[in,out] module Driver instance to operate on.
 * \param[in] tx_bufdescs address of buffer descriptor array for bytes to
 * transmit.
 * \arg \c NULL if the transfer is a simplex read.
 * \param[in,out] rx_bufdescs address of buffer descriptor array for storing
 * received bytes.
 * \arg \c NULL if the transfer is a simplex write.
 *
 * \return Status of transfer start.
 * \retval STATUS_OK if transfer succeeded.
 * \retval STATUS_BUSY if a transfer was already on-going.
 * \retval <other> if transfer failed.
 */
static inline enum status_code spi_master_vec_transceive_buffer_wait(
		struct spi_master_vec_module *const module,
		struct spi_master_vec_bufdesc tx_bufdescs[],
		struct spi_master_vec_bufdesc rx_bufdescs[])
{
	enum status_code status;

	status = spi_master_vec_transceive_buffer_job(module, tx_bufdescs,
			rx_bufdescs);

	if (status == STATUS_BUSY) {
		return status;
	}

	return spi_master_vec_get_job_status_wait(module);
}

/** @} */

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * \page asfdoc_samd20_sercom_spi_master_vec_extra Extra Information for SERCOM SPI Master Driver w/ Vectored I/O
 *
 *
 * \section asfdoc_samd20_sercom_spi_master_vec_extra_acronyms Acronyms
 *
 * The table below presents the acronyms used in this module.
 *
 * <table>
 *     <tr>
 *         <th>Acronym</td>
 *         <th>Description</td>
 *     </tr>
 *     <tr>
 *         <td>GCLK</td>
 *         <td>Generic Clock</td>
 *     </tr>
 *     <tr>
 *         <td>MISO</td>
 *         <td>Master Input, Slave Output</td>
 *     </tr>
 *     <tr>
 *         <td>MOSI</td>
 *         <td>Master Output, Slave Input</td>
 *     </tr>
 *     <tr>
 *         <td>SCK</td>
 *         <td>Serial Clock</td>
 *     </tr>
 *     <tr>
 *         <td>SPI</td>
 *         <td>Serial Peripheral Interface</td>
 *     </tr>
 *     <tr>
 *         <td>SS</td>
 *         <td>Slave Select</td>
 *     </tr>
 * </table>
 *
 *
 * \section asfdoc_samd20_sercom_spi_master_vec_extra_dependencies Dependencies
 *
 * This driver has the following dependencies:
 * - \ref asfdoc_samd20_system_group "System driver"
 * - \ref group_sam0_utils "Compiler driver"
 * - \ref asfdoc_samd20_port_group "Port driver"
 * - \ref Common SERCOM driver base
 *
 *
 * \section asfdoc_samd20_sercom_spi_master_vec_extra_errata Errata
 *
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_samd20_sercom_spi_master_vec_extra_history Module History
 *
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in the
 * table.
 *
 * <table>
 *     <tr>
 *         <th>Changelog</th>
 *     </tr>
 *     <tr>
 *         <td>Initial Release</td>
 *     </tr>
 * </table>
 */

/**
 * \page asfdoc_samd20_sercom_spi_master_vec_exqsg Examples for SERCOM SPI Master Driver w/ Vectored I/O
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_samd20_sercom_spi_master_vec_group. QSGs are
 * simple examples with step-by-step instructions to configure and use this
 * driver in a selection of use cases. Note that QSGs can be compiled as a
 * standalone application or be added to the user application.
 *
 * - \subpage asfdoc_samd20_sercom_spi_master_vec_basic
 */

 /**
  * \page asfdoc_samd20_sercom_spi_master_vec_document_revision_history Document Revision History
  *
  * <table>
  *     <tr>
  *         <th>Doc. Rev.</td>
  *         <th>Date</td>
  *         <th>Comments</td>
  *	    </tr>
  *	    <tr>
  *         <td>A</td>
  *         <td>XX/2013</td>
  *         <td>Initial release</td>
  *     </tr>
  * </table>
  */

#endif /* SPI_MASTER_VEC_H */