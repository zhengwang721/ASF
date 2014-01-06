/**
 * \file
 *
 * \brief SAMD21 USB Driver.
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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
 * \brief Define macros for USB device that is not realized in head file
 */
#define USB_DEVICE_EPINTENCLR_TRCPT0        USB_DEVICE_EPINTENCLR_TRCPT(1)
#define USB_DEVICE_EPINTENCLR_TRCPT1        USB_DEVICE_EPINTENCLR_TRCPT(2)
#define USB_DEVICE_EPINTENCLR_TRFAIL0       USB_DEVICE_EPINTENCLR_TRFAIL(1)
#define USB_DEVICE_EPINTENCLR_TRFAIL1       USB_DEVICE_EPINTENCLR_TRFAIL(2)
#define USB_DEVICE_EPINTENCLR_STALL0        USB_DEVICE_EPINTENCLR_STALL(1)
#define USB_DEVICE_EPINTENCLR_STALL1        USB_DEVICE_EPINTENCLR_STALL(2)

#define USB_DEVICE_EPINTENSET_TRCPT0        USB_DEVICE_EPINTENSET_TRCPT(1)
#define USB_DEVICE_EPINTENSET_TRCPT1        USB_DEVICE_EPINTENSET_TRCPT(2)
#define USB_DEVICE_EPINTENSET_TRFAIL0       USB_DEVICE_EPINTENSET_TRFAIL(1)
#define USB_DEVICE_EPINTENSET_TRFAIL1       USB_DEVICE_EPINTENSET_TRFAIL(2)
#define USB_DEVICE_EPINTENSET_STALL0        USB_DEVICE_EPINTENSET_STALL(1)
#define USB_DEVICE_EPINTENSET_STALL1        USB_DEVICE_EPINTENSET_STALL(2)

#define USB_DEVICE_EPINTFLAG_TRCPT0         USB_DEVICE_EPINTFLAG_TRCPT(1)
#define USB_DEVICE_EPINTFLAG_TRCPT1         USB_DEVICE_EPINTFLAG_TRCPT(2)
#define USB_DEVICE_EPINTFLAG_TRFAIL0        USB_DEVICE_EPINTFLAG_TRFAIL(1)
#define USB_DEVICE_EPINTFLAG_TRFAIL1        USB_DEVICE_EPINTFLAG_TRFAIL(2)
#define USB_DEVICE_EPINTFLAG_STALL0         USB_DEVICE_EPINTFLAG_STALL(1)
#define USB_DEVICE_EPINTFLAG_STALL1         USB_DEVICE_EPINTFLAG_STALL(2)

#define USB_DEVICE_EPSTATUSSET_STALLRQ0     USB_DEVICE_EPSTATUSSET_STALLRQ(1)
#define USB_DEVICE_EPSTATUSSET_STALLRQ1     USB_DEVICE_EPSTATUSSET_STALLRQ(2)
#define USB_DEVICE_EPSTATUSCLR_STALLRQ0     USB_DEVICE_EPSTATUSCLR_STALLRQ(1)
#define USB_DEVICE_EPSTATUSCLR_STALLRQ1     USB_DEVICE_EPSTATUSCLR_STALLRQ(2)

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

static struct usb_module *_usb_instances;

static struct usb_pipe_callback_parameter callback_para;

static struct usb_endpoint_callback_parameter ep_callback_para;

static const uint16_t _usb_device_irq_bits[USB_DEVICE_CALLBACK_N] = {
	USB_DEVICE_INTFLAG_SOF,
	USB_DEVICE_INTFLAG_EORST,
	USB_DEVICE_INTFLAG_WAKEUP | USB_DEVICE_INTFLAG_EORSM | USB_DEVICE_INTFLAG_UPRSM,
	USB_DEVICE_INTFLAG_RAMACER,
	USB_DEVICE_INTFLAG_SUSPEND,
	USB_DEVICE_INTFLAG_LPMNYET,
	USB_DEVICE_INTFLAG_LPMSUSP,
};

static const uint8_t _usb_endpoint_irq_bits[USB_DEVICE_EP_CALLBACK_N] = {
	USB_DEVICE_EPINTFLAG_TRCPT_Msk,
	USB_DEVICE_EPINTFLAG_TRFAIL_Msk,
	USB_DEVICE_EPINTFLAG_RXSTP,
	USB_DEVICE_EPINTFLAG_STALL_Msk
};

/** Bit mask for pipe job busy status */
uint32_t host_pipe_job_busy_status = 0;

COMPILER_PACK_SET(1)
COMPILER_WORD_ALIGNED
union {
	UsbDeviceDescriptor usb_endpoint_table[USB_EPT_NUM];
	UsbHostDescriptor usb_pipe_table[USB_PIPE_NUM];
} usb_descriptor_table;
COMPILER_PACK_RESET()

/**
 * \brief Registers a USB host callback
 *
 * Registers a callback function which is implemented by the user.
 *
 * \note The callback must be enabled by \ref usb_host_enable_callback,
 * in order for the interrupt handler to call it when the conditions for the
 * callback type is met.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     callback_type Callback type given by an enum
 * \param[in]     callback_func Pointer to callback function
 *
 * \return Status of the registration operation.
 * \retval STATUS_OK    The callback was registered successfully.
 */
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

/**
 * \brief Unregisters a USB host callback
 *
 * Unregisters an asynchronous callback implemented by the user. Removing it
 * from the internal callback registration table.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     callback_type Callback type given by an enum
 *
 * \return Status of the de-registration operation.
 * \retval STATUS_OK    The callback was unregistered successfully.
 */
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

/**
 * \brief Enables USB host callback generation for a given type.
 *
 * Enables asynchronous callbacks for a given logical type.
 * This must be called before usb host generate callback events.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     callback_type Callback type given by an enum
 *
 * \return Status of the callback enable operation.
 * \retval STATUS_OK    The callback was enabled successfully.
 */
