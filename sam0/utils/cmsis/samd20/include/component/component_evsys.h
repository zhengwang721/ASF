/**
 * \file
 *
 * \brief Component description for EVSYS
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

#ifndef _SAMD20_EVSYS_COMPONENT_
#define _SAMD20_EVSYS_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR EVSYS */
/* ========================================================================== */
/** \addtogroup SAMD20_EVSYS Event System Interface */
/*@{*/

#define REV_EVSYS                   0x100

/* -------- EVSYS_CTRL : (EVSYS Offset: 0x00) ( /W  8) Control Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint8_t  :3;               /*!< bit:  1.. 3  Reserved                           */
    uint8_t  GCLKREQ:1;        /*!< bit:      4  Generic Clock Request              */
    uint8_t  :3;               /*!< bit:  5.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} EVSYS_CTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EVSYS_CTRL_OFFSET           0x00         /**< \brief (EVSYS_CTRL offset) Control Register */
#define EVSYS_CTRL_RESETVALUE       0x00         /**< \brief (EVSYS_CTRL reset_value) Control Register */

#define EVSYS_CTRL_SWRST_Pos        0            /**< \brief (EVSYS_CTRL) Software Reset */
#define EVSYS_CTRL_SWRST            (0x1u << EVSYS_CTRL_SWRST_Pos)
#define EVSYS_CTRL_GCLKREQ_Pos      4            /**< \brief (EVSYS_CTRL) Generic Clock Request */
#define EVSYS_CTRL_GCLKREQ          (0x1u << EVSYS_CTRL_GCLKREQ_Pos)
#define EVSYS_CTRL_MASK             0x11u        /**< \brief (EVSYS_CTRL) MASK Register */

/* -------- EVSYS_CHANNEL : (EVSYS Offset: 0x04) (R/W 32) Channel Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t CHANNEL:8;        /*!< bit:  0.. 7  Channel Selection                  */
    uint32_t SWEVT:1;          /*!< bit:      8  Software Event                     */
    uint32_t :7;               /*!< bit:  9..15  Reserved                           */
    uint32_t EVGEN:8;          /*!< bit: 16..23  Event Generator Selection          */
    uint32_t PATH:2;           /*!< bit: 24..25  Path Selection                     */
    uint32_t EDGSEL:2;         /*!< bit: 26..27  Edge Selection                     */
    uint32_t :4;               /*!< bit: 28..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} EVSYS_CHANNEL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EVSYS_CHANNEL_OFFSET        0x04         /**< \brief (EVSYS_CHANNEL offset) Channel Register */
#define EVSYS_CHANNEL_RESETVALUE    0x00000000   /**< \brief (EVSYS_CHANNEL reset_value) Channel Register */

#define EVSYS_CHANNEL_CHANNEL_Pos   0            /**< \brief (EVSYS_CHANNEL) Channel Selection */
#define EVSYS_CHANNEL_CHANNEL_Msk   (0xFFu << EVSYS_CHANNEL_CHANNEL_Pos)
#define EVSYS_CHANNEL_CHANNEL(value) ((EVSYS_CHANNEL_CHANNEL_Msk & ((value) << EVSYS_CHANNEL_CHANNEL_Pos)))
#define EVSYS_CHANNEL_SWEVT_Pos     8            /**< \brief (EVSYS_CHANNEL) Software Event */
#define EVSYS_CHANNEL_SWEVT         (0x1u << EVSYS_CHANNEL_SWEVT_Pos)
#define EVSYS_CHANNEL_EVGEN_Pos     16           /**< \brief (EVSYS_CHANNEL) Event Generator Selection */
#define EVSYS_CHANNEL_EVGEN_Msk     (0xFFu << EVSYS_CHANNEL_EVGEN_Pos)
#define EVSYS_CHANNEL_EVGEN(value)  ((EVSYS_CHANNEL_EVGEN_Msk & ((value) << EVSYS_CHANNEL_EVGEN_Pos)))
#define EVSYS_CHANNEL_PATH_Pos      24           /**< \brief (EVSYS_CHANNEL) Path Selection */
#define EVSYS_CHANNEL_PATH_Msk      (0x3u << EVSYS_CHANNEL_PATH_Pos)
#define EVSYS_CHANNEL_PATH(value)   ((EVSYS_CHANNEL_PATH_Msk & ((value) << EVSYS_CHANNEL_PATH_Pos)))
#define EVSYS_CHANNEL_EDGSEL_Pos    26           /**< \brief (EVSYS_CHANNEL) Edge Selection */
#define EVSYS_CHANNEL_EDGSEL_Msk    (0x3u << EVSYS_CHANNEL_EDGSEL_Pos)
#define EVSYS_CHANNEL_EDGSEL(value) ((EVSYS_CHANNEL_EDGSEL_Msk & ((value) << EVSYS_CHANNEL_EDGSEL_Pos)))
#define EVSYS_CHANNEL_MASK          0x0FFF01FFu  /**< \brief (EVSYS_CHANNEL) MASK Register */

