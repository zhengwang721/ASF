/**
 * \file
 *
 * \brief USBC OTG Driver header file.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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

#ifndef _USBC_OTG_H_
#define _USBC_OTG_H_

#include "compiler.h"
#include "preprocessor.h"

// To simplify the macros definition of this file
#define USBC_CLR_BITS(reg, bit) \
		(Clr_bits(USBC->TPASTE2(USBC_,reg), TPASTE4(USBC_,reg,_,bit)))
#define USBC_SET_BITS(reg, bit) \
		(Set_bits(USBC->TPASTE2(USBC_,reg), TPASTE4(USBC_,reg,_,bit)))
#define USBC_RD_BITS(reg, bit) \
		(Rd_bits(USBC->TPASTE2(USBC_,reg), TPASTE4(USBC_,reg,_,bit)))
#define USBC_TST_BITS(reg, bit) \
		(Tst_bits(USBC->TPASTE2(USBC_,reg), TPASTE4(USBC_,reg,_,bit)))
#define USBC_RD_BITFIELD(reg, bit) \
		(Rd_bitfield(USBC->TPASTE2(USBC_,reg),\
		TPASTE5(USBC_,reg,_,bit,_Msk)))
#define USBC_WR_BITFIELD(reg, bit, value) \
		(Wr_bitfield(USBC->TPASTE2(USBC_,reg),\
		TPASTE5(USBC_,reg,_,bit,_Msk),value))
#define USBC_REG_CLR(reg, bit) \
		(USBC->TPASTE3(USBC_,reg,CLR) = TPASTE5(USBC_,reg,CLR_,bit,C))
#define USBC_REG_SET(reg, bit) \
		(USBC->TPASTE3(USBC_,reg,SET) = TPASTE5(USBC_,reg,SET_,bit,S))

//! \ingroup usb_group
//! \defgroup otg_usbc_group USBC OTG Driver
//! USBC low-level driver for OTG features
//!
//! @{

/**
 * \brief Initialize the dual role
 * This function is implemented in usbc_host.c file.
 *
 * \return \c true if the ID pin management has been started, otherwise \c false.
 */
bool otg_dual_enable(void);

/**
 * \brief Uninitialize the dual role
 * This function is implemented in usbc_host.c file.
 */
void otg_dual_disable(void);

//! @name USBC IP properties
//! These macros give access to IP properties
//! @{

//! Get IP name part 1 or 2
#define  otg_get_ip_name() \
		(((uint64_t)USBC->USBC_UNAME1<<32)|(uint64_t)USBC->USBC_UNAME2)
#define  otg_data_memory_barrier()      do { barrier(); } while (0)
//! Get IP version
#define  otg_get_ip_version()           USBC_RD_BITFIELD(UVERS,VERSION)
//! Get maximal number of pipes/endpoints
#define  otg_get_max_nbr_endpoints()    USBC_RD_BITFIELD(UFEATURES, EPTNBRMAX)
#define  otg_get_max_nbr_pipes()        USBC_RD_BITFIELD(UFEATURES, EPTNBRMAX)

//! @}

//! @name USBC OTG ID pin management
//! The ID pin come from the USB OTG connector (A and B receptable) and
//! allows to select the USB mode host or device.
//! The USBC hardware can manage it automatically. This feature is optional
//! and not implement by SAM4L.
//!
//! If ID pin is used, USB_ID_GPIO must be defined in board.h.
//!
//! @{
#ifdef USBC_USBCON_UIDE
#define  otg_enable_id_pin()                 USBC_SET_BITS(USBCON,UIDE)
#define  otg_disable_id_pin()                USBC_CLR_BITS(USBCON,UIDE)
#else
#define  otg_enable_id_pin()                 do { } while(0)
#define  otg_disable_id_pin()                do { } while(0)
#endif
#define  otg_force_device_mode()             USBC_SET_BITS(USBCON,UIMOD)
#define  Is_otg_device_mode_forced()         USBC_TST_BITS(USBCON,UIMOD)
#define  otg_force_host_mode()               USBC_CLR_BITS(USBCON,UIMOD)
#define  Is_otg_host_mode_forced()           (!Is_otg_device_mode_forced())

