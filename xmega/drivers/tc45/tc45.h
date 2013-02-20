/**
 * \file
 *
 * \brief AVR XMEGA Timer Counter type 4 or 5 (TC4/5) driver
 *
 * Copyright (c) 2012-2013 Atmel Corporation. All rights reserved.
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
#ifndef TC45_H
#define TC45_H

#include "asf.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Fix XMEGA IAR header files
 */
#if defined(__IAR_SYSTEMS_ICC__)
/* Fault A Halt Action Selection */
typedef enum FAULT_HALTA_enum
{
	FAULT_HALTA_DISABLE_gc = (0x00<<0),  /* Halt Action Disabled */
	FAULT_HALTA_HW_gc = (0x01<<0),       /* Hardware Halt Action */
	FAULT_HALTA_SW_gc = (0x02<<0),       /* Software Halt Action */
} FAULT_HALTA_t;


/* Fault B Halt Action Selection */
typedef enum FAULT_HALTB_enum
{
	FAULT_HALTB_DISABLE_gc = (0x00<<0),  /* Halt Action Disabled */
	FAULT_HALTB_HW_gc = (0x01<<0),       /* Hardware Halt Action */
	FAULT_HALTB_SW_gc = (0x02<<0),       /* Software Halt Action */
} FAULT_HALTB_t;

/* Channel index Command */
typedef enum FAULT_IDXCMD_enum
{
	FAULT_IDXCMD_DISABLE_gc = (0x00<<0),  /* Command Disabled */
	FAULT_IDXCMD_SET_gc = (0x01<<0),      /* Force Cycle B in Next Cycle */
	FAULT_IDXCMD_CLEAR_gc = (0x02<<0),    /* Force Cycle A in Next Cycle */
	FAULT_IDXCMD_HOLD_gc = (0x03<<0),     /* Hold Current Cycle Index in Next Cycle  */
} FAULT_IDXCMD_t;
#endif

/**
 * \defgroup tc45_group Timer Counter type 4/5(TC4/5)
 *
 * This is a driver for the AVR XMEGA Timer Counter type 4 and 5(TC4/5).
 * It provides functions for enabling, disabling and configuring the TC4/5
 * modules.
 *
 * \section dependencies Dependencies
 * This driver depends on the following modules:
 * - \ref sysclk_group for peripheral clock control.
 * - \ref sleepmgr_group for setting allowed sleep mode.
 * - \ref interrupt_group for ISR definition and disabling interrupts during
 * critical code sections.
 * @{
 */

/**
 * \brief Interrupt event callback function type
 *
 * The interrupt handler can be configured to do a function callback,
 * the callback function must match the tc45_callback_t type.
 *
 */
typedef void (*tc45_callback_t)(void);

/* ! Timer Counter Capture Compare Channel index */
enum tc45_cc_channel_t {
	/* ! Channel A */
	TC45_CCA = 1,
	/* ! Channel B */
	TC45_CCB = 2,
	/* ! Channel C */
	TC45_CCC = 3,
	/* ! Channel D */
	TC45_CCD = 4,
};

/* !  Timer Counter Capture Compare Channel index */
enum tc45_cc_channel_mask_enable_t {
	/* ! Channel A Enable mask */
	TC45_CCACOMP = TC45_CCAMODE_COMP_gc,
	TC45_CCACAPT = TC45_CCAMODE_CAPT_gc,
	/* ! Channel B Enable mask */
	TC45_CCBCOMP = TC45_CCBMODE_COMP_gc,
	TC45_CCBCAPT = TC45_CCCMODE_CAPT_gc,
	/* ! Channel C Enable mask */
	TC45_CCCCOMP = TC45_CCCMODE_COMP_gc,
	TC45_CCCCAPT = TC45_CCCMODE_CAPT_gc,
	/* ! Channel D Enable mask */
	TC45_CCDCOMP = TC45_CCDMODE_COMP_gc,
	TC45_CCDCAPT = TC45_CCDMODE_CAPT_gc,
};

/* ! Timer Counter Direction */
enum tc45_dir_t {
	/* ! Counting up */
	TC45_UP = 0,
	/* ! Down Counting B */
	TC45_DOWN = 1
};

/* ! Timer Counter Waveform Generator mode */
enum tc45_wg_mode_t {
	/* ! TC in normal Mode */
	TC45_WG_NORMAL = TC45_WGMODE_NORMAL_gc,
	/* ! TC in Frequency Generator mode */
	TC45_WG_FRQ = TC45_WGMODE_FRQ_gc,
	/* ! TC in single slope PWM mode */
	TC45_WG_SS = TC45_WGMODE_SINGLESLOPE_gc,
	/* ! TC in dual slope Top PWM mode */
	TC45_WG_DS_T = TC45_WGMODE_DSTOP_gc,
	/* ! TC in dual slope Top Bottom PWM mode */
	TC45_WG_DS_TB = TC45_WGMODE_DSBOTH_gc,
	/* ! TC in dual slope Bottom PWM mode */
	TC45_WG_DS_B = TC45_WGMODE_DSBOTTOM_gc
};

/* ! Fault input A selection */
enum fault_sourceA_mode_t {
	/* ! OTMX in normal Mode */
	FAULT_SRCA_DEFAULT = FAULT_SRCA_DISABLE_gc,
	/* ! OTMX in normal Mode */
	FAULT_SRCA_N = FAULT_SRCA_CHN_gc,
	/* ! OTMX in normal Mode */
	FAULT_SRCA_N1 = FAULT_SRCA_CHN1_gc,
	/* ! OTMX in normal Mode */
	FAULT_SRCA_NB = FAULT_SRCA_LINK_gc,
};

/* ! Fault input B selection */
enum fault_sourceB_mode_t {
	/* ! OTMX in normal Mode */
	FAULT_SRCB_DEFAULT = FAULT_SRCB_DISABLE_gc,
	/* ! OTMX in normal Mode */
	FAULT_SRCB_N = FAULT_SRCB_CHN_gc,
	/* ! OTMX in normal Mode */
	FAULT_SRCB_N1 = FAULT_SRCB_CHN1_gc,
	/* ! OTMX in normal Mode */
	FAULT_SRCB_NB = FAULT_SRCB_LINK_gc,
};

/* ! Fault input E selection */
enum fault_sourceE_mode_t {
	/* ! OTMX in normal Mode */
	FAULT_SRCE_DEFAULT = FAULT_SRCE_DISABLE_gc,
	/* ! OTMX in normal Mode */
	FAULT_SRCE_N = FAULT_SRCE_CHN_gc,
	/* ! OTMX in normal Mode */
	FAULT_SRCE_N1 = FAULT_SRCE_CHN1_gc,
	/* ! OTMX in normal Mode */
	FAULT_SRCE_N2 = FAULT_SRCE_CHN2_gc,
};

/* ! Fault Halt A selection */
enum fault_haltA_mode_t {
	/* ! OTMX in normal Mode */
	FAULT_HALTA_DEFAULT = FAULT_HALTA_DISABLE_gc,
	/* ! OTMX in normal Mode */
	FAULT_HALTA_HW = FAULT_HALTA_HW_gc,
	/* ! OTMX in normal Mode */
	FAULT_HALTA_SW = FAULT_HALTA_SW_gc,
};

/* ! Fault Halt B selection */
enum fault_haltB_mode_t {
	/* ! OTMX in normal Mode */
	FAULT_HALTB_DEFAULT = FAULT_HALTB_DISABLE_gc,
	/* ! OTMX in normal Mode */
	FAULT_HALTB_HW = FAULT_HALTB_HW_gc,
	/* ! OTMX in normal Mode */
	FAULT_HALTB_SW = FAULT_HALTB_SW_gc,
};

/* ! Wex Output Matrix mode */
enum wex_otmx_mode_t {
	/* ! OTMX in normal Mode */
	WEX_OTMX_DEFAULT = WEX_OTMX_DEFAULT_gc,
	/* ! OTMX in normal Mode */
	WEX_OTMX_1 = WEX_OTMX_FIRST_gc,
	/* ! OTMX in normal Mode */
	WEX_OTMX_2 = WEX_OTMX_SECOND_gc,
	/* ! OTMX in normal Mode */
	WEX_OTMX_3 = WEX_OTMX_THIRD_gc,
	/* ! OTMX in normal Mode */
	WEX_OTMX_4 = WEX_OTMX_FOURTH_gc,
};

