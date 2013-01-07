/**
 * \file
 *
 * \brief SAMD20 GPIO Port Driver
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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
 * Driver for the SAMD20 architecture devices. This driver provides a unified
 * interface for the configuration and management of the physical device pins,
 * including external edge detection, peripheral muxing, input/output control
 * and pad drive characteristics. This driver encompasses multiple physical
 * modules within the device to provide a consistent interface:
 *
 * \li \b PORT (Port I/O Management)
 * \li \b EIC (External Interrupt Controller)
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
 *     node [label="External Interrupt Module" shape=ellipse] eic;
 *     node [label="Peripheral Modules" shape=ellipse style=filled fillcolor=lightgray] peripherals;
 *   }
 *
 *   pinmux -> gpio;
 *   pad    -> pinmux;
 *   pinmux -> eic;
 *   pinmux -> peripherals;
 * }
 * \enddot
 *
 * \section module_introduction Introduction
 * The SAMD20 devices contain a number of General Purpose I/O pins, used to
 * interface the user application logic and internal hardware peripherals to
 * an external system.
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
 * \subsection peripheral_muxing Peripheral Muxltiplexing
 * SAMD20 devices contain a peripheral MUX, which is individually controllable
 * for each I/O pin of the device. The peripheral MUX allows you to select the
 * function of a physical package pin - whether it will be controlled as a user
 * controllable GPIO pin, or whether it will be connected internally to one of
 * several peripheral modules (such as a I2C module).
 *
 * \subsection edge_detection Edge Detection
 * In each SAMD20 device pin there is a connection to an External Interrupt
 * Controller module, used to detect external pin edge and level events, and
 * notify the user application via a flag or interrupt. Each pin can be
 * individually configured to sense a given logic level or pulse edge. This
 * feature is commonly used to wake up the device from a deep sleep mode when
 * an external activity (e.g. a user button press) occurs, to achieve maximum
 * power savings.
 *
 * \subsection pad_characteristics Special Pad Characteristics
 * There are several special modes that can be selected on one or more I/O pins
 * of the device, which alter the input and output characteristics of the pad:
 *
 * \subsubsection drive_strength Drive Strength
 * The Drive Strength configures the strength of the output driver on the
 * pad. Normally, there is a fixed current limit that each I/O pin can safely
 * drive, however some I/O pads offer a higher drive mode which increases this
 * limit for that I/O pin at the expense of an increased power consumption.
 *
 * \subsubsection slew_rate Slew Rate
 * The Slew Rate configures the slew rate of the output driver, limiting the
 * rate at which the pad output voltage can change with time.
 *
 * \subsubsection input_sample_mode Input Sample Mode
 * The Input Sample Mode configures the input sampler buffer of the pad. By
 * default, the input buffer is only sampled "on-demand", i.e. when the user
 * application attempts to read from the input buffer. This mode is the most
 * power efficient, but increases the latency of the input sample by two clock
 * cycles of the port clock. To reduce latency, the input sampler can instead
 * be configured to always sample the input buffer on each port clock cycle, at
 * the expense of an increased power consumption.
 *
 * \section module_dependencies Dependencies
 * The port driver has the following dependencies.
 *
 * \li None
 *
 * \section special_considerations Special Considerations
 *
 * Not all devices support all peripheral MUX combinations and drive modes;
 * see your device specific datasheet for more details.
 *
 * The SAMD20 port pin output driver and input pin sampler can be enabled and
 * disabled seperately; if a pin requires only output functionality with no
 * readback of the current pin state, the pin input sampler can be disabled to
 * save power:
 *
 * <table>
 *	<tr>
 *		<th>Input Sampler</th>
 *		<th>Output Driver</th>
 *		<th>Resulting GPIO Functionality</th>
 *	</tr>
 *	<tr>
 *		<td>Disabled</td>
 *		<td>Disabled</td>
 *		<td>Low power mode, no output or input possible.</td>
 *	</tr>
 *	<tr>
 *		<td>Enabled</td>
 *		<td>Disabled</td>
 *		<td>Input only mode, no output possible</td>
 *	</tr>
 *	<tr>
 *		<td>Disabled</td>
 *		<td>Enabled</td>
 *		<td>Output only mode, no readback of current pin state possible</td>
 *	</tr>
 *	<tr>
 *		<td>Enabled</td>
 *		<td>Enabled</td>
 *		<td>Output mode, with readback of the physical pin state</td>
 *	</tr>
 * </table>
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

#ifdef __cplusplus
extern "C" {
#endif

#define PORTA PORT.Group[0]
#define PORTB PORT.Group[1]

/** \brief Port external interrupt edge detection configuration enum.
 *
 * Enum for the possible signal edge detection modes of the External
 * Interrupt Controller module.
 */
