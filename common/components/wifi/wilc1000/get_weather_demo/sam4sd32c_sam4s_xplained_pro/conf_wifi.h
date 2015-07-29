/**
 *
 * \file
 *
 * \brief WiFi Settings.
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

#ifndef CONF_WIFI_H_INCLUDED
#define CONF_WIFI_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "board.h"
#include "osprintf.h"
#include <FreeRTOSConfig.h>

/*
   ---------------------------------
   --------- WiFi Config  ----------
   ---------------------------------
*/

// Select WILC firmware to use.
#define CONF_WILC_USE_REV_A
//#define CONF_WILC_USE_REV_B

// SSID and pass-phrase of the network to connect to.
#define CONF_WLAN_SSID					"ATSC24TIBO"
#define CONF_WLAN_AUTH					M2M_WIFI_SEC_WPA_PSK
#define	CONF_WLAN_PSK					"atmel123"

// MAC address of the WINC.
#define CONF_WIFI_MAC_ADDRESS			{0xaa, 0xff, 0xee, 0x00, 0x0f, 0xb8}

// The IP address used if DHCP is not used.
#define CONF_IP_ADDR0					192
#define CONF_IP_ADDR1					168
#define CONF_IP_ADDR2					1
#define CONF_IP_ADDR3					20

// The gateway address if DHCP is not used.
#define CONF_GATEWAY_ADDR0				192
#define CONF_GATEWAY_ADDR1				168
#define CONF_GATEWAY_ADDR2				1
#define CONF_GATEWAY_ADDR3				11

// The network mask  if DHCP is not used.
#define CONF_NET_MASK0					255
#define CONF_NET_MASK1					255
#define CONF_NET_MASK2					255
#define CONF_NET_MASK3					0

/*
   ---------------------------------
   ---------- PIN settings ---------
   ---------------------------------
*/

#define CONF_WIFI_PIN_RESET				IOPORT_CREATE_PIN(PIOA, 24)
#define CONF_WIFI_PIN_CHIP_ENABLE		IOPORT_CREATE_PIN(PIOA, 6)
#define CONF_WIFI_PIN_WAKE				IOPORT_CREATE_PIN(PIOA, 25)

/*
   ---------------------------------
   ---------- SPI settings ---------
   ---------------------------------
*/

#define CONF_WIFI_USE_SPI				(1)

/** SPI pin and instance settings. */
#define CONF_WIFI_SPI					SPI
#define CONF_WIFI_SPI_ID				ID_SPI
#define CONF_WIFI_SPI_MISO_GPIO			SPI_MISO_GPIO
#define CONF_WIFI_SPI_MISO_FLAGS		SPI_MISO_FLAGS
#define CONF_WIFI_SPI_MOSI_GPIO			SPI_MOSI_GPIO
#define CONF_WIFI_SPI_MOSI_FLAGS		SPI_MOSI_FLAGS
#define CONF_WIFI_SPI_CLK_GPIO			SPI_SPCK_GPIO
#define CONF_WIFI_SPI_CLK_FLAGS			SPI_SPCK_FLAGS
#define CONF_WIFI_SPI_CS_GPIO			SPI_NPCS0_GPIO
#define CONF_WIFI_SPI_CS_FLAGS			PIO_OUTPUT_1
#define CONF_WIFI_SPI_NPCS				(0)

/** SPI delay before SPCK and between consecutive transfer. */
#define CONF_WIFI_SPI_DLYBS				(0)
#define CONF_WIFI_SPI_DLYBCT 			(0)

/** SPI interrupt pin. */
#define CONF_WIFI_SPI_INT_PIN			IOPORT_CREATE_PIN(PIOA, 1)
#define CONF_WIFI_SPI_INT_PIO			PIOA
#define CONF_WIFI_SPI_INT_PIO_ID		ID_PIOA
#define CONF_WIFI_SPI_INT_MASK			(1 << 1)
/** Highest INT priority that can call FreeRTOS FromISR() functions. */
#define CONF_WIFI_SPI_INT_PRIORITY		(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY)

/** Clock polarity & phase. */
#define CONF_WIFI_SPI_POL				(0)
#define CONF_WIFI_SPI_PHA				(1)

/** SPI clock. */
#define CONF_WIFI_SPI_CLOCK				(48000000)

/*
   ---------------------------------
   ----------- OS options ----------
   ---------------------------------
*/

#define CONF_DRIVER_LOCK				1
#if CONF_DRIVER_LOCK
#  include <os/include/os_hook.h>
#  define DRIVER_SIGNAL_ISR				os_hook_isr
#else
#  define DRIVER_SIGNAL_ISR()
#endif

/*
   ---------------------------------
   --------- Debug options ---------
   ---------------------------------
*/

#define CONF_WIFI_DEBUG					(0)
#define CONF_WIFI_PRINTF				osprintf

#ifdef __cplusplus
}
#endif

#endif /* CONF_WIFI_H_INCLUDED */
