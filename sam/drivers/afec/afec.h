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

#ifndef AFEC_H_INCLUDED
#define AFEC_H_INCLUDED

#include "compiler.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/* Definitions for AFEC resolution */
enum afec_resolution {
	AFEC_10_BITS = AFE_EMR_RES_LOW_RES,        /* AFEC 10-bit resolution */
	AFEC_12_BITS = AFE_EMR_RES_NO_AVERAGE,     /* AFEC 12-bit resolution */
	AFEC_13_BITS = AFE_EMR_RES_OSR4,           /* AFEC 13-bit resolution */
	AFEC_14_BITS = AFE_EMR_RES_OSR16,          /* AFEC 14-bit resolution */
	AFEC_15_BITS = AFE_EMR_RES_OSR64,          /* AFEC 15-bit resolution */
	AFEC_16_BITS = AFE_EMR_RES_OSR256         /* AFEC 16-bit resolution */
};

/* Definitions for AFEC resolution */
enum afec_power_mode {
	/* AFEC core on and reference voltage circuitry on */
	AFEC_POWER_MODE_0 = 0,
	/* AFEC core off and reference voltage circuitry on */
	AFEC_POWER_MODE_1, 
	/* AFEC core off and reference voltage circuitry off */
	AFEC_POWER_MODE_2        
};

/* Definitions for AFEC trigger */
enum afec_trigger {
	/* Starting a conversion is only possible by software. */
	AFEC_TRIG_SW = AFE_MR_TRGEN_DIS,
	/* External trigger */
	AFEC_TRIG_EXT = AFE_MR_TRGSEL_AFE_TRIG0 | AFE_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 0 */
	AFEC_TRIG_TIO_CH_0 = AFE_MR_TRGSEL_AFE_TRIG1 | AFE_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 1 */
	AFEC_TRIG_TIO_CH_1 = AFE_MR_TRGSEL_AFE_TRIG2 | AFE_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 2 */
	AFEC_TRIG_TIO_CH_2 = AFE_MR_TRGSEL_AFE_TRIG3 | AFE_MR_TRGEN,
	/* PWM Event Line 0 */
	AFEC_TRIG_PWM_EVENT_LINE_0 = AFE_MR_TRGSEL_AFE_TRIG4 | AFE_MR_TRGEN,
	/* PWM Event Line 1 */
	AFEC_TRIG_PWM_EVENT_LINE_1 = AFE_MR_TRGSEL_AFE_TRIG5 | AFE_MR_TRGEN,
	/* Freerun mode conversion. */
	AFEC_TRIG_FREERUN
} ;

/* Definitions for AFEC channel number */
enum afec_channel_num {
	AFEC_CHANNEL_0  = 0,
	AFEC_CHANNEL_1  = 1,
	AFEC_CHANNEL_2  = 2,
	AFEC_CHANNEL_3  = 3,
	AFEC_CHANNEL_4  = 4,
	AFEC_CHANNEL_5  = 5,
	AFEC_CHANNEL_6  = 6,
	AFEC_CHANNEL_7  = 7,
	AFEC_CHANNEL_8  = 8,
	AFEC_CHANNEL_9  = 9,
	AFEC_CHANNEL_10 = 10,
	AFEC_CHANNEL_11 = 11,
	AFEC_CHANNEL_12 = 12,
	AFEC_CHANNEL_13 = 13,
	AFEC_CHANNEL_14 = 14,
	AFEC_TEMPERATURE_SENSOR = 15,
	AFEC_CHANNEL_16  = 16,
	AFEC_CHANNEL_17  = 17,
	AFEC_CHANNEL_18 = 18,
	AFEC_CHANNEL_19 = 19,
	AFEC_CHANNEL_20 = 20,
	AFEC_CHANNEL_21 = 21,
	AFEC_CHANNEL_22 = 22,
	AFEC_CHANNEL_23 = 23
} ;

/* Definitions for AFEC gain value */
enum afec_gainvalue {
	AFEC_GAINVALUE_0 = 0,
	AFEC_GAINVALUE_1 = 1,
	AFEC_GAINVALUE_2 = 2,
	AFEC_GAINVALUE_3 = 3
};