enum status_code usb_host_enable_callback(struct usb_module *module_inst,
		enum usb_host_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

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

/**
 * \brief Disables USB host callback generation for a given type.
 *
 * Disables asynchronous callbacks for a given logical type.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     callback_type Callback type given by an enum
 *
 * \return Status of the callback disable operation.
 * \retval STATUS_OK    The callback was disabled successfully.
 */
enum status_code usb_host_disable_callback(struct usb_module *module_inst,
		enum usb_host_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

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

/**
 * \brief Initializes an USB host pipe configuration structure to defaults.
 *
 * Initializes a given USB host pipe configuration structure to a
 * set of known default values. This function should be called on all new
 * instances of these configuration structures before being modified by the
 * user application.
 *
 * The default configuration is as follows:
 * \li device address is 0
 * \li endpoint address is 0
 * \li pipe type is control
 * \li interval is 0
 * \li pipe size is 8
 *
 * \param[out] ep_config  Configuration structure to initialize to default values
 */
void usb_host_pipe_get_config_defaults(struct usb_host_pipe_config *ep_config)
{
	/* Sanity check arguments */
	Assert(ep_config);
	/* Write default config to config struct */
	ep_config->device_address = 0;
	ep_config->endpoint_address = 0;
	ep_config->pipe_type = USB_HOST_PIPE_TYPE_CONTROL;
	ep_config->binterval = 0;
	ep_config->size = 8;
}

/**
 * \brief Writes an USB host pipe configuration to the hardware module.
 *
 * Writes out a given configuration of an USB host pipe
 * configuration to the hardware module. If the pipe is already configured,
 * the new configuration will replace the existing one.
 *
 * \param[in] module_inst    Pointer to USB software instance struct
 * \param[in] pipe_num       Pipe to configure
 * \param[in] ep_config      Configuration settings for the pipe
 *
 * \return Status of the host pipe configuration operation.
 * \retval STATUS_OK    The host pipe was configurated successfully.
 */
enum status_code usb_host_pipe_set_config(struct usb_module *module_inst, uint8_t pipe_num,
		struct usb_host_pipe_config *ep_config)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);
	Assert(pipe_num < USB_PIPE_NUM);
	Assert(ep_config);

	/* set pipe config */
	module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.BK = 0;
	module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTYPE = ep_config->pipe_type;
	module_inst->hw->HOST.HostPipe[pipe_num].BINTERVAL.reg =
			ep_config->binterval;
	if (ep_config->endpoint_address == 0) {
		module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN =
				USB_HOST_PIPE_TOKEN_SETUP;
	} else if (ep_config->endpoint_address & USB_EP_DIR_IN) {
		module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN =
				USB_HOST_PIPE_TOKEN_IN;
		module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSSET.reg =
				USB_HOST_PSTATUSSET_BK0RDY;
	} else {
		module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN =
				USB_HOST_PIPE_TOKEN_OUT;
		module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSCLR.reg =
				USB_HOST_PSTATUSCLR_BK0RDY;
	}

	memset((uint8_t *)&usb_descriptor_table.usb_pipe_table[pipe_num], 0,
			sizeof(usb_descriptor_table.usb_pipe_table[0]));
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

	/* Clear busy status */
	host_pipe_job_busy_status &= ~(1 << pipe_num);

	return STATUS_OK;
}

/**
 * \brief Gets an USB host pipe configuration.
 *
 * Gets out the configuration of an USB host pipe from the hardware module.
 *
 * \param[in] module_inst    Pointer to USB software instance struct
 * \param[in] pipe_num       Pipe to configure
 * \param[out] ep_config     Configuration settings for the pipe
 *
 * \return Status of the get host pipe configuration operation.
 * \retval STATUS_OK    The host pipe configuration was read successfully.
 */
enum status_code usb_host_pipe_get_config(struct usb_module *module_inst, uint8_t pipe_num,
		struct usb_host_pipe_config *ep_config)
{
	uint32_t size;

	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);
	Assert(pipe_num < USB_PIPE_NUM);
	Assert(ep_config);
	/* get pipe config from setting register */
	ep_config->device_address =
			usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].CTRL_PIPE.bit.PDADDR;
	ep_config->endpoint_address =
			usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].CTRL_PIPE.bit.PEPNUM;

	if (module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN ==
				USB_HOST_PIPE_TOKEN_IN) {
		ep_config->endpoint_address |= USB_EP_DIR_IN;
	}

	ep_config->pipe_type = module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTYPE;
	ep_config->binterval =
			module_inst->hw->HOST.HostPipe[pipe_num].BINTERVAL.reg;
	size = usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.SIZE;
	if (size == 0x07) {
		ep_config->size = 1023;
	} else {
		ep_config->size = (8 << size);
	}

	return STATUS_OK;
}

/**
 * \brief Registers a USB host pipe callback
 *
 * Registers a callback function which is implemented by the user.
 *
 * \note The callback must be enabled by \ref usb_host_pipe_enable_callback,
 * in order for the interrupt handler to call it when the conditions for the
 * callback type is met.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     pipe_num      Pipe to configure
 * \param[in]     callback_type Callback type given by an enum
 * \param[in]     callback_func Pointer to callback function
 *
 * \return Status of the registration operation.
 * \retval STATUS_OK    The callback was registered successfully.
 */
enum status_code usb_host_pipe_register_callback(
		struct usb_module *module_inst, uint8_t pipe_num,
		enum usb_host_pipe_callback callback_type,
		usb_host_pipe_callback_t callback_func)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(pipe_num < USB_PIPE_NUM);
	Assert(callback_func);

	/* Register callback function */
	module_inst->host_pipe_callback[pipe_num][callback_type] = callback_func;

	/* Set the bit corresponding to the callback_type */
	module_inst->host_pipe_registered_callback_mask[pipe_num] |= (1 << callback_type);

	return STATUS_OK;
}

