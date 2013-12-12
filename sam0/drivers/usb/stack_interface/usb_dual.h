/**
 * \file
 *
 * \brief SAM Dxx USB dual driver header file.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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

#ifndef _USB_DUAL_H_
#define _USB_DUAL_H_

#include "compiler.h"
#include "preprocessor.h"

/* Get USB pads pins configuration in board configuration */
//#include "conf_board.h"
//#include "board.h"
//#include "usb_host.h"
//#include "eic.h"

/** Below will be in board.h */ 
/**
 * \name USB pin definitions
 *
 * These are named according to the USBC driver's requirements.
 */
//@{
#define USB_ID_PIN                PIN_PA03
#define USB_ID_FLAGS              (IOPORT_MODE_PULLUP \
		| IOPORT_MODE_GLITCH_FILTER)
#define USB_VBUS_PIN              PIN_PA14
#define USB_VBUS_FLAGS            IOPORT_MODE_GLITCH_FILTER
//@}

/**
 * \name USB IO PADs management
 */
//@{
__always_inline static void eic_line_change_config(uint8_t line, bool b_high)
{
	struct eic_line_config eic_line_conf;
	eic_line_conf.eic_mode = EIC_MODE_LEVEL_TRIGGERED;
	eic_line_conf.eic_level = b_high ?
			EIC_LEVEL_HIGH_LEVEL : EIC_LEVEL_LOW_LEVEL;
	eic_line_conf.eic_filter = EIC_FILTER_DISABLED;
	eic_line_conf.eic_async = EIC_ASYNCH_MODE;
	eic_line_set_config(EIC, line, &eic_line_conf);
}

__always_inline static void eic_pad_init(uint8_t line, eic_callback_t callback,
		uint8_t irq_line, ioport_pin_t pin, uint8_t irq_level)
{
	eic_line_disable_interrupt(EIC, line);
	eic_line_disable(EIC, line);
	eic_line_clear_interrupt(EIC, line);
	eic_line_set_callback(EIC, line, callback, irq_line, irq_level);
	eic_line_change_config(line, !ioport_get_pin_level(pin));
	eic_line_enable(EIC, line);
	eic_line_enable_interrupt(EIC, line);
}
//@}

//! \ingroup usb_group
//! \defgroup usb_dual_group USB dual driver
//! USB low-level driver for dual features
//!
//! @{

/**
 * \brief Initialize the dual role
 * This function is implemented in usb_host.c file.
 *
 * \return \c true if the ID pin management has been started, otherwise \c false.
 */
bool usb_dual_enable(void);

/**
 * \brief Uninitialize the dual role
 * This function is implemented in usb_host.c file.
 */
void usb_dual_disable(void);

//! @name USB ID pin management
//! The ID pin come from the USB connector (A and B receptable) and
//! allows to select the USB mode host or device.
//! The ID pin can be managed through GPIO or EIC pin.
//! This feature is optional, and it is enabled if USB_ID_PIN or USB_ID_EIC
//! is defined in board.h and CONF_BOARD_USB_ID_DETECT defined in
//! conf_board.h.
//!
//! @{
#define USB_ID_DETECT       (defined(CONF_BOARD_USB_ID_DETECT))
#define USB_ID_EIC           (defined(USB_ID_PIN) && USB_ID_DETECT)

# define pad_id_init() \
	eic_pad_init(USB_ID_EIC_LINE, otg_id_handler, USB_ID_EIC_IRQn, USB_ID_EIC, UHD_USB_INT_LEVEL);
# define pad_id_interrupt_disable() eic_line_disable_interrupt(EIC, USB_ID_EIC_LINE)
# define pad_ack_id_interrupt() \
	(eic_line_change_config(USB_ID_EIC_LINE, !ioport_get_pin_level(USB_ID_EIC)), \
	eic_line_clear_interrupt(EIC, USB_ID_EIC_LINE))
# define Is_pad_id_device()         ioport_get_pin_level(USB_ID_EIC)
//! @}

//! @name USB Vbus management
//!
//! The VBus line can be monitored through a GPIO pin and
//! a basic resitor voltage divider.
//! This feature is optional, and it is enabled if USB_VBUS_PIN or USB_VBUS_EIC
//! is defined in board.h and CONF_BOARD_USB_VBUS_DETECT defined in
//! conf_board.h.
//! @{
#define USB_VBUS_DETECT     (defined(CONF_BOARD_USB_VBUS_DETECT))
#define USB_VBUS_EIC         (defined(USB_VBUS_PIN) && USB_VBUS_DETECT)

# define pad_vbus_init(level) \
	eic_pad_init(USB_VBUS_EIC_LINE, uhd_vbus_handler, USB_VBUS_EIC_IRQn, USB_VBUS_EIC, level);
# define pad_vbus_interrupt_disable() eic_line_disable_interrupt(EIC, USB_VBUS_EIC_LINE)
# define pad_ack_vbus_interrupt() \
	(eic_line_change_config(USB_VBUS_EIC_LINE, !ioport_get_pin_level(USB_VBUS_EIC)), \
	eic_line_clear_interrupt(EIC, USB_VBUS_EIC_LINE))
# define Is_pad_vbus_high()           ioport_get_pin_level(USB_VBUS_EIC)

//! @}




//! @}

#endif // _USB_DUAL_H_
