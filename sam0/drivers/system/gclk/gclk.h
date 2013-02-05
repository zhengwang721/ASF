/**
 * \file
 *
 * \brief SAMD20 Generic Clock Driver
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
#ifndef SYSTEM_CLOCK_GCLK_H_INCLUDED
#define SYSTEM_CLOCK_GCLK_H_INCLUDED

/**
 * \defgroup asfdoc_samd20_gclk_group SAMD20 Generic Clock Driver (GCLK)
 *
 * This driver for SAMD20 devices provides an interface for the configuration
 * and management of the device's Generic Clock generators and channels within
 * the device, including the enabling, disabling, clock source selection and
 * prescaling of clocks to various modules within the device.
 *
 * The following peripherals are used by this module:
 *
 *  - GCLK (Generic Clock Management)
 *
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_samd20_gclk_prerequisites
 *  - \ref asfdoc_samd20_gclk_module_overview
 *  - \ref asfdoc_samd20_gclk_special_considerations
 *  - \ref asfdoc_samd20_gclk_extra_info
 *  - \ref asfdoc_samd20_gclk_examples
 *  - \ref asfdoc_samd20_gclk_api_overview
 *
 *
 * \section asfdoc_samd20_gclk_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_samd20_gclk_module_overview Module Overview
 *
 * Within the SAMD20 devices are a number of Generic Clocks; these are used to
 * provide clocks to the various peripheral clock domains in the device in a
 * standardized manner. One or more master source clocks can be selected as the
 * input clock to a Generic Clock Generator, which can prescale down the input
 * frequency to a slower rate for use in a peripheral.
 *
 * Additionally, a number of individually selectable Generic Clock Channels are
 * provided, which multiplex and gate the various generator outputs for one or
 * more peripherals within the device. This setup allows for a single common
 * generator to feed one or more channels, which can then be enabled or disabled
 * individually as required.
 *
 * \dot
 * digraph overview {
 *   rankdir=LR;
 *   node [label="Clock\nSource a" shape=square] system_clock_source;
 *   node [label="Generator b" shape=square] clock_gen;
 *   node [label="Channel x" shape=square] clock_chan0;
 *   node [label="Channel y" shape=square] clock_chan1;
 *   node [label="Peripheral x" shape=ellipse  style=filled fillcolor=lightgray] peripheral0;
 *   node [label="Peripheral y" shape=ellipse  style=filled fillcolor=lightgray] peripheral1;
 *
 *   system_clock_source -> clock_gen;
 *   clock_gen   -> clock_chan0;
 *   clock_chan0 -> peripheral0;
 *   clock_gen   -> clock_chan1;
 *   clock_chan1 -> peripheral1;
 * }
 * \enddot
 *
 * An example setup of a complete clock chain within the device is shown below.
 *
 * \dot
 * digraph overview {
 *   rankdir=LR;
 *   node [label="External\nOscillator" shape=square] system_clock_source0;
 *   node [label="Generator a" shape=square] clock_gen0;
 *   node [label="Channel x" shape=square] clock_chan0;
 *   node [label="Core CPU" shape=ellipse  style=filled fillcolor=lightgray] peripheral0;
 *
 *   system_clock_source0 -> clock_gen0;
 *   clock_gen0    -> clock_chan0;
 *   clock_chan0   -> peripheral0;
 *
 *   node [label="8MHz R/C\nOscillator" shape=square style=outline] system_clock_source1;
 *   node [label="Generator b" shape=square] clock_gen1;
 *   node [label="Channel y" shape=square] clock_chan1;
 *   node [label="Channel z" shape=square] clock_chan2;
 *   node [label="SERCOM\nModule" shape=ellipse  style=filled fillcolor=lightgray] peripheral1;
 *   node [label="Timer\nModule" shape=ellipse  style=filled fillcolor=lightgray] peripheral2;
 *
 *   system_clock_source1 -> clock_gen1;
 *   clock_gen1    -> clock_chan1;
 *   clock_gen1    -> clock_chan2;
 *   clock_chan1   -> peripheral1;
 *   clock_chan2   -> peripheral2;
 * }
 * \enddot
 *
 * \subsection asfdoc_samd20_gclk_module_overview_source_clocks Source Clocks
 * A number of master clock sources are provided within the chip; these are
 * based on a number of internal and external clock systems, such as
 * R/C Oscillators, external crystals, PLL modules and DFLL modules. These clock
 * sources can each be individually selected as the input for a Generic Clock
 * Generator unit.
 *
 * \subsection asfdoc_samd20_gclk_module_overview_gen_clock_generators Generic Clock Generators
 * Each Generic Clock generator within the device can source its input clock
 * from one of the provided Source Clocks, and prescale the output for one or
 * more Generic Clock Channels in a one-to-many relationship. The generators
 * thus allow for several clocks to be generated of different frequencies,
 * power usages and accuracies, which can be turned on and off individually to
 * disable the clocks to multiple peripherals as a group.
 *
 * \subsection asfdoc_samd20_gclk_module_overview_gen_clock_channels Generic Clock Channels
 * To connect a Generic Clock Generator to a peripheral module within the
 * device, a Generic Clock Channel module must be used. Each peripheral or
 * peripheral group has an associated Generic Clock Channel, which serves as the
 * clock input for the peripheral(s). To supply a clock to the peripheral
 * module(s), the associated channel must be connected to a running Generic
 * Clock Generator and the channel enabled.
 *
 *
 * \subsection asfdoc_samd20_gclk_module_overview_physical Physical Connection
 *
 * The following diagram shows how this module is interconnected within the device:
 *
 * \dot
 * digraph overview {
 *   rankdir=LR;
 *   node [label="Clock\nSources" shape=square] system_clock_sources;
 *
 *   subgraph driver {
 *     node [label="<f0> GCLK | <f1> Generic Clock Generators" shape=record] gclk_gen;
 *     node [label="<f0> GCLK | <f1> Generic Clock Channels" shape=record] gclk_chan;
 *     node [label="Peripherals" shape=ellipse style=filled fillcolor=lightgray] peripherals;
 *   }
 *
 *   system_clock_sources -> gclk_gen:f1 [label="Source\nMUXs"];
 *   gclk_gen:f1   -> gclk_chan:f1 [label="Generator\nMUXs"];
 *   gclk_chan:f1  -> peripherals;
 * }
 * \enddot
 *
 *
 * \section asfdoc_samd20_gclk_special_considerations Special Considerations
 *
 * There are no special considerations for this module.
 *
 *
 * \section asfdoc_samd20_gclk_extra_info Extra Information for GCLK
 *
 * For extra information see \ref asfdoc_samd20_gclk_extra. This includes:
 *  - \ref asfdoc_samd20_gclk_extra_acronyms
 *  - \ref asfdoc_samd20_gclk_extra_dependencies
 *  - \ref asfdoc_samd20_gclk_extra_errata
 *  - \ref asfdoc_samd20_gclk_extra_history
 *
 *
 * \section asfdoc_samd20_gclk_examples Examples
 *
 * The following Quick Start guides and application examples are available for this driver:
 * - \ref asfdoc_samd20_gclk_basic_use_case
 *
 *
 * \section asfdoc_samd20_gclk_api_overview API Overview
 * @{
 */

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Generic Clock Generator configuration structure.
 *
 * Configuration structure for a Generic Clock Generator channel. This
 * structure should be initialized by the
 * \ref system_gclk_gen_get_config_defaults() function before being modified by
 * the user application.
 */
