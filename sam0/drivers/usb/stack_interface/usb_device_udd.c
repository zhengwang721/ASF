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

#define UDD_ENDPOINT_MAX_TRANS ((8*1024)-1)

//! Bit definitions about endpoint control state machine for udd_ep_control_state
typedef enum {
	UDD_EPCTRL_SETUP                  = 0, //!< Wait a SETUP packet
	UDD_EPCTRL_DATA_OUT               = 1, //!< Wait a OUT data packet
	UDD_EPCTRL_DATA_IN                = 2, //!< Wait a IN data packet
	UDD_EPCTRL_HANDSHAKE_WAIT_IN_ZLP  = 3, //!< Wait a IN ZLP packet
	UDD_EPCTRL_HANDSHAKE_WAIT_OUT_ZLP = 4, //!< Wait a OUT ZLP packet
	UDD_EPCTRL_STALL_REQ              = 5, //!< STALL enabled on IN & OUT packet
} udd_ctrl_ep_state_t;

/**
 * \name Management of bulk/interrupt/isochronous endpoints
 *
 * The UDD manages the data transfer on endpoints:
 * - Start data transfer on endpoint with USB Device DMA
 * - Send a ZLP packet if requested
 * - Call callback registered to signal end of transfer
 * The transfer abort and stall feature are supported.
 */
typedef struct {
	union {
		//! Callback to call at the end of transfer
		udd_callback_trans_t call_trans;
		//! Callback to call when the endpoint halt is cleared
		udd_callback_halt_cleared_t call_nohalt;
	};
	//! Buffer located in internal RAM to send or fill during job
	uint8_t *buf;
	//! Size of buffer to send or fill
	iram_size_t buf_size;
	//! Total number of data transfered on endpoint
	iram_size_t nb_trans;
	//! A job is registered on this endpoint
	uint8_t busy:1;
	//! A short packet is requested for this job on endpoint IN
	uint8_t b_shortpacket:1;
	//! The cache buffer is currently used on endpoint OUT
	uint8_t b_use_out_cache_buffer:1;
} udd_ep_job_t;

//! Global variable to give and record information of the setup request management
udd_ctrl_request_t udd_g_ctrlreq;

//! State of the endpoint control management
static udd_ctrl_ep_state_t udd_ep_control_state;

//! Total number of data received/sent during data packet phase with previous payload buffers
static uint16_t udd_ctrl_prev_payload_nb_trans;

//! Number of data received/sent to/from udd_g_ctrlreq.payload buffer
static uint16_t udd_ctrl_payload_nb_trans;

//! USB software device instance structure.
struct usb_module usb_device;

//static volatile uint32_t ent_cnt;

/**
 * \brief Buffer to store the data received on control endpoint (SETUP/OUT endpoint 0)
 *
 * Used to avoid a RAM buffer overflow in case of the payload buffer
 * is smaller than control endpoint size
 */
UDC_BSS(4)
uint8_t udd_ctrl_buffer[USB_DEVICE_EP_CTRL_SIZE];

/**
 * \brief Buffer to store the data received on bulk/interrupt endpoints
 *
 * Used to avoid a RAM buffer overflow in case of the user buffer
 * is smaller than endpoint size
 *
 * \warning The protected interrupt endpoint size is 512 bytes max.
 * \warning The isochronous and endpoint is not protected by this system and
 *          the user must always use a buffer corresponding at endpoint size.
 */
#if (0 != USB_DEVICE_MAX_EP)
	
#if (defined USB_DEVICE_LOW_SPEED)
UDC_BSS(4) uint8_t udd_ep_out_cache_buffer[USB_DEVICE_MAX_EP][8];
#elif (defined USB_DEVICE_HS_SUPPORT)
UDC_BSS(4) uint8_t udd_ep_out_cache_buffer[USB_DEVICE_MAX_EP][512];
#else
UDC_BSS(4) uint8_t udd_ep_out_cache_buffer[USB_DEVICE_MAX_EP][64];
#endif

//! Array to register a job on bulk/interrupt/isochronous endpoint
static udd_ep_job_t udd_ep_job[2 * USB_DEVICE_MAX_EP];

static udd_ep_job_t* udd_ep_get_job(udd_ep_id_t ep)
{
	return &udd_ep_job[(2 * (ep & USB_EP_ADDR_MASK) + ((ep & USB_EP_DIR_IN) ? 1 : 0)) - 2];
}

