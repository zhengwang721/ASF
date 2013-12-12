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
	static volatile UsbDeviceDescriptor usb_endpoint_table[USB_EPT_NUM];
	static volatile UsbHostDescriptor usb_pipe_table[USB_PIPE_NUM];
} usb_descriptor_table;
//COMPILER_PACK_RESET()

//usb_host_callback_t host_callback[USB_HOST_CALLBACK_N];
//usb_host_pipe_callback_t host_pipe_callback[USB_PIPE_NUM][USB_HOST_PIPE_CALLBACK_N];

void temp()
{
}

struct usb_module *_usb_instances;



/******************************************************************/
/*************First part: USB common start*****************************/

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
	module_config->descriptor_address = usb_descriptor_table.usb_endpoint_table;
}
enum status_code usb_init(struct usb_module *module_inst, Usb *const hw,
		struct usb_config *module_config)
{
	uint32_t i,j;

	/* Sanity check arguments */
	Assert(hw);
	Assert(module_inst);
	Assert(module_config);

	_usb_instances = module_inst;

	/* Associate the software module instance with the hardware module */
	module_inst->hw = hw;

	/* Turn on the digital interface clock */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBB, PM_APBBMASK_USB);

	/* Reset */
	hw->HOST.CTRLA.bit.SWRST = 1;
	while (hw->HOST.SYNCBUSY.bit.SWRST);

	/* Set the configuration */
	hw->HOST.CTRLA.bit.MODE = module_config->mode;
	hw->HOST.CTRLA.bit.RUNSTDBY = module_config->run_in_standby;
	hw->HOST.DESCADD = module_config->descriptor_address;

	// callback related init
	for (i = 0; i < USB_HOST_CALLBACK_N; i++) {
		module_inst->host_callback[i] = NULL;
		for (j = 0; j < USB_PIPE_NUM; j++) {
			module_inst->host_pipe_callback[i] = NULL;
		}
	};
	module_inst->host_registered_callback_mask = 0;
	module_inst->host_enabled_callback_mask = 0;
	for (j = 0; j < USB_PIPE_NUM; j++) {
		module_inst->host_pipe_registered_callback_mask[j] = 0;
		module_inst->host_pipe_enabled_callback_mask[j] = 0;
	}

	// device callback related
	for (i = 0; i < USB_DEVICE_CALLBACK_N; i++) {
		module_inst->device_callback[i] = NULL;
		for (j = 0; j < USB_EP_NUM; j++) {
			module_inst->device_endpoint_callback[i] = NULL;
		}
	};
	module_inst->device_registered_callback_mask = 0;
	module_inst->device_enabled_callback_mask = 0;
	for (j = 0; j < USB_EP_NUM; j++) {
		module_inst->deivce_endpoint_registered_callback_mask[j] = 0;
		module_inst->device_endpoint_enabled_callback_mask[j] = 0;
	}
	
	/* Enable interrupts for this USB module */
	system_interrupt_enable(SYSTEM_INTERRUPT_MODULE_USB);

	return STATUS_OK;
}

/********************First part: USB common end***********************/
/******************************************************************/

/******************************************************************/
/*************Second part: USB device start*****************************/
enum status_code usb_device_register_callback(struct usb_module *module_inst,
		enum usb_device_callback callback_type,
		usb_device_callback_t callback_func)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(callback_func);

	/* Register callback function */
	module_inst->device_callback[callback_type] = callback_func;

	/* Set the bit corresponding to the callback_type */
	module_inst->device_registered_callback_mask |= (1 << callback_type);

	return STATUS_OK;
}
enum status_code usb_device_unregister_callback(struct usb_module *module_inst,
		enum usb_device_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);

	/* Unregister callback function */
	module_inst->device_callback[callback_type] = NULL;

	/* Clear the bit corresponding to the callback_type */
	module_inst->device_registered_callback_mask &= ~(1 << callback_type);

	return STATUS_OK;
}

