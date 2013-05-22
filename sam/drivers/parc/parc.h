/**
 * \file
 *
 * \brief PARC driver for SAM.
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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

#ifndef PARC_H_INCLUDED
#define PARC_H_INCLUDED

#include "compiler.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

#if PARC_CALLBACK_MODE == true
extern struct adc_module *adc_module_instance;

#endif
/**
 * \defgroup group_sam_drivers_parc PARC - Parallel Capture
 *
 * Driver for the PARC (Parallel Capture).
 * Provides functions for configuring and operating the PARC.
 *
 * \{
 */

/**
 * \brief Sampling Edge
 *
 *  Enum for possible sampling edges for PARC.
 *
 */
enum parc_sampling_edge {
	/** Data capture occurs on rising edge of PCCK */
	PARC_RISING_EDGE = 0,
	/** Data capture occurs on falling edge of PCCK */
	PARC_FALLING_EDGE = 1,
};


/**
 * \brief Capture mode
 *
 * Enum for possible captured byte selection.
 *
 */
enum parc_capture_mode {
	/* Bytes are captured when data captured every cycle*/
	PARC_BOTH_CAPTURE = 0,
	/** Even bytes are captured when data captured every two cycles */
	PARC_EVEN_CAPTURE,
	/** Odd bytes are captured when data captured every two cycles */
	PARC_ODD_CAPTURE,
};

/**
 * \brief Sample mode
 *
 * Enum for possible SMODE (required EN pins) for PARC
 *
 */
enum parc_smode {
	/** Data capture occurs with PCEN1 high */
	PARC_SMODE_PCEN1_H,
	/** Data capture occurs with PCEN1 and PCEN2 high */
	PARC_SMODE_PCEN1_AND_PCEN2_H,
	/** Data capture occurs with PCEN1 or PCEN2 high */
	PARC_SMODE_PCEN1_OR_PCEN2_H,
	/** Data capture always occurs */
	PARC_SMODE_ALWAYS,
};

/**
 * \brief Data Size
 *
 * Enum for the possible transferred data size for PARC
 *
 */
enum parc_data_size {
	/** Data size is 8bits */
	PARC_DSIZE_BYTE = 0,
	/** Data size is 16bits */
	PARC_DSIZE_HALFWORD = 1,
	/** Data size is 32bits */
	PARC_DSIZE_WORD = 2,
};

/**
 * \brief interrupt type enum
 *
 * Enum for interrupt source for PARC.
 *
 */
enum parc_interrupt_source {
	PARC_INTERRUPT_DRDY = PARC_SR_DRDY,
	PARC_INTERRUPT_OVR = PARC_SR_OVR,
};

/**
 * \brief Status enum
 *
 * Enum for status bit for PARC.
 *
 */
enum parc_status {
	PARC_STATUS_EN = PARC_SR_EN;
	PARC_STATUS_CS = PARC_SR_CS;
	PARC_STATUS_DRDY = PARC_SR_DRDY;
	PARC_STATUS_OVR = PARC_SR_OVR;
};

#if PARC_CALLBACK_MODE == true
/**
 * \brief PARC Callback enum
 *
 * Callbacks for the PARC driver
 */
enum parc_callback {
	PARC_CALLBACK_DRDY,
	PARC_CALLBACK_OVR,
	PARC_CALLBACK_N,
}
#endif
/**
 * \brief PARC configuration structure
 *
 * Configuration structure for an PARC instance. This structure should be
 * initialized by the \ref parc_get_config_defaults()
 * function before being modified by the user application.
 */
struct parc_config {
	/*
	 * Capture mode: every byte, or odd/even indexd bytes are captured.
	 */
	enum parc_capture_mode capture_mode;

	/*
	 * Sampling edge select: capture occurs on rising edge or falling
	 * edge of PCCK.
	 */
	enum parc_sampling_edge sampling_edge;

