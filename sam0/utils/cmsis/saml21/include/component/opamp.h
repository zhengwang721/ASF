/**
 * \file
 *
 * \brief Component description for OPAMP
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
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

#ifndef _SAML21_OPAMP_COMPONENT_
#define _SAML21_OPAMP_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR OPAMP */
/* ========================================================================== */
/** \addtogroup SAML21_OPAMP Operational Amplifier */
/*@{*/

#define OPAMP_U2237
#define REV_OPAMP                   0x100

/* -------- OPAMP_CTRLA : (OPAMP Offset: 0x00) (R/W  8) Control A -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint8_t  ENABLE:1;         /*!< bit:      1  Enable                             */
    uint8_t  :5;               /*!< bit:  2.. 6  Reserved                           */
    uint8_t  LPMUX:1;          /*!< bit:      7  Low-Power Mux                      */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} OPAMP_CTRLA_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define OPAMP_CTRLA_OFFSET          0x00         /**< \brief (OPAMP_CTRLA offset) Control A */
#define OPAMP_CTRLA_RESETVALUE      0x00ul       /**< \brief (OPAMP_CTRLA reset_value) Control A */

#define OPAMP_CTRLA_SWRST_Pos       0            /**< \brief (OPAMP_CTRLA) Software Reset */
#define OPAMP_CTRLA_SWRST           (0x1ul << OPAMP_CTRLA_SWRST_Pos)
#define OPAMP_CTRLA_ENABLE_Pos      1            /**< \brief (OPAMP_CTRLA) Enable */
#define OPAMP_CTRLA_ENABLE          (0x1ul << OPAMP_CTRLA_ENABLE_Pos)
#define OPAMP_CTRLA_LPMUX_Pos       7            /**< \brief (OPAMP_CTRLA) Low-Power Mux */
#define OPAMP_CTRLA_LPMUX           (0x1ul << OPAMP_CTRLA_LPMUX_Pos)
#define OPAMP_CTRLA_MASK            0x83ul       /**< \brief (OPAMP_CTRLA) MASK Register */

/* -------- OPAMP_STATUS : (OPAMP Offset: 0x02) (R/   8) Status -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  READY0:1;         /*!< bit:      0  OPAMP 0 Ready                      */
    uint8_t  READY1:1;         /*!< bit:      1  OPAMP 1 Ready                      */
    uint8_t  READY2:1;         /*!< bit:      2  OPAMP 2 Ready                      */
    uint8_t  :5;               /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} OPAMP_STATUS_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define OPAMP_STATUS_OFFSET         0x02         /**< \brief (OPAMP_STATUS offset) Status */
#define OPAMP_STATUS_RESETVALUE     0x00ul       /**< \brief (OPAMP_STATUS reset_value) Status */

#define OPAMP_STATUS_READY0_Pos     0            /**< \brief (OPAMP_STATUS) OPAMP 0 Ready */
#define OPAMP_STATUS_READY0         (0x1ul << OPAMP_STATUS_READY0_Pos)
#define OPAMP_STATUS_READY1_Pos     1            /**< \brief (OPAMP_STATUS) OPAMP 1 Ready */
#define OPAMP_STATUS_READY1         (0x1ul << OPAMP_STATUS_READY1_Pos)
#define OPAMP_STATUS_READY2_Pos     2            /**< \brief (OPAMP_STATUS) OPAMP 2 Ready */
#define OPAMP_STATUS_READY2         (0x1ul << OPAMP_STATUS_READY2_Pos)
#define OPAMP_STATUS_MASK           0x07ul       /**< \brief (OPAMP_STATUS) MASK Register */

