/**
 * \file
 *
 * \brief Sleep mode access
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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

#include <compiler.h>
#include "sleep.h"

#define TRIGGER_SLEEP     0 /* Enable to test clock switching time */
#define TRIGGER_WAKEUP    0 /* Enable to test clock switching time */

#if (TRIGGER_SLEEP || TRIGGER_WAKEUP)
# include <asf.h>
# define PIO_PORT_SIZE ((uint32_t)PIOB-(uint32_t)PIOA)
# define pin_to_port_nb(pin_index) ((pin_index) >> 5)
# define pin_to_pio(pin_index) \
		((Pio*)((uint32_t)PIOA + PIO_PORT_SIZE*pin_to_port_nb(pin_index)))
# define pin_to_mask(pin_index) (1 << ((pin_index) & 0x1F))
# define pin_set(pin_index) do {\
			pin_to_pio(pin_index)->PIO_SODR  = pin_to_mask(pin_index);\
		} while(0)
# define pin_clear(pin_index) do {\
			pin_to_pio(pin_index)->PIO_CODR  = pin_to_mask(pin_index);\
		} while(0)
#  if 1
#   define pin_toggle(pin_index) do {\
			if (pin_to_pio(pin_index)->PIO_ODSR & pin_to_mask(pin_index)) {\
				pin_to_pio(pin_index)->PIO_CODR  = pin_to_mask(pin_index);\
			} else {\
				pin_to_pio(pin_index)->PIO_SODR  = pin_to_mask(pin_index);\
			}\
		} while(0)
#  else
#   define pin_toggle(pin_index) do {\
			pin_to_pio(pin_index)->PIO_ODSR ^= pin_to_mask(pin_index);\
		} while(0)
#  endif
/* 1 str instructon */
# define led_on(led) do {\
			if (led##_ACTIVE_LEVEL) {\
				pin_set(led##_GPIO);\
			} else {\
				pin_clear(led##_GPIO);\
			}\
		} while(0)
/* 1 str instruction */
# define led_off(led) do {\
			if (led##_ACTIVE_LEVEL) {\
				pin_clear(led##_GPIO);\
			} else {\
				pin_set(led##_GPIO);\
			}\
		} while(0)
/* 1 sub, 3 instructions (ld,xor,st) */
#  define led_toggle(led) do {\
			pin_toggle(led##_GPIO);\
		} while(0)
#else
# define led_on(...)
# define led_off(...)
# define led_toggle(...)
#endif
#if TRIGGER_SLEEP
# define sleep_start() led_on(LED1);led_on(LED0)
# define sleep_end()   led_off(LED0);led_off(LED1);
# define sleep_step()  led_toggle(LED0)
#else
# define sleep_start() 
# define sleep_end()   
# define sleep_step()  
#endif
#if TRIGGER_WAKEUP
# define wakeup_start() led_on(LED1);led_on(LED0)
# define wakeup_end()   led_off(LED0);led_off(LED1)
# define wakeup_step()  led_toggle(LED0)
#else
# define wakeup_start() 
# define wakeup_end()   
# define wakeup_step()  
#endif

/* SAM3 and SAM4 series */
#if (SAM3S || SAM3N || SAM3XA || SAM3U || SAM4S || SAM4E || SAM4N)
# include "pmc.h"
# include "board.h"

/* Checking board configuration of main clock xtal statup time */
#if !defined(BOARD_OSC_STARTUP_US)
# warning The board main clock xtal statup time has not been defined. Using default settings.
# define BOARD_OSC_STARTUP_US    (15625UL)
#endif

#if !defined(EFC0)
# define EFC0 EFC
#endif

/**
 * Save clock settings and shutdown PLLs
 */
__always_inline static void pmc_save_clock_settings(
		uint32_t *p_osc_setting,
		uint32_t *p_pll0_setting,
		uint32_t *p_pll1_setting,
		uint32_t *p_mck_setting,
		uint32_t *p_fmr_setting,
#if defined(EFC1)
		uint32_t *p_fmr_setting1,
#endif
		const bool disable_xtal)
{
	uint32_t mor  = PMC->CKGR_MOR;
	uint32_t mckr = PMC->PMC_MCKR;
	uint32_t fmr  = EFC0->EEFC_FMR;
# if defined(EFC1)
	uint32_t fmr1 = EFC1->EEFC_FMR;
# endif

	if (p_osc_setting) {
		*p_osc_setting = mor;
	}
	if (p_pll0_setting) {
		*p_pll0_setting = PMC->CKGR_PLLAR;
	}
	if (p_pll1_setting) {
#if (SAM3S || SAM4S)
		*p_pll1_setting = PMC->CKGR_PLLBR;
#elif (SAM3U || SAM3XA)
		*p_pll1_setting = PMC->CKGR_UCKR;
#else
		*p_pll1_setting = 0;
#endif
	}
	if (p_mck_setting) {
		*p_mck_setting  = mckr;
	}
	if (p_fmr_setting) {
		*p_fmr_setting  = fmr;
	}
#if defined(EFC1)
	if (p_fmr_setting1) {
		*p_fmr_setting1 = fmr1;
	}
#endif
	sleep_step();

	/* Enable FAST RC */
	PMC->CKGR_MOR = PMC_CKGR_MOR_KEY_VALUE | mor | CKGR_MOR_MOSCRCEN;
	/* if MCK source is PLL, switch to mainck */
	if ((mckr & PMC_MCKR_CSS_Msk) > PMC_MCKR_CSS_MAIN_CLK) {
		/* MCK -> MAINCK */
		mckr = (mckr & (~PMC_MCKR_CSS_Msk)) | PMC_MCKR_CSS_MAIN_CLK;
		PMC->PMC_MCKR = mckr;
		while(!(PMC->PMC_SR & PMC_SR_MCKRDY));
		sleep_step();
	}
	/* MCK prescale -> 1 */
	if (mckr & PMC_MCKR_PRES_Msk) {
		mckr = (mckr & (~PMC_MCKR_PRES_Msk));
		PMC->PMC_MCKR = mckr;
		while(!(PMC->PMC_SR & PMC_SR_MCKRDY));
		sleep_step();
	}
	/* Disable PLLs */
	pmc_disable_pllack();
#if (SAM3S || SAM4S)
	pmc_disable_pllbck();
#elif (SAM3U || SAM3XA)
	pmc_disable_upll_clock();
#endif
	sleep_step();

	/* Prepare for entering WAIT mode */
	/* Wait fast RC ready */
	while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));
	sleep_step();
	/* Switch mainck to FAST RC */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCSEL) |
			PMC_CKGR_MOR_KEY_VALUE;
	while (!(PMC->PMC_SR & PMC_SR_MOSCSELS));

	/* FWS update */
	EFC0->EEFC_FMR = fmr & (~EEFC_FMR_FWS_Msk);
