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

#include <gclk.h>
#include <clock.h>

/** Wait for the GCLK module to synchronize any pending writes. */
static void _system_gclk_wait_for_sync(void)
{
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) {
		/* Wait for sync to complete */
	}
}

/** \brief Initializes the GCLK driver.
 *
 *  Initializes the Generic Clock module, disabling and resetting all active
 *  Generic Clock Generators and Channels to their power-on default values.
 */
void system_gclk_init(void)
{
	/* Software reset the module to ensure it is re-initialized correctly */
	GCLK->CTRL.reg = GCLK_CTRL_SWRST;
	while (GCLK->CTRL.reg & GCLK_CTRL_SWRST) {
		/* Wait for reset to complete */
	}
}

/** \brief Writes a Generic Clock Generator configuration to the hardware module.
 *
 *  Writes out a given configuration of a Generic Clock Generator configuration
 *  to the hardware module. If the generator is currently running, it will be
 *  stopped.
 *
 *  \note Once called the generator will not be running; to start the generator,
 *        call \ref system_gclk_gen_enable() after configuring a generator.
 *
 *  \param[in] generator  Generic Clock Generator index to configure
 *  \param[in] config     Configuration settings for the generator
 */
void system_gclk_gen_set_config(
		const uint8_t generator,
		struct system_gclk_gen_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Cache new register configurations to minimize sync requirements. */
	uint32_t new_genctrl_config = (generator << GCLK_GENCTRL_ID_Pos);
	uint32_t new_gendiv_config  = (generator << GCLK_GENDIV_ID_Pos);

	/* Select the requested source clock for the generator */
	new_genctrl_config |= config->source_clock << GCLK_GENCTRL_SRC_Pos;

	/* Configure the clock to be either high or low when disabled */
	if (config->high_when_disabled) {
		new_genctrl_config |= GCLK_GENCTRL_OOV;
	}

	/* Configure if the clock output to I/O pin should be enabled. */
	if (config->output_enable) {
		new_genctrl_config |= GCLK_GENCTRL_OE;
	}

	/* Set division factor */
	if (config->division_factor > 1) {
		/* Check if division is a power of two */
		if (((config->division_factor & (config->division_factor - 1)) == 0)) {
			/* Determine the index of the highest bit set to get the
			 * division factor that must be loaded into the division
			 * register */
			uint32_t div2_count = 0;
			uint32_t mask;
			for (mask = (1UL << 1); mask < config->division_factor; mask <<= 1) {
				div2_count++;
			}

			/* Set binary divider power of 2 division factor */
			new_gendiv_config  |= div2_count << GCLK_GENDIV_DIV_Pos;
		} else {
			/* Set integer division factor */
			new_gendiv_config  |= config->division_factor << GCLK_GENDIV_DIV_Pos;

			/* Enable non-binary division with increased duty cycle accuracy */
			new_genctrl_config |= GCLK_GENCTRL_DIVSEL;
			new_genctrl_config |= GCLK_GENCTRL_IDC;
		}
	}

	#if defined (REVB)
	/* Enable or disable the clock in standby mode */
	if (config->run_in_standby) {
		new_genctrl_config |= GCLK_GENCTRL_RUNSTDBY;
	}
	#endif

	/* Disable generator before updating it */
	system_gclk_gen_disable(generator);

	/* Write the new generator configuration */
	_system_gclk_wait_for_sync();
	GCLK->GENDIV.reg  = new_gendiv_config;
	_system_gclk_wait_for_sync();
	GCLK->GENCTRL.reg = new_genctrl_config;
}

/** \brief Enables a Generic Clock Generator that was previously configured.
 *
 *  Starts the clock generation of a Generic Clock Generator that was previously
 *  configured via a call to \ref system_gclk_gen_set_config().
 *
 *  \param[in] generator  Generic Clock Generator index to enable
 */
void system_gclk_gen_enable(const uint8_t generator)
{
	/* Select the requested generator */
	*((uint8_t*)&GCLK->GENCTRL.reg) = generator;
	_system_gclk_wait_for_sync();

	/* Enable generator */
	GCLK->GENCTRL.reg |= GCLK_GENCTRL_GENEN;
}

/** \brief Disables a Generic Clock Generator that was previously enabled.
 *
 *  Stops the clock generation of a Generic Clock Generator that was previously
 *  started via a call to \ref system_gclk_gen_enable().
 *
 *  \param[in] generator  Generic Clock Generator index to disable
 */
void system_gclk_gen_disable(const uint8_t generator)
{
	/* Select the requested generator */
	*((uint8_t*)&GCLK->GENCTRL.reg) = generator;
	_system_gclk_wait_for_sync();

	/* Disable generator */
	GCLK->GENCTRL.reg &= ~GCLK_GENCTRL_GENEN;
	while (GCLK->GENCTRL.reg & GCLK_GENCTRL_GENEN) {
		/* Wait for clock to become disabled */
	}
}

