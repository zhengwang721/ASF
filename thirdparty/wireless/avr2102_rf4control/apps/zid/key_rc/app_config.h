/**
 * @file app_config.h
 *
 * @brief Single Button Controller application config
 *
 * $Id: app_config.h 33908 2013-01-10 04:57:43Z agasthian.s $
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
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* === Includes ============================================================= */

#include "stack_config.h"
#include "tal_timer_config.h"
#include "mac_config.h"
#include "node_config.h"
#include "rf4ce.h"
#include "zid.h"
#include "common_nvm.h"
#include "conf_nvm.h"

/* === Macros =============================================================== */

/** @brief This is the first timer identifier of the application.
 *
 *  The value of this identifier is an increment of the largest identifier
 *  value used by the MAC.
 */
#if (NUMBER_OF_TOTAL_STACK_TIMERS == 0)
#define APP_FIRST_TIMER_ID          (0)
#else
#define APP_FIRST_TIMER_ID          (LAST_STACK_TIMER_ID + 1)
#endif

/* === Types ================================================================ */

/** Timer ID's used by the Application */
typedef enum
{
    /* App Timers start from APP_FIRST_TIMER_ID */

    /** Application timer id used to switch off LED */
    T_LED_TIMER     = (APP_FIRST_TIMER_ID)
} app_timer_t;

/** Defines the number of timers used by the application. */
/** Defines the number of timers used by the application. */
#define NUMBER_OF_APP_TIMERS        (1)

#define TOTAL_STACK_TIMERS (NUMBER_OF_TAL_TIMERS + NUMBER_OF_MAC_TIMERS + NUMBER_OF_NWK_TIMERS)

/** Defines the total number of timers used by the application and the layers below. */
#define TOTAL_NUMBER_OF_TIMERS      (NUMBER_OF_APP_TIMERS + TOTAL_STACK_TIMERS)

/** Defines the number of additional large buffers used by the application */
#define NUMBER_OF_LARGE_APP_BUFS    (0)

/** Defines the number of additional small buffers used by the application */
#define NUMBER_OF_SMALL_APP_BUFS    (0)
/**
 *  Defines the total number of large buffers used by the application and the
 *  layers below.
 */
#define TOTAL_NUMBER_OF_LARGE_BUFS  (NUMBER_OF_LARGE_APP_BUFS + NUMBER_OF_LARGE_STACK_BUFS)

/**
 *  Defines the total number of small buffers used by the application and the
 *  layers below.
 */
#define TOTAL_NUMBER_OF_SMALL_BUFS  (NUMBER_OF_SMALL_APP_BUFS + NUMBER_OF_SMALL_STACK_BUFS)

#define TOTAL_NUMBER_OF_BUFS        (TOTAL_NUMBER_OF_LARGE_BUFS + TOTAL_NUMBER_OF_SMALL_BUFS)





/* Offset of IEEE address storage location within EEPROM */
#define EE_IEEE_ADDR                (0)

/* Configure RF4CE node capabilities */
/* The capabilities of this node. Implementation specific according to the format
 * illustrated in Figure 26.
 * Implementation specific
 * NodeTypeTarget
 * NodeTypeController
 * PowerSourceMainsTrue
 * PowerSourceMainsFalse
 * SecurityCapableTrue
 * SecurityCapableFalse
 * ChannelNormalizationCapableTrue
 * ChannelNormalizationCapableFalse    */

#define APP_USER_STRING             "Single Btn Ctrl"    /* 15 characters ! */
#define USER_STRING_AVAILABLE       true
#define NUM_SUPPORTED_DEV_TYPES         (1)
#define NUM_SUPPORTED_PROFILES          (1)
#define SUPPORTED_DEV_TYPE_0        DEV_TYPE_REMOTE_CONTROL
#define SUPPORTED_DEV_TYPE_1        DEV_TYPE_REMOTE_CONTROL
#define SUPPORTED_DEV_TYPE_2        DEV_TYPE_REMOTE_CONTROL
#define SUPPORTED_PROFILE_ID_0      PROFILE_ID_ZID
#define SUPPORTED_PROFILE_ID_1      PROFILE_ID_ZID
#define SUPPORTED_PROFILE_ID_2      PROFILE_ID_ZID
#define SUPPORTED_PROFILE_ID_3      PROFILE_ID_ZID
#define SUPPORTED_PROFILE_ID_4      PROFILE_ID_ZID
#define SUPPORTED_PROFILE_ID_5      PROFILE_ID_ZID
#define SUPPORTED_PROFILE_ID_6      PROFILE_ID_ZID
#if (USER_STRING_AVAILABLE == true)
#define APP_CAPABILITIES                (0x01 | (NUM_SUPPORTED_DEV_TYPES << 1) | (NUM_SUPPORTED_PROFILES << 4))
#else
#define APP_CAPABILITIES                ((NUM_SUPPORTED_DEV_TYPES << 1) | (NUM_SUPPORTED_PROFILES << 4))
#endif

/* Implementation specific ZRC constant defines */
#define FW_VERSION_MAJOR            1
#define FW_VERSION_MINOR            2
#define FW_VERSION_REV              0

#define APL_HID_NUM_DESCRIPTORS             HID_NUM_DESCRIPTORS
//#define APL_HID_DESCRIPTOR_SPEC             HID_DESCRIPTOR_SPEC

/* Define the periodic wakeup interval */
#define WDT_WAKEUP_INTERVAL         WDT_TO_1S
#define WDT_WAKEUP_INTERVAL_US      1000000 /* micro seconds */
/* Define the watchdog timeout for system reset */
#define WDT_TIMEOUT_PERIOD          WDT_TO_4S


/* Number of variables that are stored using NVM */
#define NVM_NUM_VARS                (NWKC_MAX_PAIRING_TABLE_ENTRIES + 1) // 1 = own frame counter
/* Size of the flash used for NVM; i.e. number of flash pages.
   For every variable two pages are used. */
#define NVM_MULTI_WRITE_NUM_PG_PER_VAR  2
/* Size of the multi write NVM flash section */
#define NVM_MULTI_WRITE_SIZE        (NVM_NUM_VARS * NVM_MULTI_WRITE_NUM_PG_PER_VAR * SPM_PAGESIZE)
/* Flash address where the NVM starts. For final application adjust accordingly. */
#define NVM_MULTI_WRITE_START       (NIB_FLASH_ADDR - NVM_MULTI_WRITE_SIZE)

/* === Externals ============================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_CONFIG_H */
/* EOF */
