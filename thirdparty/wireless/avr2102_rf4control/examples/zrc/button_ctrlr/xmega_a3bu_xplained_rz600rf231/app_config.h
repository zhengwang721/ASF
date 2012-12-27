/**
 * @file app_config.h
 *
 * @brief These are application-specific resources which are used
 *        in the example application of the coordinator in addition to the
 *        underlaying stack.
 *
 * $Id: app_config.h 33178 2012-09-21 05:29:13Z agasthian.s $
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
#include "zrc.h"

/* === Macros =============================================================== */


/* === Types ================================================================ */


/* Offset of IEEE address storage location within EEPROM */
#define EE_IEEE_ADDR                (0)

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


/*
 * USB-specific definitions
 */

/*
 * USB Vendor ID (16-bit number)
 */
#define USB_VID                 (0x03EB) /* Atmel's USB vendor ID */

/*
 * USB Product ID (16-bit number)
 */
#define USB_PID                 (0x2018) /* RZ USB stick product ID */

/*
 * USB Release number (BCD format, two bytes)
 */
#define USB_RELEASE             { 0x00, 0x01 } /* 01.00 */

/*
 * Maximal number of UTF-16 characters used in any of the strings
 * below.  This is only used for compilers that cannot handle the
 * initialization of flexible array members within structs.
 */
#define USB_STRING_SIZE         (10)

/*
 * String representation for the USB vendor name.
 */
#define USB_VENDOR_NAME L"ATMEL"

/*
 * String representation for the USB product name.
 */
#define USB_PRODUCT_NAME L"RZUSBSTICK"

/**
 * Defines the USB transmit buffer size
 */
#define USB_TX_BUF_SIZE             (200)    //@ToDo size? same as SIO_TX_BUF_SIZE

/**
 * Defines the USB receive buffer size
 */
#define USB_RX_BUF_SIZE             (200)

/**
 * Defines the UART transmit buffer size
 */
#define UART_MAX_TX_BUF_LENGTH      (200)        //@ToDo size? same as SIO_TX_BUF_SIZE

/**
 * Defines the UART receive buffer size
 */
#define UART_MAX_RX_BUF_LENGTH      (200)

#define APP_USER_STRING                 "My Controller  "    /* 15 characters ! */
#define USER_STRING_AVAILABLE           true
#define NUM_SUPPORTED_DEV_TYPES         (1)
#define NUM_SUPPORTED_PROFILES          (1)
#if (USER_STRING_AVAILABLE == true)
#define APP_CAPABILITIES                (0x01 | (NUM_SUPPORTED_DEV_TYPES << 1) | (NUM_SUPPORTED_PROFILES << 4))
#else
#define APP_CAPABILITIES                ((NUM_SUPPORTED_DEV_TYPES << 1) | (NUM_SUPPORTED_PROFILES << 4))
#endif
#define SUPPORTED_CEC_CMDS          {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, \
        0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, \
        0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, \
        0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}

/* Define the periodic wakeup interval */
#define WDT_WAKEUP_INTERVAL         WDT_TO_1S
#define WDT_WAKEUP_INTERVAL_US      (1000000) /* micro seconds */
/* Define the watchdog timeout for system reset */
#define WDT_TIMEOUT_PERIOD          WDT_TO_4S



#define BOOT_LOADER_SIZE 0

#define IEEE_FLASH_OFFSET (STACK_FLASH_START_ADDR - 8)

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
