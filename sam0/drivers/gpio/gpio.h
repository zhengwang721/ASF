/**
 * \file
 *
 * \brief SAM GPIO GPIO Driver for SAMB11
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
#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

/**
 * \defgroup asfdoc_sam0_port_group SAM GPIO Driver (GPIO)
 *
 * This driver for Atmel&reg; | SMART SAM devices provides an interface for the configuration
 * and management of the device's General Purpose Input/Output (GPIO) pin
 * functionality, for manual pin state reading and writing.
 *
 * The following peripherals are used by this module:
 *  - GPIO (GPIO Management)
 *
 * The following devices can use this module:
 *  - Atmel | SMART SAM B11
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_sam0_port_prerequisites
 *  - \ref asfdoc_sam0_port_module_overview
 *  - \ref asfdoc_sam0_port_special_considerations
 *  - \ref asfdoc_sam0_port_extra_info
 *  - \ref asfdoc_sam0_port_examples
 *  - \ref asfdoc_sam0_port_api_overview
 *
 *
 * \section asfdoc_sam0_port_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_sam0_port_module_overview Module Overview
 *
 * The device GPIO (GPIO) module provides an interface between the user
 * application logic and external hardware peripherals, when general pin state
 * manipulation is required. This driver provides an easy-to-use interface to
 * the physical pin input samplers and output drivers, so that pins can be read
 * from or written to for general purpose external hardware control.
 *
 * \subsection asfdoc_sam0_port_features Driver Feature Macro Definition
 *
 * \subsection asfdoc_sam0_port_module_overview_pin_numbering Physical and Logical GPIO Pins
 *
 * \subsection asfdoc_sam0_port_module_overview_physical Physical Connection
 *
 * \ref asfdoc_sam0_port_module_int_connections "The diagram below" shows how
 * this module is interconnected within the device.
 *
 * \anchor asfdoc_sam0_port_module_int_connections
 * \dot
 * digraph overview {
 *   node [label="Port Pad" shape=square] pad;
 *
 *   subgraph driver {
 *     node [label="Peripheral MUX" shape=trapezium] pinmux;
 *     node [label="GPIO Module" shape=ellipse] gpio;
 *     node [label="Other Peripheral Modules" shape=ellipse style=filled fillcolor=lightgray] peripherals;
 *   }
 *
 *   pinmux -> gpio;
 *   pad    -> pinmux;
 *   pinmux -> peripherals;
 * }
 * \enddot
 *
 *
 * \section asfdoc_sam0_port_special_considerations Special Considerations
 *
 * The SAM port pin input sampler can be disabled when the pin is configured
 * in pure output mode to save power; reading the pin state of a pin configured
 * in output-only mode will read the logical output state that was last set.
 *
 * \section asfdoc_sam0_port_extra_info Extra Information
 *
 * For extra information, see \ref asfdoc_sam0_port_extra. This includes:
 *  - \ref asfdoc_sam0_port_extra_acronyms
 *  - \ref asfdoc_sam0_port_extra_dependencies
 *  - \ref asfdoc_sam0_port_extra_errata
 *  - \ref asfdoc_sam0_port_extra_history
 *
 *
 * \section asfdoc_sam0_port_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_sam0_port_exqsg.
 *
 *
 * \section asfdoc_sam0_port_api_overview API Overview
 * @{
 */

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \brief GPIO pin direction configuration enum.
 *
 *  Enum for the possible pin direction settings of the port pin configuration
 *  structure, to indicate the direction the pin should use.
 */
enum gpio_pin_dir {
	/** The pin's input buffer should be enabled, so that the pin state can
	 *  be read. */
	GPIO_PIN_DIR_INPUT,
	/** The pin's output buffer should be enabled, so that the pin state can
	 *  be set. */
	GPIO_PIN_DIR_OUTPUT,
};

/**
 *  \brief GPIO pin input pull configuration enum.
 *
 *  Enum for the possible pin pull settings of the GPIO pin configuration
 *  structure, to indicate the type of logic level pull the pin should use.
 */
enum gpio_pin_pull {
	/** No logical pull should be applied to the pin. */
	GPIO_PIN_PULL_NONE,
	/** Pin should be pulled up when idle. */
	GPIO_PIN_PULL_UP ,
#ifdef CHIPVERSION_B0
	/** Pin should be pulled down when idle.*/
	GPIO_PIN_PULL_DOWN,
#endif
};

/**
 *  \brief GPIO pin configuration structure.
 *
 *  Configuration structure for a GPIO pin instance. This structure should be
 *  initialized by the \ref GPIO_get_config_defaults() function before being
 *  modified by the user application.
 */
struct gpio_config {
	/** GPIO buffer input/output direction. */
	enum gpio_pin_dir  direction;

	/** GPIO pull-up/pull-down for input pins. */
	enum gpio_pin_pull input_pull;

	/** Enable lowest possible powerstate on the pin
	 *
	 *  \note All other configurations will be ignored, the pin will be disabled
	 */
	bool powersave;
};

/**
@defgroup port-drv GPIO Driver API

@{
*/

/** \name Configuration and initialization
 * @{
 */

void gpio_get_config_defaults(struct gpio_config *const config);
enum status_code gpio_pin_set_config(const uint8_t gpio_pin,
		const struct gpio_config *config);

/** @} */

/** \name State reading/writing (logical pin orientated)
 * @{
 */

bool gpio_pin_get_input_level(const uint8_t gpio_pin);
bool gpio_pin_get_output_level(const uint8_t gpio_pin);
void gpio_pin_set_output_level(const uint8_t gpio_pin, const bool level);
void gpio_pin_toggle_output_level(const uint8_t gpio_pin);

/** @} */

/** @}*/

#ifdef __cplusplus
}
#endif


/**
 * \page asfdoc_sam0_port_extra Extra Information for GPIO Driver
 *
 * \section asfdoc_sam0_port_extra_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *	<tr>
 *		<th>Acronym</th>
 *		<th>Description</th>
 *	</tr>
 *	<tr>
 *		<td>GPIO</td>
 *		<td>General Purpose Input/Output</td>
 *	</tr>
 * </table>
 *
 *
 * \section asfdoc_sam0_port_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - \ref asfdoc_sam0_system_pinmux_group "System Pin Multiplexer Driver"
 *
 *
 * \section asfdoc_sam0_port_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_sam0_port_extra_history Module History
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in
 * the table.
 *
 * <table>
 *	<tr>
 *		<th>Changelog</th>
 *	</tr>
 *	<tr>
 *		<td>Added input event feature</td>
 *	</tr>
 *	<tr>
 *		<td>Initial Release</td>
 *	</tr>
 * </table>
 */

/**
 * \page asfdoc_sam0_port_exqsg Examples for PORT Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_sam0_port_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_sam0_port_basic_use_case
 *
 * \page asfdoc_sam0_port_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Doc. Rev.</td>
 *		<th>Date</td>
 *		<th>Comments</td>
 *	</tr>
 *	<tr>
 *		<td>A</td>
 *		<td>06/2013</td>
 *		<td>Initial release</td>
 *	</tr>
 * </table>
 */

#endif
