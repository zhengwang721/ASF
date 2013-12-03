/**
 * \file
 *
 * \brief PDM driver for SAM.
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

#include "pdm.h"
#include "sysclk.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \defgroup sam_drivers_pdm_group Pulse-Density-Modulation Interface Controller
 * (PDMIC) Driver.
 *
 * \par Purpose
 *
 * The Pulse Density Modulation Interface Controller (PDMIC) is a PDM interface
 * controller and decoder that is compatible with mono/stereo PDM format. It
 * integrates a clock generator driving the PDM microphones and embeds filters
 * which decimate the incoming bitstream to obtain most common audio rates.
 * @{
 */
/** PDMIC interface numbers */
#define PDMIC_NUM 2

static struct pdm_module *_pdm_instances[PDMIC_NUM];

/**
 * \brief PDM Interrupt Handler
 *
 * \param[in]  pdm_num  PDMIC controller number
 */
static void PDMIC_Handler(uint8_t pdm_num)
{
	struct pdm_module *pdm = _pdm_instances[pdm_num];
	uint32_t isr = pdm->hw->PDMIC_ISR;
	/* All of the buffer done */
	if (isr & PDMIC_ISR_RXBUFF) {
		/* Disable PDC */
		pdm->hw->PDMIC_PTCR = (PDMIC_PTCR_RXTDIS);
		/* Disable Interrupts */
		pdm->hw->PDMIC_IDR = 0xFFFFFFFF;
		/* Change job status */
		pdm->job_status = STATUS_OK;
		/* Callback */
		if ((pdm->enabled_callbacks &
				(1 << PDM_CALLBACK_TRANSFER_END)) &&
				pdm->callbacks[PDM_CALLBACK_TRANSFER_END]) {
			pdm->callbacks[PDM_CALLBACK_TRANSFER_END](pdm);
		}
	}
	/* One of the buffer done */
	else if (isr & PDMIC_ISR_ENDRX) {
		/* Stop PDC (Pause and then start again after callback) */
		pdm->hw->PDMIC_PTCR = (PDMIC_PTCR_RXTDIS);
		/* Disable Interrupt */
		pdm->hw->PDMIC_IDR = PDMIC_IDR_ENDRX;
		/* Callback */
		if ((pdm->enabled_callbacks & (1 << PDM_CALLBACK_BUFFER_END)) &&
				pdm->callbacks[PDM_CALLBACK_BUFFER_END]) {
			pdm->callbacks[PDM_CALLBACK_BUFFER_END](pdm);
		}

		/* Continue PDC */
		pdm->hw->PDMIC_PTCR = (PDMIC_PTCR_RXTEN);
	}
	/* Over Run */
	else if (isr & PDMIC_ISR_OVRE) {
		/* Callback */
		if ((pdm->enabled_callbacks & (1 << PDM_CALLBACK_OVERRUN)) &&
				pdm->callbacks[PDM_CALLBACK_OVERRUN]) {
			pdm->callbacks[PDM_CALLBACK_OVERRUN](pdm);
		}
	}
}

/**
 * \brief PDMIC 0 Interrupt Handler
 */
void PDMIC0_Handler(void)
{
	PDMIC_Handler(0);
}


/**
 * \brief PDMIC 1 Interrupt Handler
 */
void PDMIC1_Handler(void)
{
	PDMIC_Handler(1);
}

/**
 * \brief Initializes the PDM module
 *
 * Initializes the PDM struct and the hardware module based on the
 * given configuration struct values and converted data buffers.
 *
 * \note The PDC buffer and the configurations for the pdm module must be
 *  initialized before calling this function.
 *
 * \param pdm         Pointer to the PDM software instance struct
 * \param  hw          Pointer to the PDMIC hardware instance
 * \param  cfg         Pointer to the configuration struct
 *
 * \return Status of the initialization procedure
 * \retval STATUS_OK                The initialization was successful
 * \retval ERR_INVALID_ARG          Invalid argument(s) were provided
 * \retval ERR_BUSY                 The module is busy
 */