static void udd_ep_trans_in_next(udd_ep_id_t ep)
{
	uint16_t ep_size, nb_trans;
	uint16_t next_trans;
	udd_ep_id_t ep_num;
	udd_ep_job_t *ptr_job;

	ptr_job = udd_ep_get_job(ep);
	ep_num = ep & USB_EP_ADDR_MASK;

	ep_size = usb_device_endpoint_get_in_size(ep_num);
	// Update number of data transfered
	nb_trans = usb_device_endpoint_get_sent_bytes(ep_num);
	ptr_job->nb_trans += nb_trans;

	// Need to send other data
	if (ptr_job->nb_trans != ptr_job->buf_size) {
		next_trans = ptr_job->buf_size - ptr_job->nb_trans;
		if (UDD_ENDPOINT_MAX_TRANS < next_trans) {
		// The USB hardware support a maximum
		// transfer size of UDD_ENDPOINT_MAX_TRANS Bytes
			next_trans = UDD_ENDPOINT_MAX_TRANS -(UDD_ENDPOINT_MAX_TRANS % ep_size);
		}
		// Need ZLP, if requested and last packet is not a short packet
		ptr_job->b_shortpacket = ptr_job->b_shortpacket && (0 == (next_trans % ep_size));
		usb_device_endpoint_write_buffer_job(&usb_device,ep_num,&ptr_job->buf[ptr_job->nb_trans],next_trans);
		usb_set_bank_in_full(&usb_device,ep_num);
		return;
	}

	// Need to send a ZLP after all data transfer
	if (ptr_job->b_shortpacket) {
		ptr_job->b_shortpacket = false;
		// Start new transfer
		usb_device_endpoint_write_buffer_job(&usb_device,ep_num,&ptr_job->buf[ptr_job->nb_trans],0);
		usb_set_bank_in_full(&usb_device,ep_num);
		return;
	}

	// Job complete then call callback
	ptr_job->busy = false;
	if (NULL != ptr_job->call_trans) {
		ptr_job->call_trans(UDD_EP_TRANSFER_OK, ptr_job->nb_trans, ep);
	}
}

static void udd_ep_trans_out_next(udd_ep_id_t ep)
{
	uint16_t ep_size, nb_trans;
	uint16_t next_trans;
	udd_ep_id_t ep_num;
	udd_ep_job_t *ptr_job;

	ptr_job = udd_ep_get_job(ep);
	ep_num = ep & USB_EP_ADDR_MASK;

	// Lock emission of new OUT packet
	//Assert(udd_is_full_bank_out(ep_num));

	ep_size = usb_device_endpoint_get_out_size(ep_num);
	// Update number of data transfered
	nb_trans = usb_device_endpoint_get_received_bytes(ep_num);

	// Can be necessary to copy data receive from cache buffer to user buffer
	if (ptr_job->b_use_out_cache_buffer) {
		memcpy(&ptr_job->buf[ptr_job->nb_trans], udd_ep_out_cache_buffer[ep_num - 1], ptr_job->buf_size % ep_size);
	}

	// Update number of data transfered
	ptr_job->nb_trans += nb_trans;
	if (ptr_job->nb_trans > ptr_job->buf_size) {
		ptr_job->nb_trans = ptr_job->buf_size;
	}

	// If all previous data requested are received and user buffer not full
	// then need to receive other data
	if ((nb_trans == usb_device_endpoint_get_out_buf_size(ep_num))
		&& (ptr_job->nb_trans != ptr_job->buf_size)) {
		next_trans = ptr_job->buf_size - ptr_job->nb_trans;
		if (UDD_ENDPOINT_MAX_TRANS < next_trans) {
		// The USB hardware support a maximum transfer size
		// of UDD_ENDPOINT_MAX_TRANS Bytes
		next_trans = UDD_ENDPOINT_MAX_TRANS - (UDD_ENDPOINT_MAX_TRANS % ep_size);
		} else {
			next_trans -= next_trans % ep_size;
		}

		if (next_trans < ep_size) {
			// Use the cache buffer for Bulk or Interrupt size endpoint
			ptr_job->b_use_out_cache_buffer = true;
			usb_device_endpoint_read_buffer_job(&usb_device,ep_num,udd_ep_out_cache_buffer[ep_num - 1],ep_size);
		} else {
			usb_device_endpoint_read_buffer_job(&usb_device,ep_num,&ptr_job->buf[ptr_job->nb_trans],next_trans);
		}
		// Start next transfer
		usb_set_bank_out_empty(&usb_device,ep_num);
		return;
	}

	// Job complete then call callback
	ptr_job->busy = false;
	if (NULL != ptr_job->call_trans) {
		ptr_job->call_trans(UDD_EP_TRANSFER_OK, ptr_job->nb_trans, ep);
	}
}
#endif

static void udd_ep_transfer_failure(struct usb_module *module_inst, uint8_t ep_num)
{
	if(usb_device_out_is_fail(module_inst,ep_num)) {
		// clear the flag
		usb_ack_trfail_out(module_inst,ep_num);
		if(usb_nak_is_out(ep_num)){
			usb_nak_send_out(ep_num);
		}
		} else {
		// clear the flag
		usb_ack_trfail_in(module_inst,ep_num);
		if(usb_nak_is_in(ep_num)){
			usb_nak_send_in(ep_num);
		}
	}
}