#if defined(EFC1)
	EFC1->EEFC_FMR = fmr1 & (~EEFC_FMR_FWS_Msk);
#endif

	sleep_step();
	/* Disable XTALs */
	if (disable_xtal) {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTEN) |
				PMC_CKGR_MOR_KEY_VALUE;
	}
}

/**
 * Restore clock settings
 */
__always_inline static void pmc_restore_clock_setting(
		const uint32_t osc_setting,
		const uint32_t pll0_setting,
		const uint32_t pll1_setting,
		const uint32_t mck_setting,
		const uint32_t fmr_setting
#if defined(EFC1)
		, const uint32_t fmr_setting1
#endif
		)
{
	uint32_t mckr;
	uint32_t pll_sr = 0;

	/* Switch mainck to external xtal */
	if (CKGR_MOR_MOSCXTBY == (osc_setting & CKGR_MOR_MOSCXTBY)) {
		/* Bypass mode */
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTEN) |
				PMC_CKGR_MOR_KEY_VALUE | CKGR_MOR_MOSCXTBY |
				CKGR_MOR_MOSCSEL;
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCRCEN &
					~CKGR_MOR_MOSCRCF_Msk)
				| PMC_CKGR_MOR_KEY_VALUE;
		wakeup_step();
	} else if (CKGR_MOR_MOSCXTEN == (osc_setting & CKGR_MOR_MOSCXTEN)) {
		/* Enable External XTAL */
		if (!(PMC->CKGR_MOR & CKGR_MOR_MOSCXTEN)) {
			PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTBY) |
					PMC_CKGR_MOR_KEY_VALUE | CKGR_MOR_MOSCXTEN;
			/* Wait the Xtal to stabilize */
			while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));
			wakeup_step();
		}
		/* Select External XTAL */
		if (!(PMC->CKGR_MOR & CKGR_MOR_MOSCSEL)) {
			PMC->CKGR_MOR |= PMC_CKGR_MOR_KEY_VALUE | CKGR_MOR_MOSCSEL;
			while (!(PMC->PMC_SR & PMC_SR_MOSCSELS));
			wakeup_step();
		}
		/* Disable Fast RC */
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCRCEN &
						~CKGR_MOR_MOSCRCF_Msk)
					| PMC_CKGR_MOR_KEY_VALUE;
	}

	if (pll0_setting & CKGR_PLLAR_MULA_Msk) {
		PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | pll0_setting;
		pll_sr |= PMC_SR_LOCKA;
	}
#if (SAM3S || SAM4S)
	if (pll1_setting & CKGR_PLLBR_MULB_Msk) {
		PMC->CKGR_PLLBR = pll1_setting;
		pll_sr |= PMC_SR_LOCKB;
	}
