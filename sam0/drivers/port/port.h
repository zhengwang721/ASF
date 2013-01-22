/**
 * \file
 *
 * \brief SAMD20 GPIO Port Driver
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
#ifndef PORT_H_INCLUDED
#define PORT_H_INCLUDED

/**
 * \defgroup sam0_port_group SAMD20 Port Driver (PORT)
 *
 * Driver for the SAMD20 architecture devices. This driver provides a interface
 * for the configuration and management of the device's General Purpose
 * Input/Output (GPIO) pin functionality, for manual pin state reading and
 * writing. This driver encompasses the following module within the SAMD20
 * devices:
 *
 * \li \b PORT (Port I/O Management)
 *
 * Physically, the modules are interconnected within the device as shown in the
 * following diagram:
 *
 * \dot
 * digraph overview {
 *   node [label="Port Pad" shape=square] pad;
 *
 *   subgraph driver {
 *     node [label="Peripheral Mux" shape=trapezium] pinmux;
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
 * \section module_introduction Introduction
 * The SAMD20 devices contain a number of General Purpose I/O pins, used to
 * interface the user application logic and internal hardware peripherals to
 * an external system. This driver provides an easy-to-use interface to the
 * physical pin input samplers and output drivers, so that pins can be read from
 * or written to for general purpose external hardware control.
 *
 * \subsection physical_logical_pins Physical and Logical GPIO Pins
 * SAMD20 devices use two naming conventions for the I/O pins in the device; one
 * physical, and one logical. Each physical pin on a device package is assigned
 * both a physical port and pin identifier (e.g. "PORTA.0") as well as a
 * monotonically incrementing logical GPIO number (e.g. "GPIO0"). While the
 * former is used to map physical pins to their physical internal device module
 * counterparts, for simplicity the design of this driver uses the logical GPIO
 * numbers instead.
 *
 * \section module_dependencies Dependencies
 * The port driver has the following dependencies:
 *
 * \li \ref sam0_pinmux_group "System Pin Multiplexer Driver"
 *
 * \section special_considerations Special Considerations
 *
 * The SAMD20 port pin input sampler is disabled when the pin is configured as
 * an output; reading the pin state of an output will read the logical output
 * state that was last set.
 *
 * \section module_extra_info Extra Information
 * For extra information see \ref port_extra_info.
 *
 * \section module_examples Examples
 * - \ref port_quickstart
 *
 * \section api_overview API Overview
 * @{
 */