struct system_gclk_gen_conf {
	/** Source clock input channel index. */
	uint8_t source_clock;
	/** If \c true, the generator output level is high when disabled. */
	bool high_when_disabled;
	/** Integer division factor of the clock output compared to the input. */
	uint32_t division_factor;
	/** If \c true the clock is kept enabled during device standby mode. */
	bool run_in_standby;
	/** If \c true, enables GCLK generator clock output to a GPIO pin. */
	bool output_enable;
};

/**
 * \brief Generic Clock configuration structure.
 *
 * Configuration structure for a Generic Clock channel. This structure
 * should be initialized by the \ref system_gclk_ch_get_config_defaults()
 * function before being modified by the user application.
 */
struct system_gclk_ch_conf {
	/** Generic Clock Generator source channel. */
	uint8_t source_generator;
	/** If \c true, the clock is kept enabled during device standby mode. */
	bool run_in_standby;
};


/** \name Configuration and initialization
 * @{
 */

void system_gclk_init(void);

/** @} */


/**
 * \name Configuration and initialization (Generators)
 * @{
 */

/**
 * \brief Initializes a Generic Clock Generator configuration structure to defaults.
 *
 * Initializes a given Generic Clock Generator configuration structure to
 * a set of known default values. This function should be called on all
 * new instances of these configuration structures before being modified
 * by the user application.
 *
 * The default configuration is as follows:
 *  \li Clock is generated undivided from the source frequency
 *  \li Clock generator output is low when the generator is disabled
 *  \li The input clock is sourced from input clock channel 0
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
static inline void system_gclk_gen_get_config_defaults(
		struct system_gclk_gen_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->division_factor    = 1;
	config->high_when_disabled = false;
	config->source_clock       = 0;
	config->run_in_standby     = false;
	config->output_enable      = false;
}

void system_gclk_gen_set_config(
		const uint8_t generator,
		struct system_gclk_gen_conf *const config);

void system_gclk_gen_enable(
		const uint8_t generator);

void system_gclk_gen_disable(
		const uint8_t generator);

/** @} */