enum status_code pdm_init(struct pdm_module *const pdm, Pdmic *hw,
		struct pdm_config *const cfg)
{
	uint32_t i;
	uint32_t dsp_cfg = 0;
	Assert(pdm);
	Assert(pdm->buffer_size);
	Assert(hw);
	Assert(cfg);

	/* HW base init */
	pdm->hw = hw;

	hw->PDMIC_CR = PDMIC_CR_SWRST;

	/* Enable PMC clock */
	if (hw == PDMIC1) {
		sysclk_enable_peripheral_clock(ID_PDMIC1);
		_pdm_instances[1] = pdm;
	} else {
		sysclk_enable_peripheral_clock(ID_PDMIC0);
		_pdm_instances[0] = pdm;
	}

	/* Transfer must be stopped (done/aborted) before initialization */
	if ((hw->PDMIC_PTSR & (PDMIC_PTSR_RXTEN))) {
		return ERR_BUSY;
	}

	/* Oversampling only support 0 and 1 */
	if (cfg->oversampling_radio > 1) {
		return ERR_INVALID_ARG;
	}

	/* Set prescal */
	hw->PDMIC_MR = PDMIC_MR_PRESCAL(cfg->prescal);

	/* Initialize DSP0 */
	if (cfg->high_pass_filter_bypass) {
		dsp_cfg |= PDMIC_DSPR0_HPFBYP;
	}
	if (cfg->droop_compensation_filter_bypass) {
		dsp_cfg |= PDMIC_DSPR0_SINBYP;
	}
	if (cfg->conver_data_size) {
		dsp_cfg |= PDMIC_DSPR0_SIZE;
	}
	if (cfg->oversampling_radio) {
		dsp_cfg |= PDMIC_DSPR0_OSR_64;
	} else {
		dsp_cfg |= PDMIC_DSPR0_OSR_128;
	}
	dsp_cfg |= PDMIC_DSPR0_SCALE(cfg->data_scale) |
			PDMIC_DSPR0_SHIFT(cfg->data_shift);
	hw->PDMIC_DSPR0 = dsp_cfg;

	/* Set gain and offset */
	hw->PDMIC_DSPR1 = PDMIC_DSPR1_DGAIN(cfg->gain) |
			PDMIC_DSPR1_OFFSET(cfg->offset);

	/* Set PDC buffer */
	hw->PDMIC_RPR = pdm->addr;
	hw->PDMIC_RCR = pdm->buffer_size;
	hw->PDMIC_RNPR = pdm->next_addr;
	hw->PDMIC_RNCR = pdm->next_buffer_size;

	/* Initialize for Jobs */
	for (i = 0; i < PDM_CALLBACK_N; i++) {
		pdm->callbacks[i] = NULL;
	}
	pdm->enabled_callbacks = 0;
	pdm->job_status = STATUS_OK;

	return STATUS_OK;
}

/**
 * \brief Blocking PDM conversion
 *
 * \return Status of the transfer.
 * \retval STATUS_OK Data transfered all right
 * \retval ERR_UNSUPPORTED_DEV  PDM module not found
 */
enum status_code pdm_conversion_wait(void)
{
	struct pdm_module *pdm = NULL;

	if (_pdm_instances[0] != NULL) {
		PDMIC0->PDMIC_PTCR = PDMIC_PTCR_RXTEN;
		PDMIC0->PDMIC_CR = PDMIC_CR_ENPDM;
		pdm = _pdm_instances[0];
	}

	if (_pdm_instances[1] != NULL) {
		PDMIC1->PDMIC_PTCR = PDMIC_PTCR_RXTEN;
		PDMIC1->PDMIC_CR = PDMIC_CR_ENPDM;
		pdm = _pdm_instances[1];
	}

	if (pdm == NULL) {
		return ERR_UNSUPPORTED_DEV;
	}

	/* Concurrence operations as bitstream sampled are on the rising/falling
	 *  edge for PDMIC0/PDMIC1 */
	while (0 == (pdm->hw->PDMIC_ISR & PDMIC_ISR_ENDRX)) {
	}

	/* Stop conversion */
	if (_pdm_instances[1] != NULL) {
		PDMIC1->PDMIC_CR = PDMIC_CR_SWRST;
		_pdm_instances[1] = NULL;
	}