	/*
	 * Sampling mode: data capture can occur under 4 types of sampling
	 * modes:
	 * PCEN1 is high, PCEN1 and PCEN2 are high, PCEN1 or PCEN2 is high,
	 * always capture data.
	 */
	enum parc_smode smode;

	/*
	 * Data size: effective data size in RHR register. Captured data can be
	 * concatenated to make 16-bit or 32-bit in RHR
	 */
	enum parc_data_size dsize;
};

/**
 * \brief PARC driver software instance structure.
 *
 * Device instance structure for a PARC driver instance. This
 * structure should be initialized by the \ref parc_init() function to
 * associate the instance with a particular hardware module of the device.
 */
struct parc_module {
	/** Base address of the PARC module. */
	Parc				*hw;
	/** Pointer to PARC configuration structure. */
	struct parc_config	*parc_cfg;

#if PARC_CALLBACK_MODE == true
	/** Array to store callback functions */
	parc_callback_t callback[PARC_CALLBACK_N];
	/** Bit mask for callbacks registered */
	uint8_t registered_callback_mask;
	/** Bit mask for callbacks enabled */
	uint8_t enabled_callback_mask;
#endif
}

/**
 * \brief Initializes the PARC
 *
 * Initializes the PARC device struct and the hardware module based on the
 * given configuration struct values.
 *
 * \param[out] module_inst Pointer to the PARC software instance struct
 * \param[in]  hw          Pointer to the PARC module instance
 * \param[in]  config      Pointer to the configuration struct
 *
 * \return Status of the initialization procedure
 * \retval STATUS_OK                The initialization was successful
 * \retval STATUS_ERR_INVALID_ARG   Invalid argument(s) were provided
 */
enum status_code parc_init(
	struct parc_module *const module_inst,
	Parc *const hw,
	struct parc_config *const config);

/**
 * \Writes an PARC configuration to the hardware module
 *
 * Writes a given PARC module configuration to the hardware module.
 *
 * \param[out] module_inst  Pointer to the PARC software instance struct
 * \param[in]  config       Pointer to configuration struct
 *
 * \return Status of the configuration procedure
 * \retval STATUS_OK               The configuration was successful
 * \retval STATUS_ERR_INVALID_ARG  Invalid argument(s) were provided
 */
enum status_code parc_set_config(
	struct parc_module *const module_inst,
	struct parc_config *config);

/**
 * \brief Initializes an PARC configuration structure to defaults
 *
 * Initializes a given PARC configuration struct to a set of known default
 * values. This function should be called on any new instance of the
 * configuration struct before being modified by the user application.
 * 
 * The default configurations are:
 * - Capture every btye
 * - Sample the data bus on the rising edge of the PCCK input clock
 * - Sample data regardless of the levels of PCEN1 and PCEN2
 * - 8-bit data width in the Rececive Holding Register
 *
 * \param[in] config  Pointer to configuration struct to initialize to
 *                     default values
 */
static inline void parc_get_config_defaults(struct parc_config *const config)
{
	config->capture_mode = PARC_BOTH_CAPTURE;
	config->sampling_edge = PARC_RISING_EDGE;
	config->smode = PARC_SMODE_ALWAYS;
	config->dsize = PARC_DSIZE_BYTE;
}

/**
 * \brief Enables the PARC module
 *
 * Enables an PARC module that has previously been configured.
 *
 * \param[in] module_inst  Pointer to the PARC software instance struct
 *
 * \return Status of the enable procedure
 * \retval STATUS_OK       The enable procedure was successful
 */
static inline enum status_code parc_enable(
		struct parc_module *const module_inst)
{
	Assert(module_inst);
	Assert(module_inst->hw);

	Parc *const parc_module_hw = module_inst->hw;
	parc_module_hw->PARC_CR = PARC_CR_EN;

#if PARC_CALLBACK_MODE == true
	/* enable interrupt*/
	NVIC_EnableIRQ(PARC_IRQn);
#endif
	return STATUS_OK;
}

