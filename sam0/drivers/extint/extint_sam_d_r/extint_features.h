/**
 * \file
 *
 * \brief SAM External Interrupt Features
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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
#ifndef EXTINT_FEATURE_H_INCLUDED
#define EXTINT_FEATURE_H_INCLUDED
#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief External interrupt edge detection configuration enum.
 *
 * Enum for the possible signal edge detection modes of the External
 * Interrupt Controller module.
 */
enum extint_detect {
	/** No edge detection. Not allowed as a NMI detection mode on some
	 *  devices. */
	EXTINT_DETECT_NONE    = 0,
	/** Detect rising signal edges. */
	EXTINT_DETECT_RISING  = 1,
	/** Detect falling signal edges. */
	EXTINT_DETECT_FALLING = 2,
	/** Detect both signal edges. */
	EXTINT_DETECT_BOTH    = 3,
	/** Detect high signal levels. */
	EXTINT_DETECT_HIGH    = 4,
	/** Detect low signal levels. */
	EXTINT_DETECT_LOW     = 5,
};

/**
 * \brief External interrupt internal pull configuration enum.
 *
 * Enum for the possible pin internal pull configurations.
 *
 * \note Disabling the internal pull resistor is not recommended if the driver
 *       is used in interrupt (callback) mode, due the possibility of floating
 *       inputs generating continuous interrupts.
 */
enum extint_pull {
	/** Internal pull-up resistor is enabled on the pin. */
	EXTINT_PULL_UP        = SYSTEM_PINMUX_PIN_PULL_UP,
	/** Internal pull-down resistor is enabled on the pin. */
	EXTINT_PULL_DOWN      = SYSTEM_PINMUX_PIN_PULL_DOWN,
	/** Internal pull resistor is disconnected from the pin. */
	EXTINT_PULL_NONE      = SYSTEM_PINMUX_PIN_PULL_NONE,
};

/**
 * \brief External Interrupt Controller channel configuration structure.
 *
 *  Configuration structure for the edge detection mode of an external
 *  interrupt channel.
 */
struct extint_chan_conf {
	/** GPIO pin the NMI should be connected to. */
	uint32_t gpio_pin;
	/** MUX position the GPIO pin should be configured to. */
	uint32_t gpio_pin_mux;
	/** Internal pull to enable on the input pin. */
	enum extint_pull gpio_pin_pull;
	/** Wake up the device if the channel interrupt fires during sleep mode. */
	bool wake_if_sleeping;
	/** Filter the raw input signal to prevent noise from triggering an
	 *  interrupt accidentally, using a 3 sample majority filter. */
	bool filter_input_signal;
	/** Edge detection mode to use. */
	enum extint_detect detection_criteria;
};
#ifdef __cplusplus
}
#endif

#endif