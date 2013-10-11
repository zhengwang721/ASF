/**
 * @file tal_pib.h
 *
 * @brief This file contains the defines for initial TAL PIB attribute values.
 *
 * $Id: tal_pib.h 35159 2013-09-24 06:52:33Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2012, Atmel Corporation All rights reserved.
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
