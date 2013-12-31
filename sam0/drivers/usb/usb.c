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
 * \brief USB Clock Source Selection
 */
#ifndef  CONF_USB_CLOCK_SOURCE
#define  CONF_USB_CLOCK_SOURCE		GCLK_GENERATOR_0
#endif

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
union {
	UsbDeviceDescriptor usb_endpoint_table[USB_EPT_NUM];
	UsbHostDescriptor usb_pipe_table[USB_PIPE_NUM];
} usb_descriptor_table;


static struct usb_module *_usb_instances;

static struct usb_endpoint_callback_parameter ep_callback_para = {
	0,
	0,
	0,
	0,
};

static void _usb_device_interrupt_handler(void);

static const uint16_t _usb_device_irq_bits[USB_DEVICE_CALLBACK_N] = {
	USB_DEVICE_INTFLAG_SUSPEND,
	USB_DEVICE_INTFLAG_MSOF,
	USB_DEVICE_INTFLAG_SOF,
	USB_DEVICE_INTFLAG_EORST,
	USB_DEVICE_INTFLAG_WAKEUP | USB_DEVICE_INTFLAG_EORSM | USB_DEVICE_INTFLAG_UPRSM,
	USB_DEVICE_INTFLAG_RAMACER,
	USB_DEVICE_INTFLAG_LPMNYET,
	USB_DEVICE_INTFLAG_LPMSUSP,
};

static const uint8_t _usb_endpoint_irq_bits[USB_DEVICE_EP_CALLBACK_N] = {
	USB_DEVICE_EPINTFLAG_TRCPT_Msk,
	USB_DEVICE_EPINTFLAG_TRFAIL_Msk,
	USB_DEVICE_EPINTFLAG_RXSTP,
	USB_DEVICE_EPINTFLAG_STALL_Msk
};

/******************************************************************/
/*************First part: USB common start*****************************/
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
		//_usb_host_interrupt_handler();
	} else {
		//device mode ISR
		_usb_device_interrupt_handler();
	}
}

void usb_get_config_defaults(struct usb_config *module_config)
{
	/* Sanity check arguments */
	Assert(module_config);
	/* Write default config to config struct */
	module_config->select_host_mode = 0;
	module_config->run_in_standby = 0;
	module_config->speed_mode = USB_SPEED_FULL;
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
	
	/* Set up the USB DP/DN pins */
	system_pinmux_get_config_defaults(&pin_config);
	pin_config.mux_position = MUX_PA24G_USB_DM;
	system_pinmux_pin_set_config(PIN_PA24G_USB_DM, &pin_config);
	pin_config.mux_position = MUX_PA25G_USB_DP;
	system_pinmux_pin_set_config(PIN_PA25G_USB_DP, &pin_config);
	
	/* Setup clock for module */
	system_gclk_chan_get_config_defaults(&gclk_chan_config);
	gclk_chan_config.source_generator = CONF_USB_CLOCK_SOURCE;
	system_gclk_chan_set_config(USB_GCLK_ID, &gclk_chan_config);
	system_gclk_chan_enable(USB_GCLK_ID);
	
	/* Turn on the digital interface clock */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBB, PM_APBBMASK_USB);
	system_ahb_clock_set_mask(PM_AHBMASK_USB);
	
	/* Reset */
	hw->HOST.CTRLA.bit.SWRST = 1;
	while (hw->HOST.SYNCBUSY.bit.SWRST);
	
	/* Set the configuration */	
	hw->DEVICE.DESCADD.reg = (uint32_t)(&usb_descriptor_table.usb_endpoint_table[0]);
	hw->DEVICE.CTRLA.bit.MODE = module_config->select_host_mode;
	hw->DEVICE.CTRLA.bit.RUNSTDBY = module_config->run_in_standby;

	// device callback related
	for (i = 0; i < USB_DEVICE_CALLBACK_N; i++) {
		module_inst->device_callback[i] = NULL;
	}
	
	for (i = 0; i < USB_EPT_NUM; i++) {
		for(j = 0; j < USB_DEVICE_EP_CALLBACK_N; j++) {
			module_inst->device_endpoint_callback[i][j] = NULL;
		}
	}
	
	module_inst->device_registered_callback_mask = 0;
	module_inst->device_enabled_callback_mask = 0;
	
	for (j = 0; j < USB_EPT_NUM; j++) {
		module_inst->deivce_endpoint_registered_callback_mask[j] = 0;
		module_inst->device_endpoint_enabled_callback_mask[j] = 0;
	}

	if (USB_SPEED_FULL == module_config->speed_mode) {
		module_inst->hw->DEVICE.CTRLB.bit.SPDCONF = USB_DEVICE_CTRLB_SPDCONF_0_Val;
	} else if(USB_SPEED_LOW == module_config->speed_mode) {
			module_inst->hw->DEVICE.CTRLB.bit.SPDCONF = USB_DEVICE_CTRLB_SPDCONF_1_Val;
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
	module_inst->device_registered_callback_mask |= _usb_device_irq_bits[callback_type];

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
	module_inst->device_registered_callback_mask &= ~_usb_device_irq_bits[callback_type];

	return STATUS_OK;
}

