/**
 * \file
 *
 * \brief USB host driver
 * Compliance with common driver UHD
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

#include "conf_usb_host.h"
#include "sysclk.h"
#include "matrix.h"
#include "ohci\ohci_hcd.h"
#include <string.h>
#include <stdlib.h>

#if !(SAMG55)
# error The current UHP Driver supports only SAMG55.
#endif

#ifndef UHD_USB_INT_LEVEL
# define UHD_USB_INT_LEVEL 5 // By default USB interrupt have low priority
#endif

// Optional UHC callbacks
#ifndef UHC_MODE_CHANGE
# define UHC_MODE_CHANGE(arg)
#endif
#ifndef UHC_SOF_EVENT
# define UHC_SOF_EVENT()
#endif
#ifndef UHC_VBUS_CHANGE
# define UHC_VBUS_CHANGE(b_present)
#endif
#ifndef UHC_VBUS_ERROR
# define UHC_VBUS_ERROR()
#endif

// for debug text
//#define dbg_print printf
#define dbg_print(...)

/**
 * \ingroup usb_host_group
 * \defgroup uhd_group USB Host Driver (UHD)
 *
 * \section uhdp_conf UHDP Custom configuration
 * The following UHP driver configuration must be defined in conf_usb_host.h
 * file of the application.
 *
 * USB_INT_LEVEL<br>
 * Option to change the interrupt priority (0 to 15) by default 5 (recommended).
 *
 * \section Callbacks management
 * The USB driver is fully managed by interrupt and does not request periodic
 * task. Thereby, the USB events use callbacks to transfer the information.
 * The callbacks can be declared in static during compilation
 * or dynamically during code execution.
 *
 * \section Power mode management
 * The driver uses the sleepmgr service to manage the different sleep modes.
 * The sleep mode depends on USB driver state (uhd_uhp_state_enum).
 * @{
 */

/**
 * \name Power management
 */
//@{
#ifndef UHP_NO_SLEEP_MGR

#include "sleepmgr.h"
//! States of UHP interface
enum uhd_uhp_state_enum {
	UHD_STATE_OFF = 0,
	UHD_STATE_WAIT_ID_HOST = 1,
	UHD_STATE_NO_VBUS = 2,
	UHD_STATE_DISCONNECT = 3,
	UHD_STATE_SUSPEND = 4,
	UHD_STATE_IDLE = 5,
};

/*! \brief Manages the sleep mode following the UHP state
 *
 * \param new_state  New UHP state
 */
static void uhd_sleep_mode(enum uhd_uhp_state_enum new_state)
{
	enum sleepmgr_mode sleep_mode[] = {
		SLEEPMGR_ACTIVE,    // UHD_STATE_OFF (not used)
		SLEEPMGR_ACTIVE, // UHD_STATE_WAIT_ID_HOST
		SLEEPMGR_ACTIVE, // UHD_STATE_NO_VBUS
		SLEEPMGR_ACTIVE, // UHD_STATE_DISCONNECT
		SLEEPMGR_ACTIVE, // UHD_STATE_SUSPEND
		SLEEPMGR_ACTIVE, // UHD_STATE_IDLE
	};

	static enum uhd_uhp_state_enum uhd_state = UHD_STATE_OFF;

	if (uhd_state == new_state) {
		return; // No change
	}
	if (new_state != UHD_STATE_OFF) {
		// Lock new limit
		sleepmgr_lock_mode(sleep_mode[new_state]);
	}
	if (uhd_state != UHD_STATE_OFF) {
		// Unlock old limit
		sleepmgr_unlock_mode(sleep_mode[uhd_state]);
	}
	uhd_state = new_state;
}

#else
#  define uhd_sleep_mode(arg)
#endif // UHP_NO_SLEEP_MGR
//@}

//! Store the callback to be call at the end of reset signal
static uhd_callback_reset_t uhd_reset_callback = NULL;

//! Store the callback to be call at the end of setup request
struct uhd_callback_setup_end_parameter {
	usb_add_t add;
	uhd_trans_status_t status;
	uint16_t payload_trans;
};
static struct uhd_callback_setup_end_parameter callback_setup_end_para;
static uhd_callback_setup_end_t callback_setup_end_func = NULL;

//! Store the callback to be call at the end of one transaction
struct uhd_callback_trans_end_parameter {
	usb_add_t add;
	usb_ep_t ep;
	uhd_trans_status_t status;
	iram_size_t nb_transfered;
};
static struct uhd_callback_trans_end_parameter callback_trans_end_para;
static uhd_callback_trans_t callback_trans_end_func = NULL;

/**
 * \internal
 * \brief Manages callback in transfer end
 */
static void uhd_transfer_end(void *pointer)
{

	if (callback_setup_end_func != NULL) {
		callback_setup_end_func(callback_setup_end_para.add,
					callback_setup_end_para.status,
					callback_setup_end_para.payload_trans);
		callback_setup_end_func = NULL;
	}

	if (callback_trans_end_func != NULL) {
		callback_trans_end_func(callback_trans_end_para.add,
					callback_trans_end_para.ep,
					callback_trans_end_para.status,
					callback_trans_end_para.nb_transfered);
		callback_trans_end_func = NULL;
	}
}