/* ! TC interrupt levels */
enum TC45_INT_LEVEL_t {
	TC45_INT_LVL_OFF = 0x00,
	TC45_INT_LVL_LO = 0x01,
	TC45_INT_LVL_MED = 0x02,
	TC45_INT_LVL_HI = 0x03,
};

/* ! Macro to check if type of passed TC is TC5_t */
#define tc45_is_TC5(void) ((uint16_t)tc & 0x40 ? true : false)
/* ! Macro to check if type of passed TC is TC4_t */
#define tc45_is_TC4(void) ((uint16_t)tc & 0x40 ? false : true)

/**
 * \brief Enable TC
 *
 * Enables the TC.
 *
 * \param tc Pointer to TC module
 *
 * \note
 * unmask TC clock (sysclk), but does not configure the TC clock source.
 */
void tc45_enable(volatile void *tc);

/**
 * \brief Disable TC
 *
 * Disables the TC.
 *
 * \param tc Pointer to TC module
 *
 * \note
 * mask TC clock (sysclk).
 */
void tc45_disable(volatile void *tc);

/**
 * \ingroup tc45_group
 * \defgroup tc45_interrupt_group Timer Counter (TC) interrupt management
 * This group provides functions to configure TC module interrupts
 *
 *
 * @{
 */

/**
 * \brief Set TC overflow interrupt callback function
 *
 * This function allows the caller to set and change the interrupt callback
 * function. Without setting a callback function the interrupt handler in the
 * driver will only clear the interrupt flags.
 *
 * \note Once a callback function is set, the interrupt priority must be set
 *       via \ref tc45_set_overflow_interrupt_level() for interrupts to be generated
 *       each time the timer overflow event occurs.
 *
 * \param tc Pointer to the Timer Counter (TC) base address
 * \param callback Reference to a callback function
 */
void tc45_set_overflow_interrupt_callback(volatile void *tc,
		tc45_callback_t callback);

/**
 * \brief Set TC error interrupt callback function
 *
 * This function allows the caller to set and change the interrupt callback
 * function. Without setting a callback function the interrupt handler in the
 * driver will only clear the interrupt flags.
 *
 * \note Once a callback function is set, the interrupt priority must be set
 *       via \ref tc45_set_error_interrupt_level() for interrupts to be
 *       generated each time a timer error occurs.
 *
 * \param tc Pointer to the Timer Counter (TC) base address
 * \param callback Reference to a callback function
 */
void tc45_set_error_interrupt_callback(volatile void *tc,
		tc45_callback_t callback);

/**
 * \brief Set TC Capture Compare Channel A interrupt callback function
 *
 * This function allows the caller to set and change the interrupt callback
 * function. Without setting a callback function the interrupt handler in the
 * driver will only clear the interrupt flags.
 *
 * \note Once a callback function is set, the interrupt priority must be set
 *       via \ref tc45_set_cca_interrupt_level() for interrupts to be generated
 *       each time the timer channel A compare matches the current timer count.
 *
 * \param tc Pointer to the Timer Counter (TC) base address
 * \param callback Reference to a callback function
 */
void tc45_set_cca_interrupt_callback(volatile void *tc,
		tc45_callback_t callback);

/**
 * \brief Set TC Capture Compare Channel B interrupt callback function
 *
 * This function allows the caller to set and change the interrupt callback
 * function. Without setting a callback function the interrupt handler in the
 * driver will only clear the interrupt flags.
 *
 * \note Once a callback function is set, the interrupt priority must be set
 *       via \ref tc45_set_ccb_interrupt_level() for interrupts to be generated
 *       each time the timer channel B compare matches the current timer count.
 *
 * \param tc Pointer to the Timer Counter (TC) base address
 * \param callback Reference to a callback function
 */
void tc45_set_ccb_interrupt_callback(volatile void *tc,
		tc45_callback_t callback);

/**
 * \brief Set TC Capture Compare Channel C interrupt callback function
 *
 * This function allows the caller to set and change the interrupt callback
 * function. Without setting a callback function the interrupt handler in the
 * driver will only clear the interrupt flags.
 *
 * \note Once a callback function is set, the interrupt priority must be set
 *       via \ref tc45_set_ccc_interrupt_level() for interrupts to be generated
 *       each time the timer channel C compare matches the current timer count.
 *
 * \param tc Pointer to the Timer Counter (TC) base address
 * \param callback Reference to a callback function
 */
void tc45_set_ccc_interrupt_callback(volatile void *tc,
		tc45_callback_t callback);

/**
 * \brief Set TC Capture Compare Channel D interrupt callback function
 *
 * This function allows the caller to set and change the interrupt callback
 * function. Without setting a callback function the interrupt handler in the
 * driver will only clear the interrupt flags.
 *
 * \note Once a callback function is set, the interrupt priority must be set
 *       via \ref tc45_set_ccd_interrupt_level() for interrupts to be generated
 *       each time the timer channel D compare matches the current timer count.
 *
 * \param tc Pointer to the Timer Counter (TC) base address
 * \param callback Reference to a callback function
 */
void tc45_set_ccd_interrupt_callback(volatile void *tc,
		tc45_callback_t callback);

/**
 * \brief Configures TC overflow Interrupt level
 *
 * \param tc Pointer to TC module.
 * \param level Overflow interrupt level
 * \note  Configures OVFINTLVL in INTCTRLA
 */
static inline void tc45_set_overflow_interrupt_level(volatile void *tc,
		enum TC45_INT_LEVEL_t level)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->INTCTRLA = ((TC4_t *)tc)->INTCTRLA &
				~TC4_OVFINTLVL_gm;
		((TC4_t *)tc)->INTCTRLA
			= ((TC4_t *)tc)->INTCTRLA | (level << TC4_OVFINTLVL_gp);
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->INTCTRLA = ((TC5_t *)tc)->INTCTRLA &
				~TC5_OVFINTLVL_gm;
		((TC5_t *)tc)->INTCTRLA
			= ((TC5_t *)tc)->INTCTRLA | (level << TC5_OVFINTLVL_gp);
	}
}

/**
 * \brief Configures TC error Interrupt level
 *
 * \param tc Pointer to TC module.
 * \param level Error interrupt level
 * \note  Configures ERRINTLVL in INTCTRLA
 */
static inline void tc45_set_error_interrupt_level(volatile void *tc,
		enum TC45_INT_LEVEL_t level)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->INTCTRLA = ((TC4_t *)tc)->INTCTRLA &
				~TC4_ERRINTLVL_gm;
		((TC4_t *)tc)->INTCTRLA
			= ((TC4_t *)tc)->INTCTRLA | (level << TC4_ERRINTLVL_gp);
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->INTCTRLA = ((TC5_t *)tc)->INTCTRLA &
				~TC5_ERRINTLVL_gm;
		((TC5_t *)tc)->INTCTRLA
			= ((TC5_t *)tc)->INTCTRLA | (level << TC5_ERRINTLVL_gp);
	}
}

/**
 * \brief Configures TC Capture Compare A Interrupt level
 *
 * \param tc Pointer to TC module.
 * \param level CCA interrupt level
 * \note Configures CCAINTLVL in INTCTRLB
 */
static inline void tc45_set_cca_interrupt_level(volatile void *tc,
		enum TC45_INT_LEVEL_t level)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->INTCTRLB = ((TC4_t *)tc)->INTCTRLB &
				~TC4_CCAINTLVL_gm;
		((TC4_t *)tc)->INTCTRLB
			= ((TC4_t *)tc)->INTCTRLB | (level << TC4_CCAINTLVL_gp);
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->INTCTRLB = ((TC5_t *)tc)->INTCTRLB &
				~TC5_CCAINTLVL_gm;
		((TC5_t *)tc)->INTCTRLB
			= ((TC5_t *)tc)->INTCTRLB | (level << TC5_CCAINTLVL_gp);
	}
}

/**
 * \brief Configures TC Capture Compare B Interrupt level
 *
 * \param tc Pointer to TC module.
 * \param level CCB interrupt level
 * \note Configures CCBINTLVL in INTCTRLB
 */