/* Definitions for AFEC Start Up Time */
enum afec_startup_time {
	AFEC_STARTUP_TIME_0 = AFE_MR_STARTUP_SUT0,
	AFEC_STARTUP_TIME_1 = AFE_MR_STARTUP_SUT8,
	AFEC_STARTUP_TIME_2 = AFE_MR_STARTUP_SUT16,
	AFEC_STARTUP_TIME_3 = AFE_MR_STARTUP_SUT24,
	AFEC_STARTUP_TIME_4 = AFE_MR_STARTUP_SUT64,
	AFEC_STARTUP_TIME_5 = AFE_MR_STARTUP_SUT80,
	AFEC_STARTUP_TIME_6 = AFE_MR_STARTUP_SUT96,
	AFEC_STARTUP_TIME_7 = AFE_MR_STARTUP_SUT112,
	AFEC_STARTUP_TIME_8 = AFE_MR_STARTUP_SUT512,
	AFEC_STARTUP_TIME_9 = AFE_MR_STARTUP_SUT576,
	AFEC_STARTUP_TIME_10 = AFE_MR_STARTUP_SUT640,
	AFEC_STARTUP_TIME_11 = AFE_MR_STARTUP_SUT704,
	AFEC_STARTUP_TIME_12 = AFE_MR_STARTUP_SUT768,
	AFEC_STARTUP_TIME_13 = AFE_MR_STARTUP_SUT832,
	AFEC_STARTUP_TIME_14 = AFE_MR_STARTUP_SUT896,
	AFEC_STARTUP_TIME_15 = AFE_MR_STARTUP_SUT960
};

/* Definitions for AFEC analog settling time */
enum afec_settling_time {
	AFEC_SETTLING_TIME_0 = AFE_MR_SETTLING_AST3,
	AFEC_SETTLING_TIME_1 = AFE_MR_SETTLING_AST5,
	AFEC_SETTLING_TIME_2 = AFE_MR_SETTLING_AST9,
	AFEC_SETTLING_TIME_3 = AFE_MR_SETTLING_AST17
};

/* Definitions for Comparison Mode */
enum afec_cmp_mode {
	AFEC_CMP_MODE_0 = AFE_EMR_CMPMODE_LOW,
	AFEC_CMP_MODE_1 = AFE_EMR_CMPMODE_HIGH,
	AFEC_CMP_MODE_2 = AFE_EMR_CMPMODE_IN,
	AFEC_CMP_MODE_3 = AFE_EMR_CMPMODE_OUT
};

/* Definitions for Temperature Comparison Mode */
enum afec_temp_cmp_mode {
	AFEC_TEMP_CMP_MODE_0 = AFE_TEMPMR_TEMPCMPMOD_LOW,
	AFEC_TEMP_CMP_MODE_1 = AFE_TEMPMR_TEMPCMPMOD_HIGH,
	AFEC_TEMP_CMP_MODE_2 = AFE_TEMPMR_TEMPCMPMOD_IN,
	AFEC_TEMP_CMP_MODE_3 = AFE_TEMPMR_TEMPCMPMOD_OUT
};

/**
 * \brief Analog-Front-End Controller configuration structure.
 *
 * Configuration structure for a Analog-Front-End Controller instance.
 * This structure could be initialized by the \ref afec_get_config_defaults()
 * function before being modified by the user application.
 */
struct afec_config {
	/** Sleep Mode */
	bool sleep;
	/** Fast Wake Up */
	/** Resolution */
	enum afec_resolution resolution;
	bool fwup;
	/** Free Run Mode */
	bool free_run;
	/** Master Clock */
	uint32_t mck;
	/** AFEC Clock */
	uint32_t afec_clock;
	/** Start Up Time */
	enum afec_startup_time startup_time;
	/** Analog Settling Time = (settling_time + 1) / AFEC clock */
	enum afec_settling_time settling_time;
	/** Tracking Time = tracktim / AFEC clock */
	uint8_t tracktim;
	/** Transfer Period = (transfer * 2 + 3) / AFEC clock */
	uint8_t transfer;
	/** Analog Change */
	bool anach;
	/** Use Sequence Enable */
	bool useq;
	/** TAG of AFE_LDCR register */
	bool tag;
	/** Single Trigger Mode */
	bool stm;
	/** AFE Bias Current Control */
	uint8_t ibctl;
};