/* -------- EVSYS_USERMUX : (EVSYS Offset: 0x08) (R/W 16) User Mux Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t UMUXSEL:8;        /*!< bit:  0.. 7  User Mux Selection                 */
    uint16_t CHANNELEVENT:8;   /*!< bit:  8..15  Channel Event Selection            */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} EVSYS_USERMUX_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EVSYS_USERMUX_OFFSET        0x08         /**< \brief (EVSYS_USERMUX offset) User Mux Register */
#define EVSYS_USERMUX_RESETVALUE    0x0000       /**< \brief (EVSYS_USERMUX reset_value) User Mux Register */

#define EVSYS_USERMUX_UMUXSEL_Pos   0            /**< \brief (EVSYS_USERMUX) User Mux Selection */
#define EVSYS_USERMUX_UMUXSEL_Msk   (0xFFu << EVSYS_USERMUX_UMUXSEL_Pos)
#define EVSYS_USERMUX_UMUXSEL(value) ((EVSYS_USERMUX_UMUXSEL_Msk & ((value) << EVSYS_USERMUX_UMUXSEL_Pos)))
#define EVSYS_USERMUX_CHANNELEVENT_Pos 8            /**< \brief (EVSYS_USERMUX) Channel Event Selection */
#define EVSYS_USERMUX_CHANNELEVENT_Msk (0xFFu << EVSYS_USERMUX_CHANNELEVENT_Pos)
#define EVSYS_USERMUX_CHANNELEVENT(value) ((EVSYS_USERMUX_CHANNELEVENT_Msk & ((value) << EVSYS_USERMUX_CHANNELEVENT_Pos)))
#define EVSYS_USERMUX_MASK          0xFFFFu      /**< \brief (EVSYS_USERMUX) MASK Register */

/* -------- EVSYS_CHSTATUS : (EVSYS Offset: 0x0C) (R/  32) Channel Status Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t USRREADY0:8;      /*!< bit:  0.. 7  User Ready for Channels 0 to 7 (modulo 16) */
    uint32_t CHBUSY0:8;        /*!< bit:  8..15  Channels Busy 0 to 7 (modulo 16)   */
    uint32_t USRREADY1:8;      /*!< bit: 16..23  User Ready for Channels 8 to 15 (modulo 16) */
    uint32_t CHBUSY1:8;        /*!< bit: 24..31  Channels Busy 8 to 15 (modulo 16)  */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} EVSYS_CHSTATUS_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EVSYS_CHSTATUS_OFFSET       0x0C         /**< \brief (EVSYS_CHSTATUS offset) Channel Status Register */
#define EVSYS_CHSTATUS_RESETVALUE   0x00000000   /**< \brief (EVSYS_CHSTATUS reset_value) Channel Status Register */

