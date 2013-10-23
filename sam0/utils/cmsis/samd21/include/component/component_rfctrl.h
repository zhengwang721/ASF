/**
 * \file
 *
 * \brief Component description for RFCTRL
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

#ifndef _SAMD21_RFCTRL_COMPONENT_
#define _SAMD21_RFCTRL_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR RFCTRL */
/* ========================================================================== */
/** \addtogroup SAMD21_RFCTRL RF233 control module */
/*@{*/

#define RFCTRL_U2233
#define REV_RFCTRL                  0x100

/* -------- RFCTRL_FECFG : (RFCTRL Offset: 0x0) (R/W 16) Front-end control bus configuration register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t F0CFG:2;          /*!< bit:  0.. 1  Front-end control signal 0 configuration */
    uint16_t F1CFG:2;          /*!< bit:  2.. 3  Front-end control signal 1 configuration */
    uint16_t F2CFG:2;          /*!< bit:  4.. 5  Front-end control signal 2 configuration */
    uint16_t F3CFG:2;          /*!< bit:  6.. 7  Front-end control signal 3 configuration */
    uint16_t F4CFG:2;          /*!< bit:  8.. 9  Front-end control signal 4 configuration */
    uint16_t F5CFG:2;          /*!< bit: 10..11  Front-end control signal 5 configuration */
    uint16_t :4;               /*!< bit: 12..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RFCTRL_FECFG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RFCTRL_FECFG_OFFSET         0x0          /**< \brief (RFCTRL_FECFG offset) Front-end control bus configuration register */
#define RFCTRL_FECFG_RESETVALUE     0x0000       /**< \brief (RFCTRL_FECFG reset_value) Front-end control bus configuration register */

#define RFCTRL_FECFG_F0CFG_Pos      0            /**< \brief (RFCTRL_FECFG) Front-end control signal 0 configuration */
#define RFCTRL_FECFG_F0CFG_Msk      (0x3u << RFCTRL_FECFG_F0CFG_Pos)
#define RFCTRL_FECFG_F0CFG(value)   ((RFCTRL_FECFG_F0CFG_Msk & ((value) << RFCTRL_FECFG_F0CFG_Pos)))
#define RFCTRL_FECFG_F1CFG_Pos      2            /**< \brief (RFCTRL_FECFG) Front-end control signal 1 configuration */
#define RFCTRL_FECFG_F1CFG_Msk      (0x3u << RFCTRL_FECFG_F1CFG_Pos)
#define RFCTRL_FECFG_F1CFG(value)   ((RFCTRL_FECFG_F1CFG_Msk & ((value) << RFCTRL_FECFG_F1CFG_Pos)))
#define RFCTRL_FECFG_F2CFG_Pos      4            /**< \brief (RFCTRL_FECFG) Front-end control signal 2 configuration */
#define RFCTRL_FECFG_F2CFG_Msk      (0x3u << RFCTRL_FECFG_F2CFG_Pos)
#define RFCTRL_FECFG_F2CFG(value)   ((RFCTRL_FECFG_F2CFG_Msk & ((value) << RFCTRL_FECFG_F2CFG_Pos)))
#define RFCTRL_FECFG_F3CFG_Pos      6            /**< \brief (RFCTRL_FECFG) Front-end control signal 3 configuration */
#define RFCTRL_FECFG_F3CFG_Msk      (0x3u << RFCTRL_FECFG_F3CFG_Pos)
#define RFCTRL_FECFG_F3CFG(value)   ((RFCTRL_FECFG_F3CFG_Msk & ((value) << RFCTRL_FECFG_F3CFG_Pos)))
#define RFCTRL_FECFG_F4CFG_Pos      8            /**< \brief (RFCTRL_FECFG) Front-end control signal 4 configuration */
#define RFCTRL_FECFG_F4CFG_Msk      (0x3u << RFCTRL_FECFG_F4CFG_Pos)
#define RFCTRL_FECFG_F4CFG(value)   ((RFCTRL_FECFG_F4CFG_Msk & ((value) << RFCTRL_FECFG_F4CFG_Pos)))
#define RFCTRL_FECFG_F5CFG_Pos      10           /**< \brief (RFCTRL_FECFG) Front-end control signal 5 configuration */
#define RFCTRL_FECFG_F5CFG_Msk      (0x3u << RFCTRL_FECFG_F5CFG_Pos)
#define RFCTRL_FECFG_F5CFG(value)   ((RFCTRL_FECFG_F5CFG_Msk & ((value) << RFCTRL_FECFG_F5CFG_Pos)))
#define RFCTRL_FECFG_MASK           0x0FFFu      /**< \brief (RFCTRL_FECFG) MASK Register */

