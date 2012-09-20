/**
 * \file
 *
 * \brief EIC driver
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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

#ifndef _EIC_H_
#define _EIC_H_

/**
 * \defgroup group_sam_drivers_eic External Interrupt Controller
 *
 * EIC allows pins to be configured as external interrupts.
 *
 * \{
 */

#include "compiler.h"

/*! \name External Interrupt lines
 */
//! @{
#define EXT_NMI     0 //!< Line NMI
#define EXT_INT1    1 //!< Line 1
#define EXT_INT2    2 //!< Line 2
#define EXT_INT3    3 //!< Line 3
#define EXT_INT4    4 //!< Line 4
#define EXT_INT5    5 //!< Line 5
#define EXT_INT6    6 //!< Line 6
#define EXT_INT7    7 //!< Line 7
#define EXT_INT8    8 //!< Line 8
//! @}

/*! \name Mode Trigger Options
 */
//! @{
#define EIC_MODE_EDGE_TRIGGERED   0 //!<
#define EIC_MODE_LEVEL_TRIGGERED  1 //!<
//! @}

/*! \name Edge level Options
 */
//! @{
#define EIC_EDGE_FALLING_EDGE     0 //!<
#define EIC_EDGE_RISING_EDGE      1 //!<
//! @}

/*! \name Level Options
 */
//! @{
#define EIC_LEVEL_LOW_LEVEL       0 //!<
#define EIC_LEVEL_HIGH_LEVEL      1 //!<
//! @}

/*! \name Filter Options
 */
//! @{
#define EIC_FILTER_ENABLED        1 //!<
#define EIC_FILTER_DISABLED       0 //!<
//! @}

/*! \name Synch Mode Options
 */
//! @{
#define EIC_SYNCH_MODE            0 //!<
#define EIC_ASYNCH_MODE           1 //!<
//! @}

//! Configuration parameters of the EIC module.
typedef struct
{
  //!Line
  unsigned char eic_line;

  //! Mode : EDGE_LEVEL or TRIGGER_LEVEL
  unsigned char eic_mode;

  //! Edge : FALLING_EDGE or RISING_EDGE
  unsigned char eic_edge;

  //! Level :  LOW_LEVEL or HIGH_LEVEL
  unsigned char eic_level;

  //! Filter:  NOT_FILTERED or FILTERED
  unsigned char eic_filter;

  //! Async:  SYNC mode or ASYNC
  unsigned char eic_async;

} eic_options_t;


/*! \brief Init the EIC driver.
 *
 * \param eic Base address of the EIC module
 * \param opt Configuration parameters of the EIC module (see \ref eic_options_t)
 * \param nb_lines Number of lines to consider, equal to size of opt buffer
 */
extern void eic_init(volatile Eic* eic, const eic_options_t *opt, uint32_t nb_lines);

/*! \brief Enable the EIC driver.
 *
 * \param eic Base address of the EIC module
 * \param mask_lines Mask for current selected lines
 */
extern void eic_enable_lines(volatile Eic* eic, uint32_t mask_lines);

/*! \brief Enable the EIC driver.
 *
 * \param eic Base address of the EIC module
 * \param line_number Line number to enable
 */
extern void eic_enable_line(volatile Eic* eic, uint32_t line_number);

/*! \brief Disable the EIC driver.
 *
 * \param eic Base address of the EIC module
 * \param mask_lines Mask for current selected lines
 */
extern void eic_disable_lines(volatile Eic* eic, uint32_t mask_lines);

/*! \brief Disable the EIC driver.
 *
 * \param eic Base address of the EIC module
 * \param line_number Line number to disable
 */
extern void eic_disable_line(volatile Eic* eic, uint32_t line_number);

/*! \brief Tells whether an EIC line is enabled.
 *
 * \param eic Base address of the EIC module
 * \param line_number Line number to test
 *
 * \return Whether an EIC line is enabled.
 */
extern bool eic_is_line_enabled(volatile Eic* eic, uint32_t line_number);

/*! \name Interrupt Control Functions
 */
//! @{

/*! \brief Enable the interrupt feature of the EIC.
 *
 * \param eic Base address of the EIC (i.e. EIC).
 * \param mask_lines Mask for current selected lines
 */
extern void eic_enable_interrupt_lines(volatile Eic* eic, uint32_t mask_lines);

/*! \brief Enable the interrupt feature of the EIC.
 *
 * \param eic Base address of the EIC (i.e. EIC).
 * \param line_number Line number to enable
 */
extern void eic_enable_interrupt_line(volatile Eic* eic, uint32_t line_number);

/*! \brief Disable the interrupt feature of the EIC.
 *
 * \param eic Base address of the EIC (i.e. EIC).
 * \param mask_lines Mask for current selected lines
 */
extern void eic_disable_interrupt_lines(volatile Eic* eic, uint32_t mask_lines);

/*! \brief Disable the interrupt feature of the EIC.
 *
 * \param eic Base address of the EIC (i.e. EIC).
 * \param line_number Line number to disable
 */
extern void eic_disable_interrupt_line(volatile Eic* eic, uint32_t line_number);

/*! \brief Tells whether an EIC interrupt line is enabled.
 *
 * \param eic Base address of the EIC module
 * \param line_number Line number to test
 *
 * \return Whether an EIC interrupt line is enabled.
 */
extern bool eic_is_interrupt_line_enabled(volatile Eic* eic, uint32_t line_number);

/*! \brief Clear the interrupt flag.
 *         Call this function once you've handled the interrupt.
 *
 * \param eic Base address of the EIC (i.e. EIC).
 * \param mask_lines Mask for current selected lines
 */
extern void eic_clear_interrupt_lines(volatile Eic* eic, uint32_t mask_lines);

/*! \brief Clear the interrupt flag.
 *         Call this function once you've handled the interrupt.
 *
 * \param eic Base address of the EIC (i.e. EIC).
 * \param line_number Line number to clear
 */
extern void eic_clear_interrupt_line(volatile Eic* eic, uint32_t line_number);

/*! \brief Tells whether an EIC interrupt line is pending.
 *
 * \param eic Base address of the EIC module
 * \param line_number Line number to test
 *
 * \return Whether an EIC interrupt line is pending.
 */
extern bool eic_is_interrupt_line_pending(volatile Eic* eic, uint32_t line_number);

//! @}

/**
 * \}
 */

#endif  // _EIC_H_