static inline void tc45_set_ccb_interrupt_level(volatile void *tc,
		enum TC45_INT_LEVEL_t level)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->INTCTRLB = ((TC4_t *)tc)->INTCTRLB &
				~TC4_CCBINTLVL_gm;
		((TC4_t *)tc)->INTCTRLB
			= ((TC4_t *)tc)->INTCTRLB | (level << TC4_CCBINTLVL_gp);
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->INTCTRLB = ((TC5_t *)tc)->INTCTRLB &
				~TC5_CCBINTLVL_gm;
		((TC5_t *)tc)->INTCTRLB
			= ((TC5_t *)tc)->INTCTRLB | (level << TC5_CCBINTLVL_gp);
	}
}

/**
 * \brief Configures TC Capture Compare C Interrupt level
 *
 * \param tc Pointer to TC module.
 * \param level CCC interrupt level
 * \note Configures CCCINTLVL in INTCTRLB
 */
static inline void tc45_set_ccc_interrupt_level(volatile void *tc,
		enum TC45_INT_LEVEL_t level)
{
	((TC4_t *)tc)->INTCTRLB = ((TC4_t *)tc)->INTCTRLB & ~TC4_CCCINTLVL_gm;
	((TC4_t *)tc)->INTCTRLB
		= ((TC4_t *)tc)->INTCTRLB | (level << TC4_CCCINTLVL_gp);
}

/**
 * \brief Configures TC Capture Compare D Interrupt level
 *
 * \param tc Pointer to TC module.
 * \param level CCD interrupt level
 * \note Configures CCDINTLVL in INTCTRLB
 */
static inline void tc45_set_ccd_interrupt_level(volatile void *tc,
		enum TC45_INT_LEVEL_t level)
{
	((TC4_t *)tc)->INTCTRLB = ((TC4_t *)tc)->INTCTRLB & ~TC4_CCDINTLVL_gm;
	((TC4_t *)tc)->INTCTRLB
		= ((TC4_t *)tc)->INTCTRLB | (level << TC4_CCDINTLVL_gp);
}

/* @} */

/**
 * \brief Configures Synchronization on prescaler
 *
 * \param tc Pointer to TC module.
 * \param
 */
static inline void tc45_write_synchen(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLA
			= (((TC4_t *)tc)->CTRLA | 0x40);
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLA
			= (((TC5_t *)tc)->CTRLA | 0x40);
	}
}

/**
 * \brief Set Evstart bit
 *
 * \param tc Pointer to TC module.
 * \note
 */
static inline void tc45_set_evstart(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLA = (((TC4_t *)tc)->CTRLA | TC4_EVSTART_bm);
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLA = (((TC5_t *)tc)->CTRLA | TC5_EVSTART_bm);
	}
}

/**
 * \brief Set Upstop bit
 *
 * \param tc Pointer to TC module.
 * \note
 */
static inline void tc45_set_up3(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLA = (((TC4_t *)tc)->CTRLA | TC4_UPSTOP_bm);
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLA = (((TC5_t *)tc)->CTRLA | TC5_UPSTOP_bm);
	}
}

/**
 * \brief Set Stop bit
 *
 * \param tc Pointer to TC module.
 * \note
 */
static inline void tc45_set_stop(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLGSET = TC4_STOP_bm;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLGSET = TC5_STOP_bm;
	}
}

/**
 * \brief Configure Timer Clock Source
 *
 * \param tc Pointer to TC module.
 * \param TC45_CLKSEL_enum Clock source selection
 * \note Configuring the clock starts alos the timer
 */
static inline void tc45_write_clock_source(volatile void *tc,
		TC45_CLKSEL_t TC45_CLKSEL_enum)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLA
			= (((TC4_t *)tc)->CTRLA & ~TC4_CLKSEL_gm) |
				TC45_CLKSEL_enum;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLA
			= (((TC5_t *)tc)->CTRLA & ~TC5_CLKSEL_gm) |
				TC45_CLKSEL_enum;
	}
}

/**
 * \brief Read Timer Clock Source
 *
 * \param tc Pointer to TC module.
 * \return TC45_CLKSEL_enum Clock source selection
 */
static inline TC45_CLKSEL_t tc45_read_clock_source(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		return (TC45_CLKSEL_t)(((TC4_t *)tc)->CTRLA & TC4_CLKSEL_gm);
	} else if (tc45_is_TC5(void *tc)) {
		return (TC45_CLKSEL_t)(((TC5_t *)tc)->CTRLA & TC5_CLKSEL_gm);
	}
	Assert(false);
	return TC45_CLKSEL_OFF_gc;
}

/**
 * \brief Select clock for a specified TC and resolution.
 *
 * This function configures the clock selection, as prescaled CLKper, for a
 * specified TC that gives a resolution at least as high as the one specified.
 * The resolution of a TC is synonymous with its clock frequency.
 *
 * \note It is also possible to clock TCs with event channels. This is not
 * handled by this implementation.
 *
 * \param tc   ID of TC to get clock selection for.
 * \param resolution Desired resolution for the TC in Hz.
 */
static inline void tc45_set_resolution(volatile void *tc, uint32_t resolution)
{
	uint32_t tc45_clk_rate = sysclk_get_per_hz();

	if (resolution <= (tc45_clk_rate / 1024)) {
		tc45_write_clock_source(tc, TC45_CLKSEL_DIV1024_gc);
	} else if (resolution <= (tc45_clk_rate / 256)) {
		tc45_write_clock_source(tc, TC45_CLKSEL_DIV256_gc);
	} else if (resolution <= (tc45_clk_rate / 64)) {
		tc45_write_clock_source(tc, TC45_CLKSEL_DIV64_gc);
	} else if (resolution <= (tc45_clk_rate / 8)) {
		tc45_write_clock_source(tc, TC45_CLKSEL_DIV8_gc);
	} else if (resolution <= (tc45_clk_rate / 4)) {
		tc45_write_clock_source(tc, TC45_CLKSEL_DIV4_gc);
	} else if (resolution <= (tc45_clk_rate / 2)) {
		tc45_write_clock_source(tc, TC45_CLKSEL_DIV2_gc);
	} else {
		tc45_write_clock_source(tc, TC45_CLKSEL_DIV1_gc);
	}
}

/**
 * \brief Get real resolution for a specified TC.
 *
 * This function returns the resolution which the specified clock selection
 * of TC will result in. The resolution of a TC is synonymous with its clock
 * frequency.
 *
 * \note This function does not handle event channel clock selections.
 *
 * \param tc Pointer of TC module to get resolution for.
 *
 * \return The resolution of \a tc.
 */
static inline uint32_t tc45_get_resolution(volatile void *tc)
{
	uint32_t tc45_clk_rate = sysclk_get_per_hz();
	switch (tc45_read_clock_source(tc)) {
	case TC45_CLKSEL_OFF_gc:
		tc45_clk_rate = 0;
		break;

	case TC45_CLKSEL_DIV1024_gc:
		tc45_clk_rate /= 1024;
		break;

	case TC45_CLKSEL_DIV256_gc:
		tc45_clk_rate /= 256;
		break;

	case TC45_CLKSEL_DIV64_gc:
		tc45_clk_rate /= 64;
		break;

	case TC45_CLKSEL_DIV8_gc:
		tc45_clk_rate /= 8;
		break;

	case TC45_CLKSEL_DIV4_gc:
		tc45_clk_rate /= 4;
		break;

	case TC45_CLKSEL_DIV2_gc:
		tc45_clk_rate /= 2;
		break;

	case TC45_CLKSEL_DIV1_gc:
		break;

	default:
		tc45_clk_rate = 0;
		break;
	}
	return (tc45_clk_rate);
}

/**
 * \brief Configure Timer Direction
 *
 * \param tc Pointer to TC module.
 * \param dir Timer direction :
 */
static inline void tc45_set_direction(volatile void *tc, enum tc45_dir_t dir)
{
	if (dir == TC45_UP) {
		if (tc45_is_TC4(void *tc)) {
			((TC4_t *)tc)->CTRLGCLR = TC4_DIR_bm;
		} else if (tc45_is_TC5(void *tc)) {
			((TC5_t *)tc)->CTRLGCLR = TC4_DIR_bm;
		}
	} else {
		if (tc45_is_TC4(void *tc)) {
			((TC4_t *)tc)->CTRLGSET = TC4_DIR_bm;
		} else if (tc45_is_TC5(void *tc)) {
			((TC5_t *)tc)->CTRLGSET = TC5_DIR_bm;
		}
	}
}

/**
 * \brief Write the Counter value of the Timer
 *
 * \param tc Pointer to TC module.
 * \param cnt_value Counter value :
 */
