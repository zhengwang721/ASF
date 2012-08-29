/*****************************************************************************
*
* \file
*
* \brief DACIFB header for AVR32 UC3.
*
* This file defines a useful set of functions for DACIFB on AVR32 devices.
*
* Copyright (c) 2009-2012 Atmel Corporation. All rights reserved.
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
*****************************************************************************/

#ifndef _DACIFB_H_
#define _DACIFB_H_

/**
 * \defgroup group_avr32_drivers_dacifb DACIFB - DAC Interface B
 *
 * Driver for the Digital to Analog Converter (DAC) Interface B. It provides
 * functions for configuring and using the DAC and its channels.
 *
 * @{
 */

#include <avr32/io.h>
#include "compiler.h"

#if UC3C
#  define AVR32_FLASHC_CALIBRATION_FOURTH_WORD_ADDRESS    0x8080020C
#  define AVR32_FLASHC_CALIBRATION_FIVETH_WORD_ADDRESS    0x80800214
#endif

/** \name Channel Selection used by the DAC */
/** @{ */
#define  DACIFB_CHANNEL_SELECTION_NONE     0x0
#define  DACIFB_CHANNEL_SELECTION_A        0x1
#define  DACIFB_CHANNEL_SELECTION_B        0x2
#define  DACIFB_CHANNEL_SELECTION_AB       0x3
/** @} */

/** \name Reference used by the DAC */
/** @{ */
#define  DACIFB_REFERENCE_EXT              0x0
#define  DACIFB_REFERENCE_VDDANA           0x1
/** @} */

/** \name Trigger Mode used by the DAC */
/** @{ */
#define  DACIFB_TRIGGER_MODE_MANUAL        0x0
#define  DACIFB_TRIGGER_MODE_TIMER         0x1
#define  DACIFB_TRIGGER_MODE_EVENT         0x2
/** @} */

/** \name Return Status Values */
/** @{ */
#define  DACIFB_CONFIGURATION_REFUSED      0x0
#define  DACIFB_CONFIGURATION_ACCEPTED     0x1
#define  DACIFB_STATUS_COMPLETED           0x2
#define  DACIFB_STATUS_NOT_COMPLETED       0x3
/** @} */

/** CHI Min Value of 2us */
#define  DACIFB_CHI_MIN_VALUE              500000

/** CHRx Min Value of 25us */
#define  DACIFB_CHRx_MIN_VALUE             40000

/** CHRx Max Value of 35us */
#define  DACIFB_CHRx_MAX_VALUE             33333

/** Parameters for the DACIFB. */
typedef struct {
	/** Reference for DAC Conversion */
	bool reference;

	/** Channel Selection */
	uint8_t channel_selection;

	/** Gain Calibration Value */
	uint16_t gain_calibration_value;

	/** Offset Calibration Value */
	uint16_t offset_calibration_value;

	/** Low Power Mode */
	bool low_power;

	/** Dual Mode */
	bool dual;

	/** Prescaler Clock in Hertz (should be > 500000Hz) */
	uint32_t prescaler_clock_hz;
} dacifb_opt_t;

/** Parameters for the configuration of the channel. */
typedef struct {
	/** Auto Refresh Mode. */
	bool auto_refresh_mode;

	/** Trigger Mode: Manual/Timer/Event */
	uint8_t trigger_mode;

	/** Left or Right Adjustment. */
	bool left_adjustment;

	/** Data Shift Value. */
	uint8_t data_shift;

	/** Data Round. */
	bool data_round_enable;
} dacifb_channel_opt_t;

/** \brief Get DACIFB Calibration Data.
 *
 * Mandatory to call if factory calibration data are wanted to be used.
 * If not called, Calibration Data should be set by the application.
 *
 * \param *dacifb       Base address of the DACIFB
 * \param *p_dacifb_opt Structure for the DACIFB core configuration
 * \param instance      DACIFB core instance 0 for DACIFB0 or 1 for DACIFB1
 */
