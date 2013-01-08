/**
 * \file
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

#ifndef _SAMD20_SYSCTRL_COMPONENT_
#define _SAMD20_SYSCTRL_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR SYSCTRL */
/* ========================================================================== */
/** \addtogroup SAMD20_SYSCTRL System Control */
/*@{*/

#define REV_SYSCTRL    0x100

/* -------- SYSCTRL_INTENCLR : (SYSCTRL Offset: 0x00) (R/W 32) Interrupt Enable Clear Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t XOSCRDY:1;        /*!< bit:      0  XOSC Ready                         */
    uint32_t XOSC32KRDY:1;     /*!< bit:      1  XOSC32K Ready                      */
    uint32_t BOD33RDY:1;       /*!< bit:      2  BOD33 Ready                        */
    uint32_t BOD12RDY:1;       /*!< bit:      3  BOD12 Ready                        */
    uint32_t BOD33DET:1;       /*!< bit:      4  BOD33 Detection                    */
    uint32_t BOD12DET:1;       /*!< bit:      5  BOD12 Detection                    */
    uint32_t BOD33BSY:1;       /*!< bit:      6  BOD33 Synchronization Busy         */
    uint32_t BOD12BSY:1;       /*!< bit:      7  BOD12 Synchronization Busy         */
    uint32_t DFLLRDY:1;        /*!< bit:      8  DFLL Ready                         */
    uint32_t DFLLOOB:1;        /*!< bit:      9  DFLL Out Of Bounds                 */
    uint32_t DFLLLCKF:1;       /*!< bit:     10  DFLL Lock Fine                     */
    uint32_t DFLLLCKC:1;       /*!< bit:     11  DFLL Lock Coarse                   */
    uint32_t DFLLRCS:1;        /*!< bit:     12  DFLL Reference Clock Stopped       */
    uint32_t OSC32KRDY:1;      /*!< bit:     13  OSC32K Ready                       */
    uint32_t :18;              /*!< bit: 14..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_INTENCLR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_INTENCLR_OFFSET     0x00         /**< \brief (SYSCTRL_INTENCLR offset) Interrupt Enable Clear Register */
#define SYSCTRL_INTENCLR_RESETVALUE 0x00000000   /**< \brief (SYSCTRL_INTENCLR reset_value) Interrupt Enable Clear Register */

#define SYSCTRL_INTENCLR_XOSCRDY    (0x1u <<  0) /**< \brief (SYSCTRL_INTENCLR) XOSC Ready */
#define SYSCTRL_INTENCLR_XOSC32KRDY (0x1u <<  1) /**< \brief (SYSCTRL_INTENCLR) XOSC32K Ready */
#define SYSCTRL_INTENCLR_BOD33RDY   (0x1u <<  2) /**< \brief (SYSCTRL_INTENCLR) BOD33 Ready */
#define SYSCTRL_INTENCLR_BOD12RDY   (0x1u <<  3) /**< \brief (SYSCTRL_INTENCLR) BOD12 Ready */
#define SYSCTRL_INTENCLR_BOD33DET   (0x1u <<  4) /**< \brief (SYSCTRL_INTENCLR) BOD33 Detection */
#define SYSCTRL_INTENCLR_BOD12DET   (0x1u <<  5) /**< \brief (SYSCTRL_INTENCLR) BOD12 Detection */
#define SYSCTRL_INTENCLR_BOD33BSY   (0x1u <<  6) /**< \brief (SYSCTRL_INTENCLR) BOD33 Synchronization Busy */
#define SYSCTRL_INTENCLR_BOD12BSY   (0x1u <<  7) /**< \brief (SYSCTRL_INTENCLR) BOD12 Synchronization Busy */
#define SYSCTRL_INTENCLR_DFLLRDY    (0x1u <<  8) /**< \brief (SYSCTRL_INTENCLR) DFLL Ready */
#define SYSCTRL_INTENCLR_DFLLOOB    (0x1u <<  9) /**< \brief (SYSCTRL_INTENCLR) DFLL Out Of Bounds */
#define SYSCTRL_INTENCLR_DFLLLCKF   (0x1u << 10) /**< \brief (SYSCTRL_INTENCLR) DFLL Lock Fine */
#define SYSCTRL_INTENCLR_DFLLLCKC   (0x1u << 11) /**< \brief (SYSCTRL_INTENCLR) DFLL Lock Coarse */
#define SYSCTRL_INTENCLR_DFLLRCS    (0x1u << 12) /**< \brief (SYSCTRL_INTENCLR) DFLL Reference Clock Stopped */
#define SYSCTRL_INTENCLR_OSC32KRDY  (0x1u << 13) /**< \brief (SYSCTRL_INTENCLR) OSC32K Ready */
#define SYSCTRL_INTENCLR_MASK       0x00003FFFu  /**< \brief (SYSCTRL_INTENCLR) MASK Register */

/* -------- SYSCTRL_INTENSET : (SYSCTRL Offset: 0x04) (R/W 32) Interrupt Enable Set Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t XOSCRDY:1;        /*!< bit:      0  XOSC Ready                         */
    uint32_t XOSC32KRDY:1;     /*!< bit:      1  XOSC32K Ready                      */
    uint32_t BOD33RDY:1;       /*!< bit:      2  BOD33 Ready                        */
    uint32_t BOD12RDY:1;       /*!< bit:      3  BOD12 Ready                        */
    uint32_t BOD33DET:1;       /*!< bit:      4  BOD33 Detection                    */
    uint32_t BOD12DET:1;       /*!< bit:      5  BOD12 Detection                    */
    uint32_t BOD33BSY:1;       /*!< bit:      6  BOD33 Synchronization Busy         */
    uint32_t BOD12BSY:1;       /*!< bit:      7  BOD12 Synchronization Busy         */
    uint32_t DFLLRDY:1;        /*!< bit:      8  DFLL Ready                         */
    uint32_t DFLLOOB:1;        /*!< bit:      9  DFLL Out Of Bounds                 */
    uint32_t DFLLLCKF:1;       /*!< bit:     10  DFLL Lock Fine                     */
    uint32_t DFLLLCKC:1;       /*!< bit:     11  DFLL Lock Coarse                   */
    uint32_t DFLLRCS:1;        /*!< bit:     12  DFLL Reference Clock Stopped       */
    uint32_t OSC32KRDY:1;      /*!< bit:     13  OSC32K Ready                       */
    uint32_t :18;              /*!< bit: 14..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_INTENSET_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_INTENSET_OFFSET     0x04         /**< \brief (SYSCTRL_INTENSET offset) Interrupt Enable Set Register */
#define SYSCTRL_INTENSET_RESETVALUE 0x00000000   /**< \brief (SYSCTRL_INTENSET reset_value) Interrupt Enable Set Register */

#define SYSCTRL_INTENSET_XOSCRDY    (0x1u <<  0) /**< \brief (SYSCTRL_INTENSET) XOSC Ready */
#define SYSCTRL_INTENSET_XOSC32KRDY (0x1u <<  1) /**< \brief (SYSCTRL_INTENSET) XOSC32K Ready */
#define SYSCTRL_INTENSET_BOD33RDY   (0x1u <<  2) /**< \brief (SYSCTRL_INTENSET) BOD33 Ready */
#define SYSCTRL_INTENSET_BOD12RDY   (0x1u <<  3) /**< \brief (SYSCTRL_INTENSET) BOD12 Ready */
#define SYSCTRL_INTENSET_BOD33DET   (0x1u <<  4) /**< \brief (SYSCTRL_INTENSET) BOD33 Detection */
#define SYSCTRL_INTENSET_BOD12DET   (0x1u <<  5) /**< \brief (SYSCTRL_INTENSET) BOD12 Detection */
#define SYSCTRL_INTENSET_BOD33BSY   (0x1u <<  6) /**< \brief (SYSCTRL_INTENSET) BOD33 Synchronization Busy */
#define SYSCTRL_INTENSET_BOD12BSY   (0x1u <<  7) /**< \brief (SYSCTRL_INTENSET) BOD12 Synchronization Busy */
#define SYSCTRL_INTENSET_DFLLRDY    (0x1u <<  8) /**< \brief (SYSCTRL_INTENSET) DFLL Ready */
#define SYSCTRL_INTENSET_DFLLOOB    (0x1u <<  9) /**< \brief (SYSCTRL_INTENSET) DFLL Out Of Bounds */
#define SYSCTRL_INTENSET_DFLLLCKF   (0x1u << 10) /**< \brief (SYSCTRL_INTENSET) DFLL Lock Fine */
#define SYSCTRL_INTENSET_DFLLLCKC   (0x1u << 11) /**< \brief (SYSCTRL_INTENSET) DFLL Lock Coarse */
#define SYSCTRL_INTENSET_DFLLRCS    (0x1u << 12) /**< \brief (SYSCTRL_INTENSET) DFLL Reference Clock Stopped */
#define SYSCTRL_INTENSET_OSC32KRDY  (0x1u << 13) /**< \brief (SYSCTRL_INTENSET) OSC32K Ready */
#define SYSCTRL_INTENSET_MASK       0x00003FFFu  /**< \brief (SYSCTRL_INTENSET) MASK Register */