enum port_edge_detect {
	/** No edge detection. */
	PORT_EDGE_DETECT_NONE    = 0,
	/** Detect rising signal edges. */
	PORT_EDGE_DETECT_RISING  = 1,
	/** Detect falling signal edges. */
	PORT_EDGE_DETECT_FALLING = 2,
	/** Detect both signal edges. */
	PORT_EDGE_DETECT_BOTH    = 3,
	/** Detect high signal levels. */
	PORT_EDGE_DETECT_HIGH    = 4,
	/** Detect low signal levels. */
	PORT_EDGE_DETECT_LOW     = 5,
};

/** \brief Port pin type configuration enum.
 *
 * Enum for the possible type setting of the port pin configuration
 * structure, to indicate the control type the pin should use.
 */
enum port_pin_type {
	/** Pin level is user controller as a GPIO. */
	PORT_PIN_TYPE_GPIO,
	/** Pin controlled by a peripheral within the device. */
	PORT_PIN_TYPE_PERIPHERAL,
};

/** \brief Port pin input pull configuration enum.
 *
 * Enum for the possible pin pull settings of the port pin configuration
 * structure, to indicate the type of logic level pull the pin should use.
 */
enum port_pin_pull {
	/** No logical pull should be applied to the pin. */
	PORT_PIN_PULL_NONE,
	/** Pin should be pulled up when idle. */
	PORT_PIN_PULL_UP,
	/** Pin should be pulled down when idle. */
	PORT_PIN_PULL_DOWN,
};

/** \brief Port pin digital input sampling mode enum.
 *
 * Enum for the possible input sampling modes for the port pin configuration
 * structure, to indicate the type of sampling a port pin should use.
 */
enum port_pin_sampling_mode {
	/** Pin input buffer should be enabled when the IN register is read. */
	PORT_PIN_SAMPLING_ONDEMAND,
	/** Pin input buffer should continuously sample the pin state. */
	PORT_PIN_SAMPLING_CONTINUOUS,
};

/** \brief Port pin drive output strength enum.
 *
 * Enum for the possible output drive strengths for the port pin
 * configuration structure, to indicate the driver strength the pin should
 * use.
 */
enum port_pin_strength {
	/** Normal output driver strength. */
	PORT_PIN_STRENGTH_NORMAL,
	/** High current output driver strength. */
	PORT_PIN_STRENGTH_HIGH,
};

/** \brief Port pin output slew rate enum.
 *
 * Enum for the possible output drive slew rates for the port pin
 * configuration structure, to indicate the driver slew rate the pin should
 * use.
 */
enum port_pin_slew_rate {
	/** Normal pin output slew rate. */
	PORT_PIN_SLEW_RATE_NORMAL,
	/** Enable slew rate limiter on the pin. */
	PORT_PIN_SLEW_RATE_LIMITED,
};

/** \brief Port pin output drive mode enum.
 *
 * Enum for the possible output drive modes for the port pin configuration
 * structure, to indicate the output mode the pin should use.
 */
enum port_pin_drive {
	/** Use totem pole output drive mode. */
	PORT_PIN_DRIVE_TOTEM,
	/** Use open drain output drive mode. */
	PORT_PIN_DRIVE_OPEN_DRAIN,
};

/** \brief External Interrupt Controller channel configuration structure.
 *
 *  Configuration structure for the input sampler edge detection mode of a GPIO
 *  pin channel.
 */
struct port_pin_edge_conf {
	/** Wake up the microcontroller if the channel interrupt fires during
	 *  sleep mode. */
	bool wake_if_sleeping;
	/** Filter the raw input signal to prevent noise from triggering an
	 *  interrupt accidentally, using a 3 sample majority filter. */
	bool filter_input_signal;
	/** Edge detection mode to use. */
	enum port_edge_detect mode;
};

