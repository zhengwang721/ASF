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
#ifndef M2M_WIFI_EX_H_INCLUDED
#define M2M_WIFI_EX_H_INCLUDED
#include <driver/include/m2m_wifi.h>

typedef void (*m2m_wifi_callback_t)(void *arg);

sint8 os_m2m_wifi_init(tstrWifiInitParam *param);
sint8 os_m2m_wifi_connect(char *pcSsid, uint8 u8SsidLen, uint8 u8SecType, void *pvAuthInfo, uint16 u16Ch);
void os_m2m_wifi_disconnect(void);
sint8 os_m2m_wifi_enable_ap(tstrM2MAPConfig *ap);
void os_m2m_wifi_request_scan(uint8 p);
void os_m2m_wifi_req_scan_result(uint8 p);
sint8 m2m_wifi_request_callback_ex(m2m_wifi_callback_t callback, void *arg);
sint8 m2m_wifi_request_dhcp_client_ex(void);
sint8 m2m_wifi_enable_ap_ex(CONST tstrM2MAPConfig* pstrM2MAPConfig);
sint8 m2m_wifi_disable_ap_ex(void);
sint8 m2m_wifi_set_mac_address_ex(uint8 addr[6]);

// Macros to alias all other APIs to the _ex versions.
#define m2m_wifi_download_mode_ex			m2m_wifi_download_mode
#define m2m_wifi_deinit_ex					m2m_wifi_deinit
#define m2m_wifi_handle_events_ex			m2m_wifi_handle_events
#define m2m_wifi_default_connect_ex			m2m_wifi_default_connect
#define m2m_wifi_disconnect_ex				m2m_wifi_disconnect
#define m2m_wifi_start_provision_mode_ex	m2m_wifi_start_provision_mode
#define m2m_wifi_stop_provision_mode_ex		m2m_wifi_stop_provision_mode
#define m2m_wifi_get_connection_info_ex		m2m_wifi_get_connection_info
#define m2m_wifi_wps_ex						m2m_wifi_wps
#define m2m_wifi_wps_disable_ex				m2m_wifi_wps_disable 
#define m2m_wifi_p2p_ex						m2m_wifi_p2p
#define m2m_wifi_set_static_ip_ex			m2m_wifi_set_static_ip
#define m2m_wifi_request_dhcp_server_ex		m2m_wifi_request_dhcp_server
#define m2m_wifi_set_scan_options_ex		m2m_wifi_set_scan_options
#define m2m_wifi_set_scan_region_ex			m2m_wifi_set_scan_region
#define m2m_wifi_get_num_ap_found_ex		m2m_wifi_get_num_ap_found
#define m2m_wifi_req_curr_rssi_ex			m2m_wifi_req_curr_rssi
#define m2m_wifi_get_otp_mac_address_ex		m2m_wifi_get_otp_mac_address
#define m2m_wifi_get_mac_address_ex			m2m_wifi_get_mac_address
#define m2m_wifi_set_sleep_mode_ex			m2m_wifi_set_sleep_mode
#define m2m_wifi_request_sleep_ex			m2m_wifi_request_sleep
#define m2m_wifi_get_sleep_mode_ex			m2m_wifi_get_sleep_mode
#define m2m_wifi_req_client_ctrl_ex			m2m_wifi_req_client_ctrl
#define m2m_wifi_req_server_init_ex			m2m_wifi_req_server_init
#define m2m_wifi_set_device_name_ex			m2m_wifi_set_device_name
#define m2m_wifi_set_lsn_int_ex				m2m_wifi_set_lsn_int
#define m2m_wifi_enable_monitoring_mode_ex	m2m_wifi_enable_monitoring_mode
#define m2m_wifi_disable_monitoring_mode_ex	m2m_wifi_disable_monitoring_mode
#define m2m_wifi_send_wlan_pkt_ex			m2m_wifi_send_wlan_pkt
#define m2m_wifi_send_ethernet_pkt_ex		m2m_wifi_send_ethernet_pkt
#define m2m_wifi_enable_sntp_ex				m2m_wifi_enable_sntp
#define m2m_wifi_set_sytem_time_ex			m2m_wifi_set_sytem_time
#define m2m_wifi_set_cust_InfoElement_ex	m2m_wifi_set_cust_InfoElement
#define m2m_wifi_enable_mac_mcast_ex		m2m_wifi_enable_mac_mcast
#define m2m_wifi_set_receive_buffer_ex		m2m_wifi_set_receive_buffer

#endif /* M2M_WIFI_EX_H_INCLUDED */