/* -------- SYSCTRL_INTFLAG : (SYSCTRL Offset: 0x08) (R/W 32) Interrupt Flag Status and Clear Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t XOSCRDY:1;        /*!< bit:      0  XOSC Ready                         */
    uint32_t XOSC32KRDY:1;     /*!< bit:      1  XOSC32K Ready                      */
    uint32_t BOD33RDY:1;       /*!< bit:      2  BOD33 Ready                        */
    uint32_t BOD12RDY:1;       /*!< bit:      3  BOD12 Ready                        */
    uint32_t BOD33DET:1;       /*!< bit:      4  BOD33 Detection                    */
    uint32_t BOD12DET:1;       /*!< bit:      5  BOD12 Detection                    */
    uint32_t BOD33BSY:1;       /*!< bit:      6  BOD33 Synchronization Busy         */
    uint32_t BOD12BSY:1;       /*!< bit:      7  BOD12 Synchronization Busy         */
    uint32_t DFLLRDY:1;        /*!< bit:      8  DFLL Ready                         */
    uint32_t DFLLOOB:1;        /*!< bit:      9  DFLL Out Of Bounds                 */
    uint32_t DFLLLCKF:1;       /*!< bit:     10  DFLL Lock Fine                     */
    uint32_t DFLLLCKC:1;       /*!< bit:     11  DFLL Lock Coarse                   */
    uint32_t DFLLRCS:1;        /*!< bit:     12  DFLL Reference Clock Stopped       */
    uint32_t OSC32KRDY:1;      /*!< bit:     13  OSC32K Ready                       */
    uint32_t :18;              /*!< bit: 14..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_INTFLAG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_INTFLAG_OFFSET      0x08         /**< \brief (SYSCTRL_INTFLAG offset) Interrupt Flag Status and Clear Register */
#define SYSCTRL_INTFLAG_RESETVALUE  0x00000000   /**< \brief (SYSCTRL_INTFLAG reset_value) Interrupt Flag Status and Clear Register */

#define SYSCTRL_INTFLAG_XOSCRDY     (0x1u <<  0) /**< \brief (SYSCTRL_INTFLAG) XOSC Ready */
#define SYSCTRL_INTFLAG_XOSC32KRDY  (0x1u <<  1) /**< \brief (SYSCTRL_INTFLAG) XOSC32K Ready */
#define SYSCTRL_INTFLAG_BOD33RDY    (0x1u <<  2) /**< \brief (SYSCTRL_INTFLAG) BOD33 Ready */
#define SYSCTRL_INTFLAG_BOD12RDY    (0x1u <<  3) /**< \brief (SYSCTRL_INTFLAG) BOD12 Ready */
#define SYSCTRL_INTFLAG_BOD33DET    (0x1u <<  4) /**< \brief (SYSCTRL_INTFLAG) BOD33 Detection */
#define SYSCTRL_INTFLAG_BOD12DET    (0x1u <<  5) /**< \brief (SYSCTRL_INTFLAG) BOD12 Detection */
#define SYSCTRL_INTFLAG_BOD33BSY    (0x1u <<  6) /**< \brief (SYSCTRL_INTFLAG) BOD33 Synchronization Busy */
#define SYSCTRL_INTFLAG_BOD12BSY    (0x1u <<  7) /**< \brief (SYSCTRL_INTFLAG) BOD12 Synchronization Busy */
#define SYSCTRL_INTFLAG_DFLLRDY     (0x1u <<  8) /**< \brief (SYSCTRL_INTFLAG) DFLL Ready */
#define SYSCTRL_INTFLAG_DFLLOOB     (0x1u <<  9) /**< \brief (SYSCTRL_INTFLAG) DFLL Out Of Bounds */
#define SYSCTRL_INTFLAG_DFLLLCKF    (0x1u << 10) /**< \brief (SYSCTRL_INTFLAG) DFLL Lock Fine */
#define SYSCTRL_INTFLAG_DFLLLCKC    (0x1u << 11) /**< \brief (SYSCTRL_INTFLAG) DFLL Lock Coarse */
#define SYSCTRL_INTFLAG_DFLLRCS     (0x1u << 12) /**< \brief (SYSCTRL_INTFLAG) DFLL Reference Clock Stopped */
#define SYSCTRL_INTFLAG_OSC32KRDY   (0x1u << 13) /**< \brief (SYSCTRL_INTFLAG) OSC32K Ready */
#define SYSCTRL_INTFLAG_MASK        0x00003FFFu  /**< \brief (SYSCTRL_INTFLAG) MASK Register */

/* -------- SYSCTRL_PCLKSR : (SYSCTRL Offset: 0x0C) (R/  32) Power and Clocks Status Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t XOSCRDY:1;        /*!< bit:      0  XOSC Ready                         */
    uint32_t XOSC32KRDY:1;     /*!< bit:      1  XOSC32K Ready                      */
    uint32_t BOD33RDY:1;       /*!< bit:      2  BOD33 Ready                        */
    uint32_t BOD12RDY:1;       /*!< bit:      3  BOD12 Ready                        */
    uint32_t BOD33DET:1;       /*!< bit:      4  BOD33 Detection                    */
    uint32_t BOD12DET:1;       /*!< bit:      5  BOD12 Detection                    */
    uint32_t BOD33BSY:1;       /*!< bit:      6  BOD33 Synchronization Busy         */
    uint32_t BOD12BSY:1;       /*!< bit:      7  BOD12 Synchronization Busy         */
    uint32_t DFLLRDY:1;        /*!< bit:      8  DFLL Ready                         */
    uint32_t DFLLOOB:1;        /*!< bit:      9  DFLL Out Of Bounds                 */
    uint32_t DFLLLCKF:1;       /*!< bit:     10  DFLL Lock Fine                     */
    uint32_t DFLLLCKC:1;       /*!< bit:     11  DFLL Lock Coarse                   */
    uint32_t DFLLRCS:1;        /*!< bit:     12  DFLL Reference Clock Stopped       */
    uint32_t OSC32KRDY:1;      /*!< bit:     13  OSC32K Ready                       */
    uint32_t :18;              /*!< bit: 14..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_PCLKSR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_PCLKSR_OFFSET       0x0C         /**< \brief (SYSCTRL_PCLKSR offset) Power and Clocks Status Register */
#define SYSCTRL_PCLKSR_RESETVALUE   0x00000000   /**< \brief (SYSCTRL_PCLKSR reset_value) Power and Clocks Status Register */

#define SYSCTRL_PCLKSR_XOSCRDY      (0x1u <<  0) /**< \brief (SYSCTRL_PCLKSR) XOSC Ready */
#define SYSCTRL_PCLKSR_XOSC32KRDY   (0x1u <<  1) /**< \brief (SYSCTRL_PCLKSR) XOSC32K Ready */
#define SYSCTRL_PCLKSR_BOD33RDY     (0x1u <<  2) /**< \brief (SYSCTRL_PCLKSR) BOD33 Ready */
#define SYSCTRL_PCLKSR_BOD12RDY     (0x1u <<  3) /**< \brief (SYSCTRL_PCLKSR) BOD12 Ready */
#define SYSCTRL_PCLKSR_BOD33DET     (0x1u <<  4) /**< \brief (SYSCTRL_PCLKSR) BOD33 Detection */
#define SYSCTRL_PCLKSR_BOD12DET     (0x1u <<  5) /**< \brief (SYSCTRL_PCLKSR) BOD12 Detection */
#define SYSCTRL_PCLKSR_BOD33BSY     (0x1u <<  6) /**< \brief (SYSCTRL_PCLKSR) BOD33 Synchronization Busy */
#define SYSCTRL_PCLKSR_BOD12BSY     (0x1u <<  7) /**< \brief (SYSCTRL_PCLKSR) BOD12 Synchronization Busy */
#define SYSCTRL_PCLKSR_DFLLRDY      (0x1u <<  8) /**< \brief (SYSCTRL_PCLKSR) DFLL Ready */
#define SYSCTRL_PCLKSR_DFLLOOB      (0x1u <<  9) /**< \brief (SYSCTRL_PCLKSR) DFLL Out Of Bounds */
#define SYSCTRL_PCLKSR_DFLLLCKF     (0x1u << 10) /**< \brief (SYSCTRL_PCLKSR) DFLL Lock Fine */
#define SYSCTRL_PCLKSR_DFLLLCKC     (0x1u << 11) /**< \brief (SYSCTRL_PCLKSR) DFLL Lock Coarse */
#define SYSCTRL_PCLKSR_DFLLRCS      (0x1u << 12) /**< \brief (SYSCTRL_PCLKSR) DFLL Reference Clock Stopped */
#define SYSCTRL_PCLKSR_OSC32KRDY    (0x1u << 13) /**< \brief (SYSCTRL_PCLKSR) OSC32K Ready */
#define SYSCTRL_PCLKSR_MASK         0x00003FFFu  /**< \brief (SYSCTRL_PCLKSR) MASK Register */

/* -------- SYSCTRL_XOSC : (SYSCTRL Offset: 0x10) (R/W 16) XOSC Control Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t :1;               /*!< bit:      0  Reserved                           */
    uint16_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint16_t XTALEN:1;         /*!< bit:      2  Crystal Oscillator Enable          */
    uint16_t :5;               /*!< bit:  3.. 7  Reserved                           */
    uint16_t GAIN:3;           /*!< bit:  8..10  Gain Value                         */
    uint16_t AMPGC:1;          /*!< bit:     11  Automatic Amplitude Gain Control   */
    uint16_t STARTUP:4;        /*!< bit: 12..15  Start-Up Time                      */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_XOSC_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_XOSC_OFFSET         0x10         /**< \brief (SYSCTRL_XOSC offset) XOSC Control Register */
