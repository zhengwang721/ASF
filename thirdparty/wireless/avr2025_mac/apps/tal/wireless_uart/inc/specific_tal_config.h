/**
 * @file specific_tal_config.h
 *
 * @brief File contains TAL configuration parameters.
 *
 * $Id: specific_tal_config.h 35117 2013-09-18 09:04:56Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2013, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#ifdef SPECIFIC_TAL_CONFIG

/* Prevent double inclusion */
#ifndef SPECIFIC_TAL_CONFIG_H
#define SPECIFIC_TAL_CONFIG_H

/* === INCLUDES ============================================================ */


#ifdef AT86RF215LT
#include "at86rf215lt.h"
#else
#include "at86rf215.h"
#endif

/* === EXTERNALS =========================================================== */

/* === MACROS ============================================================== */

#define TAL_FIRST_TIMER_ID              (0)

/**
 * To include the filter tuning functions to the build uncomment the define
 * ENABLE_FTN_PLL_CALIBRATION.
 */
//#define ENABLE_FTN_PLL_CALIBRATION

/**
 * To include the functionality of measuring the on air duration to the build,
 * uncomment the define MEASURE_ON_AIR_DURATION.
 * The on-air-duration can be read/written using the PIB attribute
 * phyOnAirDuration.
 */
//#define MEASURE_ON_AIR_DURATION

/**
 * To include the functionality of adaption the data rate of the incoming frame
 * to the ACK frame, uncomment the define ENABLE_ACK_RATE_MODE_ADAPTION.
 * The actual use of this feature can be enabled/disabled during runtime
 * using the PIB attribute macAdaptDataRateForACK.
 */
//#define ENABLE_ACK_RATE_MODE_ADAPTION

/**
 * During CSMA backoff periods the receiver can be set to receive mode.
 * This feature can be enabled during compile time by uncomment the define
 * RX_WHILE_BACKOFF.
 */
//#define RX_WHILE_BACKOFF

/**
 * For applications such as sniffer or beacon-enabled applications a time stamp
 * for each frame is required.
 * This feature can be enabled during compile time by uncomment the define
 * ENABLE_TSTAMP.
 */
//#define ENABLE_TSTAMP

/**
 * To include the promiscuous mode functionality to the build, uncomment the
 * following define PROMISCUOUS_MODE.
 * The actual use of this feature can be enabled/disabled during runtime
 * using the PIB attribute macPromiscuousMode.
 */
#ifndef PROMISCUOUS_MODE
//#define PROMISCUOUS_MODE
#endif

/**
 * To include the frame filter configuration functionality to the build,
 * uncomment the following define FRAME_FILTER_CONFIGURATION.
 * The actual use of this feature can be controlled during runtime
 * using the PIB attributes macFrameFilterFrameTypes and
 * macFrameFilterFrameVersion.
 */
//#define FRAME_FILTER_CONFIGURATION

/**
 * To include the clock output configuration to the build,
 * uncomment the following define TRX_CLOCK_OUTPUT_SELECTION to the required value.
 * Value - meaning
 * 0       off
 * 1       26 MHz (default/reset)
 * 2       32 MHz
 */
#define TRX_CLOCK_OUTPUT_SELECTION  0

/**
 * To change the transceiver's interrupt polarity,
 * uncomment the following define TRX_IRQ_POLARITY to the required value.
 * Value - meaning
 * 1 = HIGH (default/reset)
 * 0 = LOW
 */
#define TRX_IRQ_POLARITY          0

/**
 * To include the legacy O-QPSK support,
 * uncomment the following define SUPPORT_LEGACY_OQPSK.
 */
//#define SUPPORT_LEGACY_OQPSK

/**
 * Enable support of MR-OQPSK modulation
 * To include the MR-OQSK support to the build,
 * uncomment the following define SUPPORT_OQPSK.
 */
#define SUPPORT_OQPSK

/**
 * Enable support of MR-OFDM modulation
 * To include the MR-OFDM support to the build,
 * uncomment the following define SUPPORT_OFDM.
 */
#define SUPPORT_OFDM

/**
 * Enable support of FSK modulation
 * To include the FSK support to the build,
 * uncomment the following define SUPPORT_FSK.
 */
#ifndef AT86RF215LT
#define SUPPORT_FSK
#endif

/**
 * Check if at least one modulation is enabled.
 */
#if ((!defined SUPPORT_LEGACY_OQPSK) && (!defined SUPPORT_OQPSK) && \
     (!defined SUPPORT_OFDM) && (!defined SUPPORT_FSK))
#error "Missing modulation support, enable at least one modulation support."
#endif

/* === TYPES =============================================================== */


/* === PROTOTYPES ========================================================== */

#endif /* #ifdef SPECIFIC_TAL_CONFIG */

#endif /* SPECIFIC_TAL_CONFIG_H */
