/**
 * \file
 *
 * \brief PubNub Example.
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

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/** Use DEMO_ENABLE_WEB_PROVISIONING macro to enable/disable Wi-Fi web provisioning. */
#define DEMO_ENABLE_WEB_PROVISIONING

/** Provisioning method 1: Access point configuration. */
#define DEMO_WLAN_AP_NAME                               "WINC1500_00:00"
#define DEMO_WLAN_AP_CHANNEL                            1
#define DEMO_WLAN_AP_WEP_INDEX                          0
#define DEMO_WLAN_AP_WEP_SIZE                           WEP_40_KEY_STRING_SIZE
#define DEMO_WLAN_AP_WEP_KEY                            "1234567890"
#define DEMO_WLAN_AP_SECURITY                           M2M_WIFI_SEC_OPEN
#define DEMO_WLAN_AP_MODE                               SSID_MODE_VISIBLE
#define DEMO_WLAN_AP_DOMAIN_NAME                        "atmelconfig.com"
#define DEMO_WLAN_AP_IP_ADDRESS                         {192, 168, 1, 1}
/** Provisioning method 2: SSID and pass phrase of the AP to connect to. */
#define DEMO_WLAN_SSID                                  "ATSC24TIBO"
#define DEMO_WLAN_PSK                                   "atmel123"
#define DEMO_WLAN_AUTH                                  M2M_WIFI_SEC_WPA_PSK

/** PubNub Settings. */
#define DEMO_PUBNUB_PUBLISH_KEY                         "demo"
#define DEMO_PUBNUB_SUBSCRIBE_KEY                       "demo"
#define DEMO_PUBNUB_CHANNEL						        "WINC1500_00:00"
#define DEMO_PUBNUB_PUBLISH_INTERVAL                    (3000)
#define DEMO_PUBNUB_SUBSCRIBE_INTERVAL                  (1000)

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H_INCLUDED */