#define SYSCTRL_XOSC_RESETVALUE     0x0000       /**< \brief (SYSCTRL_XOSC reset_value) XOSC Control Register */

#define SYSCTRL_XOSC_ENABLE         (0x1u <<  1) /**< \brief (SYSCTRL_XOSC) Enable */
#define SYSCTRL_XOSC_XTALEN         (0x1u <<  2) /**< \brief (SYSCTRL_XOSC) Crystal Oscillator Enable */
#define SYSCTRL_XOSC_GAIN_Pos       8
#define SYSCTRL_XOSC_GAIN_Msk       (0x7u << SYSCTRL_XOSC_GAIN_Pos) /**< \brief (SYSCTRL_XOSC) Gain Value */
#define SYSCTRL_XOSC_GAIN(value)    ((SYSCTRL_XOSC_GAIN_Msk & ((value) << SYSCTRL_XOSC_GAIN_Pos)))
#define SYSCTRL_XOSC_AMPGC          (0x1u << 11) /**< \brief (SYSCTRL_XOSC) Automatic Amplitude Gain Control */
#define SYSCTRL_XOSC_STARTUP_Pos    12
#define SYSCTRL_XOSC_STARTUP_Msk    (0xFu << SYSCTRL_XOSC_STARTUP_Pos) /**< \brief (SYSCTRL_XOSC) Start-Up Time */
#define SYSCTRL_XOSC_STARTUP(value) ((SYSCTRL_XOSC_STARTUP_Msk & ((value) << SYSCTRL_XOSC_STARTUP_Pos)))
#define SYSCTRL_XOSC_MASK           0xFF06u      /**< \brief (SYSCTRL_XOSC) MASK Register */

/* -------- SYSCTRL_XOSC32K : (SYSCTRL Offset: 0x14) (R/W 16) XOSC32K Control Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t :1;               /*!< bit:      0  Reserved                           */
    uint16_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint16_t XTALEN:1;         /*!< bit:      2  Crystal Oscillator Enable          */
    uint16_t EN32K:1;          /*!< bit:      3  32kHz Output Enable                */
    uint16_t EN1K:1;           /*!< bit:      4  1kHz Output Enable                 */
    uint16_t AAMPEN:1;         /*!< bit:      5  Automatic Amplitude Control Enable */
    uint16_t :2;               /*!< bit:  6.. 7  Reserved                           */
    uint16_t STARTUP:3;        /*!< bit:  8..10  Start-Up Time                      */
    uint16_t :5;               /*!< bit: 11..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_XOSC32K_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_XOSC32K_OFFSET      0x14         /**< \brief (SYSCTRL_XOSC32K offset) XOSC32K Control Register */
#define SYSCTRL_XOSC32K_RESETVALUE  0x0000       /**< \brief (SYSCTRL_XOSC32K reset_value) XOSC32K Control Register */

#define SYSCTRL_XOSC32K_ENABLE      (0x1u <<  1) /**< \brief (SYSCTRL_XOSC32K) Enable */
#define SYSCTRL_XOSC32K_XTALEN      (0x1u <<  2) /**< \brief (SYSCTRL_XOSC32K) Crystal Oscillator Enable */
#define SYSCTRL_XOSC32K_EN32K       (0x1u <<  3) /**< \brief (SYSCTRL_XOSC32K) 32kHz Output Enable */
#define SYSCTRL_XOSC32K_EN1K        (0x1u <<  4) /**< \brief (SYSCTRL_XOSC32K) 1kHz Output Enable */
#define SYSCTRL_XOSC32K_AAMPEN      (0x1u <<  5) /**< \brief (SYSCTRL_XOSC32K) Automatic Amplitude Control Enable */
#define SYSCTRL_XOSC32K_STARTUP_Pos    8
#define SYSCTRL_XOSC32K_STARTUP_Msk    (0x7u << SYSCTRL_XOSC32K_STARTUP_Pos) /**< \brief (SYSCTRL_XOSC32K) Start-Up Time */
#define SYSCTRL_XOSC32K_STARTUP(value) ((SYSCTRL_XOSC32K_STARTUP_Msk & ((value) << SYSCTRL_XOSC32K_STARTUP_Pos)))
#define SYSCTRL_XOSC32K_MASK        0x073Eu      /**< \brief (SYSCTRL_XOSC32K) MASK Register */

/* -------- SYSCTRL_XOSC32KTEST : (SYSCTRL Offset: 0x16) (R/W  8) XOSC32K Test Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  MCALIBEN:1;       /*!< bit:      0  Manual Amplitude Calibration Enable */
    uint8_t  MCALIB:6;         /*!< bit:  1.. 6  Manual Amplitude Calibration Value */
    uint8_t  :1;               /*!< bit:      7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} SYSCTRL_XOSC32KTEST_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_XOSC32KTEST_OFFSET  0x16         /**< \brief (SYSCTRL_XOSC32KTEST offset) XOSC32K Test Register */
#define SYSCTRL_XOSC32KTEST_RESETVALUE 0x00         /**< \brief (SYSCTRL_XOSC32KTEST reset_value) XOSC32K Test Register */

#define SYSCTRL_XOSC32KTEST_MCALIBEN (0x1u <<  0) /**< \brief (SYSCTRL_XOSC32KTEST) Manual Amplitude Calibration Enable */
#define SYSCTRL_XOSC32KTEST_MCALIB_Pos    1
#define SYSCTRL_XOSC32KTEST_MCALIB_Msk    (0x3Fu << SYSCTRL_XOSC32KTEST_MCALIB_Pos) /**< \brief (SYSCTRL_XOSC32KTEST) Manual Amplitude Calibration Value */
#define SYSCTRL_XOSC32KTEST_MCALIB(value) ((SYSCTRL_XOSC32KTEST_MCALIB_Msk & ((value) << SYSCTRL_XOSC32KTEST_MCALIB_Pos)))
#define SYSCTRL_XOSC32KTEST_MASK    0x7Fu        /**< \brief (SYSCTRL_XOSC32KTEST) MASK Register */

/* -------- SYSCTRL_DFLLCTRL : (SYSCTRL Offset: 0x18) (R/W 16) DFLL Config Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t :1;               /*!< bit:      0  Reserved                           */
    uint16_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint16_t MODE:1;           /*!< bit:      2  Mode Selection                     */
    uint16_t STABLE:1;         /*!< bit:      3  Stable Frequency                   */
    uint16_t LLAW:1;           /*!< bit:      4  Lose Lock After Wake               */
    uint16_t USBCRM:1;         /*!< bit:      5  USB Clock Recovery Mode            */
    uint16_t CCDIS:1;          /*!< bit:      6  Chill Cycle Disable                */
    uint16_t QLDIS:1;          /*!< bit:      7  Quick Lock Disable                 */
    uint16_t :8;               /*!< bit:  8..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_DFLLCTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_DFLLCTRL_OFFSET     0x18         /**< \brief (SYSCTRL_DFLLCTRL offset) DFLL Config Register */
#define SYSCTRL_DFLLCTRL_RESETVALUE 0x0080       /**< \brief (SYSCTRL_DFLLCTRL reset_value) DFLL Config Register */

#define SYSCTRL_DFLLCTRL_ENABLE     (0x1u <<  1) /**< \brief (SYSCTRL_DFLLCTRL) Enable */
#define SYSCTRL_DFLLCTRL_MODE       (0x1u <<  2) /**< \brief (SYSCTRL_DFLLCTRL) Mode Selection */
#define SYSCTRL_DFLLCTRL_STABLE     (0x1u <<  3) /**< \brief (SYSCTRL_DFLLCTRL) Stable Frequency */
#define SYSCTRL_DFLLCTRL_LLAW       (0x1u <<  4) /**< \brief (SYSCTRL_DFLLCTRL) Lose Lock After Wake */
#define SYSCTRL_DFLLCTRL_USBCRM     (0x1u <<  5) /**< \brief (SYSCTRL_DFLLCTRL) USB Clock Recovery Mode */
#define SYSCTRL_DFLLCTRL_CCDIS      (0x1u <<  6) /**< \brief (SYSCTRL_DFLLCTRL) Chill Cycle Disable */
#define SYSCTRL_DFLLCTRL_QLDIS      (0x1u <<  7) /**< \brief (SYSCTRL_DFLLCTRL) Quick Lock Disable */
#define SYSCTRL_DFLLCTRL_MASK       0x00FEu      /**< \brief (SYSCTRL_DFLLCTRL) MASK Register */

/* -------- SYSCTRL_DFLLVAL : (SYSCTRL Offset: 0x1C) (R/W 32) DFLL Calibration Value Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t FINE:8;           /*!< bit:  0.. 7  Fine Calibration Value             */
    uint32_t COARSE:5;         /*!< bit:  8..12  Coarse Calibration Value           */
    uint32_t :3;               /*!< bit: 13..15  Reserved                           */
    uint32_t DIFF:16;          /*!< bit: 16..31  Multiplication Ratio Difference    */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_DFLLVAL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_DFLLVAL_OFFSET      0x1C         /**< \brief (SYSCTRL_DFLLVAL offset) DFLL Calibration Value Register */
#define SYSCTRL_DFLLVAL_RESETVALUE  0x00000000   /**< \brief (SYSCTRL_DFLLVAL reset_value) DFLL Calibration Value Register */