//! @name USBC OTG ID pin interrupt management
//! These macros manage the ID pin interrupt
//! @{
#ifdef USBC_USBCON_IDTE
#define  otg_enable_id_interrupt()           USBC_SET_BITS(USBCON,IDTE)
#define  otg_disable_id_interrupt()          USBC_CLR_BITS(USBCON,IDTE)
#define  Is_otg_id_interrupt_enabled()       USBC_TST_BITS(USBCON,IDTE)
#define  otg_ack_id_transition()             USBC_REG_CLR(USBSTA,IDTI)
#define  Is_otg_id_transition()              USBC_TST_BITS(USBSTA,IDTI)
#else
#define  otg_enable_id_interrupt()           do { } while(0)
#define  otg_disable_id_interrupt()          do { } while(0)
#define  Is_otg_id_interrupt_enabled()       false
#define  otg_ack_id_transition()             do { } while(0)
#define  Is_otg_id_transition()              false
#endif

#ifdef USBC_USBCON_ID
#define  Is_otg_id_device()                  USBC_TST_BITS(USBSTA,ID)
#else
#define  Is_otg_id_device()                  Is_otg_device_mode_forced()
#endif
#define  Is_otg_id_host()                    (!Is_otg_id_device())
//! @}
//! @}

//! @name USBC OTG Vbus management
//! The USBC hardware can manage VBus automatically. This feature is optional
//! and not implement by SAM4L.
//! @{
#ifdef USBC_USBCON_VBUSHWC
#define  otg_enable_vbus_hw_control()          USBC_SET_BITS(USBCON,VBUSHWC)
#define  otg_disable_vbus_hw_control()         USBC_CLR_BITS(USBCON,VBUSHWC)
#define  Is_otg_vbus_hw_control_enabled()      USBC_TST_BITS(USBCON,VBUSHWC)
#else
#define  otg_enable_vbus_hw_control()          do { } while(0)
#define  otg_disable_vbus_hw_control()         do { } while(0)
#define  Is_otg_vbus_hw_control_enabled()      false
#endif

#ifdef USBC_USBCON_VBUSTE
#define  otg_enable_vbus_interrupt()         USBC_SET_BITS(USBCON,VBUSTE)
#define  otg_disable_vbus_interrupt()        USBC_CLR_BITS(USBCON,VBUSTE)
#define  Is_otg_vbus_interrupt_enabled()     USBC_TST_BITS(USBCON,VBUSTE)
#define  otg_ack_vbus_transition()           USBC_REG_CLR(USBSTA,VBUSTI)
#define  otg_raise_vbus_transition()         USBC_REG_SET(USBSTA,VBUSTI)
#define  Is_otg_vbus_transition()            USBC_TST_BITS(USBSTA,VBUSTI)
#else
#define  otg_enable_vbus_interrupt()         do { } while(0)
#define  otg_disable_vbus_interrupt()        do { } while(0)
#define  Is_otg_vbus_interrupt_enabled()     false
#define  otg_ack_vbus_transition()           do { } while(0)
#define  otg_raise_vbus_transition()         do { } while(0)
#define  Is_otg_vbus_transition()            false
#endif

#ifdef USBC_USBSTA_VBUS
#define  Is_otg_vbus_high()                  USBC_TST_BITS(USBSTA,VBUS)
#else
#define  Is_otg_vbus_high()                  true
#endif
#define  Is_otg_vbus_low()                   (!Is_otg_vbus_high())

#ifdef USBC_USBCON_VBERRE
#define  otg_enable_vbus_error_interrupt()     USBC_SET_BITS(USBCON,VBERRE)
#define  otg_disable_vbus_error_interrupt()    USBC_CLR_BITS(USBCON,VBERRE)
#define  Is_otg_vbus_error_interrupt_enabled() USBC_TST_BITS(USBCON,VBERRE)
#define  Is_otg_vbus_error()                   USBC_TST_BITS(USBSTA,VBERRI)
#define  otg_ack_vbus_error()                  USBC_REG_CLR(USBSTA,VBERRI)
#define  otg_raise_vbus_error()                USBC_REG_SET(USBSTA,VBERRI)
#else
#define  otg_enable_vbus_error_interrupt()     do { } while(0)
#define  otg_disable_vbus_error_interrupt()    do { } while(0)
#define  Is_otg_vbus_error_interrupt_enabled() false
#define  Is_otg_vbus_error()                   false
#define  otg_ack_vbus_error()                  do { } while(0)
#define  otg_raise_vbus_error()                do { } while(0)
#endif

