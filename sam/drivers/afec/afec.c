/**
 * \file
 *
 * \brief Analog-Front-End Controller driver for SAM.
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

#include "afec.h"
#include "sleepmgr.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \defgroup sam_drivers_afec_group Analog-Front-End Controller
 *
 * See \ref sam_afec_quickstart.
 *
 * Driver for the Analog-Front-End Controller. This driver provides access to the main 
 * features of the AFEC controller.
 *
 * @{
 */

afec_callback_t afec_callback_pointer[2];
struct afec_dev_inst *afec_instances[2];

/**
 * \internal
 * \brief Get AFEC instance number.
 *
 * \param afec  Base address of the AFEC
 *
 * \return      AFEC instancel number
 */
static uint32_t afec_find_ch_num(Afec *const afec)
{
#if defined(ID_AFEC1)
	if (afec == AFEC1) {
		return 1;
	}
#endif
#if defined(ID_AFEC0)
	if (afec == AFEC0) {
		return 0;
	}
#endif

	return 0;
}

/**
 * \internal
 * \brief Configure the AFEC Module.
 *
 * \param afec  Base address of the AFEC
 * \param config   Configuration for the AFEC
 */
static void afec_set_config(Afec *afec, struct afec_config *config)
{
	uint32_t reg = 0;
	
	reg = (config->anach? AFE_MR_ANACH_ALLOWED : 0) |
			(config->useq ? AFE_MR_USEQ_REG_ORDER : 0) |
			AFE_MR_PRESCAL(config->mck / (2 * config->afec_clock) - 1) |
			AFE_MR_TRACKTIM(config->tracktim) |
			AFE_MR_TRANSFER(config->transfer) |
			(config->resolution) | 
			(config->settling_time)|
			(config->startup_time);
	
	afec->AFE_MR = reg;
	
	afec->AFE_EMR = (config->tag ? AFE_EMR_TAG : 0) |
			(config->stm ? AFE_EMR_STM : 0);

	afec->AFE_ACR = AFE_ACR_IBCTL(config->ibctl);
}

/**
 * \brief Configure the AFEC channel.
 *
 * \param afec  Base address of the AFEC
 * \param channel The channel number
 * \param config   Configuration for the AFEC channel
 */
void afec_ch_set_config(Afec *afec, const enum afec_channel_num channel,
		struct afec_ch_config *config)
{
	afec->AFE_CDOR = (config->offset) ? (0x1u << channel) : 0;
	afec->AFE_DIFFR = (config->diff) ? (0x1u << channel) : 0;
	afec->AFE_CGR = (0x03u << (2 * channel)) & ((config->gain) << (2 * channel));
}

/**
 * \brief Configure the AFEC temperature sensor.
 *
 * \param afec  Base address of the AFEC
 * \param config   Configuration for the AFEC temperature sensor
 */
void afec_temp_sensor_set_config(Afec *afec, struct afec_temp_sensor_config *config)
{
	uint32_t reg = 0;

	reg = (config->rctc) ? AFE_TEMPMR_RTCT : 0 | (config->mode);
	afec->AFE_TEMPMR = reg;
	
	afec->AFE_TEMPCWR = AFE_TEMPCWR_TLOWTHRES(config->low_threshold) |
			AFE_TEMPCWR_THIGHTHRES(config->high_threshold);
}

/**
 * \brief Get the AFEC default configurations.
 *
 * Use to initialize the configuration structure to known default values. This
 * function should be called at the start of any AFEC initiation.
 *
 * The default configuration is as follows:
 * - 7-bit addressing
 * - Self address is 0x50.
 * - Normal mode
 * - Do not stretch clock on data byte reception
 * - Do not stretch clock on address match
 * - Stretch clock if RHR is full or THR is empty
 * - Do not acknowledge the general call address
 * - Acknowledge the specified slave address
 * - Disable packet error checking
 * - Do not acknowledge the SMBus host header
 * - Do not acknowledge the SMBus default address
 * - 0 data setup cycles in F/S mode and high speed mode
 * - Zero-initialization for slew rate setting in F/S mode and high speed mode
 * - Clock Prescaler is 0
 * - 0 SMBus TIMEOUT cycle
 * - 0 SMBus Low:Sext cycle
 *
 * \param cfg Pointer to configuration structure to be initiated.
 */
void afec_get_config_defaults(struct afec_config *const cfg)
{
	/*Sanity check argument. */
	Assert(cfg);

	cfg->ten_bit = false;
	cfg->chip = 0x50;
	cfg->smbus = false;
	cfg->stretch_clk_data = false;
	cfg->stretch_clk_addr = false;
	cfg->stretch_clk_hr = true;
	cfg->ack_general_call = false;
	cfg->ack_slave_addr = true;
	cfg->enable_pec = false;
	cfg->ack_smbus_host_header = false;
	cfg->ack_smbus_default_addr = false;
	cfg->sudat = 0;
	cfg->fs_filter = 0;
	cfg->fs_daslew = 0;
	cfg->fs_dadrivel = 0;
	cfg->hddat = 0;
	cfg->hs_filter = 0;
	cfg->hs_daslew = 0;
	cfg->hs_dadrivel = 0;
	cfg->exp = 0;
	cfg->ttouts = 0;
	cfg->tlows = 0;
}

/**
 * \brief Initialize the AFEC Module.
 *
 * \param afec  Base address of the AFEC
 * \param config   Configuration for the AFEC
 *
 * \return Status of module initialization.
 */
enum status_code afec_init(Afec *const afec, struct afec_config *config)
{
	Assert(afec);
	Assert(config);

	/* Reset the TWIS module */
	afec->AFE_CR = AFE_CR_SWRST;
	afec_set_config(afec, config);

	return STATUS_OK;
}

/**
 * \brief Set callback for AFEC
 *
 * \param afec  Base address of the AFEC
 * \param source    Interrupt source
 * \param callback  Callback function pointer
 * \param irq_level Interrupt level
 */
void twis_set_callback(Afec *afec, afec_interrupt_source_t source,
		afec_callback_t callback, uint8_t irq_level)
{
	Assert(afec);
	Assert(callback);

	uint32_t i = afec_find_ch_num(afec);
	afec_callback_pointer[i] = callback;
	if (!i) {
		irq_register_handler(AFEC0_IRQn, irq_level);
	} else if (i == 1) {
		irq_register_handler(AFEC1_IRQn, irq_level);
	}
	/* Enable the specified interrupt source */
	afec_enable_interrupt(afec, source);
}

/**
 * \brief Enable AFEC Module.
 *
 * \param afec  Base address of the AFEC
 */
void twis_enable(Afec *afec)
{
	Assert(afec);

	sleepmgr_lock_mode(SLEEPMGR_SLEEP_1);
	sysclk_enable_peripheral_clock(afec);
}

/**
 * \brief Disable AFEC Module.
 *
 * \param afec  Base address of the AFEC
 */
void twis_disable(Afec *afec)
{
	Assert(afec);

	sysclk_disable_peripheral_clock(afec);
	sleepmgr_unlock_mode(SLEEPMGR_SLEEP_1);
}

//@}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

