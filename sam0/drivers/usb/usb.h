/**
 * \file
 *
 * \brief SAM D21 USB Driver
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
#ifndef USB_H_INCLUDED
#define USB_H_INCLUDED

#include <compiler.h>
#include <clock.h>
#include <gclk.h>
#include <pinmux.h>
#include <system_interrupt.h>

/** Enum for the speed status for the USB host module */
enum usb_speed {
	USB_SPEED_LOW,
	USB_SPEED_FULL,
//	USB_SPEED_HIGH,
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
//	USB_HOST_PIPE_CALLBACK_TRCPT0,
//	USB_HOST_PIPE_CALLBACK_TRCPT1,
	USB_HOST_PIPE_CALLBACK_FAIL,
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

#ifdef Roeder
/******************************/
//Roeder add here
//! @name USBC Device related contents
//! @{

/** Enumeration for the possible callback types for the USB in device module */
enum usb_device_callback {
	USB_DEVICE_CALLBACK_SUSPEND = 0,
	/** Reserved */
	USB_DEVICE_CALLBACK_SOF = 2,
	USB_DEVICE_CALLBACK_EORST = 3,
	USB_DEVICE_CALLBACK_WAKEUP = 4,
	USB_DEVICE_CALLBACK_EORSM = 5,
	USB_DEVICE_CALLBACK_UPRSM = 6,
	USB_DEVICE_CALLBACK_RAMACER = 7,
	USB_DEIVCE_CALLBACK_LPMNYET = 8,
	USB_DEVICE_CALLBACK_LPMUSP = 9,
};

/** Enumeration for the possible callback types for the USB endpoint in device module */
enum usb_device_endpoint_callback {
	USB_DEVICE_ENDPOINT_CALLBACK_TRCPT0,
	USB_DEVICE_ENDPOINT_CALLBACK_TRCPT1,
	USB_DEVICE_ENDPOINT_CALLBACK_TRCPT2,
	USB_DEVICE_ENDPOINT_CALLBACK_TRFAIL1,
	USB_DEVICE_ENDPOINT_CALLBACK_RXSTP,
	USB_DEVICE_ENDPOINT_CALLBACK_STALL0,
	USB_DEVICE_ENDPOINT_CALLBACK_STALL1,
};

/**
 * \brief Device Endpoint types.
 */
enum usb_device_endpoint_type {
	USB_DEVICE_ENDPOINT_TYPE_DISABLE,
	USB_DEVICE_ENDPOINT_TYPE_CONTROL_IN,
	USB_DEVICE_ENDPOINT_TYPE_CONTROL_OUT, 
	USB_DEVICE_ENDPOINT_TYPE_ISOCHRONOUS_IN,
	USB_DEVICE_ENDPOINT_TYPE_ISOCHRONOUS_OUT,
	USB_DEVICE_ENDPOINT_TYPE_BULK_IN,
	USB_DEVICE_ENDPOINT_TYPE_BULK_OUT,
	USB_DEVICE_ENDPOINT_TYPE_INTERRUPT_IN,
	USB_DEVICE_ENDPOINT_TYPE_INTERRUPT_OUT,
	USB_DEVICE_ENDPOINT_TYPE_DUALBANK_IN,
	USB_DEVICE_ENDPOINT_TYPE_DUALBANK_OUT,
};

/**
 * \brief Link Power Management Handshake.
 */
enum usb_lpmhd_sel {
	USB_LPM_NOT_SUPPORT,
	USB_LPM_ACK,
	USB_LPM_NYET,
};

/**
 * \brief Endpoint Status.
 */
enum usb_device_endpoint_status {
	USB_EP_DTGLOUT_STATUS = 0,
	USB_EP_DTGLIN_STATUS = 1,
	USB_EP_CURBK_STATUS = 2,
	/** Reserved */
	USB_EP_STALLRQ0_STATUS = 4,
	USB_EP_STALLRQ1_STATUS = 5,
	USB_EP_BK0RDY_STATUS = 6,
	USB_EP_BK1RDY_STATUS = 7,
};
#endif

struct usb_module;

typedef void (*usb_host_callback_t)(struct usb_module *module_inst, void *);
typedef void (*usb_host_pipe_callback_t)(struct usb_module *module_inst, void *);


/** USB configurations */
struct usb_config {
	/** 1 for host, 0 for device. */
	bool mode;
	/** When \c true the module is enabled during standby. */
	bool run_in_standby;
};

/**
 * \brief USB software device instance structure.
 *
 * USB software instance structure, used to retain software state information
 * of an associated hardware module instance.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct usb_module {
	/** Hardware module pointer of the associated USB peripheral. */
	Usb *hw;

#ifdef Roeder
	/** Array to store device related callback functions */
	usb_device_callback device_callback[USB_DEVICE_CALLBACK_N];
	usb_device_endpoint_callback device_endpoint_callback[USB_EP_NUM][USB_DEVICE_EP_CALLBACK_N];
	/** Bit mask for device callbacks registered */
	uint8_t device_registered_callback_mask;
	/** Bit mask for device callbacks enabled */
	uint8_t device_enabled_callback_mask;
	/** Bit mask for device endpoint callbacks registered */
	uint8_t deivce_endpoint_registered_callback_mask[USB_EP_NUM];
	/** Bit mask for device endpoint callbacks enabled */
	uint8_t device_endpoint_enabled_callback_mask[USB_EP_NUM];
#endif
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

#ifdef Roeder
/** USB device configurations */
struct usb_device_config {
	/** device speed mode */
	enum usb_speed device_speed;
	/** link power management handshake selection */
	enum usb_lpmhd_sel lpmhd_sel;
	/** operation mode of the NAK, 0: normal mode, 1 NAK handshake is answered for each transaction */
	bool globe_nak;
	/** no replay feature excepted SETUP token, 0: disable, 1: enable */
	bool no_reply;
	/** upstream resume, 0: no effect, 1: generate an upstream resume to the host for a remote wakeup */
	bool upstream_resume;
	/** detach, 0: the device is attached to the USB bus, 1: removing the device from the USB bus */
	bool detach;
}

/** USB device endpoint configurations */
struct usb_device_endpoint_config {
	/** device address */
	uint8_t device_address;
	/** endpoint number */
	uint8_t ep_number;
	/** endpoint size */
	uint8_t ep_size;
	/** multiple packets size */
	uint8_t multi_packet_size;
	/** automatic zero length packet mode, 1 for enable, 0 for disable */
	bool auto_zlp;
	/** 1 for double bank, 0 for single */
	bool bank;
	/** type of endpoint with Bank0 */
	enum usb_device_endpoint_type bk0_ep_type;
	/** type of endpoint with Bank1 */
	enum usb_device_endpoint_type bk1_ep_type;
	/** pointer to Bank0 RAM */
	uint8_t *pBk0;
	/** pointer to Bank1 RAM */
	uint8_t *pBk1;
};
#endif

/** USB host piple configurations */
struct usb_host_pipe_config {
	/** device address */
	uint8_t device_address;
	/** endpoint address  */
	uint8_t endpoint_address;
	/** 1 for double bank, 0 for single */
//	bool bank;
	/** Pipe type */
	enum usb_host_pipe_type pipe_type;
	/** interval add more comments , change name */
	uint8_t binterval;
	/** pipe size */
	uint16_t size;
	/** pipe token */
	enum usb_host_pipe_token pipe_token;
};

struct usb_pipe_callback_parameter {
	uint8_t pipe_num;
	uint8_t error_type;
	uint16_t transfer_size;
};

/** USB simple operation functions */
void usb_enable(struct usb_module *module_inst);
void usb_disable(struct usb_module *module_inst);
/** USB init functions */
void usb_get_config_defaults(struct usb_config *module_config);
enum status_code usb_init(struct usb_module *module_inst, Usb *const hw,
		struct usb_config *module_config);

///////////////////////////Roeder part/////////////////////////////////////
#ifdef Roeder
/** device initial functions */
void usb_device_get_config_defaults(struct usb_device_config *dev_config);
enum status_code usb_device_set_config(struct usb_module *module_inst,
		struct usb_device_config *dev_config);
enum status_code usb_device_get_config(struct usb_module *module_inst,
		struct usb_device_config *dev_config);

/** device interrupt functions*/
typedef void (*usb_device_callback_t)(struct usb_module *module_inst);
typedef void (*usb_device_endpoint_callback_t)(struct usb_module *module_inst);

enum status_code usb_device_register_callback(struct usb_module *module_inst,
		enum usb_device_callback callback_type,
		usb_device_callback_t callback_func);
enum status_code usb_device_unregister_callback(struct usb_module *module_inst,
		enum usb_device_callback callback_type);
enum status_code usb_device_enable_callback(struct usb_module *module_inst,
		enum usb_device_callback callback_type);
enum status_code usb_device_disable_callback(struct usb_module *module_inst,
		enum usb_device_callback callback_type);

/** endpoint initial functions */
void usb_device_endpoint_get_config_defaults(struct usb_device_endpoint_config *ep_config);
enum status_code usb_device_endpoint_set_config(struct usb_module *module_inst, uint8_t ep_num,
		struct usb_device_endpoint_config *ep_config);
enum status_code usb_device_endpoint_get_config(struct usb_module *module_inst, uint8_t ep_num,
		struct usb_device_endpoint_config *ep_config);

/** endpoint interrupt functions */
enum status_code usb_device_endpoint_register_callback(
		struct usb_module *module_inst, uint8_t ep_num,
		enum usb_device_endpoint_callback callback_type,
		usb_device_endpoint_callback_t callback_func);
enum status_code usb_device_endpoint_unregister_callback(
		struct usb_module *module_inst, uint8_t ep_num,
		enum usb_device_endpoint_callback callback_type);
enum status_code usb_device_endpoint_enable_callback(
		struct usb_module *module_inst, uint8_t ep_num,
		enum usb_device_endpoint_callback callback_type);
enum status_code usb_device_endpoint_disable_callback(
		struct usb_module *module_inst, uint8_t ep_num,
		enum usb_device_endpoint_callback callback_type);

/**
 * \brief USB device attach to the bus
 *
 * \param module_inst pointer to USB device module instance
 */
void usb_device_attach(struct usb_module *module_inst);

/**
 * \brief USB device detach from the bus
 *
 * \param module_inst pointer to USB device module instance
 */
void usb_device_detach(struct usb_module *module_inst);

/**
 * \brief Get the speed mode of USB device
 *
 * \param module_inst pointer to USB device module instance
 * \return enum usb_speed mode
 */
enum usb_speed usb_device_get_speed(struct usb_module *module_inst);

/**
 * \brief Get the address of USB device
 *
 * \param module_inst pointer to USB device module instance
 * \return usb device address value
 */
uint8_t usb_device_get_address(struct usb_module *module_inst);
/**
 * \brief Set the speed mode of USB device
 *
 * \param module_inst pointer to USB device module instance
 * \param usb device address value
 */
void usb_device_set_address(struct usb_module *module_inst, uint8_t address);

/**
 * \brief Get the frame number of USB device
 *
 * \param module_inst pointer to USB device module instance
 * \return usb device frame number value 
 */
uint16_t usb_device_get_frame_number(struct usb_module *module_inst);
/**
 * \brief Get the micro-frame number of USB device
 *
 * \param module_inst pointer to USB device module instance
 * \return usb device micro-frame number value 
 */
uint16_t usb_device_get_micro_frame_number(struct usb_module *module_inst);
/**
 * \brief Detect whether frame number CRC error occured
 *
 * \param module_inst pointer to USB device module instance
 * \return true or false 
 */
bool usb_device_frame_number_crc_error_detectd(struct usb_module *module_inst);
/**
 * \brief Detect whether error flow occured
 *
 * \param module_inst pointer to USB device module instance
 * \return true or false 
 */
bool usb_device_error_flow_detectd(struct usb_module *module_inst);
/**
 * \brief Detect whether crc error occured
 *
 * \param module_inst pointer to USB device module instance
 * \return true or false 
 */
bool usb_device_crc_error_detectd(struct usb_module *module_inst);
/**
 * \brief Update endpoint status (set or clear)
 *
 * \param module_inst, Pointer to USB device module instance
 * \param ep_num, Number of endpoint
 * \param usb_device_endpoint_status, Related field in endpoint status register
 * \param attribute, true: set this field, false: clear this field
 */
void usb_device_endpoint_status_update(struct usb_module *module_inst, 
		uint8_t ep_num, enum usb_device_endpoint_status ep_status, bool attribute);
/**
 * \brief whether endpoint number n is halted
 *
 * \param module_inst pointer to USB device module instance
 * \param ep_num number of endpoint
 *
 * \return true or false
 */
bool usb_device_endpoint_is_halted(struct usb_module *module_inst,uint8_t ep_num);
/**
 * \brief Set endpoint number n halted
 *
 * \param module_inst pointer to USB device module instance
 * \param ep_num number of endpoint
 *
 */
void usb_device_endpoint_set_halt(struct usb_module *module_inst,uint8_t ep_num);
/**
 * \brief Free endpoint number n from halted
 *
 * \param module_inst pointer to USB device module instance
 * \param ep_num number of endpoint
 *
 */
void usb_device_endpoint_clear_halt(struct usb_module *module_inst,uint8_t ep_num);
/**
 * \brief endpoint number n write job
 *
 * \param module_inst pointer to USB device module instance
 * \param ep_num number of endpoint
 * \param pbuf pointer to buffer
 * \param buf_size size of buffer
 */
void usb_device_endpoint_write_job(struct usb_module *module_inst,uint8_t ep_num,
		uint8_t* pbuf, uint32_t buf_size);
/**
 * \brief endpoint number n read job
 *
 * \param module_inst pointer to USB device module instance
 * \param ep_num number of endpoint
 * \param pbuf pointer to buffer
 * \param buf_size size of buffer
 */
void usb_device_endpoint_read_job(struct usb_module *module_inst,uint8_t ep_num,
		uint8_t* pbuf, uint32_t buf_size);
/**
 * \brief endpoint number n abort job
 *
 * \param module_inst pointer to USB device module instance
 * \param ep_num number of endpoint
 */
void usb_device_endpoint_abort_job(struct usb_module *module_inst,uint8_t ep_num);
#endif
/////////////////////////////////////////////////////////////////////


/** host simple operation functions*/
static inline void usb_host_enable(struct usb_module *module_inst)
{
	module_inst->hw->HOST.CTRLB.bit.VBUSOK = 1;
}
static inline void usb_host_send_reset(struct usb_module *module_inst)
{
	module_inst->hw->HOST.CTRLB.bit.BUSRESET = 1;
}
static inline void usb_host_enable_sof(struct usb_module *module_inst)
{
	module_inst->hw->HOST.CTRLB.bit.SOFE = 1;
}
static inline void usb_host_disable_sof(struct usb_module *module_inst)
{
	module_inst->hw->HOST.CTRLB.bit.SOFE = 0;
}
static inline bool usb_host_is_sof_enabled(struct usb_module *module_inst)
{
	return module_inst->hw->HOST.CTRLB.bit.SOFE;
}
static inline void usb_host_send_resume(struct usb_module *module_inst)
{
	module_inst->hw->HOST.CTRLB.bit.RESUME= 1;
}
static inline void usb_host_send_l1_resume(struct usb_module *module_inst)
{
	module_inst->hw->HOST.CTRLB.bit.L1RESUME = 1;
}
static inline enum usb_speed usb_host_get_speed(struct usb_module *module_inst)
{
	if (module_inst->hw->HOST.STATUS.bit.SPEED == 0) {
		return USB_SPEED_FULL;
	} else {
		return USB_SPEED_LOW;
	}
}
static inline uint16_t usb_host_get_frame_number(struct usb_module *module_inst)
{
	return (uint16_t)((module_inst->hw->HOST.FLENHIGH.reg << 11) + (module_inst->hw->HOST.FNUM.bit.FNUM));
}
/** host interrupt functions*/
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
		uint8_t pipe_num, uint8_t *buf, uint32_t buf_size);
