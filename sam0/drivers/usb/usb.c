/**
 * \file
 *
 * \brief SAM D21 USB Driver.
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
#include "usb.h"

/**
 * \brief Mask selecting the index part of an endpoint address
 */
#define  USB_EP_ADDR_MASK     0x0f

/**
 * \brief Endpoint transfer direction is IN
 */
#define  USB_EP_DIR_IN        0x80

/**
 * \brief Endpoint transfer direction is OUT
 */
#define  USB_EP_DIR_OUT       0x00

/**
 * @brief USB SRAM data containing pipe descriptor table
 * The content of the USB SRAM can be :
 * - modified by USB hardware interface to update pipe status.
 *   Thereby, it is read by software.
 * - modified by USB software to control pipe.
 *   Thereby, it is read by hardware.
 * This data section is volatile.
 *
 * @{
 */
//COMPILER_PACK_SET(1)
//COMPILER_WORD_ALIGNED
union {
	UsbDeviceDescriptor usb_endpoint_table[USB_EPT_NUM];
	UsbHostDescriptor usb_pipe_table[USB_PIPE_NUM];
} usb_descriptor_table;
//COMPILER_PACK_RESET()

struct usb_module *_usb_instances;

struct usb_pipe_callback_parameter callback_para;

/******************************************************************/
/*************Second part: USB device start*****************************/

//Roeder will finish this part 


/********************Second part: USB device end***********************/
/******************************************************************/

/******************************************************************/
/********************Third part: USB Host start*********************/

enum status_code usb_host_register_callback(struct usb_module *module_inst,
		enum usb_host_callback callback_type,
		usb_host_callback_t callback_func)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(callback_func);

	/* Register callback function */
	module_inst->host_callback[callback_type] = callback_func;

	/* Set the bit corresponding to the callback_type */
	module_inst->host_registered_callback_mask |= (1 << callback_type);

	return STATUS_OK;
}
enum status_code usb_host_unregister_callback(struct usb_module *module_inst,
		enum usb_host_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);

	/* Unregister callback function */
	module_inst->host_callback[callback_type] = NULL;

	/* Clear the bit corresponding to the callback_type */
	module_inst->host_registered_callback_mask &= ~(1 << callback_type);

	return STATUS_OK;
}
enum status_code usb_host_enable_callback(struct usb_module *module_inst,
		enum usb_host_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);

	/* Enable callback */
	module_inst->host_enabled_callback_mask |= (1 << callback_type);

	if (callback_type == USB_HOST_CALLBACK_SOF) {
		module_inst->hw->HOST.INTENSET.reg = USB_HOST_INTENSET_HSOF;
	}
	if (callback_type == USB_HOST_CALLBACK_RESET) {
		module_inst->hw->HOST.INTENSET.reg = USB_HOST_INTENSET_RST;
	}
	if (callback_type == USB_HOST_CALLBACK_WAKEUP) {
		module_inst->hw->HOST.INTENSET.reg = USB_HOST_INTENSET_WAKEUP;
	}
	if (callback_type == USB_HOST_CALLBACK_DNRSM) {
		module_inst->hw->HOST.INTENSET.reg = USB_HOST_INTENSET_DNRSM;
	}
	if (callback_type == USB_HOST_CALLBACK_UPRSM) {
		module_inst->hw->HOST.INTENSET.reg = USB_HOST_INTENSET_UPRSM;
	}
	if (callback_type == USB_HOST_CALLBACK_RAMACER) {
		module_inst->hw->HOST.INTENSET.reg = USB_HOST_INTENSET_RAMACER;
	}
	if (callback_type == USB_HOST_CALLBACK_CONNECT) {
		module_inst->hw->HOST.INTENSET.reg = USB_HOST_INTENSET_DCONN;
	}
	if (callback_type == USB_HOST_CALLBACK_DISCONNECT) {
		module_inst->hw->HOST.INTENSET.reg = USB_HOST_INTENSET_DDISC;
	}

	return STATUS_OK;
}
enum status_code usb_host_disable_callback(struct usb_module *module_inst,
		enum usb_host_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);

	/* Disable callback */
	module_inst->host_enabled_callback_mask &= ~(1 << callback_type);

	if (callback_type == USB_HOST_CALLBACK_SOF) {
		module_inst->hw->HOST.INTENCLR.reg = USB_HOST_INTENCLR_HSOF;
	}
	if (callback_type == USB_HOST_CALLBACK_RESET) {
		module_inst->hw->HOST.INTENCLR.reg = USB_HOST_INTENCLR_RST;
	}
	if (callback_type == USB_HOST_CALLBACK_WAKEUP) {
		module_inst->hw->HOST.INTENCLR.reg = USB_HOST_INTENCLR_WAKEUP;
	}
	if (callback_type == USB_HOST_CALLBACK_DNRSM) {
		module_inst->hw->HOST.INTENCLR.reg = USB_HOST_INTENCLR_DNRSM;
	}
	if (callback_type == USB_HOST_CALLBACK_UPRSM) {
		module_inst->hw->HOST.INTENCLR.reg = USB_HOST_INTENCLR_UPRSM;
	}
	if (callback_type == USB_HOST_CALLBACK_RAMACER) {
		module_inst->hw->HOST.INTENCLR.reg = USB_HOST_INTENCLR_RAMACER;
	}
	if (callback_type == USB_HOST_CALLBACK_CONNECT) {
		module_inst->hw->HOST.INTENCLR.reg = USB_HOST_INTENCLR_DCONN;
	}
	if (callback_type == USB_HOST_CALLBACK_DISCONNECT) {
		module_inst->hw->HOST.INTENCLR.reg = USB_HOST_INTENCLR_DDISC;
	}

	return STATUS_OK;
}