/** \brief Port pin configuration structure.
 *
 *  Configuration structure for a port pin instance. This structure should be
 *  structure should be initialized by the \ref port_pin_get_config_defaults()
 *  function before being modified by the user application.
 */
struct port_pin_conf {
	/** Port pin control type. */
	enum port_pin_type type;

	/** Index of the peripheral that should control the pin, if peripheral
	 *  control type is selected. If \ref type if set to \ref PORT_PIN_TYPE_GPIO
	 *  this value is ignored. */
	uint8_t peripheral_index;

	/** Configuration of the input buffer of a GPIO pin. */
	struct {
		/** Enables the pin level input buffer if set. */
		bool enabled;

		/** Logic level pull of the input buffer. */
		enum port_pin_pull pull;

		/** Sampling mode of the input buffer. */
		enum port_pin_sampling_mode sampling_mode;

		/** External interrupt controller configuration. */
		struct port_pin_edge_conf edge_detection;
	} input; /**< Pin input sampler configuration. */

	/** Configuration of the output buffer of a GPIO pin. */
	struct {
		/** Enables the pin level output buffer if set. */
		bool enabled;

		/** Strength of the output driver. */
		enum port_pin_strength drive_strength;

		/** Slew rate of the output driver. */
		enum port_pin_slew_rate slew_rate;

		/** Drive mode of the output buffer. */
		enum port_pin_drive drive;
	} output; /**< Pin output driver configuration. */
};

/** \name State reading/writing (physical module orientated)
 * @{
 */

/** \brief Retrieves the base PORT module address from a given GPIO pin number.
 *
 *  Retrieves the base address of a PORT hardware module associated with
 *  the given GPIO pin number.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to convert
 *
 *  \return Base address of the associated PORT module.
 */
static inline PortGroup* port_get_port_from_gpio_pin(
		const uint8_t gpio_pin)
{
	uint8_t port_index = (gpio_pin / 128);
	uint8_t group_index = (gpio_pin / 32);

	/* Array of available ports. */
	Port *ports[PORT_INST_NUM] = PORT_INSTS;

	if (port_index < PORT_INST_NUM) {
		return &(ports[port_index]->Group[group_index]);
	} else {
		Assert(false);
		return NULL;
	}
}

/** \brief Retrieves the base EIC module address from a given GPIO pin number.
 *
 *  Retrieves the base address of a EIC hardware module associated with
 *  the given GPIO pin number.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to convert
 *
 *  \return Base address of the associated EIC module.
 */
static inline Eic* port_get_eic_from_gpio_pin(
		const uint8_t gpio_pin)
{
	uint8_t port_index = (gpio_pin / 32);

	if (port_index == 0) {
		return &EIC;
	}
	else {
		Assert(false);
		return NULL;
	}
}

/** \brief Retrieves the state of a port's pins that are configured as inputs.
 *
 *  Reads the current logic level of a port module's pins and returns the
 *  current levels as a bitmask.
 *
 *  \param[in] port      Base of the PORT module to read from
 *  \param[in] pin_mask  Mask of the port pin(s) to read
 *
 *  \return Status of the port pin(s) input buffers.
 */
static inline uint32_t port_get_input_levels(
		const PortGroup *const port,
		const uint32_t pin_mask)
{
	Assert(port);

	return (port->IN.reg & pin_mask);
}

/** \brief Sets the state of a port's pins that are configured as outputs.
 *
 *  Sets the current output level of a port module's pins to a given logic
 *  level.
 *
 *  \param[out] port        Base of the PORT module to write to
 *  \param[in]  pin_mask    Mask of the port pin(s) to change
 *  \param[in]  level_mask  Mask of the port level(s) to set
 */
static inline void port_set_output_levels(
		PortGroup *const port,
		const uint32_t pin_mask,
		const uint32_t level_mask)
{
	Assert(port);

	port->OUTSET.reg = (pin_mask &  level_mask);
	port->OUTCLR.reg = (pin_mask & ~level_mask);
}

