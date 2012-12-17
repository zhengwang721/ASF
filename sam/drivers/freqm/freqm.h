/**
 * \file
 *
 * \brief Frequency Meter driver for SAM4L.
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

#ifndef FREQM_H_INCLUDED
#define FREQM_H_INCLUDED

#include "compiler.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/** Time out value (number of attempts) */
#define FREQM_NUM_OF_ATTEMPTS                  1000000

typedef void (*freqm_callback_t)(void);

void freqm_set_configure(Freqm *p_freqm, uint8_t ref_clk, uint8_t msr_clk,uint8_t duration);
uint32_t freqm_get_result(Freqm *p_freqm);
void freqm_enable(Freqm *p_freqm);
void frqem_disable(Freqm *p_freqm);
void freqm_set_callback(Freqm *p_freqm, freqm_callback_t callback,
		uint8_t irq_line, uint8_t irq_level, uint32_t ul_sources);

/**
 * \brief Start a measurement
 *
 * \param p_freqm Pointer to an FREQM instance
 */
static inline void freqm_start_measure(Freqm *p_freqm)
{
	p_freqm->FREQM_CTRL = FREQM_CTRL_START;
}

/**
 * \brief Enable refclk
 *
 * \param p_freqm Pointer to an FREQM instance
 */
static inline void freqm_enable_refclk(Freqm *p_freqm)
{
	p_freqm->FREQM_MODE |= FREQM_MODE_REFCEN;
}

/**
 * \brief Disable refclk
 *
 * \param p_freqm Pointer to an FREQM instance
 */
static inline void freqm_disable_refclk(Freqm *p_freqm)
{
	p_freqm->FREQM_MODE &= ~FREQM_MODE_REFCEN;
}

/**
 * \brief Get FREQM status
 *
 * \param p_freqm Pointer to an FREQM instance
 */
static inline uint32_t freqm_get_status(Freqm *p_freqm)
{
	return p_freqm->FREQM_STATUS;
}

/**
 * \brief Enable FREQM interrupt
 *
 * \param p_freqm Pointer to an FREQM instance
 * \param ul_source  Interrupt source
 */
static inline void freqm_enable_interrupt(Freqm *p_freqm, const uint32_t ul_source)
{
	p_freqm->FREQM_IER |= ul_source;
}

/**
 * \brief Disable FREQM interrupt
 *
 * \param p_freqm Pointer to an FREQM instance
 * \param ul_source  Interrupt source
 */
static inline void freqm_disable_interrupt(Freqm *p_freqm, const uint32_t ul_source)
{
	p_freqm->FREQM_IDR |= ul_source;
}

/**
 * \brief Get FREQM interrupt status.
 *
 * \param p_freqm Pointer to an FREQM instance.
 *
 */
static inline uint32_t freqm_get_interrupt_status(Freqm *p_freqm)
{
	return p_freqm->FREQM_ISR;
}

/**
 * \brief Clear FREQM interrupt status.
 *
 * \param p_freqm Pointer to an FREQM instance.
 * \param int_status_flags interrupt status flag.
 *
 */
static inline void freqm_clear_interrupt_status(Freqm *p_freqm,
		const uint32_t int_status_flags)
{
	p_freqm->FREQM_ICR |= int_status_flags;
}

/**
 * \brief Get FREQM interrupt mask.
 *
 * \param p_freqm Pointer to an FREQM instance.
 *
 */
static inline uint32_t freqm_get_interrupt_mask(Freqm *p_freqm)
{
	return p_freqm->FREQM_IMR;
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \page sam_freqm_quickstart Quickstart guide for SAM FREQM driver
 *
 * This is the quickstart guide for the \ref freqm_group "SAM FREQM driver",
 * with step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section freqm_basic_use_case Basic use case
 * In this basic use case, the FREQM module are configured for:
 * - Select CLK32K as refclk
 * - Select CLK_CPU as msrclk.
 * - Duration of a measurement is 128
 *
 * \subsection sam_freqm_quickstart_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (Sysclock)"
 *
 * \section freqm_basic_use_case_setup Setup steps
 * \subsection freqm_basic_use_case_setup_code Example code
 * Add to application C-file:
 * \code
 *    freqm_enable(FREQM);
 *
 *    freqm_set_configure(FREQM, 1, 0, 128);
 *
 *    freqm_start_measure(FREQM);
 *
 *    cpu_clk = (freqm_get_result(FREQM) / 128) * 32768; 
 * \endcode
 *
 * \subsection freqm_basic_use_case_setup_flow Workflow
 * -# Enable FREQM module:
 *   - \code freqm_enable(FREQM); \endcode
 *   - \note Including enable module clock and lock sleep mode.
 * -# Configure FREQM line with specified mode:
 *   - \code freqm_set_configure(FREQM, 1, 0, 128); \endcode
 * -# Start Measurement.
 *   - \code freqm_start_measure(FREQM); \endcode
 * -# Get mesurement result:
 *   - \code cpu_clk = (freqm_get_result(FREQM) / 128) * 32768; \endcode
 */
#endif /* FREQM_H_INCLUDED */