#define SYSCTRL_DFLLVAL_FINE_Pos    0
#define SYSCTRL_DFLLVAL_FINE_Msk    (0xFFu << SYSCTRL_DFLLVAL_FINE_Pos) /**< \brief (SYSCTRL_DFLLVAL) Fine Calibration Value */
#define SYSCTRL_DFLLVAL_FINE(value) ((SYSCTRL_DFLLVAL_FINE_Msk & ((value) << SYSCTRL_DFLLVAL_FINE_Pos)))
#define SYSCTRL_DFLLVAL_COARSE_Pos    8
#define SYSCTRL_DFLLVAL_COARSE_Msk    (0x1Fu << SYSCTRL_DFLLVAL_COARSE_Pos) /**< \brief (SYSCTRL_DFLLVAL) Coarse Calibration Value */
#define SYSCTRL_DFLLVAL_COARSE(value) ((SYSCTRL_DFLLVAL_COARSE_Msk & ((value) << SYSCTRL_DFLLVAL_COARSE_Pos)))
#define SYSCTRL_DFLLVAL_DIFF_Pos    16
#define SYSCTRL_DFLLVAL_DIFF_Msk    (0xFFFFu << SYSCTRL_DFLLVAL_DIFF_Pos) /**< \brief (SYSCTRL_DFLLVAL) Multiplication Ratio Difference */
#define SYSCTRL_DFLLVAL_DIFF(value) ((SYSCTRL_DFLLVAL_DIFF_Msk & ((value) << SYSCTRL_DFLLVAL_DIFF_Pos)))
#define SYSCTRL_DFLLVAL_MASK        0xFFFF1FFFu  /**< \brief (SYSCTRL_DFLLVAL) MASK Register */

/* -------- SYSCTRL_DFLLMUL : (SYSCTRL Offset: 0x20) (R/W 32) DFLL Multiplier Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t MUL:16;           /*!< bit:  0..15  Multiplication Value               */
    uint32_t FSTEP:8;          /*!< bit: 16..23  Maximum Fine Step Size             */
    uint32_t CSTEP:5;          /*!< bit: 24..28  Maximum Coarse Step Size           */
    uint32_t :3;               /*!< bit: 29..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_DFLLMUL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_DFLLMUL_OFFSET      0x20         /**< \brief (SYSCTRL_DFLLMUL offset) DFLL Multiplier Register */
#define SYSCTRL_DFLLMUL_RESETVALUE  0x00000000   /**< \brief (SYSCTRL_DFLLMUL reset_value) DFLL Multiplier Register */

#define SYSCTRL_DFLLMUL_MUL_Pos     0
#define SYSCTRL_DFLLMUL_MUL_Msk     (0xFFFFu << SYSCTRL_DFLLMUL_MUL_Pos) /**< \brief (SYSCTRL_DFLLMUL) Multiplication Value */
#define SYSCTRL_DFLLMUL_MUL(value)  ((SYSCTRL_DFLLMUL_MUL_Msk & ((value) << SYSCTRL_DFLLMUL_MUL_Pos)))
#define SYSCTRL_DFLLMUL_FSTEP_Pos    16
#define SYSCTRL_DFLLMUL_FSTEP_Msk    (0xFFu << SYSCTRL_DFLLMUL_FSTEP_Pos) /**< \brief (SYSCTRL_DFLLMUL) Maximum Fine Step Size */
#define SYSCTRL_DFLLMUL_FSTEP(value) ((SYSCTRL_DFLLMUL_FSTEP_Msk & ((value) << SYSCTRL_DFLLMUL_FSTEP_Pos)))
#define SYSCTRL_DFLLMUL_CSTEP_Pos    24
#define SYSCTRL_DFLLMUL_CSTEP_Msk    (0x1Fu << SYSCTRL_DFLLMUL_CSTEP_Pos) /**< \brief (SYSCTRL_DFLLMUL) Maximum Coarse Step Size */
#define SYSCTRL_DFLLMUL_CSTEP(value) ((SYSCTRL_DFLLMUL_CSTEP_Msk & ((value) << SYSCTRL_DFLLMUL_CSTEP_Pos)))
#define SYSCTRL_DFLLMUL_MASK        0x1FFFFFFFu  /**< \brief (SYSCTRL_DFLLMUL) MASK Register */

/* -------- SYSCTRL_DFLLSYNC : (SYSCTRL Offset: 0x24) (R/W  8) DFLL Synchronization Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  :7;               /*!< bit:  0.. 6  Reserved                           */
    uint8_t  READREQ:1;        /*!< bit:      7  Read Request Synchronization       */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} SYSCTRL_DFLLSYNC_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_DFLLSYNC_OFFSET     0x24         /**< \brief (SYSCTRL_DFLLSYNC offset) DFLL Synchronization Register */
#define SYSCTRL_DFLLSYNC_RESETVALUE 0x00         /**< \brief (SYSCTRL_DFLLSYNC reset_value) DFLL Synchronization Register */

#define SYSCTRL_DFLLSYNC_READREQ    (0x1u <<  7) /**< \brief (SYSCTRL_DFLLSYNC) Read Request Synchronization */
#define SYSCTRL_DFLLSYNC_MASK       0x80u        /**< \brief (SYSCTRL_DFLLSYNC) MASK Register */

/* -------- SYSCTRL_BOD33 : (SYSCTRL Offset: 0x28) (R/W 32) BOD33 Control Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t :1;               /*!< bit:      0  Reserved                           */
    uint32_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint32_t HYST:1;           /*!< bit:      2  Hysteresis Enable                  */
    uint32_t ACTION:2;         /*!< bit:  3.. 4  Action when Threshold Crossed      */
    uint32_t :3;               /*!< bit:  5.. 7  Reserved                           */
    uint32_t MODE:1;           /*!< bit:      8  Operation Modes                    */
    uint32_t CEN:1;            /*!< bit:      9  Clock Enable                       */
    uint32_t :2;               /*!< bit: 10..11  Reserved                           */
    uint32_t PSEL:4;           /*!< bit: 12..15  Prescaler Select                   */
    uint32_t LEVEL:6;          /*!< bit: 16..21  Threshold Level                    */
    uint32_t :10;              /*!< bit: 22..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_BOD33_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_BOD33_OFFSET        0x28         /**< \brief (SYSCTRL_BOD33 offset) BOD33 Control Register */

#define SYSCTRL_BOD33_ENABLE        (0x1u <<  1) /**< \brief (SYSCTRL_BOD33) Enable */
#define SYSCTRL_BOD33_HYST          (0x1u <<  2) /**< \brief (SYSCTRL_BOD33) Hysteresis Enable */
#define SYSCTRL_BOD33_ACTION_Pos    3
#define SYSCTRL_BOD33_ACTION_Msk    (0x3u << SYSCTRL_BOD33_ACTION_Pos) /**< \brief (SYSCTRL_BOD33) Action when Threshold Crossed */
#define SYSCTRL_BOD33_ACTION(value) ((SYSCTRL_BOD33_ACTION_Msk & ((value) << SYSCTRL_BOD33_ACTION_Pos)))
#define SYSCTRL_BOD33_MODE          (0x1u <<  8) /**< \brief (SYSCTRL_BOD33) Operation Modes */
#define SYSCTRL_BOD33_CEN           (0x1u <<  9) /**< \brief (SYSCTRL_BOD33) Clock Enable */
#define SYSCTRL_BOD33_PSEL_Pos      12
#define SYSCTRL_BOD33_PSEL_Msk      (0xFu << SYSCTRL_BOD33_PSEL_Pos) /**< \brief (SYSCTRL_BOD33) Prescaler Select */
#define SYSCTRL_BOD33_PSEL(value)   ((SYSCTRL_BOD33_PSEL_Msk & ((value) << SYSCTRL_BOD33_PSEL_Pos)))
#define SYSCTRL_BOD33_LEVEL_Pos     16
#define SYSCTRL_BOD33_LEVEL_Msk     (0x3Fu << SYSCTRL_BOD33_LEVEL_Pos) /**< \brief (SYSCTRL_BOD33) Threshold Level */
#define SYSCTRL_BOD33_LEVEL(value)  ((SYSCTRL_BOD33_LEVEL_Msk & ((value) << SYSCTRL_BOD33_LEVEL_Pos)))
#define SYSCTRL_BOD33_MASK          0x003FF31Eu  /**< \brief (SYSCTRL_BOD33) MASK Register */

/* -------- SYSCTRL_BOD12 : (SYSCTRL Offset: 0x2C) (R/W 32) BOD12 Control Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t :1;               /*!< bit:      0  Reserved                           */
    uint32_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint32_t HYST:1;           /*!< bit:      2  Hysteresis Enable                  */
    uint32_t ACTION:2;         /*!< bit:  3.. 4  Action when Threshold Crossed      */
    uint32_t :3;               /*!< bit:  5.. 7  Reserved                           */
    uint32_t MODE:1;           /*!< bit:      8  Operation Modes                    */
    uint32_t CEN:1;            /*!< bit:      9  Clock Enable                       */
    uint32_t :2;               /*!< bit: 10..11  Reserved                           */
    uint32_t PSEL:4;           /*!< bit: 12..15  Prescaler Select                   */
    uint32_t LEVEL:5;          /*!< bit: 16..20  Threshold Level                    */
    uint32_t :11;              /*!< bit: 21..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_BOD12_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_BOD12_OFFSET        0x2C         /**< \brief (SYSCTRL_BOD12 offset) BOD12 Control Register */

