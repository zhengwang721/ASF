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
#include "status_codes.h"
#include "sysclk.h"
#include "pmc.h"

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

afec_callback_t afec_callback_pointer[2][23];

uint32_t g_interrupt_source[2][23];

/**
 * \internal
 * \brief Get AFEC instance number.
 *
 * \param afec  Base address of the AFEC
 *
 * \return   AFEC instance number
 */
static uint32_t afec_find_inst_num(Afec *const afec)
{
#if defined(AFEC1)
	if (afec == AFEC1) {
		return 1;
	}
#endif
#if defined(AFEC0)
	if (afec == AFEC0) {
		return 0;
	}
#endif
	return 0;
}

/**
 * \internal
 * \brief Get AFEC Peripheral ID.
 *
 * \param afec  Base address of the AFEC
 *
 * \return   AFEC Peripheral ID
 */
static uint32_t afec_find_pid(Afec *const afec)
{
#if defined(ID_AFEC1)
	if (afec == AFEC1) {
		return ID_AFEC1;
	}
#endif
#if defined(ID_AFEC0)
	if (afec == AFEC0) {
		return ID_AFEC0;
	}
#endif
	return ID_AFEC0;
}

/**
 * \internal
 * \brief Configure the AFEC Module.
 *
 * \param afec  Base address of the AFEC
 * \param config   Configuration for the AFEC
 */