static inline void tc45_write_count(volatile void *tc, uint16_t cnt_value)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CNT = cnt_value;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CNT = cnt_value;
	}
}

/**
 * \brief Reads the Counter value of the Timer
 *
 * \param tc Pointer to TC module.
 * \note Output the Counter value CNT
 */
static inline uint16_t tc45_read_count(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		return (((TC4_t *)tc)->CNT);
	} else if (tc45_is_TC5(void *tc)) {
		return (((TC5_t *)tc)->CNT);
	}
	Assert(false);
	return 0;
}

/**
 * \brief Writes the Period value of the Timer
 *
 * \param tc Pointer to TC module.
 * \param per_value Period value : PER
 */
static inline void tc45_write_period(volatile void *tc, uint16_t per_value)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->PER = per_value;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->PER = per_value;
	}
}

/**
 * \brief Reads the Period value of the Timer
 *
 * \param tc Pointer to TC module.
 * \return Period value : PER
 */
static inline uint16_t tc45_read_period(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		return (((TC4_t *)tc)->PER);
	} else if (tc45_is_TC5(void *tc)) {
		return (((TC5_t *)tc)->PER);
	}
	Assert(false);
	return 0;
}

/**
 * \brief Writes the Period Buffer value of the Timer
 *
 * \param tc Pointer to TC module.
 * \param per_buf Period Buffer value : PERH/PERL
 */
static inline void tc45_write_period_buffer(volatile void *tc, uint16_t per_buf)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->PERBUF = per_buf;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->PERBUF = per_buf;
	}
}

/**
 * \brief Reads the Period Buffer value of the Timer
 *
 * \param tc Pointer to TC module.
 * \return Period Buffer value : PERH/PERL
 */
static inline uint16_t tc45_read_period_buffer(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		return (((TC4_t *)tc)->PERBUF);
	} else if (tc45_is_TC5(void *tc)) {
		return (((TC5_t *)tc)->PERBUF);
	}
	Assert(false);
	return 0;
}

/**
 * \brief Tests if the Period Buffer is valid
 *
 * \param tc Pointer to TC module.
 * \return  period Buffer is valid or not:PERBV
 */
static inline bool tc45_period_buffer_is_valid(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		return (((TC4_t *)tc)->CTRLHCLR & TC4_PERBV_bm);
	} else if (tc45_is_TC5(void *tc)) {
		return (((TC5_t *)tc)->CTRLHCLR & TC5_PERBV_bm);
	}
	Assert(false);
	return false;
}

/**
 * \brief Enables delay (used for 32bit timer mode)
 *
 * \param tc Pointer to TC module.
 * \note  enables Delay mode
 */
static inline void tc45_enable_delay(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLD = (((TC4_t *)tc)->CTRLD &
				~TC4_EVDLY_bm) | (1 << TC4_EVDLY_bp);
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLD = (((TC5_t *)tc)->CTRLD &
				~TC5_EVDLY_bm) | (1 << TC5_EVDLY_bp);
	}
}

/**
 * \brief Disables delay
 *
 * \param tc Pointer to TC module.
 * \note  disables Delay mode
 */
static inline void tc45_disable_delay(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLD = ((TC4_t *)tc)->CTRLD & ~TC4_EVDLY_bm;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLD = ((TC5_t *)tc)->CTRLD & ~TC5_EVDLY_bm;
	}
}

/**
 * \brief Tests if the Overflow flag is set
 *
 * \param tc Pointer to TC module.
 * \return  overflow has occured or not : OVFIF
 */
static inline bool tc45_is_overflow(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		return (((TC4_t *)tc)->INTFLAGS & TC4_OVFIF_bm);
	} else if (tc45_is_TC5(void *tc)) {
		return (((TC5_t *)tc)->INTFLAGS & TC5_OVFIF_bm);
	}
	Assert(false);
	return false;
}

/**
 * \brief Clears the Overflow flag
 *
 * \param tc Pointer to TC module.
 * \note  OVFIF is cleared
 */
static inline void tc45_clear_overflow(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->INTFLAGS |= TC4_OVFIF_bm;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->INTFLAGS |= TC5_OVFIF_bm;
	}
}

/**
 * \brief Tests if the Error flag is set
 *
 * \param tc Pointer to TC module.
 * \return  Error has occured or not : ERRIF
 */
static inline bool tc45_read_error(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		return (((TC4_t *)tc)->INTFLAGS & TC4_ERRIF_bm);
	} else if (tc45_is_TC5(void *tc)) {
		return (((TC5_t *)tc)->INTFLAGS & TC5_ERRIF_bm);
	}
	Assert(false);
	return false;
}

/**
 * \brief Clears the Error flag
 *
 * \param tc Pointer to TC module.
 * \note  ERRIF is cleared
 */
static inline void tc45_clear_error(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->INTFLAGS |= TC4_ERRIF_bm;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->INTFLAGS |= TC5_ERRIF_bm;
	}
}

/**
 * \brief Restart the Timer
 *
 * \param tc Pointer to TC module.
 * \note  CMD[3] in CTRLFSET is set to 1 and CMD[2] in CTRLFCLR is set
 */
static inline void tc45_restart(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLGSET = TC45_CMD_RESTART_gc;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLGSET = TC45_CMD_RESTART_gc;
	}
}

/**
 * \brief Reset the Timer
 *
 * \param tc Pointer to TC module.
 * \note  CMD[3:2] in CTRLFSET are set to 1
 */
static inline void tc45_reset(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLGSET = TC45_CMD_RESET_gc;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLGSET = TC45_CMD_RESET_gc;
	}
}

/**
 * \brief Update the Timer
 *
 * \param tc Pointer to TC module.
 * \note  CMD[2] in CTRLFSET is set to 1 and CMD[3] in CTRLFCLR is set
 */
static inline void tc45_update(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLGSET = TC45_CMD_UPDATE_gc;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLGSET = TC45_CMD_UPDATE_gc;
	}
}

/**
 * \brief Configures the Timer in Byte mode
 *
 * \param tc Pointer to TC module.
 * \note Configures  BYTEM in CTRLE
 */
static inline void tc45_set_8bits_mode(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
#ifdef TC4_BYTEM0_bm
		((TC4_t *)tc)->CTRLB |= TC4_BYTEM0_bm;
#else
		((TC4_t *)tc)->CTRLB |= TC4_BYTEM_bm;
#endif
	} else if (tc45_is_TC5(void *tc)) {
#ifdef TC5_BYTEM0_bm
		((TC5_t *)tc)->CTRLB |= TC5_BYTEM0_bm;
#else
		((TC5_t *)tc)->CTRLB |= TC5_BYTEM_bm;
#endif
	}
}

/**
 * \brief Locks the Update of the Buffered registers
 *
 * \param tc Pointer to TC module.
 *
 *  */
static inline void tc45_lock_update_buffers(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLGSET = TC4_LUPD_bm;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLGSET = TC5_LUPD_bm;
	}
}

/**
 * \brief Unlocks the Update of the Buffered registers
 *
 * \param tc Pointer to TC module.
 * \note  Configures LUPD in CTRLFCLR
 */
static inline void tc45_unlock_update_buffers(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLGCLR = TC4_LUPD_bm;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLGCLR = TC5_LUPD_bm;
	}
}

/**
 * \brief Change the polarity of outputs
 *
 * \param tc Pointer to TC module.
 * \note  Configures LUPD in CTRLFCLR
 */
static inline void tc45_pol(volatile void *tc)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLC = ((TC4_t *)tc)->CTRLC | TC4_POLD_bm |
				TC4_POLC_bm | TC4_POLB_bm | TC4_POLA_bm;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLC |= ((TC5_t *)tc)->CTRLC | TC5_POLB_bm |
				TC5_POLA_bm;
	}
}

/**
 * \brief Enables Compare channel
 *
 * \param tc Pointer to TC module.
 * \param enablemask CC channel
 */
static inline void tc45_enable_cc_channels(volatile void *tc,
		enum tc45_cc_channel_mask_enable_t enablemask)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLE |= enablemask;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLE
			|= enablemask &
				((uint8_t)TC45_CCAMODE_COMP_gc |
				(uint8_t)TC45_CCBMODE_COMP_gc |
				(uint8_t)TC45_CCAMODE_CAPT_gc |
				(uint8_t)TC45_CCBMODE_CAPT_gc);
	}
}