static void udd_ep_transfer_process(struct usb_module *module_inst, uint8_t ep_num)
{
	if(usb_device_out_is_received(module_inst,ep_num)) {
		usb_ack_out_received(module_inst,ep_num);
		udd_ep_id_t ep = ep_num;
		udd_ep_trans_out_next(ep);
	} else {
		usb_ack_in_sent(module_inst,ep_num);
		udd_ep_id_t ep = ep_num | USB_REQ_DIR_IN;
		udd_ep_trans_in_next(ep);
	}
}

void udd_ep_abort(udd_ep_id_t ep)
{
	udd_ep_job_t *ptr_job;
	udd_ep_id_t ep_num;

	ep_num = ep & USB_EP_ADDR_MASK;

	// Stop transfer
	if (ep & USB_EP_DIR_IN) {
		usb_set_bank_in_empty(&usb_device,ep_num);
		// Eventually ack a transfer occured during abort
		usb_ack_in_sent(&usb_device,ep_num);
	} else {
		usb_set_bank_out_full(&usb_device,ep_num);
		// Eventually ack a transfer occured during abort
		usb_ack_out_received(&usb_device,ep_num);
	}

	// Job complete then call callback
	ptr_job = udd_ep_get_job(ep);
	if (!ptr_job->busy) {
		return;
	}
	ptr_job->busy = false;
	if (NULL != ptr_job->call_trans) {
		// It can be a Transfer or stall callback
		ptr_job->call_trans(UDD_EP_TRANSFER_ABORT, ptr_job->nb_trans, ep);
	}
}

uint16_t udd_get_frame_number(void)
{
	return usb_device_get_frame_number(&usb_device);
}

uint16_t udd_get_micro_frame_number(void)
{
	return usb_device_get_micro_frame_number(&usb_device);
}

void udd_ep_free(udd_ep_id_t ep)
{
	struct usb_device_endpoint_config config_ep;
	usb_device_endpoint_get_config_defaults(&config_ep);
	
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;
	udd_ep_abort(ep);
	
	config_ep.ep_address = ep;
	config_ep.ep_type = USB_DEVICE_ENDPOINT_TYPE_DISABLE;
	usb_device_endpoint_set_config(&usb_device, &config_ep);
	usb_device_endpoint_unregister_callback(&usb_device,ep_num,USB_DEVICE_ENDPOINT_CALLBACK_TRCPT);
	usb_device_endpoint_disable_callback(&usb_device,ep,USB_DEVICE_ENDPOINT_CALLBACK_TRCPT);
}



bool udd_ep_alloc(udd_ep_id_t ep, uint8_t bmAttributes, uint16_t MaxEndpointSize)
{
	struct usb_device_endpoint_config config_ep;
	usb_device_endpoint_get_config_defaults(&config_ep);
	
	config_ep.ep_address = ep;
	
	if(MaxEndpointSize <= 8) {
		config_ep.ep_size = USB_ENDPOINT_8_BYTE;
	} else if(MaxEndpointSize <= 16) {
		config_ep.ep_size = USB_ENDPOINT_16_BYTE;
	} else if(MaxEndpointSize <= 32) {
		config_ep.ep_size = USB_ENDPOINT_32_BYTE;
	} else if(MaxEndpointSize <= 64) {
		config_ep.ep_size = USB_ENDPOINT_64_BYTE;
	} else if(MaxEndpointSize <= 128) {
		config_ep.ep_size = USB_ENDPOINT_128_BYTE;
	} else if(MaxEndpointSize <= 256) {
		config_ep.ep_size = USB_ENDPOINT_256_BYTE;
	} else if(MaxEndpointSize <= 512) {
		config_ep.ep_size = USB_ENDPOINT_512_BYTE;
	} else if(MaxEndpointSize <= 1023) {
		config_ep.ep_size = USB_ENDPOINT_1023_BYTE;
	} else {
		return false;
	}
	
	bmAttributes = bmAttributes & USB_EP_TYPE_MASK;
	
	// Check endpoint type
	if(USB_EP_TYPE_ISOCHRONOUS == bmAttributes) {
		config_ep.ep_type = USB_DEVICE_ENDPOINT_TYPE_ISOCHRONOUS;
	} else if (USB_EP_TYPE_BULK == bmAttributes) {
		config_ep.ep_type = USB_DEVICE_ENDPOINT_TYPE_BULK;
	} else if (USB_EP_TYPE_INTERRUPT == bmAttributes) {
		config_ep.ep_type = USB_DEVICE_ENDPOINT_TYPE_INTERRUPT;
	} else {
		return false;
	}
	
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;
	
	if (ep & USB_EP_DIR_IN) {
		if (usb_device_endpoint_in_is_enabled(&usb_device,ep_num)) {
			return false;
		} else {
			usb_device_endpoint_set_config(&usb_device, &config_ep);
			usb_device_endpoint_register_callback(&usb_device,ep_num,USB_DEVICE_ENDPOINT_CALLBACK_TRCPT,udd_ep_transfer_process);
			usb_device_endpoint_enable_callback(&usb_device,ep,USB_DEVICE_ENDPOINT_CALLBACK_TRCPT);
			usb_device_endpoint_register_callback(&usb_device,ep_num,USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL,udd_ep_transfer_failure);
			usb_device_endpoint_enable_callback(&usb_device,ep,USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL);
			usb_set_bank_in_empty(&usb_device,ep_num);
			return true;
		}
	} else {
		if (usb_device_endpoint_out_is_enabled(&usb_device,ep_num)) {
			return false;
		} else {
			usb_device_endpoint_set_config(&usb_device, &config_ep);
			usb_device_endpoint_register_callback(&usb_device,ep_num,USB_DEVICE_ENDPOINT_CALLBACK_TRCPT,udd_ep_transfer_process);
			usb_device_endpoint_enable_callback(&usb_device,ep,USB_DEVICE_ENDPOINT_CALLBACK_TRCPT);
			usb_device_endpoint_register_callback(&usb_device,ep_num,USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL,udd_ep_transfer_failure);
			usb_device_endpoint_enable_callback(&usb_device,ep,USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL);
			usb_set_bank_out_full(&usb_device,ep_num);
			return true;
		}
		//#if (defined USB_DISABLE_NYET_FOR_OUT_ENDPOINT)
		///*Disable the NYET feature for OUT endpoint. Using OUT multipacket, each
		//OUT packet are always NYET.*/
		//udd_disable_nyet(ep_num);
		//#endif
	}
}


