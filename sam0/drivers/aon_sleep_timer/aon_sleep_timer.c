/**
 * \file
 *
 * \brief SAM AON Sleep Timer Driver for SAMB11
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
#include "aon_sleep_timer.h"


static void delay_cycle(uint32_t cycles)
{
	volatile uint32_t i = 0;
	
	for (i=0; i < cycles*100; i++) {
		__NOP();
	}
}


/**
 * \internal Get the set or clear status
 */
static enum aon_sleep_timer_status_code _aon_sleep_timer_get_reload_status(void)
{
	uint32_t regval = AON_SLEEP_TIMER0->CONTROL.bit.SLP_TIMER_CLK_RELOAD_DLY;
	
	switch (regval) {
	case 0:
		return AON_SLEEP_TIMER_CLEAR_COMPLETE;
	case 1:
		return AON_SLEEP_TIMER_SET_PROCESS;
	case 2:
		return AON_SLEEP_TIMER_CLEAR_PROCESS;
	case 3:
		return AON_SLEEP_TIMER_SET_COMPLETE;
	default:
		return AON_SLEEP_TIMER_CLEAR_ERR;
	}
}

/**
 * \internal Get the set or clear status
 */
static enum aon_sleep_timer_status_code _aon_sleep_timer_get_single_status(void)
{
	uint32_t regval = AON_SLEEP_TIMER0->CONTROL.bit.SLP_TIMER_SINGLE_COUNT_ENABLE_DLY;
	
	switch (regval) {
	case 0:
		return AON_SLEEP_TIMER_CLEAR_COMPLETE;
	case 2:
		return AON_SLEEP_TIMER_SET_PROCESS;
	case 4:
		return AON_SLEEP_TIMER_CLEAR_PROCESS;
	case 7:
		return AON_SLEEP_TIMER_SET_COMPLETE;
	default:
		return AON_SLEEP_TIMER_CLEAR_ERR;
	}
}

/**
 * \brief Initializes config with predefined default values.
 *
 * This function will initialize a given AON Sleep Timer configuration structure to
 * a set of known default values. This function should be called on
 * any new instance of the configuration structures before being
 * modified by the user application.
 *
 * \param[out]  config  Pointer to a AON Sleep Timer module configuration structure to set
 */
void aon_sleep_timer_get_config_defaults(struct aon_sleep_timer_config *config)
{
	/* Default configuration values */
	config->wakeup = AON_SLEEP_TIMER_WAKEUP_DIS;
	config->mode = AON_SLEEP_TIMER_SINGLE_MODE;
	config->counter = 32000;
}

/**
 * \brief Initializes AON Sleep Timer module instance.
 *
 * Initializes the AON Sleep Timer module, based on the parameters,
 * and start timer.
 *
 * \param[in]     config       Pointer to the AON Sleep Timer configuration options struct
 *
 */
void aon_sleep_timer_init(const struct aon_sleep_timer_config *config)
{
	uint32_t aon_st_ctrl = 0;

	(*(volatile uint32_t *)(0x4000E00C)) = 0;
	if (config->wakeup == AON_SLEEP_TIMER_WAKEUP_EN) {
		/* Enable ARM wakeup */
		(*(volatile uint32_t *)(0x4000E00C)) = 1;
	}
	
	aon_st_ctrl = AON_SLEEP_TIMER0->CONTROL.reg;
	while (aon_st_ctrl & ((1UL<<31) -1)) {
		AON_SLEEP_TIMER0->CONTROL.reg = 0;
		delay_cycle(3);
		while (aon_st_ctrl & ((config->mode == AON_SLEEP_TIMER_RELOAD_MODE) ?
				(1<<9) : (1<<14))) {
			aon_st_ctrl = AON_SLEEP_TIMER0->CONTROL.reg;
		}
		aon_st_ctrl = AON_SLEEP_TIMER0->CONTROL.reg;
	}
	
	AON_SLEEP_TIMER0->SINGLE_COUNT_DURATION.reg = config->counter;
	if (config->mode == AON_SLEEP_TIMER_RELOAD_MODE) {
		/* Reload counter will start here */
		AON_SLEEP_TIMER0->CONTROL.reg = AON_SLEEP_TIMER_CONTROL_RELOAD_ENABLE;
	} else {
		/* Single counter will start here */
		AON_SLEEP_TIMER0->CONTROL.reg = AON_SLEEP_TIMER_CONTROL_SINGLE_COUNT_ENABLE;
	}
	
	aon_st_ctrl = AON_SLEEP_TIMER0->CONTROL.reg;
	if (config->mode == AON_SLEEP_TIMER_SINGLE_MODE) {
		while (_aon_sleep_timer_get_single_status() != AON_SLEEP_TIMER_SET_COMPLETE);
		AON_SLEEP_TIMER0->CONTROL.reg = 0;
	} else {
		while (_aon_sleep_timer_get_reload_status() != AON_SLEEP_TIMER_SET_COMPLETE);
	}
}

/**
 * \brief Disable AON Sleep Timer module instance.
 *
 * AON Sleep Timer module instance disable.
 */
void aon_sleep_tiemer_disable(void)
{
	uint32_t regval;
	
	AON_SLEEP_TIMER0->SINGLE_COUNT_DURATION.reg = 0;
	regval = AON_SLEEP_TIMER0->CONTROL.reg;
	regval &= ~AON_SLEEP_TIMER_CONTROL_RELOAD_ENABLE;
	regval &= ~AON_SLEEP_TIMER_CONTROL_SINGLE_COUNT_ENABLE;
	AON_SLEEP_TIMER0->CONTROL.reg = regval;
	
	while ((AON_SLEEP_TIMER0->CONTROL.reg & 
			AON_SLEEP_TIMER_CONTROL_SLP_TIMER_SINGLE_COUNT_ENABLE_DLY_Msk)
			!= AON_SLEEP_TIMER_CONTROL_SLP_TIMER_SINGLE_COUNT_ENABLE_DLY_Msk) {
	}
}

/**
 * \brief Get AON Sleep Timer module instance current value.
 *
 * \retval Current value
 */
uint32_t aon_sleep_timer_get_current_value(void)
{
	return AON_SLEEP_TIMER0->CURRENT_COUNT_VALUE.reg;
}

/**
 * \brief If AON Sleep Timer is active
 *
 * \return Active status of the AON Sleep Timer.
 */
bool aon_sleep_timer_sleep_timer_active(void)
{
	return AON_SLEEP_TIMER0->CONTROL.bit.SLEEP_TIMER_ACTIVE;
}

/**
 * \brief If AON Sleep Timer is not active
 *
 * \return Not active status of the AON Sleep Timer.
 */
bool aon_sleep_timer_sleep_timer_not_active(void)
{
	return AON_SLEEP_TIMER0->CONTROL.bit.SLEEP_TIMER_NOT_ACTIVE;
}


/**
 * \brief Clear AON Sleep Timer module instance interrupt.
 *
 * This flag will be cleared automatically once the IRQ 
 * has been seen on the sleep clock.
 */
void aon_sleep_timer_clear_interrup(void)
{
	AON_SLEEP_TIMER0->CONTROL.reg |= AON_SLEEP_TIMER_CONTROL_IRQ_CLEAR;
}