/**
 * \brief Analog-Front-End Controller driver software instance structure.
 *
 * Device instance structure for a Analog-Front-End Controller driver instance. This
 * structure should be initialized by the \ref afec_init() function to
 * associate the instance with a particular hardware module of the device.
 */
struct afec_dev_inst {
	/** Base address of the AFEC module. */
	Afec *hw_dev;
	/** Pointer to AFEC configuration structure. */
	struct afec_config  *afec_cfg;
};

/** AFEC channel input configuration structure.*/
struct afec_input_config {
	bool diff;
	bool offset;
	enum afec_gainvalue gain;
};

/** AFEC Temperature Sensor configuration structure.*/
struct afec_temp_sensor_config {
	bool rctc;
	enum afec_temp_cmp_mode mode;
	uint16_t low_threshold;
	uint16_t high_threshold;
};

void afec_get_config_defaults(struct afec_config *const cfg);
void afec_temp_sensor_get_config_defaults(struct afec_temp_sensor_config *const cfg);
void afec_init(struct afec_dev_inst *const dev_inst, Afec *const afec,
		struct afec_config *const cfg);
void afec_configure_sequence(struct afec_dev_inst *const dev_inst,
		const enum adc_channel_num_t ch_list[], const uint8_t uc_num);
void afec_configure_channel_input(struct afec_dev_inst *const dev_inst,
		const enum afec_channel_num channel, struct afec_input_config *input_config);
void afec_configure_temp_sensor(struct afec_dev_inst *const dev_inst,
		afec_temp_sensor_config config);
void afec_enable(struct afec_dev_inst *const dev_inst);
void afec_disable(struct afec_dev_inst *const dev_inst);
void afec_set_callback(struct afec_dev_inst *const dev_inst,
		afec_interrupt_source_t source, afec_callback_t callback, uint8_t irq_level);

/**
 * \brief Configure conversion trigger and free run mode.
 *
 * \param dev_inst  Device structure pointer.
 * \param trigger Conversion trigger.
 *
 */
static inline void afec_set_trigger(struct afec_dev_inst *const dev_inst,
		const enum afec_trigger trigger)
{
	Afec *afec = dev_inst->hw_dev;
	
	if(trigger == AFEC_TRIG_FREERUN) {
		afec->AFE_MR |= AFE_MR_FREERUN_ON;
	} else {
		afec->AFE_MR &= ~AFE_MR_FREERUN_ON;
		afec->AFE_MR |= trigger;
	}
}

/**
 * \brief Configure comparison mode.
 *
 * \param dev_inst  Device structure pointer..
 * \param mode AFEC comparison mode.
 */
static inline void afec_set_comparison_mode(struct afec_dev_inst *const dev_inst, const enum afec_cmp_mode mode,
		const enum afec_channel_num channel, bool all_channel, uint8_t cmp_filter)
{
	dev_inst->afec->AFE_EMR = mode | (all_channel) ? AFE_EMR_CMPALL : AFE_EMR_CMPSEL(channel)
			| AFE_EMR_CMPFILTER(cmp_filter);
}

/**
 * \brief Get comparison mode.
 *
 * \param dev_inst  Device structure pointer..
 *
 * \retval Compare mode value.
 */
afec_cmp_mode afec_get_comparison_mode(struct afec_dev_inst *const dev_inst)
{
	Afec *afec = dev_inst->hw_dev;
	return afec->AFE_EMR & AFE_EMR_CMPMODE_Msk;
}

/**
 * \brief Configure AFEC compare window.
 *
 * \param dev_inst  Device structure pointer.
 * \param w_low_threshold Low threshold of compare window.
 * \param w_high_threshold High threshold of compare window.
 */
static inline void afec_set_comparison_window(struct afec_dev_inst *const dev_inst,
		const uint16_t us_low_threshold, const uint16_t us_high_threshold)
{
	Afec *afec = dev_inst->hw_dev;
	afec->AFE_CWR = AFE_CWR_LOWTHRES(us_low_threshold) |
			AFE_CWR_HIGHTHRES(us_high_threshold);
}