bool udd_ep_is_halted(udd_ep_id_t ep)
{
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;

	if (ep & USB_EP_DIR_IN) {
		return usb_device_is_in_stall_requested(&usb_device,ep_num);
	} else {
		return usb_device_is_out_stall_requested(&usb_device,ep_num);
	}
}


bool udd_ep_set_halt(udd_ep_id_t ep)
{
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;

	if (USB_DEVICE_MAX_EP < ep_num) {
		return false;
	}

	// Stall endpoint
	if (ep & USB_EP_DIR_IN) {
		usb_device_enable_stall_handshake_in(&usb_device,ep_num);
		} else {
		usb_device_enable_stall_handshake_out(&usb_device,ep_num);
	}
	udd_ep_abort(ep);
	return true;
}


bool udd_ep_clear_halt(udd_ep_id_t ep)
{
	udd_ep_job_t *ptr_job;
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;

	if (USB_DEVICE_MAX_EP < ep_num) {
		return false;
	}
	ptr_job = udd_ep_get_job(ep);

	if (ep & USB_EP_DIR_IN) {
		if (usb_device_is_in_stall_requested(&usb_device,ep_num)) {
			// Remove stall request
			usb_device_disable_stall_handshake_in(&usb_device,ep_num);
			if (usb_device_is_stall_in(&usb_device,ep_num)) {
				usb_ack_stall_in(&usb_device,ep_num);
				// The Stall has occurred, then reset data toggle
				usb_clear_data_toggle_in(&usb_device,ep_num);
			}
			// If a job is register on clear halt action
			// then execute callback
			if (ptr_job->busy == true) {
				ptr_job->busy = false;
				ptr_job->call_nohalt();
			}
		}
	} else {
		if (usb_device_is_out_stall_requested(&usb_device,ep_num)) {
			// Remove stall request
			usb_device_disable_stall_handshake_out(&usb_device,ep_num);
			if (usb_device_is_stall_out(&usb_device,ep_num)) {
				usb_ack_stall_out(&usb_device,ep_num);
				// The Stall has occurred, then reset data toggle
				usb_clear_data_toggle_out(&usb_device,ep_num);
			}
			// If a job is register on clear halt action
			// then execute callback
			if (ptr_job->busy == true) {
				ptr_job->busy = false;
				ptr_job->call_nohalt();
			}
		}
	}
	return true;
}

bool udd_ep_wait_stall_clear(udd_ep_id_t ep, udd_callback_halt_cleared_t callback)
{
	udd_ep_id_t ep_num;
	udd_ep_job_t *ptr_job;

	ep_num = ep & USB_EP_ADDR_MASK;
	if (USB_DEVICE_MAX_EP < ep_num) {
		return false;
	}

	ptr_job = udd_ep_get_job(ep);
	if (ptr_job->busy == true) {
		return false; // Job already on going
	}

	if (ep & USB_EP_DIR_IN) {
		if (!usb_device_endpoint_in_is_enabled(&usb_device,ep_num)) {
			return false; // Endpoint not enabled
		}
		// Wait clear halt endpoint
		if (usb_device_is_in_stall_requested(&usb_device,ep_num)) {
			// Endpoint halted then registers the callback
			ptr_job->busy = true;
			ptr_job->call_nohalt = callback;
			return true;
		}
		} else {
		if (!usb_device_endpoint_out_is_enabled(&usb_device,ep_num)) {
			return false; // Endpoint not enabled
		}
		// Wait clear halt endpoint
		if (usb_device_is_out_stall_requested(&usb_device,ep_num)) {
			// Endpoint halted then registers the callback
			ptr_job->busy = true;
			ptr_job->call_nohalt = callback;
			return true;
		}
	}

	// Endpoint not halted then call directly callback
	callback();
	return true;
}

