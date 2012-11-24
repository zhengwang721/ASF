/**
 * \file
 *
 * \brief SAM0+ Generic Clock Driver
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

#include <gclk.h>
#include <clock.h>

/** \cond INTERNAL */

/** \internal GCLK MUX group position index from group prefix. */
#define GCLK_MUX_GROUPPOS(prefix)  prefix ## _gp

/** \internal GCLK MUX group mask from group prefix. */
#define GCLK_MUX_GROUPMASK(prefix) prefix ## _gm

/** \internal GCLK MUX selection helper macro. */
#define GCLK_MUX_SELECT(reg, mask_prefix, value) \
	do { \
		reg = (reg & ~GCLK_MUX_GROUPMASK(mask_prefix)) | \
				((value << \
				GCLK_MUX_GROUPPOS(mask_prefix)) & \
				GCLK_MUX_GROUPMASK(mask_prefix)); \
	} \
	while (0)

/** \endcond */

/** \brief Initializes the GCLK driver.
 *
 *  Initializes the Generic Clock module, disabling and resetting all active
 *  Generic Clock Generators and Channels to their power-on default values.
 */
void clock_gclk_init(void)
{
	/* Software reset the module to ensure it is re-initialized correctly */
	GCLK.CTRL = GCLK_CTRL_SWRST_bm;
	while (GCLK.CTRL & GCLK_CTRL_SWRST_bm) {
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
 *        call \ref clock_gclk_gen_enable() after configuring a generator.
 *
 *  \param[in] generator  Generic Clock Generator index to configure
 *  \param[in] config     Configuration settings for the generator
 */
void clock_gclk_gen_set_config(
		const uint8_t generator,
		struct clock_gclk_gen_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Select the appropriate generator in the generator control register */
	GCLK_MUX_SELECT(GCLK.GENCTRL, GCLK_GENCTRL_GENID, generator);

	/* Disable generator */
	GCLK.GENCTRL &= ~GCLK_GENCTRL_CEN_bm;
	while (GCLK.GENCTRL & GCLK_GENCTRL_CEN_bm) {
		/* Wait for clock to become enabled */
	}

	/* Select the requested source clock for the generator */
	GCLK_MUX_SELECT(GCLK.GENCTRL, GCLK_GENCTRL_SRCSEL,
			config->source_clock);

	/* Configure the clock to be either high or low when disabled */
	if (config->high_when_disabled) {
		GCLK.GENCTRL |= GCLK_GENCTRL_OOV_bm;
	} else {
		GCLK.GENCTRL &= ~GCLK_GENCTRL_OOV_bm;
	}

	/* Set division factor */
	if (config->division_factor > 1) {
		/* Select the appropriate generator in the division register */
		GCLK_MUX_SELECT(GCLK.GENDIV, GCLK_GENDIV_GENID, generator);

		/* Clear existing divider settings */
		GCLK.GENDIV &= ~(GCLK_GENDIV_DIV_gm | GCLK_GENCTRL_DIVFN_bm);

		/* Check if division is a power of two */
		if ((config->division_factor & (config->division_factor - 1)) == 0) {
			/* Determine the index of the highest bit set to get the
			 * division factor that must be loaded into the division
			 * register */
			uint32_t div2_count = 0;
			uint32_t mask = (1 << 0);
			for (mask = 1; mask != config->division_factor; mask <<= 1) {
				div2_count++;
			}

			/* Set binary divider power of 2 division factor */
			GCLK.GENDIV |= (div2_count << GCLK_GENDIV_DIV_gp);

			#if !defined (REVB)
			/* Enable binary division */
			GCLK.GENCTRL |= GCLK_GENCTRL_DIVEN_bm;
			GCLK.GENCTRL &= ~(GCLK_GENCTRL_IDC_bm | GCLK_GENCTRL_DIVFN_bm);
			#else
			/* Enable binary division and disable increased duty cycle accuracy*/
			GCLK.GENCTRL &= ~(GCLK_GENCTRL_DIVSEL_bm | GCLK_GENCTRL_IDC_bm);
			#endif

		} else {
			/* Set integer division factor */
			GCLK.GENDIV
				|= (config->division_factor <<
					GCLK_GENDIV_DIV_gp);
			#if defined (REVB)
			/* Enable non-binary division with increased duty cycle accuracy */
			GCLK.GENCTRL |= GCLK_GENCTRL_IDC_bm | GCLK_GENCTRL_DIVSEL_bm;
			#else
			/* Enable non-binary division with increased duty cycle accuracy */
			GCLK.GENCTRL
				|= (GCLK_GENCTRL_DIVEN_bm |
					GCLK_GENCTRL_DIVFN_bm |
					GCLK_GENCTRL_IDC_bm);
			#endif

		}
	} else {
		#if defined (REVB)
		/* Disable clock division, divide by 1 */
		GCLK.GENCTRL &= GCLK_GENCTRL |= GCLK_GENCTRL_DIVSEL_bm;
		GCLK.GENDIV = 1;
		#else
		/* Turn off clock division */
		GCLK.GENCTRL &= ~GCLK_GENCTRL_DIVEN_bm;
		#endif
	}
	#if defined (REVB)
	/* Enable or disable the clock in standby mode */
	if (config->run_in_standby) {
		GCLK.GENCTRL |= GCLK_GENCTRL_RUNSTDBY_bm;
	} else {
		GCLK.GENCTRL &= ~GCLK_GENCTRL_RUNSTDBY_bm;
	}
	#endif
}

/** \brief Enables a Generic Clock Generator that was previously configured.
 *
 *  Starts the clock generation of a Generic Clock Generator that was previously
 *  configured via a call to \ref clock_gclk_gen_set_config().
 *
 *  \param[in] generator  Generic Clock Generator index to enable
 */
void clock_gclk_gen_enable(const uint8_t generator)
{
	/* Select the requested generator */
	GCLK_MUX_SELECT(GCLK.GENCTRL, GCLK_GENCTRL_GENID, generator);

	/* Enable generator generator */
	GCLK.GENCTRL |= GCLK_GENCTRL_CEN_bm;
}

/** \brief Disables a Generic Clock Generator that was previously enabled.
 *
 *  Stops the clock generation of a Generic Clock Generator that was previously
 *  started via a call to \ref clock_gclk_gen_enable().
 *
 *  \param[in] generator  Generic Clock Generator index to disable
 */
void clock_gclk_gen_disable(const uint8_t generator)
{
	/* Select the requested generator */
	GCLK_MUX_SELECT(GCLK.GENCTRL, GCLK_GENCTRL_GENID, generator);

	/* Disable generator */
	GCLK.GENCTRL &= ~GCLK_GENCTRL_CEN_bm;
	while (GCLK.GENCTRL & GCLK_GENCTRL_CEN_bm) {
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
uint32_t clock_gclk_gen_get_hz(const uint8_t generator)
{
	/* Select the appropriate generator */
	GCLK_MUX_SELECT(GCLK.GENCTRL, GCLK_GENCTRL_GENID, generator);

	uint32_t source_clock_index =
			(GCLK.CLKCTRL & GCLK_CLKCTRL_GENID_gm) >> GCLK_CLKCTRL_GENID_gp;

	/* Get the frequency of the source connected to the GCLK generator */
	uint32_t gen_input_hz = clock_source_get_hz(source_clock_index);

	/* Check if the divider is enabled for the generator */
	if (GCLK.GENCTRL & GCLK_GENCTRL_DIVEN_bm) {
		GCLK_MUX_SELECT(GCLK.GENDIV, GCLK_GENDIV_GENID, generator);

		/* Get the generator divider setting (can be fractional or binary) */
		uint32_t divider = (GCLK.GENDIV & GCLK_GENDIV_DIV_gm);

		/* Check if the generator is using fractional or binary division */
		if (GCLK.GENCTRL & GCLK_GENCTRL_DIVFN_bm) {
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
 *        call \ref clock_gclk_ch_enable() after configuring a clock channel.
 *
 *  \param[in] channel   Generic Clock channel to configure
 *  \param[in] config    Configuration settings for the clock
 */
void clock_gclk_ch_set_config(
		const uint8_t channel,
		struct clock_gclk_ch_conf *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Select the requested generic clock channel */
	GCLK_MUX_SELECT(GCLK.CLKCTRL, GCLK_CLKCTRL_CLKID, channel);

	/* Disable generic clock channel */
	GCLK.CLKCTRL &= ~GCLK_CLKCTRL_CEN_bm;
	while (GCLK.CLKCTRL & GCLK_CLKCTRL_CEN_bm) {
		/* Wait for clock to become disabled */
	}

	/* Select the desired generic clock channel */
	GCLK_MUX_SELECT(GCLK.CLKCTRL, GCLK_CLKCTRL_GENID,
			config->source_generator);

	#if !defined (REVB)
	/* Enable or disable the clock in standby mode */
	if (config->enable_during_sleep) {
		GCLK.CLKCTRL |= GCLK_CLKCTRL_SLPMASK_bm;
	} else {
		GCLK.CLKCTRL &= ~GCLK_CLKCTRL_SLPMASK_bm;
	}
	#endif
}

/** \brief Enables a Generic Clock that was previously configured.
 *
 *  Starts the clock generation of a Generic Clock that was previously
 *  configured via a call to \ref clock_gclk_ch_set_config().
 *
 *  \param[in] channel   Generic Clock channel to enable
 */
void clock_gclk_ch_enable(const uint8_t channel)
{
	/* Select the requested generator channel */
	GCLK_MUX_SELECT(GCLK.CLKCTRL, GCLK_CLKCTRL_CLKID, channel);

	/* Enable the generic clock */
	GCLK.CLKCTRL |= GCLK_CLKCTRL_CEN_bm;
}

/** \brief Disables a Generic Clock that was previously enabled.
 *
 *  Stops the clock generation of a Generic Clock that was previously started
 *  via a call to \ref clock_gclk_ch_enable().
 *
 *  \param[in] channel   Generic Clock channel to disable
 */
void clock_gclk_ch_disable(const uint8_t channel)
{
	/* Select the requested generator channel */
	GCLK_MUX_SELECT(GCLK.CLKCTRL, GCLK_CLKCTRL_CLKID, channel);

	/* Disable the generic clock */
	GCLK.CLKCTRL &= ~GCLK_CLKCTRL_CEN_bm;
	while (GCLK.CLKCTRL & GCLK_CLKCTRL_CEN_bm) {
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
uint32_t clock_gclk_ch_get_hz(const uint8_t channel)
{
	/* Select the requested generic clock channel */
	GCLK_MUX_SELECT(GCLK.CLKCTRL, GCLK_CLKCTRL_CLKID, channel);

	/* Return the clock speed of the associated GCLK generator */
	return clock_gclk_gen_get_hz(GCLK.CLKCTRL & GCLK_CLKCTRL_GENID_gm);
}