/**
 * \brief Disables the PARC module
 *
 * Disables an PARC module that has previously been configured.
 *
 * \param[in] module_inst  Pointer to the PARC software instance struct
 *
 * \return Status of the enable procedure
 * \retval STATUS_OK       The disable procedure was successful
 */
static inline enum status_code parc_disable(
		struct parc_module *const module_inst)
{
	Assert(module_inst);
	Assert(module_inst->hw);

	Parc *const parc_module_hw = module_inst->hw;
	parc_module_hw->PARC_CR = PARC_CR_DIS;

#if PARC_CALLBACK_MODE == true
	/* disable interrupt*/
	NVIC_DisableIRQ(PARC_IRQn);
#endif
	return STATUS_OK;
}

/**
 * \brief Reads the PARC result
 *
 * Reads the result from an PARC conversion that was previously started.
 *
 * \param[in]  module_inst  Pointer to the PARC software instance struct
 * \param[out] data         Pointer to store the captured data in. Captured
 *                          data size is configured through parc_config.
 *
 * \return Status of the PARC read request.
 * \retval STATUS_OK    The result was retrieved successfully
 * \retval STATUS_BUSY  A captured data was not ready
 */
static inline enum status_code parc_read(struct parc_module *const module_inst,
		const uint32_t *data)
{
	Assert(module_inst);
	Assert(module_inst->hw);
	Assert(data);

	if(!(parc_get_status(module_inst) & PARC_SR_DRDY)){
		/* Captured data not ready*/
		return STATUS_BUSY.
	}

	Parc *const parc_module_hw = module_inst->hw;
	*data = parc_module_hw->PARC_RHR;

	parc_clear_status(module_inst, PARC_STATUS_DRDY);
	return STATUS_OK;
}

/**
 * \brief Start an PARC conversion
 *
 * Start PARC capture.
 *
 * \param[in] module_inst  Pointer to the PARC software instance struct
 */
static inline void parc_start_capture(
		struct parc_module *const module_inst)
{
	Assert(module_inst);
	Assert(module_inst->hw);

	Parc *const parc_module_hw = module_inst->hw;

	parc_module_hw->CR = PARC_CR_START;
}

/**
 * \brief Stop an PARC conversion
 *
 * Stop PARC capture.
 *
 * \param[in] module_inst  Pointer to the PARC software instance struct
 */
static inline void parc_stop_capture(
		struct parc_module *const module_inst)
{
	Assert(module_inst);
	Assert(module_inst->hw);

	Parc *const parc_module_hw = module_inst->hw;

	parc_module_hw->PARC_CR = PARC_CR_STOP;
}

/**
 * \brief Retrieves the current module status.
 *
 * Retrieves the status of the module, giving overall state information.
 *
 * \param[in] module_inst  Pointer to the PARC software instance struct
 *
 * \return Bitmask of PARC_STATUS_* flags in parc_status enum
 *
 * \retval PARC_STATUS_EN           PARC is enabled
 * \retval PARC_STATUS_CS           PARC capture status
 * \retval PARC_STATUS_DRDY         PARC data ready
 * \retval PARC_STATUS_OVR          PARC overrun
 */
static inline uint32_t parc_get_status(struct parc_module *const module_inst)
{
	Parc *const parc_module_hw = module_inst->hw;
	return parc_module_hw->PARC_SR;
}

/**
 * \brief Check the data ready status of PARC
 *
 * \param[in] module_inst Pointer to the PARC software instance struct
 *
 * \return 1 if captured data is ready, else it will return 0.
 */
static inline bool parc_is_data_ready(struct parc_module *const module_inst)
{
	Parc *const parc_module_hw = module_inst->hw;
	return (parc_module_hw->PARC_SR & PARC_SR_DRDY) != 0);
}

/**
 * \brief Clear status flag
 *
 * Clear the given interrupt-related status flag from the PARC module.
 *
 * \param[in] module_inst Pointer to the PARC software instance struct
 * \param[in] status      Enum to interrupt-related PARC status
 */
