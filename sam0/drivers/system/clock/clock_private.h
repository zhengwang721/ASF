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
