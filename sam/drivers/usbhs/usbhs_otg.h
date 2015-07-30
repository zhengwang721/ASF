/**
 * \file
 *
 * \brief USB OTG Driver for USBHS.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef USBHS_OTG_H_INCLUDED
#define USBHS_OTG_H_INCLUDED

#include "compiler.h"

#ifdef __cplusplus
extern "C" {
#endif


//! \ingroup usb_group
//! \defgroup otg_group USBHS OTG Driver
//! USBHS low-level driver for OTG features
//!
//! @{

/**
 * \brief Initialize the dual role
 * This function is implemented in usbhs_host.c file.
 *
 * \return \c true if the ID pin management has been started, otherwise \c false.
 */
bool otg_dual_enable(void);

/**
 * \brief Uninitialize the dual role
 * This function is implemented in usbhs_host.c file.
 */
void otg_dual_disable(void);


//! @name USBHS OTG ID pin management
//! The ID pin come from the USB OTG connector (A and B receptable) and
//! allows to select the USB mode host or device.
//! The USBHS hardware can manage it automatically. This feature is optional.
//! When USB_ID_GPIO is defined (in board.h), this feature is enabled.
//!
//! @{
   //! Enable external OTG_ID pin (listened to by USB)
 //! Disable external OTG_ID pin and force device mode
#define otg_force_device_mode()             (Set_bits(USBHS->USBHS_CTRL, USBHS_CTRL_UIMOD))
   //! Test if device mode is forced
#define Is_otg_device_mode_forced()         (Tst_bits(USBHS->USBHS_CTRL, USBHS_CTRL_UIMOD))
   //! Disable external OTG_ID pin and force host mode
#define otg_force_host_mode()               (Clr_bits(USBHS->USBHS_CTRL, USBHS_CTRL_UIMOD))
   //! Test if host mode is forced
#define Is_otg_host_mode_forced()           (!Tst_bits(USBHS->USBHS_CTRL, USBHS_CTRL_UIMOD))

//! @name USBHS OTG main management
//! These macros allows to enable/disable pad and USBHS hardware
//! @{
  //! Reset USB macro
#define otg_reset()                         \
	do {                                    \
		USBHS->USBHS_CTRL = 0;            \
		while( USBHS->USBHS_SR & 0x3FFF) {\
			USBHS->USBHS_SCR = 0xFFFFFFFF;\
		}                                   \
	} while (0)
  //! Enable USB macro
#define otg_enable()                        (Set_bits(USBHS->USBHS_CTRL, USBHS_CTRL_USBE))
  //! Disable USB macro                     
#define otg_disable()                       (Clr_bits(USBHS->USBHS_CTRL, USBHS_CTRL_USBE))
#define Is_otg_enabled()                    (Tst_bits(USBHS->USBHS_CTRL, USBHS_CTRL_USBE))

 //! Check Clock Usable               
  //! For parts with HS feature, this one corresponding at UTMI clock
#define Is_otg_clock_usable()               (Tst_bits(USBHS->USBHS_SR, USBHS_SR_CLKUSABLE))

  //! Stop (freeze) internal USB clock
#define otg_freeze_clock()                  (Set_bits(USBHS->USBHS_CTRL, USBHS_CTRL_FRZCLK))
#define otg_unfreeze_clock()                (Clr_bits(USBHS->USBHS_CTRL, USBHS_CTRL_FRZCLK))
#define Is_otg_clock_frozen()               (Tst_bits(USBHS->USBHS_CTRL, USBHS_CTRL_FRZCLK))

 //! @name USBHS OTG hardware protocol
//! These macros manages the hardware OTG protocol
//! @{
   //! Enables RDERR interrupt
#define otg_enable_rderr_interrupt()          (Set_bits(USBHS->USBHS_CTRL, USBHS_CTRL_RDERRE))
  //! Disables RDERR interrupt
#define otg_disable_rderr_interrupt()         (Clr_bits(USBHS->USBHS_CTRL, USBHS_CTRL_RDERRE))
#define Is_otg_rderr_interrupt_enabled()      (Tst_bits(USBHS->USBHS_CTRL, USBHS_CTRL_RDERRE))
  //! ACKs RDERR interrupt
#define otg_ack_rderr_interrupt()             (USBHS->USBHS_SCR = USBHS_SCR_RDERRIC)
  //! Raises RDERR interrupt
#define otg_raise_rderr_interrupt()           (USBHS->USBHS_SFR = USBHS_SFR_RDERRIS)
  //! Tests if a RDERR occurs
#define Is_otg_rderr_interrupt()              (Tst_bits(USBHS->USBHS_SR, USBHS_SR_RDERRI))
//! @}

//! @}

#ifdef __cplusplus
}
#endif


#endif /* USBHS_OTG_H_INCLUDED */