void usb_host_pipe_get_config_defaults(struct usb_host_pipe_config *ep_config)
{
	/* Sanity check arguments */
	Assert(ep_config);
	/* Write default config to config struct */
	ep_config->device_address = 0;
	ep_config->endpoint_address = 0;
//	ep_config->bank = 0;
	ep_config->pipe_type = USB_HOST_PIPE_TYPE_CONTROL;
	ep_config->binterval = 0xFF;
	ep_config->size = 8;
	ep_config->pipe_token = USB_HOST_PIPE_TOKEN_SETUP;
}

enum status_code usb_host_pipe_set_config(struct usb_module *module_inst, uint8_t pipe_num,
		struct usb_host_pipe_config *ep_config)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(pipe_num);
	Assert(ep_config);
	/* set pipe config */
	module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.BK = 0;
	module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTYPE = ep_config->pipe_type;
	module_inst->hw->HOST.HostPipe[pipe_num].BINTERVAL.reg =
			ep_config->binterval;
	if (ep_config->endpoint_address == 0) {
		module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN =
				USB_HOST_PCFG_PTOKEN(USB_HOST_PIPE_TOKEN_SETUP);
	} else if (ep_config->endpoint_address & USB_EP_DIR_IN) {
		module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN =
				USB_HOST_PCFG_PTOKEN(USB_HOST_PIPE_TOKEN_IN);
	} else {
		module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN
				= USB_HOST_PCFG_PTOKEN(USB_HOST_PIPE_TOKEN_OUT);
	}
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].CTRL_PIPE.bit.PDADDR =
			ep_config->device_address;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].CTRL_PIPE.bit.PEPNUM =
			ep_config->endpoint_address & USB_EP_ADDR_MASK;
	if (ep_config->size == 1023) {
		usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.SIZE = 0x07;
	} else {
		usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.SIZE = 
				(32 - clz(((uint32_t)min(max(ep_config->size, 8), 1024) << 1) - 1) - 1 - 3);
	}

	return STATUS_OK;
}
enum status_code usb_host_pipe_get_config(struct usb_module *module_inst, uint8_t pipe_num,
		struct usb_host_pipe_config *ep_config)
{
	uint32_t size;

	/* Sanity check arguments */
	Assert(module_inst);
	Assert(pipe_num);
	Assert(ep_config);
	/* get pipe config from setting register */
	ep_config->device_address =
			usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].CTRL_PIPE.bit.PDADDR;
	ep_config->endpoint_address =
			usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].CTRL_PIPE.bit.PEPNUM;

	if (module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN ==
				USB_HOST_PCFG_PTOKEN(USB_HOST_PIPE_TOKEN_IN)) {
		ep_config->endpoint_address = USB_EP_DIR_IN;
	}

