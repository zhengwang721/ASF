/**
 * \file
 *
 * \brief USB Device wrapper layer for compliance with common driver UHD
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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
#include <string.h>
#include <stdlib.h>

/* Get USB host configuration */
#include "conf_usb_device.h"
#include "udd.h"
#include "usb.h"

// Check USB host configuration
#ifdef USB_DEVICE_HS_SUPPORT
#  error The High speed mode is not supported on this part, please remove USB_DEVICE_HS_SUPPORT in conf_usb_device.h
#endif

#if !(SAMD21)
# error The current USB Device Driver supports only SAMD21
#endif

static void _usb_on_vbus_change(dev)
{
	//UDC_VBUS_EVENT(usb_device_is_vbus_available(dev));
}

static void _usb_on_sof_notify(dev)
{
	//UDC_SOF_EVENT();
}

static void _usb_on_suspend(dev)
{
	//UDC_SUSPEND_EVENT();
}

static void _usb_on_wakeup(dev)
{
	//UDC_RESUME_EVENT();
}

static void _usb_on_lpm(dev)
{
	//UDC_SUSPEND_EVENT();
}

void udd_enable(void)
{
	usb_device_init(dev,cfg);
	usb_device_enable(dev);
	usb_device_register_callback(dev,0,_usb_on_vbus_change);
	usb_device_register_callback(dev,1,_usb_on_sof_notify);
	usb_device_register_callback(dev,2,_usb_on_bus_reset);
	usb_device_register_callback(dev,3,_usb_on_suspend);
	usb_device_register_callback(dev,4,_usb_on_wakeup);
	usb_device_register_callback(dev,5,_usb_on_lpm);
	usb_device_register_callback(dev,6,_usb_on_error);
	usb_device_enable_callback(dev,0);
	usb_device_enable_callback(dev,1);
	usb_device_enable_callback(dev,2);
	usb_device_enable_callback(dev,3);
	usb_device_enable_callback(dev,4);
	usb_device_enable_callback(dev,5);
	usb_device_enable_callback(dev,6);
}

void udd_disable(void)
{
	usb_device_disable(dev);
	usb_device_disable_callback(dev,0);
	usb_device_disable_callback(dev,1);
	usb_device_disable_callback(dev,2);
	usb_device_disable_callback(dev,3);
	usb_device_disable_callback(dev,4);
	usb_device_disable_callback(dev,5);
	usb_device_disable_callback(dev,6);
}

void udd_attach(void)
{
	usb_device_attach(dev);
}

void udd_detach(void)
{
	usb_device_detach(dev);
}

void udd_is_high_speed(void)
{
	return (USB_SPEED_HIGH == usb_device_get_speed(dev));
}

void udd_set_address(uint8_t address)
{
	usb_device_set_address(dev,address);
}

uint8_t udd_getaddress(void)
{
	return usb_device_get_address(dev);
}

uint16_t udd_get_frame_number(void)
{
	return usb_device_get_frame_number(dev);
}

uint16_t udd_get_micro_frame_number(void)
{
	return usb_device_get_micro_frame_number(dev);
}

void udd_send_remotewakeup(void)
{
	usb_device_send_remote_wakeup(dev);
}

uint8_t *ctrl_payload;
uint16_t ctrl_size;
void udd_set_setup_payload(uint8_t *payload, uint16_t payload_size)
{
	ctrl_payload = payload;
	ctrl_size = payload_size;
	if (request.dir) usb_device_ep_write_job();
	else             usb_device_ep_read_job();
}

bool udd_ep_alloc(ep,bmAttributes,MaxEndpointSize)
{
	usb_device_ep_config_get_defaults(cfg);
	// Modify cfg
	usb_device_ep_init(dev,ep,cfg);
	usb_device_ep_enable(dev,ep);
}

void udd_ep_free(ep)
{
	usb_device_ep_disable(dev,ep);
}

bool udd_ep_is_halted(ep)
{
	return usb_device_ep_is_halted(dev,ep);
}

bool udd_ep_set_halt(udd_ep_id_t ep)
{
	usb_device_ep_set_halt(dev,ep);
}

bool udd_ep_clear_halt(udd_ep_id_t ep)
{
	usb_device_ep_clear_halt(dev,ep);
}

void _udd_ep_on_stall_change(dev,ep)
{
	if (!usb_device_ep_is_halted(dev,ep))
		job[ep].halt_clear_callback();
}
bool udd_ep_wait_stall_clear(udd_ep_id_t ep,
		udd_callback_halt_cleared_t callback)
{
	usb_device_ep_register_callback(dev,ep,STALL_CHANGE,_udd_ep_on_stall_change);
	usb_device_ep_enable_callback(dev,ep,STALL_CHANGE);
	job[ep].halt_clear_callback() = callback;
}

void _udd_ep_on_in(dev,ep)
{
	job[ep].callback();
}
void _udd_ep_on_out(dev,ep)
{
	job[ep].callback();
}
bool udd_ep_run(udd_ep_id_t ep, bool b_shortpacket,
		uint8_t * buf, iram_size_t buf_size,
		udd_callback_trans_t callback)
{
	...
	if (b_shortpacket) {
		usb_device_ep_enable_short_packet();
	} else {
		usb_device_ep_disable_short_packet();
	}
	...
	job[ep].callback = callback;
	if (ep & 0x80) {
		usb_device_ep_register_callback(dev,ep,IN,_udd_ep_on_in);
		usb_device_ep_enable_callback(dev,ep,IN);
		usb_device_ep_write_job(dev,ep,buf,buf_size);
	} else {
		usb_device_ep_register_callback(dev,ep,OUT,_udd_ep_on_out);
		usb_device_ep_enable_callback(dev,ep,OUT);
		usb_device_ep_read_job(dev,ep,buf,buf_size);
	}
	return ...;
}

void udd_ep_abort(udd_ep_id_t ep)
{
	usb_device_ep_abort_job(ep);
}

