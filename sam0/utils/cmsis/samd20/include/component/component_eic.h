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

#ifndef _SAMD20_EIC_COMPONENT_
#define _SAMD20_EIC_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR EIC */
/* ========================================================================== */
/** \addtogroup SAMD20_EIC External Interrupt Controller */
/*@{*/

#define REV_EIC        0x100

/* -------- EIC_CTRL : (EIC Offset: 0x00) (R/W  8) Control Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint8_t  ENABLE:1;         /*!< bit:      1  Enable                             */
    uint8_t  :6;               /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} EIC_CTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EIC_CTRL_OFFSET             0x00         /**< \brief (EIC_CTRL offset) Control Register */
#define EIC_CTRL_RESETVALUE         0x00         /**< \brief (EIC_CTRL reset_value) Control Register */

#define EIC_CTRL_SWRST              (0x1u <<  0) /**< \brief (EIC_CTRL) Software Reset */
#define EIC_CTRL_ENABLE             (0x1u <<  1) /**< \brief (EIC_CTRL) Enable */
#define EIC_CTRL_MASK               0x03u        /**< \brief (EIC_CTRL) MASK Register */

/* -------- EIC_STATUS : (EIC Offset: 0x01) (R/   8) Status Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  :7;               /*!< bit:  0.. 6  Reserved                           */
    uint8_t  SYNCBUSY:1;       /*!< bit:      7  Sync Busy                          */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} EIC_STATUS_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EIC_STATUS_OFFSET           0x01         /**< \brief (EIC_STATUS offset) Status Register */
#define EIC_STATUS_RESETVALUE       0x00         /**< \brief (EIC_STATUS reset_value) Status Register */

#define EIC_STATUS_SYNCBUSY         (0x1u <<  7) /**< \brief (EIC_STATUS) Sync Busy */
#define EIC_STATUS_MASK             0x80u        /**< \brief (EIC_STATUS) MASK Register */

/* -------- EIC_NMICTRL : (EIC Offset: 0x02) (R/W  8) NMI Control Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  NMISENSE:3;       /*!< bit:  0.. 2  NMI Input Sense Configuration      */
    uint8_t  NMIFILTEN:1;      /*!< bit:      3  NMI Filter Enable                  */
    uint8_t  :4;               /*!< bit:  4.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} EIC_NMICTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EIC_NMICTRL_OFFSET          0x02         /**< \brief (EIC_NMICTRL offset) NMI Control Register */
#define EIC_NMICTRL_RESETVALUE      0x00         /**< \brief (EIC_NMICTRL reset_value) NMI Control Register */

#define EIC_NMICTRL_NMISENSE_Pos    0
#define EIC_NMICTRL_NMISENSE_Msk    (0x7u << EIC_NMICTRL_NMISENSE_Pos) /**< \brief (EIC_NMICTRL) NMI Input Sense Configuration */
#define EIC_NMICTRL_NMISENSE(value) ((EIC_NMICTRL_NMISENSE_Msk & ((value) << EIC_NMICTRL_NMISENSE_Pos)))
#define EIC_NMICTRL_NMIFILTEN       (0x1u <<  3) /**< \brief (EIC_NMICTRL) NMI Filter Enable */
#define EIC_NMICTRL_MASK            0x0Fu        /**< \brief (EIC_NMICTRL) MASK Register */

/* -------- EIC_NMIFLAG : (EIC Offset: 0x03) (R/W  8) NMI Interrupt Flag Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  NMI:1;            /*!< bit:      0  NMI Interrupt Flag                 */
    uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} EIC_NMIFLAG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EIC_NMIFLAG_OFFSET          0x03         /**< \brief (EIC_NMIFLAG offset) NMI Interrupt Flag Register */
#define EIC_NMIFLAG_RESETVALUE      0x00         /**< \brief (EIC_NMIFLAG reset_value) NMI Interrupt Flag Register */

#define EIC_NMIFLAG_NMI             (0x1u <<  0) /**< \brief (EIC_NMIFLAG) NMI Interrupt Flag */
#define EIC_NMIFLAG_MASK            0x01u        /**< \brief (EIC_NMIFLAG) MASK Register */

/* -------- EIC_EVCTRL : (EIC Offset: 0x04) (R/W 32) Event Control Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} EIC_EVCTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EIC_EVCTRL_OFFSET           0x04         /**< \brief (EIC_EVCTRL offset) Event Control Register */
#define EIC_EVCTRL_RESETVALUE       0x00000000   /**< \brief (EIC_EVCTRL reset_value) Event Control Register */
#define EIC_EVCTRL_MASK             0xFFFFFFFFu  /**< \brief (EIC_EVCTRL) MASK Register */

