/**
 * \file
 *
 * \brief Analog Comparator interface driver for SAM4L.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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

#ifndef ACIFC_H_INCLUDED
#define ACIFC_H_INCLUDED

#include "compiler.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/** ACIFC configuration */
typedef struct {
	/** Test mode */
	bool actest;
	/** Peripheral Event Trigger Enable */
	bool eventen;
} acifc_cfg_t;

/** AC channel configuration */
typedef struct {
	/** Hysteresis value */
	enum {
		ACIFC_HYS_0,
		ACIFC_HYS_1,
		ACIFC_HYS_2,
		ACIFC_HYS_3
	} hysteresis_value;
	/** Always on enable */
	bool alwayson;
	/** Fast mode enable */
	bool fast;
	/** Output event when ACOUT is zero? */
	bool event_negative;
	/** Output event when ACOUT is one? */
	bool event_positive;
	/** Set the negative input */
	enum {
		NI_ACN,
		RESERVED1,
		RESERVED2,
		RESERVED3,
	} negative_input;
	/** Set the comparator mode */
	enum {
		MODE_OFF,
		MODE_CONTINUOUS,
		MODE_USER_TRIGGERED,
		MODE_EVENT_TRIGGERED
	} mode;
	/** Interrupt settings */
	enum {
		IS_VINP_GT_VINN,
		IS_VINP_LT_VINN,
		IS_OUTPUT_TGL,
		IS_COMP_DONE
	} interrupt_settings;
} acifc_channel_cfg_t;

/** AC Window configuration */
typedef struct {
	/** Window Mode Enable/Disable */
	bool window_mode;
	/** Window Event from awout Enable/Disable */
	bool window_event_enable;
	/** Window Event output configuration */
	enum {
		EVENT_ON_ACWOUT_RISING_EDGE,
		EVENT_ON_ACWOUT_FALLING_EDGE,
		EVENT_ON_ACWOUT_ON_ANY_EDGE,
		EVENT_ON_INSIDE_WINDOW,
		EVENT_ON_OUTSIDE_WINDOW,
		EVENT_ON_COMPARISON_COMPLETE,
		RESERVED4,
		RESERVED5,
	} window_event;
	/** Interrupt settings */
	enum {
		IS_VINP_INSIDE_WINDOW,
		IS_VINP_OUTSIDE_WINDOW,
		IS_WINDOW_OUTPUT_TGL,
		IS_WINDOW_COMP_DONE,
		IS_VINP_ENTER_WINDOW,
		IS_VINP_LEAVE_WINDOW,
		RESERVED6,
		RESERVED7,
	} interrupt_settings;
} acifc_window_cfg_t;

typedef void (*acifc_callback_t) (void);

void acifc_channel_configure(Acifc *p_acifc, const acifc_channel_cfg_t *ac_chan_cfg,
		uint8_t nb_chan);
void acifc_configure(Acifc *p_acifc, const acifc_cfg_t *ac_cfg);
void acifc_window_configure(Acifc *p_acifc, const acifc_window_cfg_t *ac_window_cfg,
		uint8_t nb_window);
void acifc_set_callback(Acifc *p_acifc, acifc_callback_t callback,
		uint8_t irq_line, uint8_t irq_level, uint32_t interrupt_flags);
void acifc_enable(Acifc *p_acifc);
void acifc_disable(Acifc *p_acifc);

/**
 * \brief User starts a single comparison.
 *
 * \param p_acifc Pointer to an ACIFC instance.
 */
static inline void acifc_user_trigger_single_comparison(Acifc *p_acifc)
{
	p_acifc->ACIFC_CTRL |= ACIFC_CTRL_USTART;
}

/**
 * \brief Test event trigger start a single analog comparison.
 *
 * \param p_acifc Pointer to an ACIFC instance.
 */
static inline void acifc_test_event_trigger_single_comparison(Acifc *p_acifc)
{
	p_acifc->ACIFC_CTRL |= ACIFC_CTRL_ESTART;
}

/**
 * \brief Get ACIFC status.
 *
 * \param p_acifc Pointer to an ACIFC instance.
 *
 */
static inline uint32_t acifc_get_status(Acifc *p_acifc)
{
	return p_acifc->ACIFC_SR;
}

/**
 * \brief Get ACIFC interrupt status.
 *
 * \param p_acifc Pointer to an ACIFC instance.
 *
 */
static inline uint32_t acifc_get_interrupt_status(Acifc *p_acifc)
{
	return p_acifc->ACIFC_ISR;
}