#define SYSCTRL_BOD12_ENABLE        (0x1u <<  1) /**< \brief (SYSCTRL_BOD12) Enable */
#define SYSCTRL_BOD12_HYST          (0x1u <<  2) /**< \brief (SYSCTRL_BOD12) Hysteresis Enable */
#define SYSCTRL_BOD12_ACTION_Pos    3
#define SYSCTRL_BOD12_ACTION_Msk    (0x3u << SYSCTRL_BOD12_ACTION_Pos) /**< \brief (SYSCTRL_BOD12) Action when Threshold Crossed */
#define SYSCTRL_BOD12_ACTION(value) ((SYSCTRL_BOD12_ACTION_Msk & ((value) << SYSCTRL_BOD12_ACTION_Pos)))
#define SYSCTRL_BOD12_MODE          (0x1u <<  8) /**< \brief (SYSCTRL_BOD12) Operation Modes */
#define SYSCTRL_BOD12_CEN           (0x1u <<  9) /**< \brief (SYSCTRL_BOD12) Clock Enable */
#define SYSCTRL_BOD12_PSEL_Pos      12
#define SYSCTRL_BOD12_PSEL_Msk      (0xFu << SYSCTRL_BOD12_PSEL_Pos) /**< \brief (SYSCTRL_BOD12) Prescaler Select */
#define SYSCTRL_BOD12_PSEL(value)   ((SYSCTRL_BOD12_PSEL_Msk & ((value) << SYSCTRL_BOD12_PSEL_Pos)))
#define SYSCTRL_BOD12_LEVEL_Pos     16
#define SYSCTRL_BOD12_LEVEL_Msk     (0x1Fu << SYSCTRL_BOD12_LEVEL_Pos) /**< \brief (SYSCTRL_BOD12) Threshold Level */
#define SYSCTRL_BOD12_LEVEL(value)  ((SYSCTRL_BOD12_LEVEL_Msk & ((value) << SYSCTRL_BOD12_LEVEL_Pos)))
#define SYSCTRL_BOD12_MASK          0x001FF31Eu  /**< \brief (SYSCTRL_BOD12) MASK Register */

/* -------- SYSCTRL_OSC32K : (SYSCTRL Offset: 0x30) (R/W 32) OSC32K Control Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t :1;               /*!< bit:      0  Reserved                           */
    uint32_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint32_t EN32K:1;          /*!< bit:      2  32kHz Output Enable                */
    uint32_t EN1K:1;           /*!< bit:      3  1kHz Output Enable                 */
    uint32_t :4;               /*!< bit:  4.. 7  Reserved                           */
    uint32_t STARTUP:3;        /*!< bit:  8..10  Start-Up Time                      */
    uint32_t :5;               /*!< bit: 11..15  Reserved                           */
    uint32_t CALIB:7;          /*!< bit: 16..22  Calibration Value                  */
    uint32_t :9;               /*!< bit: 23..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_OSC32K_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_OSC32K_OFFSET       0x30         /**< \brief (SYSCTRL_OSC32K offset) OSC32K Control Register */
#define SYSCTRL_OSC32K_RESETVALUE   0x003F0000   /**< \brief (SYSCTRL_OSC32K reset_value) OSC32K Control Register */

#define SYSCTRL_OSC32K_ENABLE       (0x1u <<  1) /**< \brief (SYSCTRL_OSC32K) Enable */
#define SYSCTRL_OSC32K_EN32K        (0x1u <<  2) /**< \brief (SYSCTRL_OSC32K) 32kHz Output Enable */
#define SYSCTRL_OSC32K_EN1K         (0x1u <<  3) /**< \brief (SYSCTRL_OSC32K) 1kHz Output Enable */
#define SYSCTRL_OSC32K_STARTUP_Pos    8
#define SYSCTRL_OSC32K_STARTUP_Msk    (0x7u << SYSCTRL_OSC32K_STARTUP_Pos) /**< \brief (SYSCTRL_OSC32K) Start-Up Time */
#define SYSCTRL_OSC32K_STARTUP(value) ((SYSCTRL_OSC32K_STARTUP_Msk & ((value) << SYSCTRL_OSC32K_STARTUP_Pos)))
#define SYSCTRL_OSC32K_CALIB_Pos    16
#define SYSCTRL_OSC32K_CALIB_Msk    (0x7Fu << SYSCTRL_OSC32K_CALIB_Pos) /**< \brief (SYSCTRL_OSC32K) Calibration Value */
#define SYSCTRL_OSC32K_CALIB(value) ((SYSCTRL_OSC32K_CALIB_Msk & ((value) << SYSCTRL_OSC32K_CALIB_Pos)))
#define SYSCTRL_OSC32K_MASK         0x007F070Eu  /**< \brief (SYSCTRL_OSC32K) MASK Register */

/* -------- SYSCTRL_OSCULP32K : (SYSCTRL Offset: 0x34) (R/W  8) OSCULP32K Control Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  CALIB:5;          /*!< bit:  0.. 4  Calibration Value                  */
    uint8_t  :3;               /*!< bit:  5.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} SYSCTRL_OSCULP32K_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_OSCULP32K_OFFSET    0x34         /**< \brief (SYSCTRL_OSCULP32K offset) OSCULP32K Control Register */
#define SYSCTRL_OSCULP32K_RESETVALUE 0x0F         /**< \brief (SYSCTRL_OSCULP32K reset_value) OSCULP32K Control Register */

#define SYSCTRL_OSCULP32K_CALIB_Pos    0
#define SYSCTRL_OSCULP32K_CALIB_Msk    (0x1Fu << SYSCTRL_OSCULP32K_CALIB_Pos) /**< \brief (SYSCTRL_OSCULP32K) Calibration Value */
#define SYSCTRL_OSCULP32K_CALIB(value) ((SYSCTRL_OSCULP32K_CALIB_Msk & ((value) << SYSCTRL_OSCULP32K_CALIB_Pos)))
#define SYSCTRL_OSCULP32K_MASK      0x1Fu        /**< \brief (SYSCTRL_OSCULP32K) MASK Register */

/* -------- SYSCTRL_OSCULP32KTEST : (SYSCTRL Offset: 0x38) (R/W  8) OSCULP32K Test Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  DISABLE:1;        /*!< bit:      0  Disable Test Mode                  */
    uint8_t  BIASOUTEN:1;      /*!< bit:      1  Bias Output Enable                 */
    uint8_t  CONTREF:1;        /*!< bit:      2  Reference Forced to Continuous Mode */
    uint8_t  :5;               /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} SYSCTRL_OSCULP32KTEST_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_OSCULP32KTEST_OFFSET 0x38         /**< \brief (SYSCTRL_OSCULP32KTEST offset) OSCULP32K Test Register */
#define SYSCTRL_OSCULP32KTEST_RESETVALUE 0x00         /**< \brief (SYSCTRL_OSCULP32KTEST reset_value) OSCULP32K Test Register */

#define SYSCTRL_OSCULP32KTEST_DISABLE (0x1u <<  0) /**< \brief (SYSCTRL_OSCULP32KTEST) Disable Test Mode */
#define SYSCTRL_OSCULP32KTEST_BIASOUTEN (0x1u <<  1) /**< \brief (SYSCTRL_OSCULP32KTEST) Bias Output Enable */
#define SYSCTRL_OSCULP32KTEST_CONTREF (0x1u <<  2) /**< \brief (SYSCTRL_OSCULP32KTEST) Reference Forced to Continuous Mode */
#define SYSCTRL_OSCULP32KTEST_MASK  0x07u        /**< \brief (SYSCTRL_OSCULP32KTEST) MASK Register */

/* -------- SYSCTRL_OSC8M : (SYSCTRL Offset: 0x3C) (R/W 32) OSC8M Control Register A -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t :1;               /*!< bit:      0  Reserved                           */
    uint32_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint32_t :6;               /*!< bit:  2.. 7  Reserved                           */
    uint32_t PRESC:2;          /*!< bit:  8.. 9  Prescaler Select                   */
    uint32_t :6;               /*!< bit: 10..15  Reserved                           */
    uint32_t CALIB:12;         /*!< bit: 16..27  Calibration Value                  */
    uint32_t :2;               /*!< bit: 28..29  Reserved                           */
    uint32_t FRANGE:2;         /*!< bit: 30..31  Frequency Range                    */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_OSC8M_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_OSC8M_OFFSET        0x3C         /**< \brief (SYSCTRL_OSC8M offset) OSC8M Control Register A */
#define SYSCTRL_OSC8M_RESETVALUE    0x00000080   /**< \brief (SYSCTRL_OSC8M reset_value) OSC8M Control Register A */

#define SYSCTRL_OSC8M_ENABLE        (0x1u <<  1) /**< \brief (SYSCTRL_OSC8M) Enable */
#define SYSCTRL_OSC8M_PRESC_Pos     8
#define SYSCTRL_OSC8M_PRESC_Msk     (0x3u << SYSCTRL_OSC8M_PRESC_Pos) /**< \brief (SYSCTRL_OSC8M) Prescaler Select */
#define SYSCTRL_OSC8M_PRESC(value)  ((SYSCTRL_OSC8M_PRESC_Msk & ((value) << SYSCTRL_OSC8M_PRESC_Pos)))
#define SYSCTRL_OSC8M_CALIB_Pos     16
#define SYSCTRL_OSC8M_CALIB_Msk     (0xFFFu << SYSCTRL_OSC8M_CALIB_Pos) /**< \brief (SYSCTRL_OSC8M) Calibration Value */
#define SYSCTRL_OSC8M_CALIB(value)  ((SYSCTRL_OSC8M_CALIB_Msk & ((value) << SYSCTRL_OSC8M_CALIB_Pos)))
#define SYSCTRL_OSC8M_FRANGE_Pos    30
#define SYSCTRL_OSC8M_FRANGE_Msk    (0x3u << SYSCTRL_OSC8M_FRANGE_Pos) /**< \brief (SYSCTRL_OSC8M) Frequency Range */
#define SYSCTRL_OSC8M_FRANGE(value) ((SYSCTRL_OSC8M_FRANGE_Msk & ((value) << SYSCTRL_OSC8M_FRANGE_Pos)))
#define SYSCTRL_OSC8M_MASK          0xCFFF0302u  /**< \brief (SYSCTRL_OSC8M) MASK Register */