/**
 * \brief Unregisters a USB host pipe callback
 *
 * Unregisters an asynchronous callback implemented by the user. Removing it
 * from the internal callback registration table.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     pipe_num      Pipe to configure
 * \param[in]     callback_type Callback type given by an enum
 *
 * \return Status of the de-registration operation.
 * \retval STATUS_OK    The callback was unregistered successfully.
 */
enum status_code usb_host_pipe_unregister_callback(
		struct usb_module *module_inst, uint8_t pipe_num,
		enum usb_host_pipe_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(pipe_num < USB_PIPE_NUM);

	/* Unregister callback function */
	module_inst->host_pipe_callback[pipe_num][callback_type] = NULL;

	/* Clear the bit corresponding to the callback_type */
	module_inst->host_pipe_registered_callback_mask[pipe_num] &= ~(1 << callback_type);

	return STATUS_OK;
}

/**
 * \brief Enables USB host pipe callback generation for a given type.
 *
 * Enables asynchronous callbacks for a given logical type.
 * This must be called before usb host pipe generate callback events.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     pipe_num      Pipe to configure
 * \param[in]     callback_type Callback type given by an enum
 *
 * \return Status of the callback enable operation.
 * \retval STATUS_OK    The callback was enabled successfully.
 */
enum status_code usb_host_pipe_enable_callback(
		struct usb_module *module_inst, uint8_t pipe_num,
		enum usb_host_pipe_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);
	Assert(pipe_num < USB_PIPE_NUM);

	/* Enable callback */
	module_inst->host_pipe_enabled_callback_mask[pipe_num] |= (1 << callback_type);

	if (callback_type == USB_HOST_PIPE_CALLBACK_TRANSFER_COMPLETE) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENSET.reg = USB_HOST_PINTENSET_TRCPT_Msk;
	}
	if (callback_type == USB_HOST_PIPE_CALLBACK_ERROR) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENSET.reg =
				USB_HOST_PINTENSET_TRFAIL | USB_HOST_PINTENSET_PERR;
	}
	if (callback_type == USB_HOST_PIPE_CALLBACK_SETUP) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENSET.reg = USB_HOST_PINTENSET_TXSTP;
	}
	if (callback_type == USB_HOST_PIPE_CALLBACK_STALL) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENSET.reg = USB_HOST_PINTENSET_STALL;
	}

	return STATUS_OK;
}

/**
 * \brief Disables USB host callback generation for a given type.
 *
 * Disables asynchronous callbacks for a given logical type.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     pipe_num      Pipe to configure
 * \param[in]     callback_type Callback type given by an enum
 *
 * \return Status of the callback disable operation.
 * \retval STATUS_OK    The callback was disabled successfully.
 */
enum status_code usb_host_pipe_disable_callback(
		struct usb_module *module_inst, uint8_t pipe_num,
		enum usb_host_pipe_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);
	Assert(pipe_num < USB_PIPE_NUM);

	/* Enable callback */
	module_inst->host_pipe_enabled_callback_mask[pipe_num] &= ~(1 << callback_type);

	if (callback_type == USB_HOST_PIPE_CALLBACK_TRANSFER_COMPLETE) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENCLR.reg = USB_HOST_PINTENCLR_TRCPT_Msk;
	}
	if (callback_type == USB_HOST_PIPE_CALLBACK_ERROR) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENCLR.reg =
				USB_HOST_PINTENCLR_TRFAIL| USB_HOST_PINTENCLR_PERR;
	}
	if (callback_type == USB_HOST_PIPE_CALLBACK_SETUP) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENCLR.reg = USB_HOST_PINTENCLR_TXSTP;
	}
	if (callback_type == USB_HOST_PIPE_CALLBACK_STALL) {
		module_inst->hw->HOST.HostPipe[pipe_num].PINTENCLR.reg = USB_HOST_PINTENCLR_STALL;
	}

	return STATUS_OK;
}

/**
 * \brief Sends the setup package.
 *
 * Sends the setup package.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     pipe_num      Pipe to configure
 *
 * \return Status of the setup operation.
 * \retval STATUS_OK    The setup job was set successfully.
 * \retval STATUS_ERR_NOT_INITIALIZED    The pipe has not been configurated.
 */
