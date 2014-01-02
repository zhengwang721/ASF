/**
 * \file
 *
 * \brief SAMD21 USB Driver
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
#ifndef USB_H_INCLUDED
#define USB_H_INCLUDED

#include <compiler.h>
#include <clock.h>
#include <gclk.h>
#include <pinmux.h>
#include <system_interrupt.h>

/** Enum for the speed status for the USB module */
enum usb_speed {
	USB_SPEED_LOW,
	USB_SPEED_FULL,
};

//! @name USB Host related contents
//! @{

/** Enum for the possible callback types for the USB in host module */
enum usb_host_callback {
	USB_HOST_CALLBACK_SOF,
	USB_HOST_CALLBACK_RESET,
	USB_HOST_CALLBACK_WAKEUP,
	USB_HOST_CALLBACK_DNRSM,
	USB_HOST_CALLBACK_UPRSM,
	USB_HOST_CALLBACK_RAMACER,
	USB_HOST_CALLBACK_CONNECT,
	USB_HOST_CALLBACK_DISCONNECT,
	USB_HOST_CALLBACK_N,
};

/** Enum for the possible callback types for the USB pipe in host module */
enum usb_host_pipe_callback {
	USB_HOST_PIPE_CALLBACK_TRANSFER_COMPLETE,
	USB_HOST_PIPE_CALLBACK_ERROR,
	USB_HOST_PIPE_CALLBACK_SETUP,
	USB_HOST_PIPE_CALLBACK_STALL,
	USB_HOST_PIPE_CALLBACK_N,
};

/**
 * \brief Host pipe types.
 */
enum usb_host_pipe_type {
	USB_HOST_PIPE_TYPE_DISABLE,
	USB_HOST_PIPE_TYPE_CONTROL,
	USB_HOST_PIPE_TYPE_ISO,
	USB_HOST_PIPE_TYPE_BULK,
	USB_HOST_PIPE_TYPE_INTERRUPT,
	USB_HOST_PIPE_TYPE_EXTENDED,
};

/**
 * \brief Host pipe token types.
 */
enum usb_host_pipe_token {
	USB_HOST_PIPE_TOKEN_SETUP,
	USB_HOST_PIPE_TOKEN_IN,
	USB_HOST_PIPE_TOKEN_OUT,
};

/**
 * \brief module structure.
 */
struct usb_module;

/**
 * \brief Host callback functions types.
 */
typedef void (*usb_host_callback_t)(struct usb_module *module_inst);
typedef void (*usb_host_pipe_callback_t)(struct usb_module *module_inst, void *);


/** USB configurations */
struct usb_config {
	/** 1 for host, 0 for device. */
	bool select_host_mode;
	/** When \c true the module is enabled during standby. */
	bool run_in_standby;
	/** Generic Clock Generator source channel. */
	enum gclk_generator source_generator;
};

/**
 * \brief USB software device instance structure.
 *
 * USB software instance structure, used to retain software state information
 * of an associated hardware module instance.
 *
 */
struct usb_module {
	/** Hardware module pointer of the associated USB peripheral. */
	Usb *hw;

	/** Array to store host related callback functions */
	usb_host_callback_t host_callback[USB_HOST_CALLBACK_N];
	usb_host_pipe_callback_t host_pipe_callback[USB_PIPE_NUM][USB_HOST_PIPE_CALLBACK_N];
	/** Bit mask for host callbacks registered */
	uint8_t host_registered_callback_mask;
	/** Bit mask for host callbacks enabled */
	uint8_t host_enabled_callback_mask;
	/** Bit mask for host pipe callbacks registered */
	uint8_t host_pipe_registered_callback_mask[USB_PIPE_NUM];
	/** Bit mask for host pipe callbacks enabled */
	uint8_t host_pipe_enabled_callback_mask[USB_PIPE_NUM];
};

/** USB host piple configurations */
struct usb_host_pipe_config {
	/** device address */
	uint8_t device_address;
	/** endpoint address  */
	uint8_t endpoint_address;
	/** Pipe type */
	enum usb_host_pipe_type pipe_type;
	/** interval */
	uint8_t binterval;
	/** pipe size */
	uint16_t size;
};

/** USB host piple callback parameter structure */
struct usb_pipe_callback_parameter {
	/** current pipe number */
	uint8_t pipe_num;
	/** pipe error status */
	uint8_t pipe_error_status;
	/** actual transfered data size */
	uint16_t transfered_size;
	/** required data size */
	uint16_t required_size;
};

/** USB simple operation functions */
void usb_enable(struct usb_module *module_inst);
void usb_disable(struct usb_module *module_inst);
static inline uint8_t usb_get_state_machine_status(struct usb_module *module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	return module_inst->hw->HOST.FSMSTATUS.reg;
}
/** USB init functions */
void usb_get_config_defaults(struct usb_config *module_config);
enum status_code usb_init(struct usb_module *module_inst, Usb *const hw,
		struct usb_config *module_config);