static void afec_set_config(Afec *const afec, struct afec_config *config)
{
	uint32_t reg = 0;

	reg = (config->anach? AFE_MR_ANACH_ALLOWED : 0) |
			(config->useq ? AFE_MR_USEQ_REG_ORDER : 0) |
			AFE_MR_PRESCAL(config->mck / (2 * config->afec_clock) - 1) |
			AFE_MR_TRACKTIM(config->tracktim) |
			AFE_MR_TRANSFER(config->transfer) |
			(config->settling_time)|
			(config->startup_time);

	afec->AFE_MR = reg;

	afec->AFE_EMR = (config->tag ? AFE_EMR_TAG : 0) |
			(config->resolution) |
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
void afec_ch_set_config(Afec *const afec, const enum afec_channel_num channel,
		struct afec_ch_config *config)
{
	if(afec == AFEC0) {
		Assert(channel < 16);
	} else if(afec == AFEC1)	{
		Assert(channel < 8);
	}

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
void afec_temp_sensor_set_config(Afec *const afec, struct afec_temp_sensor_config *config)
{
	Assert(afec == AFEC0);

	uint32_t reg = 0;

	reg = ((config->rctc) ? AFE_TEMPMR_RTCT : 0) | (config->mode);
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
 * - 12 -bit resolution
 * - AFEC clock frequency is 6MHz
 * - Start Up Time is 64 periods AFEClock
 * - Analog Settling Time is 3 periods of AFEClock
 * - Tracking Time is 3 periods of AFEClock
 * - Transfer Period is 5 periods AFEClock
 * - Allows different analog settings for each channel
 * - The controller converts channels in a simple numeric order
 * - Appends the channel number to the conversion result in AFE_LDCR register
 * - Only a Single Trigger is required to get an averaged value
 * - AFE Bias Current Control value is 1
 *
 * \param cfg Pointer to configuration structure to be initiated.
 */
void afec_get_config_defaults(struct afec_config *const cfg)
{
	/*Sanity check argument. */
	Assert(cfg);

	cfg->resolution = AFEC_12_BITS;
	cfg->mck = sysclk_get_cpu_hz();
	cfg->afec_clock = 6000000;
	cfg->startup_time = AFEC_STARTUP_TIME_4;
	cfg->settling_time = AFEC_SETTLING_TIME_0;
	cfg->tracktim = 2;
	cfg->transfer = 1;
	cfg->anach = true;
	cfg->useq = false;
	cfg->tag = true;
	cfg->stm = true;
	cfg->ibctl = 1;
}

/**
 * \brief Get the AFEC channel default configurations.
 *
 * Use to initialize the configuration structure to known default values.
 *
 * The default configuration is as follows:
 * - No Offset
 * - Single Ended Mode
 * - Gain value is 1
 *
 * \param cfg Pointer to channel configuration structure to be initiated.
 */
void afec_ch_get_config_defaults(struct afec_ch_config *const cfg)
{
	/*Sanity check argument. */
	Assert(cfg);

	cfg->diff = false;
	cfg->offset = false;
	cfg->gain = AFEC_GAINVALUE_0;
}

/**
 * \brief Get the AFEC Temperature Sensor default configurations.
 *
 * Use to initialize the configuration structure to known default values.
 *
 * The default configuration is as follows:
 * - The temperature sensor measure is not triggered by RTC event
 * - Generates an event when the converted data is in the comparison window
 * - The window range is 0xFF ~ 0xFFF
 *
 * \param cfg Pointer to temperature sensor configuration structure to be initiated.
 */
void afec_temp_sensor_get_config_defaults(struct afec_temp_sensor_config *const cfg)
{
	/*Sanity check argument. */
	Assert(cfg);

	cfg->rctc = false;
	cfg->mode= AFEC_TEMP_CMP_MODE_2;
	cfg->low_threshold= 0xFF;
	cfg->high_threshold= 0xFFF;
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

	if ((afec_get_interrupt_status(afec) & AFE_ISR_DRDY) == AFE_ISR_DRDY) {
		return STATUS_ERR_BUSY;
	}

	/* Reset the AFEC module */
	afec->AFE_CR = AFE_CR_SWRST;
	afec_set_config(afec, config);

	return STATUS_OK;
}

/**
 * \brief Set callback for AFEC
 *
 * \param afec  Base address of the AFEC
 * \param source    Interrupt source
 * \param src_num Interrupt source number
 * \param callback  Callback function pointer
 * \param irq_level Interrupt level
 */
void afec_set_callback(Afec *const afec, enum afec_interrupt_source source,
		uint8_t src_num, afec_callback_t callback, uint8_t irq_level)
{
	Assert(afec);
	Assert(callback);

	uint32_t i = afec_find_inst_num(afec);
	afec_callback_pointer[i][src_num] = callback;
	if (!i) {
		irq_register_handler(AFEC0_IRQn, irq_level);
	} else if (i == 1) {
		irq_register_handler(AFEC1_IRQn, irq_level);
	}
	/* Enable the specified interrupt source */
	afec_enable_interrupt(afec, source);

	g_interrupt_source[i][src_num] = source;
}

/**
 * \internal
 * \brief Common AFEC interrupt handler
 *
 * The optional callback used by the interrupt handler is set by the
 * afec_set_callback() function.
 *
 * \param inst_num AFEC instance number to handle interrupt for
 * \param src_num Interrupt source number
 */
static void afec_interrupt(uint8_t inst_num, uint8_t src_num)
{
	if (afec_callback_pointer[inst_num][src_num]) {
		afec_callback_pointer[inst_num][src_num]();
	} else {
		Assert(false); /* Catch unexpected interrupt */
	}
}

/**
 * \brief Interrupt handler for AFEC0.
 */
void AFEC0_Handler(void)
{
	volatile uint32_t status;

	status = afec_get_interrupt_status(AFEC0);

	if ((status & AFE_ISR_EOC0) == AFE_ISR_EOC0) {
		if(g_interrupt_source[0][0]) {
			afec_interrupt(0, 0);
		}
	}
	if ((status & AFE_ISR_EOC1) == AFE_ISR_EOC1) {
		if(g_interrupt_source[0][1]) {
			afec_interrupt(0, 1);
		}
	}
	if ((status & AFE_ISR_EOC2) == AFE_ISR_EOC2) {
		if(g_interrupt_source[0][2]) {
			afec_interrupt(0, 2);
		}
	}
	if ((status & AFE_ISR_EOC3) == AFE_ISR_EOC3) {
		if(g_interrupt_source[0][3]) {
			afec_interrupt(0, 3);
		}
	}
	if ((status & AFE_ISR_EOC4) == AFE_ISR_EOC4) {
		if(g_interrupt_source[0][4]) {
			afec_interrupt(0, 4);
		}
	}
	if ((status & AFE_ISR_EOC5) == AFE_ISR_EOC5) {
		if(g_interrupt_source[0][5]) {
			afec_interrupt(0, 5);
		}
	}
	if ((status & AFE_ISR_EOC6) == AFE_ISR_EOC6) {
		if(g_interrupt_source[0][6]) {
			afec_interrupt(0, 6);
		}
	}
	if ((status & AFE_ISR_EOC7) == AFE_ISR_EOC7) {
		if(g_interrupt_source[0][7]) {
			afec_interrupt(0, 7);
		}
	}
	if ((status & AFE_ISR_EOC8) == AFE_ISR_EOC8) {
		if(g_interrupt_source[0][8]) {
			afec_interrupt(0, 8);
		}
	}
	if ((status & AFE_ISR_EOC9) == AFE_ISR_EOC9) {
		if(g_interrupt_source[0][9]) {
			afec_interrupt(0, 9);
		}
	}
	if ((status & AFE_ISR_EOC10) == AFE_ISR_EOC10) {
		if(g_interrupt_source[0][10]) {
			afec_interrupt(0, 10);
		}
	}
	if ((status & AFE_ISR_EOC11) == AFE_ISR_EOC11) {
		if(g_interrupt_source[0][11]) {
			afec_interrupt(0, 11);
		}
	}
	if ((status & AFE_ISR_EOC12) == AFE_ISR_EOC12) {
		if(g_interrupt_source[0][12]) {
			afec_interrupt(0, 12);
		}
	}
	if ((status & AFE_ISR_EOC13) == AFE_ISR_EOC13) {
		if(g_interrupt_source[0][13]) {
			afec_interrupt(0, 13);
		}
	}
	if ((status & AFE_ISR_EOC14) == AFE_ISR_EOC14) {
		if(g_interrupt_source[0][14]) {
			afec_interrupt(0, 14);
		}
	}
	if ((status & AFE_ISR_EOC15) == AFE_ISR_EOC15) {
		if(g_interrupt_source[0][15]) {
			afec_interrupt(0, 15);
		}
	}
	if ((status & AFE_ISR_DRDY) == (AFE_ISR_DRDY)) {
		if(g_interrupt_source[0][16]) {
			afec_interrupt(0, 16);
		}
	}
	if ((status & AFE_ISR_GOVRE) == AFE_ISR_GOVRE) {
		if(g_interrupt_source[0][17]) {
			afec_interrupt(0, 17);
		}
	}
	if ((status & AFE_ISR_COMPE) == AFE_ISR_COMPE) {
		if(g_interrupt_source[0][18]) {
			afec_interrupt(0, 18);
		}
	}
	if ((status & AFE_ISR_ENDRX) == AFE_ISR_ENDRX) {
		if(g_interrupt_source[0][19]) {
			afec_interrupt(0, 19);
		}
	}
	if ((status & AFE_ISR_RXBUFF) == AFE_ISR_RXBUFF) {
		if(g_interrupt_source[0][20]) {
			afec_interrupt(0, 20);
		}
	}
	if ((status & AFE_ISR_TEMPCHG) == AFE_ISR_TEMPCHG) {
		if(g_interrupt_source[0][21]) {
			afec_interrupt(0, 21);
		}
	}
	if ((status & AFE_ISR_EOCAL) == AFE_ISR_EOCAL) {
		if(g_interrupt_source[0][22]) {
			afec_interrupt(0, 22);
		}
	}
}

/**
 * \brief Interrupt handler for AFEC1.
 */
void AFEC1_Handler(void)
{
	volatile uint32_t status;

	status = afec_get_interrupt_status(AFEC1);

	if ((status & AFE_ISR_EOC0) == AFE_ISR_EOC0) {
		if(g_interrupt_source[1][0]) {
			afec_interrupt(1, 0);
		}
	}
	if ((status & AFE_ISR_EOC1) == AFE_ISR_EOC1) {
		if(g_interrupt_source[1][1]) {
			afec_interrupt(1, 1);
		}
	}
	if ((status & AFE_ISR_EOC2) == AFE_ISR_EOC2) {
		if(g_interrupt_source[1][2]) {
			afec_interrupt(1, 2);
		}
	}
	if ((status & AFE_ISR_EOC3) == AFE_ISR_EOC3) {
		if(g_interrupt_source[1][3]) {
			afec_interrupt(1, 3);
		}
	}
	if ((status & AFE_ISR_EOC4) == AFE_ISR_EOC4) {
		if(g_interrupt_source[1][4]) {
			afec_interrupt(1, 4);
		}
	}
	if ((status & AFE_ISR_EOC5) == AFE_ISR_EOC5) {
		if(g_interrupt_source[1][5]) {
			afec_interrupt(1, 5);
		}
	}
	if ((status & AFE_ISR_EOC6) == AFE_ISR_EOC6) {
		if(g_interrupt_source[1][6]) {
			afec_interrupt(1, 6);
		}
	}
	if ((status & AFE_ISR_EOC7) == AFE_ISR_EOC7) {
		if(g_interrupt_source[1][7]) {
			afec_interrupt(1, 7);
		}
	}
	if ((status & AFE_ISR_DRDY) == (AFE_ISR_DRDY)) {
		if(g_interrupt_source[1][16]) {
			afec_interrupt(1, 16);
		}
	}
	if ((status & AFE_ISR_GOVRE) == AFE_ISR_GOVRE) {
		if(g_interrupt_source[1][17]) {
			afec_interrupt(1, 17);
		}
	}
	if ((status & AFE_ISR_COMPE) == AFE_ISR_COMPE) {
		if(g_interrupt_source[1][18]) {
			afec_interrupt(1, 18);
		}
	}
	if ((status & AFE_ISR_ENDRX) == AFE_ISR_ENDRX) {
		if(g_interrupt_source[1][19]) {
			afec_interrupt(1, 19);
		}
	}
	if ((status & AFE_ISR_RXBUFF) == AFE_ISR_RXBUFF) {
		if(g_interrupt_source[1][20]) {
			afec_interrupt(1, 20);
		}
	}
	if ((status & AFE_ISR_TEMPCHG) == AFE_ISR_TEMPCHG) {
		if(g_interrupt_source[1][21]) {
			afec_interrupt(1, 21);
		}
	}
	if ((status & AFE_ISR_EOCAL) == AFE_ISR_EOCAL) {
		if(g_interrupt_source[1][22]) {
			afec_interrupt(1, 22);
		}
	}
}

/**
 * \brief Enable AFEC Module.
 *
 * \param afec  Base address of the AFEC
 */
void afec_enable(Afec *const afec)
{
	Assert(afec);
	uint32_t pid;

	pid = afec_find_pid(afec);
	/* Enable peripheral clock. */
	pmc_enable_periph_clk(pid);
	sleepmgr_lock_mode(SLEEPMGR_SLEEP_WFI);
}

/**
 * \brief Disable AFEC Module.
 *
 * \param afec  Base address of the AFEC
 */
void afec_disable(Afec *const afec)
{
	Assert(afec);
	uint32_t pid;

	pid = afec_find_pid(afec);
	/* Enable peripheral clock. */
	pmc_disable_periph_clk(pid);
	sleepmgr_unlock_mode(SLEEPMGR_SLEEP_WFI);
}

/**
 * \brief Configure conversion sequence.
 *
 * \param afec  Base address of the AFEC.
 * \param ch_list Channel sequence list.
 * \param uc_num Number of channels in the list.
 */
void afec_configure_sequence(Afec *const afec, const enum afec_channel_num ch_list[],
		uint8_t uc_num)
{
	uint8_t uc_counter;

	/* Set user sequence mode */
	afec->AFE_MR |= AFE_MR_USEQ_REG_ORDER;
	afec->AFE_SEQ1R = 0;
	afec->AFE_SEQ2R = 0;

	if (uc_num < 8) {
		for (uc_counter = 0; uc_counter < uc_num; uc_counter++) {
			afec->AFE_SEQ1R |=
					ch_list[uc_counter] << (4 * uc_counter);
		}
	} else {
		for (uc_counter = 0; uc_counter < 8; uc_counter++) {
			afec->AFE_SEQ1R |=
					ch_list[uc_counter] << (4 * uc_counter);
		}
		for (uc_counter = 0; uc_counter < uc_num - 8; uc_counter++) {
			afec->AFE_SEQ2R |=
					ch_list[uc_counter] << (4 * uc_counter);
		}
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

