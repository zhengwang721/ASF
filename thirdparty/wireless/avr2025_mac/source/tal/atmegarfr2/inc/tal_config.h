/**
 * @file tal_config.h
 *
 * @brief File contains TAL configuration parameters.
 *
 * $Id: tal_config.h 29695 2011-12-07 16:44:22Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2011, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef TAL_CONFIG_H
#define TAL_CONFIG_H

/* === INCLUDES ============================================================ */

#include "atmega256rfr2.h"

/* === EXTERNALS =========================================================== */


/* === MACROS ============================================================== */

#define TAL_RADIO_WAKEUP_TIME_SYM       (TAL_CONVERT_US_TO_SYMBOLS(SLEEP_TO_TRX_OFF_TYP_US))

#ifndef ANTENNA_DEFAULT
#define ANTENNA_DEFAULT                 (ANT_CTRL_2)
#endif

#ifdef ENABLE_FTN_PLL_CALIBRATION
/*
 * PLL calibration and filter tuning timer timeout in minutes
 */
#define TAL_CALIBRATION_TIMEOUT_MIN         (5UL)

/*
 * PLL calibration and filter tuning timer timeout in us,
 */
#define TAL_CALIBRATION_TIMEOUT_US          ((TAL_CALIBRATION_TIMEOUT_MIN) * (60UL) * (1000UL) * (1000UL))
#endif  /* ENABLE_FTN_PLL_CALIBRATION */

/* === TYPES =============================================================== */


#ifdef ENABLE_QUEUE_CAPACITY
#define TAL_INCOMING_FRAME_QUEUE_CAPACITY   (255)
#endif  /* ENABLE_QUEUE_CAPACITY */

/* === PROTOTYPES ========================================================== */


#endif /* TAL_CONFIG_H */
