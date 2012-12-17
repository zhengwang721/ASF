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

#include "freqm.h"
#include "sysclk.h"
#include "sleepmgr.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \defgroup sam_drivers_freqm_group Frequency Meter (FREQM)
 *
 * See \ref sam_freqm_quickstart.
 *
 * Driver for the Frequency Meter. This driver provides access to the main
 * features of the FREQM controller.
 *
 * @{
 */

/**
 * \internal
 * \brief EIC callback function pointer array
 */
freqm_callback_t freqm_callback;

/**
 * \brief Configure FREQM with specified value.
 *
 * \param p_freqm Pointer to a FREQM instance
 * \param ref_clk  Select the reference clock
 * \param msr_clk Select the clock to be measured
 * \param duration Set the duration of a measurement
 */
void freqm_set_configure(Freqm *p_freqm, uint8_t ref_clk, uint8_t msr_clk,uint8_t duration)
{
	uint32_t timeout = FREQM_NUM_OF_ATTEMPTS;
	freqm_disable_refclk(FREQM);
	while (freqm_get_status(FREQM) & FREQM_STATUS_RCLKBUSY) {
		if (!timeout--) {
			break;
		}	
	}
	p_freqm->FREQM_MODE |= FREQM_MODE_REFSEL(ref_clk);
	freqm_enable_refclk(FREQM);
	while (freqm_get_status(FREQM) & FREQM_STATUS_RCLKBUSY) {
		if (!timeout--) {
			break;
		}	
	}
	p_freqm->FREQM_MODE |= FREQM_MODE_REFNUM(duration);
	p_freqm->FREQM_MODE |= FREQM_MODE_CLKSEL(msr_clk);
}

/**
 * \brief Get measurement result.
 */
uint32_t freqm_get_result(Freqm *p_freqm)
{
	uint32_t timeout = FREQM_NUM_OF_ATTEMPTS;
	
	/* Wait until the measurement is done */
	while (freqm_get_status(FREQM) & FREQM_STATUS_BUSY) {
		if (!timeout--) {
			break;
		}	
	}
	return p_freqm->FREQM_VALUE;
}

/**
 * \brief Enable FEQM.
 *
 * \param p_freqm Pointer to a FREQM instance.
 *
 */
void freqm_enable(Freqm *p_freqm)
{
	sysclk_enable_peripheral_clock(p_freqm);
	sleepmgr_lock_mode(SLEEPMGR_SLEEP_1);
}

/**
 * \brief Disable FREQM.
 *
 * \param p_freqm Pointer to a FREQM instance.
 *
 */
void frqem_disable(Freqm *p_freqm)
{
	uint32_t timeout = FREQM_NUM_OF_ATTEMPTS;
	
	/* Wait until the measurement is done */
	while (freqm_get_status(FREQM) & FREQM_STATUS_BUSY) {
		if (!timeout--) {
			break;
		}	
	}
	sysclk_disable_peripheral_clock(p_freqm);
	sleepmgr_unlock_mode(SLEEPMGR_SLEEP_1);
}

/**
 * \brief Set callback for FREQM interrupt handler
 *
 * \param p_freqm Pointer to a FREQM instance
 * \param callback Callback function pointer.
 * \param irq_line  Interrupt line.
 * \param irq_level Interrupt level.
 * \param ul_sources Interrupt sources.
 */
void freqm_set_callback(Freqm *p_freqm, freqm_callback_t callback,
		uint8_t irq_line, uint8_t irq_level, uint32_t ul_sources)
{
	freqm_callback = callback;
	irq_register_handler((IRQn_Type)irq_line, irq_level);
	freqm_enable_interrupt(p_freqm, ul_sources);
}

/**
 * \brief Interrupt handler for FREQM.
 */
void FREQM_Handler(void)
{
	if(freqm_callback) {
		freqm_callback();
	}
}

//@}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
