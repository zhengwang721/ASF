/**
 *
 * \file
 *
 * \brief
 *
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
 *
 */

#include "asf.h"
#include <string.h>
#include "os/include/sm_wifi.h"
#include "os/include/net_init.h"
#include "os/include/m2m_wifi_ex.h"
#include "osprintf.h"

static int wifi_mode;

static void connect_to_ap(void)
{
	m2m_wifi_connect((char *)CONF_WLAN_SSID, sizeof(CONF_WLAN_SSID),
			CONF_WLAN_AUTH, (char *)CONF_WLAN_PSK, M2M_WIFI_CH_ALL);
}

static void start_ap(void)
{
	tstrM2MAPConfig cfg;
	memset(&cfg, 0, sizeof(cfg));
	strcpy((char *)cfg.au8SSID, "WINC-SOFTAP");
	cfg.u8ListenChannel = 11;
	cfg.u8SecType = M2M_WIFI_SEC_OPEN;
	m2m_wifi_enable_ap_ex(&cfg);
}

static void sm_wifi_init_callback(void *arg)
{

	if (wifi_mode == WIFI_MODE_AP) {
		start_ap();
	} else {
		connect_to_ap();
	}
}

void sm_wifi_init(int mode, void *arg)
{
	wifi_mode = mode;
	m2m_wifi_request_callback_ex(sm_wifi_init_callback, arg);
}
