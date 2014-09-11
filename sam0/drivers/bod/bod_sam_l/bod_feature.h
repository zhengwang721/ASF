/**
 * \file
 *
 * \brief SAM Brown Out Detector Driver
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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
#ifndef BOD_FEATURE_H_INCLUDED
#define BOD_FEATURE_H_INCLUDED



#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup asfdoc_sam0_bod_group
 * @{
 */


/**
 * \brief Brown Out Detector input clock prescale values.
 *
 * List of possible BOD33 controller prescaler values, to reduce the sampling
 * speed of a BOD33 to lower the power consumption.
 */
enum bod33_prescale {
	/** Divide input prescaler clock by 2 */
	BOD33_PRESCALE_DIV_2       = SUPC_BOD33_PSEL(0),
	/** Divide input prescaler clock by 4 */
	BOD33_PRESCALE_DIV_4       = SUPC_BOD33_PSEL(1),
	/** Divide input prescaler clock by 8 */
	BOD33_PRESCALE_DIV_8       = SUPC_BOD33_PSEL(2),
	/** Divide input prescaler clock by 16 */
	BOD33_PRESCALE_DIV_16      = SUPC_BOD33_PSEL(3),
	/** Divide input prescaler clock by 32*/
	BOD33_PRESCALE_DIV_32      = SUPC_BOD33_PSEL(4),
	/** Divide input prescaler clock by 64 */
	BOD33_PRESCALE_DIV_64      = SUPC_BOD33_PSEL(5),
	/** Divide input prescaler clock by 128 */
	BOD33_PRESCALE_DIV_128     = SUPC_BOD33_PSEL(6),
	/** Divide input prescaler clock by 256 */
	BOD33_PRESCALE_DIV_256     = SUPC_BOD33_PSEL(7),
	/** Divide input prescaler clock by 512 */
	BOD33_PRESCALE_DIV_512     = SUPC_BOD33_PSEL(8),
	/** Divide input prescaler clock by 1024 */
	BOD33_PRESCALE_DIV_1024    = SUPC_BOD33_PSEL(9),
	/** Divide input prescaler clock by 2048 */
	BOD33_PRESCALE_DIV_2048    = SUPC_BOD33_PSEL(10),
	/** Divide input prescaler clock by 4096 */
	BOD33_PRESCALE_DIV_4096    = SUPC_BOD33_PSEL(11),
	/** Divide input prescaler clock by 8192 */
	BOD33_PRESCALE_DIV_8192    = SUPC_BOD33_PSEL(12),
	/** Divide input prescaler clock by 16384 */
	BOD33_PRESCALE_DIV_16384   = SUPC_BOD33_PSEL(13),
	/** Divide input prescaler clock by 32768 */
	BOD33_PRESCALE_DIV_32768   = SUPC_BOD33_PSEL(14),
	/** Divide input prescaler clock by 65536 */
	BOD33_PRESCALE_DIV_65536   = SUPC_BOD33_PSEL(15),
};

/**
 * \brief Brown Out Detector voltage monitors.
 *
 * List of possible BOD33 module voltage monitored in active and standby mode.
 */
enum bod33_vol_monitor {
	/** The BOD33 monitors the VDD power pin in active and standby mode. */
	BOD33_VMON_VDD  = 0,
	/** The BOD33 monitors the VBAT power pin in active and standby mode. */
	BOD33_VMON_VBAT = SUPC_BOD33_VMON,
};

/**
 * \brief Brown Out Detector sampling modes in active sleep mode.
 *
 * List of possible BOD33 module voltage sampling modes in active sleep mode.
 */
enum bod33_activecfg {
	/** BOD33 will sample the supply line continuously. */
	BOD33_ACTCFG_CONTINUOUS = 0,
	/** BOD33 will use the BOD33 sampling clock (1kHz) to sample the supply line. */
	BOD33_ACTCFG_SAMPLED    = SUPC_BOD33_ACTCFG,
};

/**
 * \brief Brown Out Detector sampling modes in standby sleep mode.
 *
 * List of possible BOD33 module voltage sampling modes in standby sleep mode.
 */
enum bod33_standbycfg {
	/** BOD33 will sample the supply line continuously. */
	BOD33_STDBYCFG_CONTINUOUS = 0,
	/** BOD33 will use the BOD33 sampling clock (1kHz) to sample the supply line. */
	BOD33_STDBYCFG_SAMPLED    = SUPC_BOD33_STDBYCFG,
};