/** Host simple operation functions*/
static inline void usb_host_enable(struct usb_module *module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	module_inst->hw->HOST.CTRLB.bit.VBUSOK = 1;
}
static inline void usb_host_send_reset(struct usb_module *module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	module_inst->hw->HOST.CTRLB.bit.BUSRESET = 1;
}
static inline void usb_host_enable_sof(struct usb_module *module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	module_inst->hw->HOST.CTRLB.bit.SOFE = 1;
}
static inline void usb_host_disable_sof(struct usb_module *module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	module_inst->hw->HOST.CTRLB.bit.SOFE = 0;
}
static inline bool usb_host_is_sof_enabled(struct usb_module *module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	return module_inst->hw->HOST.CTRLB.bit.SOFE;
}
static inline void usb_host_send_resume(struct usb_module *module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	module_inst->hw->HOST.CTRLB.bit.RESUME= 1;
}
static inline void usb_host_send_l1_resume(struct usb_module *module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	module_inst->hw->HOST.CTRLB.bit.L1RESUME = 1;
}
static inline enum usb_speed usb_host_get_speed(struct usb_module *module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	if (module_inst->hw->HOST.STATUS.bit.SPEED == 0) {
		return USB_SPEED_FULL;
	} else {
		return USB_SPEED_LOW;
	}
}
static inline uint16_t usb_host_get_frame_number(struct usb_module *module_inst)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	return (uint16_t)(module_inst->hw->HOST.FNUM.bit.FNUM);
}
/** Host interrupt functions*/
enum status_code usb_host_register_callback(struct usb_module *module_inst,
		enum usb_host_callback callback_type,
		usb_host_callback_t callback_func);
enum status_code usb_host_unregister_callback(struct usb_module *module_inst,
		enum usb_host_callback callback_type);
enum status_code usb_host_enable_callback(struct usb_module *module_inst,
		enum usb_host_callback callback_type);
enum status_code usb_host_disable_callback(struct usb_module *module_inst,
		enum usb_host_callback callback_type);

/** Pipe init functions */
void usb_host_pipe_get_config_defaults(struct usb_host_pipe_config *ep_config);
enum status_code usb_host_pipe_set_config(struct usb_module *module_inst, uint8_t pipe_num,
		struct usb_host_pipe_config *ep_config);
enum status_code usb_host_pipe_get_config(struct usb_module *module_inst, uint8_t pipe_num,
		struct usb_host_pipe_config *ep_config);

/** Pipe interrupt functions */
enum status_code usb_host_pipe_register_callback(
		struct usb_module *module_inst, uint8_t pipe_num,
		enum usb_host_pipe_callback callback_type,
		usb_host_pipe_callback_t callback_func);
enum status_code usb_host_pipe_unregister_callback(
		struct usb_module *module_inst, uint8_t pipe_num,
		enum usb_host_pipe_callback callback_type);
enum status_code usb_host_pipe_enable_callback(
		struct usb_module *module_inst, uint8_t pipe_num,
		enum usb_host_pipe_callback callback_type);
enum status_code usb_host_pipe_disable_callback(
		struct usb_module *module_inst, uint8_t pipe_num,
		enum usb_host_pipe_callback callback_type);

/** Pipe high level job functions */
enum status_code usb_host_pipe_setup_job(struct usb_module *module_inst,
		uint8_t pipe_num, uint8_t *buf);
enum status_code usb_host_pipe_read_job(struct usb_module *module_inst,
		uint8_t pipe_num, uint8_t *buf, uint32_t buf_size);
enum status_code usb_host_pipe_write_job(struct usb_module *module_inst,
		uint8_t pipe_num, uint8_t *buf, uint32_t buf_size);
enum status_code usb_host_pipe_abort_job(struct usb_module *module_inst, uint8_t pipe_num);
/** Pipe simple operation functions*/
static inline void usb_host_pipe_freeze(struct usb_module *module_inst, uint8_t pipe_num)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSSET.reg = USB_HOST_PSTATUSSET_PFREEZE;
}
static inline void usb_host_pipe_unfreeze(struct usb_module *module_inst, uint8_t pipe_num)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_PFREEZE;
}
static inline bool usb_host_pipe_is_frozen(struct usb_module *module_inst, uint8_t pipe_num)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	return (module_inst->hw->HOST.HostPipe[pipe_num].PSTATUS.bit.PFREEZE == 1);
}
static inline void usb_host_pipe_set_toggle(struct usb_module *module_inst, uint8_t pipe_num)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSSET.reg = USB_HOST_PSTATUSSET_DTGL;
}
static inline void usb_host_pipe_clear_toggle(struct usb_module *module_inst, uint8_t pipe_num)
{
	/* Sanity check arguments */
	Assert(module_inst);
	Assert(module_inst->hw);

	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_DTGL;
}
void usb_host_pipe_set_auto_zlp(struct usb_module *module_inst, uint8_t pipe_num, bool value);

#endif /* USB_H_INCLUDED */