/* -------- RFCTRL_TEST : (RFCTRL Offset: 0x4) (R/W 16) Test Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t TEN:1;            /*!< bit:      0  Transparent Test Enable            */
    uint16_t :1;               /*!< bit:      1  Reserved                           */
    uint16_t DIG1CFG:2;        /*!< bit:  2.. 3  DIG1 Direction during test         */
    uint16_t DIG2CFG:2;        /*!< bit:  4.. 5  DIG2 Direction during test         */
    uint16_t DIG3CFG:2;        /*!< bit:  6.. 7  DIG3/TSTIP Direction during test   */
    uint16_t DIG4CFG:2;        /*!< bit:  8.. 9  DIG4/TSTIN Direction during test   */
    uint16_t TSTOPCFG:2;       /*!< bit: 10..11  TSTOP Direction during test        */
    uint16_t TSTONCFG:2;       /*!< bit: 12..13  TSTON Direction during test        */
    uint16_t :2;               /*!< bit: 14..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RFCTRL_TEST_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RFCTRL_TEST_OFFSET          0x4          /**< \brief (RFCTRL_TEST offset) Test Register */
#define RFCTRL_TEST_RESETVALUE      0x0000       /**< \brief (RFCTRL_TEST reset_value) Test Register */

#define RFCTRL_TEST_TEN_Pos         0            /**< \brief (RFCTRL_TEST) Transparent Test Enable */
#define RFCTRL_TEST_TEN             (0x1u << RFCTRL_TEST_TEN_Pos)
#define RFCTRL_TEST_DIG1CFG_Pos     2            /**< \brief (RFCTRL_TEST) DIG1 Direction during test */
#define RFCTRL_TEST_DIG1CFG_Msk     (0x3u << RFCTRL_TEST_DIG1CFG_Pos)
#define RFCTRL_TEST_DIG1CFG(value)  ((RFCTRL_TEST_DIG1CFG_Msk & ((value) << RFCTRL_TEST_DIG1CFG_Pos)))
#define RFCTRL_TEST_DIG2CFG_Pos     4            /**< \brief (RFCTRL_TEST) DIG2 Direction during test */
#define RFCTRL_TEST_DIG2CFG_Msk     (0x3u << RFCTRL_TEST_DIG2CFG_Pos)
#define RFCTRL_TEST_DIG2CFG(value)  ((RFCTRL_TEST_DIG2CFG_Msk & ((value) << RFCTRL_TEST_DIG2CFG_Pos)))
#define RFCTRL_TEST_DIG3CFG_Pos     6            /**< \brief (RFCTRL_TEST) DIG3/TSTIP Direction during test */
#define RFCTRL_TEST_DIG3CFG_Msk     (0x3u << RFCTRL_TEST_DIG3CFG_Pos)
#define RFCTRL_TEST_DIG3CFG(value)  ((RFCTRL_TEST_DIG3CFG_Msk & ((value) << RFCTRL_TEST_DIG3CFG_Pos)))
#define RFCTRL_TEST_DIG4CFG_Pos     8            /**< \brief (RFCTRL_TEST) DIG4/TSTIN Direction during test */
#define RFCTRL_TEST_DIG4CFG_Msk     (0x3u << RFCTRL_TEST_DIG4CFG_Pos)
#define RFCTRL_TEST_DIG4CFG(value)  ((RFCTRL_TEST_DIG4CFG_Msk & ((value) << RFCTRL_TEST_DIG4CFG_Pos)))
#define RFCTRL_TEST_TSTOPCFG_Pos    10           /**< \brief (RFCTRL_TEST) TSTOP Direction during test */
#define RFCTRL_TEST_TSTOPCFG_Msk    (0x3u << RFCTRL_TEST_TSTOPCFG_Pos)
#define RFCTRL_TEST_TSTOPCFG(value) ((RFCTRL_TEST_TSTOPCFG_Msk & ((value) << RFCTRL_TEST_TSTOPCFG_Pos)))
#define RFCTRL_TEST_TSTONCFG_Pos    12           /**< \brief (RFCTRL_TEST) TSTON Direction during test */
#define RFCTRL_TEST_TSTONCFG_Msk    (0x3u << RFCTRL_TEST_TSTONCFG_Pos)
#define RFCTRL_TEST_TSTONCFG(value) ((RFCTRL_TEST_TSTONCFG_Msk & ((value) << RFCTRL_TEST_TSTONCFG_Pos)))
#define RFCTRL_TEST_MASK            0x3FFDu      /**< \brief (RFCTRL_TEST) MASK Register */

/** \brief RFCTRL hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  __IO RFCTRL_FECFG_Type         FECFG;       /**< \brief Offset: 0x0 (R/W 16) Front-end control bus configuration register */
       RoReg8                    Reserved1[0x2];
  __IO RFCTRL_TEST_Type          TEST;        /**< \brief Offset: 0x4 (R/W 16) Test Register */
} Rfctrl;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAMD21_RFCTRL_COMPONENT_ */