/**
 * \brief Brown Out Detector detection actions.
 *
 * List of possible BOD33 actions when a BOD33 module detects a brown-out condition.
 */
enum bod33_action {
	/** A BOD33 detect will do nothing, and the BOD33 state must be polled. */
	BOD33_ACTION_NONE      = SUPC_BOD33_ACTION(0),
	/** A BOD33 detect will reset the device. */
	BOD33_ACTION_RESET     = SUPC_BOD33_ACTION(1),
	/** A BOD33 detect will fire an interrupt. */
	BOD33_ACTION_INTERRUPT = SUPC_BOD33_ACTION(2),
	/** A BOD33 detect will put the device in backup sleep mode. */
	BOD33_ACTION_BACKUP    = SUPC_BOD33_ACTION(3),
};

/** Configuration structure for a BOD33 module. */
struct bod33_config {
	/** Input sampler clock prescaler factor, to reduce the 1KHz clock from the
	 *  ULP32K to lower the sampling rate of the BOD33. */
	enum bod33_prescale prescaler;
	/** Voltage monitored in active and standby mode. */
	enum bod33_vol_monitor monitor;
	/** BOD33 configuration in active mode. */
	enum bod33_activecfg activecfg;
	/** BOD33 configuration in backup sleep mode. */
	enum bod33_standbycfg standbycfg;
	/** Action to perform when a low power detection is made. */
	enum bod33_action action;
	/** BOD33 level to trigger at when monitors VBAT or in backup sleep mode. */
	uint8_t backuplevel;
	/** BOD33 level to trigger at when monitors VDD excpt in backup sleep mode. */
	uint8_t level;
	/** If \c true, the BOD33 is kept enabled and sampled during device sleep. */
	bool run_in_backup;
	/** If \c true, the BOD33 is kept enabled and sampled during standby. */
	bool run_in_standby;
	/** If \c true, enables detection hysteresis. */
	bool hysteresis;
};


/**
 * \brief Brown Out Detector input clock prescale values.
 *
 * List of possible BOD12 controller prescaler values, to reduce the sampling
 * speed of a BOD12 to lower the power consumption.
 */
enum bod12_prescale {
	/** Divide input prescaler clock by 2 */
	BOD12_PRESCALE_DIV_2       = SUPC_BOD12_PSEL(0),
	/** Divide input prescaler clock by 4 */
	BOD12_PRESCALE_DIV_4       = SUPC_BOD12_PSEL(1),
	/** Divide input prescaler clock by 8 */
	BOD12_PRESCALE_DIV_8       = SUPC_BOD12_PSEL(2),
	/** Divide input prescaler clock by 16 */
	BOD12_PRESCALE_DIV_16      = SUPC_BOD12_PSEL(3),
	/** Divide input prescaler clock by 32*/
	BOD12_PRESCALE_DIV_32      = SUPC_BOD12_PSEL(4),
	/** Divide input prescaler clock by 64 */
	BOD12_PRESCALE_DIV_64      = SUPC_BOD12_PSEL(5),
	/** Divide input prescaler clock by 128 */
	BOD12_PRESCALE_DIV_128     = SUPC_BOD12_PSEL(6),
	/** Divide input prescaler clock by 256 */
	BOD12_PRESCALE_DIV_256     = SUPC_BOD12_PSEL(7),
	/** Divide input prescaler clock by 512 */
	BOD12_PRESCALE_DIV_512     = SUPC_BOD12_PSEL(8),
	/** Divide input prescaler clock by 1024 */
	BOD12_PRESCALE_DIV_1024    = SUPC_BOD12_PSEL(9),
	/** Divide input prescaler clock by 2048 */
	BOD12_PRESCALE_DIV_2048    = SUPC_BOD12_PSEL(10),
	/** Divide input prescaler clock by 4096 */
	BOD12_PRESCALE_DIV_4096    = SUPC_BOD12_PSEL(11),
	/** Divide input prescaler clock by 8192 */
	BOD12_PRESCALE_DIV_8192    = SUPC_BOD12_PSEL(12),
	/** Divide input prescaler clock by 16384 */
	BOD12_PRESCALE_DIV_16384   = SUPC_BOD12_PSEL(13),
	/** Divide input prescaler clock by 32768 */
	BOD12_PRESCALE_DIV_32768   = SUPC_BOD12_PSEL(14),
	/** Divide input prescaler clock by 65536 */
	BOD12_PRESCALE_DIV_65536   = SUPC_BOD12_PSEL(15),
};