enum status_code usb_host_pipe_setup_job(struct usb_module *module_inst,
		uint8_t pipe_num, uint8_t *buf)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);
	Assert(pipe_num < USB_PIPE_NUM);

	if (host_pipe_job_busy_status & (1 << pipe_num)) {
		return STATUS_BUSY;
	}

	/* Set busy status */
	host_pipe_job_busy_status |= 1 << pipe_num;

	if (module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTYPE ==
			USB_HOST_PIPE_TYPE_DISABLE) {
		return STATUS_ERR_NOT_INITIALIZED;
	}

	/* get pipe config from setting register */
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].ADDR.reg = (uint32_t)buf;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT =
			8;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE =
			0;
	module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN =
			USB_HOST_PIPE_TOKEN_SETUP;

	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSSET.reg = USB_HOST_PSTATUSSET_BK0RDY;
	usb_host_pipe_unfreeze(module_inst, pipe_num);

	return STATUS_OK;
}
enum status_code usb_host_pipe_read_job(struct usb_module *module_inst,
		uint8_t pipe_num, uint8_t *buf, uint32_t buf_size)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);
	Assert(pipe_num < USB_PIPE_NUM);

	if (host_pipe_job_busy_status & (1 << pipe_num)) {
		return STATUS_BUSY;
	}

	/* Set busy status */
	host_pipe_job_busy_status |= 1 << pipe_num;

	if (module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTYPE ==
			USB_HOST_PIPE_TYPE_DISABLE) {
		return STATUS_ERR_NOT_INITIALIZED;
	}

	/* get pipe config from setting register */
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].ADDR.reg = (uint32_t)buf;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT =
			0;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE =
			buf_size;
	module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN =
			USB_HOST_PIPE_TOKEN_IN;

	/* Start transfer */
	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_BK0RDY;
	usb_host_pipe_unfreeze(module_inst, pipe_num);

	return STATUS_OK;
}
enum status_code usb_host_pipe_write_job(struct usb_module *module_inst,
		uint8_t pipe_num, uint8_t *buf, uint32_t buf_size)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);
	Assert(pipe_num < USB_PIPE_NUM);

	if (host_pipe_job_busy_status & (1 << pipe_num)) {
		return STATUS_BUSY;
	}

	/* Set busy status */
	host_pipe_job_busy_status |= 1 << pipe_num;

	if (module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTYPE ==
			USB_HOST_PIPE_TYPE_DISABLE) {
		return STATUS_ERR_NOT_INITIALIZED;
	}

	/* get pipe config from setting register */
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].ADDR.reg = (uint32_t)buf;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT =
			buf_size;
	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE =
			0;
	module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN =
			USB_HOST_PIPE_TOKEN_OUT;

	/* Start transfer */
	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSSET.reg = USB_HOST_PSTATUSSET_BK0RDY;
	usb_host_pipe_unfreeze(module_inst, pipe_num);

	return STATUS_OK;
}
enum status_code usb_host_pipe_abort_job(struct usb_module *module_inst, uint8_t pipe_num)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);
	Assert(pipe_num < USB_PIPE_NUM);

	if (module_inst->hw->HOST.HostPipe[pipe_num].PCFG.bit.PTYPE ==
			USB_HOST_PIPE_TYPE_DISABLE) {
		return STATUS_ERR_NOT_INITIALIZED;
	}

	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSSET.reg = USB_HOST_PSTATUSSET_PFREEZE;

	/* Clear busy status */
	host_pipe_job_busy_status &= ~(1 << pipe_num);

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

	/* Manage pipe interrupts */
	pipe_int = ctz(_usb_instances->hw->HOST.PINTSMRY.reg);
	if (pipe_int < 32) {
		/* pipe interrupts */

		/* get interrupt flags */
		flags = _usb_instances->hw->HOST.HostPipe[pipe_int].PINTFLAG.reg;

		/* host pipe transfer complete interrupt */
		if (flags & USB_HOST_PINTFLAG_TRCPT_Msk) {
			/* Clear busy status */
			host_pipe_job_busy_status &= ~(1 << pipe_int);
			/* clear the flag */
			_usb_instances->hw->HOST.HostPipe[pipe_int].PINTFLAG.reg =
					USB_HOST_PINTFLAG_TRCPT_Msk;
			if(_usb_instances->host_pipe_enabled_callback_mask[pipe_int] &
					(1 << USB_HOST_PIPE_CALLBACK_TRANSFER_COMPLETE)) {
				callback_para.pipe_num = pipe_int;
				if (_usb_instances->hw->HOST.HostPipe[pipe_int].PCFG.bit.PTOKEN ==
							USB_HOST_PIPE_TOKEN_IN) {
					/* in  */
					callback_para.transfered_size = usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT;
					callback_para.required_size = usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE;
					usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT = 0;
				} else {
					/* out */
					callback_para.transfered_size = usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE;
					callback_para.required_size = usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT;
					usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
					if (0 == callback_para.transfered_size) {
						callback_para.transfered_size = usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT;
					}
				}
				(_usb_instances->host_pipe_callback[pipe_int]
						[USB_HOST_PIPE_CALLBACK_TRANSFER_COMPLETE])(_usb_instances, &callback_para);
			}
		}

		/* host pipe transfer fail interrupt */
		if (flags & USB_HOST_PINTFLAG_TRFAIL) {
			/* Clear busy status */
			host_pipe_job_busy_status &= ~(1 << pipe_int);
			/* clear the flag */
			_usb_instances->hw->HOST.HostPipe[pipe_int].PINTFLAG.reg =
					USB_HOST_PINTFLAG_TRFAIL;
		}

		/* host pipe error interrupt */
		if (flags & USB_HOST_PINTFLAG_PERR) {
			/* Clear busy status */
			host_pipe_job_busy_status &= ~(1 << pipe_int);
			/* clear the flag */
			_usb_instances->hw->HOST.HostPipe[pipe_int].PINTFLAG.reg =
					USB_HOST_PINTFLAG_PERR;
			if(_usb_instances->host_pipe_enabled_callback_mask[pipe_int] &
					(1 << USB_HOST_PIPE_CALLBACK_ERROR)) {
				callback_para.pipe_num = pipe_int;
				callback_para.pipe_error_status =
						usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].STATUS_PIPE.reg & 0x1F;
				(_usb_instances->host_pipe_callback[pipe_int]
						[USB_HOST_PIPE_CALLBACK_ERROR])(_usb_instances, &callback_para);
			}
		}

		/* host pipe transmitted setup interrupt */
		if (flags & USB_HOST_PINTFLAG_TXSTP) {
			/* Clear busy status */
			host_pipe_job_busy_status &= ~(1 << pipe_int);
			/* clear the flag */
			_usb_instances->hw->HOST.HostPipe[pipe_int].PINTFLAG.reg =
					USB_HOST_PINTFLAG_TXSTP;
			if(_usb_instances->host_pipe_enabled_callback_mask[pipe_int] &
					(1 << USB_HOST_PIPE_CALLBACK_SETUP)) {
				callback_para.pipe_num = pipe_int;
				callback_para.transfered_size = usb_descriptor_table.usb_pipe_table[pipe_int].HostDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE;
				(_usb_instances->host_pipe_callback[pipe_int]
						[USB_HOST_PIPE_CALLBACK_SETUP])(_usb_instances, NULL);
			}
		}

		/* host pipe stall interrupt */
		if (flags & USB_HOST_PINTFLAG_STALL) {
			/* Clear busy status */
			host_pipe_job_busy_status &= ~(1 << pipe_int);
			/* clear the flag */
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
		/* host interrupts */

		/* get interrupt flags */
		flags = _usb_instances->hw->HOST.INTFLAG.reg;

		/* host SOF interrupt */
		if (flags & USB_HOST_INTFLAG_HSOF) {
			/* clear the flag */
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_HSOF;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_SOF)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_SOF])(_usb_instances);
			}
		}

		/* host reset interrupt */
		if (flags & USB_HOST_INTFLAG_RST) {
			/* Clear busy status */
			host_pipe_job_busy_status = 0;
			/* clear the flag */
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_RST;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_RESET)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_RESET])(_usb_instances);
			}
		}

		/* host wakeup interrupts */
		if (flags & USB_HOST_INTFLAG_WAKEUP) {
			/* clear the flags */
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_WAKEUP;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_WAKEUP)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_WAKEUP])(_usb_instances);
			}
		}

		/* host downstream resume interrupts */
		if (flags & USB_HOST_INTFLAG_DNRSM) {
			/* clear the flags */
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_DNRSM;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_DNRSM)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_DNRSM])(_usb_instances);
			}
		}

		/* host upstream resume interrupts */
		if (flags & USB_HOST_INTFLAG_UPRSM) {
			/* clear the flags */
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_UPRSM;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_UPRSM)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_UPRSM])(_usb_instances);
			}
		}

		/* host ram access interrupt  */
		if (flags & USB_HOST_INTFLAG_RAMACER) {
			/* Clear busy status */
			host_pipe_job_busy_status = 0;
			/* clear the flag */
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_RAMACER;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_RAMACER)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_RAMACER])(_usb_instances);
			}
		}

		/* host connect interrupt */
		if (flags & USB_HOST_INTFLAG_DCONN) {
			/* Clear busy status */
			host_pipe_job_busy_status = 0;
			/* clear the flag */
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_DCONN;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_CONNECT)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_CONNECT])(_usb_instances);
			}
		}

		/* host disconnect interrupt 	*/
		if (flags & USB_HOST_INTFLAG_DDISC) {
			/* Clear busy status */
			host_pipe_job_busy_status = 0;
			/* clear the flag */
			_usb_instances->hw->HOST.INTFLAG.reg = USB_HOST_INTFLAG_DDISC;
			if(_usb_instances->host_enabled_callback_mask & (1 << USB_HOST_CALLBACK_DISCONNECT)) {
				(_usb_instances->host_callback[USB_HOST_CALLBACK_DISCONNECT])(_usb_instances);
			}
		}

	}
}