enum status_code usb_device_enable_callback(struct usb_module *module_inst,
		enum usb_device_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);

	/* Enable callback */
	module_inst->device_enabled_callback_mask |= (1 << callback_type);

	if (callback_type == USB_DEVICE_CALLBACK_SUSPEND) {
		module_inst->hw->DEVICE.INTENSET.reg = USB_DEVICE_CALLBACK_SUSPEND;
	}
	if (callback_type == USB_DEVICE_CALLBACK_SOF) {
		module_inst->hw->DEVICE.INTENSET.reg = USB_DEVICE_CALLBACK_SOF;
	}
	if (callback_type == USB_DEVICE_CALLBACK_RESET) {
		module_inst->hw->DEVICE.INTENSET.reg = USB_DEVICE_CALLBACK_RESET;
	}
	if (callback_type == USB_DEVICE_CALLBACK_WAKEUP) {
		module_inst->hw->DEVICE.INTENSET.reg = USB_DEVICE_CALLBACK_WAKEUP;
	}
	if (callback_type == USB_DEVICE_CALLBACK_EORSM) {
		module_inst->hw->DEVICE.INTENSET.reg = USB_DEVICE_CALLBACK_EORSM;
	}
	if (callback_type == USB_DEVICE_CALLBACK_UPRSM) {
		module_inst->hw->DEVICE.INTENSET.reg = USB_DEVICE_CALLBACK_UPRSM;
	}
	if (callback_type == USB_DEVICE_CALLBACK_RAMACER) {
		module_inst->hw->DEVICE.INTENSET.reg = USB_DEVICE_CALLBACK_RAMACER;
	}
	if (callback_type == USB_DEIVCE_CALLBACK_LPMNYET) {
		module_inst->hw->DEVICE.INTENSET.reg = USB_DEIVCE_CALLBACK_LPMNYET;
	}
	if (callback_type == USB_DEIVCE_CALLBACK_LPMSUSP) {
		module_inst->hw->DEVICE.INTENSET.reg = USB_DEIVCE_CALLBACK_LPMSUSP;
	}
	return STATUS_OK;
}
enum status_code usb_device_disable_callback(struct usb_module *module_inst,
		enum usb_device_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);

	/* Disable callback */
	module_inst->enabled_callback_mask &= ~(1 << callback_type);

	if (callback_type == USB_DEVICE_CALLBACK_SUSPEND) {
		module_inst->hw->DEVICE.INTENCLR.reg = USB_DEVICE_CALLBACK_SUSPEND;
	}
	if (callback_type == USB_DEVICE_CALLBACK_SOF) {
		module_inst->hw->DEVICE.INTENCLR.reg = USB_DEVICE_CALLBACK_SOF;
	}
	if (callback_type == USB_DEVICE_CALLBACK_RESET) {
		module_inst->hw->DEVICE.INTENCLR.reg = USB_DEVICE_CALLBACK_RESET;
	}
	if (callback_type == USB_DEVICE_CALLBACK_WAKEUP) {
		module_inst->hw->DEVICE.INTENCLR.reg = USB_DEVICE_CALLBACK_WAKEUP;
	}
	if (callback_type == USB_DEVICE_CALLBACK_EORSM) {
		module_inst->hw->DEVICE.INTENCLR.reg = USB_DEVICE_CALLBACK_EORSM;
	}
	if (callback_type == USB_DEVICE_CALLBACK_UPRSM) {
		module_inst->hw->DEVICE.INTENCLR.reg = USB_DEVICE_CALLBACK_UPRSM;
	}
	if (callback_type == USB_DEVICE_CALLBACK_RAMACER) {
		module_inst->hw->DEVICE.INTENCLR.reg = USB_DEVICE_CALLBACK_RAMACER;
	}
	if (callback_type == USB_DEIVCE_CALLBACK_LPMNYET) {
		module_inst->hw->DEVICE.INTENCLR.reg = USB_DEIVCE_CALLBACK_LPMNYET;
	}
	if (callback_type == USB_DEIVCE_CALLBACK_LPMSUSP) {
		module_inst->hw->DEVICE.INTENCLR.reg = USB_DEIVCE_CALLBACK_LPMSUSP;
	}
	return STATUS_OK;
}