static inline enum status_code parc_clear_status(
	struct parc_module *const module_inst, const uint32_t status)
{
	/* Sanity check arguments*/
	Assert(module_inst);
	Assert(module_inst->hw);

	Parc *const parc_module_hw = module_inst->hw;

	uint32_t int_flags = 0;

	/*check for PARC data ready*/
	if(status & PARC_STATUS_DRDY)
	{
		int_flags |= PARC_ICR_DRDY;
	}
	/*check for PARC overrun*/
	if(status & PARC_STATUS_OVR)
	{
		int_flags |= PARC_ICR_OVR;
	}

	/*clear interrupt flags*/
	parc_module_hw->ICR = int_flags;
}

#if PARC_CALLBACK_MODE == true
/**
 * \brief Enable interrupt
 *
 * Enable the given interrupt request from the PARC module.
 *
 * \param[in] module_inst Pointer to the PARC software instance struct
 * \param[in] source Interrupt source to enable
 */
static inline void parc_enable_interrupts(struct parc_module *const module,
	enum parc_interrupt_source source)
{
	/* Sanity check arguments*/
	Assert(module_inst);
	Assert(module_inst->hw);

	Parc *const parc_module_hw = module_inst->hw;

	parc_module_hw->IER = source;
}
/**
 * \brief Disable interrupt
 *
 * Disable the given interrupt request from the PARC module.
 *
 * \param[in] module_inst Pointer to the PARC software instance struct
 * \param[in] source Interrupt source to disable
 */
static inline void parc_disable_interrupts(struct parc_module *const module,
	enum parc_interrupt_source source)
{
	/* Sanity check arguments*/
	Assert(module_inst);
	Assert(module_inst->hw);

	Parc *const parc_module_hw = module_inst->hw;

	parc_module_hw->IDR = source;
}

/**
 * \brief This function return the PARC interrupts mask value.
 *
 * \param module_inst      Pointer to the PARC software instance struct
 *
 * \return Interrupt mask value
 */
static inline uint32_t parc_read_interrupt_mask(
	struct parc_module *const module_inst)
{
	Parc *const parc_module_hw = module_inst->hw;
	return parc_module_hw->PARC_IMR;
}
#endif

/**
 * \brief Enables the PARC events mode
 *
 * Enables event mode on the PARC which has previously been configured.
 *
 * \param[in] module_inst  Pointer to the PARC software instance struct
 *
 * \return Status of the enable procedure
 * \retval STATUS_OK       The enable procedure was successful
 */
static inline enum status_code parc_enable_events(
	struct parc_module *const module_inst)
{
	Parc *const parc_module_hw = module_inst->hw;
	parc_module_hw->PARC_CFG |= PARC_CFG_EMODE;

	return STATUS_OK;
}

/**
 * \brief Disables the PARC events mode
 *
 * Disables event mode on the PARC which has previously been configured.
 *
 * \param[in] module_inst  Pointer to the PARC software instance struct
 *
 * \return Status of the enable procedure
 * \retval STATUS_OK       The enable procedure was successful
 */
static inline enum status_code parc_disable_events(
	struct parc_module *const module_inst)
{
	Parc *const parc_module_hw = module_inst->hw;
	parc_module_hw->PARC_CFG &= ~(PARC_CFG_EMODE);

	return STATUS_OK;
}

/**
 * \brief This function return the PARC module version.
 *
 * \param module_inst        Pointer to the PARC software instance struct
 *
 * \return PARC module version
 */
static inline uint32_t parc_get_version(struct parc_module *const module_inst)
{
	Parc *const parc_module_hw = module_inst->hw;
	return parc_module_hw->PARC_VERSION;
}

/**
 * \}
 */

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \page sam_parc_quick_start Quick Start Guide for the parc driver
 *
 */

#endif  /* PARC_H_INCLUDED */