static void udd_ctrl_stall_data(void)
{
	udd_ep_control_state = UDD_EPCTRL_STALL_REQ;
	usb_device_enable_stall_handshake_in(&usb_device,0);
	usb_device_disable_stall_handshake_out(&usb_device,0);
}

bool udd_ep_run(udd_ep_id_t ep, bool b_shortpacket, uint8_t * buf, iram_size_t buf_size, udd_callback_trans_t callback)
{
	udd_ep_id_t ep_num;
	udd_ep_job_t *ptr_job;
	//irqflags_t flags;

	ep_num = ep & USB_EP_ADDR_MASK;
	
	if (USB_DEVICE_MAX_EP < ep_num) {
		return false;
	}
	
	if (ep & USB_EP_DIR_IN) {
		if ((!usb_device_endpoint_in_is_enabled(&usb_device,ep_num)) 
			|| usb_device_is_in_stall_requested(&usb_device,ep_num)) {
			return false;
		}
	} else {
		if ((!usb_device_endpoint_out_is_enabled(&usb_device,ep_num)) 
			|| usb_device_is_out_stall_requested(&usb_device,ep_num)) {
			return false;
		}
	}

	ptr_job = udd_ep_get_job(ep);

	//flags = cpu_irq_save();
	if (ptr_job->busy == true) {
		//cpu_irq_restore(flags);
		return false; // Job already on going
	}
	ptr_job->busy = true;
	//cpu_irq_restore(flags);

	// No job running. Let's setup a new one.
	//
	ptr_job->buf = buf;
	ptr_job->buf_size = buf_size;
	ptr_job->nb_trans = 0;
	ptr_job->call_trans = callback;
	ptr_job->b_shortpacket = b_shortpacket;
	ptr_job->b_use_out_cache_buffer = false;

	if ( (USB_EP_DIR_IN != (ep & USB_EP_DIR_IN)) 
		&& (USB_DEVICE_ENDPOINT_TYPE_ISOCHRONOUS == usb_device_get_endpoint_out_type(&usb_device,ep_num)) 
		&& (0 != (buf_size % usb_device_endpoint_get_out_size(ep_num)))) {
		// The user must use a buffer size modulo endpoint size
		// for an isochronous OUT endpoint
		ptr_job->busy = false;
		return false;
	}

	// Initialize value to simulate a empty transfer
	if (ep & USB_EP_DIR_IN) {
		usb_device_endpoint_clear_sent_bytes(ep_num);
		// Request next transfer
		udd_ep_trans_in_next(ep);
	} else {
		usb_device_endpoint_clear_received_bytes(ep_num);
		// Request next transfer
		udd_ep_trans_out_next(ep);
	}
	return true;
}

void udd_set_address(uint8_t address)
{
	usb_device_set_address(&usb_device,address);
}

uint8_t udd_getaddress(void)
{
	return usb_device_get_address(&usb_device);
}

void udd_send_remotewakeup(void)
{
	usb_send_remote_wake_up(&usb_device);
}

void udd_set_setup_payload( uint8_t *payload, uint16_t payload_size )
{
	udd_g_ctrlreq.payload = payload;
	udd_g_ctrlreq.payload_size = payload_size;
}

static void udd_ctrl_fetch_ram(void)
{
	udd_g_ctrlreq.req.bmRequestType = udd_ctrl_buffer[0];
	udd_g_ctrlreq.req.bRequest = udd_ctrl_buffer[1];
	udd_g_ctrlreq.req.wValue = ((uint16_t)(udd_ctrl_buffer[3]) << 8) + udd_ctrl_buffer[2];
	udd_g_ctrlreq.req.wIndex = ((uint16_t)(udd_ctrl_buffer[5]) << 8) + udd_ctrl_buffer[4];
	udd_g_ctrlreq.req.wLength = ((uint16_t)(udd_ctrl_buffer[7]) << 8) + udd_ctrl_buffer[6];
	usb_device_endpoint_clear_received_bytes(0);
}

static void udd_ctrl_send_zlp_in(void)
{
	udd_ep_control_state = UDD_EPCTRL_HANDSHAKE_WAIT_IN_ZLP;
	usb_device_endpoint_write_buffer_job(&usb_device,0,udd_g_ctrlreq.payload,0);
	
	usb_set_bank_in_full(&usb_device,0);
}