void usb_device_get_config_defaults(struct usb_device_config *dev_config)
{
	/* Sanity check arguments */
	Assert(dev_config);
	/* Write default config to config struct */
	dev_config->device_speed = USB_SPEED_FULL;
	dev_config->lpm_mode = USB_LPM_NOT_SUPPORT;
}

enum status_code usb_device_get_config(struct usb_module *module_inst,
		struct usb_device_config *dev_config)
{
	Assert(module_inst);
	Assert(dev_config);

	dev_config->device_speed = usb_device_get_speed(module_inst);
	dev_config->lpm_mode = (enum usb_lpm_mode)(((module_inst->hw.CTRLB.reg) & \
								USB_DEVICE_CTRLB_LPMHDSK_Msk) >> USB_DEVICE_CTRLB_LPMHDSK_Pos);
	return STATUS_OK;
}

enum status_code usb_device_endpoint_register_callback(
		struct usb_module *module_inst, uint8_t ep_num,
		enum usb_device_endpoint_callback callback_type,
		usb_device_endpoint_callback_t callback_func)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(ep_num);
	Assert(callback_func);
	
	/* Register callback function */
	module_inst->device_endpoint_callback[ep_num][callback_type] = callback_func;

	/* Set the bit corresponding to the callback_type */
	module_inst->deivce_endpoint_registered_callback_mask[ep_num] |= (1 << callback_type);

	return STATUS_OK;
}

enum status_code usb_device_endpoint_unregister_callback(
		struct usb_module *module_inst, uint8_t ep_num,
		enum usb_device_endpoint_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(ep_num);

	/* Unregister callback function */
	module_inst->device_endpoint_callback[ep_num][callback_type] = NULL;

	/* Clear the bit corresponding to the callback_type */
	module_inst->deivce_endpoint_registered_callback_mask[ep_num] &= ~(1 << callback_type);

	return STATUS_OK;
}

enum status_code usb_device_endpoint_enable_callback(
		struct usb_module *module_inst, uint8_t ep_num,
		enum usb_device_endpoint_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(ep_num);

	/* Enable callback */
	module_inst->device_endpoint_enabled_callback_mask[ep_num] |= (1 << callback_type);

	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_TRCPT) {
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENSET.reg = USB_DEVICE_EPINTENSET_TRCPT_Msk;
	}
	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL) {
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENSET.reg = USB_DEVICE_EPINTENSET_TRFAIL_Msk;
	}
	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_RXSTP) {
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENSET.reg = USB_DEVICE_EPINTENSET_RXSTP;
	}
	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_STALL) {
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENSET.reg = USB_DEVICE_EPINTENSET_STALL_Msk;
	}
	
	return STATUS_OK;
}

enum status_code usb_device_endpoint_disable_callback(
		struct usb_module *module_inst, uint8_t ep_num,
		enum usb_device_endpoint_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(ep_num);

	/* Enable callback */
	module_inst->device_endpoint_enabled_callback_mask[ep_num] &= ~(1 << callback_type);

	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_TRCPT) {
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].PINTENCLR.reg = USB_DEVICE_EPINTENSET_TRCPT_Msk;
	}
	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL) {
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].PINTENCLR.reg = USB_DEVICE_EPINTENSET_TRFAIL_Msk;
	}
	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_RXSTP) {
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].PINTENCLR.reg = USB_DEVICE_EPINTENSET_RXSTP;
	}
	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_STALL) {
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].PINTENCLR.reg = USB_DEVICE_EPINTENSET_STALL_Msk;
	}
	
	return STATUS_OK;
}

void usb_device_endpoint_get_config_defaults(struct usb_device_endpoint_config *ep_config)
{
	/* Sanity check arguments */
	Assert(ep_config);
	
	/* Write default config to config struct */
	ep_config->ep_address = 0;
	ep_config->ep_size = 8;
	ep_config->auto_zlp = false;
	ep_config->bk_ep_type = USB_DEVICE_ENDPOINT_TYPE_CONTROL_SETUP;
}

