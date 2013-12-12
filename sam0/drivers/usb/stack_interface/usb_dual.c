/**
 * \file
 *
 * \brief SAM Dxx USB dual driver file.
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
#include <compiler.h>
#include "usb_dual.h"

//! State of USBC dual initialization
static bool usb_initialized = false;

/**
 * \name Power management
 */
//@{
#ifndef UHD_NO_SLEEP_MGR

#include "sleepmgr.h"
//! States of USBC interface
enum uhd_usb_state_enum {
	UHD_STATE_OFF = 0,
	UHD_STATE_WAIT_ID_HOST = 1,
	UHD_STATE_NO_VBUS = 2,
	UHD_STATE_DISCONNECT = 3,
	UHD_STATE_SUSPEND = 4,
	UHD_STATE_SUSPEND_LPM = 5,
	UHD_STATE_IDLE = 6,
};

/*! \brief Manages the sleep mode following the USB state
 *
 * \param new_state  New USB state
 */
static void uhd_sleep_mode(enum uhd_usb_state_enum new_state)
{
	enum sleepmgr_mode sleep_mode[] = {
		SLEEPMGR_BACKUP,  // UHD_STATE_OFF (not used)
#if USB_ID_IO || USB_VBUS_IO || UHD_VBERR_IO
		SLEEPMGR_SLEEP_1, // UHD_STATE_WAIT_ID_HOST
		SLEEPMGR_SLEEP_1, // UHD_STATE_NO_VBUS
		SLEEPMGR_SLEEP_1, // UHD_STATE_DISCONNECT
#else
		SLEEPMGR_RET,     // UHD_STATE_WAIT_ID_HOST
		SLEEPMGR_RET,     // UHD_STATE_NO_VBUS
		SLEEPMGR_RET,     // UHD_STATE_DISCONNECT
#endif
		/* In suspend state, the SLEEPMGR_RET level is authorized
		 * even if ID Pin, Vbus... pins are managed through IO.
		 * When a ID disconnection or Vbus low event occurs,
		 * the asynchrone USB wakeup occurs.
		 */
		SLEEPMGR_RET,     // UHD_STATE_SUSPEND
		SLEEPMGR_RET,     // UHD_STATE_SUSPEND_LPM
		SLEEPMGR_SLEEP_0, // UHD_STATE_IDLE
	};
	static enum uhd_usb_state_enum uhd_state = UHD_STATE_OFF;

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
#endif // UHD_NO_SLEEP_MGR
//@}

/**
 * \name USB IO PADs handlers
 */
//@{

#if USB_ID_EIC
/**
 * USB ID pin change handler
 */
static void usb_id_handler(void)
{
	pad_ack_id_interrupt();
	if (Is_pad_id_device()) {
		uhc_stop(false);
		UHC_MODE_CHANGE(false);
//		usb_enable_device_mode();  //it will be done in udd_enanable
		udc_start();
	} else {
		udc_stop();
		UHC_MODE_CHANGE(true);
//		usb_enable_host_mode();  //it will be done in uhd_enanable
		uhc_start();
	}
}
#endif

#if USB_VBUS_EIC
/**
 * USB VBus pin change handler
 */
static void uhd_vbus_handler(void)
{
	pad_ack_vbus_interrupt();
	if (Is_pad_vbus_high()) {
		uhd_vbus_is_on();
		/* Freeze USB clock to use wakeup interrupt
		 * to detect connection.
		 * After detection of wakeup interrupt,
		 * the clock is unfreeze to have the true
		 * connection interrupt.
		 */
		uhd_enable_wakeup_interrupt();
		uhd_sleep_mode(UHD_STATE_DISCONNECT);
		UHC_VBUS_CHANGE(true);
	} else {
		uhd_vbus_is_off();
		uhd_sleep_mode(UHD_STATE_NO_VBUS);
		UHC_VBUS_CHANGE(false);
	}
}
#endif

//@}

bool usb_dual_enable(void)
{
	if (usb_initialized) {
		return false; // Dual role already initialized
	}
	usb_initialized = true;

	//* Enable USB hardware clock
	//sysclk_enable_usb();

	//* Link USB interrupt on dual interrupt in dual role
	NVIC_ClearPendingIRQ(USB_IRQn);
	NVIC_SetPriority(USB_IRQn, UHD_USB_INT_LEVEL);
	NVIC_EnableIRQ(USB_IRQn);

#if USB_ID_EIC || USB_VBUS_EIC
	eic_enable(EIC);
#endif

#if USB_ID_EIC
	pad_id_init();
	if (Is_pad_id_device()) {
		usb_enable_device_mode();
		uhd_sleep_mode(UHD_STATE_WAIT_ID_HOST);
		UHC_MODE_CHANGE(false);
		udc_start();
	} else {
		usb_enable_host_mode();
		UHC_MODE_CHANGE(true);
		uhc_start();
	}

	// End of host or device startup,
	// the current mode selected is already started now
	return true; // ID pin management has been enabled
#else
	uhd_sleep_mode(UHD_STATE_OFF);
	return false; // ID pin management has not been enabled
#endif
}


void usb_dual_disable(void)
{
	if (!usb_initialized) {
		return; // Dual role not initialized
	}
	usb_initialized = false;

#if USB_ID_EIC
	pad_id_interrupt_disable();
#endif
#if USB_ID_EIC || USB_VBUS_EIC
	eic_disable(EIC);
#endif
	usb_disable();
	//sysclk_disable_usb();
	uhd_sleep_mode(UHD_STATE_OFF);
}


#endif /* USB_H_INCLUDED */