	if (_pdm_instances[0] != NULL) {
		PDMIC0->PDMIC_CR = PDMIC_CR_SWRST;
		_pdm_instances[0] = NULL;
	}

	return STATUS_OK;
}

/**
 * \brief None-blocking PDM conversion
 *
 * \return Status of the job start.
 * \retval STATUS_OK Job started or queued successful
 */
enum status_code pdm_conversion_job(void)
{
	if (_pdm_instances[1] != NULL) {
		PDMIC1->PDMIC_IER = PDMIC_IER_ENDRX | PDMIC_IER_RXBUFF | PDMIC_IER_OVRE;
		NVIC_EnableIRQ(PDMIC1_IRQn);
	}

	if (_pdm_instances[0] != NULL) {
		PDMIC0->PDMIC_IER = PDMIC_IER_ENDRX | PDMIC_IER_RXBUFF | PDMIC_IER_OVRE;
		NVIC_EnableIRQ(PDMIC0_IRQn);
	}

	/* Start actual conversion, the data will handled in the ISR */
	if (_pdm_instances[0] != NULL) {
		PDMIC0->PDMIC_PTCR = PDMIC_PTCR_RXTEN;
		PDMIC0->PDMIC_CR = PDMIC_CR_ENPDM;
	}

	if (_pdm_instances[1] != NULL) {
		PDMIC1->PDMIC_PTCR = PDMIC_PTCR_RXTEN;
		PDMIC1->PDMIC_CR = PDMIC_CR_ENPDM;
	}

	return STATUS_OK;
}

/**
 * \brief Gets the status of a job
 *
 * Gets the status of an ongoing or the last job.
 *
 * \param [in]  pdm    Pointer to the PDM software instance struct
 *
 * \return Status of the job
 * \retval STATUS_OK Transfer finished/One of buffer done
 * \retval ERR_BUSY  Transfer in progress
 */
enum status_code pdm_get_job_status(struct pdm_module *const pdm)
{
	Assert(pdm);

	return (0 == pdm->hw->PDMIC_RCR) ? STATUS_OK : ERR_BUSY;
}

/**
 * \brief Aborts all ongoing jobs
 *
 * Aborts all ongoing jobs.
 *
 * \param [in]  pdm    Pointer to the PDM software instance struct
 */
void pdm_abort_job(struct pdm_module *const pdm)
{
	Assert(pdm);
	Assert(pdm->hw);
	/* Disable PDC */
	pdm->hw->PDMIC_PTCR = (PDMIC_PTCR_RXTDIS | PDMIC_PTCR_RXCBDIS);
	/* Disable interrupts */
	pdm->hw->PDMIC_IDR = 0xFFFFFFFF;
	pdm->hw->PDMIC_CR = PDMIC_CR_SWRST;
	/* Mark job aborted */
	pdm->job_status = ERR_ABORTED;
}

/**
 * \brief Registers a callback
 *
 * Registers a callback function which is implemented by the user.
 *
 * \note The callback must be enabled by for the interrupt handler to call it
 * when the condition for the callback is met.
 *
 * \param[in] pdm       Pointer to PDM software instance struct
 * \param[in] callback  Pointer to callback function
 * \param[in] type      Callback type given by an enum
 *
 */
void pdm_register_callback(struct pdm_module *const pdm,
		pdm_callback_t callback, enum pdm_callback_type type)
{
	Assert(pdm);
	Assert(callback);
	if (type < PDM_CALLBACK_N) {
		pdm->callbacks[type] = callback;
	}
}

/**
 * \brief Unregisters a callback
 *
 * Unregisters a callback function which is implemented by the user.
 *
 * \note The callback must be enabled by for the interrupt handler to call it
 * when the condition for the callback is met.
 *
 * \param[in] pdm       Pointer to PDM software instance struct
 * \param[in] type      Callback type given by an enum
 *
 */
void pdm_unregister_callback(struct pdm_module *const pdm,
		enum pdm_callback_type type)
{
	Assert(pdm);
	if (type < PDM_CALLBACK_N) {
		pdm->callbacks[type] = NULL;
	}
}

/** @} */

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