enum status_code usb_device_endpoint_set_config(struct usb_module *module_inst, uint8_t ep_num,
		struct usb_device_endpoint_config *ep_config)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(ep_num);
	Assert(ep_config);
	
	usb_descriptor_table.usb_endpoint_table[ep_num].UsbDeviceDescBank[0].PCKSIZE.bit.SIZE = ep_config->ep_size;

	if (true == ep_config->auto_zlp) {
		usb_descriptor_table.usb_endpoint_table[ep_num].HostDescBank[0].PCKSIZE.bit.AUTO_ZLP = 1;
	} else {
		usb_descriptor_table.usb_endpoint_table[ep_num].HostDescBank[0].PCKSIZE.bit.AUTO_ZLP = 0;
	}
	
	usb_device_set_address(module_inst,ep_config->ep_address);
	
	switch (ep_config->ep_type) {
		case USB_DEVICE_ENDPOINT_TYPE_CONTROL_SETUP:
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0(1);
			break;
		case USB_DEVICE_ENDPOINT_TYPE_CONTROL_IN:
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE1(1);
			break;
		case USB_DEVICE_ENDPOINT_TYPE_CONTROL_OUT:
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0(1);
			break;
		case USB_DEVICE_ENDPOINT_TYPE_ISOCHRONOUS_IN:
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE1(2);
			break;
		case USB_DEVICE_ENDPOINT_TYPE_ISOCHRONOUS_OUT:
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0(2);
			break;
		case USB_DEVICE_ENDPOINT_TYPE_BULK_IN:
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE1(3);
			break;
		case USB_DEVICE_ENDPOINT_TYPE_BULK_OUT:
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0(3);
			break;
		case USB_DEVICE_ENDPOINT_TYPE_INTERRUPT_IN:
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE1(4);
			break;
		case USB_DEVICE_ENDPOINT_TYPE_INTERRUPT_OUT:
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0(4);
			break;
		case USB_DEVICE_ENDPOINT_TYPE_DUALBANK_IN:
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0(5);
			break;
		case USB_DEVICE_ENDPOINT_TYPE_DUALBANK_OUT:
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE1(5);
			break;
		default:
			break;
	}
	
	return STATUS_OK;
}

enum status_code usb_device_endpoint_write_buffer_job(struct usb_module *module_inst,uint8_t ep_num,
		uint8_t* pbuf, uint32_t buf_size)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(ep_num);

	/* get pipe config from setting register */
	usb_descriptor_table.usb_endpoint_table[ep_num].UsbDeviceDescBank[0].ADDR = pbuf;
	usb_descriptor_table.usb_endpoint_table[ep_num].UsbDeviceDescBank[0].PCKSIZE.bit.BYTE_COUNT =
			buf_size;

	return STATUS_OK;
}

enum status_code usb_device_endpoint_read__buffer_job(struct usb_module *module_inst,uint8_t ep_num,
		uint8_t* pbuf, uint32_t buf_size)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(ep_num);

	/* get pipe config from setting register */
	usb_descriptor_table.usb_endpoint_table[ep_num].UsbDeviceDescBank[0].ADDR = pbuf;
	usb_descriptor_table.usb_endpoint_table[ep_num].UsbDeviceDescBank[0].PCKSIZE.bit.BYTE_COUNT =
			buf_size;

	return STATUS_OK;
}

enum status_code usb_device_endpoint_setup_buffer_job(struct usb_module *module_inst,uint8_t ep_num,
		uint8_t* pbuf, uint32_t buf_size)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(ep_num);

	/* get pipe config from setting register */
	usb_descriptor_table.usb_endpoint_table[ep_num].UsbDeviceDescBank[0].ADDR = pbuf;
	usb_descriptor_table.usb_endpoint_table[ep_num].UsbDeviceDescBank[0].PCKSIZE.bit.BYTE_COUNT =
			buf_size;

	return STATUS_OK;
}

