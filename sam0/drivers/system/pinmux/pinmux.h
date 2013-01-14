/**
 * \file
 *
 * \brief SAMD20 Pin Multiplexer Driver
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
#ifndef PINMUX_H_INCLUDED
#define PINMUX_H_INCLUDED

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Peripheral multiplexer index to select GPIO mode for a pin. */
#define SYSTEM_PINMUX_GPIO    (1 << 7)

/** \brief Port pin direction configuration enum.
 *
 * Enum for the possible pin direction settings of the port pin configuration
 * structure, to indicate the direction the pin should use.
 */
enum system_pinmux_pin_dir {
	/** The pin's input buffer should be enabled, so that the pin state can
	 *  be read. */
	SYTEM_PINMUX_DIR_INPUT,
	/** The pin's output buffer should be enabled, so that the pin state can
	 *  be set. */
	SYTEM_PINMUX_DIR_OUTPUT,
	/** The pin's input and buffers should be enabled, so that the pin state can
	 *  be set and the physical state read back. */
	SYTEM_PINMUX_DIR_BOTH,
};

/** \brief Port pin input pull configuration enum.
 *
 * Enum for the possible pin pull settings of the port pin configuration
 * structure, to indicate the type of logic level pull the pin should use.
 */
enum system_pinmux_pin_pull {
	/** No logical pull should be applied to the pin. */
	SYSTEM_PINMUX_PIN_PULL_NONE,
	/** Pin should be pulled up when idle. */
	SYSTEM_PINMUX_PIN_PULL_UP,
	/** Pin should be pulled down when idle. */
	SYSTEM_PINMUX_PIN_PULL_DOWN,
};

/** \brief Port pin digital input sampling mode enum.
 *
 * Enum for the possible input sampling modes for the port pin configuration
 * structure, to indicate the type of sampling a port pin should use.
 */
enum system_pinmux_pin_sampling_mode {
	/** Pin input buffer should be enabled when the IN register is read. */
	SYSTEM_PINMUX_PIN_SAMPLING_ONDEMAND,
	/** Pin input buffer should continuously sample the pin state. */
	SYSTEM_PINMUX_PIN_SAMPLING_CONTINUOUS,
};

/** \brief Port pin drive output strength enum.
 *
 * Enum for the possible output drive strengths for the port pin
 * configuration structure, to indicate the driver strength the pin should
 * use.
 */
enum system_pinmux_pin_strength {
	/** Normal output driver strength. */
	SYSTEM_PINMUX_PIN_STRENGTH_NORMAL,
	/** High current output driver strength. */
	SYSTEM_PINMUX_PIN_STRENGTH_HIGH,
};

/** \brief Port pin output slew rate enum.
 *
 * Enum for the possible output drive slew rates for the port pin
 * configuration structure, to indicate the driver slew rate the pin should
 * use.
 */
enum system_pinmux_pin_slew_rate {
	/** Normal pin output slew rate. */
	SYSTEM_PINMUX_PIN_SLEW_RATE_NORMAL,
	/** Enable slew rate limiter on the pin. */
	SYSTEM_PINMUX_PIN_SLEW_RATE_LIMITED,
};

/** \brief Port pin output drive mode enum.
 *
 * Enum for the possible output drive modes for the port pin configuration
 * structure, to indicate the output mode the pin should use.
 */
enum system_pinmux_pin_drive {
	/** Use totem pole output drive mode. */
	SYSTEM_PINMUX_PIN_DRIVE_TOTEM,
	/** Use open drain output drive mode. */
	SYSTEM_PINMUX_PIN_DRIVE_OPEN_DRAIN,
};

/** \brief Port pin configuration structure.
 *
 *  Configuration structure for a port pin instance. This structure should be
 *  structure should be initialized by the \ref port_pin_get_config_defaults()
 *  function before being modified by the user application.
 */
struct system_pinmux_pin_conf {
	/** MUX index of the peripheral that should control the pin, if peripheral
	 *  control is desired. For GPIO use, this should be set to
	 *  \ref SYSTEM_PINMUX_GPIO. */
	uint8_t mux_position;

	/** Port buffer input/output direction. */
	enum system_pinmux_pin_dir direction;

	/** Configuration of the input buffer of a GPIO pin. */
	struct {
		/** Logic level pull of the input buffer. */
		enum system_pinmux_pin_pull pull;

		/** Sampling mode of the input buffer. */
		enum system_pinmux_pin_sampling_mode sampling_mode;
	} input; /**< Pin input sampler configuration. */

	/** Configuration of the output buffer of a GPIO pin. */
	struct {
		/** Strength of the output driver. */
		enum system_pinmux_pin_strength drive_strength;

		/** Slew rate of the output driver. */
		enum system_pinmux_pin_slew_rate slew_rate;

		/** Drive mode of the output buffer. */
		enum system_pinmux_pin_drive drive;
	} output; /**< Pin output driver configuration. */
};

/** \brief Retrieves the base PORT module address from a given GPIO pin number.
 *
 *  Retrieves the base address of a PORT hardware module associated with
 *  the given GPIO pin number.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to convert
 *
 *  \return Base address of the associated PORT module.
 */
static inline PortGroup* system_pinmux_get_port_from_gpio_pin(
		const uint8_t gpio_pin)
{
	uint8_t port_index = (gpio_pin / 128);
	uint8_t group_index = (gpio_pin / 32);

	/* Array of available ports. */
	Port *const ports[PORT_INST_NUM] = PORT_INSTS;

	if (port_index < PORT_INST_NUM) {
		return &(ports[port_index]->Group[group_index]);
	} else {
		Assert(false);
		return NULL;
	}
}

/** \name Configuration and initialization
 * @{
 */

/** \brief Initializes a Port pin configuration structure to defaults.
 *
 *  Initializes a given Port pin configuration structure to a set of
 *  known default values. This function should be called on all new
 *  instances of these configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *   \li Non peripheral (i.e. GPIO) controlled
 *   \li Input mode with internal pullup enabled, output mode disabled
 *   \li Edge detection disabled
 *   \li On-demand input sampling
 *
 *  \param[out] config  Configuration structure to initialize to default values
 */
static inline void system_pinmux_pin_get_config_defaults(
		struct system_pinmux_pin_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->mux_position = SYSTEM_PINMUX_GPIO;
	config->direction = SYSTEM_PINMUX_PIN_DIR_INPUT;
	config->input.pull = SYSTEM_PINMUX_PIN_PULL_UP;
	config->input.sampling_mode = SYSTEM_PINMUX_PIN_SAMPLING_ONDEMAND;
	config->output.strength = SYSTEM_PINMUX_PIN_STRENGTH_NORMAL;
	config->output.slew_rate = SYSTEM_PINMUX_PIN_SLEW_RATE_NORMAL;
	config->output.drive = SYSTEM_PINMUX_PIN_DRIVE_TOTEM;
}

void system_pinmux_pin_set_config(
		const uint8_t gpio_pin,
		const struct system_pinmux_pin_conf *const config);

/** @} */

#ifdef __cplusplus
}
#endif

/** @} */

/**
 * \page port_extra_info Extra Information
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
 *		<td>MUX</td>
 *		<td>Multiplexer</td>
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
 * \page port_quickstart Quick Start Guides for the PORT module
 *
 * This is the quick start guide list for the \ref sam0_port_group module, with
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
 * \section port_use_cases PORT module use cases
 * - \subpage port_basic_use_case
 */

#endif