//	ep_config->bank = module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.BK;
	ep_config->pipe_type = module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTYPE;
	ep_config->binterval =
			module_inst->hw->HOST.HostPipe[pipe_num].BINTERVAL.reg;
	size = usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.SIZE;
	if (size == 0x07) {
		ep_config->size = 1023;
	} else {
		ep_config->size = (8 << size);
	}
	ep_config->pipe_token = module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN;

	return STATUS_OK;
}

enum status_code usb_host_pipe_register_callback(
		struct usb_module *module_inst, uint8_t pipe_num,
		enum usb_host_pipe_callback callback_type,
		usb_host_pipe_callback_t callback_func)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(pipe_num);
	Assert(callback_func);

	/* Register callback function */
	module_inst->host_pipe_callback[pipe_num][callback_type] = callback_func;

	/* Set the bit corresponding to the callback_type */
	module_inst->host_pipe_registered_callback_mask[pipe_num] |= (1 << callback_type);

	return STATUS_OK;
}
enum status_code usb_host_pipe_unregister_callback(
		struct usb_module *module_inst, uint8_t pipe_num,
		enum usb_host_pipe_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(pipe_num);

	/* Unregister callback function */
	module_inst->host_pipe_callback[pipe_num][callback_type] = NULL;

	/* Clear the bit corresponding to the callback_type */
	module_inst->host_pipe_registered_callback_mask[pipe_num] &= ~(1 << callback_type);

	return STATUS_OK;
}
enum status_code usb_host_pipe_enable_callback(
		struct usb_module *module_inst, uint8_t pipe_num,
		enum usb_host_pipe_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(pipe_num);

	/* Enable callback */
	module_inst->host_pipe_enabled_callback_mask[pipe_num] |= (1 << callback_type);

	if (callback_type == USB_HOST_PIPE_CALLBACK_TRANSFER_COMPLETE) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENSET.reg = USB_HOST_PINTENSET_TRCPT_Msk;
	}
	if (callback_type == USB_HOST_PIPE_CALLBACK_FAIL) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENSET.reg = USB_HOST_PINTENSET_TRFAIL;
	}
	if (callback_type == USB_HOST_PIPE_CALLBACK_ERROR) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENSET.reg = USB_HOST_PINTENSET_PERR;
	}
	if (callback_type == USB_HOST_PIPE_CALLBACK_SETUP) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENSET.reg = USB_HOST_PINTENSET_TXSTP;
	}
	if (callback_type == USB_HOST_PIPE_CALLBACK_STALL) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENSET.reg = USB_HOST_PINTENSET_STALL;
	}

	return STATUS_OK;
}
enum status_code usb_host_pipe_disable_callback(
		struct usb_module *module_inst, uint8_t pipe_num,
		enum usb_host_pipe_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(pipe_num);

	/* Enable callback */
	module_inst->host_pipe_enabled_callback_mask[pipe_num] &= ~(1 << callback_type);

	if (callback_type == USB_HOST_PIPE_CALLBACK_TRANSFER_COMPLETE) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENCLR.reg = USB_HOST_PINTENCLR_TRCPT_Msk;
	}
	if (callback_type == USB_HOST_PIPE_CALLBACK_FAIL) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENCLR.reg = USB_HOST_PINTENCLR_TRFAIL;
	}
	if (callback_type == USB_HOST_PIPE_CALLBACK_ERROR) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENCLR.reg = USB_HOST_PINTENCLR_PERR;
	}
	if (callback_type == USB_HOST_PIPE_CALLBACK_SETUP) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENCLR.reg = USB_HOST_PINTENCLR_TXSTP;
	}
	if (callback_type == USB_HOST_PIPE_CALLBACK_STALL) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENCLR.reg = USB_HOST_PINTENCLR_STALL;
	}

	return STATUS_OK;
}
enum status_code usb_host_pipe_setup_job(struct usb_module *module_inst,
		uint8_t pipe_num, uint8_t *buf, uint32_t buf_size)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(pipe_num);

	/* get pipe config from setting register */
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].ADDR.reg = (uint32_t)buf;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT =
			buf_size;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE =
			0;
	module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN =
			USB_HOST_PCFG_PTOKEN(USB_HOST_PIPE_TOKEN_SETUP);

	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSSET.reg = USB_HOST_PSTATUSSET_BK0RDY;
	usb_host_pipe_unfreeze(module_inst, pipe_num);
	usb_host_pipe_enable_callback(module_inst, 0, USB_HOST_PIPE_CALLBACK_SETUP);

	return STATUS_OK;
}
enum status_code usb_host_pipe_read_job(struct usb_module *module_inst,
		uint8_t pipe_num, uint8_t *buf, uint32_t buf_size)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(pipe_num);

	/* get pipe config from setting register */
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].ADDR.reg = (uint32_t)buf;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT =
			0;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE =
			buf_size;
	module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN =
			USB_HOST_PCFG_PTOKEN(USB_HOST_PIPE_TOKEN_IN);

	// Start transfer
	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_BK0RDY;
	usb_host_pipe_unfreeze(module_inst, pipe_num);

	return STATUS_OK;
}
enum status_code usb_host_pipe_write_job(struct usb_module *module_inst,
		uint8_t pipe_num, uint8_t *buf, uint32_t buf_size)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(pipe_num);

	/* get pipe config from setting register */
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].ADDR.reg = (uint32_t)buf;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT =
			buf_size;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE =
			0;
	module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN =
			USB_HOST_PCFG_PTOKEN(USB_HOST_PIPE_TOKEN_OUT);

	// Start transfer
	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSSET.reg = USB_HOST_PSTATUSSET_BK0RDY;
	usb_host_pipe_unfreeze(module_inst, pipe_num);

	return STATUS_OK;
}
enum status_code usb_host_pipe_abort_job(struct usb_module *module_inst, uint8_t pipe_num)
{
	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSSET.reg = USB_HOST_PSTATUSSET_PFREEZE;
	return STATUS_OK;
}
enum status_code usb_host_pipe_get_job_status(struct usb_module *module_inst, uint8_t pipe_num)
{
	return STATUS_OK;
}

