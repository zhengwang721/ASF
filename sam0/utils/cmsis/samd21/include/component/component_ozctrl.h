/**
 * \file
 *
 * \brief Component description for OZCTRL
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

#ifndef _SAMD21_OZCTRL_COMPONENT_
#define _SAMD21_OZCTRL_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR OZCTRL */
/* ========================================================================== */
/** \addtogroup SAMD21_OZCTRL OZMO control module */
/*@{*/

#define OZCTRL_U2243
#define REV_OZCTRL                  0x100

/* -------- OZCTRL_CTRL : (OZCTRL Offset: 0x0) (R/W  8) Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  RESET:1;          /*!< bit:      0                                     */
    uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} OZCTRL_CTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define OZCTRL_CTRL_OFFSET          0x0          /**< \brief (OZCTRL_CTRL offset) Control */
#define OZCTRL_CTRL_RESETVALUE      0x00         /**< \brief (OZCTRL_CTRL reset_value) Control */

#define OZCTRL_CTRL_RESET_Pos       0            /**< \brief (OZCTRL_CTRL)  */
#define OZCTRL_CTRL_RESET           (0x1u << OZCTRL_CTRL_RESET_Pos)
#define OZCTRL_CTRL_MASK            0x01u        /**< \brief (OZCTRL_CTRL) MASK Register */

/* -------- OZCTRL_DIR : (OZCTRL Offset: 0x2) (R/W 16) Wire Direction -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t DIR:10;           /*!< bit:  0.. 9                                     */
    uint16_t :6;               /*!< bit: 10..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} OZCTRL_DIR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define OZCTRL_DIR_OFFSET           0x2          /**< \brief (OZCTRL_DIR offset) Wire Direction */
#define OZCTRL_DIR_RESETVALUE       0x0000       /**< \brief (OZCTRL_DIR reset_value) Wire Direction */

#define OZCTRL_DIR_DIR_Pos          0            /**< \brief (OZCTRL_DIR)  */
#define OZCTRL_DIR_DIR_Msk          (0x3FFu << OZCTRL_DIR_DIR_Pos)
#define OZCTRL_DIR_DIR(value)       ((OZCTRL_DIR_DIR_Msk & ((value) << OZCTRL_DIR_DIR_Pos)))
#define OZCTRL_DIR_MASK             0x03FFu      /**< \brief (OZCTRL_DIR) MASK Register */

/** \brief OZCTRL hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  __IO OZCTRL_CTRL_Type          CTRL;        /**< \brief Offset: 0x0 (R/W  8) Control */
       RoReg8                    Reserved1[0x1];
  __IO OZCTRL_DIR_Type           DIR;         /**< \brief Offset: 0x2 (R/W 16) Wire Direction */
} Ozctrl;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAMD21_OZCTRL_COMPONENT_ */