#ifdef USBC_USBCON_VBUSPO
#define  otg_set_vbus_polarity_active_high()   USBC_CLR_BITS(USBCON,VBUSPO)
#define  otg_set_vbus_polarity_active_low()    USBC_SET_BITS(USBCON,VBUSPO)
#define  Is_otg_vbus_polarity_active_low()     USBC_TST_BITS(USBCON,VBUSPO)
#else
#define  otg_set_vbus_polarity_active_high()   do { } while(0)
#define  otg_set_vbus_polarity_active_low()    do { } while(0)
#define  Is_otg_vbus_polarity_active_low()     false
#endif

#ifdef USBC_USBSTA_VBUSRQ
#define  otg_enable_vbus()                     USBC_REG_SET(USBSTA,VBUSRQ)
#define  otg_disable_vbus()                    USBC_REG_CLR(USBSTA,VBUSRQ)
#define  Is_otg_vbus_enabled()                 USBC_TST_BITS(USBSTA,VBUSRQ)
#else
#define  otg_enable_vbus()                     do { } while(0)
#define  otg_disable_vbus()                    do { } while(0)
#define  Is_otg_vbus_enabled()                 true
#endif
//! @}

//! @name USBC OTG main management
//! These macros allows to enable/disable pad and USBC hardware
//! @{
#define  otg_enable()                        USBC_SET_BITS(USBCON,USBE)
#define  otg_disable()                       USBC_CLR_BITS(USBCON,USBE)

#ifdef USBC_USBCON_OTGPADE
#define  otg_enable_pad()                    USBC_SET_BITS(USBCON,OTGPADE)
#define  otg_disable_pad()                   USBC_CLR_BITS(USBCON,OTGPADE)
#else
#define  otg_enable_pad()                    do { } while(0)
#define  otg_disable_pad()                   do { } while(0)
#endif

#define otg_register_desc_tab(addr) \
		(Wr_bitfield(USBC->USBC_UDESC, USBC_UDESC_UDESCA_Msk, addr))

//! Check transceiver state
#define Is_otg_suspend()                     USBC_TST_BITS(USBSTA,SUSPEND)
#define Is_otg_transceiver_on()              (!Is_otg_suspend())
#define Is_otg_transceiver_off()             (Is_otg_suspend())

//! Check USB interface clock usable
#define  Is_otg_clock_usable()               USBC_TST_BITS(USBSTA,CLKUSABLE)

#define  otg_freeze_clock()                  USBC_SET_BITS(USBCON,FRZCLK)
#define  otg_unfreeze_clock()                USBC_CLR_BITS(USBCON,FRZCLK)
#define  Is_otg_clock_frozen()               USBC_TST_BITS(USBCON,FRZCLK)
//! @}

//! @name USBC OTG hardware protocol
//! These macros manages the hardware OTG protocol
//! @{

#ifdef USBC_USBCON_TIMPAGE
//! Configure time-out of specified OTG timer
#define  otg_configure_timeout(timer, timeout) \
		(USBC_SET_BITS(USBCON,UNLOCK),\
		USBC_WR_BITFIELD(USBCON,TIMPAGE,timer),\
		USBC_WR_BITFIELD(USBCON,TIMVALUE,timeout),\
		USBC_CLR_BITS(USBCON,UNLOCK))
//! Get configured time-out of specified OTG timer
#define  otg_get_timeout(timer) \
		(USBC_SET_BITS(USBCON,UNLOCK),\
		USBC_WR_BITFIELD(USBCON,TIMPAGE,timer),\
		USBC_CLR_BITS(USBCON,UNLOCK),\
		USBC_RD_BITFIELD(USBCON,TIMVALUE))
#else
//! Configure time-out of specified OTG timer
#define  otg_configure_timeout(timer, timeout) do { } while (0)
//! Get configured time-out of specified OTG timer
#define  otg_get_timeout(timer)                0
#endif

//! Get the dual-role device state of the internal USB finite state machine of the USBC controller
#define  otg_get_fsm_drd_state()             USBC_RD_BITFIELD(USBFSM,DRDSTATE)