/**
 * \internal
 * \brief Function called by USB interrupt to manage USB host interrupts
 *
 * USB host interrupt events are split into four sections:
 * - USB line events
 *   (VBus error, device dis/connection, SOF, reset, suspend, resume, wakeup)
 * - control endpoint events
 *   (setup reception, end of data transfer, underflow, overflow, stall, error)
 * - bulk/interrupt/isochronous endpoints events
 *   (end of data transfer, stall, error)
 */
static void _usb_host_interrupt_handler(void)
{
	uint32_t pipe_int;
	uint32_t flags;

	// Manage pipe interrupts
	pipe_int = ctz(_usb_instances->hw->HOST.PINTSMRY.reg);
	if (pipe_int < 32) {
		// pipe interrupts

		/* get interrupt flags */
		flags = _usb_instances->hw->HOST.HostPipe[pipe_int].PINTFLAG.reg;

		// host pipe transfer complete interrupt
		if (flags & USB_HOST_PINTFLAG_TRCPT_Msk) {
			// clear the flag
			_usb_instances->hw->HOST.HostPipe[pipe_int].PINTFLAG.reg =
					USB_HOST_PINTFLAG_TRCPT_Msk;
			if(_usb_instances->host_pipe_enabled_callback_mask[pipe_int] &
					(1 << USB_HOST_PIPE_CALLBACK_TRANSFER_COMPLETE)) {
				callback_para.pipe_num = pipe_int;
				if (_usb_instances->hw->HOST.HostPipe[pipe_int].PCFG.bit.PTOKEN ==
							USB_HOST_PCFG_PTOKEN(USB_HOST_PIPE_TOKEN_IN)) {
					// in 
					callback_para.transfer_size = usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT;
					usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT = 0;
				} else {
					// out
					callback_para.transfer_size = usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE;
					usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
					if (0 == callback_para.transfer_size) {
						callback_para.transfer_size = usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT;
					}
				}
				(_usb_instances->host_pipe_callback[pipe_int]
						[USB_HOST_PIPE_CALLBACK_TRANSFER_COMPLETE])(_usb_instances, &callback_para);
			}		
		}

		// host pipe transfer fail interrupt
		if (flags & USB_HOST_PINTFLAG_TRFAIL) {
			// clear the flag
			_usb_instances->hw->HOST.HostPipe[pipe_int].PINTFLAG.reg =
					USB_HOST_PINTFLAG_TRFAIL;
			if(_usb_instances->host_pipe_enabled_callback_mask[pipe_int] &
					(1 << USB_HOST_PIPE_CALLBACK_FAIL)) {
				(_usb_instances->host_pipe_callback[pipe_int]
						[USB_HOST_PIPE_CALLBACK_FAIL])(_usb_instances, NULL);
			}		
		}

		// host pipe error interrupt
		if (flags & USB_HOST_PINTFLAG_PERR) {
			// clear the flag
			_usb_instances->hw->HOST.HostPipe[pipe_int].PINTFLAG.reg =
					USB_HOST_PINTFLAG_PERR;
			if(_usb_instances->host_pipe_enabled_callback_mask[pipe_int] &
					(1 << USB_HOST_PIPE_CALLBACK_ERROR)) {
				callback_para.pipe_num = pipe_int;
				callback_para.error_type = 
						usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].STATUS_PIPE.reg & 0x1F;
				(_usb_instances->host_pipe_callback[pipe_int]
						[USB_HOST_PIPE_CALLBACK_ERROR])(_usb_instances, &callback_para);
			}		
		}

		// host pipe transmitted setup interrupt
		if (flags & USB_HOST_PINTFLAG_TXSTP) {
			// clear the flag
			_usb_instances->hw->HOST.HostPipe[pipe_int].PINTFLAG.reg =
					USB_HOST_PINTFLAG_TXSTP;
			if(_usb_instances->host_pipe_enabled_callback_mask[pipe_int] &
					(1 << USB_HOST_PIPE_CALLBACK_SETUP)) {
				(_usb_instances->host_pipe_callback[pipe_int]
						[USB_HOST_PIPE_CALLBACK_SETUP])(_usb_instances, NULL);
			}		
		}

		// host pipe stall interrupt
		if (flags & USB_HOST_PINTFLAG_STALL) {
			// clear the flag
			_usb_instances->hw->HOST.HostPipe[pipe_int].PINTFLAG.reg =
					USB_HOST_PINTFLAG_STALL;
			if(_usb_instances->host_pipe_enabled_callback_mask[pipe_int] &
					(1 << USB_HOST_PIPE_CALLBACK_STALL)) {
				callback_para.pipe_num = pipe_int;
				(_usb_instances->host_pipe_callback[pipe_int]
						[USB_HOST_PIPE_CALLBACK_STALL])(_usb_instances, &callback_para);
			}		
		}

	} else {
		// host interrupts

		/* get interrupt flags */
		flags = _usb_instances->hw->HOST.INTFLAG.reg;

		// host SOF interrupt
		if (flags & USB_HOST_INTFLAG_HSOF) {
			// clear the flag
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_HSOF;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_SOF)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_SOF])(_usb_instances, NULL);
			}		
		}

		// host reset interrupt		
		if (flags & USB_HOST_INTFLAG_RST) {
			// clear the flag
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_RST;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_RESET)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_RESET])(_usb_instances, NULL);
			}		
		}

		// host wakeup interrupts
		if ((flags & USB_HOST_INTFLAG_WAKEUP) || (flags & USB_HOST_INTFLAG_DNRSM) ||
				(flags & USB_HOST_INTFLAG_UPRSM)) {
			// clear the flags
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_WAKEUP |
					USB_HOST_INTFLAG_DNRSM| USB_HOST_INTFLAG_UPRSM;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_WAKEUP)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_WAKEUP])(_usb_instances, NULL);
			}		
		}
		
		// host ram access interrupt 	
		if (flags & USB_HOST_INTFLAG_RAMACER) {
			// clear the flag
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_RAMACER;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_RAMACER)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_RAMACER])(_usb_instances, NULL);
			}		
		}

		// host connect interrupt
		if (flags & USB_HOST_INTFLAG_DCONN) {
			// clear the flag
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_DCONN;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_CONNECT)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_CONNECT])(_usb_instances, NULL);
			}		
		}
		
		// host disconnect interrupt 	
		if (flags & USB_HOST_INTFLAG_DDISC) {
			// clear the flag
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_DDISC;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_DISCONNECT)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_DISCONNECT])(_usb_instances, NULL);
			}		
		}

	}
}