void usb_host_pipe_set_auto_zlp(struct usb_module *module_inst, uint8_t pipe_num, bool value)
{
	Assert(module_inst);

	usb_descriptor_table.usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.AUTO_ZLP = value;
}

/**
 * \brief Registers a USB device callback
 *
 * Registers a callback function which is implemented by the user.
 *
 * \note The callback must be enabled by \ref usb_host_enable_callback,
 * in order for the interrupt handler to call it when the conditions for the
 * callback type is met.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     callback_type Callback type given by an enum
 * \param[in]     callback_func Pointer to callback function
 *
 * \return Status of the registration operation.
 * \retval STATUS_OK    The callback was registered successfully.
 */
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

/**
 * \brief Unregisters a USB device callback
 *
 * Unregisters an asynchronous callback implemented by the user. Removing it
 * from the internal callback registration table.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     callback_type Callback type given by an enum
 *
 * \return Status of the de-registration operation.
 * \retval STATUS_OK    The callback was unregistered successfully.
 */
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

/**
 * \brief Enables USB device callback generation for a given type.
 *
 * Enables asynchronous callbacks for a given logical type.
 * This must be called before usb host generate callback events.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     callback_type Callback type given by an enum
 *
 * \return Status of the callback enable operation.
 * \retval STATUS_OK    The callback was enabled successfully.
 */
enum status_code usb_device_enable_callback(struct usb_module *module_inst,
		enum usb_device_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	/* clear related flag */
	module_inst->hw->DEVICE.INTFLAG.reg = _usb_device_irq_bits[callback_type];

	/* Enable callback */
	module_inst->device_enabled_callback_mask |= _usb_device_irq_bits[callback_type];

	module_inst->hw->DEVICE.INTENSET.reg = _usb_device_irq_bits[callback_type];

	return STATUS_OK;
}

/**
 * \brief Disables USB device callback generation for a given type.
 *
 * Disables asynchronous callbacks for a given logical type.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     callback_type Callback type given by an enum
 *
 * \return Status of the callback disable operation.
 * \retval STATUS_OK    The callback was disabled successfully.
 */
enum status_code usb_device_disable_callback(struct usb_module *module_inst,
		enum usb_device_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	/* Disable callback */
	module_inst->device_enabled_callback_mask &= ~_usb_device_irq_bits[callback_type];

	module_inst->hw->DEVICE.INTENCLR.reg = _usb_device_irq_bits[callback_type];

	return STATUS_OK;
}

/**
 * \brief Registers a USB device endpoint callback
 *
 * Registers a callback function which is implemented by the user.
 *
 * \note The callback must be enabled by \ref usb_device_endpoint_enable_callback,
 * in order for the interrupt handler to call it when the conditions for the
 * callback type is met.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     ep_num        Endpoint to configure
 * \param[in]     callback_type Callback type given by an enum
 * \param[in]     callback_func Pointer to callback function
 *
 * \return Status of the registration operation.
 * \retval STATUS_OK    The callback was registered successfully.
 */
enum status_code usb_device_endpoint_register_callback(
		struct usb_module *module_inst, uint8_t ep_num,
		enum usb_device_endpoint_callback callback_type,
		usb_device_endpoint_callback_t callback_func)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(ep_num < USB_EPT_NUM);
	Assert(callback_func);

	/* Register callback function */
	module_inst->device_endpoint_callback[ep_num][callback_type] = callback_func;

	/* Set the bit corresponding to the callback_type */
	module_inst->deivce_endpoint_registered_callback_mask[ep_num] |= _usb_endpoint_irq_bits[callback_type];

	return STATUS_OK;
}