static void udd_ctrl_in_sent(void)
{
	static bool b_shortpacket = false;
	uint16_t nb_remain;

	nb_remain = udd_g_ctrlreq.payload_size - udd_ctrl_payload_nb_trans;
	
	if (0 == nb_remain) {
		// All content of current buffer payload are sent
		// Update number of total data sending by previous payload buffer
		udd_ctrl_prev_payload_nb_trans += udd_ctrl_payload_nb_trans;
		if ((udd_g_ctrlreq.req.wLength == udd_ctrl_prev_payload_nb_trans) || b_shortpacket) {
			// All data requested are transfered or a short packet has been sent
			// then it is the end of data phase.
			// Generate an OUT ZLP for handshake phase.
			udd_ep_control_state = UDD_EPCTRL_HANDSHAKE_WAIT_OUT_ZLP;
			usb_set_bank_out_empty(&usb_device, 0);
			return;
		}
		// Need of new buffer because the data phase is not complete
		if ((!udd_g_ctrlreq.over_under_run) || (!udd_g_ctrlreq.over_under_run())) {
			// Underrun then send zlp on IN
			// Here nb_remain=0, this allows to send a IN ZLP
			} else {
			// A new payload buffer is given
			udd_ctrl_payload_nb_trans = 0;
			nb_remain = udd_g_ctrlreq.payload_size;
		}
	}
	
	// Continue transfer and send next data
	if (nb_remain >= USB_DEVICE_EP_CTRL_SIZE) {
		nb_remain = USB_DEVICE_EP_CTRL_SIZE;
		b_shortpacket = false;
		} else {
		b_shortpacket = true;
	}

	// Link payload buffer directly on USB hardware
	usb_device_endpoint_write_buffer_job(&usb_device,0,udd_g_ctrlreq.payload + udd_ctrl_payload_nb_trans,nb_remain);
	
	udd_ctrl_payload_nb_trans += nb_remain;
	
	usb_set_bank_out_empty(&usb_device, 0);
	usb_set_bank_in_full(&usb_device,0);
}

static void udd_ctrl_out_received(void)
{
	uint16_t nb_data;

	// Read data received during OUT phase
	nb_data = usb_device_endpoint_get_received_bytes(0);
	usb_device_endpoint_clear_received_bytes(0);
	
	if (udd_g_ctrlreq.payload_size < (udd_ctrl_payload_nb_trans + nb_data)) {
		// Payload buffer too small
		nb_data = udd_g_ctrlreq.payload_size - udd_ctrl_payload_nb_trans;
	}

	memcpy((uint8_t *) (udd_g_ctrlreq.payload + udd_ctrl_payload_nb_trans), udd_ctrl_buffer, nb_data);
	udd_ctrl_payload_nb_trans += nb_data;

	if ((USB_DEVICE_EP_CTRL_SIZE != nb_data) || \
	(udd_g_ctrlreq.req.wLength <= (udd_ctrl_prev_payload_nb_trans + udd_ctrl_payload_nb_trans))) {
		// End of reception because it is a short packet
		// or all data are transfered

		// Before send ZLP, call intermediate callback
		// in case of data receive generate a stall
		udd_g_ctrlreq.payload_size = udd_ctrl_payload_nb_trans;
		if (NULL != udd_g_ctrlreq.over_under_run) {
			if (!udd_g_ctrlreq.over_under_run()) {
				// Stall ZLP
				udd_ep_control_state = UDD_EPCTRL_STALL_REQ;
				// Stall all packets on IN & OUT control endpoint
				udd_ep_set_halt(0);
				// Ack reception of OUT to replace NAK by a STALL
				return;
			}
		}
		// Send IN ZLP to ACK setup request
		udd_ctrl_send_zlp_in();
		return;
	}

	if (udd_g_ctrlreq.payload_size == udd_ctrl_payload_nb_trans) {
		// Overrun then request a new payload buffer
		if (!udd_g_ctrlreq.over_under_run) {
			// No callback available to request a new payload buffer
			// Stall ZLP
			udd_ep_control_state = UDD_EPCTRL_STALL_REQ;
			// Stall all packets on IN & OUT control endpoint
			udd_ep_set_halt(0);
			return;
		}
		if (!udd_g_ctrlreq.over_under_run()) {
			// No new payload buffer delivered
			// Stall ZLP
			udd_ep_control_state = UDD_EPCTRL_STALL_REQ;
			// Stall all packets on IN & OUT control endpoint
			udd_ep_set_halt(0);
			return;
		}
		// New payload buffer available
		// Update number of total data received
		udd_ctrl_prev_payload_nb_trans += udd_ctrl_payload_nb_trans;

		// Reinit reception on payload buffer
		udd_ctrl_payload_nb_trans = 0;
	}
	// Init buffer size and enable OUT bank
	usb_set_bank_out_empty(&usb_device,0);
}