/* -------- OPAMP_OPAMPCTRL0 : (OPAMP Offset: 0x04) (R/W 32) OPAMP Control 0 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t :1;               /*!< bit:      0  Reserved                           */
    uint32_t ENABLE:1;         /*!< bit:      1  Operational Amplifier Enable       */
    uint32_t ANAOUT:1;         /*!< bit:      2  Analog Output                      */
    uint32_t BIAS:2;           /*!< bit:  3.. 4  Bias Selection                     */
    uint32_t :1;               /*!< bit:      5  Reserved                           */
    uint32_t RUNSTDBY:1;       /*!< bit:      6  Run in Standby                     */
    uint32_t ONDEMAND:1;       /*!< bit:      7  On Demand Control                  */
    uint32_t RES2OUT:1;        /*!< bit:      8  Resistor ladder To Output          */
    uint32_t RES2VCC:1;        /*!< bit:      9  Resistor ladder To VCC             */
    uint32_t RES1EN:1;         /*!< bit:     10  Resistor 1 Enable                  */
    uint32_t RES1MUX:2;        /*!< bit: 11..12  Resistor 1 Mux                     */
    uint32_t POTMUX:3;         /*!< bit: 13..15  Potentiometer Selection            */
    uint32_t MUXPOS:3;         /*!< bit: 16..18  Positive Input Mux Selection       */
    uint32_t :1;               /*!< bit:     19  Reserved                           */
    uint32_t MUXNEG:3;         /*!< bit: 20..22  Negative Input Mux Selection       */
    uint32_t :9;               /*!< bit: 23..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} OPAMP_OPAMPCTRL0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define OPAMP_OPAMPCTRL0_OFFSET     0x04         /**< \brief (OPAMP_OPAMPCTRL0 offset) OPAMP Control 0 */
#define OPAMP_OPAMPCTRL0_RESETVALUE 0x00000000ul /**< \brief (OPAMP_OPAMPCTRL0 reset_value) OPAMP Control 0 */

#define OPAMP_OPAMPCTRL0_ENABLE_Pos 1            /**< \brief (OPAMP_OPAMPCTRL0) Operational Amplifier Enable */
#define OPAMP_OPAMPCTRL0_ENABLE     (0x1ul << OPAMP_OPAMPCTRL0_ENABLE_Pos)
#define OPAMP_OPAMPCTRL0_ANAOUT_Pos 2            /**< \brief (OPAMP_OPAMPCTRL0) Analog Output */
#define OPAMP_OPAMPCTRL0_ANAOUT     (0x1ul << OPAMP_OPAMPCTRL0_ANAOUT_Pos)
#define OPAMP_OPAMPCTRL0_BIAS_Pos   3            /**< \brief (OPAMP_OPAMPCTRL0) Bias Selection */
#define OPAMP_OPAMPCTRL0_BIAS_Msk   (0x3ul << OPAMP_OPAMPCTRL0_BIAS_Pos)
#define OPAMP_OPAMPCTRL0_BIAS(value) ((OPAMP_OPAMPCTRL0_BIAS_Msk & ((value) << OPAMP_OPAMPCTRL0_BIAS_Pos)))
#define OPAMP_OPAMPCTRL0_RUNSTDBY_Pos 6            /**< \brief (OPAMP_OPAMPCTRL0) Run in Standby */
#define OPAMP_OPAMPCTRL0_RUNSTDBY   (0x1ul << OPAMP_OPAMPCTRL0_RUNSTDBY_Pos)
#define OPAMP_OPAMPCTRL0_ONDEMAND_Pos 7            /**< \brief (OPAMP_OPAMPCTRL0) On Demand Control */
#define OPAMP_OPAMPCTRL0_ONDEMAND   (0x1ul << OPAMP_OPAMPCTRL0_ONDEMAND_Pos)
#define OPAMP_OPAMPCTRL0_RES2OUT_Pos 8            /**< \brief (OPAMP_OPAMPCTRL0) Resistor ladder To Output */
#define OPAMP_OPAMPCTRL0_RES2OUT    (0x1ul << OPAMP_OPAMPCTRL0_RES2OUT_Pos)
#define OPAMP_OPAMPCTRL0_RES2VCC_Pos 9            /**< \brief (OPAMP_OPAMPCTRL0) Resistor ladder To VCC */
#define OPAMP_OPAMPCTRL0_RES2VCC    (0x1ul << OPAMP_OPAMPCTRL0_RES2VCC_Pos)
#define OPAMP_OPAMPCTRL0_RES1EN_Pos 10           /**< \brief (OPAMP_OPAMPCTRL0) Resistor 1 Enable */
#define OPAMP_OPAMPCTRL0_RES1EN     (0x1ul << OPAMP_OPAMPCTRL0_RES1EN_Pos)
#define OPAMP_OPAMPCTRL0_RES1MUX_Pos 11           /**< \brief (OPAMP_OPAMPCTRL0) Resistor 1 Mux */
#define OPAMP_OPAMPCTRL0_RES1MUX_Msk (0x3ul << OPAMP_OPAMPCTRL0_RES1MUX_Pos)
#define OPAMP_OPAMPCTRL0_RES1MUX(value) ((OPAMP_OPAMPCTRL0_RES1MUX_Msk & ((value) << OPAMP_OPAMPCTRL0_RES1MUX_Pos)))
#define OPAMP_OPAMPCTRL0_POTMUX_Pos 13           /**< \brief (OPAMP_OPAMPCTRL0) Potentiometer Selection */
#define OPAMP_OPAMPCTRL0_POTMUX_Msk (0x7ul << OPAMP_OPAMPCTRL0_POTMUX_Pos)
#define OPAMP_OPAMPCTRL0_POTMUX(value) ((OPAMP_OPAMPCTRL0_POTMUX_Msk & ((value) << OPAMP_OPAMPCTRL0_POTMUX_Pos)))
#define OPAMP_OPAMPCTRL0_MUXPOS_Pos 16           /**< \brief (OPAMP_OPAMPCTRL0) Positive Input Mux Selection */
#define OPAMP_OPAMPCTRL0_MUXPOS_Msk (0x7ul << OPAMP_OPAMPCTRL0_MUXPOS_Pos)
#define OPAMP_OPAMPCTRL0_MUXPOS(value) ((OPAMP_OPAMPCTRL0_MUXPOS_Msk & ((value) << OPAMP_OPAMPCTRL0_MUXPOS_Pos)))
#define OPAMP_OPAMPCTRL0_MUXNEG_Pos 20           /**< \brief (OPAMP_OPAMPCTRL0) Negative Input Mux Selection */
#define OPAMP_OPAMPCTRL0_MUXNEG_Msk (0x7ul << OPAMP_OPAMPCTRL0_MUXNEG_Pos)
#define OPAMP_OPAMPCTRL0_MUXNEG(value) ((OPAMP_OPAMPCTRL0_MUXNEG_Msk & ((value) << OPAMP_OPAMPCTRL0_MUXNEG_Pos)))
#define OPAMP_OPAMPCTRL0_MASK       0x0077FFDEul /**< \brief (OPAMP_OPAMPCTRL0) MASK Register */