enum status_code usb_device_enable_callback(struct usb_module *module_inst,
		enum usb_device_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	
	/* clear related flag */
	module_inst->hw->DEVICE.INTFLAG.reg = _usb_device_irq_bits[callback_type];
	
	/* Enable callback */
	module_inst->device_enabled_callback_mask |= _usb_device_irq_bits[callback_type];

	module_inst->hw->DEVICE.INTENSET.reg = _usb_device_irq_bits[callback_type];
	
	return STATUS_OK;
}

enum status_code usb_device_disable_callback(struct usb_module *module_inst,
		enum usb_device_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);

	/* Disable callback */
	module_inst->device_enabled_callback_mask &= ~_usb_device_irq_bits[callback_type];

	module_inst->hw->DEVICE.INTENCLR.reg = _usb_device_irq_bits[callback_type];
	
	return STATUS_OK;
}


enum status_code usb_device_endpoint_register_callback(
		struct usb_module *module_inst, uint8_t ep_num,
		enum usb_device_endpoint_callback callback_type,
		usb_device_endpoint_callback_t callback_func)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(callback_func);
	
	/* Register callback function */
	module_inst->device_endpoint_callback[ep_num][callback_type] = callback_func;

	/* Set the bit corresponding to the callback_type */
	module_inst->deivce_endpoint_registered_callback_mask[ep_num] |= _usb_endpoint_irq_bits[callback_type];

	return STATUS_OK;
}

enum status_code usb_device_endpoint_unregister_callback(
		struct usb_module *module_inst, uint8_t ep_num,
		enum usb_device_endpoint_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);

	/* Unregister callback function */
	module_inst->device_endpoint_callback[ep_num][callback_type] = NULL;

	/* Clear the bit corresponding to the callback_type */
	module_inst->deivce_endpoint_registered_callback_mask[ep_num] &= ~_usb_endpoint_irq_bits[callback_type];

	return STATUS_OK;
}

enum status_code usb_device_endpoint_enable_callback(
		struct usb_module *module_inst, uint8_t ep,
		enum usb_device_endpoint_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;

	/* Enable callback */
	module_inst->device_endpoint_enabled_callback_mask[ep_num] |= _usb_endpoint_irq_bits[callback_type];

	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_TRCPT) {
		if (ep_num == 0) { // control endpoint
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENSET.reg = USB_DEVICE_EPINTENSET_TRCPT0 | USB_DEVICE_EPINTENSET_TRCPT1;
		} else if (ep & USB_EP_DIR_IN) {
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENSET.reg = USB_DEVICE_EPINTENSET_TRCPT1;
		} else {
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENSET.reg = USB_DEVICE_EPINTENSET_TRCPT0;
		}
	}
	
	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL) {
		if (ep_num == 0) { // control endpoint
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENSET.reg = USB_DEVICE_EPINTENSET_TRFAIL0 | USB_DEVICE_EPINTENSET_TRFAIL1;
		} else if (ep & USB_EP_DIR_IN) {
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENSET.reg = USB_DEVICE_EPINTENSET_TRFAIL1;
		} else {
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENSET.reg = USB_DEVICE_EPINTENSET_TRFAIL0;
		}
	}
	
	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_RXSTP) {
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENSET.reg = USB_DEVICE_EPINTENSET_RXSTP;
	}
	
	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_STALL) {
		if (ep & USB_EP_DIR_IN) {
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENSET.reg = USB_DEVICE_EPINTENSET_STALL1;
		} else {
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENSET.reg = USB_DEVICE_EPINTENSET_STALL0;
		}
	}
	
	return STATUS_OK;
}

