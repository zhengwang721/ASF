/**
 * \file
 *
 * \brief SAM TIMER Driver for SAMB11
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
#include "timer.h"

/**
 * \brief Initializes config with predefined default values.
 *
 * This function will initialize a given TIMER configuration structure to
 * a set of known default values. This function should be called on
 * any new instance of the configuration structures before being
 * modified by the user application.
 *
 * The default configuration is as follows:
 *  \li Timer interrupt set as enable
 *  \li Select external input as clock set as disable
 *  \li Select external input as enable set as disable
 *
 * \param[out]  config  Pointer to a TIMER module configuration structure to set
 */
void timer_get_config_defaults(struct timer_config *config)
{
	config->interrupt_enable = TIMER_CTRL_INTERRUPT_ENABLE;
	config->select_ext_clk = 0;
	config->select_ext_enable = 0;
}

/**
 * \brief Initializes TIMER0 module instance.
 *
 * Initializes the TIMER0 module, based on the given
 * configuration values.
 *
 * \param[in]     config       Pointer to the TIMER configuration options struct
 *
 * \return Status of the initialization procedure.
 */
void timer_init(const struct timer_config *config)
{
	TIMER0->CTRL.reg = config->interrupt_enable
		| config->select_ext_clk
		| config->select_ext_enable;
}

/**
 * \brief Get TIMER0 module current value.
 *
 * \retval Current value
 */
uint32_t timer_get_value(void)
{
	return TIMER0->VALUE.reg;
}

/**
 * \brief Set TIMER0 module value.
 *
 * \param[in]     value        Reload value
 */
void timer_set_value(uint32_t value)
{
	TIMER0->RELOAD.reg = value;
}

/**
 * \brief Get TIMER0 module interrupt status
 *
 * \retval The status of module
 */
uint32_t timer_get_int_status(void)
{
	return TIMER0->INTSTATUSCLEAR.reg;
}

/**
 * \brief Clear TIMER0 module interrupt status
 *
 * Clear the TIMER0 module interrupt status
 */
void timer_clear_int_status(void)
{
	TIMER0->INTSTATUSCLEAR.reg = 1;
}

/**
 * \brief Set TIMER0 module enable
 *
 * Enable the TIMER0 module
 */
void timer_enable(void)
{
	TIMER0->CTRL.reg |= TIMER_CTRL_INTERRUPT_ENABLE;
}

/**
 * \brief Set TIMER0 disable
 *
 * Disable the TIMER0 module
 */
void timer_disable(void)
{
	TIMER0->CTRL.reg &= (~TIMER_CTRL_INTERRUPT_ENABLE);
}