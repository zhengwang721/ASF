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
#include "conf_usb.h"
#include "udd.h"
#include "usb.h"

// Check USB host configuration
#ifdef USB_DEVICE_HS_SUPPORT
#  error The High speed mode is not supported on this part, please remove USB_DEVICE_HS_SUPPORT in conf_usb_device.h
#endif

#if !(SAMD21)
# error The current USB Device Driver supports only SAMD21
#endif


uint8_t udd_ep0_buffer[64];
struct usb_module usb_device;
udd_ctrl_request_t udd_g_ctrlreq;

void udd_ep_abort(udd_ep_id_t ep)
{
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;
	usb_device_endpoint_abort_job(&usb_device,ep_num);
}


bool udd_ep_alloc(udd_ep_id_t ep, uint8_t bmAttributes, uint16_t MaxEndpointSize)
{
	 
	struct usb_device_endpoint_config config_ep;
	uint8_t tran_type;
	 
	usb_device_endpoint_get_config_defaults(&config_ep);
	config_ep.ep_address = ep;
	tran_type = (enum usb_ep_type)(bmAttributes & 0x03);
	 
	if (USB_EP_TYPE_CONTROL == tran_type) {
		config_ep.ep_type = USB_DEVICE_ENDPOINT_TYPE_CONTROL;
	} else if (USB_EP_TYPE_ISOCHRONOUS == tran_type) {
		config_ep.ep_type = USB_DEVICE_ENDPOINT_TYPE_ISOCHRONOUS;
	} else if (USB_EP_TYPE_BULK == tran_type) {
		config_ep.ep_type = USB_DEVICE_ENDPOINT_TYPE_BULK;
	} else if (USB_EP_TYPE_INTERRUPT == tran_type) {
		config_ep.ep_type = USB_DEVICE_ENDPOINT_TYPE_INTERRUPT;
	}

	 if (MaxEndpointSize < 8) {
		 return false;
	 } else if (MaxEndpointSize >=8 && MaxEndpointSize < 16) {
		 config_ep.ep_size = USB_ENDPOINT_8_BYTE;
	 } else if (MaxEndpointSize >=16 && MaxEndpointSize < 32) {
		 config_ep.ep_size = USB_ENDPOINT_16_BYTE;
	 } else if (MaxEndpointSize >=32 && MaxEndpointSize < 64) {
		 config_ep.ep_size = USB_ENDPOINT_32_BYTE;
	 } else if (MaxEndpointSize >=64 && MaxEndpointSize < 128) {
		 config_ep.ep_size = USB_ENDPOINT_64_BYTE;
	 } else if (MaxEndpointSize >=128 && MaxEndpointSize < 256) {
		 config_ep.ep_size = USB_ENDPOINT_128_BYTE;
	 } else if (MaxEndpointSize >=256 && MaxEndpointSize < 512) {
		 config_ep.ep_size = USB_ENDPOINT_256_BYTE;
	 } else if (MaxEndpointSize >=512 && MaxEndpointSize < 1023) {
		 config_ep.ep_size = USB_ENDPOINT_512_BYTE;
	 } else if (MaxEndpointSize >=1023) {
		 config_ep.ep_size = USB_ENDPOINT_1023_BYTE;
	 } 
	 
	 usb_device_endpoint_set_config(&usb_device,&config_ep);
	 return true;
}

void udd_ep_free(udd_ep_id_t ep)
{
	;
}

void udd_set_address(uint8_t address)
{
	usb_device_set_address(&usb_device,address);
}

uint8_t udd_getaddress(void)
{
	return usb_device_get_address(&usb_device);
}

bool udd_ep_clear_halt(udd_ep_id_t ep)
{
	uint8_t ep_num;
	ep_num = ep & USB_EP_ADDR_MASK;
	usb_device_endpoint_clear_halt(&usb_device,ep_num);
	return true;
}

bool udd_ep_set_halt(udd_ep_id_t ep)
{
	uint8_t ep_num;
	ep_num = ep & USB_EP_ADDR_MASK;
	usb_device_endpoint_set_halt(&usb_device,ep_num);
	return true;
}

bool udd_ep_is_halted(udd_ep_id_t ep)
{
	uint8_t ep_num;
	ep_num = ep & USB_EP_ADDR_MASK;
	return usb_device_endpoint_is_halted(&usb_device,ep_num);
}

void udd_set_setup_payload( uint8_t *payload, uint16_t payload_size )
{
	;
}

static void _usb_ep_on_setup(struct usb_module *module_inst)
{
	udc_process_setup();
}

static void _usb_ep_on_tansfer_fail(struct usb_module *module_inst)
{
	;
}


static void _usb_ep_on_tansfer_ok(struct usb_module *module_inst)
{
	;
}