/** \brief Toggles the state of a port's pins that are configured as an outputs.
 *
 *  Toggles the current output levels of a port module's pins.
 *
 *  \param[out] port      Base of the PORT module to write to
 *  \param[in]  pin_mask  Mask of the port pin(s) to toggle
 */
static inline void port_toggle_output_levels(
		PortGroup *const port,
		const uint32_t pin_mask)
{
	Assert(port);

	port->OUTTGL.reg = pin_mask;
}

/** @} */

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
static inline void port_pin_get_config_defaults(
		struct port_pin_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->type = PORT_PIN_TYPE_GPIO;
	config->input.enabled = true;
	config->input.pull = PORT_PIN_PULL_UP;
	config->input.sampling_mode = PORT_PIN_SAMPLING_ONDEMAND;
	config->input.edge_detection.wake_if_sleeping = false;
	config->input.edge_detection.filter_input_signal = false;
	config->input.edge_detection.mode = PORT_EDGE_DETECT_NONE;
	config->output.enabled = false;
	config->output.drive_strength = PORT_PIN_STRENGTH_NORMAL;
	config->output.slew_rate = PORT_PIN_SLEW_RATE_NORMAL;
	config->output.drive = PORT_PIN_DRIVE_TOTEM;
}

enum status_code port_pin_set_config(
		const uint8_t gpio_pin,
		const struct port_pin_conf *const config);

/** @} */


/** \name Edge detection
 * @{
 */

/** \brief Retrieves the edge detection state of a configured port pin.
 *
 *  Reads the current state of a configured port pin, and determines if
 *  if the detection criteria of the channel has been met.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to check
 *
 *  \return Status of the requested pin's edge detection state.
 */
static inline bool port_pin_is_edge_detected(
		const uint8_t gpio_pin)
{
	Eic   *eic_base = port_get_eic_from_gpio_pin(gpio_pin);
	uint32_t eic_mask = (1UL << (gpio_pin % 32));

	return (eic_base->INTFLAG.reg & eic_mask);
}

/** \brief Clears the edge detection state of a configured port pin.
 *
 *  Clears the current state of a configured port pin, readying it for
 *  the next level or edge detection.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin edge detection state to clear
 */
static inline void port_pin_clear_edge_detected(
		const uint8_t gpio_pin)
{
	Eic   *eic_base = port_get_eic_from_gpio_pin(gpio_pin);
	uint32_t eic_mask = (1UL << (gpio_pin % 32));

	eic_base->INTFLAG.reg = eic_mask;
}

/** @} */

/** \name State reading/writing (logical pin orientated)
 * @{
 */

/** \brief Retrieves the state of a port pin that is configured as an input.
 *
 *  Reads the current logic level of a port pin and returns the current
 *  level as a boolean value.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to read
 *
 *  \return Status of the port pin's input buffer.
 */
static inline bool port_pin_get_input_level(
		const uint8_t gpio_pin)
{
	PortGroup  *port_base = port_get_port_from_gpio_pin(gpio_pin);
	uint32_t pin_mask  = (1UL << (gpio_pin % 32));

	return (port_base->IN.reg & pin_mask);
}

/** \brief Sets the state of a port pin that is configured as an output.
 *
 *  Sets the current output level of a port pin to a given logic level.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to write to
 *  \param[in] level     Logical level to set the given pin to
 */
static inline void port_pin_set_output_level(
		const uint8_t gpio_pin,
		const bool level)
{
	PortGroup *port_base = port_get_port_from_gpio_pin(gpio_pin);
	uint32_t pin_mask  = (1UL << (gpio_pin % 32));

	/* Set the pin to high or low atomically based on the requested level */
	if (level) {
		port_base->OUTSET.reg = pin_mask;
	} else {
		port_base->OUTCLR.reg = pin_mask;
	}
}

/** \brief Toggles the state of a port pin that is configured as an output.
 *
 *  Toggles the current output level of a port pin.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to toggle
 */
static inline void port_pin_toggle_output_level(
		const uint8_t gpio_pin)
{
	PortGroup  *port_base = port_get_port_from_gpio_pin(gpio_pin);
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
 *		<td>EIC</td>
 *		<td>External Interrupt Controller</td>
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