void usb_host_pipe_set_zlp(struct usb_module *module_inst, uint8_t pipe_num, bool value)
{
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.AUTO_ZLP = value;
}

/********************Third part: USB host end***********************/
/******************************************************************/

void usb_enable(struct usb_module *module_inst)
{
	module_inst->hw->HOST.CTRLA.reg |= USB_CTRLA_ENABLE;
	while (module_inst->hw->HOST.SYNCBUSY.reg == USB_SYNCBUSY_ENABLE);
}
void usb_disable(struct usb_module *module_inst)
{
	module_inst->hw->HOST.CTRLA.reg &= ~USB_CTRLA_ENABLE;
	while (module_inst->hw->HOST.SYNCBUSY.reg == USB_SYNCBUSY_ENABLE);
}

/** Interrupt handler for the USB module. */
void USB_Handler(void)
{
	if (_usb_instances->hw->HOST.CTRLA.bit.MODE) {
		//host mode ISR
		_usb_host_interrupt_handler();
	} else {
		//device mode ISR
	}
}

void usb_get_config_defaults(struct usb_config *module_config)
{
	/* Sanity check arguments */
	Assert(module_config);
	/* Write default config to config struct */
	module_config->mode = 0;
	module_config->run_in_standby = 1;
//	module_config->descriptor_address = usb_descriptor_table.usb_endpoint_table;
}
enum status_code usb_init(struct usb_module *module_inst, Usb *const hw,
		struct usb_config *module_config)
{
	/* Sanity check arguments */
	Assert(hw);
	Assert(module_inst);
	Assert(module_config);