/* -------- EIC_INTENCLR : (EIC Offset: 0x08) (R/W 32) Interrupt Enable Clear Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} EIC_INTENCLR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EIC_INTENCLR_OFFSET         0x08         /**< \brief (EIC_INTENCLR offset) Interrupt Enable Clear Register */
#define EIC_INTENCLR_RESETVALUE     0x00000000   /**< \brief (EIC_INTENCLR reset_value) Interrupt Enable Clear Register */
#define EIC_INTENCLR_MASK           0xFFFFFFFFu  /**< \brief (EIC_INTENCLR) MASK Register */

/* -------- EIC_INTENSET : (EIC Offset: 0x0C) (R/W 32) Interrupt Enable Set Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} EIC_INTENSET_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EIC_INTENSET_OFFSET         0x0C         /**< \brief (EIC_INTENSET offset) Interrupt Enable Set Register */
#define EIC_INTENSET_RESETVALUE     0x00000000   /**< \brief (EIC_INTENSET reset_value) Interrupt Enable Set Register */
#define EIC_INTENSET_MASK           0xFFFFFFFFu  /**< \brief (EIC_INTENSET) MASK Register */

/* -------- EIC_INTFLAG : (EIC Offset: 0x10) (R/W 32) Interrupt Flag Status and Clear Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} EIC_INTFLAG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EIC_INTFLAG_OFFSET          0x10         /**< \brief (EIC_INTFLAG offset) Interrupt Flag Status and Clear Register */
#define EIC_INTFLAG_RESETVALUE      0x00000000   /**< \brief (EIC_INTFLAG reset_value) Interrupt Flag Status and Clear Register */
#define EIC_INTFLAG_MASK            0xFFFFFFFFu  /**< \brief (EIC_INTFLAG) MASK Register */

/* -------- EIC_WAKEUP : (EIC Offset: 0x14) (R/W 32) Wake-up Enable Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} EIC_WAKEUP_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EIC_WAKEUP_OFFSET           0x14         /**< \brief (EIC_WAKEUP offset) Wake-up Enable Register */
#define EIC_WAKEUP_RESETVALUE       0x00000000   /**< \brief (EIC_WAKEUP reset_value) Wake-up Enable Register */
#define EIC_WAKEUP_MASK             0xFFFFFFFFu  /**< \brief (EIC_WAKEUP) MASK Register */

/* -------- EIC_CONFIG : (EIC Offset: 0x18) (R/W 32) Config Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} EIC_CONFIG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define EIC_CONFIG_OFFSET           0x18         /**< \brief (EIC_CONFIG offset) Config Register */
#define EIC_CONFIG_RESETVALUE       0x00000000   /**< \brief (EIC_CONFIG reset_value) Config Register */
#define EIC_CONFIG_MASK             0xFFFFFFFFu  /**< \brief (EIC_CONFIG) MASK Register */

#define EIC_NUMBER_OF_CONFIG_REGS   2

/** \brief EIC hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  __IO EIC_CTRL_Type             CTRL;        /**< \brief Offset: 0x00 (R/W  8) Control Register */
  __I  EIC_STATUS_Type           STATUS;      /**< \brief Offset: 0x01 (R/   8) Status Register */
  __IO EIC_NMICTRL_Type          NMICTRL;     /**< \brief Offset: 0x02 (R/W  8) NMI Control Register */
  __IO EIC_NMIFLAG_Type          NMIFLAG;     /**< \brief Offset: 0x03 (R/W  8) NMI Interrupt Flag Register */
  __IO EIC_EVCTRL_Type           EVCTRL;      /**< \brief Offset: 0x04 (R/W 32) Event Control Register */
  __IO EIC_INTENCLR_Type         INTENCLR;    /**< \brief Offset: 0x08 (R/W 32) Interrupt Enable Clear Register */
  __IO EIC_INTENSET_Type         INTENSET;    /**< \brief Offset: 0x0C (R/W 32) Interrupt Enable Set Register */
  __IO EIC_INTFLAG_Type          INTFLAG;     /**< \brief Offset: 0x10 (R/W 32) Interrupt Flag Status and Clear Register */
  __IO EIC_WAKEUP_Type           WAKEUP;      /**< \brief Offset: 0x14 (R/W 32) Wake-up Enable Register */
  __IO EIC_CONFIG_Type           CONFIG[EIC_NUMBER_OF_CONFIG_REGS]; /**< \brief Offset: 0x18 (R/W 32) Config Register */
       EIC_CONFIG_Type           Reserved1[4 - EIC_NUMBER_OF_CONFIG_REGS];
} Eic;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAMD20_EIC_COMPONENT_ */