/**
 * \brief Unregisters a USB device endpoint callback
 *
 * Unregisters an asynchronous callback implemented by the user. Removing it
 * from the internal callback registration table.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     ep_num        Endpoint to configure
 * \param[in]     callback_type Callback type given by an enum
 *
 * \return Status of the de-registration operation.
 * \retval STATUS_OK    The callback was unregistered successfully.
 */
enum status_code usb_device_endpoint_unregister_callback(
		struct usb_module *module_inst, uint8_t ep_num,
		enum usb_device_endpoint_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(ep_num < USB_EPT_NUM);

	/* Unregister callback function */
	module_inst->device_endpoint_callback[ep_num][callback_type] = NULL;

	/* Clear the bit corresponding to the callback_type */
	module_inst->deivce_endpoint_registered_callback_mask[ep_num] &= ~_usb_endpoint_irq_bits[callback_type];

	return STATUS_OK;
}

/**
 * \brief Enables USB device endpoint callback generation for a given type.
 *
 * Enables asynchronous callbacks for a given logical type.
 * This must be called before usb host pipe generate callback events.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     ep            Endpoint to configure
 * \param[in]     callback_type Callback type given by an enum
 *
 * \return Status of the callback enable operation.
 * \retval STATUS_OK    The callback was enabled successfully.
 */
enum status_code usb_device_endpoint_enable_callback(
		struct usb_module *module_inst, uint8_t ep,
		enum usb_device_endpoint_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);
	Assert(ep_num < USB_EPT_NUM);

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

/**
 * \brief Disables USB device endpoint callback generation for a given type.
 *
 * Disables asynchronous callbacks for a given logical type.
 *
 * \param[in]     module_inst   Pointer to USB software instance struct
 * \param[in]     ep            Endpoint to configure
 * \param[in]     callback_type Callback type given by an enum
 *
 * \return Status of the callback disable operation.
 * \retval STATUS_OK    The callback was disabled successfully.
 */
enum status_code usb_device_endpoint_disable_callback(
		struct usb_module *module_inst, uint8_t ep,
		enum usb_device_endpoint_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);
	Assert(ep_num < USB_EPT_NUM);

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

/**
 * \brief Initializes an USB device endpoint configuration structure to defaults.
 *
 * Initializes a given USB device endpoint configuration structure to a
 * set of known default values. This function should be called on all new
 * instances of these configuration structures before being modified by the
 * user application.
 *
 * The default configuration is as follows:
 * \li endpoint address is 0
 * \li endpoint size is 8 bytes
 * \li auto_zlp is false
 * \li endpoint type is control
 *
 * \param[out] ep_config  Configuration structure to initialize to default values
 */
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

/**
 * \brief Writes an USB device endpoint configuration to the hardware module.
 *
 * Writes out a given configuration of an USB device endpoint
 * configuration to the hardware module. If the pipe is already configured,
 * the new configuration will replace the existing one.
 *
 * \param[in] module_inst    Pointer to USB software instance struct
 * \param[in] ep_config      Configuration settings for the endpoint
 *
 * \return Status of the device endpoint configuration operation.
 * \retval STATUS_OK    The device endpoint was configured successfully.
 */
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

/**
 * \brief current endpoint is configured
 * \param module_inst   Pointer to USB software instance struct
 * \param endpoint address (direction & number)
 * \return null
 */
bool usb_device_endpoint_is_configured(struct usb_module *module_inst, uint8_t ep)
{
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;
	uint8_t flag;

	if (ep & USB_EP_DIR_IN) {
		flag = (uint8_t)(module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.bit.EPTYPE1);
	} else {
		flag = (uint8_t)(module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPCFG.bit.EPTYPE0);
	}
	return ((enum usb_device_endpoint_type)(flag) != USB_DEVICE_ENDPOINT_TYPE_DISABLE);
}


/**
 * \brief current endpoint abort
 * \param module_inst   Pointer to USB software instance struct
 * \param endpoint address (direction & number)
 * \return null
 */
void usb_device_endpoint_abort_job(struct usb_module *module_inst, uint8_t ep)
{
	uint8_t ep_num;
	ep_num = ep & USB_EP_ADDR_MASK;

	// Stop transfer
	if (ep & USB_EP_DIR_IN) {
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK1RDY;
		// Eventually ack a transfer occur during abort
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1;
	} else {
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_BK0RDY;
		// Eventually ack a transfer occur during abort
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0;
	}
}

/**
 * \brief Is endpoint halted
 * \param module_inst Pointer to USB software instance struct
 * \param endpoint address (direction & number)
 * \param endpoint enable flag
 * \return true or false
 */
bool usb_device_endpoint_is_halted(struct usb_module *module_inst, uint8_t ep)
{
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;

	if (ep & USB_EP_DIR_IN) {
		return (module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUS.reg & USB_DEVICE_EPSTATUSSET_STALLRQ1);
	} else {
		return (module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUS.reg & USB_DEVICE_EPSTATUSSET_STALLRQ0);
	}
}

/**
 * \brief set endpoint halt
 * \param module_inst Pointer to USB software instance struct
 * \param endpoint address (direction & number)
 * \return null
 */
void usb_device_endpoint_set_halt(struct usb_module *module_inst, uint8_t ep)
{
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;

	// Stall endpoint
	if (ep & USB_EP_DIR_IN) {
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_STALLRQ1;
	} else {
		module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_STALLRQ0;
	}
}

/**
 * \brief clear endpoint from halted
 * \param module_inst Pointer to USB software instance struct
 * \param endpoint address (direction & number)
 * \return null
 */
void usb_device_endpoint_clear_halt(struct usb_module *module_inst, uint8_t ep)
{
	uint8_t ep_num = ep & USB_EP_ADDR_MASK;

	if (ep & USB_EP_DIR_IN) {
		if (module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUS.reg & USB_DEVICE_EPSTATUSSET_STALLRQ1) {
			// Remove stall request
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_STALLRQ1;
			if (module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_STALL1) {
				module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL1;
				// The Stall has occurred, then reset data toggle
				module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSSET_DTGLIN;
			}
		}
	} else {
		if (module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUS.reg & USB_DEVICE_EPSTATUSSET_STALLRQ0) {
			// Remove stall request
			module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_STALLRQ0;
			if (module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_STALL0) {
				module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL0;
				// The Stall has occurred, then reset data toggle
				module_inst->hw->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSSET_DTGLOUT;
			}
		}
	}
}