#elif (SAM3U || SAM3XA)
	if (pll1_setting & CKGR_UCKR_UPLLEN) {
		PMC->CKGR_UCKR = pll1_setting;
		pll_sr |= PMC_SR_LOCKU;
	}
#else
	UNUSED(pll1_setting);
#endif
	/* Wait MCK source ready */
	switch(mck_setting & PMC_MCKR_CSS_Msk) {
	case PMC_MCKR_CSS_PLLA_CLK:
		while (!(PMC->PMC_SR & PMC_SR_LOCKA));
		break;
#if (SAM3S || SAM4S)
	case PMC_MCKR_CSS_PLLB_CLK:
		while (!(PMC->PMC_SR & PMC_SR_LOCKB));
		break;
#elif (SAM3U || SAM3XA)
	case PMC_MCKR_CSS_UPLL_CLK:
		while (!(PMC->PMC_SR & PMC_SR_LOCKU));
		break;
#endif
	}
	wakeup_step();

	/* Switch to faster clock */
	mckr = PMC->PMC_MCKR;

	/* Set PRES */
	PMC->PMC_MCKR = (mckr & ~PMC_MCKR_PRES_Msk)
		| (mck_setting & PMC_MCKR_PRES_Msk);
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
	wakeup_step();

	/* Restore flash wait states */
	EFC0->EEFC_FMR = fmr_setting;
#if defined(EFC1)
	EFC1->EEFC_FMR = fmr_setting1;
#endif

	/* Set CSS and others */
	PMC->PMC_MCKR = mck_setting;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
	wakeup_step();

	/* Waiting all restored PLLs ready */
	while (!(PMC->PMC_SR & pll_sr));
}

/** If clocks are switched for some sleep mode */
static volatile bool b_is_sleep_clock_used = false;
/** Callback invoked once when clocks are restored */
static pmc_callback_wakeup_clocks_restored_t callback_clocks_restored = NULL;

void pmc_sleep(int sleep_mode)
{
	switch (sleep_mode) {
	case SAM_PM_SMODE_SLEEP_WFI:
	case SAM_PM_SMODE_SLEEP_WFE:
#if (SAM4S || SAM4E || SAM4N)
		SCB->SCR &= (uint32_t)~SCR_SLEEPDEEP;
		cpu_irq_enable();
		__WFI();
		break;
#else
		PMC->PMC_FSMR &= (uint32_t)~PMC_FSMR_LPM;
		SCB->SCR &= (uint32_t)~SCR_SLEEPDEEP;
		cpu_irq_enable();
		if (sleep_mode == SAM_PM_SMODE_SLEEP_WFI)
			__WFI();
		else
			__WFE();
		break;
#endif
	case SAM_PM_SMODE_WAIT_FAST:
	case SAM_PM_SMODE_WAIT: {
		uint32_t mor, pllr0, pllr1, mckr;
		uint32_t fmr;
#if defined(EFC1)
		uint32_t fmr1;
#endif

		sleep_start();
		cpu_irq_disable();
		b_is_sleep_clock_used = true;
		pmc_save_clock_settings(&mor, &pllr0, &pllr1, &mckr, &fmr,
#if defined(EFC1)
				&fmr1,
#endif
				(sleep_mode == SAM_PM_SMODE_WAIT));

		/* Enter wait mode */
		cpu_irq_enable();
		sleep_end();

		pmc_enable_waitmode();

		wakeup_start();
		cpu_irq_disable();
		pmc_restore_clock_setting(mor, pllr0, pllr1, mckr, fmr
#if defined(EFC1)
				, fmr1
#endif
				);
		b_is_sleep_clock_used = false;
		if (callback_clocks_restored) {
			callback_clocks_restored();
			callback_clocks_restored = NULL;
		}
		cpu_irq_enable();
		wakeup_end();

		break;
	}

	case SAM_PM_SMODE_BACKUP:
		SCB->SCR |= SCR_SLEEPDEEP;
#if (SAM4S || SAM4E || SAM4N)
		SUPC->SUPC_CR = SUPC_CR_KEY(0xA5u) | SUPC_CR_VROFF_STOP_VREG;
		cpu_irq_enable();
		__WFI() ;
#else
		cpu_irq_enable();
		__WFE() ;
#endif
		break;
	}
}

bool pmc_is_wakeup_clocks_restored(void)
{
	return !b_is_sleep_clock_used;
}

void pmc_wait_wakeup_clocks_restore(
		pmc_callback_wakeup_clocks_restored_t callback)
{
	if (b_is_sleep_clock_used) {
		cpu_irq_disable();
		callback_clocks_restored = callback;
	} else if (callback) {
		callback();
	}
}

#endif /* #if (SAM3S || SAM3N || SAM3XA || SAM3U || SAM4S || SAM4E || SAM4N) */