/* -------- SYSCTRL_VREG : (SYSCTRL Offset: 0x40) (R/W 16) VREG Control Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t :1;               /*!< bit:      0  Reserved                           */
    uint16_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint16_t :6;               /*!< bit:  2.. 7  Reserved                           */
    uint16_t LEVEL:3;          /*!< bit:  8..10  Output Voltage Level               */
    uint16_t :1;               /*!< bit:     11  Reserved                           */
    uint16_t CALIB:4;          /*!< bit: 12..15  Calibration Value                  */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_VREG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_VREG_OFFSET         0x40         /**< \brief (SYSCTRL_VREG offset) VREG Control Register */
#define SYSCTRL_VREG_RESETVALUE     0x0000       /**< \brief (SYSCTRL_VREG reset_value) VREG Control Register */

#define SYSCTRL_VREG_ENABLE         (0x1u <<  1) /**< \brief (SYSCTRL_VREG) Enable */
#define SYSCTRL_VREG_LEVEL_Pos      8
#define SYSCTRL_VREG_LEVEL_Msk      (0x7u << SYSCTRL_VREG_LEVEL_Pos) /**< \brief (SYSCTRL_VREG) Output Voltage Level */
#define SYSCTRL_VREG_LEVEL(value)   ((SYSCTRL_VREG_LEVEL_Msk & ((value) << SYSCTRL_VREG_LEVEL_Pos)))
#define SYSCTRL_VREG_CALIB_Pos      12
#define SYSCTRL_VREG_CALIB_Msk      (0xFu << SYSCTRL_VREG_CALIB_Pos) /**< \brief (SYSCTRL_VREG) Calibration Value */
#define SYSCTRL_VREG_CALIB(value)   ((SYSCTRL_VREG_CALIB_Msk & ((value) << SYSCTRL_VREG_CALIB_Pos)))
#define SYSCTRL_VREG_MASK           0xF702u      /**< \brief (SYSCTRL_VREG) MASK Register */

/* -------- SYSCTRL_VREGTEST : (SYSCTRL Offset: 0x44) (R/W 32) VREG Test Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t ULPENABLE:1;      /*!< bit:      0  Force ULP VREG Enable              */
    uint32_t BODIENABLE:1;     /*!< bit:      1  ULP VREG BOD I Enable              */
    uint32_t BODIIENABLE:1;    /*!< bit:      2  ULP VREG BOD II Enable             */
    uint32_t OSCENABLE:1;      /*!< bit:      3  ULP VREG Oscillator Enable         */
    uint32_t LDOENABLE:1;      /*!< bit:      4  ULP VREG LDO Enable                */
    uint32_t REFSEL:1;         /*!< bit:      5  Voltage Reference Select           */
    uint32_t :2;               /*!< bit:  6.. 7  Reserved                           */
    uint32_t FORCEGAIN:2;      /*!< bit:  8.. 9  ULP VREG External Gain Mode        */
    uint32_t RAMREFSEL:3;      /*!< bit: 10..12  RAM Reference Voltage Select       */
    uint32_t :3;               /*!< bit: 13..15  Reserved                           */
    uint32_t REQBIAS:1;        /*!< bit:     16  Force ULP VREG Bias Generator      */
    uint32_t REQHELPI:1;       /*!< bit:     17  ULP VREG BOD I Output              */
    uint32_t REQHELPII:1;      /*!< bit:     18  ULP VREG BOD II Output             */
    uint32_t :12;              /*!< bit: 19..30  Reserved                           */
    uint32_t TESTEN:1;         /*!< bit:     31  Test Enable                        */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_VREGTEST_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_VREGTEST_OFFSET     0x44         /**< \brief (SYSCTRL_VREGTEST offset) VREG Test Register */
#define SYSCTRL_VREGTEST_RESETVALUE 0x00000000   /**< \brief (SYSCTRL_VREGTEST reset_value) VREG Test Register */

#define SYSCTRL_VREGTEST_ULPENABLE  (0x1u <<  0) /**< \brief (SYSCTRL_VREGTEST) Force ULP VREG Enable */
#define SYSCTRL_VREGTEST_BODIENABLE (0x1u <<  1) /**< \brief (SYSCTRL_VREGTEST) ULP VREG BOD I Enable */
#define SYSCTRL_VREGTEST_BODIIENABLE (0x1u <<  2) /**< \brief (SYSCTRL_VREGTEST) ULP VREG BOD II Enable */
#define SYSCTRL_VREGTEST_OSCENABLE  (0x1u <<  3) /**< \brief (SYSCTRL_VREGTEST) ULP VREG Oscillator Enable */
#define SYSCTRL_VREGTEST_LDOENABLE  (0x1u <<  4) /**< \brief (SYSCTRL_VREGTEST) ULP VREG LDO Enable */
#define SYSCTRL_VREGTEST_REFSEL     (0x1u <<  5) /**< \brief (SYSCTRL_VREGTEST) Voltage Reference Select */
#define SYSCTRL_VREGTEST_FORCEGAIN_Pos    8
#define SYSCTRL_VREGTEST_FORCEGAIN_Msk    (0x3u << SYSCTRL_VREGTEST_FORCEGAIN_Pos) /**< \brief (SYSCTRL_VREGTEST) ULP VREG External Gain Mode */
#define SYSCTRL_VREGTEST_FORCEGAIN(value) ((SYSCTRL_VREGTEST_FORCEGAIN_Msk & ((value) << SYSCTRL_VREGTEST_FORCEGAIN_Pos)))
#define SYSCTRL_VREGTEST_RAMREFSEL_Pos    10
#define SYSCTRL_VREGTEST_RAMREFSEL_Msk    (0x7u << SYSCTRL_VREGTEST_RAMREFSEL_Pos) /**< \brief (SYSCTRL_VREGTEST) RAM Reference Voltage Select */
#define SYSCTRL_VREGTEST_RAMREFSEL(value) ((SYSCTRL_VREGTEST_RAMREFSEL_Msk & ((value) << SYSCTRL_VREGTEST_RAMREFSEL_Pos)))
#define SYSCTRL_VREGTEST_REQBIAS    (0x1u << 16) /**< \brief (SYSCTRL_VREGTEST) Force ULP VREG Bias Generator */
#define SYSCTRL_VREGTEST_REQHELPI   (0x1u << 17) /**< \brief (SYSCTRL_VREGTEST) ULP VREG BOD I Output */
#define SYSCTRL_VREGTEST_REQHELPII  (0x1u << 18) /**< \brief (SYSCTRL_VREGTEST) ULP VREG BOD II Output */
#define SYSCTRL_VREGTEST_TESTEN     (0x1u << 31) /**< \brief (SYSCTRL_VREGTEST) Test Enable */
#define SYSCTRL_VREGTEST_MASK       0x80071F3Fu  /**< \brief (SYSCTRL_VREGTEST) MASK Register */

/* -------- SYSCTRL_VREF : (SYSCTRL Offset: 0x48) (R/W 32) VREF Control Register A -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t :1;               /*!< bit:      0  Reserved                           */
    uint32_t TSEN:1;           /*!< bit:      1  Temperature Sensor Output Enable   */
    uint32_t BGOUTEN:1;        /*!< bit:      2  Bandgap Output Enable              */
    uint32_t :13;              /*!< bit:  3..15  Reserved                           */
    uint32_t CALIB:11;         /*!< bit: 16..26  Voltage Reference Calibration Value */
    uint32_t :5;               /*!< bit: 27..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_VREF_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_VREF_OFFSET         0x48         /**< \brief (SYSCTRL_VREF offset) VREF Control Register A */
#define SYSCTRL_VREF_RESETVALUE     0x00000000   /**< \brief (SYSCTRL_VREF reset_value) VREF Control Register A */

#define SYSCTRL_VREF_TSEN           (0x1u <<  1) /**< \brief (SYSCTRL_VREF) Temperature Sensor Output Enable */
#define SYSCTRL_VREF_BGOUTEN        (0x1u <<  2) /**< \brief (SYSCTRL_VREF) Bandgap Output Enable */
#define SYSCTRL_VREF_CALIB_Pos      16
#define SYSCTRL_VREF_CALIB_Msk      (0x7FFu << SYSCTRL_VREF_CALIB_Pos) /**< \brief (SYSCTRL_VREF) Voltage Reference Calibration Value */
#define SYSCTRL_VREF_CALIB(value)   ((SYSCTRL_VREF_CALIB_Msk & ((value) << SYSCTRL_VREF_CALIB_Pos)))
#define SYSCTRL_VREF_MASK           0x07FF0006u  /**< \brief (SYSCTRL_VREF) MASK Register */