enum status_code usb_device_endpoint_disable_callback(
		struct usb_module *module_inst, uint8_t ep,
		enum usb_device_endpoint_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;

	/* Enable callback */
	module_inst->device_endpoint_enabled_callback_mask[ep_num] &= ~_usb_endpoint_irq_bits[callback_type];
	
	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_TRCPT) {
		if (ep_num == 0) { // control endpoint
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENCLR.reg =  USB_DEVICE_EPINTENCLR_TRCPT0 | USB_DEVICE_EPINTENCLR_TRCPT1;
		} else if (ep & USB_EP_DIR_IN) {
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENCLR.reg =  USB_DEVICE_EPINTENCLR_TRCPT1;
		} else {
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENCLR.reg =  USB_DEVICE_EPINTENCLR_TRCPT0;
		}
	}
	
	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL) {
		if (ep_num == 0) { // control endpoint
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENCLR.reg = USB_DEVICE_EPINTENCLR_TRFAIL0 | USB_DEVICE_EPINTENCLR_TRFAIL1;
		} else if (ep & USB_EP_DIR_IN) {
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENCLR.reg = USB_DEVICE_EPINTENCLR_TRFAIL1;
		} else {
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENCLR.reg = USB_DEVICE_EPINTENCLR_TRFAIL0;
		}
	}
	
	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_RXSTP) {
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENCLR.reg = USB_DEVICE_EPINTENCLR_RXSTP;
	}
	
	if (callback_type == USB_DEVICE_ENDPOINT_CALLBACK_STALL) {
		if (ep & USB_EP_DIR_IN) {
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENCLR.reg = USB_DEVICE_EPINTENCLR_STALL1;
		} else {
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTENCLR.reg = USB_DEVICE_EPINTENCLR_STALL0;
		}
	}
	
	return STATUS_OK;
}

void usb_device_endpoint_get_config_defaults(struct usb_device_endpoint_config *ep_config)
{
	/* Sanity check arguments */
	Assert(ep_config);
	
	/* Write default config to config struct */
	ep_config->ep_address = 0;
	ep_config->ep_size = USB_ENDPOINT_8_BYTE;
	ep_config->auto_zlp = false;
	ep_config->ep_type = USB_DEVICE_ENDPOINT_TYPE_CONTROL;
}

