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

#endif /* ACIFC_H_INCLUDED */