/* -------- OPAMP_OPAMPCTRL1 : (OPAMP Offset: 0x08) (R/W 32) OPAMP Control 1 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t :1;               /*!< bit:      0  Reserved                           */
    uint32_t ENABLE:1;         /*!< bit:      1  Operational Amplifier Enable       */
    uint32_t ANAOUT:1;         /*!< bit:      2  Analog Output                      */
    uint32_t BIAS:2;           /*!< bit:  3.. 4  Bias Selection                     */
    uint32_t :1;               /*!< bit:      5  Reserved                           */
    uint32_t RUNSTDBY:1;       /*!< bit:      6  Run in Standby                     */
    uint32_t ONDEMAND:1;       /*!< bit:      7  On Demand Control                  */
    uint32_t RES2OUT:1;        /*!< bit:      8  Resistor ladder To Output          */
    uint32_t RES2VCC:1;        /*!< bit:      9  Resistor ladder To VCC             */
    uint32_t RES1EN:1;         /*!< bit:     10  Resistor 1 Enable                  */
    uint32_t RES1MUX:2;        /*!< bit: 11..12  Resistor 1 Mux                     */
    uint32_t POTMUX:3;         /*!< bit: 13..15  Potentiometer Selection            */
    uint32_t MUXPOS:3;         /*!< bit: 16..18  Positive Input Mux Selection       */
    uint32_t :1;               /*!< bit:     19  Reserved                           */
    uint32_t MUXNEG:3;         /*!< bit: 20..22  Negative Input Mux Selection       */
    uint32_t :9;               /*!< bit: 23..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} OPAMP_OPAMPCTRL1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define OPAMP_OPAMPCTRL1_OFFSET     0x08         /**< \brief (OPAMP_OPAMPCTRL1 offset) OPAMP Control 1 */
#define OPAMP_OPAMPCTRL1_RESETVALUE 0x00000000ul /**< \brief (OPAMP_OPAMPCTRL1 reset_value) OPAMP Control 1 */