/**
 * \internal
 * \brief Manages timeouts and actions based on SOF events
 * - UHC user notification
 * - SOF user notification
 */
static void uhd_sof_interrupt(void *pointer)
{
	// Notify the UHC
	uhc_notify_sof(false);

	// Notify the user application
	UHC_SOF_EVENT();
}

static void uhd_remote_wakeup(void *pointer)
{
	// Notify the UHC
	uhc_notify_resume();
	uhd_sleep_mode(UHD_STATE_IDLE);
}

static void uhd_status_change(void *pointer)
{
	uint32_t *bus_flag;

	bus_flag = (uint32_t *)pointer;

	if (*bus_flag == BUS_CONNECT) {
		uhc_notify_connection(true);
	} else if (*bus_flag == BUS_DISCONNECT) {
		uhc_notify_connection(false);
	} else if (*bus_flag == BUS_RESET) {
		if (uhd_reset_callback != NULL) {
			uhd_reset_callback();
			uhd_reset_callback = NULL;
		}
	}
}

void uhd_enable(void)
{
	irqflags_t flags;

	uhd_sleep_mode(UHD_STATE_DISCONNECT);

#if SAMG55
	matrix_set_usb_host();
#endif

	//* Enable USB hardware clock
	pmc_enable_periph_clk(ID_UHP);
	sysclk_enable_usb();

	// Always authorize asynchronous USB interrupts to exit of sleep mode
	// For SAMG55 USB wake up device except BACKUP mode
	NVIC_SetPriority((IRQn_Type) ID_UHP, UHD_USB_INT_LEVEL);
	NVIC_EnableIRQ((IRQn_Type) ID_UHP);
//	pmc_set_fast_startup_input(PMC_FSMR_USBAL);

	// To avoid USB interrupt before end of initialization
	flags = cpu_irq_save();

	ohci_init();
	ohci_register_callback(OHCI_INTERRUPT_WDH, (void *)uhd_transfer_end);
	ohci_register_callback(OHCI_INTERRUPT_SF, (void *)uhd_sof_interrupt);
	ohci_register_callback(OHCI_INTERRUPT_RD, (void *)uhd_remote_wakeup);
	ohci_register_callback(OHCI_INTERRUPT_RHSC, (void *)uhd_status_change);

	cpu_irq_restore(flags);
}


void uhd_disable(bool b_id_stop)
{
	irqflags_t flags;

	flags = cpu_irq_save();
	ohci_deinit();
	cpu_irq_restore(flags);

	// Do not authorize asynchronous USB interrupts
//	pmc_clr_fast_startup_input(PMC_FSMR_USBAL);
	sysclk_disable_usb();
	pmc_disable_periph_clk(ID_UHP);
	uhd_sleep_mode(UHD_STATE_OFF);

}

uhd_speed_t uhd_get_speed(void)
{
	switch (ohci_get_device_speed()) {

	case 0:
		return UHD_SPEED_FULL;

	case 1:
		return UHD_SPEED_LOW;

	default:
		Assert(false);
		return UHD_SPEED_LOW;
	}
}

uint16_t uhd_get_frame_number(void)
{
	return ohci_get_frame_number();
}

uint16_t uhd_get_microframe_number(void)
{
	return 0; // Not supported
}

void uhd_send_reset(uhd_callback_reset_t callback)
{
	uhd_reset_callback = callback;
	ohci_bus_reset();
//	ohci_start_port_reset();
//reset complete status can be get from HcRhPortStatus Register bit 20 PRSC
//maybe polling to do the callback
}

void uhd_suspend(void)
{
	// Wait three SOFs before entering in suspend state
//	uhd_suspend_start = 3;

	ohci_bus_suspend();
}

bool uhd_is_suspend(void)
{
	if(ohci_is_suspend()) {
		return true;
	} else {
		return false;
	}
}

void uhd_resume(void)
{
//	otg_unfreeze_clock();
//	uhd_enable_sof();

	ohci_bus_resume();
	uhd_sleep_mode(UHD_STATE_IDLE);
}

bool uhd_ep0_alloc(usb_add_t add, uint8_t ep_size)
{
	ed_info_t ed_info_temp;

	if (ep_size < 8) {
		return false;
	}

	ed_info_temp.ed_info_s.bFunctionAddress = 1;               // device address=UHC_DEVICE_ENUM_ADD=1
	ed_info_temp.ed_info_s.bEndpointNumber = 0;                // endpoint 0
	ed_info_temp.ed_info_s.bDirection = 0;                     // in TD
	ed_info_temp.ed_info_s.bSpeed = ohci_get_device_speed();   // speed
	ed_info_temp.ed_info_s.bSkip = 0;                          // no skip
	ed_info_temp.ed_info_s.bFormat = 0;                        // General TD
	ed_info_temp.ed_info_s.bMaximumPacketSize = ep_size;       // max packet size

	ohci_add_ed_control(&ed_info_temp);

	return true;
}