/**
 * \brief Disables Compare/Capture channel
 *
 * \param tc Pointer to TC module.
 * \param disablemask CC channel
 */
static inline void tc45_disable_cc_channels(volatile void *tc,
		enum tc45_cc_channel_mask_enable_t disablemask)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLE &= ~disablemask;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLE
			&= ~(disablemask &
				((uint8_t)TC45_CCAMODE_COMP_gc |
				(uint8_t)TC45_CCBMODE_COMP_gc));
	}
}

/**
 * \brief Enables Input capture mode
 *
 * \param tc Pointer to TC module.
 * \param eventsource Source for the capture
 * \param eventaction Event action capture type
 */
static inline void tc45_set_input_capture(volatile void *tc,
		TC45_EVSEL_t eventsource, TC45_EVACT_t eventaction)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLD &= ~(TC4_EVSEL_gm | TC4_EVACT_gm);
		((TC4_t *)tc)->CTRLD |= ((uint8_t)eventsource | (uint8_t)eventaction);
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLD &= ~(TC5_EVSEL_gm | TC5_EVACT_gm);
		((TC5_t *)tc)->CTRLD |= ((uint8_t)eventsource | (uint8_t)eventaction);
	}
}

/**
 * \brief Reads the Capture value
 *
 * \param tc Pointer to TC module.
 * \param channel_index Channel x
 * \return  Read value of CCx
 */
static inline uint16_t tc45_read_cc(volatile void *tc,
		enum tc45_cc_channel_t channel_index)
{
	if (tc45_is_TC4(void *tc)) {
		switch (channel_index) {
		case TC45_CCA:
			return (((TC4_t *)tc)->CCA);

		case TC45_CCB:
			return (((TC4_t *)tc)->CCB);

		case TC45_CCC:
			return (((TC4_t *)tc)->CCC);

		case TC45_CCD:
			return (((TC4_t *)tc)->CCD);
		}
	} else if (tc45_is_TC5(void *tc)) {
		switch (channel_index) {
		case TC45_CCA:
			return (((TC5_t *)tc)->CCA);

		case TC45_CCB:
			return (((TC5_t *)tc)->CCB);

		default:
			return (0);
		}
	}

	return (0);
}

/**
 * \brief Writes the CC value
 *
 * \param tc Pointer to TC module.
 * \param channel_index CC Channel
 * \param value Counter value
 */
static inline void tc45_write_cc(volatile void *tc,
		enum tc45_cc_channel_t channel_index, uint16_t value)
{
	if (tc45_is_TC4(void *tc)) {
		switch (channel_index) {
		case TC45_CCA:
			((TC4_t *)tc)->CCA = value;
			break;

		case TC45_CCB:
			((TC4_t *)tc)->CCB = value;
			break;

		case TC45_CCC:
			((TC4_t *)tc)->CCC = value;
			break;

		case TC45_CCD:
			((TC4_t *)tc)->CCD = value;
			break;
		}
	} else if (tc45_is_TC5(void *tc)) {
		switch (channel_index) {
		case TC45_CCA:
			((TC5_t *)tc)->CCA = value;
			break;

		case TC45_CCB:
			((TC5_t *)tc)->CCB = value;
			break;

		default:
			return;
		}
	}
}

/**
 * \brief Writes the Capture/Compare Buffer value
 *
 * \param tc Pointer to TC module.
 * \param channel_index CC Channel
 * \param buffer_value Counter Buffer value
 */
static inline void tc45_write_cc_buffer(volatile void *tc,
		enum tc45_cc_channel_t channel_index, uint16_t buffer_value)
{
	if (tc45_is_TC4(void *tc)) {
		switch (channel_index) {
		case TC45_CCA:
			((TC4_t *)tc)->CCABUF = buffer_value;
			break;

		case TC45_CCB:
			((TC4_t *)tc)->CCBBUF = buffer_value;
			break;

		case TC45_CCC:
			((TC4_t *)tc)->CCCBUF = buffer_value;
			break;

		case TC45_CCD:
			((TC4_t *)tc)->CCDBUF = buffer_value;
			break;
		}
	} else if (tc45_is_TC5(void *tc)) {
		switch (channel_index) {
		case TC45_CCA:
			((TC5_t *)tc)->CCABUF = buffer_value;
			break;

		case TC45_CCB:
			((TC5_t *)tc)->CCBBUF = buffer_value;
			break;

		default:
			return;
		}
	}
}

/**
 * \brief Reads the Capture/Compare Buffer value
 *
 * \param tc Pointer to TC module.
 * \param channel_index CC Channel
 * \return  CCx Buffer value
 */
static inline uint16_t tc45_read_cc_buffer(volatile void *tc,
		enum tc45_cc_channel_t channel_index)
{
	if (tc45_is_TC4(void *tc)) {
		switch (channel_index) {
		case TC45_CCA:
			return (((TC4_t *)tc)->CCABUF);

		case TC45_CCB:
			return (((TC4_t *)tc)->CCBBUF);

		case TC45_CCC:
			return (((TC4_t *)tc)->CCCBUF);

		case TC45_CCD:
			return (((TC4_t *)tc)->CCDBUF);
		}
	} else if (tc45_is_TC5(void *tc)) {
		switch (channel_index) {
		case TC45_CCA:
			return (((TC5_t *)tc)->CCABUF);

		case TC45_CCB:
			return (((TC5_t *)tc)->CCBBUF);

		default:
			return (0);
		}
	}

	return (0);
}

/**
 * \brief Reports is Capture/Compare Buffer is valid
 *
 * \param tc Pointer to TC module.
 * \param channel_index CC Channel
 * \return  CCx Buffer is valid or not
 */
static inline bool tc45_cc_buffer_is_valid(volatile void *tc,
		enum tc45_cc_channel_t channel_index)
{
	if (tc45_is_TC4(void *tc)) {
		switch (channel_index) {
		case TC45_CCA:
			return ((TC4_t *)tc)->CTRLGCLR & TC4_CCABV_bm;

		case TC45_CCB:
			return ((TC4_t *)tc)->CTRLGCLR & TC4_CCBBV_bm;

		case TC45_CCC:
			return ((TC4_t *)tc)->CTRLGCLR & TC4_CCCBV_bm;

		case TC45_CCD:
			return ((TC4_t *)tc)->CTRLGCLR & TC4_CCDBV_bm;
		}
	} else if (tc45_is_TC5(void *tc)) {
		switch (channel_index) {
		case TC45_CCA:
			return (((TC5_t *)tc)->CTRLGCLR &
			       TC5_CCABV_bm);

		case TC45_CCB:
			return (((TC5_t *)tc)->CTRLGCLR &
			       TC5_CCBBV_bm);

		default:
			return (0);
		}
	}

	return (0);
}

/**
 * \brief Reports if Capture/Compare interrupt has occured
 *
 * \param tc Pointer to TC module.
 * \param channel_index CC Channel
 * \return  CCx Interrupt or not
 */
static inline bool tc45_is_cc_interrupt(volatile void *tc,
		enum tc45_cc_channel_t channel_index)
{
	if (tc45_is_TC4(void *tc)) {
		switch (channel_index) {
		case TC45_CCA:
			return (((TC4_t *)tc)->INTFLAGS & TC4_CCAIF_bm);

		case TC45_CCB:
			return (((TC4_t *)tc)->INTFLAGS & TC4_CCBIF_bm);

		case TC45_CCC:
			return (((TC4_t *)tc)->INTFLAGS & TC4_CCCIF_bm);

		case TC45_CCD:
			return (((TC4_t *)tc)->INTFLAGS & TC4_CCDIF_bm);
		}
	} else if (tc45_is_TC5(void *tc)) {
		switch (channel_index) {
		case TC45_CCA:
			return (((TC5_t *)tc)->INTFLAGS &
			       TC5_CCAIF_bm);

		case TC45_CCB:
			return (((TC5_t *)tc)->INTFLAGS &
			       TC5_CCBIF_bm);

		default:
			return (0);
		}
	}

	return (0);
}

/**
 * \brief Clears Capture/Compare interrupt
 *
 * \param tc Pointer to TC module.
 * \param channel_index CC Channel
 */