/**
 * \name Configuration and initialization (Channels)
 * @{
 */

/**
 * \brief Initializes a Generic Clock configuration structure to defaults.
 *
 * Initializes a given Generic Clock configuration structure to a set of
 * known default values. This function should be called on all new
 * instances of these configuration structures before being modified by the
 * user application.
 *
 * The default configuration is as follows:
 *  \li Clock is sourced from the Generic Clock Generator channel 0
 *  \li Clock is disabled during device sleep
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
static inline void system_gclk_ch_get_config_defaults(
		struct system_gclk_ch_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->source_generator = 0;

	#if !defined (REVB)
	config->run_in_standby = false;
	#endif
}

void system_gclk_ch_set_config(
		const uint8_t channel,
		struct system_gclk_ch_conf *const config);

void system_gclk_ch_enable(
		const uint8_t channel);

void system_gclk_ch_disable(
		const uint8_t channel);

/** @} */


/**
 * \name Clock frequency retrieval
 * @{
 */

uint32_t system_gclk_gen_get_hz(
		const uint8_t generator);

uint32_t system_gclk_ch_get_hz(
		const uint8_t channel);

/** @} */

#ifdef __cplusplus
}
#endif

/** @} */

/**
 * \page asfdoc_samd20_gclk_extra Extra Information
 *
 * \section asfdoc_samd20_gclk_extra_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *	<tr>
 *		<th>Acronym</th>
 *		<th>Description</th>
 *	</tr>
 *	<tr>
 *		<td>DFLL</td>
 *		<td>Digital Frequency Locked Loop</td>
 *	</tr>
 *	<tr>
 *		<td>MUX</td>
 *		<td>Multiplexer</td>
 *	</tr>
 *	<tr>
 *		<td>PLL</td>
 *		<td>Phase Locked Loop</td>
 *	</tr>
 *	<tr>
 *		<td>R/C Oscillator</td>
 *		<td>Resistor/Capacitor Oscillator</td>
 *	</tr>
 * </table>
 *
 *
 * \section asfdoc_samd20_gclk_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - None
 *
 *
 * \section asfdoc_samd20_gclk_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_samd20_gclk_extra_history Module History
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
 *		<td>Initial Release</td>
 *	</tr>
 * </table>
 */

/**
 * \page asfdoc_samd20_gclk_exqsg Examples for GCLK Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_samd20_gclk_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_samd20_gclk_basic_use_case
 */

#endif