enum status_code usb_device_endpoint_abort_job(struct usb_module *module_inst,uint8_t ep_num)
{
	// to be check, whether it is necessary?
}

static void _usb_device_interrupt_handler(void)
{
	uint16_t ep_inst;
	uint16_t flags;

	ep_inst = _usb_instances->hw->DEVICE.EPINTSMRY.reg;
	
	/* device interrupt */
	if ( 0 == ep_inst) {
	/* get interrupt flags */
		flags = _usb_instances->hw->DEVICE.INTFLAG.reg;

		// device suspend interrupt
		if (flags & USB_DEVICE_INTFLAG_SUSPEND) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_SUSPEND;
			if(_usb_instances->device_enabled_callback_mask & (1 << USB_DEVICE_CALLBACK_SUSPEND)) {
				(_usb_instances->device_callback[USB_DEVICE_CALLBACK_SUSPEND])();
			}		
		}
		// device sof interrupt
		if (flags & USB_DEVICE_INTFLAG_SOF) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_SOF;
			if(_usb_instances->device_enabled_callback_mask & (1 << USB_DEVICE_CALLBACK_SOF)) {
				(_usb_instances->device_callback[USB_DEVICE_CALLBACK_SOF])();
			}		
		}
		// device end of reset interrupt
		if (flags & USB_DEVICE_INTFLAG_EORST) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_EORST;
			if(_usb_instances->device_enabled_callback_mask & (1 << USB_DEVICE_CALLBACK_RESET)) {
				(_usb_instances->device_callback[USB_DEVICE_CALLBACK_RESET])();
			}		
		}
		// device wakeup interrupt
		if (flags & USB_DEVICE_INTFLAG_WAKEUP) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_WAKEUP;
			if(_usb_instances->device_enabled_callback_mask & (1 << USB_DEVICE_CALLBACK_WAKEUP)) {
				(_usb_instances->device_callback[USB_DEVICE_CALLBACK_WAKEUP])();
			}		
		}
		// device end of resume interrupt
		if (flags & USB_DEVICE_INTFLAG_EORSM) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_EORSM;
			if(_usb_instances->device_enabled_callback_mask & (1 << USB_DEVICE_CALLBACK_EORSM)) {
				(_usb_instances->device_callback[USB_DEVICE_CALLBACK_EORSM])();
			}		
		}
		// device upstream resume interrupt
		if (flags & USB_DEVICE_INTFLAG_UPRSM) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_UPRSM;
			if(_usb_instances->device_enabled_callback_mask & (1 << USB_DEVICE_CALLBACK_UPRSM)) {
				(_usb_instances->device_callback[USB_DEVICE_CALLBACK_UPRSM])();
			}		
		}
		// device ram access interrupt
		if (flags & USB_DEVICE_INTFLAG_RAMACER) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_RAMACER;
			if(_usb_instances->device_enabled_callback_mask & (1 << USB_DEVICE_CALLBACK_RAMACER)) {
				(_usb_instances->device_callback[USB_DEVICE_CALLBACK_RAMACER])();
			}		
		}
		// device lpm not yet interrupt
		if (flags & USB_DEVICE_INTFLAG_LPMNYET) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_LPMNYET;
			if(_usb_instances->device_enabled_callback_mask & (1 << USB_DEIVCE_CALLBACK_LPMNYET)) {
				(_usb_instances->device_callback[USB_DEIVCE_CALLBACK_LPMNYET])();
			}		
		}
		// device lpm suspend interrupt
		if (flags & USB_DEVICE_INTFLAG_LPMSUSP) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_LPMSUSP;
			if(_usb_instances->device_enabled_callback_mask & (1 << USB_DEIVCE_CALLBACK_LPMSUSP)) {
				(_usb_instances->device_callback[USB_DEIVCE_CALLBACK_LPMSUSP])();
			}		
		}
	} else {
	/* endpoint interrupt */
		for (uint8_t i; i < USB_EP_NUM; i++) {
			if (ep_inst & (1 << i)) {
				flags = _usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg;

				// endpoint transfer complete interrupt
				if (flags & USB_DEVICE_EPINTFLAG_TRCPT_Msk) {
					// clear the flag
					_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT_Msk;
					if(_usb_instances->host_pipe_enabled_callback_mask[i] &
							(1 << USB_DEVICE_ENDPOINT_CALLBACK_TRCPT)) {
						(_usb_instances->host_pipe_callback[i][USB_DEVICE_ENDPOINT_CALLBACK_TRCPT])();
					}
				}
				// endpoint transfer fail interrupt
				if (flags & USB_DEVICE_EPINTFLAG_TRFAIL_Msk) {
					// clear the flag
					_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRFAIL_Msk;
					if(_usb_instances->host_pipe_enabled_callback_mask[i] &
							(1 << USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL)) {
						(_usb_instances->host_pipe_callback[i][USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL])();
					}
				}
				// endpoint received setup interrupt
				if (flags & USB_DEVICE_EPINTFLAG_RXSTP) {
					// clear the flag
					_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_RXSTP;
					if(_usb_instances->host_pipe_enabled_callback_mask[i] &
							(1 << USB_DEVICE_ENDPOINT_CALLBACK_RXSTP)) {
						(_usb_instances->host_pipe_callback[i][USB_DEVICE_ENDPOINT_CALLBACK_RXSTP])();
					}
				}
				// endpoint transfer stall interrupt
				if (flags & USB_DEVICE_EPINTFLAG_STALL_Msk) {
					// clear the flag
					_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL_Msk;
					if(_usb_instances->host_pipe_enabled_callback_mask[i] &
							(1 << USB_DEVICE_ENDPOINT_CALLBACK_STALL)) {
						(_usb_instances->host_pipe_callback[i][USB_DEVICE_ENDPOINT_CALLBACK_STALL])();
					}
				}
			}
		}
	}
}

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
	module_inst->enabled_callback_mask &= ~(1 << callback_type);

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
	ep_config->ep_number = 0;
	ep_config->bank = 0;
	ep_config->pipe_type = USB_HOST_PIPE_TYPE_CONTROL;
	ep_config->binterval = 0xFF;
	ep_config->size = 8;
}