static inline void tc45_clear_cc_interrupt(volatile void *tc,
		enum tc45_cc_channel_t channel_index)
{
	if (tc45_is_TC4(void *tc)) {
		switch (channel_index) {
		case TC45_CCA:
			((TC4_t *)tc)->INTFLAGS = TC4_CCAIF_bm;
			break;

		case TC45_CCB:
			((TC4_t *)tc)->INTFLAGS = TC4_CCBIF_bm;
			break;

		case TC45_CCC:
			((TC4_t *)tc)->INTFLAGS = TC4_CCCIF_bm;
			break;

		case TC45_CCD:
			((TC4_t *)tc)->INTFLAGS = TC4_CCDIF_bm;
			break;
		}
	} else if (tc45_is_TC5(void *tc)) {
		switch (channel_index) {
		case TC45_CCA:
			((TC5_t *)tc)->INTFLAGS = TC5_CCAIF_bm;
			break;

		case TC45_CCB:
			((TC5_t *)tc)->INTFLAGS = TC5_CCBIF_bm;
			break;

		default:
			return;
		}
	}
}

/**
 * \brief Configures TC in the specified Waveform generator mode
 *
 * \param tc Pointer to TC module.
 * \param wgm : waveform generator
 */
static inline void tc45_set_wgm(volatile void *tc, enum tc45_wg_mode_t wgm)
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLB
			= (((TC4_t *)tc)->CTRLB & ~TC4_WGMODE_gm) | wgm;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLB
			= (((TC5_t *)tc)->CTRLB & ~TC5_WGMODE_gm) | wgm;
	}
}

/*******************************************************
*      FAULT module                                   *
*******************************************************/

/**
 * \brief FAULT module : configures the RAMP2 mode of Fault *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_ramp2_mode(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLA |= FAULT_RAMP_RAMP2_gc;
}

/**
 * \brief FAULT module : configures the QUAL mode of Fault A
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_qualA_mode(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLC |= FAULT_QUALA_bm;
}

/**
 * \brief FAULT module : configures the QUAL mode of fault B
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_qualB_mode(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLE |= FAULT_QUALB_bm;
}

/**
 * \brief FAULT module : configures the BLANK mode of fault A
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_blankA_mode(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLC |= FAULT_BLANKA_bm;
}

/**
 * \brief FAULT module: configures the BLANK mode of fault B
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_blankB_mode(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLC |= FAULT_BLANKB_bm;
}

/**
 * \brief WEX extension : configures the blanking time
 *
 * \param WEX Pointer to WEX module (WEXC )
 * \param value :  blanking value
 */
static inline void tc45_fault_set_blank_time(WEX_t *WEX, int16_t value)
{
	((WEX_t *)WEX)->DTLS = value; /* blanking time*/
}

/* \brief FAULT module : configures the Fault A input
 *
 * \param FAULT Pointer to FAULT module
 * \ param Fault source mode : Event channel N,N+1...
 */

static inline void tc45_fault_configure_sourceA_input(FAULT_t *FAULT,
		enum fault_sourceA_mode_t source)
{
	((FAULT_t *)FAULT)->CTRLB
		= (((FAULT_t *)FAULT)->CTRLB & ~FAULT_SRCA_gm) | source;
}

/**
 * \brief FAULT module : configures the Fault B input
 *
 * \param FAULT Pointer to FAULT module
 * \ param Fault source mode : Event channel N,N+1...
 */

static inline void tc45_fault_configure_sourceB_input(FAULT_t *FAULT,
		enum fault_sourceB_mode_t source)
{
	((FAULT_t *)FAULT)->CTRLD
		= (((FAULT_t *)FAULT)->CTRLD & ~FAULT_SRCB_gm) | source;
}

/**
 * \brief FAULT module : configures the Fault E input
 *
 * \param FAULT Pointer to FAULT module
 * \ param Fault source mode : Event channel N,N+1...
 */

static inline void tc45_fault_configure_sourceE_input(FAULT_t *FAULT,
		enum fault_sourceE_mode_t source)
{
	((FAULT_t *)FAULT)->CTRLA
		= (((FAULT_t *)FAULT)->CTRLA & ~FAULT_SRCE_gm) | source;
}

/**
 * \brief FAULT module : enables the Filter on Fault E
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_faultE_filter(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLA |= FAULT_FILTERE_bm;
}

/**
 * \brief FAULT module : enables the Filter on Fault A
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_faultA_filter(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLC |= FAULT_FILTERA_bm;
}

/**
 * \brief FAULT module : enables the Filter on Fault B
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_faultB_filter(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLE |= FAULT_FILTERB_bm;
}

/**
 * \brief FAULT module : Set the Restart mode of Fault A
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_restartA_mode(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLB |= FAULT_RESTARTA_bm;
}

/**
 * \brief FAULT module : Set the Restart mode of Fault B
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_restartB_mode(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLD |= FAULT_RESTARTB_bm;
}

/**
 * \brief FAULT module : Set the Keep mode of Fault A
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_keepA_mode(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLB |= FAULT_KEEPA_bm;
}

/**
 * \brief FAULT module : Set the Keep mode of Fault B
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_keepB_mode(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLD |= FAULT_KEEPB_bm;
}

/**
 * \brief FAULT module : cSet the Soft mode of Fault A
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_softA_mode(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLB |= FAULT_SOFTA_bm;
}

/**
 * \brief FAULT module : Set the Soft mode of Fault B
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_softB_mode(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLD |= FAULT_SOFTB_bm;
}

/**
 * \brief FAULT module : Set the Capt mode of Fault A
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_captA_mode(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLC |= FAULT_CAPTA_bm;
}

/**
 * \brief FAULT module : Set the Capt mode of Fault B
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_captB_mode(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLE |= FAULT_CAPTB_bm;
}

/**
 * \brief FAULT module : configures the Halt mode of Fault A
 *
 * \param FAULT Pointer to FAULT module
 * \param Halt mode
 */

static inline void tc45_fault_set_haltA_mode(FAULT_t *FAULT,
		enum fault_haltA_mode_t fault)
{
	((FAULT_t *)FAULT)->CTRLB
		= (((FAULT_t *)FAULT)->CTRLB & ~FAULT_HALTA_gm) | fault;
}

/**
 * \brief FAULT module : configures the Halt mode of Fault B
 *
 * \param FAULT Pointer to FAULT module
 * \param Halt mode
 */

static inline void tc45_fault_set_haltB_mode(FAULT_t *FAULT,
		enum fault_haltB_mode_t fault)
{
	((FAULT_t *)FAULT)->CTRLD
		= (((FAULT_t *)FAULT)->CTRLD & ~FAULT_HALTB_gm) | fault;
}

/*! \brief Configures the Timer/Counter event channel source
 *  \param tc               Timer/Counter module instance.
 *  \param  eventSource   Event channel base
 */
static inline void tc45_config_fault_input_evsel(volatile void *tc,
		TC45_EVSEL_t eventSource )
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLD
			= (((TC4_t *)tc)->CTRLD & ~(TC4_EVSEL_gm)) |
				eventSource;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLD
			= (((TC5_t *)tc)->CTRLD & ~(TC5_EVSEL_gm)) |
				eventSource;
	}
}

/*! \brief Configures the Timer/Counter Event operation.
 *  \param tc               Timer/Counter module instance.
 *  \param    eventAction : capture, qdec....
 */
static inline void tc45_config_fault_input_evact(volatile void *tc,
		TC45_EVSEL_t eventAction )
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLD
			= (((TC4_t *)tc)->CTRLD & ~(TC4_EVACT_gm)) |
				eventAction;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLD
			= (((TC5_t *)tc)->CTRLD & ~(TC5_EVACT_gm)) |
				eventAction;
	}
}

/*! \brief Configures the Timer/Counter FMODE1.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param
 */
static inline void tc45_config_fault_input_fmode1(volatile void *tc,
		TC45_EVSEL_t eventSource )
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLD
			= (((TC4_t *)tc)->CTRLD &
				~(TC4_EVSEL_gm | TC4_EVACT_gm)) |
				eventSource |
				TC45_EVACT_FMODE1_gc;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLD
			= (((TC5_t *)tc)->CTRLD &
				~(TC5_EVSEL_gm | TC5_EVACT_gm)) |
				eventSource |
				TC45_EVACT_FMODE1_gc;
	}
}

/*! \brief Configures the Timer/Counter FMODE2.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param
 */