static void udd_ctrl_ep_enable(struct usb_module *module_inst)
{
	/* USB Device Endpoint0 Configuration */
	 struct usb_device_endpoint_config config_ep0;
	 
	 usb_device_endpoint_get_config_defaults(&config_ep0);
	 usb_device_endpoint_set_config(module_inst,&config_ep0);
	 
	 usb_device_endpoint_register_callback(module_inst,0,USB_DEVICE_ENDPOINT_CALLBACK_RXSTP, _usb_ep_on_setup );
	 usb_device_endpoint_register_callback(module_inst,0,USB_DEVICE_ENDPOINT_CALLBACK_TRCPT,_usb_ep_on_tansfer_ok );
	 usb_device_endpoint_register_callback(module_inst,0,USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL,_usb_ep_on_tansfer_fail );
	 usb_device_endpoint_enable_callback(module_inst,0,USB_DEVICE_ENDPOINT_CALLBACK_RXSTP);
	 usb_device_endpoint_enable_callback(module_inst,0,USB_DEVICE_ENDPOINT_CALLBACK_TRCPT);
	 usb_device_endpoint_enable_callback(module_inst,0,USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL);
	 
	 usb_device_endpoint_setup_buffer_job(module_inst,udd_ep0_buffer,64);
}


void udd_attach(void)
{
	usb_device_attach(&usb_device);
}

static void _usb_on_suspend(struct usb_module *module_inst)
{
	;
}

static void _usb_on_sof_notify(struct usb_module *module_inst)
{
	;
}

static void _usb_on_bus_reset(struct usb_module *module_inst)
{
	usb_device_set_address(module_inst,0);
	udd_ctrl_ep_enable(module_inst);
}

static void _usb_on_wakeup(struct usb_module *module_inst)
{
	;
}

static void _usb_end_resume(struct usb_module *module_inst)
{
	;
}

static void _usb_upstream_resume(struct usb_module *module_inst)
{
	;
}

static void _usb_on_error(struct usb_module *module_inst)
{
	;
}

static void _usb_lpm_nyet(struct usb_module *module_inst)
{
	;
}

static void _usb_lpm_suspend(struct usb_module *module_inst)
{
	;
}

void udd_enable(void)
{
	struct usb_config config_usb;
	struct usb_device_config config_device;
	
	/* USB detach */
	usb_device_detach(&usb_device);
	
	/* USB Module configuration */
	usb_get_config_defaults(&config_usb);
	usb_init(&usb_device, USB, &config_usb);

	/* USB Device Configuration */
	usb_device_get_config_defaults(&config_device);

    /* USB Device Interrupt Callback Register */
	usb_device_register_callback(&usb_device, USB_DEVICE_CALLBACK_SUSPEND, _usb_on_suspend );
	usb_device_register_callback(&usb_device, USB_DEVICE_CALLBACK_SOF, _usb_on_sof_notify);
	usb_device_register_callback(&usb_device, USB_DEVICE_CALLBACK_RESET, _usb_on_bus_reset);
	usb_device_register_callback(&usb_device, USB_DEVICE_CALLBACK_WAKEUP, _usb_on_wakeup);
	usb_device_register_callback(&usb_device, USB_DEVICE_CALLBACK_EORSM, _usb_end_resume);
	usb_device_register_callback(&usb_device, USB_DEVICE_CALLBACK_UPRSM, _usb_upstream_resume);
	usb_device_register_callback(&usb_device, USB_DEVICE_CALLBACK_RAMACER, _usb_on_error);
	usb_device_register_callback(&usb_device, USB_DEIVCE_CALLBACK_LPMNYET, _usb_lpm_nyet);
	usb_device_register_callback(&usb_device, USB_DEIVCE_CALLBACK_LPMSUSP, _usb_lpm_suspend);
	
	/* USB Device Interrupt Callback Enable */
	usb_device_enable_callback(&usb_device, USB_DEVICE_CALLBACK_SUSPEND);
	usb_device_enable_callback(&usb_device, USB_DEVICE_CALLBACK_SOF);
	usb_device_enable_callback(&usb_device, USB_DEVICE_CALLBACK_RESET);
	usb_device_enable_callback(&usb_device, USB_DEVICE_CALLBACK_WAKEUP);
    usb_device_enable_callback(&usb_device, USB_DEVICE_CALLBACK_EORSM);
	usb_device_enable_callback(&usb_device, USB_DEVICE_CALLBACK_UPRSM);
	usb_device_enable_callback(&usb_device, USB_DEVICE_CALLBACK_RAMACER);
	usb_device_enable_callback(&usb_device, USB_DEIVCE_CALLBACK_LPMNYET);
	usb_device_enable_callback(&usb_device, USB_DEIVCE_CALLBACK_LPMSUSP);
	
	/* USB Module Enable */
	usb_enable(&usb_device);
	
	/* USB Attach */
	usb_device_attach(&usb_device);
}