/**
 * \brief endpoint number n write job
 *
 * \param module_inst pointer to USB device module instance
 * \param ep_num number of endpoint
 * \param pbuf pointer to buffer
 * \param buf_size size of buffer
 * \return status code
 */
enum status_code usb_device_endpoint_write_buffer_job(struct usb_module *module_inst,uint8_t ep_num,
		uint8_t* pbuf, uint32_t buf_size)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);
	Assert(ep_num < USB_EPT_NUM);

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

/**
 * \brief endpoint number n read job
 *
 * \param module_inst pointer to USB device module instance
 * \param ep_num number of endpoint
 * \param pbuf pointer to buffer
 * \param buf_size size of buffer
 * \return status code
 */
enum status_code usb_device_endpoint_read_buffer_job(struct usb_module *module_inst,uint8_t ep_num,
		uint8_t* pbuf, uint32_t buf_size)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);
	Assert(ep_num < USB_EPT_NUM);

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

/**
 * \brief endpoint number n setup job
 *
 * \param module_inst pointer to USB device module instance
 * \param pbuf pointer to buffer
 * \return status code
 */
enum status_code usb_device_endpoint_setup_buffer_job(struct usb_module *module_inst,
		uint8_t* pbuf)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);
	Assert(ep_num < USB_EPT_NUM);

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
	uint16_t flags, flags_run;
	ep_inst = _usb_instances->hw->DEVICE.EPINTSMRY.reg;

	/* device interrupt */
	if (0 == ep_inst) {
		int i;

		/* get interrupt flags */
		flags = _usb_instances->hw->DEVICE.INTFLAG.reg;
		flags_run = flags &
				_usb_instances->device_enabled_callback_mask &
				_usb_instances->device_registered_callback_mask;

		for (i = 0; i < USB_DEVICE_CALLBACK_N; i ++) {
			if (flags & _usb_device_irq_bits[i]) {
				_usb_instances->hw->DEVICE.INTFLAG.reg =
						_usb_device_irq_bits[i];
			}
			if (flags_run & _usb_device_irq_bits[i]) {
				(_usb_instances->device_callback[i])(_usb_instances);
			}
		}

	} else {
		/* endpoint interrupt */

		for (uint8_t i = 0; i < USB_EPT_NUM; i++) {

			if (ep_inst & (1 << i)) {
				flags = _usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg;
				flags_run = flags &
						_usb_instances->device_endpoint_enabled_callback_mask[i] &
						_usb_instances->deivce_endpoint_registered_callback_mask[i];

				// endpoint transfer stall interrupt
				if (flags & USB_DEVICE_EPINTFLAG_STALL_Msk) {
					if (_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_STALL1) {
						_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL1;
						ep_callback_para.endpoint_address = USB_EP_DIR_IN | i;
					} else if (_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_STALL0) {
						_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL0;
						ep_callback_para.endpoint_address = USB_EP_DIR_OUT | i;
					}

					if (flags_run & USB_DEVICE_EPINTFLAG_STALL_Msk) {
						(_usb_instances->device_endpoint_callback[i][USB_DEVICE_ENDPOINT_CALLBACK_STALL])(_usb_instances,&ep_callback_para);
					}
					return;
				}

				// endpoint received setup interrupt
				if (flags & USB_DEVICE_EPINTFLAG_RXSTP) {
					_usb_instances->hw->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_RXSTP;
					if(_usb_instances->device_endpoint_enabled_callback_mask[i] & _usb_endpoint_irq_bits[USB_DEVICE_ENDPOINT_CALLBACK_RXSTP]) {
						ep_callback_para.received_bytes = (uint16_t)(usb_descriptor_table.usb_endpoint_table[i].DeviceDescBank[0].PCKSIZE.bit.BYTE_COUNT);
						(_usb_instances->device_endpoint_callback[i][USB_DEVICE_ENDPOINT_CALLBACK_RXSTP])(_usb_instances,&ep_callback_para);
					}
					return;
				}

				// endpoint transfer fail interrupt
				if (flags & USB_DEVICE_EPINTFLAG_TRFAIL_Msk) {
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

					if(flags_run & USB_DEVICE_EPINTFLAG_TRFAIL_Msk) {
						(_usb_instances->device_endpoint_callback[i][USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL])(_usb_instances,&ep_callback_para);
					}
					return;
				}

				// endpoint transfer complete interrupt
				if (flags & USB_DEVICE_EPINTFLAG_TRCPT_Msk) {
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
					if(flags_run & USB_DEVICE_EPINTFLAG_TRCPT_Msk) {
						(_usb_instances->device_endpoint_callback[i][USB_DEVICE_ENDPOINT_CALLBACK_TRCPT])(_usb_instances,&ep_callback_para);
					}
					return;
				}
			}
		}
	}
}

void usb_enable(struct usb_module *module_inst)
{
	Assert(module_inst);
	Assert(module_inst->hw);

	module_inst->hw->HOST.CTRLA.reg |= USB_CTRLA_ENABLE;
	while (module_inst->hw->HOST.SYNCBUSY.reg == USB_SYNCBUSY_ENABLE);
}
void usb_disable(struct usb_module *module_inst)
{
	Assert(module_inst);
	Assert(module_inst->hw);

	module_inst->hw->HOST.CTRLA.reg &= ~USB_CTRLA_ENABLE;
	while (module_inst->hw->HOST.SYNCBUSY.reg == USB_SYNCBUSY_ENABLE);
}