enum status_code usb_host_pipe_read_job(struct usb_module *module_inst,
		uint8_t pipe_num, uint8_t *buf, uint32_t buf_size);
enum status_code usb_host_pipe_write_job(struct usb_module *module_inst,
		uint8_t pipe_num, uint8_t *buf, uint32_t buf_size);
enum status_code usb_host_pipe_abort_job(struct usb_module *module_inst, uint8_t pipe_num);
enum status_code usb_host_pipe_get_job_status(struct usb_module *module_inst, uint8_t pipe_num);
/** Pipe simple operation functions*/
static inline void usb_host_pipe_freeze(struct usb_module *module_inst, uint8_t pipe_num)
{
	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSSET.reg = USB_HOST_PSTATUSSET_PFREEZE;
}
static inline void usb_host_pipe_unfreeze(struct usb_module *module_inst, uint8_t pipe_num)
{
	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_PFREEZE;
}
static inline bool usb_host_pipe_is_freezed(struct usb_module *module_inst, uint8_t pipe_num)
{
	return (module_inst->hw->HOST.HostPipe[pipe_num].PSTATUS.bit.PFREEZE == 1);
}
static inline void usb_host_pipe_set_toggle(struct usb_module *module_inst, uint8_t pipe_num)
{
	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSSET.reg = USB_HOST_PSTATUSSET_DTGL;
}
static inline void usb_host_pipe_clear_toggle(struct usb_module *module_inst, uint8_t pipe_num)
{
	module_inst->hw->HOST.HostPipe[pipe_num].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_DTGL;
}
void usb_host_pipe_set_zlp(struct usb_module *module_inst, uint8_t pipe_num, bool value);

#endif /* USB_H_INCLUDED */