static inline void tc45_config_fault_input_fmode2(volatile void *tc,
		TC45_EVSEL_t eventSource )
{
	if (tc45_is_TC4(void *tc)) {
		((TC4_t *)tc)->CTRLD
			= (((TC4_t *)tc)->CTRLD &
				~(TC4_EVSEL_gm | TC4_EVACT_gm)) |
				eventSource | TC45_EVACT_FMODE2_gc;
	} else if (tc45_is_TC5(void *tc)) {
		((TC5_t *)tc)->CTRLD
			= (((TC5_t *)tc)->CTRLD &
				~(TC5_EVSEL_gm | TC5_EVACT_gm)) |
				eventSource | TC45_EVACT_FMODE2_gc;
	}
}

/**
 * \brief FAULT module : returns if fault is present
 *
 * \param FAULT Pointer to FAULT module
 */

static inline bool tc45_fault_faultAin_is_detected(FAULT_t *FAULT)
{
	return ((FAULT_t *)FAULT)->STATUS & FAULT_FAULTAIN_bm;
}

/**
 * \brief FAULT module : returns if fault is present
 *
 * \param FAULT Pointer to FAULT module
 */

static inline bool tc45_fault_faultBin_is_detected(FAULT_t *FAULT)
{
	return ((FAULT_t *)FAULT)->STATUS & FAULT_FAULTBIN_bm;
}

/**
 * \brief FAULT module : returns if fault is present
 *
 * \param FAULT Pointer to FAULT module
 */

static inline bool tc45_fault_faultEin_is_detected(FAULT_t *FAULT)
{
	return ((FAULT_t *)FAULT)->STATUS & FAULT_FAULTEIN_bm;
}

/**
 * \brief FAULT module : returns if fault is detected in STATEA
 *
 * \param FAULT Pointer to FAULT module
 */

static inline bool tc45_fault_stateA_is_detected(FAULT_t *FAULT)
{
	return ((FAULT_t *)FAULT)->STATUS & FAULT_STATEA_bm;
}

/**
 * \brief FAULT module : clears the STATEA Fault status
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_clear_haltA(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLGCLR = FAULT_HALTACLR_bm;
}

/**
 * \brief FAULT module : returns if fault is detected in STATEA
 *
 * \param FAULT Pointer to FAULT module
 */

static inline bool tc45_fault_stateB_is_detected(FAULT_t *FAULT)
{
	return ((FAULT_t *)FAULT)->STATUS & FAULT_STATEB_bm;
}

/**
 * \brief FAULT module : clears the STATEB Fault status
 *
 * \param FAULT Pointer to FAULT module
 */
static inline void tc45_fault_clear_haltB(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLGCLR = FAULT_HALTBCLR_bm;
}

/**
 * \brief FAULT module : returns if fault is detected in STATEE
 *
 * \param FAULT Pointer to FAULT module
 */

static inline bool tc45_fault_stateE_is_detected(FAULT_t *FAULT)
{
	return ((FAULT_t *)FAULT)->STATUS & FAULT_STATEB_bm;
}

/**
 * \brief FAULT module : clears the STATEE Fault status
 *
 * \param FAULT Pointer to FAULT module
 */
static inline void tc45_fault_clear_stateE(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLGCLR = FAULT_STATEECLR_bm;
}

/**
 * \brief FAULT module : clears the FAULTA Fault detection
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_clear_faultA(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLGCLR |= FAULT_FAULTA_bm;
}

/**
 * \brief FAULT module : clears the FAULTB Fault detection
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_clear_faultB(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLGCLR |= FAULT_FAULTB_bm;
}

/**
 * \brief FAULT module : clears the FAULTE Fault detection
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_clear_faultE(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLGCLR |= FAULT_FAULTE_bm;
}

/**
 * \brief FAULT module : set the Software Fault input A
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_softwareA(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLGSET |= FAULT_FAULTASW_bm;
}

/**
 * \brief FAULT module : set the Software Fault input B
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_softwareB(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLGSET |= FAULT_FAULTBSW_bm;
}

/**
 * \brief FAULT module : set the Software Fault input E
 *
 * \param FAULT Pointer to FAULT module
 */

static inline void tc45_fault_set_softwareE(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLGSET |= FAULT_FAULTESW_bm;
}

/**
 * \brief FAULT module : set the Portcontrol from Fault module
 *
 * \param FAULT Pointer to FAULT module
 */
static inline void tc45_fault_set_portcontrol(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLA |= FAULT_PORTCTRL_bm;
}

/**
 * \brief WEX extension : reset the Fault Port control
 *
 * \param WEX Pointer to WEX module (WEXC )
 */

static inline void tc45_fault_reset_portcontrol(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLA &= ~FAULT_PORTCTRL_bm;
}

/**
 * \brief WEX extension : force the Cycle B (IDX set) in RAMP2 mode
 *
 * \param WEX Pointer to WEX module (WEXC )
 */

static inline void tc45_fault_set_idx(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLGSET
		= (((FAULT_t *)FAULT)->CTRLGSET &
			~FAULT_IDXCMD_gm) | FAULT_IDXCMD_SET_gc;
}

/**
 * \brief WEX extension : force the Cycle A (IDX clear) in RAMP2 mode
 *
 * \param WEX Pointer to WEX module (WEXC )
 */

static inline void tc45_fault_clear_idx(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLGSET
		= (((FAULT_t *)FAULT)->CTRLGSET &
			~FAULT_IDXCMD_gm) | FAULT_IDXCMD_CLEAR_gc;
}

/**
 * \brief WEX extension : Hold the Cycle N (IDX Hold) in RAMP2 mode
 *
 * \param WEX Pointer to WEX module (WEXC )
 */

static inline void tc45_fault_hold_idx(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLGSET
		= (((FAULT_t *)FAULT)->CTRLGSET &
			~FAULT_IDXCMD_gm) | FAULT_IDXCMD_HOLD_gc;
}

/**
 * \brief WEX extension : Default IDX in RAMP2 mode
 *
 * \param WEX Pointer to WEX module (WEXC )
 */

static inline void tc45_fault_default_idx(FAULT_t *FAULT)
{
	((FAULT_t *)FAULT)->CTRLGSET
		= (((FAULT_t *)FAULT)->CTRLGSET & ~FAULT_IDXCMD_gm);
}

/**
 * \brief WEX extension : reads IDX in RAMP2 mode
 *
 * \param WEX Pointer to WEX module (WEXC )
 */

static inline bool tc45_fault_read_idx(FAULT_t *FAULT)
{
	return ((FAULT_t *)FAULT)->STATUS & FAULT_IDX_bm;
}

/*******************************************************
*      AWEX extension                                 *
*******************************************************/

/**
 * \brief WEX extension : Configures WEX in the specified output matrix mode
 *
 * \param WEX Pointer to WEX module (WEXC )tmx
 *
 */
static inline void tc45_WEX_set_otmx(WEX_t *WEX, enum wex_otmx_mode_t otmx)
{
	((WEX_t *)WEX)->CTRL = (((WEX_t *)WEX)->CTRL & ~WEX_OTMX_gm) | otmx;
}

/**
 * \brief WEX extension : Read the output matrix mode
 *
 * \param WEX Pointer to WEX module (WEXC )tmx
 *
 */
static inline uint16_t tc45_WEX_read_otmx(WEX_t *WEX)
{
	return (((WEX_t *)WEX)->CTRL & WEX_OTMX_gm);
}

/**
 * \brief WEX extension : enable Deadtime insertion on ccA
 *
 * \param WEX Pointer to WEX module (WEXC )
 */
static inline void tc45_WEX_enable_cca_deadtime(WEX_t *WEX)
{
	((WEX_t *)WEX)->CTRL |= WEX_DTI0EN_bm;
}

/**
 * \brief WEX extension : disable Deadtime insertion on ccA
 *
 * \param WEX Pointer to WEX module (WEXC )
 */
static inline void tc45_WEX_disable_cca_deadtime(WEX_t *WEX)
{
	((WEX_t *)WEX)->CTRL &= ~WEX_DTI0EN_bm;
}

/**
 * \brief WEX extension : enable Deadtime insertion on ccB
 *
 * \param WEX Pointer to WEX module (WEXC )
 */
static inline void tc45_WEX_enable_ccb_deadtime(WEX_t *WEX)
{
	((WEX_t *)WEX)->CTRL |= WEX_DTI1EN_bm;
}

/**
 * \brief WEX extension : disable Deadtime insertion on ccB
 *
 * \param WEX Pointer to WEX module (WEXC )
 */