enum status_code usb_host_pipe_set_config(struct usb_module *module_inst, uint8_t pipe_num,
		struct usb_host_pipe_config *ep_config)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(pipe_num);
	Assert(ep_config);
	/* get pipe config from setting register */
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].CTRL_PIPE.bit.PDADDR =
			ep_config->device_address;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].CTRL_PIPE.bit.PEPNUM =
			ep_config->ep_number;
	module_inst->hw.HOST.HostPipe[pipe_num].PCFG.bit.BK = ep_config->bank;
	module_inst->hw.HOST.HostPipe[pipe_num].PCFG.bit.PTYPE = ep_config->pipe_type;
	module_inst->hw.HOST.HostPipe[pipe_num].BINTERVAL =
			ep_config->binterval;
	if (ep_config->size == 1023) {
		usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.SIZE = 0x07;
	} else {
		usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.SIZE = 
				(32 - clz(((uint32_t)min(max(size, 8), 1024) << 1) - 1) - 1 - 3);
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
	ep_config->ep_number =
			usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].CTRL_PIPE.bit.PEPNUM;
	ep_config->bank = module_inst->hw.HOST.HostPipe[pipe_num].PCFG.bit.BK;
	ep_config->pipe_type = module_inst->hw.HOST.HostPipe[pipe_num].PCFG.bit.PTYPE;
	ep_config->binterval =
			module_inst->hw.HOST.HostPipe[pipe_num].BINTERVAL;
	size = usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.SIZE;
	if (size == 0x07) {
		ep_config->size = 1023;
	} else {
		ep_config->size = (8 << size);
	}

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
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].ADDR = buf;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT =
			buf_size;
	module_inst->hw.HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN = USB_HOST_PCFG_PTOKEN(0);

	return STATUS_OK;
}
enum status_code usb_host_pipe_read_job(struct usb_module *module_inst,
		uint8_t pipe_num, uint8_t *buf, uint32_t buf_size)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(pipe_num);

	/* get pipe config from setting register */
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].ADDR = buf;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT =
			buf_size;
	module_inst->hw.HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN = USB_HOST_PCFG_PTOKEN(1);

	return STATUS_OK;
}
enum status_code usb_host_pipe_write_job(struct usb_module *module_inst,
		uint8_t pipe_num, uint8_t *buf, uint32_t buf_size)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(pipe_num);

	/* get pipe config from setting register */
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].ADDR = buf;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT =
			buf_size;
	module_inst->hw.HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN = USB_HOST_PCFG_PTOKEN(2);

	return STATUS_OK;
}
enum status_code usb_host_pipe_abort_job(struct usb_module *module_inst, uint8_t pipe_num)
{
	module_inst.hw->HOST.HostPipe[pipe_num].PSTATUSSET.reg = USB_HOST_PSTATUSSET_PFREEZE;
	return STATUS_OK;
}
enum status_code usb_host_pipe_get_job_status(struct usb_module *module_inst, uint8_t pipe_num)
{
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
				(_usb_instances->host_pipe_callback[pipe_int]
						[USB_HOST_PIPE_CALLBACK_TRANSFER_COMPLETE])();
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
						[USB_HOST_PIPE_CALLBACK_FAIL])();
			}		
		}

		// host pipe error interrupt
		if (flags & USB_HOST_PINTFLAG_PERR) {
			// clear the flag
			_usb_instances->hw->HOST.HostPipe[pipe_int].PINTFLAG.reg =
					USB_HOST_PINTFLAG_PERR;
			if(_usb_instances->host_pipe_enabled_callback_mask[pipe_int] &
					(1 << USB_HOST_PIPE_CALLBACK_ERROR)) {
				(_usb_instances->host_pipe_callback[pipe_int]
						[USB_HOST_PIPE_CALLBACK_ERROR])();
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
						[USB_HOST_PIPE_CALLBACK_SETUP])();
			}		
		}

		// host pipe stall interrupt
		if (flags & USB_HOST_PINTFLAG_STALL) {
			// clear the flag
			_usb_instances->hw->HOST.HostPipe[pipe_int].PINTFLAG.reg =
					USB_HOST_PINTFLAG_STALL;
			if(_usb_instances->host_pipe_enabled_callback_mask[pipe_int] &
					(1 << USB_HOST_PIPE_CALLBACK_STALL)) {
				(_usb_instances->host_pipe_callback[pipe_int]
						[USB_HOST_PIPE_CALLBACK_STALL])();
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
				(_usb_instances->host_callback[USB_HOST_CALLBACK_SOF])();
			}		
		}

		// host reset interrupt		
		if (flags & USB_HOST_INTFLAG_RST) {
			// clear the flag
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_RST;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_RESET)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_RESET])();
			}		
		}

		// host wakeup interrupts
		if ((flags & USB_HOST_INTFLAG_WAKEUP) || (flags & USB_HOST_INTFLAG_DNRSM) ||
				(flags & USB_HOST_INTFLAG_UPRSM)) {
			// clear the flags
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_HSOF |
					USB_HOST_INTFLAG_DNRSM| USB_HOST_INTFLAG_UPRSM;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_WAKEUP)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_WAKEUP])();
			}		
		}
		
		// host ram access interrupt 	
		if (flags & USB_HOST_INTFLAG_RAMACER) {
			// clear the flag
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_RAMACER;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_RAMACER)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_RAMACER])();
			}		
		}

		// host connect interrupt
		if (flags & USB_HOST_INTFLAG_DCONN) {
			// clear the flag
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_DCONN;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_CONNECT)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_CONNECT])();
			}		
		}
		
		// host disconnect interrupt 	
		if (flags & USB_HOST_INTFLAG_DDISC) {
			// clear the flag
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_DDISC;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_DISCONNECT)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_DISCONNECT])();
			}		
		}

	}
}


/********************Third part: USB host end***********************/
/******************************************************************/