static void _usb_ep_on_setup(struct usb_module *module_inst, uint8_t ep_num)
{
	if (UDD_EPCTRL_SETUP != udd_ep_control_state) {
		if (NULL != udd_g_ctrlreq.callback) {
			udd_g_ctrlreq.callback();
		}
		udd_ep_control_state = UDD_EPCTRL_SETUP;
	}
	if ( 8 != usb_device_endpoint_get_received_bytes(ep_num)) {
		udd_ctrl_stall_data();
		return;
	} else {
		udd_ctrl_fetch_ram();
		if (false == udc_process_setup()) {
			udd_ctrl_stall_data();
			return;
		} else if (Udd_setup_is_in()) {
			udd_ctrl_prev_payload_nb_trans = 0;
			udd_ctrl_payload_nb_trans = 0;
			udd_ep_control_state = UDD_EPCTRL_DATA_IN;
			udd_ctrl_in_sent();
		} else {
			if(0 == udd_g_ctrlreq.req.wLength) {
				udd_ctrl_send_zlp_in();
				return;
			} else {
				udd_ctrl_prev_payload_nb_trans = 0;
				udd_ctrl_payload_nb_trans = 0;
				udd_ep_control_state = UDD_EPCTRL_DATA_OUT;
				usb_set_bank_out_empty(module_inst,ep_num);
			}
		}
	}
}

static void udd_ctrl_underflow(void)
{
	if (usb_device_out_is_received(&usb_device,0)) {
		return; // underflow ignored if OUT data is received
	}

	if (UDD_EPCTRL_DATA_OUT == udd_ep_control_state) {
		// Host want to stop OUT transaction
		// then stop to wait OUT data phase and wait IN ZLP handshake
		udd_ctrl_send_zlp_in();
	} else if (UDD_EPCTRL_HANDSHAKE_WAIT_OUT_ZLP == udd_ep_control_state) {
		// A OUT handshake is waiting by device,
		// but host want extra IN data then stall extra IN data
		usb_device_enable_stall_handshake_in(&usb_device,0);
	}
}


static void udd_ctrl_overflow(void)
{
	if (usb_device_in_is_sent(&usb_device,0)) {
		return; // overflow ignored if IN data is received
	}
	if (UDD_EPCTRL_DATA_IN == udd_ep_control_state) {
		// Host want to stop IN transaction
		// then stop to wait IN data phase and wait OUT ZLP handshake
		udd_ep_control_state = UDD_EPCTRL_HANDSHAKE_WAIT_OUT_ZLP;
	} else if (UDD_EPCTRL_HANDSHAKE_WAIT_IN_ZLP == udd_ep_control_state) {
		// A IN handshake is waiting by device,
		// but host want extra OUT data then stall extra OUT data and following status stage
		usb_device_enable_stall_handshake_out(&usb_device,0);
	}
}

static void _usb_ep_on_tansfer_fail(struct usb_module *module_inst, uint8_t ep_num)
{
	if(usb_device_out_is_fail(module_inst,ep_num)) {
		// clear the flag
		usb_ack_trfail_out(module_inst,ep_num);
		if(usb_nak_is_out(ep_num)){
			usb_nak_send_out(ep_num);
			udd_ctrl_overflow();
		}
	} else {
		// clear the flag
		usb_ack_trfail_in(module_inst,ep_num);
		if(usb_nak_is_in(ep_num)){
			usb_nak_send_in(ep_num);
			udd_ctrl_underflow();
		}
	}
}


static void _usb_ep_on_tansfer_ok(struct usb_module *module_inst,uint8_t ep_num)
{
	if(usb_device_out_is_received(module_inst,ep_num)) {
		// clear the flag
		usb_ack_out_received(module_inst,ep_num);
		// handshake Out for status stage
		if (UDD_EPCTRL_DATA_OUT  == udd_ep_control_state) {
			udd_ctrl_out_received();	
		} else {
			if (NULL != udd_g_ctrlreq.callback) {
				udd_g_ctrlreq.callback();
			}
			udd_ep_control_state = UDD_EPCTRL_SETUP;
		}
	} else {
		// clear the flag
		usb_ack_in_sent(module_inst,ep_num);
		// handshake In for status stage
		if (UDD_EPCTRL_DATA_IN == udd_ep_control_state) {
			udd_ctrl_in_sent();
		} else {
			if (NULL != udd_g_ctrlreq.callback) {
				udd_g_ctrlreq.callback();
			}
			udd_ep_control_state = UDD_EPCTRL_SETUP;
		}			
	}
}