/** Interrupt handler for the USB module. */
void USB_Handler(void)
{
	if (_usb_instances->hw->HOST.CTRLA.bit.MODE) {
		/*host mode ISR */
		_usb_host_interrupt_handler();
	} else {
		/*device mode ISR */
		_usb_device_interrupt_handler();
	}
}

void usb_get_config_defaults(struct usb_config *module_config)
{
	Assert(module_config);

	/* Sanity check arguments */
	Assert(module_config);
	/* Write default config to config struct */
	module_config->select_host_mode = 0;
	module_config->run_in_standby = 1;
	module_config->source_generator = GCLK_GENERATOR_0;
	module_config->speed_mode = USB_SPEED_FULL;
}

#define NVM_USB_PAD_TRANSN_POS  45
#define NVM_USB_PAD_TRANSN_SIZE 5
#define NVM_USB_PAD_TRANSP_POS  50
#define NVM_USB_PAD_TRANSP_SIZE 5
#define NVM_USB_PAD_TRIM_POS  55
#define NVM_USB_PAD_TRIM_SIZE 3

enum status_code usb_init(struct usb_module *module_inst, Usb *const hw,
		struct usb_config *module_config)
{
	/* Sanity check arguments */
	Assert(hw);
	Assert(module_inst);
	Assert(module_config);

	uint32_t i,j;
	uint32_t pad_transn, pad_transp, pad_trim;
	struct system_pinmux_config pin_config;
	struct system_gclk_chan_config gclk_chan_config;

	host_pipe_job_busy_status = 0;

	_usb_instances = module_inst;

	/* Associate the software module instance with the hardware module */
	module_inst->hw = hw;

	/* Turn on the digital interface clock */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBB, PM_APBBMASK_USB);

	/* Load Pad Calibration */
	pad_transn =( *((uint32_t *)(NVMCTRL_OTP4)
			+ (NVM_USB_PAD_TRANSN_POS / 32))
		>> (NVM_USB_PAD_TRANSN_POS % 32))
		& ((1 << NVM_USB_PAD_TRANSN_SIZE) - 1);

	if (pad_transn == 0x1F) {
		pad_transn = 5;
	}

	hw->HOST.PADCAL.bit.TRANSN = pad_transn;

	pad_transp =( *((uint32_t *)(NVMCTRL_OTP4)
			+ (NVM_USB_PAD_TRANSP_POS / 32))
			>> (NVM_USB_PAD_TRANSP_POS % 32))
			& ((1 << NVM_USB_PAD_TRANSP_SIZE) - 1);

	if (pad_transp == 0x1F) {
		pad_transp = 29;
	}

	hw->HOST.PADCAL.bit.TRANSP = pad_transp;

	pad_trim =( *((uint32_t *)(NVMCTRL_OTP4)
			+ (NVM_USB_PAD_TRIM_POS / 32))
			>> (NVM_USB_PAD_TRIM_POS % 32))
			& ((1 << NVM_USB_PAD_TRIM_SIZE) - 1);

	if (pad_trim == 0x7) {
		pad_trim = 3;
	}

	hw->HOST.PADCAL.bit.TRIM = pad_trim;

	/* Set up the USB DP/DN pins */
	system_pinmux_get_config_defaults(&pin_config);
	pin_config.mux_position = MUX_PA24G_USB_DM;
	system_pinmux_pin_set_config(PIN_PA24G_USB_DM, &pin_config);
	pin_config.mux_position = MUX_PA25G_USB_DP;
	system_pinmux_pin_set_config(PIN_PA25G_USB_DP, &pin_config);

	/* Setup clock for module */
	system_gclk_chan_get_config_defaults(&gclk_chan_config);
	gclk_chan_config.source_generator = module_config->source_generator;
	system_gclk_chan_set_config(USB_GCLK_ID, &gclk_chan_config);
	system_gclk_chan_enable(USB_GCLK_ID);
	pin_config.mux_position = MUX_PB14H_GCLK_IO0;
	pin_config.direction    = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
	system_pinmux_pin_set_config(PIN_PB14H_GCLK_IO0, &pin_config);

	/* Reset */
	hw->HOST.CTRLA.bit.SWRST = 1;
	while (hw->HOST.SYNCBUSY.bit.SWRST);

	/* Set the configuration */
	hw->HOST.CTRLA.bit.MODE = module_config->select_host_mode;
	hw->HOST.CTRLA.bit.RUNSTDBY = module_config->run_in_standby;
	hw->HOST.DESCADD.reg = (uint32_t)(&usb_descriptor_table.usb_endpoint_table[0]);
	if (USB_SPEED_FULL == module_config->speed_mode) {
		module_inst->hw->DEVICE.CTRLB.bit.SPDCONF = USB_DEVICE_CTRLB_SPDCONF_0_Val;
	} else if(USB_SPEED_LOW == module_config->speed_mode) {
		module_inst->hw->DEVICE.CTRLB.bit.SPDCONF = USB_DEVICE_CTRLB_SPDCONF_1_Val;
	}

	memset((uint8_t *)(&usb_descriptor_table.usb_endpoint_table[0]), 0,
			sizeof(usb_descriptor_table.usb_endpoint_table));

	/* callback related init */
	for (i = 0; i < USB_HOST_CALLBACK_N; i++) {
		module_inst->host_callback[i] = NULL;
	};
	for (i = 0; i < USB_PIPE_NUM; i++) {
		for (j = 0; j < USB_HOST_PIPE_CALLBACK_N; j++) {
			module_inst->host_pipe_callback[i][j] = NULL;
		}
	};
	module_inst->host_registered_callback_mask = 0;
	module_inst->host_enabled_callback_mask = 0;
	for (i = 0; i < USB_PIPE_NUM; i++) {
		module_inst->host_pipe_registered_callback_mask[i] = 0;
		module_inst->host_pipe_enabled_callback_mask[i] = 0;
	}
	/*  device callback related */
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

	/* Enable interrupts for this USB module */
	system_interrupt_enable(SYSTEM_INTERRUPT_MODULE_USB);

	return STATUS_OK;
}