/* -------- SYSCTRL_VREFTESTA : (SYSCTRL Offset: 0x4C) (R/W 32) VREF Test Register A -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t BIASEN:1;         /*!< bit:      0  Bias Current Generator Enable      */
    uint32_t BGEN:1;           /*!< bit:      1  Bandgap Voltage Reference Enable   */
    uint32_t :6;               /*!< bit:  2.. 7  Reserved                           */
    uint32_t BGBUFEN:8;        /*!< bit:  8..15  Bandgap Buffers Enable             */
    uint32_t BGRDY:1;          /*!< bit:     16  Bandgap Voltage Reference Ready    */
    uint32_t BIASRDY:1;        /*!< bit:     17  PTAT Bias Ready                    */
    uint32_t ACBUFRDY:1;       /*!< bit:     18  AC Bandgap Buffer Ready            */
    uint32_t ADCBUFRDY:1;      /*!< bit:     19  ADC Bandgap Buffer Ready           */
    uint32_t DACBUFRDY:1;      /*!< bit:     20  DAC Bandgap Buffer Ready           */
    uint32_t VDOUBBUFRDY:1;    /*!< bit:     21  Voltage Doubler Bandgap Buffer Ready */
    uint32_t BUFRRRDY:1;       /*!< bit:     22  Rail-to-Rail Buffer Ready          */
    uint32_t DIV2BUFRDY:1;     /*!< bit:     23  Divide-by-2 Buffer Ready           */
    uint32_t BUFRRSEL:2;       /*!< bit: 24..25  Rail-to-Rail Buffer Input Select   */
    uint32_t :5;               /*!< bit: 26..30  Reserved                           */
    uint32_t TESTEN:1;         /*!< bit:     31  Test Enable                        */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_VREFTESTA_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_VREFTESTA_OFFSET    0x4C         /**< \brief (SYSCTRL_VREFTESTA offset) VREF Test Register A */
#define SYSCTRL_VREFTESTA_RESETVALUE 0x00000000   /**< \brief (SYSCTRL_VREFTESTA reset_value) VREF Test Register A */

#define SYSCTRL_VREFTESTA_BIASEN    (0x1u <<  0) /**< \brief (SYSCTRL_VREFTESTA) Bias Current Generator Enable */
#define SYSCTRL_VREFTESTA_BGEN      (0x1u <<  1) /**< \brief (SYSCTRL_VREFTESTA) Bandgap Voltage Reference Enable */
#define SYSCTRL_VREFTESTA_BGBUFEN_Pos    8
#define SYSCTRL_VREFTESTA_BGBUFEN_Msk    (0xFFu << SYSCTRL_VREFTESTA_BGBUFEN_Pos) /**< \brief (SYSCTRL_VREFTESTA) Bandgap Buffers Enable */
#define SYSCTRL_VREFTESTA_BGBUFEN(value) ((SYSCTRL_VREFTESTA_BGBUFEN_Msk & ((value) << SYSCTRL_VREFTESTA_BGBUFEN_Pos)))
#define SYSCTRL_VREFTESTA_BGRDY     (0x1u << 16) /**< \brief (SYSCTRL_VREFTESTA) Bandgap Voltage Reference Ready */
#define SYSCTRL_VREFTESTA_BIASRDY   (0x1u << 17) /**< \brief (SYSCTRL_VREFTESTA) PTAT Bias Ready */
#define SYSCTRL_VREFTESTA_ACBUFRDY  (0x1u << 18) /**< \brief (SYSCTRL_VREFTESTA) AC Bandgap Buffer Ready */
#define SYSCTRL_VREFTESTA_ADCBUFRDY (0x1u << 19) /**< \brief (SYSCTRL_VREFTESTA) ADC Bandgap Buffer Ready */
#define SYSCTRL_VREFTESTA_DACBUFRDY (0x1u << 20) /**< \brief (SYSCTRL_VREFTESTA) DAC Bandgap Buffer Ready */
#define SYSCTRL_VREFTESTA_VDOUBBUFRDY (0x1u << 21) /**< \brief (SYSCTRL_VREFTESTA) Voltage Doubler Bandgap Buffer Ready */
#define SYSCTRL_VREFTESTA_BUFRRRDY  (0x1u << 22) /**< \brief (SYSCTRL_VREFTESTA) Rail-to-Rail Buffer Ready */
#define SYSCTRL_VREFTESTA_DIV2BUFRDY (0x1u << 23) /**< \brief (SYSCTRL_VREFTESTA) Divide-by-2 Buffer Ready */
#define SYSCTRL_VREFTESTA_BUFRRSEL_Pos    24
#define SYSCTRL_VREFTESTA_BUFRRSEL_Msk    (0x3u << SYSCTRL_VREFTESTA_BUFRRSEL_Pos) /**< \brief (SYSCTRL_VREFTESTA) Rail-to-Rail Buffer Input Select */
#define SYSCTRL_VREFTESTA_BUFRRSEL(value) ((SYSCTRL_VREFTESTA_BUFRRSEL_Msk & ((value) << SYSCTRL_VREFTESTA_BUFRRSEL_Pos)))
#define SYSCTRL_VREFTESTA_TESTEN    (0x1u << 31) /**< \brief (SYSCTRL_VREFTESTA) Test Enable */
#define SYSCTRL_VREFTESTA_MASK      0x83FFFF03u  /**< \brief (SYSCTRL_VREFTESTA) MASK Register */

/* -------- SYSCTRL_VREFTESTB : (SYSCTRL Offset: 0x50) (R/W 16) VREF Test Register B -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t SAMPLECLK:2;      /*!< bit:  0.. 1  Sample Clock Division Factor       */
    uint16_t DIVLEV0:3;        /*!< bit:  2.. 4  ULP VREG Reference Division Level  */
    uint16_t DIVLEV1:3;        /*!< bit:  5.. 7  ULP VREG Gain Reference Division Level */
    uint16_t DIVLEV2:3;        /*!< bit:  8..10  XOSC Reference Division Level      */
    uint16_t DIVLEV3:3;        /*!< bit: 11..13  VREG Reference Division Level      */
    uint16_t :1;               /*!< bit:     14  Reserved                           */
    uint16_t TESTEN:1;         /*!< bit:     15  Test Enable                        */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} SYSCTRL_VREFTESTB_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_VREFTESTB_OFFSET    0x50         /**< \brief (SYSCTRL_VREFTESTB offset) VREF Test Register B */

#define SYSCTRL_VREFTESTB_SAMPLECLK_Pos    0
#define SYSCTRL_VREFTESTB_SAMPLECLK_Msk    (0x3u << SYSCTRL_VREFTESTB_SAMPLECLK_Pos) /**< \brief (SYSCTRL_VREFTESTB) Sample Clock Division Factor */
#define SYSCTRL_VREFTESTB_SAMPLECLK(value) ((SYSCTRL_VREFTESTB_SAMPLECLK_Msk & ((value) << SYSCTRL_VREFTESTB_SAMPLECLK_Pos)))
#define SYSCTRL_VREFTESTB_DIVLEV0_Pos    2
#define SYSCTRL_VREFTESTB_DIVLEV0_Msk    (0x7u << SYSCTRL_VREFTESTB_DIVLEV0_Pos) /**< \brief (SYSCTRL_VREFTESTB) ULP VREG Reference Division Level */
#define SYSCTRL_VREFTESTB_DIVLEV0(value) ((SYSCTRL_VREFTESTB_DIVLEV0_Msk & ((value) << SYSCTRL_VREFTESTB_DIVLEV0_Pos)))
#define SYSCTRL_VREFTESTB_DIVLEV1_Pos    5
#define SYSCTRL_VREFTESTB_DIVLEV1_Msk    (0x7u << SYSCTRL_VREFTESTB_DIVLEV1_Pos) /**< \brief (SYSCTRL_VREFTESTB) ULP VREG Gain Reference Division Level */
#define SYSCTRL_VREFTESTB_DIVLEV1(value) ((SYSCTRL_VREFTESTB_DIVLEV1_Msk & ((value) << SYSCTRL_VREFTESTB_DIVLEV1_Pos)))
#define SYSCTRL_VREFTESTB_DIVLEV2_Pos    8
#define SYSCTRL_VREFTESTB_DIVLEV2_Msk    (0x7u << SYSCTRL_VREFTESTB_DIVLEV2_Pos) /**< \brief (SYSCTRL_VREFTESTB) XOSC Reference Division Level */
#define SYSCTRL_VREFTESTB_DIVLEV2(value) ((SYSCTRL_VREFTESTB_DIVLEV2_Msk & ((value) << SYSCTRL_VREFTESTB_DIVLEV2_Pos)))
#define SYSCTRL_VREFTESTB_DIVLEV3_Pos    11
#define SYSCTRL_VREFTESTB_DIVLEV3_Msk    (0x7u << SYSCTRL_VREFTESTB_DIVLEV3_Pos) /**< \brief (SYSCTRL_VREFTESTB) VREG Reference Division Level */
#define SYSCTRL_VREFTESTB_DIVLEV3(value) ((SYSCTRL_VREFTESTB_DIVLEV3_Msk & ((value) << SYSCTRL_VREFTESTB_DIVLEV3_Pos)))
#define SYSCTRL_VREFTESTB_TESTEN    (0x1u << 15) /**< \brief (SYSCTRL_VREFTESTB) Test Enable */
#define SYSCTRL_VREFTESTB_MASK      0xBFFFu      /**< \brief (SYSCTRL_VREFTESTB) MASK Register */

