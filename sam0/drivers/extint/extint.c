/**
 * \file
 *
 * \brief SAMD20 External Interrupt Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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
#include <extint.h>

static void _eic_wait_for_sync(Eic* module)
{
	while (module->STATUS.reg & EIC_STATUS_SYNCBUSY) {
		/* Wait for sync to complete */
	}
}

/**
 * \brief Resets and disables the External Interrupt driver.
 *
 * Resets and disables the External Interrupt driver, resetting all hardware
 * module registers to their power-on defaults.
 */
void extint_reset(void)
{
	Eic *const eics[EIC_INST_NUM] = EIC_INSTS;

	/* Disable all EIC modules before resetting them. */
	extint_disable();

	/* Reset all EIC hardware modules. */
	for (uint32_t i = 0; i < EIC_INST_NUM; i++) {
		eics[i]->CTRL.reg |= EIC_CTRL_SWRST;
		_eic_wait_for_sync(eics[i]);
	}
}

/**
 * \brief Enables the External Interrupt driver.
 *
 * Enables EIC modules ready for use. This function must be called before
 * attempting to use any NMI or standard external interrupt channel functions.
 */
void extint_enable(void)
{
	Eic *const eics[EIC_INST_NUM] = EIC_INSTS;

	/* Enable all EIC hardware modules. */
	for (uint32_t i = 0; i < EIC_INST_NUM; i++) {
		eics[i]->CTRL.reg |= EIC_CTRL_ENABLE;
		_eic_wait_for_sync(eics[i]);
	}
}

/**
 * \brief Disables the External Interrupt driver.
 *
 * Disables EIC modules that were previously started via a call to
 * \ref eic_enable().
 */
void extint_disable(void)
{
	Eic *const eics[EIC_INST_NUM] = EIC_INSTS;

	/* Disable all EIC hardware modules. */
	for (uint32_t i = 0; i < EIC_INST_NUM; i++) {
		eics[i]->CTRL.reg &= ~EIC_CTRL_ENABLE;
		_eic_wait_for_sync(eics[i]);
	}
}

/**
 * \brief Writes an External Interrupt channel configuration to the hardware module.
 *
 * Writes out a given configuration of an External Interrupt channel
 * configuration to the hardware module. If the channel is already configured,
 * the new configuration will replace the existing one.
 *
 * \param channel   External Interrupt channel to configure
 * \param config    Configuration settings for the channel
 *
 * \returns Status code indicating the success or failure of the request.
 * \retval  STATUS_OK                   Configuration succeeded
 * \retval  STATUS_ERR_PIN_MUX_INVALID  An invalid pin mux value was supplied
 */
enum status_code extint_ch_set_config(
		const uint8_t channel,
		const struct extint_ch_conf *const config)
{
	if (!config->pinmux_position == 0) {
		return STATUS_ERR_PIN_MUX_INVALID;
	}

	struct system_pinmux_conf pinmux_config;
	system_pinmux_get_config_defaults(&pinmux_config);

	pinmux_config.mux_position = (config->pinmux_position & 0xFFFF);
	pinmux_config.direction    = SYSTEM_PINMUX_PIN_DIR_INPUT;
	pinmux_config.input_pull   = SYSTEM_PINMUX_PIN_PULL_UP;

	system_pinmux_pin_set_config(config->pinmux_position >> 16, &pinmux_config);

	// TODO
}

/**
 * \brief Writes an External Interrupt NMI channel configuration to the hardware module.
 *
 *  Writes out a given configuration of an External Interrupt NMI channel
 *  configuration to the hardware module. If the channel is already configured,
 *  the new configuration will replace the existing one.
 *
 *  \param nmi_channel   External Interrupt NMI channel to configure
 *  \param config        Configuration settings for the channel
 *
 * \returns Status code indicating the success or failure of the request.
 * \retval  STATUS_OK                   Configuration succeeded
 * \retval  STATUS_ERR_PIN_MUX_INVALID  An invalid pin mux value was supplied
 */
enum status_code extint_nmi_set_config(
		const uint8_t nmi_channel,
		const struct extint_nmi_conf *const config)
{
	if (!config->pinmux_position == 0) {
		return STATUS_ERR_PIN_MUX_INVALID;
	}

	struct system_pinmux_conf pinmux_config;
	system_pinmux_get_config_defaults(&pinmux_config);

	pinmux_config.mux_position = (config->pinmux_position & 0xFFFF);
	pinmux_config.direction    = SYSTEM_PINMUX_PIN_DIR_INPUT;
	pinmux_config.input_pull   = SYSTEM_PINMUX_PIN_PULL_UP;

	system_pinmux_pin_set_config(config->pinmux_position >> 16, &pinmux_config);

	// TODO
}