/**
 * \brief Clear ACIFC interrupt status.
 *
 * \param p_acifc Pointer to an ACIFC instance.
 * \param status_flags status flags.
 *
 */
static inline void acifc_clear_interrupt_status(Acifc *p_acifc,
		const uint32_t status_flags)
{
	p_acifc->ACIFC_ICR |= status_flags;
}

/**
 * \brief Enable ACIFC interrupt.
 *
 * \param p_acifc Pointer to an ACIFC instance.
 * \param interrupt_flags interrupt source.
 *
 */
static inline void acifc_enable_interrupt(Acifc *p_acifc,
		const uint32_t interrupt_flags)
{
	p_acifc->ACIFC_IER |= interrupt_flags;
}

/**
 * \brief Disable ACIFC interrupt.
 *
 * \param p_acifc Pointer to an ACIFC instance.
 * \param interrupt_flags interrupt source.
 *
 */
static inline void acifc_disable_interrupt(Acifc *p_acifc,
		const uint32_t interrupt_flags)
{
	p_acifc->ACIFC_IDR |= interrupt_flags;
}

/**
 * \brief Get ACIFC interrupt mask.
 *
 * \param p_acifc Pointer to an ACIFC instance.
 *
 */
static inline uint32_t acifc_get_interrupt_mask(Acifc *p_acifc)
{
	return p_acifc->ACIFC_IMR;
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \page sam_acifc_quickstart Quickstart guide for SAM ACIFC driver
 *
 * This is the quickstart guide for the \ref group_sam_drivers_acifc
 * "SAM ACIFC driver", with step-by-step instructions on how to
 * configure and use the driver in a selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section acifc_basic_use_case Basic use case
 * In this basic use case, the last page page and the user page will be written
 * with a specific magic number.
 *
 * \subsection sam_acifc_quickstart_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (Sysclock)"
 *
 * \section acifc_basic_use_case_setup Setup steps
 * by default.
 * \subsection acifc_basic_use_case_setup_code Example code
 * Enable the following macro in the conf_clock.h:
 * \code
 *  #define CONFIG_SYSCLK_SOURCE       SYSCLK_SRC_DFLL
 *  #define CONFIG_DFLL0_SOURCE         GENCLK_SRC_OSC32K
 * \endcode
 *
 * Add the following code in the application C-file:
 * \code
 *  sysclk_init();
 * \endcode
 *
 * \subsection acifc_basic_use_case_setup_flow Workflow
 * -# Set system clock source as DFLL:
 *   - \code #define CONFIG_SYSCLK_SOURCE       SYSCLK_SRC_DFLL \endcode
 * -# Set DFLL source as OSC32K:
 *   - \code #define CONFIG_DFLL0_SOURCE         GENCLK_SRC_OSC32K \endcode
 * -# Initialize the system clock.
 *   - \code sysclk_init(); \endcode
 *
 * \section acifc_basic_use_case_usage Usage steps
 * \subsection acifc_basic_use_case_usage_code Example code
 * Add to, e.g., main loop in application C-file:
 * \code
 *    acifc_enable(ACIFC);
 *    acifc_configure(ACIFC, &acifc_opt);
 *    acifc_channel_configure(ACIFC, &acifc_channel_opt, EXAMPLE_ACIFC_CHANNEL);
 *    acifc_set_callback(ACIFC, compare_result_output, ACIFC_IRQn, 1, ACIFC_IER_ACINT0);
 *    acifc_user_trigger_single_comparison(ACIFC);
 * \endcode
 *
 * \subsection acifc_basic_use_case_usage_flow Workflow
 * -# Enable ACIFC Module:
 *   - \code acifc_enable(ACIFC); \endcode
 * -# Configure the ACIFC module:
 *   - \code acifc_configure(ACIFC, &acifc_opt); \endcode
 * -# Configure a set of AC channels in normal mode:
 *   - \code acifc_channel_configure(ACIFC, &acifc_channel_opt, EXAMPLE_ACIFC_CHANNEL); 
 * \endcode
 * -# Set callback for ACIFC:
 *   - \code acifc_set_callback(ACIFC, compare_result_output, ACIFC_IRQn, 1, ACIFC_IER_ACINT0);
 *  \endcode
 * -# User starts a single comparison:
 *   - \code acifc_user_trigger_single_comparison(ACIFC); \endcode
 */
#endif /* ACIFC_H_INCLUDED */