#define OPAMP_OPAMPCTRL1_ENABLE_Pos 1            /**< \brief (OPAMP_OPAMPCTRL1) Operational Amplifier Enable */
#define OPAMP_OPAMPCTRL1_ENABLE     (0x1ul << OPAMP_OPAMPCTRL1_ENABLE_Pos)
#define OPAMP_OPAMPCTRL1_ANAOUT_Pos 2            /**< \brief (OPAMP_OPAMPCTRL1) Analog Output */
#define OPAMP_OPAMPCTRL1_ANAOUT     (0x1ul << OPAMP_OPAMPCTRL1_ANAOUT_Pos)
#define OPAMP_OPAMPCTRL1_BIAS_Pos   3            /**< \brief (OPAMP_OPAMPCTRL1) Bias Selection */
#define OPAMP_OPAMPCTRL1_BIAS_Msk   (0x3ul << OPAMP_OPAMPCTRL1_BIAS_Pos)
#define OPAMP_OPAMPCTRL1_BIAS(value) ((OPAMP_OPAMPCTRL1_BIAS_Msk & ((value) << OPAMP_OPAMPCTRL1_BIAS_Pos)))
#define OPAMP_OPAMPCTRL1_RUNSTDBY_Pos 6            /**< \brief (OPAMP_OPAMPCTRL1) Run in Standby */
#define OPAMP_OPAMPCTRL1_RUNSTDBY   (0x1ul << OPAMP_OPAMPCTRL1_RUNSTDBY_Pos)
#define OPAMP_OPAMPCTRL1_ONDEMAND_Pos 7            /**< \brief (OPAMP_OPAMPCTRL1) On Demand Control */
#define OPAMP_OPAMPCTRL1_ONDEMAND   (0x1ul << OPAMP_OPAMPCTRL1_ONDEMAND_Pos)
#define OPAMP_OPAMPCTRL1_RES2OUT_Pos 8            /**< \brief (OPAMP_OPAMPCTRL1) Resistor ladder To Output */
#define OPAMP_OPAMPCTRL1_RES2OUT    (0x1ul << OPAMP_OPAMPCTRL1_RES2OUT_Pos)
#define OPAMP_OPAMPCTRL1_RES2VCC_Pos 9            /**< \brief (OPAMP_OPAMPCTRL1) Resistor ladder To VCC */
#define OPAMP_OPAMPCTRL1_RES2VCC    (0x1ul << OPAMP_OPAMPCTRL1_RES2VCC_Pos)
#define OPAMP_OPAMPCTRL1_RES1EN_Pos 10           /**< \brief (OPAMP_OPAMPCTRL1) Resistor 1 Enable */
#define OPAMP_OPAMPCTRL1_RES1EN     (0x1ul << OPAMP_OPAMPCTRL1_RES1EN_Pos)
#define OPAMP_OPAMPCTRL1_RES1MUX_Pos 11           /**< \brief (OPAMP_OPAMPCTRL1) Resistor 1 Mux */
#define OPAMP_OPAMPCTRL1_RES1MUX_Msk (0x3ul << OPAMP_OPAMPCTRL1_RES1MUX_Pos)
#define OPAMP_OPAMPCTRL1_RES1MUX(value) ((OPAMP_OPAMPCTRL1_RES1MUX_Msk & ((value) << OPAMP_OPAMPCTRL1_RES1MUX_Pos)))
#define OPAMP_OPAMPCTRL1_POTMUX_Pos 13           /**< \brief (OPAMP_OPAMPCTRL1) Potentiometer Selection */
#define OPAMP_OPAMPCTRL1_POTMUX_Msk (0x7ul << OPAMP_OPAMPCTRL1_POTMUX_Pos)
#define OPAMP_OPAMPCTRL1_POTMUX(value) ((OPAMP_OPAMPCTRL1_POTMUX_Msk & ((value) << OPAMP_OPAMPCTRL1_POTMUX_Pos)))
#define OPAMP_OPAMPCTRL1_MUXPOS_Pos 16           /**< \brief (OPAMP_OPAMPCTRL1) Positive Input Mux Selection */
#define OPAMP_OPAMPCTRL1_MUXPOS_Msk (0x7ul << OPAMP_OPAMPCTRL1_MUXPOS_Pos)
#define OPAMP_OPAMPCTRL1_MUXPOS(value) ((OPAMP_OPAMPCTRL1_MUXPOS_Msk & ((value) << OPAMP_OPAMPCTRL1_MUXPOS_Pos)))
#define OPAMP_OPAMPCTRL1_MUXNEG_Pos 20           /**< \brief (OPAMP_OPAMPCTRL1) Negative Input Mux Selection */
#define OPAMP_OPAMPCTRL1_MUXNEG_Msk (0x7ul << OPAMP_OPAMPCTRL1_MUXNEG_Pos)
#define OPAMP_OPAMPCTRL1_MUXNEG(value) ((OPAMP_OPAMPCTRL1_MUXNEG_Msk & ((value) << OPAMP_OPAMPCTRL1_MUXNEG_Pos)))
#define OPAMP_OPAMPCTRL1_MASK       0x0077FFDEul /**< \brief (OPAMP_OPAMPCTRL1) MASK Register */