/**
 * \brief Enable or disable write protection of AFEC registers.
 *
 * \param dev_inst  Device structure pointer.
 * \param ul_enable 1 to enable, 0 to disable.
 */
static inline void afec_set_writeprotect(struct afec_dev_inst *const dev_inst,
		const uint32_t ul_enable)
{
	Afec *afec = dev_inst->hw_dev;
	afec->AFE_WPMR |= AFE_WPMR_WPKEY(ul_enable);
}

/**
 * \brief Indicate write protect status.
 *
 * \param dev_inst  Device structure pointer.
 *
 * \return 0 if the peripheral is not protected, or 16-bit write protect
 * violation Status.
 */
static inline uint32_t afec_get_writeprotect_status(struct afec_dev_inst *const dev_inst)
{
	Afec *afec = dev_inst->hw_dev;
	return afec->AFE_WPSR & AFE_WPSR_WPVS;
}

/**
 * \brief Get AFEC overrun error status.
 *
 * \param dev_inst  Device structure pointer.
 *
 * \return AFEC overrun error status.
 */
static inline uint32_t afec_get_overrun_status(struct afec_dev_inst *const dev_inst)
{
	Afec *afec = dev_inst->hw_dev;
	return afec->AFE_OVER;
}

/**
 * \brief Start analog-to-digital conversion.
 *
 * \note If one of the hardware event is selected as AFEC trigger,
 * this function can NOT start analog to digital conversion.
 *
 * \param dev_inst  Device structure pointer.
 */
static inline void afec_start_software_conversion(struct afec_dev_inst *const dev_inst)
{
	Afec *afec = dev_inst->hw_dev;
	afec->AFE_CR = AFE_CR_START;
}

/**
 * \brief Configures AFEC power mode.
 *
 * \param afec  Pointer to an AFEC instance.
 * \param sleep  AFE_MR_SLEEP_NORMAL keeps the AFEC Core and reference voltage 
 * circuitry ON between conversions.
 * AFE_MR_SLEEP_SLEEP keeps the AFEC Core and reference voltage circuitry OFF 
 * between conversions.
 * \param fwup  AFE_MR_FWUP_OFF configures sleep mode as sleep setting, 
 * AFE_MR_FWUP_ON keeps voltage reference ON and AFEC Core OFF between conversions.
 */
static inline void afec_configure_power_mode(Afec *afec, const enum afec_power_mode mode)
{
	switch(mode) {
		case AFEC_POWER_MODE_0:
			afec->AFE_MR &= ~ AFE_MR_SLEEP_SLEEP;
			break;
		case AFEC_POWER_MODE_1:
			afec->AFE_MR |= AFE_MR_SLEEP_SLEEP;
			afec->AFE_MR &= ~AFE_MR_FWUP_ON;
			break;
		case AFEC_POWER_MODE_2:
			afec->AFE_MR |= AFE_MR_SLEEP_SLEEP;
			afec->AFE_MR |= AFE_MR_FWUP_ON;
			break;	
	}
}

/**
 * \brief Enable the specified AFEC channel.
 *
 * \param dev_inst  Device structure pointer.
 * \param afec_ch AFEC channel number.
 */
static inline void afec_enable_one_channel(struct afec_dev_inst *const dev_inst,
		const enum afec_channel_num afec_ch)
{
	Afec *afec = dev_inst->hw_dev;
	afec->AFE_CHER = 1 << afec_ch;
}

/**
 * \brief Enable all AFEC channels.
 *
 * \param dev_inst  Device structure pointer.
 */
static inline void afec_enable_all_channel(struct afec_dev_inst *const dev_inst)
{
	Afec *afec = dev_inst->hw_dev;
	afec->AFE_CHER = 0xFFFFFF;
}

/**
 * \brief Disable the specified AFEC channel.
 *
 * \param dev_inst  Device structure pointer.
 * \param afec_ch AFEC channel number.
 */