bool uhd_ep_alloc(usb_add_t add, usb_ep_desc_t * ep_desc)
{
	ed_info_t ed_info_temp;
	bool return_value;
//	uint8_t ep_addr;
	uint8_t ep_type;
	uint8_t ep_dir;
	uint8_t ep_interval;

//	ep_addr = ep_desc->bEndpointAddress & USB_EP_ADDR_MASK;
	ep_type = ep_desc->bmAttributes&USB_EP_TYPE_MASK;
	if (ep_desc->bEndpointAddress & USB_EP_DIR_IN) {
		ep_dir = 2;
	} else {
		ep_dir = 1;
	}

	ed_info_temp.ed_info_s.bFunctionAddress = 1;			   // device address=UHC_DEVICE_ENUM_ADD=1
	ed_info_temp.ed_info_s.bEndpointNumber = ep_desc->bEndpointAddress; // endpoint number
	ed_info_temp.ed_info_s.bDirection = ep_dir;				   // Set direction
	ed_info_temp.ed_info_s.bSpeed = ohci_get_device_speed();   // speed
	ed_info_temp.ed_info_s.bSkip = 0;						   // no skip
	ed_info_temp.ed_info_s.bFormat = 0; 					   // General TD
	ed_info_temp.ed_info_s.bMaximumPacketSize = ep_desc->wMaxPacketSize;  // max packet size

		// Bank choice
		switch(ep_type) {
		case USB_EP_TYPE_ISOCHRONOUS:
			ep_interval = ep_desc->bInterval;
			ed_info_temp.ed_info_s.bFormat = 1;                // ISO TD
			return_value = ohci_add_ed_period(&ed_info_temp);
			break;
		case USB_EP_TYPE_INTERRUPT:
			ep_interval = ep_desc->bInterval;
			return_value = ohci_add_ed_period(&ed_info_temp);
			break;
		case USB_EP_TYPE_BULK:
			// 0 is required by UHP hardware for bulk
			ep_interval = 0;
			return_value = ohci_add_ed_bulk(&ed_info_temp);
			break;
		default:
			Assert(false);
			return false;
		}

		return return_value;

}


void uhd_ep_free(usb_add_t add, usb_ep_t endp)
{
	ohci_remove_ed(endp);
}

bool uhd_setup_request(
		usb_add_t add,
		usb_setup_req_t *req,
		uint8_t *payload,
		uint16_t payload_size,
		uhd_callback_setup_run_t callback_run,
		uhd_callback_setup_end_t callback_end)
{
	bool return_value = true;

	irqflags_t flags;
	flags = cpu_irq_save();

	// add setup TD
	return_value = ohci_add_td_control(TD_PID_SETUP, (uint8_t *)req, sizeof(usb_setup_req_t));
	if (return_value == false) {
		cpu_irq_restore(flags);
		return false;
	}

	if ((req->bmRequestType & USB_REQ_DIR_MASK) == USB_REQ_DIR_IN) {
		// add in TD
		return_value = ohci_add_td_control(TD_PID_IN, payload, payload_size);
		if (return_value == false) {
			cpu_irq_restore(flags);
			return false;
		}
	} else {
		// add out TD
		return_value = ohci_add_td_control(TD_PID_OUT, payload, payload_size);
		if (return_value == false) {
			cpu_irq_restore(flags);
			return false;
		}
	}

	callback_setup_end_para.add = add;
	callback_setup_end_para.status = UHD_TRANS_NOERROR;
	callback_setup_end_para.payload_trans = payload_size;
	callback_setup_end_func = callback_end;

	cpu_irq_restore(flags);

	return true;
}

bool uhd_ep_run(usb_add_t add,
		usb_ep_t endp,
		bool b_shortpacket,
		uint8_t *buf,
		iram_size_t buf_size,
		uint16_t timeout,
		uhd_callback_trans_t callback)
{
	bool return_value = true;

	irqflags_t flags;

	flags = cpu_irq_save();

	// No job running. Let's setup a new one.
	// Fill structure
//	ptr_job->buf = buf;
//	ptr_job->buf_size = buf_size;
//	ptr_job->nb_trans = 0;
//	ptr_job->timeout = timeout;
//	ptr_job->b_shortpacket = b_shortpacket;
//	ptr_job->call_end = callback;
//	if ((Is_uhd_pipe_int(pipe) || Is_uhd_pipe_iso(pipe))
//			&& (Is_uhd_pipe_out(pipe))) {
//		ptr_job->b_periodic_start = true;
//	}

	return_value = ohci_add_td_non_control(endp, buf, buf_size);
	if (return_value == false) {
		cpu_irq_restore(flags);
		return false;
	}

	callback_trans_end_para.add = add;
	callback_trans_end_para.ep = endp;
	callback_trans_end_para.status = UHD_TRANS_NOERROR;
	callback_trans_end_para.nb_transfered = buf_size;
	callback_trans_end_func = callback;

	cpu_irq_restore(flags);

	return true;
}

void uhd_ep_abort(usb_add_t add, usb_ep_t endp)
{
	ohci_remove_td(endp);
}



//@}