#define EVSYS_CHSTATUS_USRREADY0_Pos 0            /**< \brief (EVSYS_CHSTATUS) User Ready for Channels 0 to 7 (modulo 16) */
#define EVSYS_CHSTATUS_USRREADY0_Msk (0xFFu << EVSYS_CHSTATUS_USRREADY0_Pos)
#define EVSYS_CHSTATUS_USRREADY0(value) ((EVSYS_CHSTATUS_USRREADY0_Msk & ((value) << EVSYS_CHSTATUS_USRREADY0_Pos)))
#define EVSYS_CHSTATUS_CHBUSY0_Pos  8            /**< \brief (EVSYS_CHSTATUS) Channels Busy 0 to 7 (modulo 16) */
#define EVSYS_CHSTATUS_CHBUSY0_Msk  (0xFFu << EVSYS_CHSTATUS_CHBUSY0_Pos)
#define EVSYS_CHSTATUS_CHBUSY0(value) ((EVSYS_CHSTATUS_CHBUSY0_Msk & ((value) << EVSYS_CHSTATUS_CHBUSY0_Pos)))
#define EVSYS_CHSTATUS_USRREADY1_Pos 16           /**< \brief (EVSYS_CHSTATUS) User Ready for Channels 8 to 15 (modulo 16) */
#define EVSYS_CHSTATUS_USRREADY1_Msk (0xFFu << EVSYS_CHSTATUS_USRREADY1_Pos)
#define EVSYS_CHSTATUS_USRREADY1(value) ((EVSYS_CHSTATUS_USRREADY1_Msk & ((value) << EVSYS_CHSTATUS_USRREADY1_Pos)))
#define EVSYS_CHSTATUS_CHBUSY1_Pos  24           /**< \brief (EVSYS_CHSTATUS) Channels Busy 8 to 15 (modulo 16) */
#define EVSYS_CHSTATUS_CHBUSY1_Msk  (0xFFu << EVSYS_CHSTATUS_CHBUSY1_Pos)
#define EVSYS_CHSTATUS_CHBUSY1(value) ((EVSYS_CHSTATUS_CHBUSY1_Msk & ((value) << EVSYS_CHSTATUS_CHBUSY1_Pos)))
#define EVSYS_CHSTATUS_MASK         0xFFFFFFFFu  /**< \brief (EVSYS_CHSTATUS) MASK Register */

/* -------- EVSYS_INTENCLR : (EVSYS Offset: 0x10) (R/W 32) Interrupt Enable Clear Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t OVR0:8;           /*!< bit:  0.. 7  Overrun Interrupt Disable for Channels 0 to 7 (modulo 16) */
    uint32_t EVD0:8;           /*!< bit:  8..15  Event Detection Interrupt Disable for Channels 0 to 7 (modulo 16) */
    uint32_t OVR1:8;           /*!< bit: 16..23  Overrun Interrupt Disable for Channels 8 to 15 (modulo 16) */
    uint32_t EVD1:8;           /*!< bit: 24..31  Event Detection Interrupt Disable for Channels 8 to 15 (modulo 16) */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} EVSYS_INTENCLR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EVSYS_INTENCLR_OFFSET       0x10         /**< \brief (EVSYS_INTENCLR offset) Interrupt Enable Clear Register */
#define EVSYS_INTENCLR_RESETVALUE   0x00000000   /**< \brief (EVSYS_INTENCLR reset_value) Interrupt Enable Clear Register */

#define EVSYS_INTENCLR_OVR0_Pos     0            /**< \brief (EVSYS_INTENCLR) Overrun Interrupt Disable for Channels 0 to 7 (modulo 16) */
#define EVSYS_INTENCLR_OVR0_Msk     (0xFFu << EVSYS_INTENCLR_OVR0_Pos)
#define EVSYS_INTENCLR_OVR0(value)  ((EVSYS_INTENCLR_OVR0_Msk & ((value) << EVSYS_INTENCLR_OVR0_Pos)))
#define EVSYS_INTENCLR_EVD0_Pos     8            /**< \brief (EVSYS_INTENCLR) Event Detection Interrupt Disable for Channels 0 to 7 (modulo 16) */
#define EVSYS_INTENCLR_EVD0_Msk     (0xFFu << EVSYS_INTENCLR_EVD0_Pos)
#define EVSYS_INTENCLR_EVD0(value)  ((EVSYS_INTENCLR_EVD0_Msk & ((value) << EVSYS_INTENCLR_EVD0_Pos)))
#define EVSYS_INTENCLR_OVR1_Pos     16           /**< \brief (EVSYS_INTENCLR) Overrun Interrupt Disable for Channels 8 to 15 (modulo 16) */
#define EVSYS_INTENCLR_OVR1_Msk     (0xFFu << EVSYS_INTENCLR_OVR1_Pos)
#define EVSYS_INTENCLR_OVR1(value)  ((EVSYS_INTENCLR_OVR1_Msk & ((value) << EVSYS_INTENCLR_OVR1_Pos)))
#define EVSYS_INTENCLR_EVD1_Pos     24           /**< \brief (EVSYS_INTENCLR) Event Detection Interrupt Disable for Channels 8 to 15 (modulo 16) */
#define EVSYS_INTENCLR_EVD1_Msk     (0xFFu << EVSYS_INTENCLR_EVD1_Pos)
#define EVSYS_INTENCLR_EVD1(value)  ((EVSYS_INTENCLR_EVD1_Msk & ((value) << EVSYS_INTENCLR_EVD1_Pos)))
#define EVSYS_INTENCLR_MASK         0xFFFFFFFFu  /**< \brief (EVSYS_INTENCLR) MASK Register */