static void udd_ctrl_ep_enable(struct usb_module *module_inst)
{
	/* USB Device Endpoint0 Configuration */
	 struct usb_device_endpoint_config config_ep0;
	 
	 usb_device_endpoint_get_config_defaults(&config_ep0);
	 config_ep0.ep_size =  (32 - clz(((uint32_t)Min(Max(USB_DEVICE_EP_CTRL_SIZE, 8), 1024) << 1) - 1) - 1 - 3);
	 usb_device_endpoint_set_config(module_inst,&config_ep0);
	 
	 usb_device_endpoint_setup_buffer_job(module_inst,udd_ctrl_buffer,USB_DEVICE_EP_CTRL_SIZE);
	 usb_set_bank_out_empty(module_inst,0);
	 usb_set_bank_in_empty(module_inst,0);
	 
	 usb_device_endpoint_register_callback(module_inst,0,USB_DEVICE_ENDPOINT_CALLBACK_RXSTP, _usb_ep_on_setup );
	 usb_device_endpoint_register_callback(module_inst,0,USB_DEVICE_ENDPOINT_CALLBACK_TRCPT,_usb_ep_on_tansfer_ok );
	 usb_device_endpoint_register_callback(module_inst,0,USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL,_usb_ep_on_tansfer_fail );
	 usb_device_endpoint_enable_callback(module_inst,0,USB_DEVICE_ENDPOINT_CALLBACK_RXSTP);
	 usb_device_endpoint_enable_callback(module_inst,0,USB_DEVICE_ENDPOINT_CALLBACK_TRCPT);
	 usb_device_endpoint_enable_callback(module_inst,0,USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL);
	 
	 usb_ack_out_received(module_inst,0);
	 usb_ack_in_sent(module_inst,0);
	 usb_ack_trfail_out(module_inst,0);
	 usb_nak_send_out(0);
	 usb_ack_trfail_in(module_inst,0);
	 usb_nak_send_in(0);

	 udd_ep_control_state = UDD_EPCTRL_SETUP;
}

static void _usb_on_suspend(struct usb_module *module_inst)
{
	usb_device_disable_callback(&usb_device, USB_DEVICE_CALLBACK_SUSPEND);
	usb_device_clear_wakeup_int_flag(&usb_device);
	usb_device_enable_callback(&usb_device, USB_DEVICE_CALLBACK_WAKEUP);
	#ifdef UDC_SUSPEND_EVENT
	UDC_SUSPEND_EVENT();
	#endif
}

static void _usb_on_sof_notify(struct usb_module *module_inst)
{
	udc_sof_notify();
	#ifdef UDC_SOF_EVENT
	UDC_SOF_EVENT();
	#endif
}

static void _usb_on_bus_reset(struct usb_module *module_inst)
{
	usb_device_set_address(module_inst,0);
	udd_ctrl_ep_enable(module_inst);
}

static void _usb_on_wakeup(struct usb_module *module_inst)
{
	usb_device_disable_callback(&usb_device, USB_DEVICE_CALLBACK_WAKEUP);
	usb_device_clear_suspend_int_flag(&usb_device);
	usb_device_enable_callback(&usb_device, USB_DEVICE_CALLBACK_SUSPEND);
	#ifdef UDC_RESUME_EVENT
	UDC_RESUME_EVENT();
	#endif
}

//static void _usb_end_resume(struct usb_module *module_inst)
//{
	//;
//}
//
//static void _usb_upstream_resume(struct usb_module *module_inst)
//{
	//;
//}

//static void _usb_on_error(struct usb_module *module_inst)
//{
	//;
//}

//static void _usb_lpm_nyet(struct usb_module *module_inst)
//{
	//;
//}
//
//static void _usb_lpm_suspend(struct usb_module *module_inst)
//{
	//;
//}

void udd_detach(void)
{
	usb_device_detach(&usb_device);
}

void udd_attach(void)
{
	usb_device_attach(&usb_device);
	
	usb_device_register_callback(&usb_device, USB_DEVICE_CALLBACK_SUSPEND, _usb_on_suspend);
	usb_device_register_callback(&usb_device, USB_DEVICE_CALLBACK_SOF, _usb_on_sof_notify);
	usb_device_register_callback(&usb_device, USB_DEVICE_CALLBACK_RESET, _usb_on_bus_reset);
	usb_device_register_callback(&usb_device, USB_DEVICE_CALLBACK_WAKEUP, _usb_on_wakeup);
	
	usb_device_enable_callback(&usb_device, USB_DEVICE_CALLBACK_SUSPEND);
	usb_device_enable_callback(&usb_device, USB_DEVICE_CALLBACK_SOF);
	usb_device_enable_callback(&usb_device, USB_DEVICE_CALLBACK_RESET);
	usb_device_enable_callback(&usb_device, USB_DEVICE_CALLBACK_WAKEUP);
	
	usb_device_clear_reset_int_flag(&usb_device);
	usb_device_clear_sof_int_flag(&usb_device);
	usb_device_clear_wakeup_int_flag(&usb_device);
	usb_device_clear_suspend_int_flag(&usb_device);
}

void udd_enable(void)
{
	//irqflags_t flags;
	//flags = cpu_irq_save();
	
	struct usb_config config_usb;
	
	/* USB Module configuration */
	usb_get_config_defaults(&config_usb);
	usb_init(&usb_device, USB, &config_usb);
	
	/* USB Module Enable */
	usb_enable(&usb_device);
	
	/* USB Attach */
	udd_attach();
	//cpu_irq_restore(flags);
}

void udd_disable(void)
{
	////irqflags_t flags;
	////flags = cpu_irq_save();
	
	udd_detach();
	usb_disable(&usb_device);
	////cpu_irq_restore(flags);
}