void dacifb_get_calibration_data(volatile avr32_dacifb_t *dacifb,
		dacifb_opt_t *p_dacifb_opt,
		uint8_t instance);

/** \brief Configure DACIFB.
 *
 * Mandatory to call. If not called, DACIFB channels will have side effects.
 *
 * \param *dacifb        Base address of the DACIFB
 * \param *p_dacifb_opt  Structure for the DACIFB core configuration
 * \param pb_hz          Peripheral Bus frequency
 * \return DACIFB_CONFIGURATION_REFUSED or DACIFB_CONFIGURATION_ACCEPTED
 */
uint8_t dacifb_configure(volatile avr32_dacifb_t *dacifb,
		dacifb_opt_t *p_dacifb_opt,
		uint32_t pb_hz);

/** \brief Configure DACIFB specific channel.
 *
 * Sets channel Adjustment, Refresh_time and Trigger Mode settings.
 *
 * \param  *dacifb                Base address of the ADCIFA
 * \param  channel                DACIFB_CHANNEL_SELECTION_NONE /
 *                                DACIFB_CHANNEL_SELECTION_A /
 *                                DACIFB_CHANNEL_SELECTION_B /
 *                                DACIFB_CHANNEL_SELECTION_AB
 * \param  p_dacifb_channel_opt   Structure for the sequencer configuration
 * \param  prescaler_clock_hz     Prescaler Clock in Hertz (should be >
 *                                500000Hz)
 * \return DACIFB_CONFIGURATION_REFUSED or DACIFB_CONFIGURATION_ACCEPTED
 */
uint8_t dacifb_configure_channel(volatile avr32_dacifb_t *dacifb,
		uint8_t channel,
		dacifb_channel_opt_t *p_dacifb_channel_opt,
		uint32_t prescaler_clock_hz);

/** \brief Start analog to digital conversion
 * \param *dacifb   Base address of the DACIFB
 * \param  channel  DACIFB_CHANNEL_SELECTION_NONE /
 *                  DACIFB_CHANNEL_SELECTION_A / DACIFB_CHANNEL_SELECTION_B /
 *                  DACIFB_CHANNEL_SELECTION_AB
 * \param  cpu_hz   CPU Clock frequency
 */
void dacifb_start_channel(volatile avr32_dacifb_t *dacifb,
		uint8_t channel,
		uint32_t cpu_hz);

/** \brief Check channel conversion status
 *
 * \param *dacifb    Base address of the DACIFB
 * \param  channel   channel to check (0 to 1)
 * \return Boolean true if conversion not running,  false if conversion running.
 */
bool dacifb_check_eoc(volatile avr32_dacifb_t *dacifb,
		uint8_t channel);

/** \brief Set channel value
 *
 * \param *dacifb    Base address of the DACIFB
 * \param  channel   channel to handle (0 to 1)
 * \param  dual      Dual Mode Selection
 * \param  value     Value to be converted
 */
void dacifb_set_value(volatile avr32_dacifb_t *dacifb,
		uint8_t channel,
		bool dual,
		uint32_t value);

/** \brief Reload Timer for Automatic Trigger on DAC
 *  \param *dacifb  Base address of the DACIFB
 *  \param channel  DACIFB_CHANNEL_SELECTION_NONE / DACIFB_CHANNEL_SELECTION_A /
 *                  DACIFB_CHANNEL_SELECTION_B / DACIFB_CHANNEL_SELECTION_AB
 * \param  timer_us Timer Value in Microseconds
 * \param  prescaler_clock_hz   Prescaler Clock in Hertz (should be > 500000Hz)
 */
void dacifb_reload_timer(volatile avr32_dacifb_t *dacifb,
		uint8_t channel,
		uint8_t timer_us,
		uint32_t prescaler_clock_hz);

/** @} */

#endif  /* _DACIFB_H_ */