enum status_code usb_device_endpoint_set_config(struct usb_module *module_inst, 
		struct usb_device_endpoint_config *ep_config)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(ep_config);
	
	uint8_t ep_num = ep_config->ep_address & USB_EP_ADDR_MASK;
	uint8_t ep_bank = (ep_config->ep_address & USB_EP_DIR_IN) ? 1 : 0;
	
	switch (ep_config->ep_type) {
		case USB_DEVICE_ENDPOINT_TYPE_DISABLE:
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0(0) |  USB_DEVICE_EPCFG_EPTYPE1(0);
			return STATUS_OK;
			
		case USB_DEVICE_ENDPOINT_TYPE_CONTROL:
			if ((module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg & USB_DEVICE_EPCFG_EPTYPE0_Msk) == 0 && \
				(module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg & USB_DEVICE_EPCFG_EPTYPE1_Msk) == 0) {
				module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0(1) | USB_DEVICE_EPCFG_EPTYPE1(1);
				module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_BK0RDY; 
				module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK1RDY;
			} else {
				return STATUS_ERR_DENIED;
			}
			if (true == ep_config->auto_zlp) { 
				usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[0].PCKSIZE.reg |= USB_DEVICE_PCKSIZE_AUTO_ZLP;
				usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[1].PCKSIZE.reg |= USB_DEVICE_PCKSIZE_AUTO_ZLP;
			} else {
				usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[0].PCKSIZE.reg &= ~USB_DEVICE_PCKSIZE_AUTO_ZLP;
				usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[1].PCKSIZE.reg &= ~USB_DEVICE_PCKSIZE_AUTO_ZLP;
			}
			usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[0].PCKSIZE.bit.SIZE = ep_config->ep_size;
			usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[1].PCKSIZE.bit.SIZE = ep_config->ep_size;
			return STATUS_OK;
			
		case USB_DEVICE_ENDPOINT_TYPE_ISOCHRONOUS:
			if (ep_bank) {
				if ((module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg & USB_DEVICE_EPCFG_EPTYPE1_Msk) == 0){
					module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE1(2);
					module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK1RDY;
				} else {
					return STATUS_ERR_DENIED;
				}
			} else {
				if ((module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg & USB_DEVICE_EPCFG_EPTYPE0_Msk) == 0){
					module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0(2);
					module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_BK0RDY;
				} else {
					return STATUS_ERR_DENIED;
				}
			}
			break;
			
		case USB_DEVICE_ENDPOINT_TYPE_BULK:
			if (ep_bank) {
				if ((module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg & USB_DEVICE_EPCFG_EPTYPE1_Msk) == 0){
					module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE1(3);
					module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK1RDY;
				} else {
					return STATUS_ERR_DENIED;
				}
			} else {
				if ((module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg & USB_DEVICE_EPCFG_EPTYPE0_Msk) == 0){
					module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0(3);
					module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_BK0RDY;
				} else {
					return STATUS_ERR_DENIED;
				}
			}
			break;
			
		case USB_DEVICE_ENDPOINT_TYPE_INTERRUPT:
			if (ep_bank) {
				if ((module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg & USB_DEVICE_EPCFG_EPTYPE1_Msk) == 0){
					module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE1(4);
					module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK1RDY;
				} else {
					return STATUS_ERR_DENIED;
				}
			} else {
				if ((module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg & USB_DEVICE_EPCFG_EPTYPE0_Msk) == 0){
					module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0(4);
					module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_BK0RDY;
				} else {
					return STATUS_ERR_DENIED;
				}
			}			
			break;
			
		default:
			break;
	}

	usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[ep_bank].PCKSIZE.bit.SIZE = ep_config->ep_size;

	if (true == ep_config->auto_zlp) {
		usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[ep_bank].PCKSIZE.reg |= USB_DEVICE_PCKSIZE_AUTO_ZLP;
		} else {
		usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[ep_bank].PCKSIZE.reg &= ~USB_DEVICE_PCKSIZE_AUTO_ZLP;
	}

	return STATUS_OK;
}


void usb_ep_abort(uint8_t ep)
{
	uint8_t ep_num;
	ep_num = ep & USB_EP_ADDR_MASK;
	
	// Stop transfer
	if (ep & USB_EP_DIR_IN) {
		_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK1RDY;
		// Eventually ack a transfer occured during abort
		_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1;
	} else {
		_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_BK0RDY;
		// Eventually ack a transfer occured during abort
		_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0;
	}
}

bool usb_ep_is_halted(uint8_t ep, bool* ep_enable)
{
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;
	uint8_t flag;
	
	if (ep & USB_EP_DIR_IN) {
		if( NULL != ep_enable) {
			flag = (uint8_t)(_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.bit.EPTYPE1);
			*ep_enable = ((enum usb_device_endpoint_type)(flag) != USB_DEVICE_ENDPOINT_TYPE_DISABLE);
		}
		return (_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUS.reg & USB_DEVICE_EPSTATUSSET_STALLRQ1);
	} else {
		if (NULL != ep_enable) {
			flag = (uint8_t)(_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.bit.EPTYPE0);
			*ep_enable = ((enum usb_device_endpoint_type)(flag) != USB_DEVICE_ENDPOINT_TYPE_DISABLE);
		}
		return (_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUS.reg & USB_DEVICE_EPSTATUSSET_STALLRQ0);
	}
}