/* -------- EVSYS_INTENSET : (EVSYS Offset: 0x14) (R/W 32) Interrupt Enable Set Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t OVR0:8;           /*!< bit:  0.. 7  Overrun Interrupt Enable for Channels 0 to 7 (modulo 16) */
    uint32_t EVD0:8;           /*!< bit:  8..15  Event Detection Interrupt Enable for Channels 0 to 7 (modulo 16) */
    uint32_t OVR1:8;           /*!< bit: 16..23  Overrun Interrupt Enable for Channels 8 to 15 (modulo 16) */
    uint32_t EVD1:8;           /*!< bit: 24..31  Event Detection Interrupt Enable for Channels 8 to 15 (modulo 16) */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} EVSYS_INTENSET_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EVSYS_INTENSET_OFFSET       0x14         /**< \brief (EVSYS_INTENSET offset) Interrupt Enable Set Register */
#define EVSYS_INTENSET_RESETVALUE   0x00000000   /**< \brief (EVSYS_INTENSET reset_value) Interrupt Enable Set Register */

#define EVSYS_INTENSET_OVR0_Pos     0            /**< \brief (EVSYS_INTENSET) Overrun Interrupt Enable for Channels 0 to 7 (modulo 16) */
#define EVSYS_INTENSET_OVR0_Msk     (0xFFu << EVSYS_INTENSET_OVR0_Pos)
#define EVSYS_INTENSET_OVR0(value)  ((EVSYS_INTENSET_OVR0_Msk & ((value) << EVSYS_INTENSET_OVR0_Pos)))
#define EVSYS_INTENSET_EVD0_Pos     8            /**< \brief (EVSYS_INTENSET) Event Detection Interrupt Enable for Channels 0 to 7 (modulo 16) */
#define EVSYS_INTENSET_EVD0_Msk     (0xFFu << EVSYS_INTENSET_EVD0_Pos)
#define EVSYS_INTENSET_EVD0(value)  ((EVSYS_INTENSET_EVD0_Msk & ((value) << EVSYS_INTENSET_EVD0_Pos)))
#define EVSYS_INTENSET_OVR1_Pos     16           /**< \brief (EVSYS_INTENSET) Overrun Interrupt Enable for Channels 8 to 15 (modulo 16) */
#define EVSYS_INTENSET_OVR1_Msk     (0xFFu << EVSYS_INTENSET_OVR1_Pos)
#define EVSYS_INTENSET_OVR1(value)  ((EVSYS_INTENSET_OVR1_Msk & ((value) << EVSYS_INTENSET_OVR1_Pos)))
#define EVSYS_INTENSET_EVD1_Pos     24           /**< \brief (EVSYS_INTENSET) Event Detection Interrupt Enable for Channels 8 to 15 (modulo 16) */
#define EVSYS_INTENSET_EVD1_Msk     (0xFFu << EVSYS_INTENSET_EVD1_Pos)
#define EVSYS_INTENSET_EVD1(value)  ((EVSYS_INTENSET_EVD1_Msk & ((value) << EVSYS_INTENSET_EVD1_Pos)))
#define EVSYS_INTENSET_MASK         0xFFFFFFFFu  /**< \brief (EVSYS_INTENSET) MASK Register */