#include <compiler.h>
#include <pinmux.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \name PORT Alias Macros
 * @{
 */

/** Convenience definition for GPIO module group A on the device (if
 *  available). */
#if (PORT_GROUPS > 0) || defined(__DOXYGEN__)
#  define PORTA             PORT.Group[0]
#endif

#if (PORT_GROUPS > 1) || defined(__DOXYGEN__)
/** Convenience definition for GPIO module group B on the device (if
 *  available). */
#  define PORTB             PORT.Group[1]
#endif

#if (PORT_GROUPS > 2) || defined(__DOXYGEN__)
/** Convenience definition for GPIO module group C on the device (if
 *  available). */
#  define PORTC             PORT.Group[2]
#endif

#if (PORT_GROUPS > 3) || defined(__DOXYGEN__)
/** Convenience definition for GPIO module group D on the device (if
 *  available). */
#  define PORTD             PORT.Group[3]
#endif

/** @} */

/**
 *  \brief Port pin direction configuration enum.
 *
 *  Enum for the possible pin direction settings of the port pin configuration
 *  structure, to indicate the direction the pin should use.
 */
enum port_pin_dir {
	/** The pin's input buffer should be enabled, so that the pin state can
	 *  be read. */
	PORT_PIN_DIR_INPUT               = SYSTEM_PINMUX_PIN_DIR_INPUT,
	/** The pin's output buffer should be enabled, so that the pin state can
	 *  be set. */
	PORT_PIN_DIR_OUTPUT              = SYSTEM_PINMUX_PIN_DIR_OUTPUT,
	/** The pin's output and input buffers should be enabled, so that the pin
	 *  state can be set and read back. */
	PORT_PIN_DIR_OUTPUT_WTH_READBACK = SYSTEM_PINMUX_PIN_DIR_OUTPUT_WITH_READBACK,
};

/**
 *  \brief Port pin input pull configuration enum.
 *
 *  Enum for the possible pin pull settings of the port pin configuration
 *  structure, to indicate the type of logic level pull the pin should use.
 */
enum port_pin_pull {
	/** No logical pull should be applied to the pin. */
	PORT_PIN_PULL_NONE = SYSTEM_PINMUX_PIN_PULL_NONE,
	/** Pin should be pulled up when idle. */
	PORT_PIN_PULL_UP   = SYSTEM_PINMUX_PIN_PULL_UP,
	/** Pin should be pulled down when idle. */
	PORT_PIN_PULL_DOWN = SYSTEM_PINMUX_PIN_PULL_DOWN,
};

/**
 *  \brief Port pin configuration structure.
 *
 *  Configuration structure for a port pin instance. This structure should be
 *  structure should be initialized by the \ref port_get_config_defaults()
 *  function before being modified by the user application.
 */
struct port_conf {
	/** Port buffer input/output direction. */
	enum port_pin_dir  direction;

	/** Port pull-up/pull-down for input pins. */
	enum port_pin_pull input_pull;
};

/** \name State reading/writing (physical group orientated)
 * @{
 */

/**
 *  \brief Retrieves the PORT module group instance from a given GPIO pin number.
 *
 *  Retrieves the PORT module group instance associated with a given logical
 *  GPIO pin number.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to convert.
 *
 *  \return Base address of the associated PORT module.
 */
static inline PortGroup* port_get_group_from_gpio_pin(
		const uint8_t gpio_pin)
{
	return system_pinmux_get_group_from_gpio_pin(gpio_pin);
}

/**
 *  \brief Retrieves the state of a port's pins that are configured as inputs.
 *
 *  Reads the current logic level of a port module's pins and returns the
 *  current levels as a bitmask.
 *
 *  \param[in] port  Base of the PORT module to read from.
 *  \param[in] mask  Mask of the port pin(s) to read.
 *
 *  \return Status of the port pin(s) input buffers.
 */
static inline uint32_t port_group_get_input_level(
		const PortGroup *const port,
		const uint32_t mask)
{
	/* Sanity check arguments */
	Assert(port);

	return (port->IN.reg & mask);
}

/**
 *  \brief Sets the state of a port's pins that are configured as outputs.
 *
 *  Sets the current output level of a port module's pins to a given logic
 *  level.
 *
 *  \param[out] port        Base of the PORT module to write to.
 *  \param[in]  mask        Mask of the port pin(s) to change.
 *  \param[in]  level_mask  Mask of the port level(s) to set.
 */
static inline void port_group_set_output_level(
		PortGroup *const port,
		const uint32_t mask,
		const uint32_t level_mask)
{
	/* Sanity check arguments */
	Assert(port);

	port->OUTSET.reg = (mask &  level_mask);
	port->OUTCLR.reg = (mask & ~level_mask);
}

/**
 *  \brief Toggles the state of a port's pins that are configured as an outputs.
 *
 *  Toggles the current output levels of a port module's pins.
 *
 *  \param[out] port  Base of the PORT module to write to.
 *  \param[in]  mask  Mask of the port pin(s) to toggle.
 */
static inline void port_group_toggle_output_level(
		PortGroup *const port,
		const uint32_t mask)
{
	/* Sanity check arguments */
	Assert(port);

	port->OUTTGL.reg = mask;
}

/** @} */

/** \name Configuration and initialization
 * @{
 */

/**
 *  \brief Initializes a Port pin configuration structure to defaults.
 *
 *  Initializes a given Port pin configuration structure to a set of
 *  known default values. This function should be called on all new
 *  instances of these configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *   \li Input mode with internal pullup enabled
 *
 *  \param[out] config  Configuration structure to initialize to default values.
 */
static inline void port_get_config_defaults(
		struct port_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->direction = PORT_PIN_DIR_INPUT;
	config->input_pull = PORT_PIN_PULL_UP;
}

void port_pin_set_config(
		const uint8_t gpio_pin,
		const struct port_conf *const config);

void port_group_set_config(
		PortGroup *const port,
		const uint32_t mask,
		const struct port_conf *const config);

/** @} */

/** \name State reading/writing (logical pin orientated)
 * @{
 */

/**
 *  \brief Retrieves the state of a port pin that is configured as an input.
 *
 *  Reads the current logic level of a port pin and returns the current
 *  level as a boolean value.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to read.
 *
 *  \return Status of the port pin's input buffer.
 */
static inline bool port_pin_get_input_level(
		const uint8_t gpio_pin)
{
	PortGroup *const port_base = port_get_group_from_gpio_pin(gpio_pin);
	uint32_t pin_mask  = (1UL << (gpio_pin % 32));

	return (port_base->IN.reg & pin_mask);
}

/**
 *  \brief Sets the state of a port pin that is configured as an output.
 *
 *  Sets the current output level of a port pin to a given logic level.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to write to.
 *  \param[in] level     Logical level to set the given pin to.
 */
static inline void port_pin_set_output_level(
		const uint8_t gpio_pin,
		const bool level)
{
	PortGroup *const port_base = port_get_group_from_gpio_pin(gpio_pin);
	uint32_t pin_mask  = (1UL << (gpio_pin % 32));

	/* Set the pin to high or low atomically based on the requested level */
	if (level) {
		port_base->OUTSET.reg = pin_mask;
	} else {
		port_base->OUTCLR.reg = pin_mask;
	}
}

/**
 *  \brief Toggles the state of a port pin that is configured as an output.
 *
 *  Toggles the current output level of a port pin.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to toggle.
 */
static inline void port_pin_toggle_output_level(
		const uint8_t gpio_pin)
{
	PortGroup *const port_base = port_get_group_from_gpio_pin(gpio_pin);
	uint32_t pin_mask  = (1UL << (gpio_pin % 32));

	/* Toggle pin output level */
	port_base->OUTTGL.reg = pin_mask;
}

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
 *		<td>GPIO</td>
 *		<td>General Purpose Input/Output</td>
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