void usb_ep_set_halt(uint8_t ep)
{
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;

	// Stall endpoint
	if (ep & USB_EP_DIR_IN) {
		_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_STALLRQ1;
	} else {
		_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_STALLRQ0;
	}
}

void usb_ep_clear_halt(uint8_t ep)
{
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;
	
	if (ep & USB_EP_DIR_IN) {
		if (_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUS.reg & USB_DEVICE_EPSTATUSSET_STALLRQ1) {
			// Remove stall request
			_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_STALLRQ1;
			if (_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_STALL1) {
				_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL1;
				// The Stall has occurred, then reset data toggle
				_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSSET_DTGLIN;
			}
		}
	} else {
		if (_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUS.reg & USB_DEVICE_EPSTATUSSET_STALLRQ0) {
			// Remove stall request
			_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_STALLRQ0;
			if (_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_STALL0) {
				_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL0;
				// The Stall has occurred, then reset data toggle
				_usb_instances->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSSET_DTGLOUT;
			}
		}
	}
}

enum status_code usb_device_endpoint_write_buffer_job(struct usb_module *module_inst,uint8_t ep_num,
		uint8_t* pbuf, uint32_t buf_size)
{
	/* Sanity check arguments */
	Assert(module_inst);
	
	uint8_t flag;
	flag = (uint8_t)(module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.bit.EPTYPE1);
	if ((enum usb_device_endpoint_type)(flag) == USB_DEVICE_ENDPOINT_TYPE_DISABLE) {
		return STATUS_ERR_DENIED;
	};
	
	/* get pipe config from setting register */
	usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[1].ADDR.reg = (uint32_t)pbuf;
	usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[1].PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
	usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[1].PCKSIZE.bit.BYTE_COUNT = buf_size;
	module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK0RDY;
	module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_BK1RDY;

	return STATUS_OK;
}

enum status_code usb_device_endpoint_read_buffer_job(struct usb_module *module_inst,uint8_t ep_num,
		uint8_t* pbuf, uint32_t buf_size)
{
	/* Sanity check arguments */
	Assert(module_inst);
	
	uint8_t flag;
	flag = (uint8_t)(module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.bit.EPTYPE0);
	if ((enum usb_device_endpoint_type)(flag) == USB_DEVICE_ENDPOINT_TYPE_DISABLE) {
		return STATUS_ERR_DENIED;
	};
	
	/* get pipe config from setting register */
	usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[0].ADDR.reg = (uint32_t)pbuf;
	usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE = buf_size;
	usb_descriptor_table.usb_endpoint_table[ep_num].DeviceDescBank[0].PCKSIZE.bit.BYTE_COUNT = 0;
	module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK0RDY;
	
	return STATUS_OK;
}