static inline void afec_disable_one_channel(struct afec_dev_inst *const dev_inst,
		const enum afec_channel_num afec_ch)
{
	Afec *afec = dev_inst->hw_dev;
	afec->AFE_CHDR = 1 << afec_ch;
}

/**
 * \brief Disable all AFEC channel.
 *
 * \param dev_inst  Device structure pointer.
 */
static inline void afec_disable_all_channel(struct afec_dev_inst *const dev_inst)
{
	Afec *afec = dev_inst->hw_dev;
	afec->AFE_CHDR = 0xFFFFFF;
}

/**
 * \brief Get the AFEC channel status.
 *
 * \param dev_inst  Device structure pointer.
 * \param afec_ch AFEC channel number.
 *
 * \retval 1 if channel is enabled.
 * \retval 0 if channel is disabled.
 */
static inline uint32_t afec_get_channel_status(struct afec_dev_inst *const dev_inst,
		const enum afec_channel_num afec_ch)
{
	Afec *afec = dev_inst->hw_dev;
	return afec->AFE_CHSR & (1 << afec_ch);
}
/**
 * \brief Read the Converted Data of the selected channel.
 *
 * \param dev_inst  Device structure pointer.
 *
 * \return AFEC converted value of the selected channel.
 */
static inline uint32_t afec_get_channel_value(struct afec_dev_inst *const dev_inst)
{
	Afec *afec = dev_inst->hw_dev;
	return afec->AFE_CDR;
}

/**
 * \brief Read the Last Data Converted.
 *
 * \param dev_inst  Device structure pointer.
 *
 * \return AFEC latest converted value.
 */
static inline uint32_t afec_get_latest_value(struct afec_dev_inst *const dev_inst)
{
	Afec *afec = dev_inst->hw_dev;
	return afec->AFE_LCDR;
}

/**
 * \brief Enable AFEC interrupts.
 *
 * \param dev_inst  Device structure pointer.
 * \param ul_source Interrupts to be enabled.
 */
static inline void afec_enable_interrupt(struct afec_dev_inst *const dev_inst,
		const uint32_t ul_source)
{
	Afec *afec = dev_inst->hw_dev;
	afec->AFE_IER = ul_source;
}

/**
 * \brief Disable AFEC interrupts.
 *
 * \param dev_inst  Device structure pointer.
 * \param ul_source Interrupts to be disabled.
 */
static inline void afec_disable_interrupt(struct afec_dev_inst *const dev_inst,
		const uint32_t ul_source)
{
	Afec *afec = dev_inst->hw_dev;
	afec->AFE_IDR = ul_source;
}

/**
 * \brief Get AFEC interrupt status.
 *
 * \param dev_inst  Device structure pointer.
 *
 * \return The interrupt status value.
 */
static inline uint32_t afec_get_interrupt_status(struct afec_dev_inst *const dev_inst)
{
	Afec *afec = dev_inst->hw_dev;
	return afec->AFE_ISR;
}

/**
 * \brief Get AFEC interrupt mask.
 *
 * \param dev_inst  Device structure pointer.
 *
 * \return The interrupt mask value.
 */
static inline uint32_t afec_get_interrupt_mask(struct afec_dev_inst *const dev_inst)
{
	Afec *afec = dev_inst->hw_dev;
	return afec->AFE_IMR;
}

/**
 * \brief Get PDC registers base address.
 *
 * \return AFEC PDC register base address.
 */
static inline Pdc *afec_get_pdc_base(void)
{
	return PDC_AFEC0;
}

/**
 * \brief Set analog offset to be used for channel CSEL.
 *
 * \param dev_inst  Device structure pointer.
 */
static inline void afec_set_analog_offset(struct afec_dev_inst *const dev_inst,
		uint16_t offset)
{
	Afec *afec = dev_inst->hw_dev;
	afec->AFE_COCR = offset;
}

/**
 * \brief Set AFEC auto calibration mode.
 *
 * \param dev_inst  Device structure pointer.
 */
static inline void afec_set_calib_mode(struct afec_dev_inst *const dev_inst)
{
	Afec *afec = dev_inst->hw_dev;
	afec->AFE_CR |= AFE_CR_AUTOCAL;
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* AFEC_H_INCLUDED */
