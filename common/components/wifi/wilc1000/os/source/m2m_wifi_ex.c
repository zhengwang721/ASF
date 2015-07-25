/**
 *
 * \file
 *
 * \brief Wireless Link Controller Driver.
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

#include <lwip/opt.h>
#include <lwip/tcpip.h>
#include "os/include/m2m_wifi_ex.h"
#include "os/include/net_init.h"
#include "delay.h"

extern void winc_fill_callback_info(tstrEthInitParam *info);
extern uint8_t *winc_get_mac_addr(void);
extern void winc_set_mac_addr(uint8_t *addr);

sint8 m2m_wifi_set_mac_address_ex(uint8 addr[6])
{
	winc_set_mac_addr(addr);
	return M2M_SUCCESS;
}

struct init_params {
	struct params_dispatch dispatch;
	tstrWifiInitParam *init;
};

static void os_m2m_wifi_init_imp(struct init_params *p)
{
	uint8_t self[] = CONF_WIFI_MAC_ADDRESS;

	/* Save tcpip task handle. */
	os_hook_set_handle(xTaskGetCurrentTaskHandle());

	/* Init low level. */
	nm_bsp_init();

	/* Set MAC address. */
	m2m_wifi_set_mac_address_ex(self);

	/* Register lwIP low level driver hook. */
	winc_fill_callback_info(&p->init->strEthInitParam);
	
	/* Init WILC1000 driver. */
	p->dispatch.retval = m2m_wifi_init(p->init);
	if (M2M_SUCCESS == p->dispatch.retval) {
		m2m_wifi_set_mac_address(winc_get_mac_addr());
		net_add_winc_netif();
	}

	if (p->dispatch.signal_semaphore) {
		os_hook_notify();
	}
}

sint8 os_m2m_wifi_init(tstrWifiInitParam *param)
{
	struct init_params params;
	params.init = param;
	params.dispatch.retval = M2M_ERR_TIME_OUT;

	os_hook_init();
	os_hook_dispatch_wait((tcpip_callback_fn)os_m2m_wifi_init_imp, &params.dispatch);
	return params.dispatch.retval;
}

struct connect_params {
	struct params_dispatch dispatch;
	char *pcSsid;
	uint8 u8SsidLen;
	uint8 u8SecType;
	void *pvAuthInfo;
	uint16 u16Ch;
};

static void os_m2m_wifi_connect_imp(struct connect_params *p)
{
	m2m_wifi_set_control_ifc(1);
	p->dispatch.retval = m2m_wifi_connect(p->pcSsid, p->u8SsidLen, p->u8SecType, p->pvAuthInfo, p->u16Ch);
	if (p->dispatch.signal_semaphore) {
		os_hook_notify();
	}
}

sint8 os_m2m_wifi_connect(char *pcSsid, uint8 u8SsidLen, uint8 u8SecType, void *pvAuthInfo, uint16 u16Ch)
{
	struct connect_params params;
	params.pcSsid = pcSsid;
	params.u8SsidLen = u8SsidLen;
	params.u8SecType = u8SecType;
	params.pvAuthInfo = pvAuthInfo;
	params.u16Ch = u16Ch;
	params.dispatch.retval = M2M_ERR_TIME_OUT;

	os_hook_dispatch_wait((tcpip_callback_fn)os_m2m_wifi_connect_imp, &params.dispatch);
	delay_ms(200);
	return params.dispatch.retval;
}

static void os_m2m_wifi_disconnect_imp(void)
{
	m2m_wifi_disconnect();
}

void os_m2m_wifi_disconnect(void)
{
	os_hook_dispatch_no_wait((tcpip_callback_fn)os_m2m_wifi_disconnect_imp, 0);
}

struct connect_ap_params {
	struct params_dispatch dispatch;
	tstrM2MAPConfig *ap;
};

static void os_m2m_wifi_enable_ap_imp(struct connect_ap_params *p)
{	
	m2m_wifi_set_control_ifc(2);
	p->dispatch.retval = m2m_wifi_enable_ap(p->ap);
	if (p->dispatch.signal_semaphore) {
		os_hook_notify();
	}
}

sint8 os_m2m_wifi_enable_ap(tstrM2MAPConfig *ap)
{
	struct connect_ap_params params;
	params.ap = ap;
	params.dispatch.retval = M2M_ERR_TIME_OUT;

	os_hook_dispatch_wait((tcpip_callback_fn)os_m2m_wifi_enable_ap_imp, &params.dispatch);
	if (!params.dispatch.retval) {
		net_interface_up(NET_IF_C);
	}
	delay_ms(200);
	return params.dispatch.retval;
}

static void os_m2m_wifi_request_scan_imp(uint8 *p)
{
	m2m_wifi_request_scan((uint8)p);
}

void os_m2m_wifi_request_scan(uint8 p)
{
	os_hook_dispatch_no_wait((tcpip_callback_fn)os_m2m_wifi_request_scan_imp, (void *)p);
}

static void os_m2m_wifi_req_scan_result_imp(uint8 *p)
{
	m2m_wifi_req_scan_result((uint8)p);
}

void os_m2m_wifi_req_scan_result(uint8 p)
{
	os_hook_dispatch_no_wait((tcpip_callback_fn)os_m2m_wifi_req_scan_result_imp, (void *)p);
}

sint8 m2m_wifi_request_callback_ex(m2m_wifi_callback_t callback, void *arg)
{
	return tcpip_callback_with_block(callback, arg, 0);
}

sint8 m2m_wifi_request_dhcp_client_ex(void)
{
	net_set_mode(NET_IF_STA, NET_MODE_USE_DHCP);
	return 0;
}

sint8 m2m_wifi_enable_ap_ex(CONST tstrM2MAPConfig* pstrM2MAPConfig)
{
	sint8 rc;
	rc = m2m_wifi_enable_ap(pstrM2MAPConfig);
	if (!rc) {
		net_interface_up(NET_IF_C);
	}
	return rc;
}

sint8 m2m_wifi_disable_ap_ex(void)
{
	net_set_mode(NET_IF_C, 0);
	return m2m_wifi_disable_ap();	
}