/* -------- EVSYS_INTFLAG : (EVSYS Offset: 0x18) (R/W 32) Interrupt Flag Status and Clear Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t OVR0:8;           /*!< bit:  0.. 7  Overrun Interrupt Flag for Channels 0 to 7 (modulo 16) */
    uint32_t EVD0:8;           /*!< bit:  8..15  Event Detection Interrupt Flag for Channels 0 to 7 (modulo 16) */
    uint32_t OVR1:8;           /*!< bit: 16..23  Overrun Interrupt Flag for Channels 8 to 15 (modulo 16) */
    uint32_t EVD1:8;           /*!< bit: 24..31  Event Detection Interrupt Flag for Channels 8 to 15 (modulo 16) */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} EVSYS_INTFLAG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EVSYS_INTFLAG_OFFSET        0x18         /**< \brief (EVSYS_INTFLAG offset) Interrupt Flag Status and Clear Register */
#define EVSYS_INTFLAG_RESETVALUE    0x00000000   /**< \brief (EVSYS_INTFLAG reset_value) Interrupt Flag Status and Clear Register */

#define EVSYS_INTFLAG_OVR0_Pos      0            /**< \brief (EVSYS_INTFLAG) Overrun Interrupt Flag for Channels 0 to 7 (modulo 16) */
#define EVSYS_INTFLAG_OVR0_Msk      (0xFFu << EVSYS_INTFLAG_OVR0_Pos)
#define EVSYS_INTFLAG_OVR0(value)   ((EVSYS_INTFLAG_OVR0_Msk & ((value) << EVSYS_INTFLAG_OVR0_Pos)))
#define EVSYS_INTFLAG_EVD0_Pos      8            /**< \brief (EVSYS_INTFLAG) Event Detection Interrupt Flag for Channels 0 to 7 (modulo 16) */
#define EVSYS_INTFLAG_EVD0_Msk      (0xFFu << EVSYS_INTFLAG_EVD0_Pos)
#define EVSYS_INTFLAG_EVD0(value)   ((EVSYS_INTFLAG_EVD0_Msk & ((value) << EVSYS_INTFLAG_EVD0_Pos)))
#define EVSYS_INTFLAG_OVR1_Pos      16           /**< \brief (EVSYS_INTFLAG) Overrun Interrupt Flag for Channels 8 to 15 (modulo 16) */
#define EVSYS_INTFLAG_OVR1_Msk      (0xFFu << EVSYS_INTFLAG_OVR1_Pos)
#define EVSYS_INTFLAG_OVR1(value)   ((EVSYS_INTFLAG_OVR1_Msk & ((value) << EVSYS_INTFLAG_OVR1_Pos)))
#define EVSYS_INTFLAG_EVD1_Pos      24           /**< \brief (EVSYS_INTFLAG) Event Detection Interrupt Flag for Channels 8 to 15 (modulo 16) */
#define EVSYS_INTFLAG_EVD1_Msk      (0xFFu << EVSYS_INTFLAG_EVD1_Pos)
#define EVSYS_INTFLAG_EVD1(value)   ((EVSYS_INTFLAG_EVD1_Msk & ((value) << EVSYS_INTFLAG_EVD1_Pos)))
#define EVSYS_INTFLAG_MASK          0xFFFFFFFFu  /**< \brief (EVSYS_INTFLAG) MASK Register */

/** \brief EVSYS hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  __O  EVSYS_CTRL_Type           CTRL;        /**< \brief Offset: 0x00 ( /W  8) Control Register */
       RoReg8                    Reserved1[0x3];
  __IO EVSYS_CHANNEL_Type        CHANNEL;     /**< \brief Offset: 0x04 (R/W 32) Channel Register */
  __IO EVSYS_USERMUX_Type        USERMUX;     /**< \brief Offset: 0x08 (R/W 16) User Mux Register */
       RoReg8                    Reserved2[0x2];
  __I  EVSYS_CHSTATUS_Type       CHSTATUS;    /**< \brief Offset: 0x0C (R/  32) Channel Status Register */
  __IO EVSYS_INTENCLR_Type       INTENCLR;    /**< \brief Offset: 0x10 (R/W 32) Interrupt Enable Clear Register */
  __IO EVSYS_INTENSET_Type       INTENSET;    /**< \brief Offset: 0x14 (R/W 32) Interrupt Enable Set Register */
  __IO EVSYS_INTFLAG_Type        INTFLAG;     /**< \brief Offset: 0x18 (R/W 32) Interrupt Flag Status and Clear Register */
} Evsys;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAMD20_EVSYS_COMPONENT_ */
