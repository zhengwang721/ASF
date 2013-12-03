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

#ifndef PDM_H_INCLUDED
#define PDM_H_INCLUDED

#include "compiler.h"
#include "status_codes.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \addtogroup sam_drivers_pdm_group
 *
 * See \ref sam_pdm_quickstart.
 *
 * @{
 */

/**
 * \name PDM low level access
 *
 * @{
 */

/**
 * Interrupt source (bitmap)
 */
enum pdmic_interrupt_source {
	PDMIC_INTERRUPT_SRC_DATA_READY = PDMIC_ISR_DRDY,
	PDMIC_INTERRUPT_SRC_OVERRUN = PDMIC_ISR_OVRE,
	PDMIC_INTERRUPT_SRC_BUFFER_END = PDMIC_ISR_ENDRX,
	PDMIC_INTERRUPT_SRC_TRANSFER_END = PDMIC_ISR_RXBUFF
};

/**
 * Resets the PDMIC, simulating a hardware reset
 * \param[in] pdmic PDMIC peripheral base.
 */
__always_inline static void pdmic_reset(Pdmic *pdmic)
{
	pdmic->PDMIC_CR = PDMIC_CR_SWRST;
}

/**
 * Enables the PDM and starts the conversions
 * \param[in] pdmic PDMIC peripheral base.
 */
__always_inline static void pdmic_conversion_enable(Pdmic *pdmic)
{
	pdmic->PDMIC_CR = PDMIC_CR_ENPDM;
}

/**
 * Disable the PDM conversions
 * \param[in] pdmic PDMIC peripheral base.
 */
__always_inline static void pdmic_conversion_disable(Pdmic *pdmic)
{
	pdmic->PDMIC_CR = (~PDMIC_CR_ENPDM & ~PDMIC_CR_SWRST);
}

/**
 * Set prescale for the PDMIC
 * \param[in] pdmic PDMIC peripheral base.
 */
__always_inline static void pdmic_set_prescal(Pdmic *pdmic, uint8_t prescal)
{
	pdmic->PDMIC_MR = PDMIC_MR_PRESCAL(prescal);
}

/**
 * The analog-to-digital conversion data is placed into this register at the end
 * of a conversion and remains until it is read
 * \param[in] pdmic PDMIC peripheral base.
 */
__always_inline static uint32_t pdmic_get_convert_data(Pdmic *pdmic)
{
	return pdmic->PDMIC_CDR;
}

/**
 * Enable interrupts
 * \param[in] pdmic PDMIC peripheral base.
 * \param[in] sources Interrupts sources bitmap.
 */
__always_inline static void pdmic_interrupts_enable(Pdmic *pdmic,
		enum pdmic_interrupt_source sources)
{
	pdmic->PDMIC_IER = sources;
}

/**
 * Disable interrupts
 * \param[in] pdmic PDMIC peripheral base.
 * \param[in] sources Interrupts sources bitmap.
 */
__always_inline static void pdmic_interrupts_disable(Pdmic *pdmic,
		enum pdmic_interrupt_source sources)
{
	pdmic->PDMIC_IDR = sources;
}

/**
 * Return interrupt mask
 * \param[in] pdmic PDMIC peripheral base.
 */
__always_inline static uint32_t pdmic_get_interrupt_mask(Pdmic *pdmic)
{
	return pdmic->PDMIC_IMR;
}

/**
 * Return status
 * \param[in] pdmic PDMIC peripheral base.
 */
__always_inline static uint32_t pdmic_get_status(Pdmic *pdmic)
{
	return pdmic->PDMIC_ISR;
}

/**
 * Return PDMIC PDC base
 * \param[in] pdmic PDMIC peripheral base.
 */
__always_inline static Pdc *pdmic_get_pdc_base(Pdmic *pdmic)
{
	return (Pdc *)((uint32_t)pdmic + 0x100);
}

/**
 * Set PDMIC buffer
 * \param[in] pdmic PDMIC peripheral base.
 * \param[in] buffer Source buffer address
 */
__always_inline static void pdmic_buffer_set(Pdmic *pdmic,
		uint8_t *buffer)
{
	pdmic->PDMIC_RPR = (uint32_t)buffer;
}

/**
 * Set PDMIC next buffer address
 * \param[in] pdmic PDMIC peripheral base.
 * \param[in] buffer Next source buffer address
 */
__always_inline static void pdmic_transfer_set_next(Pdmic *pdmic,
		uint8_t *buffer)
{
	pdmic->PDMIC_RNPR = (uint32_t)buffer;
}

/**
 * Enable PDMIC transfer
 * \param[in] pdmic PDMIC peripheral base.
 */
__always_inline static void pdmic_transfer_enable(Pdmic *pdmic)
{
	pdmic->PDMIC_PTCR = PDMIC_PTCR_RXTEN | PDMIC_PTCR_RXCBEN;
}

/**
 * Disable PDMIC transfer
 * \param[in] pdmic PDMIC peripheral base.
 */
__always_inline static void pdmic_transfer_disable(Pdmic *pdmic)
{
	pdmic->PDMIC_PTCR = PDMIC_PTCR_RXTDIS | PDMIC_PTCR_RXCBDIS;
}

/**
 * Check if PDMIC transfer is enabled (in progress)
 * \param[in] pdmic PDMIC peripheral base.
 */
__always_inline static bool pdmic_transfer_is_enabled(Pdmic *pdmic)
{
	return ((pdmic->PDMIC_PTSR & (PDMIC_PTSR_RXTEN | PDMIC_PTSR_RXCBEN))
	       == (PDMIC_PTSR_RXTEN | PDMIC_PTSR_RXCBEN));
}

/**
 * Configure DSP configuration register 0
 * \param[in] pdmic PDMIC peripheral base.
 * \param[in] cfg0 Configurations to DSP configuration regsiter 0.
 */
__always_inline static void pdmic_dsp_cfg0(Pdmic *pdmic, uint32_t cfg0)
{
	pdmic->PDMIC_DSPR0 = cfg0;
}

/**
 * Configure DSP configuration register 1
 * \param[in] pdmic PDMIC peripheral base.
 * \param[in] cfg1 Configurations to DSP configuration regsiter 1.
 */
__always_inline static void pdmic_dsp_cfg1(Pdmic *pdmic, uint32_t cfg1)
{
	pdmic->PDMIC_DSPR1 = cfg1;
}

/**
 * Enable write protect
 * \param[in] pdmic PDMIC peripheral base.
 */
__always_inline static void pdmic_write_protect_enable(Pdmic *pdmic)
{
	pdmic->PDMIC_WPMR = PDMIC_WPMR_WPEN | PDMIC_WPMR_WPKEY_PASSWD;
}

/**
 * Disable write protect
 * \param[in] pdmic PDMIC peripheral base.
 */
__always_inline static void pdmic_write_protect_disable(Pdmic *pdmic)
{
	pdmic->PDMIC_WPMR = PDMIC_WPMR_WPKEY_PASSWD;
}

/**
 * Get write protect status
 * \param[in] pdmic PDMIC peripheral base.
 */
__always_inline static uint32_t pdmic_get_write_protect_status(Pdmic *pdmic)
{
	return pdmic->PDMIC_WPSR;
}

/** @} */

/**
 * \name PDMIC driver access
 *
 * @{
 */

/** Forward definition of the PDMIC driver instance */
struct pdm_module;
/** Type of the callback function for PDMIC driver  */
typedef void (*pdm_callback_t)(const struct pdm_module *const module);

/**
 * Callback types for PDM callback driver
 */
enum pdm_callback_type {
	/** Callback for transfer done */
	PDM_CALLBACK_TRANSFER_END,
	/** Callback for any of transfer buffer done */
	PDM_CALLBACK_BUFFER_END,
	/** Callback for data overrun */
	PDM_CALLBACK_OVERRUN,
#  if !defined(__DOXYGEN__)
	/** Number of available callbacks */
	PDM_CALLBACK_N
#  endif
};

/**
 * \name PDM Module status flags
 *
 * PDM status flags, returned by \ref pdmic_get_status().
 *
 * @{
 */

/** PDM End of (one of the) Buffer */
#define PDM_STATUS_BUFFER_END    (1UL << 0)
/** PDM Buffer Full (End of transfer) */
#define PDM_STATUS_TRANSFER_END  (1UL << 1)

/** Default prescale for PDMIC */
#define PDM_DEFAULT_PRESCAL (3)

/** @} */

/**
 * PDM software driver instance structure.
 */
struct pdm_module {
	/** Base address for PDMIC0 peripheral */
	Pdmic *hw;
	/** Callback functions */
	pdm_callback_t callbacks[PDM_CALLBACK_N];
	/** Bit mask for enabled callbacks */
	uint8_t enabled_callbacks;
	/** Status of the last job */
	volatile enum status_code job_status;
	/** Pointer to the converted data buffer */
	uint32_t addr;
	/** Converted data size */
	uint32_t buffer_size;
	/** Pointer to the next converted data buffer */
	uint32_t next_addr;
	/** Next converted data size */
	uint32_t next_buffer_size;
};

/**
 * PDM configuration structure
 */
struct pdm_config {
	/** prescal determines the frequency of PDMCLK.*/
	uint8_t prescal;
	/** High-pass filter enabled flag */
	bool high_pass_filter_bypass;
	/** Droop compensation filter enabled flag */
	bool droop_compensation_filter_bypass;
	/** Converted data size */
	uint8_t conver_data_size;
	/** Oversampling Ratio */
	uint8_t oversampling_radio;
	/** Shifts the multiplication operation result by SCALE bits to the right */
	uint8_t data_scale;
	/** Shifts the scaled result by SHIFT bits to the right */
	uint8_t data_shift;
	/** Gain correction to apply to the final result */
	uint16_t gain;
	/** Offset correction to apply to the final result */
	uint16_t offset;
};

/**
 * \name Driver initialization and configuration
 * @{
 */

enum status_code pdm_init(struct pdm_module *const pdm, Pdmic *hw,
		struct pdm_config *const cfg);

/**
 * \brief Initializes PDMIC configuration structure to defaults
 *
 * Initializes a given PDMIC configuration struct to a set of known default
 * values. This function should be called on any new instance of the
 * configuration struct before being modified by the user application.
 *
 * The default configuration is as follows:
 * - PDMCLK = MCK/8
 * - High-pass filter enabled
 * - Droop compensation filter enabled.
 * - Converted data size is 16-bit.
 * - Oversampling ratio is 128
 * - No data scale
 * - No data shift
 * - Gain is set to 0
 * - Offset is set to 0
 *
 * \param[out] cfg Pointer to configuration struct to initialize to defaults
 */
__always_inline static void pdm_get_config_default(struct pdm_config *const cfg)
{
	Assert(cfg);
	cfg->prescal = PDM_DEFAULT_PRESCAL;
}

/** @} */

/**
 * \name Status Management
 * @{
 */

/**
 * \brief Retrieves the current module status.
 *
 * Retrieves the status of the module, giving overall state information.
 *
 * \param[in] pdm  Pointer to the PDMIC software instance struct
 *
 * \return Bitmask of \c PDM_STATUS_* flags
 *
 * \retval PDM_STATUS_TRANSFER_END  All of PDC buffers are done
 * \retval PDM_STATUS_BUFFER_END    One of PDC buffer is done
 */
__always_inline static uint32_t pdm_get_status(struct pdm_module *const pdm)
{
	Assert(pdm);
	Assert(pdm->hw);
	uint32_t isr = pdm->hw->PDMIC_ISR;
	return isr; /* Since the bits in register and driver definition are same */
}

/** @} */

enum status_code pdm_conversion_wait(void);

/**
 * \name Enable and disable interrupts
 * @{
 */

/**
 * \brief Enable interrupt
 *
 * Enable the given interrupt request from the PDMIC module .
 *
 * \param[in] pdm Pointer to the PDMIC software instance struct
 * \param[in] interrupt Interrupts to enable
 */
static inline void pdm_enable_interrupt(struct pdm_module *const pdm,
		enum pdmic_interrupt_source interrupt)
{
	/* Sanity check arguments */
	Assert(pdm);
	Assert(pdm->hw);
	pdm->hw->PDMIC_IER = interrupt;
}

/**
 * \brief Disable interrupt
 *
 * Disable the given interrupt request from the PDM module.
 *
 * \param[in] pdm Pointer to the PDMIC software instance struct
 * \param[in] interrupt Interrupt to disable
 */
static inline void pdm_disable_interrupt(struct pdm_module *const pdm,
		enum pdmic_interrupt_source interrupt)
{
	/* Sanity check arguments */
	Assert(pdm);
	Assert(pdm->hw);
	pdm->hw->PDMIC_IDR = interrupt;
}

/** @} */

/**
 * \name Callback Management
 * @{
 */

void pdm_register_callback(struct pdm_module *const pdm,
		pdm_callback_t callback, enum pdm_callback_type type);

void pdm_unregister_callback(struct pdm_module *const pdm,
		enum pdm_callback_type type);

/**
 * \brief Enables callback
 *
 * Enables the callback function registered by \ref
 * pdmic_register_callback. The callback function will be called from the
 * interrupt handler when the conditions for the callback type are met.
 *
 * \param[in]     pdm  Pointer to PDMIC software instance struct
 * \param[in]     type Callback type given by an enum
 */
__always_inline static void pdm_enable_callback(struct pdm_module *const pdm,
		enum pdm_callback_type type)
{
	Assert(pdm);
	Assert(pdm->hw);
	if (type < PDM_CALLBACK_N) {
		pdm->enabled_callbacks |= (1 << type);
	}
}

/**
 * \brief Disables callback
 *
 * Disables the callback function registered by the \ref
 * pdmic_register_callback.
 *
 * \param[in]     pdm  Pointer to PDMIC software instance struct
 * \param[in]     type Callback type given by an enum
 */
__always_inline static void pdm_disable_callback(struct pdm_module *const pdm,
		enum pdm_callback_type type)
{
	Assert(pdm);
	Assert(pdm->hw);
	if (type < PDM_CALLBACK_N) {
		pdm->enabled_callbacks &= ~(1 << type);
	}
}

/** @} */

/**
 * \name Job Management
 * @{
 */

enum status_code pdm_conversion_job(void);

enum status_code pdm_get_job_status(struct pdm_module *const pdm);

void pdm_abort_job(struct pdm_module *const pdm);

/** @} */

/** @} */

/** @} */

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \page sam_pdm_quickstart Quickstart guide for SAM pdm module
 *
 * This is the quickstart guide for the "SAM pdm module", with step-by-step
 * instructions on how to configure and use the module in a selection of use
 * cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g.the main application function.
 *
 * \section pdm_use_cases PDM use cases
 * - \ref pdm_basic_use_case
 * - \subpage pdm_use_case_2
 *
 * \section pdm_basic_use_case Basic use case
 * In this basic use case, the pdm module are configured for:
 * - Block the program till receiving 1024 converted data from PDMIC0 with
 * default configuration.
 * - The default configuration is as follows:
 *   PDMCLK = MCK/8
 *   High-pass filter enabled
 *   Droop compensation filter enabled.
 *   Converted data size is 16-bit.
 *   Oversampling ratio is 128
 *   No data scale
 *   No data shift
 *   Gain is set to 0
 *   Offset is set to 0
 * \note If the application expected receving converted again after a blocking
 * conversion, the pdm module requires re-initialized.
 *
 * \subsection sam_pdm_quickstart_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (Sysclock)"
 *
 * \section pdm_basic_use_case_setup Setup steps
 * \subsection pdm_basic_use_case_setup_code Example code
 * Add to application C-file:
 * \code
 * struct pdm_module pdm;
 * struct pdm_config pdm_cfg;
 * uint8_t mem[1024];
 *
 * pdm_get_config_default(&pdm_cfg);
 * pdm.addr = (uint32_t)mem;
 * pdm.buffer_size = 1024;
 * pdm_init(&pdm, PDMIC0, &pdm_cfg);
 *
 * \endcode
 *
 * \subsection pdm_basic_use_case_setup_flow Workflow
 * -# Get default configurations:
 *   - \code
 *   pdm_get_config_default(&pdm_cfg);
 * \endcode
 * -# Set the buffer for the converted data:
 *   - \code
 *   pdm.addr = (uint32_t)mem;
 *   pdm.buffer_size = 1024;
 * \endcode
 * -# Initialize the pdm module with configuration and buffer address and size:
 *   - \code
 *   pdm_init(&pdm, PDMIC0, &pdm_cfg);
 * \endcode
 * -# Enable pdm module blocking conversion:
 *   - \code pdm_conversion_wait(); \endcode
 */
/**
 * \page pdm_use_case_2 Advanced use case - Get conversion data in background job
 * In this case, the pdm module is configured to transfer data without
 * CPU intervention. Callbacks will be invoked after conversion is done.
 *
 * \section pdm_advanced_use_case_setup Setup steps
 * \subsection pdm_advanced_use_case_setup Example code
 * Add to application C-file:
 * \code
 * static void _transfer_done(const struct pdm_module *const module)
 * {
 *    UNUSED(module);
 *    // Do something here as conversion data received.
 * }
 *
 * \endcode
 * Add to application main routine:
 * \code
 * struct pdm_module pdm;
 * struct pdm_config pdm_cfg;
 * uint8_t mem[1024];
 *
 * pdm_get_config_default(&pdm_cfg);
 * pdm.addr = (uint32_t)mem;
 * pdm.buffer_size = 1024;
 * pdm_init(&pdm, PDMIC0, &pdm_cfg);
 *
 * pdm_register_callback(&pdm, _transfer_done, PDM_CALLBACK_TRANSFER_END);
 * pdm_enable_callback(&pdm, PDM_CALLBACK_TRANSFER_END);
 *
 * \endcode
 *
 * \subsection pdm_basic_use_case_setup_flow Workflow
 * -# Add the callback routine for receiving 1024 converted data buffer
 *   - \code
 * static void _transfer_done(const struct pdm_module *const module)
 * {
 *    UNUSED(module);
 *    transfer_is_done = true;
 * }
 * \endcode
 * -# Get default configurations:
 *   - \code
 *   pdm_get_config_default(&pdm_cfg);
 * \endcode
 * -# Set the buffer for the converted data:
 *   - \code
 *   pdm.addr = (uint32_t)mem;
 *   pdm.buffer_size = 1024;
 * \endcode
 * -# Initialize the pdm module with configuration and buffer address and size:
 *   - \code
 *   pdm_init(&pdm, PDMIC0, &pdm_cfg);
 * \endcode
 * -# Register the callback when conversion done.
 *   - \code
 *   pdm_register_callback(&pdm, _transfer_done, PDM_CALLBACK_TRANSFER_END);
 *   pdm_enable_callback(&pdm, PDM_CALLBACK_TRANSFER_END);
 * \endcode
 * -# Enable pdm module conversion job:
 *   - \code pdm_conversion_job(); \endcode
 */

#endif /* PDM_H_INCLUDED */