static inline void tc45_WEX_disable_ccb_deadtime(WEX_t *WEX)
{
	((WEX_t *)WEX)->CTRL &= ~WEX_DTI1EN_bm;
}

/**
 * \brief WEX extension : enable Deadtime insertion on ccC
 *
 * \param WEX Pointer to WEX module (WEXC )
 */
static inline void tc45_WEX_enable_ccc_deadtime(WEX_t *WEX)
{
	((WEX_t *)WEX)->CTRL |= WEX_DTI2EN_bm;
}

/**
 * \brief WEX extension : disable Deadtime insertion on ccD
 *
 * \param WEX Pointer to WEX module (WEXC )
 */
static inline void tc45_WEX_disable_ccc_deadtime(WEX_t *WEX)
{
	((WEX_t *)WEX)->CTRL &= ~WEX_DTI2EN_bm;
}

/**
 * \brief WEX extension : enable Deadtime insertion on ccD
 *
 * \param WEX Pointer to WEX module (WEXC )
 */
static inline void tc45_WEX_enable_ccd_deadtime(WEX_t *WEX)
{
	((WEX_t *)WEX)->CTRL |= WEX_DTI3EN_bm;
}

/**
 * \brief WEX extension : disable Deadtime insertion on ccD
 *
 * \param WEX Pointer to WEX module (WEXC )
 */
static inline void tc45_WEX_disable_ccd_deadtime(WEX_t *WEX)
{
	((WEX_t *)WEX)->CTRL &= ~WEX_DTI3EN_bm;
}

/**
 * \brief WEX extension : configures high side deadtime
 *
 * \param WEX Pointer to WEX module (WEXC )
 * \param value : deadtime value
 */
static inline void tc45_WEX_set_dti_high(WEX_t *WEX, int16_t value)
{
	((WEX_t *)WEX)->DTHS = value;
}

/**
 * \brief WEX extension : configures low side deadtime
 *
 * \param WEX Pointer to WEX module (WEXC )
 * \param value : deadtime value
 */
static inline void tc45_WEX_set_dti_low(WEX_t *WEX, int16_t value)
{
	((WEX_t *)WEX)->DTLS = value;
}

/**
 * \brief WEX extension : configures symetrical deadtime
 *
 * \param WEX Pointer to WEX module (WEXC )
 * \param value : deadtime value
 */
static inline void tc45_WEX_set_dti_both(WEX_t *WEX, int16_t value)
{
	((WEX_t *)WEX)->DTBOTH = value;
}

/**
 * \brief WEX extension : Enable Swap on OTMX 0  and 1
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_enable_swap0(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAP |= WEX_SWAP0_bm;
}

/**
 * \brief WEX extension : Disable Swap on OTMX 0  and 1
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_disable_swap0(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAP &= ~WEX_SWAP0_bm;
}

/**
 * \brief WEX extension : Enable Swap Buffer on OTMX 0  and 1
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_enable_swap0_buffer(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAPBUF |= WEX_SWAP0BUF_bm;
}

/**
 * \brief WEX extension : Disable Swap on OTMX 0  and 1
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_disable_swap0_buffer(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAPBUF &= ~WEX_SWAP0BUF_bm;
}

/**
 * \brief WEX extension : Enable Swap on OTMX 2  and 3
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_enable_swap1(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAP |= WEX_SWAP1_bm;
}

/**
 * \brief WEX extension : Disable Swap on OTMX 2  and 3
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_disable_swap1(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAP &= ~WEX_SWAP1_bm;
}

/**
 * \brief WEX extension : Enable Swap buffer on OTMX 2  and 3
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_enable_swap1_buffer(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAPBUF |= WEX_SWAP1BUF_bm;
}

/**
 * \brief WEX extension : Disable Swap on OTMX 2  and 3
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_disable_swap1_buffer(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAPBUF &= ~WEX_SWAP1BUF_bm;
}

/**
 * \brief WEX extension : Enable Swap on OTMX 4  and 5
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_enable_swap2(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAP |= WEX_SWAP2_bm;
}

/**
 * \brief WEX extension : Disable Swap on OTMX 4  and 5
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_disable_swap2(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAP &= ~WEX_SWAP2_bm;
}

/**
 * \brief WEX extension : Ennable Swap buffer on OTMX 4  and 5
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_enable_swap2_buffer(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAPBUF |= WEX_SWAP2BUF_bm;
}

/**
 * \brief WEX extension : Disable Swap on OTMX 4  and 5
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_disable_swap2_buffer(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAPBUF &= ~WEX_SWAP2BUF_bm;
}

/**
 * \brief WEX extension : Enable Swap on OTMX 6  and 7
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_enable_swap3(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAP |= WEX_SWAP3_bm;
}

/**
 * \brief WEX extension : Disable Swap on OTMX 6  and 7
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_disable_swap3(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAP &= ~WEX_SWAP3_bm;
}

/**
 * \brief WEX extension : Enable Swap buffer on OTMX 6  and 7
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_enable_swap3_buffer(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAPBUF |= WEX_SWAP3BUF_bm;
}

/**
 * \brief WEX extension : Disable Swap on OTMX 6  and 7
 *
 * \param WEX Pointer to WEX module (WEXC )
 *
 */
static inline void tc45_WEX_disable_swap3_buffer(WEX_t *WEX)
{
	((WEX_t *)WEX)->SWAPBUF &= ~WEX_SWAP3BUF_bm;
}

/**
 * \brief WEX extension : configures Pattern Generator output enable
 *
 * \param WEX Pointer to WEX module (WEXC )
 * \param PGO enable
 */

static inline void tc45_WEX_write_pgo(WEX_t *WEX, int16_t value)
{
	((WEX_t *)WEX)->PGO = (((WEX_t *)WEX)->PGO & 0x00) |
			value;
}

/**
 * \brief WEX extension : configures Pattern Generator Buffer output enable
 *
 * \param WEX Pointer to WEX module (WEXC )
 * \param PGO enable
 */

static inline void tc45_WEX_write_pgo_buffer(WEX_t *WEX, int16_t value)
{
	/*	((WEX_t *)WEX)->PGOBUF = (((WEX_t *)WEX)->PGOBUF & 0x00) | */
	/*		value ; */
	((WEX_t *)WEX)->PGOBUF = value;
}

/**
 * \brief Reads the Pattern generator Output buffer
 *
 * \param tc Pointer to TC module.
 * \return Patter Generator Buffer PGOBUF
 */
static inline uint16_t tc45_WEX_read_pgo_buffer(volatile void *WEX)
{
	return (((WEX_t *)WEX)->PGOBUF);
}

/**
 * \brief WEX extension : configures Pattern Generator Value
 *
 * \param WEX Pointer to WEX module (WEXC )
 * \param PGO value
 */

static inline void tc45_WEX_write_pgv(WEX_t *WEX, int16_t value)
{
	((WEX_t *)WEX)->PGV = (((WEX_t *)WEX)->PGV & 0x00) |
			value;
}

/**
 * \brief WEX extension : configures Pattern Generator Value Buffer
 *
 * \param WEX Pointer to WEX module (WEXC )
 * \param PGO value
 */

static inline void tc45_WEX_write_pgv_buffer(WEX_t *WEX, int16_t value)
{
	((WEX_t *)WEX)->PGVBUF = (((WEX_t *)WEX)->PGVBUF & 0x00) |
			value;
}

/**
 * \brief WEX extension : configures Output override
 *
 * \param WEX Pointer to WEX module (WEXC )
 * \param PGO value
 */

static inline void tc45_WEX_set_output_override(WEX_t *WEX, int16_t value)
{
	((WEX_t *)WEX)->OUTOVDIS = (((WEX_t *)WEX)->OUTOVDIS & 0x00) |
			value;
}

/*******************************************************
*      HIRES extension                                *
*******************************************************/

/* @} */

/**
 * \ingroup tc45_group
 * \defgroup tc45_hires_group Hi-Res extension driver
 * This group provides low level drivers to configure Hi-Res extension
 * @{
 */

/**
 * \brief Hi-Res Extension : configures the Hi-Res
 *
 * \param hires Pointer to WEX module (WEXC )
 * \param hi_res_mode HIRES configuration
 */
static inline void tc45_hires_set_mode(HIRES_t *hires, HIRES_HREN_t hi_res_mode)
{
	((HIRES_t *)hires)->CTRLA = hi_res_mode;
}

/* @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* TC45_H */