enum status_code usb_device_endpoint_setup_buffer_job(struct usb_module *module_inst,
		uint8_t* pbuf)
{
	/* Sanity check arguments */
	Assert(module_inst);
   
	/* get pipe config from setting register */
	usb_descriptor_table.usb_endpoint_table[0].DeviceDescBank[0].ADDR.reg = (uint32_t)pbuf;
	usb_descriptor_table.usb_endpoint_table[0].DeviceDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE = 8;
	usb_descriptor_table.usb_endpoint_table[0].DeviceDescBank[0].PCKSIZE.bit.BYTE_COUNT = 0;
	module_inst->hw->DEVICE.DeviceEndpoint[0].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK0RDY; 
	
	return STATUS_OK;
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
		
		// device sof interrupt
		if (flags & USB_DEVICE_INTFLAG_SOF) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_SOF;
			if(_usb_instances->device_enabled_callback_mask & (_usb_device_irq_bits[USB_DEVICE_CALLBACK_SOF])) {
				(_usb_instances->device_callback[USB_DEVICE_CALLBACK_SOF])(_usb_instances);
			}
			return;
		}
		
		// device end of reset interrupt
		if (flags & USB_DEVICE_INTFLAG_EORST) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_EORST;
			if(_usb_instances->device_enabled_callback_mask & (_usb_device_irq_bits[USB_DEVICE_CALLBACK_RESET])) {
				(_usb_instances->device_callback[USB_DEVICE_CALLBACK_RESET])(_usb_instances);
			}
			return;
		}
		
		// device suspend interrupt
		if (flags & USB_DEVICE_INTFLAG_SUSPEND) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_SUSPEND;
			if(_usb_instances->device_enabled_callback_mask & (_usb_device_irq_bits[USB_DEVICE_CALLBACK_SUSPEND])) {
				(_usb_instances->device_callback[USB_DEVICE_CALLBACK_SUSPEND])(_usb_instances);
			}		
			return;
		}
		
		// device resume interrupt
		if ((flags & USB_DEVICE_INTFLAG_WAKEUP) || (flags & USB_DEVICE_INTFLAG_UPRSM) ||  (flags & USB_DEVICE_INTFLAG_EORSM)) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_WAKEUP | USB_DEVICE_INTFLAG_UPRSM | USB_DEVICE_INTFLAG_EORSM;
			if(_usb_instances->device_enabled_callback_mask & (_usb_device_irq_bits[USB_DEVICE_CALLBACK_RESUME])) {
				(_usb_instances->device_callback[USB_DEVICE_CALLBACK_RESUME])(_usb_instances);
			}
			return;
		}
		
		// device ram access interrupt
		if (flags & USB_DEVICE_INTFLAG_RAMACER) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_RAMACER;
			if(_usb_instances->device_enabled_callback_mask & (_usb_device_irq_bits[USB_DEVICE_CALLBACK_RAMACER])) {
				(_usb_instances->device_callback[USB_DEVICE_CALLBACK_RAMACER])(_usb_instances);
			}
			return;
		}
		
		// device lpm suspend interrupt
		if (flags & USB_DEVICE_INTFLAG_LPMSUSP) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_LPMSUSP;
			if(_usb_instances->device_enabled_callback_mask & (_usb_device_irq_bits[USB_DEIVCE_CALLBACK_LPMSUSP])) {
				(_usb_instances->device_callback[USB_DEIVCE_CALLBACK_LPMSUSP])(_usb_instances);
			}
			return;
		}
		
		// device lpm not yet interrupt
		if (flags & USB_DEVICE_INTFLAG_LPMNYET) {
			// clear the flag
			_usb_instances->hw->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_LPMNYET;
			if(_usb_instances->device_enabled_callback_mask & (_usb_device_irq_bits[USB_DEIVCE_CALLBACK_LPMNYET])) {
				(_usb_instances->device_callback[USB_DEIVCE_CALLBACK_LPMNYET])(_usb_instances);
			}
			return;
		}
		
	} else {
	/* endpoint interrupt */
		for (uint8_t i = 0; i < USB_EPT_NUM; i++) {
			if (ep_inst & (1 << i)) {
				flags = _usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg;

				// endpoint transfer stall interrupt
				if (flags & USB_DEVICE_EPINTFLAG_STALL_Msk) {
					// clear the flag
					if (_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_STALL1) {
						_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL1;
						ep_callback_para.endpoint_address = USB_EP_DIR_IN | i;
					} else if (_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_STALL0) {
						_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL0;
						ep_callback_para.endpoint_address = USB_EP_DIR_OUT | i;
					}
					
					if (_usb_instances->device_endpoint_enabled_callback_mask[i] & _usb_endpoint_irq_bits[USB_DEVICE_ENDPOINT_CALLBACK_STALL]) {
						(_usb_instances->device_endpoint_callback[i][USB_DEVICE_ENDPOINT_CALLBACK_STALL])(_usb_instances,&ep_callback_para);
					}
					return;
				}
				
				// endpoint received setup interrupt
				if (flags & USB_DEVICE_EPINTFLAG_RXSTP) {
					// clear the flag
					_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_RXSTP;
					if(_usb_instances->device_endpoint_enabled_callback_mask[i] & _usb_endpoint_irq_bits[USB_DEVICE_ENDPOINT_CALLBACK_RXSTP]) {
						ep_callback_para.received_bytes = (uint16_t)(usb_descriptor_table.usb_endpoint_table[i].DeviceDescBank[0].PCKSIZE.bit.BYTE_COUNT);
						(_usb_instances->device_endpoint_callback[i][USB_DEVICE_ENDPOINT_CALLBACK_RXSTP])(_usb_instances,&ep_callback_para);
					}
					return;
				}
				
				// endpoint transfer fail interrupt
				if (flags & USB_DEVICE_EPINTFLAG_TRFAIL_Msk) {
					// clear the flag
					if (_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_TRFAIL1) {
						_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRFAIL1;
						if (usb_descriptor_table.usb_endpoint_table[i].DeviceDescBank[1].STATUS_BK.reg & USB_DEVICE_STATUS_BK_ERRORFLOW) {
							usb_descriptor_table.usb_endpoint_table[i].DeviceDescBank[1].STATUS_BK.reg &= ~USB_DEVICE_STATUS_BK_ERRORFLOW;
						}
						ep_callback_para.endpoint_address = USB_EP_DIR_IN | i;
						if (_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_TRCPT1) {
							return;
						}
					} else if(_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_TRFAIL0) {
						_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRFAIL0;
						if (usb_descriptor_table.usb_endpoint_table[i].DeviceDescBank[0].STATUS_BK.reg & USB_DEVICE_STATUS_BK_ERRORFLOW) {
							usb_descriptor_table.usb_endpoint_table[i].DeviceDescBank[0].STATUS_BK.reg &= ~USB_DEVICE_STATUS_BK_ERRORFLOW;
						} 
						ep_callback_para.endpoint_address = USB_EP_DIR_OUT | i;
						if (_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_TRCPT0) {
							return;
						}
					}
					
					if(_usb_instances->device_endpoint_enabled_callback_mask[i] & _usb_endpoint_irq_bits[USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL]) {
						(_usb_instances->device_endpoint_callback[i][USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL])(_usb_instances,&ep_callback_para);
					}
					return;
				}
				
				// endpoint transfer complete interrupt
				if (flags & USB_DEVICE_EPINTFLAG_TRCPT_Msk) {
					// clear the flag
					//_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT_Msk;
					if (_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_TRCPT1) {
						_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1;
						ep_callback_para.endpoint_address = USB_EP_DIR_IN | i;
						ep_callback_para.sent_bytes = (uint16_t)(usb_descriptor_table.usb_endpoint_table[i].DeviceDescBank[1].PCKSIZE.bit.BYTE_COUNT);
						
					} else if (_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_TRCPT0) {
						_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0;
						ep_callback_para.endpoint_address = USB_EP_DIR_OUT | i;
						ep_callback_para.received_bytes = (uint16_t)(usb_descriptor_table.usb_endpoint_table[i].DeviceDescBank[0].PCKSIZE.bit.BYTE_COUNT);
						ep_callback_para.out_buffer_size = (uint16_t)(usb_descriptor_table.usb_endpoint_table[i].DeviceDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE);
					}
					if(_usb_instances->device_endpoint_enabled_callback_mask[i] & _usb_endpoint_irq_bits[USB_DEVICE_ENDPOINT_CALLBACK_TRCPT]) {
						(_usb_instances->device_endpoint_callback[i][USB_DEVICE_ENDPOINT_CALLBACK_TRCPT])(_usb_instances,&ep_callback_para);
					}
					return;
				}
			}
		}
	}
}

/********************Second part: USB device end***********************/
/******************************************************************/