//! Host Negotiation Protocol
//! @{
#ifdef USBC_USBCON_HNPREQ
#define  otg_device_initiate_hnp()            USBC_SET_BITS(USBCON,HNPREQ)
#define  otg_host_accept_hnp()                USBC_SET_BITS(USBCON,HNPREQ)
#define  otg_host_reject_hnp()                USBC_CLR_BITS(USBCON,HNPREQ)
#define  Is_otg_hnp()                         USBC_TST_BITS(USBCON,HNPREQ)
#define  otg_enable_hnp_error_interrupt()     USBC_SET_BITS(USBCON,HNPERRE)
#define  otg_disable_hnp_error_interrupt()    USBC_CLR_BITS(USBCON,HNPERRE)
#define  Is_otg_hnp_error_interrupt_enabled() USBC_TST_BITS(USBCON,HNPERRE)
#define  otg_ack_hnp_error_interrupt()        USBC_REG_CLR(USBSTA,HNPERRI)
#define  Is_otg_hnp_error_interrupt()         USBC_TST_BITS(USBSTA,HNPERRI)
#else
#define  otg_device_initiate_hnp()            do { } while(0)
#define  otg_host_accept_hnp()                do { } while(0)
#define  otg_host_reject_hnp()                do { } while(0)
#define  Is_otg_hnp()                         false
#define  otg_enable_hnp_error_interrupt()     do { } while(0)
#define  otg_disable_hnp_error_interrupt()    do { } while(0)
#define  Is_otg_hnp_error_interrupt_enabled() false
#define  otg_ack_hnp_error_interrupt()        do { } while(0)
#define  Is_otg_hnp_error_interrupt()         false
#endif
//! @}

//! Session Request Protocol
//! @{
#ifdef USBC_USBCON_SRPREQ
#define  otg_device_initiate_srp()           USBC_SET_BITS(USBCON,SRPREQ)
#define  Is_otg_device_srp()                 USBC_TST_BITS(USBCON,SRPREQ)
#define  otg_select_vbus_srp_method()        USBC_SET_BITS(USBCON,SRPSEL)
#define  Is_otg_vbus_srp_method_selected()   USBC_TST_BITS(USBCON,SRPSEL)
#define  otg_select_data_srp_method()        USBC_CLR_BITS(USBCON,SRPSEL)
#define  otg_enable_srp_interrupt()          USBC_SET_BITS(USBCON,SRPE)
#define  otg_disable_srp_interrupt()         USBC_CLR_BITS(USBCON,SRPE)
#define  Is_otg_srp_interrupt_enabled()      USBC_TST_BITS(USBCON,SRPE)
#define  otg_ack_srp_interrupt()             USBC_REG_CLR(USBSTA,SRPI)
#define  Is_otg_srp_interrupt()              USBC_TST_BITS(USBSTA,SRPI)
#else
#define  otg_device_initiate_srp()           do { } while (0)
#define  Is_otg_device_srp()                 do { } while (0)
#define  otg_select_vbus_srp_method()        do { } while (0)
#define  Is_otg_vbus_srp_method_selected()   false
#define  otg_select_data_srp_method()        do { } while (0)
#define  otg_enable_srp_interrupt()          do { } while (0)
#define  otg_disable_srp_interrupt()         do { } while (0)
#define  Is_otg_srp_interrupt_enabled()      false
#define  otg_ack_srp_interrupt()             do { } while (0)
#define  Is_otg_srp_interrupt()              false
#endif
#define  Is_otg_data_srp_method_selected()   (!Is_otg_vbus_srp_method_selected())
//! @}

//! Role exchange interrupt
//! @{
#ifdef USBC_USBCON_ROLEEXE
#define  otg_enable_role_exchange_interrupt()     USBC_SET_BITS(USBCON,ROLEEXE)
#define  otg_disable_role_exchange_interrupt()    USBC_CLR_BITS(USBCON,ROLEEXE)
#define  Is_otg_role_exchange_interrupt_enabled() USBC_TST_BITS(USBCON,ROLEEXE)
#define  otg_ack_role_exchange_interrupt()        USBC_REG_CLR(USBSTA,ROLEEXI)
#define  otg_raise_role_exchange_interrupt()      USBC_REG_SET(USBSTA,ROLEEXI)
#define  Is_otg_role_exchange_interrupt()         USBC_TST_BITS(USBSTA,ROLEEXI)
#else
#define  otg_enable_role_exchange_interrupt()     do { } while (0)
#define  otg_disable_role_exchange_interrupt()    do { } while (0)
#define  Is_otg_role_exchange_interrupt_enabled() false
#define  otg_ack_role_exchange_interrupt()        do { } while (0)
#define  otg_raise_role_exchange_interrupt()      do { } while (0)
#define  Is_otg_role_exchange_interrupt()         false
#endif
//! @}
//! @}

//! @}

#endif // _USBC_OTG_H_
