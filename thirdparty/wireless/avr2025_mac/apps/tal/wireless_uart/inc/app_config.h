/**
 * \file app_config.h
 *
 * \brief These are application-specific resources which are used
 *        in the application in addition to the underlaying stack.
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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

/* === Macros =============================================================== */

#if (NUMBER_OF_TOTAL_STACK_TIMERS == 0)
#define APP_FIRST_TIMER_ID          (0)
#else
#define APP_FIRST_TIMER_ID          (LAST_STACK_TIMER_ID + 1)
#endif

/* === Types ================================================================ */

/* Timer ID's used by the Application */

uint8_t T_APP_TX_DELAY;
uint8_t T_APP_LED_TX;
uint8_t T_APP_LED_RX;


#define NUMBER_OF_APP_TIMERS        (3)

#define TOTAL_NUMBER_OF_TIMERS      (NUMBER_OF_APP_TIMERS + NUMBER_OF_TOTAL_STACK_TIMERS)

/* Additional buffers used by the application */
#define NUMBER_OF_LARGE_APP_BUFS    (10)
#define NUMBER_OF_SMALL_APP_BUFS    (0)

#define TOTAL_NUMBER_OF_LARGE_BUFS  (NUMBER_OF_LARGE_APP_BUFS + NUMBER_OF_LARGE_STACK_BUFS)
#define TOTAL_NUMBER_OF_SMALL_BUFS  (NUMBER_OF_SMALL_APP_BUFS + NUMBER_OF_SMALL_STACK_BUFS)

#define TOTAL_NUMBER_OF_BUFS        (TOTAL_NUMBER_OF_LARGE_BUFS + TOTAL_NUMBER_OF_SMALL_BUFS)

/*
 * USB transmit buffer size
 */
#define USB_TX_BUF_SIZE             (64)

/*
 * USB receive buffer size
 */
#define USB_RX_BUF_SIZE             (64)

/*
 * USB-specific definitions
 */

/*
 * USB Vendor ID (16-bit number)
 */
#define USB_VID                 0x03EB /* Atmel's USB vendor ID */

/*
 * USB Product ID (16-bit number)
 */
#ifdef RF233_SAM4_BAB_NATIVE_USB
#if (BOARD_TYPE == RF233_SAM4_BAB_NATIVE_USB) && (!defined(USB_PID))
#error "Define an USB PIB in the Makefile"
#endif
#endif

/** CDC IN endpoint address.
 *  Endpoint address (host view) for CDC bulk out transfers from host to device.
 */
#if !defined(USB_EP_CDC_OUT)
#define USB_EP_CDC_OUT          0x01
#endif

/** CDC OUT endpoint address.
 *  Endpoint address (host view) for CDC bulk out transfers from device to host.
 */
#if !defined(USB_EP_CDC_IN)
#define USB_EP_CDC_IN           0x02
#endif

/** CDC IRQ endpoint address.
 *  Endpoint address for CDC interrupt transfers from device to host.
 */
#if !defined(USB_EP_CDC_IRQ)
#define USB_EP_CDC_IRQ          0x03
#endif

/*
 * USB Release number (BCD format, two bytes)
 */
#define USB_RELEASE             { 0x00, 0x01 } /* 01.00 */

/*
 * Maximal number of UTF-16 characters used in any of the strings
 * below.  This is only used for compilers that cannot handle the
 * initialization of flexible array members within structs.
 */
#define USB_STRING_SIZE         10

/*
 * String representation for the USB vendor name.
 */
#define USB_VENDOR_NAME L"ATMEL"

/*
 * String representation for the USB product name.
 */
#define USB_PRODUCT_NAME L"RZUSBSTICK"

/*
 * USB Flushed data size
 */
#define NB_MS_BEFORE_FLUSH           (50)

/*
 * UART transmit buffer size
 */
#define UART_MAX_TX_BUF_LENGTH      (10)

/*
 * UART receive buffer size
 */
#define UART_MAX_RX_BUF_LENGTH      (10)

/* Offset of IEEE address storage location within EEPROM */
#define EE_IEEE_ADDR                (0)

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
