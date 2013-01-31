/**
 * @file nwk_config.h
 *
 * @brief These are all timer IDs which are used in NWK.
 *
 * $Id: nwk_config.h 33817 2012-12-14 05:54:00Z agasthian.s $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef NWK_CONFIG_H
#define NWK_CONFIG_H

/* === Includes ============================================================= */


//#include "mac_config.h"
#include "ieee_const.h"

/* === Macros =============================================================== */




/* === Types ================================================================ */



#if ((defined CHANNEL_AGILITY) || (defined RF4CE_PLATFORM))
#define NUMBER_OF_CH_AG_TIMERS          1
#else
#define NUMBER_OF_CH_AG_TIMERS          0
#endif

#ifdef ZRC_PROFILE
#define NUMBER_OF_PROFILE_TIMERS        1
#else
#define NUMBER_OF_PROFILE_TIMERS        0
#endif

#ifdef PBP_REC
#define PBP_TIMER                       1
#else
#define PBP_TIMER                       0
#endif

#define NUMBER_OF_NWK_TIMERS            1 + NUMBER_OF_CH_AG_TIMERS + \
                                        NUMBER_OF_PROFILE_TIMERS + PBP_TIMER
#if (HIGHEST_STACK_LAYER == RF4CE)
#define NUMBER_OF_LARGE_STACK_BUFS              (5)

#define NUMBER_OF_SMALL_STACK_BUFS              (0)
#endif
#if (HIGHEST_STACK_LAYER == RF4CE)


/* Reduce the header file dependency by using hard-coded values */
#if (MEGA || XMEGA)
#define MCPS_DATA_IND_SIZE  (32)        /* Size of mcps_data_ind_t w/o MAC security */
#define LARGE_BUFFER_SIZE           (MCPS_DATA_IND_SIZE + \
                                     aMaxMACPayloadSize + \
                                     FCS_LEN + \
                                     LQI_LEN + ED_VAL_LEN)
#elif (UC3)
// The discovery confirm message needs to be held by the buffer; its size is 180.
#define LARGE_BUFFER_SIZE            180
#elif (SAM)
// The discovery confirm message needs to be held by the buffer; its size is 180.
#define LARGE_BUFFER_SIZE            176
#else
#error "Unknown platform type"
#endif
#endif  /*#if (HIGHEST_STACK_LAYER == RF4CE)  */


#ifdef ENABLE_QUEUE_CAPACITY
/**
 * Macro configuring the queue capacities.
 */
#define NHLE_NWK_QUEUE_CAPACITY         (255)
#define NWK_NHLE_QUEUE_CAPACITY         (255)
#define MAC_NWK_QUEUE_CAPACITY          (255)
#endif  /* ENABLE_QUEUE_CAPACITY */

#define DEVICE_TYPE_LIST_SIZE           3
#define PROFILE_ID_LIST_SIZE            7
#define NWKC_VENDOR_IDENTIFIER           (0x1014)
#define NWKC_VENDOR_STRING               "ATMEL  "          /* 7 characters ! */



/* === Externals ============================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NWK_CONFIG_H */
/* EOF */
