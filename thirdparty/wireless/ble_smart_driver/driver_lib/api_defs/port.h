/**
 ****************************************************************************************
 *
 * @file port.h
 *
 * @brief SAMB11 Port(GPIO) API for Applications
 *
 * This module contains the public API and the necessary enumerations and structures that are required for 
 * SAMB11 Application Developers using GPIOs
 *
 *
 *  Copyright (c) 2014 Atmel Corporation. All rights reserved.
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 *  3. The name of Atmel may not be used to endorse or promote products derived from this software 
 *  without specific prior written permission.
 *
 *  4. This software may only be redistributed and used in connection with an Atmel microcontroller product.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
  * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
 *  THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 *  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************************
 */

#ifndef PORT_H_INCLUDED
#define PORT_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Port/GPIO Status codes.
 *
 * Status codes of PORT/GPIO drivers.
 */
enum port_status_code {
	PORT_STATUS_OK = 0,
	PORT_STATUS_RESOURCE_NOT_AVAILABLE,
	PORT_STATUS_ERR_NOT_INITIALIZED,
	PORT_STATUS_ERR_INVALID_ARG,
};

/**
 * \brief SAMB11 available Port pins.
 *
 * List of port/gpio pins available.
 */

enum port_pin {
	/** PORTPIN_0 is not available for User application*/
	PORTPIN_0	= 0,
	/** PORTPIN_1 is not available for User application*/
	PORTPIN_1,
	PORTPIN_2,
	PORTPIN_3,
	PORTPIN_4,
	PORTPIN_5,
	PORTPIN_6,
	PORTPIN_7,
	PORTPIN_8,
	PORTPIN_9,
	PORTPIN_10,
	PORTPIN_11,
	PORTPIN_12,
	PORTPIN_13,
	/** PORTPIN_14 is not available for User application*/
	PORTPIN_14,
	PORTPIN_15,
	PORTPIN_16,
	PORTPIN_17,
	PORTPIN_18,
	PORTPIN_19,
	PORTPIN_20,
#ifdef CHIPVERSION_B0
	PORTPIN_21,
	PORTPIN_22,
	PORTPIN_23,
	PORTPIN_24,
#endif
	PORTPIN_MAX,
};	

/** Type definition for an GPIO/PORT pin interrupt module callback function. */
typedef void (*portint_callback_t)(void);

#ifdef CHIPVERSION_B0
	#define GPIO0_COMBINED_VECTOR_TABLE_INDEX		39
	#define GPIO1_COMBINED_VECTOR_TABLE_INDEX		40
	#define GPIO2_COMBINED_VECTOR_TABLE_INDEX		41
#else
	#define GPIO0_COMBINED_VECTOR_TABLE_INDEX		23
	#define GPIO1_COMBINED_VECTOR_TABLE_INDEX		24
#endif	//CHIPVERSION_B0

/** Enum for the possible callback types for the GPIO/PORT pin interrupt module. */
enum portint_callback_type
{
	/** Callback type for when an external interrupt detects the configured
	 *  channel criteria (i.e. edge or level detection)
	 */
	PORTINT_CALLBACK_TYPE_DETECT = 1,
};

/**
 *  \brief Port pin direction configuration enum.
 *
 *  Enum for the possible pin direction settings of the port pin configuration
 *  structure, to indicate the direction the pin should use.
 */
enum port_pin_dir {
	/** The pin's input buffer should be enabled, so that the pin state can
	 *  be read. */
	PORT_PIN_DIR_INPUT,
	/** The pin's output buffer should be enabled, so that the pin state can
	 *  be set. */
	PORT_PIN_DIR_OUTPUT,
};

/**
 *  \brief Port pin input pull configuration enum.
 *
 *  Enum for the possible pin pull settings of the port pin configuration
 *  structure, to indicate the type of logic level pull the pin should use.
 */