/* -------- SYSCTRL_FORCECLKON : (SYSCTRL Offset: 0x54) (R/W 32) Force Clock On Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t XOSCON:1;         /*!< bit:      0                                     */
    uint32_t XOSC32KON:1;      /*!< bit:      1                                     */
    uint32_t DFLLON:1;         /*!< bit:      2                                     */
    uint32_t OSC32KON:1;       /*!< bit:      3                                     */
    uint32_t OSC8MON:1;        /*!< bit:      4                                     */
    uint32_t :27;              /*!< bit:  5..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_FORCECLKON_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_FORCECLKON_OFFSET   0x54         /**< \brief (SYSCTRL_FORCECLKON offset) Force Clock On Register */

#define SYSCTRL_FORCECLKON_XOSCON   (0x1u <<  0) /**< \brief (SYSCTRL_FORCECLKON)  */
#define SYSCTRL_FORCECLKON_XOSC32KON (0x1u <<  1) /**< \brief (SYSCTRL_FORCECLKON)  */
#define SYSCTRL_FORCECLKON_DFLLON   (0x1u <<  2) /**< \brief (SYSCTRL_FORCECLKON)  */
#define SYSCTRL_FORCECLKON_OSC32KON (0x1u <<  3) /**< \brief (SYSCTRL_FORCECLKON)  */
#define SYSCTRL_FORCECLKON_OSC8MON  (0x1u <<  4) /**< \brief (SYSCTRL_FORCECLKON)  */
#define SYSCTRL_FORCECLKON_MASK     0x0000001Fu  /**< \brief (SYSCTRL_FORCECLKON) MASK Register */

/* -------- SYSCTRL_TESTMODE : (SYSCTRL Offset: 0x58) (R/W 32) Test Mode Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t XOSC:1;           /*!< bit:      0  XOSC Test Mode Enable              */
    uint32_t XOSC32:1;         /*!< bit:      1  XOSC32 Test Mode Enable            */
    uint32_t DFLL:1;           /*!< bit:      2  DFLL Test Mode Enable              */
    uint32_t BOD33:1;          /*!< bit:      3  BOD33 Test Mode Enable             */
    uint32_t BOD12:1;          /*!< bit:      4  BOD12 Test Mode Enable             */
    uint32_t OSC32K:1;         /*!< bit:      5  OSC32K Test Mode Enable            */
    uint32_t OSCULP32K:1;      /*!< bit:      6  OSCULP32K Test Mode Enable         */
    uint32_t OSC8M:1;          /*!< bit:      7  OSC8M Test Mode Enable             */
    uint32_t VREG:1;           /*!< bit:      8  VREG Test Mode Enable              */
    uint32_t VREF:1;           /*!< bit:      9  VREF Test Mode Enable              */
    uint32_t ALLCLK:1;         /*!< bit:     10  All Clocks Test Mode Enable        */
    uint32_t POR33:1;          /*!< bit:     11  POR33 Test Mode Enable             */
    uint32_t :20;              /*!< bit: 12..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} SYSCTRL_TESTMODE_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define SYSCTRL_TESTMODE_OFFSET     0x58         /**< \brief (SYSCTRL_TESTMODE offset) Test Mode Register */

#define SYSCTRL_TESTMODE_XOSC       (0x1u <<  0) /**< \brief (SYSCTRL_TESTMODE) XOSC Test Mode Enable */
#define SYSCTRL_TESTMODE_XOSC32     (0x1u <<  1) /**< \brief (SYSCTRL_TESTMODE) XOSC32 Test Mode Enable */
#define SYSCTRL_TESTMODE_DFLL       (0x1u <<  2) /**< \brief (SYSCTRL_TESTMODE) DFLL Test Mode Enable */
#define SYSCTRL_TESTMODE_BOD33      (0x1u <<  3) /**< \brief (SYSCTRL_TESTMODE) BOD33 Test Mode Enable */
#define SYSCTRL_TESTMODE_BOD12      (0x1u <<  4) /**< \brief (SYSCTRL_TESTMODE) BOD12 Test Mode Enable */
#define SYSCTRL_TESTMODE_OSC32K     (0x1u <<  5) /**< \brief (SYSCTRL_TESTMODE) OSC32K Test Mode Enable */
#define SYSCTRL_TESTMODE_OSCULP32K  (0x1u <<  6) /**< \brief (SYSCTRL_TESTMODE) OSCULP32K Test Mode Enable */
#define SYSCTRL_TESTMODE_OSC8M      (0x1u <<  7) /**< \brief (SYSCTRL_TESTMODE) OSC8M Test Mode Enable */
#define SYSCTRL_TESTMODE_VREG       (0x1u <<  8) /**< \brief (SYSCTRL_TESTMODE) VREG Test Mode Enable */
#define SYSCTRL_TESTMODE_VREF       (0x1u <<  9) /**< \brief (SYSCTRL_TESTMODE) VREF Test Mode Enable */
#define SYSCTRL_TESTMODE_ALLCLK     (0x1u << 10) /**< \brief (SYSCTRL_TESTMODE) All Clocks Test Mode Enable */
#define SYSCTRL_TESTMODE_POR33      (0x1u << 11) /**< \brief (SYSCTRL_TESTMODE) POR33 Test Mode Enable */
#define SYSCTRL_TESTMODE_MASK       0x00000FFFu  /**< \brief (SYSCTRL_TESTMODE) MASK Register */

/** \brief SYSCTRL hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  __IO SYSCTRL_INTENCLR_Type     INTENCLR;    /**< \brief Offset: 0x00 (R/W 32) Interrupt Enable Clear Register */
  __IO SYSCTRL_INTENSET_Type     INTENSET;    /**< \brief Offset: 0x04 (R/W 32) Interrupt Enable Set Register */
  __IO SYSCTRL_INTFLAG_Type      INTFLAG;     /**< \brief Offset: 0x08 (R/W 32) Interrupt Flag Status and Clear Register */
  __I  SYSCTRL_PCLKSR_Type       PCLKSR;      /**< \brief Offset: 0x0C (R/  32) Power and Clocks Status Register */
  __IO SYSCTRL_XOSC_Type         XOSC;        /**< \brief Offset: 0x10 (R/W 16) XOSC Control Register */
       RoReg8                    Reserved1[0x2];
  __IO SYSCTRL_XOSC32K_Type      XOSC32K;     /**< \brief Offset: 0x14 (R/W 16) XOSC32K Control Register */
  __IO SYSCTRL_XOSC32KTEST_Type  XOSC32KTEST; /**< \brief Offset: 0x16 (R/W  8) XOSC32K Test Register */
       RoReg8                    Reserved2[0x1];
  __IO SYSCTRL_DFLLCTRL_Type     DFLLCTRL;    /**< \brief Offset: 0x18 (R/W 16) DFLL Config Register */
       RoReg8                    Reserved3[0x2];
  __IO SYSCTRL_DFLLVAL_Type      DFLLVAL;     /**< \brief Offset: 0x1C (R/W 32) DFLL Calibration Value Register */
  __IO SYSCTRL_DFLLMUL_Type      DFLLMUL;     /**< \brief Offset: 0x20 (R/W 32) DFLL Multiplier Register */
  __IO SYSCTRL_DFLLSYNC_Type     DFLLSYNC;    /**< \brief Offset: 0x24 (R/W  8) DFLL Synchronization Register */
       RoReg8                    Reserved4[0x3];
  __IO SYSCTRL_BOD33_Type        BOD33;       /**< \brief Offset: 0x28 (R/W 32) BOD33 Control Register */
  __IO SYSCTRL_BOD12_Type        BOD12;       /**< \brief Offset: 0x2C (R/W 32) BOD12 Control Register */
  __IO SYSCTRL_OSC32K_Type       OSC32K;      /**< \brief Offset: 0x30 (R/W 32) OSC32K Control Register */
  __IO SYSCTRL_OSCULP32K_Type    OSCULP32K;   /**< \brief Offset: 0x34 (R/W  8) OSCULP32K Control Register */
       RoReg8                    Reserved5[0x3];
  __IO SYSCTRL_OSCULP32KTEST_Type OSCULP32KTEST; /**< \brief Offset: 0x38 (R/W  8) OSCULP32K Test Register */
       RoReg8                    Reserved6[0x3];
  __IO SYSCTRL_OSC8M_Type        OSC8M;       /**< \brief Offset: 0x3C (R/W 32) OSC8M Control Register A */
  __IO SYSCTRL_VREG_Type         VREG;        /**< \brief Offset: 0x40 (R/W 16) VREG Control Register */
       RoReg8                    Reserved7[0x2];
  __IO SYSCTRL_VREGTEST_Type     VREGTEST;    /**< \brief Offset: 0x44 (R/W 32) VREG Test Register */
  __IO SYSCTRL_VREF_Type         VREF;        /**< \brief Offset: 0x48 (R/W 32) VREF Control Register A */
  __IO SYSCTRL_VREFTESTA_Type    VREFTESTA;   /**< \brief Offset: 0x4C (R/W 32) VREF Test Register A */
  __IO SYSCTRL_VREFTESTB_Type    VREFTESTB;   /**< \brief Offset: 0x50 (R/W 16) VREF Test Register B */
       RoReg8                    Reserved8[0x2];
  __IO SYSCTRL_FORCECLKON_Type   FORCECLKON;  /**< \brief Offset: 0x54 (R/W 32) Force Clock On Register */
  __IO SYSCTRL_TESTMODE_Type     TESTMODE;    /**< \brief Offset: 0x58 (R/W 32) Test Mode Register */
} Sysctrl;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAMD20_SYSCTRL_COMPONENT_ */