/**
 * \brief Brown Out Detector sampling modes in active sleep mode.
 *
 * List of possible BOD12 module voltage sampling modes in active sleep mode.
 */
enum bod12_activecfg {
	/** BOD12 will sample the supply line continuously. */
	BOD12_ACTCFG_CONTINUOUS = 0,
	/** BOD12 will use the BOD12 sampling clock (1kHz) to sample the supply line. */
	BOD12_ACTCFG_SAMPLED    = SUPC_BOD12_ACTCFG,
};

/**
 * \brief Brown Out Detector sampling modes in standby sleep mode.
 *
 * List of possible BOD12 module voltage sampling modes in standby sleep mode.
 */
enum bod12_standbycfg {
	/** BOD12 will sample the supply line continuously. */
	BOD12_STDBYCFG_CONTINUOUS = 0,
	/** BOD12 will use the BOD12 sampling clock (1kHz) to sample the supply line. */
	BOD12_STDBYCFG_SAMPLED    = SUPC_BOD12_STDBYCFG,
};

/**
 * \brief Brown Out Detector detection actions.
 *
 * List of possible BOD12 actions when a BOD12 module detects a brown-out condition.
 */
enum bod12_action {
	/** A BOD12 detect will do nothing, and the BOD12 state must be polled. */
	BOD12_ACTION_NONE      = SUPC_BOD12_ACTION(0),
	/** A BOD12 detect will reset the device. */
	BOD12_ACTION_RESET     = SUPC_BOD12_ACTION(1),
	/** A BOD12 detect will fire an interrupt. */
	BOD12_ACTION_INTERRUPT = SUPC_BOD12_ACTION(2),
};

/** Configuration structure for a BOD12 module. */
struct bod12_config {
	/** Input sampler clock prescaler factor, to reduce the 1KHz clock from the
	 *  ULP32K to lower the sampling rate of the BOD12. */
	enum bod12_prescale prescaler;
	/** BOD12 configuration in active mode. */
	enum bod12_activecfg activecfg;
	/** BOD12 configuration in backup sleep mode. */
	enum bod12_standbycfg standbycfg;
	/** Action to perform when a low power detection is made. */
	enum bod12_action action;
	/** BOD12 level to trigger at (see electrical section of device datasheet). */
	uint8_t level;
	/** If \c true, the BOD12 is kept enabled and sampled during device sleep. */
	bool run_in_standby;
	/** If \c true, enables detection hysteresis. */
	bool hysteresis;
};


/**
 * \name Configuration and Initialization
 * @{
 */

/**
 * \brief Get default BOD33 configuration.
 *
 * The default BOD33 configuration is:
 * - Clock prescaler set to divide the input clock by 2
 * - Continuous in active mode
 * - Continuous in standby mode
 * - Monitor the VDD power pin
 * - Reset on BOD33 detect
 * - Hysteresis enabled
 * - BOD33 level 0x27 on VDD
 * - BOD33 level 0x27 on VBAT
 * - BOD33 kept enabled during device sleep
 * - BOD33 kept enabled during standby
 *
 * \param[out] conf  BOD33 configuration struct to set to default settings
 */
static inline void bod33_get_config_defaults(
		struct bod33_config *const conf)
{
	/* Sanity check arguments */
	Assert(conf);

	conf->prescaler      = BOD33_PRESCALE_DIV_2;
	conf->monitor        = BOD33_VMON_VDD;
	conf->activecfg      = BOD33_ACTCFG_CONTINUOUS;
	conf->standbycfg     = BOD33_STDBYCFG_CONTINUOUS;
	conf->action         = BOD33_ACTION_RESET;
	conf->level          = 0x27;
	conf->backuplevel    = 0x27;
	conf->run_in_backup  = true;
	conf->run_in_standby = true;
	conf->hysteresis     = true;
}

enum status_code bod33_set_config(
		struct bod33_config *const conf);

