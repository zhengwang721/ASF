/**
 * @file tal_pib.h
 *
 * @brief This file contains the defines for initial TAL PIB attribute values.
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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
 *
 */

/*
 * Copyright (c) 2013, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef TAL_PIB_H
#define TAL_PIB_H

/* === INCLUDES ============================================================ */

#include "ieee_15_4g.h"

/* === EXTERNALS =========================================================== */

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/*
 * TAL PIB default values
 */

/**
 * Default value of maximum backoff exponent used while performing CSMA
 */
#define TAL_RF09_MAXBE_DEF                      (0x05)
#define TAL_RF24_MAXBE_DEF                      (0x05)

/**
 * Default value of PIB attribute macMaxFrameRetries
 */
#define TAL_RF09_MAXFRAMERETRIES_DEF            (0x03)
#define TAL_RF24_MAXFRAMERETRIES_DEF            (0x03)

/**
 * Default value of maximum CSMA backoffs
 */
#define TAL_RF09_MAX_CSMA_BACKOFFS_DEF          (0x04)
#define TAL_RF24_MAX_CSMA_BACKOFFS_DEF          (0x04)

/**
 * Default value of minimum backoff exponent used while performing CSMA
 */
#define TAL_RF09_MINBE_DEF                      (0x03)
#define TAL_RF24_MINBE_DEF                      (0x03)

/**
 * Default value of the maximum number of frames that are allowed to be
 * received during CSMA backoff periods for a tx transaction.
 */
#define TAL_RF09_MAX_FRAMES_DURING_BACKOFF_DEF  (0x05)
#define TAL_RF24_MAX_FRAMES_DURING_BACKOFF_DEF  (0x05)

/**
 * Value of a broadcast PAN ID
 */
#define TAL_PANID_BC_DEF                        (0xFFFF)

/**
 * Default value of short address
 */
#define TAL_SHORT_ADDRESS_DEF                   (0xFFFF)

/**
 * Default value of current channel in TAL
 */
#define TAL_RF09_CURRENT_CHANNEL_DEF            (0x00)
#define TAL_RF24_CURRENT_CHANNEL_DEF            (0x00)
#define TAL_RF24_CURRENT_CHANNEL_LEG_DEF        (0x0B)
#define TAL_RF09_CURRENT_CHANNEL_LEG_DEF        (0x01)

#ifndef CUSTOM_DEFAULT_TX_PWR
/*
 * Default value of transmit power of transceiver: Use highest tx power
 */
#define TAL_TRANSMIT_POWER_DEFAULT              (0x04)
#endif

/**
 * Default value of PAN Coordinator custom TAL PIB
 */
#define TAL_RF09_PAN_COORDINATOR_DEF            (false)
#define TAL_RF24_PAN_COORDINATOR_DEF            (false)

/**
 * FCS type default value
 */
#define TAL_RF09_FCS_TYPE_DEFAULT               FCS_TYPE_4_OCTETS
#define TAL_RF24_FCS_TYPE_DEFAULT               FCS_TYPE_4_OCTETS

/**
 * Default value of PHY / modulation
 */
#define TAL_RF09_MODULATION_DEF                 OFDM
#define TAL_RF24_MODULATION_DEF                 OFDM

/**
 * Default value of PHY / frequency band
 */
#define TAL_RF09_FRQ_BAND_DEF                   US_915
#define TAL_RF24_FRQ_BAND_DEF                   WORLD_2450

/**
 * Default value of frequency F0
 */
#define TAL_RF09_FRQ_F0                         OFDM_915_OPT1_F0
#define TAL_RF24_FRQ_F0                         OFDM_2450_OPT1_F0

/**
 * Default value of channel spacing
 */
#define TAL_RF09_CH_SPACING                     OFDM_915_OPT1_CH_SPAC
#define TAL_RF24_CH_SPACING                     OFDM_2450_OPT1_CH_SPAC

/**
 * Default value of PHY / OFDM option
 */
#define TAL_RF09_OFDM_OPT_DEF                   OFDM_OPT_1
#define TAL_RF24_OFDM_OPT_DEF                   OFDM_OPT_1

/**
 * Default value of PHY / OFMD MCS value
 */
#define TAL_RF09_OFDM_MCS_VAL_DEF               MCS3
#define TAL_RF24_OFDM_MCS_VAL_DEF               MCS3

/**
 * Default value of PHY / OFMD interleaving enable value
 */
#define TAL_RF09_OFDM_INTERL_DEF                true
#define TAL_RF24_OFDM_INTERL_DEF                true

/**
 * Default value of adapt data rate of the incoming frame for ACK transmission
 */
#define TAL_RF09_ADAPT_DATA_RATE_FOR_ACK        true
#define TAL_RF24_ADAPT_DATA_RATE_FOR_ACK        true

/**
 * Default value of transmit power in dBm
 */
#define TAL_RF09_OFDM_MAX_TX_PWR                11
#define TAL_RF24_OFDM_MAX_TX_PWR                11

/**
 * Default value of frame filter settings
 */
#define TAL_RF09_FRAMES_TYPES       ((1 << FCF_FRAMETYPE_BEACON) | \
                                     (1 << FCF_FRAMETYPE_DATA) | \
                                     (1 << FCF_FRAMETYPE_MAC_CMD))
#define TAL_RF09_FRAME_VERSIONS     ((1 << FCF1_FV_2003) | (1 << FCF1_FV_2006))
#define TAL_RF24_FRAMES_TYPES       ((1 << FCF_FRAMETYPE_BEACON) | \
                                     (1 << FCF_FRAMETYPE_DATA) | \
                                     (1 << FCF_FRAMETYPE_MAC_CMD))
#define TAL_RF24_FRAME_VERSIONS     ((1 << FCF1_FV_2003) | (1 << FCF1_FV_2006))

/* === PROTOTYPES ========================================================== */


#endif /* TAL_PIB_H */

/* EOF */