/* -------- OPAMP_OPAMPCTRL2 : (OPAMP Offset: 0x0C) (R/W 32) OPAMP Control 2 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t :1;               /*!< bit:      0  Reserved                           */
    uint32_t ENABLE:1;         /*!< bit:      1  Operational Amplifier Enable       */
    uint32_t ANAOUT:1;         /*!< bit:      2  Analog Output                      */
    uint32_t BIAS:2;           /*!< bit:  3.. 4  Bias Selection                     */
    uint32_t :1;               /*!< bit:      5  Reserved                           */
    uint32_t RUNSTDBY:1;       /*!< bit:      6  Run in Standby                     */
    uint32_t ONDEMAND:1;       /*!< bit:      7  On Demand Control                  */
    uint32_t RES2OUT:1;        /*!< bit:      8  Resistor ladder To Output          */
    uint32_t RES2VCC:1;        /*!< bit:      9  Resistor ladder To VCC             */
    uint32_t RES1EN:1;         /*!< bit:     10  Resistor 1 Enable                  */
    uint32_t RES1MUX:2;        /*!< bit: 11..12  Resistor 1 Mux                     */
    uint32_t POTMUX:3;         /*!< bit: 13..15  Potentiometer Selection            */
    uint32_t MUXPOS:3;         /*!< bit: 16..18  Positive Input Mux Selection       */
    uint32_t :1;               /*!< bit:     19  Reserved                           */
    uint32_t MUXNEG:3;         /*!< bit: 20..22  Negative Input Mux Selection       */
    uint32_t :9;               /*!< bit: 23..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} OPAMP_OPAMPCTRL2_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define OPAMP_OPAMPCTRL2_OFFSET     0x0C         /**< \brief (OPAMP_OPAMPCTRL2 offset) OPAMP Control 2 */
#define OPAMP_OPAMPCTRL2_RESETVALUE 0x00000000ul /**< \brief (OPAMP_OPAMPCTRL2 reset_value) OPAMP Control 2 */