/**
 * \brief Enables a configured BOD33 module.
 *
 * Enables the BOD33 module that has been previously configured.
 *
 * \return Error code indicating the status of the enable operation.
 *
 * \retval STATUS_OK               If the BOD33 was successfully enabled
 */
static inline enum status_code bod33_enable(void)
{
	SUPC->BOD33.reg |= SUPC_BOD33_ENABLE;
	return STATUS_OK;
}

/**
 * \brief Disables an enabled BOD33 module.
 *
 * Disables the BOD33 module that was previously enabled.
 *
 * \return Error code indicating the status of the disable operation.
 *
 * \retval STATUS_OK               If the BOD33 was successfully disabled
 */
static inline enum status_code bod33_disable(void)
{
	SUPC->BOD33.reg &= ~SUPC_BOD33_ENABLE;
	return STATUS_OK;
}

/**
 * \brief Checks if the BOD33 low voltage detection has occurred.
 *
 * Determines if the BOD33 has detected a voltage lower than its
 * configured threshold.
 *
 * \return Detection status of the BOD33.
 *
 * \retval true   If the BOD33 has detected a low voltage condition
 * \retval false  If the BOD33 has not detected a low voltage condition
 */
static inline bool bod33_is_detected(void)
{
	return SUPC->STATUS.bit.BOD33DET;
}

/**
 * \brief Clears the low voltage detection state of the BOD33.
 *
 * Clears the low voltage condition of BOD33 module, so that new
 * low voltage conditions can be detected.
 *
 */
static inline void bod33_clear_detected(void)
{
	SUPC->INTFLAG.bit.BOD33DET = true;
	return;
}

/**
 * \brief Get default BOD12 configuration.
 *
 * The default BOD12 configuration is:
 * - Clock prescaler set to divide the input clock by 2
 * - Continuous in active mode
 * - Continuous in standby mode
 * - Reset on BOD12 detect
 * - Hysteresis enabled
 * - BOD12 level 0x12
 * - BOD12 kept enabled during device sleep
 *
 * \param[out] conf  BOD12 configuration struct to set to default settings
 */
static inline void bod12_get_config_defaults(
		struct bod12_config *const conf)
{
	/* Sanity check arguments */
	Assert(conf);

	conf->prescaler      = BOD12_PRESCALE_DIV_2;
	conf->activecfg      = BOD12_ACTCFG_CONTINUOUS;
	conf->standbycfg     = BOD12_STDBYCFG_CONTINUOUS;
	conf->action         = BOD12_ACTION_RESET;
	conf->level          = 0x12;
	conf->run_in_standby = true;
	conf->hysteresis     = true;
}

enum status_code bod12_set_config(
		struct bod12_config *const conf);

/**
 * \brief Enables a configured BOD12 module.
 *
 * Enables the BOD12 module that has been previously configured.
 *
 * \return Error code indicating the status of the enable operation.
 *
 * \retval STATUS_OK               If the BOD12 was successfully enabled
 */
static inline enum status_code bod12_enable(void)
{
	SUPC->BOD12.reg |= SUPC_BOD12_ENABLE;
	return STATUS_OK;
}

/**
 * \brief Disables an enabled BOD12 module.
 *
 * Disables the BOD12 module that was previously enabled.
 *
 * \return Error code indicating the status of the disable operation.
 *
 * \retval STATUS_OK               If the BOD12 was successfully disabled
 */
static inline enum status_code bod12_disable(void)
{
	SUPC->BOD12.reg &= ~SUPC_BOD12_ENABLE;
	return STATUS_OK;
}

/**
 * \brief Checks if the BOD12 low voltage detection has occurred.
 *
 * Determines if the BOD12 has detected a voltage lower than its
 * configured threshold.
 *
 * \return Detection status of the BOD12.
 *
 * \retval true   If the BOD12 has detected a low voltage condition
 * \retval false  If the BOD12 has not detected a low voltage condition
 */
static inline bool bod12_is_detected(void)
{
	return SUPC->STATUS.bit.BOD12DET;
}

/**
 * \brief Clears the low voltage detection state of the BOD12.
 *
 * Clears the low voltage condition of BOD12 module, so that new
 * low voltage conditions can be detected.
 *
 */
static inline void bod12_clear_detected(void)
{
	SUPC->INTFLAG.bit.BOD12DET = true;
	return;
}

/** @} */

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /* BOD_FEATURE_H_INCLUDED */