enum port_pin_pull {
	/** No logical pull should be applied to the pin. */
	PORT_PIN_PULL_NONE,
	/** Pin should be pulled up when idle. */
	PORT_PIN_PULL_UP ,
#ifdef CHIPVERSION_B0	
	/** Pin should be pulled down when idle.*/ 
	PORT_PIN_PULL_DOWN,
#endif	//CHIPVERSION_B0
};

/**
 *  \brief Port pin configuration structure.
 *
 *  Configuration structure for a port pin instance. This structure should be
 *  initialized by the \ref port_get_config_defaults() function before being
 *  modified by the user application.
 */
struct port_config {
	/** Port buffer input/output direction. */
	enum port_pin_dir  direction;

	/** Port pull-up/pull-down for input pins. */
	enum port_pin_pull input_pull;

	/** Enable lowest possible powerstate on the pin
	 *
	 *  \note All other configurations will be ignored, the pin will be disabled
	 */
	bool powersave;
};

/**
 * \brief Port Pin interrupt edge detection configuration enum.
 *
 * Enum for the possible signal edge detection modes of the External
 * Interrupt Controller module.
 */
enum port_int_detect {
	/** No edge detection. Not allowed as a NMI detection mode on some
	 *  devices. */
	PORT_INT_DETECT_NONE    = 0,
	/** Detect rising signal edges. */
	PORT_INT_DETECT_RISING,
	/** Detect falling signal edges. */
	PORT_INT_DETECT_FALLING,
	/** Detect high signal levels. */
	PORT_INT_DETECT_HIGH,
	/** Detect low signal levels. */
	PORT_INT_DETECT_LOW,
};

/**
 * \brief Port pin configuration structure.
 *
 *  Configuration structure for the edge detection mode of an port pin
 *  interrupt channel.
 */
struct port_int_chan_conf {
	/** GPIO pin the NMI should be connected to. */
	uint8_t port_pin;
	/** MUX position for the port pin should be configured to. */
	uint8_t pin_mux;
	/** Internal pull to enable on the input pin. */
	enum port_pin_pull port_pin_pull;
	/** Wake up the device if the channel interrupt fires during sleep mode. */
	bool wake_if_sleeping;
	/** Edge detection mode to use. */
	enum port_int_detect detection_type;
};

/**
@defgroup port-drv Port Driver API

@{
*/
/** \name Configuration and initialization
 * @{
 */

/**
 *  \brief Initializes a Port pin/group configuration structure to defaults.
 *
 *  Initializes a given Port pin/group configuration structure to a set of
 *  known default values. This function should be called on all new
 *  instances of these configuration structures before being modified by the
 *  user application.
 *
 *  The default configuration is as follows:
 *   \li Input mode with internal pullup enabled
 *
 *  \param[out] config  Configuration structure to initialize to default values.
 */
void port_get_config_defaults(
		struct port_config *const config);

/**
 * \brief Initializes an external port interrupt pin configuration structure to defaults.
 *
 * Initializes a given port pin interrupt channel configuration structure to a
 * set of known default values. This function should be called on all new
 * instances of these configuration structures before being modified by the
 * user application.
 *
 * The default configuration is as follows:
 * \li not a wakeup interrupt
 * \li Internal pull-up enabled
 * \li Detect falling edges of a signal
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
void portint_get_config_defaults(
		struct port_int_chan_conf *const config);

/**
 *  \brief Writes a Port pin configuration to the hardware module.
 *
 *  Writes out a given configuration of a Port pin configuration to the hardware
 *  module. If the configuration is NULL then it releases the port pin.
 *
 *  \note If the pin direction is set as an output, the pull-up/pull-down input
 *        configuration setting is ignored. Also certain port pin is used by 
 *				FW and not available for user application. Please \ref port_pin
 *				for list of port_pin available.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to configure.
 *  \param[in] config    Configuration settings for the pin.
 *
 * 	\return Status of initialization.
 * 	\retval PORT_STATUS_OK                   		Port configured correctly
 * 	\retval PORT_STATUS_ERR_INVALID_ARG      		Invalid port number, Certain 
 *																						 ports are used by FW and not allowed to change.
 * 	\retval PORT_STATUS_RESOURCE_NOT_AVAILABLE 	Requested port is already in use.
 * 	\retval PORT_STATUS_ERR_INVALID_ARG   			Invalid pull-up/pull-down configuration.
 *
 */