/** \brief Retrieves the clock frequency of a Generic Clock generator.
 *
 *  Determines the clock frequency (in Hz) of a specified Generic Clock
 *  generator, used as a source to a Generic Clock Channel module.
 *
 *  \param[in] generator  Generic Clock Generator index
 *
 *  \return The frequency of the generic clock generator, in Hz.
 */
uint32_t system_gclk_gen_get_hz(const uint8_t generator)
{
	/* Select the appropriate generator */
	*((uint8_t*)&GCLK->GENCTRL.reg) = generator;
	_system_gclk_wait_for_sync();

	/* Get the frequency of the source connected to the GCLK generator */
	uint32_t gen_input_hz = system_clock_source_get_hz(GCLK->GENCTRL.bit.SRC);

	/* Check if the divider is enabled for the generator */
	if (!((GCLK->GENCTRL.reg & GCLK_GENCTRL_DIVSEL) == 1 &&
			GCLK->GENDIV.reg <= 1)) {

		/* Select the appropriate generator division register */
		*((uint8_t*)&GCLK->GENDIV.reg) = generator;
		_system_gclk_wait_for_sync();

		/* Get the generator divider setting (can be fractional or binary) */
		uint32_t divider = GCLK->GENDIV.bit.DIV;

		/* Check if the generator is using fractional or binary division */
		if (GCLK->GENCTRL.bit.DIVSEL) {
			gen_input_hz /= divider;
		}
		else {
			gen_input_hz >>= (divider + 1);
		}
	}

	return gen_input_hz;

}

/** \brief Writes a Generic Clock configuration to the hardware module.
 *
 *  Writes out a given configuration of a Generic Clock configuration to the
 *  hardware module. If the clock is currently running, it will be stopped.
 *
 *  \note Once called the clock will not be running; to start the clock,
 *        call \ref system_gclk_ch_enable() after configuring a clock channel.
 *
 *  \param[in] channel   Generic Clock channel to configure
 *  \param[in] config    Configuration settings for the clock
 */
void system_gclk_ch_set_config(
		const uint8_t channel,
		struct system_gclk_ch_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Cache the new config to reduce sync requirements */
	uint32_t new_clkctrl_config = (channel << GCLK_CLKCTRL_ID_Pos);

	/* Select the desired generic clock generator */
	new_clkctrl_config |= config->source_generator << GCLK_CLKCTRL_GEN_Pos;

	#if !defined (REVB)
	/* Enable or disable the clock in standby mode */
	if (config->enable_during_sleep) {
		new_clkctrl_config |= GCLK_CLKCTRL_RUNSTDBY;
	}
	#endif

	/* Disable generic clock channel */
	system_gclk_ch_disable(channel);

	/* Write the new configuration */
	GCLK->CLKCTRL.reg = new_clkctrl_config;
}

/** \brief Enables a Generic Clock that was previously configured.
 *
 *  Starts the clock generation of a Generic Clock that was previously
 *  configured via a call to \ref system_gclk_ch_set_config().
 *
 *  \param[in] channel   Generic Clock channel to enable
 */
void system_gclk_ch_enable(const uint8_t channel)
{
	/* Select the requested generator channel */
	*((uint8_t*)&GCLK->CLKCTRL.reg) = channel;

	/* Enable the generic clock */
	GCLK->CLKCTRL.reg |= GCLK_CLKCTRL_CLKEN;
}

/** \brief Disables a Generic Clock that was previously enabled.
 *
 *  Stops the clock generation of a Generic Clock that was previously started
 *  via a call to \ref system_gclk_ch_enable().
 *
 *  \param[in] channel   Generic Clock channel to disable
 */
void system_gclk_ch_disable(const uint8_t channel)
{
	/* Select the requested generator channel */
	*((uint8_t*)&GCLK->CLKCTRL.reg) = channel;

	/* Disable the generic clock */
	GCLK->CLKCTRL.reg &= ~GCLK_CLKCTRL_CLKEN;
	while (GCLK->CLKCTRL.reg & GCLK_CLKCTRL_CLKEN) {
		/* Wait for clock to become disabled */
	}
}

/** \brief Retrieves the clock frequency of a Generic Clock channel.
 *
 *  Determines the clock frequency (in Hz) of a specified Generic Clock
 *  channel, used as a source to a device peripheral module.
 *
 *  \param[in] channel  Generic Clock Channel index
 *
 *  \return The frequency of the generic clock channel, in Hz.
 */
uint32_t system_gclk_ch_get_hz(const uint8_t channel)
{
	/* Select the requested generic clock channel */
	*((uint8_t*)&GCLK->CLKCTRL.reg) = channel;

	/* Return the clock speed of the associated GCLK generator */
	return system_gclk_gen_get_hz(GCLK->CLKCTRL.bit.GEN);
}
