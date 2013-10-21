/**
 * \file
 *
 * \brief SAM D2x Clock Driver
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

#ifndef SYSTEM_CLOCK_PRIVATE_INCLUDED
#define SYSTEM_CLOCK_PRIVATE_INCLUDED

#include "clock.h"

/**
 * \internal
 * \brief DFLL-specific data container
 */
struct _system_clock_dfll_config {
	uint32_t control;
	uint32_t val;
	uint32_t mul;
};

/**
 * \internal
 * \brief DPLL-specific data container
 */
struct _system_clock_dpll_config {
	uint32_t frequency;
};


/**
 * \internal
 * \brief XOSC-specific data container
 */
struct _system_clock_xosc_config {
	uint32_t frequency;
};

/**
 * \internal
 * \brief System clock module data container
 */
struct _system_clock_module {
	volatile struct _system_clock_dfll_config dfll;

#ifdef FEATURE_SYSTEM_CLOCK_DPLL
	volatile struct _system_clock_dpll_config dpll;
#endif

	volatile struct _system_clock_xosc_config xosc;
	volatile struct _system_clock_xosc_config xosc32k;
};

/**
 * \internal
 * \brief Internal module instance to cache configuration values
 */
static struct _system_clock_module _system_clock_inst = {
		.dfll = {
			.control     = 0,
			.val     = 0,
			.mul     = 0,
		},

#ifdef FEATURE_SYSTEM_CLOCK_DPLL
		.dpll = {
			.frequency   = 0,
		},
#endif
		.xosc = {
			.frequency   = 0,
		},
		.xosc32k = {
			.frequency   = 0,
		},
	};

/**
 * \internal
 * \brief Wait for sync to the DFLL control registers
 */
static inline void _system_dfll_wait_for_sync(void)
{
	while (!(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY)) {
		/* Wait for DFLL sync */
	}
}

/**
 * \internal
 * \brief Wait for sync to the OSC32K control registers
 */
static inline void _system_osc32k_wait_for_sync(void)
{
	while (!(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_OSC32KRDY)) {
		/* Wait for OSC32K sync */
	}
}

static inline void _system_clock_source_dfll_set_config_errata_9905(void)
{

	/* Disable ONDEMAND mode while writing configurations */
	SYSCTRL->DFLLCTRL.reg = _system_clock_inst.dfll.control & ~SYSCTRL_DFLLCTRL_ONDEMAND;
	_system_dfll_wait_for_sync();

	SYSCTRL->DFLLMUL.reg = _system_clock_inst.dfll.mul;
	SYSCTRL->DFLLVAL.reg = _system_clock_inst.dfll.val;

	/* Write full configuration to DFLL control register */
	SYSCTRL->DFLLCTRL.reg = _system_clock_inst.dfll.control;
}

#endif /* SYSTEM_CLOCK_PRIV_INCLUDED */