	uint32_t i,j;
	struct system_pinmux_config pin_config;
	struct system_gclk_chan_config gclk_chan_config;

	_usb_instances = module_inst;

	/* Associate the software module instance with the hardware module */
	module_inst->hw = hw;

	/* Turn on the digital interface clock */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBB, PM_APBBMASK_USB);

	/* Set up the USB DP/DN pins */
	system_pinmux_get_config_defaults(&pin_config);
	pin_config.mux_position = MUX_PA24G_USB_DM;
	system_pinmux_pin_set_config(PIN_PA24G_USB_DM, &pin_config);
	pin_config.mux_position = MUX_PA25G_USB_DP;
	system_pinmux_pin_set_config(PIN_PA25G_USB_DP, &pin_config);

	/* Setup clock for module */
	system_gclk_chan_get_config_defaults(&gclk_chan_config);
	gclk_chan_config.source_generator = GCLK_GENERATOR_0;
	system_gclk_chan_set_config(USB_GCLK_ID, &gclk_chan_config);
	system_gclk_chan_enable(USB_GCLK_ID);
	pin_config.mux_position = MUX_PB14H_GCLK_IO0;
	pin_config.direction    = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
	system_pinmux_pin_set_config(PIN_PB14H_GCLK_IO0, &pin_config);

	/* Reset */
	hw->HOST.CTRLA.bit.SWRST = 1;
	while (hw->HOST.SYNCBUSY.bit.SWRST);

	/* Set the configuration */
	hw->HOST.CTRLA.bit.MODE = module_config->mode;
	hw->HOST.CTRLA.bit.RUNSTDBY = module_config->run_in_standby;
	hw->HOST.DESCADD.reg = (uint32_t)(&usb_descriptor_table.usb_endpoint_table[0]);

	// callback related init
	for (i = 0; i < USB_PIPE_NUM; i++) {
		module_inst->host_callback[i] = NULL;
		for (j = 0; j < USB_HOST_CALLBACK_N; j++) {
			module_inst->host_pipe_callback[i][j] = NULL;
		}
	};
	module_inst->host_registered_callback_mask = 0;
	module_inst->host_enabled_callback_mask = 0;
	for (i = 0; i < USB_PIPE_NUM; i++) {
		module_inst->host_pipe_registered_callback_mask[i] = 0;
		module_inst->host_pipe_enabled_callback_mask[i] = 0;
	}

//roeder add device callback related

	/* Enable interrupts for this USB module */
	system_interrupt_enable(SYSTEM_INTERRUPT_MODULE_USB);

	return STATUS_OK;
}