enum port_status_code port_pin_set_config(
		const uint8_t gpio_pin,
		const struct port_config *const config);

/**
 *  \brief Writes a Port pin interrupt configuration to the hardware module.
 *
 *  Writes out a given interrupt configuration of a Port pin to the hardware
 *  module. If the configuration is NULL then it releases the port pin.
 *
 *  \note Pin direction needs to be set as an input, Also certain port pin is 
 *	used by FW and not available for user application. Please \ref port_pin
 *	for list of port_pin available.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to configure.
 *  \param[in] config    Interrupt Configuration settings for the pin.
 *
 * 	\return Status of initialization.
 * 	\retval PORT_STATUS_OK                   		Port configured correctly
 * 	\retval PORT_STATUS_ERR_INVALID_ARG      		Invalid port number, Certain 
 *																						 ports are used by FW and not allowed to change.
 * 	\retval PORT_STATUS_RESOURCE_NOT_AVAILABLE 	Requested port is already in use.
 * 	\retval PORT_STATUS_ERR_INVALID_ARG   			Invalid pull-up/pull-down configuration.
 *
 */
enum port_status_code port_pin_set_int_config(
		const uint8_t gpio_pin,
		const struct port_int_chan_conf *const config);

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
bool port_pin_get_input_level(
		const uint8_t gpio_pin);

/**
 *  \brief Retrieves the state of a port pin that is configured as an output.
 *
 *  Reads the current logical output level of a port pin and returns the current
 *  level as a boolean value.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to read.
 *
 *  \return Status of the port pin's output buffer.
 */
bool port_pin_get_output_level(
		const uint8_t gpio_pin);

/**
 *  \brief Sets the state of a port pin that is configured as an output.
 *
 *  Sets the current output level of a port pin to a given logic level.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to write to.
 *  \param[in] level     Logical level to set the given pin to.
 */
void port_pin_set_output_level(
		const uint8_t gpio_pin,
		const bool level);

/**
 *  \brief Toggles the state of a port pin that is configured as an output.
 *
 *  Toggles the current output level of a port pin.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to toggle.
 */
void port_pin_toggle_output_level(
		const uint8_t gpio_pin);

/**
 *  \brief Registers a callback function for a port pin interrupt.
 *
 *  Registes a callback function for a given port pin.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin.
 *  \param[in] fp  			 Callback function \ref portint_callback_t.
 *
 * 	\retval PORT_STATUS_OK                   		Callback function is registered successfully.
 * 	\retval PORT_STATUS_ERR_INVALID_ARG   			Invalid port pin.
 */
enum port_status_code port_int_register_callback(uint8_t gpio_pin,
																	portint_callback_t fp);

/**
 *  \brief Enables the port pin interrupt
 *
 *  Enables the Interrupt for the given port pin.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to toggle.
 *
 * 	\retval PORT_STATUS_OK                   		Callback function is registered successfully.
 * 	\retval PORT_STATUS_ERR_INVALID_ARG   			Invalid port pin.
 */
enum port_status_code port_int_enable(uint8_t gpio_pin);

/**
 *  \brief Disables the port pin interrupt
 *
 *  Disables the Interrupt for the given port pin.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to toggle.
 *
 * 	\retval PORT_STATUS_OK                   		Callback function is registered successfully.
 * 	\retval PORT_STATUS_ERR_INVALID_ARG   			Invalid port pin.
 */
enum port_status_code port_int_disable(uint8_t gpio_pin);


/** @} */

/** @}*/

#ifdef __cplusplus
}
#endif

#endif	//PORT_H_INCLUDED