#define OPAMP_OPAMPCTRL2_ENABLE_Pos 1            /**< \brief (OPAMP_OPAMPCTRL2) Operational Amplifier Enable */
#define OPAMP_OPAMPCTRL2_ENABLE     (0x1ul << OPAMP_OPAMPCTRL2_ENABLE_Pos)
#define OPAMP_OPAMPCTRL2_ANAOUT_Pos 2            /**< \brief (OPAMP_OPAMPCTRL2) Analog Output */
#define OPAMP_OPAMPCTRL2_ANAOUT     (0x1ul << OPAMP_OPAMPCTRL2_ANAOUT_Pos)
#define OPAMP_OPAMPCTRL2_BIAS_Pos   3            /**< \brief (OPAMP_OPAMPCTRL2) Bias Selection */
#define OPAMP_OPAMPCTRL2_BIAS_Msk   (0x3ul << OPAMP_OPAMPCTRL2_BIAS_Pos)
#define OPAMP_OPAMPCTRL2_BIAS(value) ((OPAMP_OPAMPCTRL2_BIAS_Msk & ((value) << OPAMP_OPAMPCTRL2_BIAS_Pos)))
#define OPAMP_OPAMPCTRL2_RUNSTDBY_Pos 6            /**< \brief (OPAMP_OPAMPCTRL2) Run in Standby */
#define OPAMP_OPAMPCTRL2_RUNSTDBY   (0x1ul << OPAMP_OPAMPCTRL2_RUNSTDBY_Pos)
#define OPAMP_OPAMPCTRL2_ONDEMAND_Pos 7            /**< \brief (OPAMP_OPAMPCTRL2) On Demand Control */
#define OPAMP_OPAMPCTRL2_ONDEMAND   (0x1ul << OPAMP_OPAMPCTRL2_ONDEMAND_Pos)
#define OPAMP_OPAMPCTRL2_RES2OUT_Pos 8            /**< \brief (OPAMP_OPAMPCTRL2) Resistor ladder To Output */
#define OPAMP_OPAMPCTRL2_RES2OUT    (0x1ul << OPAMP_OPAMPCTRL2_RES2OUT_Pos)
#define OPAMP_OPAMPCTRL2_RES2VCC_Pos 9            /**< \brief (OPAMP_OPAMPCTRL2) Resistor ladder To VCC */
#define OPAMP_OPAMPCTRL2_RES2VCC    (0x1ul << OPAMP_OPAMPCTRL2_RES2VCC_Pos)
#define OPAMP_OPAMPCTRL2_RES1EN_Pos 10           /**< \brief (OPAMP_OPAMPCTRL2) Resistor 1 Enable */
#define OPAMP_OPAMPCTRL2_RES1EN     (0x1ul << OPAMP_OPAMPCTRL2_RES1EN_Pos)
#define OPAMP_OPAMPCTRL2_RES1MUX_Pos 11           /**< \brief (OPAMP_OPAMPCTRL2) Resistor 1 Mux */
#define OPAMP_OPAMPCTRL2_RES1MUX_Msk (0x3ul << OPAMP_OPAMPCTRL2_RES1MUX_Pos)
#define OPAMP_OPAMPCTRL2_RES1MUX(value) ((OPAMP_OPAMPCTRL2_RES1MUX_Msk & ((value) << OPAMP_OPAMPCTRL2_RES1MUX_Pos)))
#define OPAMP_OPAMPCTRL2_POTMUX_Pos 13           /**< \brief (OPAMP_OPAMPCTRL2) Potentiometer Selection */
#define OPAMP_OPAMPCTRL2_POTMUX_Msk (0x7ul << OPAMP_OPAMPCTRL2_POTMUX_Pos)
#define OPAMP_OPAMPCTRL2_POTMUX(value) ((OPAMP_OPAMPCTRL2_POTMUX_Msk & ((value) << OPAMP_OPAMPCTRL2_POTMUX_Pos)))
#define OPAMP_OPAMPCTRL2_MUXPOS_Pos 16           /**< \brief (OPAMP_OPAMPCTRL2) Positive Input Mux Selection */
#define OPAMP_OPAMPCTRL2_MUXPOS_Msk (0x7ul << OPAMP_OPAMPCTRL2_MUXPOS_Pos)
#define OPAMP_OPAMPCTRL2_MUXPOS(value) ((OPAMP_OPAMPCTRL2_MUXPOS_Msk & ((value) << OPAMP_OPAMPCTRL2_MUXPOS_Pos)))
#define OPAMP_OPAMPCTRL2_MUXNEG_Pos 20           /**< \brief (OPAMP_OPAMPCTRL2) Negative Input Mux Selection */
#define OPAMP_OPAMPCTRL2_MUXNEG_Msk (0x7ul << OPAMP_OPAMPCTRL2_MUXNEG_Pos)
#define OPAMP_OPAMPCTRL2_MUXNEG(value) ((OPAMP_OPAMPCTRL2_MUXNEG_Msk & ((value) << OPAMP_OPAMPCTRL2_MUXNEG_Pos)))
#define OPAMP_OPAMPCTRL2_MASK       0x0077FFDEul /**< \brief (OPAMP_OPAMPCTRL2) MASK Register */

/** \brief OPAMP hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  __IO OPAMP_CTRLA_Type          CTRLA;       /**< \brief Offset: 0x00 (R/W  8) Control A */
       RoReg8                    Reserved1[0x1];
  __I  OPAMP_STATUS_Type         STATUS;      /**< \brief Offset: 0x02 (R/   8) Status */
       RoReg8                    Reserved2[0x1];
  __IO OPAMP_OPAMPCTRL0_Type     OPAMPCTRL0;  /**< \brief Offset: 0x04 (R/W 32) OPAMP Control 0 */
  __IO OPAMP_OPAMPCTRL1_Type     OPAMPCTRL1;  /**< \brief Offset: 0x08 (R/W 32) OPAMP Control 1 */
  __IO OPAMP_OPAMPCTRL2_Type     OPAMPCTRL2;  /**< \brief Offset: 0x0C (R/W 32) OPAMP Control 2 */
} Opamp;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAML21_OPAMP_COMPONENT_ */
