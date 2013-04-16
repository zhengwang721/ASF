/**
 * \file
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

#ifndef _SAMD20_PTC_COMPONENT_
#define _SAMD20_PTC_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR PTC */
/* ========================================================================== */
/** \addtogroup SAMD20_PTC Peripheral Touch Controller */
/*@{*/

#define REV_PTC                     0x101

/* -------- PTC_CTRLA : (PTC Offset: 0x00) (R/W  8) Control Register A -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint8_t  ENABLE:1;         /*!< bit:      1  Enable                             */
    uint8_t  RUNSTDBY:1;       /*!< bit:      2  Run during Standby                 */
    uint8_t  :5;               /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_CTRLA_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_CTRLA_OFFSET            0x00         /**< \brief (PTC_CTRLA offset) Control Register A */
#define PTC_CTRLA_RESETVALUE        0x00         /**< \brief (PTC_CTRLA reset_value) Control Register A */

#define PTC_CTRLA_SWRST_Pos         0            /**< \brief (PTC_CTRLA) Software Reset */
#define PTC_CTRLA_SWRST             (0x1u << PTC_CTRLA_SWRST_Pos)
#define PTC_CTRLA_ENABLE_Pos        1            /**< \brief (PTC_CTRLA) Enable */
#define PTC_CTRLA_ENABLE            (0x1u << PTC_CTRLA_ENABLE_Pos)
#define PTC_CTRLA_RUNSTDBY_Pos      2            /**< \brief (PTC_CTRLA) Run during Standby */
#define PTC_CTRLA_RUNSTDBY          (0x1u << PTC_CTRLA_RUNSTDBY_Pos)
#define PTC_CTRLA_MASK              0x07u        /**< \brief (PTC_CTRLA) MASK Register */

/* -------- PTC_STATUS : (PTC Offset: 0x01) (R/   8) Status Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  :7;               /*!< bit:  0.. 6  Reserved                           */
    uint8_t  SYNCBUSY:1;       /*!< bit:      7  Synchronization Busy               */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_STATUS_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_STATUS_OFFSET           0x01         /**< \brief (PTC_STATUS offset) Status Register */
#define PTC_STATUS_RESETVALUE       0x00         /**< \brief (PTC_STATUS reset_value) Status Register */

#define PTC_STATUS_SYNCBUSY_Pos     7            /**< \brief (PTC_STATUS) Synchronization Busy */
#define PTC_STATUS_SYNCBUSY         (0x1u << PTC_STATUS_SYNCBUSY_Pos)
#define PTC_STATUS_MASK             0x80u        /**< \brief (PTC_STATUS) MASK Register */

/* -------- PTC_CTRLB : (PTC Offset: 0x04) (R/W  8) Control Register B -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  PRSC:2;           /*!< bit:  0.. 1  Clock Prescaler                    */
    uint8_t  FREERUN:1;        /*!< bit:      2  Freerunning PTC                    */
    uint8_t  ADRS:1;           /*!< bit:      3  ADC Resolution Select              */
    uint8_t  :4;               /*!< bit:  4.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_CTRLB_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_CTRLB_OFFSET            0x04         /**< \brief (PTC_CTRLB offset) Control Register B */
#define PTC_CTRLB_RESETVALUE        0x00         /**< \brief (PTC_CTRLB reset_value) Control Register B */

#define PTC_CTRLB_PRSC_Pos          0            /**< \brief (PTC_CTRLB) Clock Prescaler */
#define PTC_CTRLB_PRSC_Msk          (0x3u << PTC_CTRLB_PRSC_Pos)
#define PTC_CTRLB_PRSC(value)       ((PTC_CTRLB_PRSC_Msk & ((value) << PTC_CTRLB_PRSC_Pos)))
#define PTC_CTRLB_FREERUN_Pos       2            /**< \brief (PTC_CTRLB) Freerunning PTC */
#define PTC_CTRLB_FREERUN           (0x1u << PTC_CTRLB_FREERUN_Pos)
#define PTC_CTRLB_ADRS_Pos          3            /**< \brief (PTC_CTRLB) ADC Resolution Select */
#define PTC_CTRLB_ADRS              (0x1u << PTC_CTRLB_ADRS_Pos)
#define PTC_CTRLB_MASK              0x0Fu        /**< \brief (PTC_CTRLB) MASK Register */

/* -------- PTC_EVCTRL : (PTC Offset: 0x05) (R/W  8) Event Control Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  STCONVEI:1;       /*!< bit:      0  Start Conversion Event Input       */
    uint8_t  EOCEO:1;          /*!< bit:      1  End of Conversion Event Output     */
    uint8_t  WCOMPEO:1;        /*!< bit:      2  Window Comparator Event Output     */
    uint8_t  :5;               /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_EVCTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_EVCTRL_OFFSET           0x05         /**< \brief (PTC_EVCTRL offset) Event Control Register */
#define PTC_EVCTRL_RESETVALUE       0x00         /**< \brief (PTC_EVCTRL reset_value) Event Control Register */

#define PTC_EVCTRL_STCONVEI_Pos     0            /**< \brief (PTC_EVCTRL) Start Conversion Event Input */
#define PTC_EVCTRL_STCONVEI         (0x1u << PTC_EVCTRL_STCONVEI_Pos)
#define PTC_EVCTRL_EOCEO_Pos        1            /**< \brief (PTC_EVCTRL) End of Conversion Event Output */
#define PTC_EVCTRL_EOCEO            (0x1u << PTC_EVCTRL_EOCEO_Pos)
#define PTC_EVCTRL_WCOMPEO_Pos      2            /**< \brief (PTC_EVCTRL) Window Comparator Event Output */
#define PTC_EVCTRL_WCOMPEO          (0x1u << PTC_EVCTRL_WCOMPEO_Pos)
#define PTC_EVCTRL_MASK             0x07u        /**< \brief (PTC_EVCTRL) MASK Register */

/* -------- PTC_INTENCLR : (PTC Offset: 0x08) (R/W  8) Interrupt Enable Clear Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  EOC:1;            /*!< bit:      0  End of Conversion Interrupt Disable */
    uint8_t  WCOMP:1;          /*!< bit:      1  Window Compare Interrupt Disable   */
    uint8_t  SYNCRDY:1;        /*!< bit:      2  Synchronization Ready Interrupt Disable */
    uint8_t  :5;               /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_INTENCLR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_INTENCLR_OFFSET         0x08         /**< \brief (PTC_INTENCLR offset) Interrupt Enable Clear Register */
#define PTC_INTENCLR_RESETVALUE     0x00         /**< \brief (PTC_INTENCLR reset_value) Interrupt Enable Clear Register */

#define PTC_INTENCLR_EOC_Pos        0            /**< \brief (PTC_INTENCLR) End of Conversion Interrupt Disable */
#define PTC_INTENCLR_EOC            (0x1u << PTC_INTENCLR_EOC_Pos)
#define PTC_INTENCLR_WCOMP_Pos      1            /**< \brief (PTC_INTENCLR) Window Compare Interrupt Disable */
#define PTC_INTENCLR_WCOMP          (0x1u << PTC_INTENCLR_WCOMP_Pos)
#define PTC_INTENCLR_SYNCRDY_Pos    2            /**< \brief (PTC_INTENCLR) Synchronization Ready Interrupt Disable */
#define PTC_INTENCLR_SYNCRDY        (0x1u << PTC_INTENCLR_SYNCRDY_Pos)
#define PTC_INTENCLR_MASK           0x07u        /**< \brief (PTC_INTENCLR) MASK Register */

/* -------- PTC_INTENSET : (PTC Offset: 0x09) (R/W  8) Interrupt Enable Set Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  EOC:1;            /*!< bit:      0  End of Conversion Interrupt Enable */
    uint8_t  WCOMP:1;          /*!< bit:      1  Window Compare Interrupt Enable    */
    uint8_t  SYNCRDY:1;        /*!< bit:      2  Synchronization Ready Interrupt Enable */
    uint8_t  :5;               /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_INTENSET_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_INTENSET_OFFSET         0x09         /**< \brief (PTC_INTENSET offset) Interrupt Enable Set Register */
#define PTC_INTENSET_RESETVALUE     0x00         /**< \brief (PTC_INTENSET reset_value) Interrupt Enable Set Register */

#define PTC_INTENSET_EOC_Pos        0            /**< \brief (PTC_INTENSET) End of Conversion Interrupt Enable */
#define PTC_INTENSET_EOC            (0x1u << PTC_INTENSET_EOC_Pos)
#define PTC_INTENSET_WCOMP_Pos      1            /**< \brief (PTC_INTENSET) Window Compare Interrupt Enable */
#define PTC_INTENSET_WCOMP          (0x1u << PTC_INTENSET_WCOMP_Pos)
#define PTC_INTENSET_SYNCRDY_Pos    2            /**< \brief (PTC_INTENSET) Synchronization Ready Interrupt Enable */
#define PTC_INTENSET_SYNCRDY        (0x1u << PTC_INTENSET_SYNCRDY_Pos)
#define PTC_INTENSET_MASK           0x07u        /**< \brief (PTC_INTENSET) MASK Register */

/* -------- PTC_INTFLAG : (PTC Offset: 0x0A) (R/W  8) Interrupt Flag Status and Clear Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  EOC:1;            /*!< bit:      0  End of Conversion Interrupt Flag   */
    uint8_t  WCOMP:1;          /*!< bit:      1  Window Compare Interrupt Flag      */
    uint8_t  SYNCRDY:1;        /*!< bit:      2  Synchronization Ready Interrupt Flag */
    uint8_t  :5;               /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_INTFLAG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_INTFLAG_OFFSET          0x0A         /**< \brief (PTC_INTFLAG offset) Interrupt Flag Status and Clear Register */
#define PTC_INTFLAG_RESETVALUE      0x00         /**< \brief (PTC_INTFLAG reset_value) Interrupt Flag Status and Clear Register */

#define PTC_INTFLAG_EOC_Pos         0            /**< \brief (PTC_INTFLAG) End of Conversion Interrupt Flag */
#define PTC_INTFLAG_EOC             (0x1u << PTC_INTFLAG_EOC_Pos)
#define PTC_INTFLAG_WCOMP_Pos       1            /**< \brief (PTC_INTFLAG) Window Compare Interrupt Flag */
#define PTC_INTFLAG_WCOMP           (0x1u << PTC_INTFLAG_WCOMP_Pos)
#define PTC_INTFLAG_SYNCRDY_Pos     2            /**< \brief (PTC_INTFLAG) Synchronization Ready Interrupt Flag */
#define PTC_INTFLAG_SYNCRDY         (0x1u << PTC_INTFLAG_SYNCRDY_Pos)
#define PTC_INTFLAG_MASK            0x07u        /**< \brief (PTC_INTFLAG) MASK Register */

/* -------- PTC_CTSCTRLA : (PTC Offset: 0x0C) (R/W  8) CTS Control Register A -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  SDS:4;            /*!< bit:  0.. 3  Sampling Delay Selection           */
    uint8_t  ASDV:1;           /*!< bit:      4  Automatic Sampling Delay Variation */
    uint8_t  CCDS:2;           /*!< bit:  5.. 6  Channel Change Delay Selection     */
    uint8_t  :1;               /*!< bit:      7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_CTSCTRLA_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_CTSCTRLA_OFFSET         0x0C         /**< \brief (PTC_CTSCTRLA offset) CTS Control Register A */
#define PTC_CTSCTRLA_RESETVALUE     0x00         /**< \brief (PTC_CTSCTRLA reset_value) CTS Control Register A */

#define PTC_CTSCTRLA_SDS_Pos        0            /**< \brief (PTC_CTSCTRLA) Sampling Delay Selection */
#define PTC_CTSCTRLA_SDS_Msk        (0xFu << PTC_CTSCTRLA_SDS_Pos)
#define PTC_CTSCTRLA_SDS(value)     ((PTC_CTSCTRLA_SDS_Msk & ((value) << PTC_CTSCTRLA_SDS_Pos)))
#define PTC_CTSCTRLA_ASDV_Pos       4            /**< \brief (PTC_CTSCTRLA) Automatic Sampling Delay Variation */
#define PTC_CTSCTRLA_ASDV           (0x1u << PTC_CTSCTRLA_ASDV_Pos)
#define PTC_CTSCTRLA_CCDS_Pos       5            /**< \brief (PTC_CTSCTRLA) Channel Change Delay Selection */
#define PTC_CTSCTRLA_CCDS_Msk       (0x3u << PTC_CTSCTRLA_CCDS_Pos)
#define PTC_CTSCTRLA_CCDS(value)    ((PTC_CTSCTRLA_CCDS_Msk & ((value) << PTC_CTSCTRLA_CCDS_Pos)))
#define PTC_CTSCTRLA_MASK           0x7Fu        /**< \brief (PTC_CTSCTRLA) MASK Register */

/* -------- PTC_CTSCTRLC : (PTC Offset: 0x0D) (R/W  8) CTS Control Register C -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  ADAS:3;           /*!< bit:  0.. 2  ADC Accumulation Number Select     */
    uint8_t  :4;               /*!< bit:  3.. 6  Reserved                           */
    uint8_t  STCONV:1;         /*!< bit:      7  Start Conversion                   */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_CTSCTRLC_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_CTSCTRLC_OFFSET         0x0D         /**< \brief (PTC_CTSCTRLC offset) CTS Control Register C */
#define PTC_CTSCTRLC_RESETVALUE     0x00         /**< \brief (PTC_CTSCTRLC reset_value) CTS Control Register C */

#define PTC_CTSCTRLC_ADAS_Pos       0            /**< \brief (PTC_CTSCTRLC) ADC Accumulation Number Select */
#define PTC_CTSCTRLC_ADAS_Msk       (0x7u << PTC_CTSCTRLC_ADAS_Pos)
#define PTC_CTSCTRLC_ADAS(value)    ((PTC_CTSCTRLC_ADAS_Msk & ((value) << PTC_CTSCTRLC_ADAS_Pos)))
#define PTC_CTSCTRLC_STCONV_Pos     7            /**< \brief (PTC_CTSCTRLC) Start Conversion */
#define PTC_CTSCTRLC_STCONV         (0x1u << PTC_CTSCTRLC_STCONV_Pos)
#define PTC_CTSCTRLC_MASK           0x87u        /**< \brief (PTC_CTSCTRLC) MASK Register */

/* -------- PTC_YSEL : (PTC Offset: 0x10) (R/W 16) Y Line Select Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t YSEL:16;          /*!< bit:  0..15  Y Line Selection                   */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_YSEL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_YSEL_OFFSET             0x10         /**< \brief (PTC_YSEL offset) Y Line Select Register */
#define PTC_YSEL_RESETVALUE         0x0000       /**< \brief (PTC_YSEL reset_value) Y Line Select Register */

#define PTC_YSEL_YSEL_Pos           0            /**< \brief (PTC_YSEL) Y Line Selection */
#define PTC_YSEL_YSEL_Msk           (0xFFFFu << PTC_YSEL_YSEL_Pos)
#define PTC_YSEL_YSEL(value)        ((PTC_YSEL_YSEL_Msk & ((value) << PTC_YSEL_YSEL_Pos)))
#define PTC_YSEL_MASK               0xFFFFu      /**< \brief (PTC_YSEL) MASK Register */

/* -------- PTC_XSEL : (PTC Offset: 0x12) (R/W 16) X Line Select Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t XSEL:16;          /*!< bit:  0..15  X Line Selection                   */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_XSEL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_XSEL_OFFSET             0x12         /**< \brief (PTC_XSEL offset) X Line Select Register */
#define PTC_XSEL_RESETVALUE         0x0000       /**< \brief (PTC_XSEL reset_value) X Line Select Register */

#define PTC_XSEL_XSEL_Pos           0            /**< \brief (PTC_XSEL) X Line Selection */
#define PTC_XSEL_XSEL_Msk           (0xFFFFu << PTC_XSEL_XSEL_Pos)
#define PTC_XSEL_XSEL(value)        ((PTC_XSEL_XSEL_Msk & ((value) << PTC_XSEL_XSEL_Pos)))
#define PTC_XSEL_MASK               0xFFFFu      /**< \brief (PTC_XSEL) MASK Register */

/* -------- PTC_YEN : (PTC Offset: 0x14) (R/W 16) Y Line Enable Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t YEN:16;           /*!< bit:  0..15  Y Line Enable                      */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_YEN_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_YEN_OFFSET              0x14         /**< \brief (PTC_YEN offset) Y Line Enable Register */
#define PTC_YEN_RESETVALUE          0x0000       /**< \brief (PTC_YEN reset_value) Y Line Enable Register */

#define PTC_YEN_YEN_Pos             0            /**< \brief (PTC_YEN) Y Line Enable */
#define PTC_YEN_YEN_Msk             (0xFFFFu << PTC_YEN_YEN_Pos)
#define PTC_YEN_YEN(value)          ((PTC_YEN_YEN_Msk & ((value) << PTC_YEN_YEN_Pos)))
#define PTC_YEN_MASK                0xFFFFu      /**< \brief (PTC_YEN) MASK Register */

/* -------- PTC_XEN : (PTC Offset: 0x16) (R/W 16) X Line Enable Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t XEN:16;           /*!< bit:  0..15  X Line Enable                      */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_XEN_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_XEN_OFFSET              0x16         /**< \brief (PTC_XEN offset) X Line Enable Register */
#define PTC_XEN_RESETVALUE          0x0000       /**< \brief (PTC_XEN reset_value) X Line Enable Register */

#define PTC_XEN_XEN_Pos             0            /**< \brief (PTC_XEN) X Line Enable */
#define PTC_XEN_XEN_Msk             (0xFFFFu << PTC_XEN_XEN_Pos)
#define PTC_XEN_XEN(value)          ((PTC_XEN_XEN_Msk & ((value) << PTC_XEN_XEN_Pos)))
#define PTC_XEN_MASK                0xFFFFu      /**< \brief (PTC_XEN) MASK Register */

/* -------- PTC_CCCAL : (PTC Offset: 0x18) (R/W 16) CTS Compensation Capacitor Calibration Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t CCA:4;            /*!< bit:  0.. 3  Compensation Capacitor Accurate Value */
    uint16_t CCF:4;            /*!< bit:  4.. 7  Compensation Capacitor Fine Value  */
    uint16_t CCC:4;            /*!< bit:  8..11  Compensation Capacitor Coarse Value */
    uint16_t CCR:2;            /*!< bit: 12..13  Compensation Capacitor Rough Value */
    uint16_t :2;               /*!< bit: 14..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_CCCAL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_CCCAL_OFFSET            0x18         /**< \brief (PTC_CCCAL offset) CTS Compensation Capacitor Calibration Register */
#define PTC_CCCAL_RESETVALUE        0x0000       /**< \brief (PTC_CCCAL reset_value) CTS Compensation Capacitor Calibration Register */

#define PTC_CCCAL_CCA_Pos           0            /**< \brief (PTC_CCCAL) Compensation Capacitor Accurate Value */
#define PTC_CCCAL_CCA_Msk           (0xFu << PTC_CCCAL_CCA_Pos)
#define PTC_CCCAL_CCA(value)        ((PTC_CCCAL_CCA_Msk & ((value) << PTC_CCCAL_CCA_Pos)))
#define PTC_CCCAL_CCF_Pos           4            /**< \brief (PTC_CCCAL) Compensation Capacitor Fine Value */
#define PTC_CCCAL_CCF_Msk           (0xFu << PTC_CCCAL_CCF_Pos)
#define PTC_CCCAL_CCF(value)        ((PTC_CCCAL_CCF_Msk & ((value) << PTC_CCCAL_CCF_Pos)))
#define PTC_CCCAL_CCC_Pos           8            /**< \brief (PTC_CCCAL) Compensation Capacitor Coarse Value */
#define PTC_CCCAL_CCC_Msk           (0xFu << PTC_CCCAL_CCC_Pos)
#define PTC_CCCAL_CCC(value)        ((PTC_CCCAL_CCC_Msk & ((value) << PTC_CCCAL_CCC_Pos)))
#define PTC_CCCAL_CCR_Pos           12           /**< \brief (PTC_CCCAL) Compensation Capacitor Rough Value */
#define PTC_CCCAL_CCR_Msk           (0x3u << PTC_CCCAL_CCR_Pos)
#define PTC_CCCAL_CCR(value)        ((PTC_CCCAL_CCR_Msk & ((value) << PTC_CCCAL_CCR_Pos)))
#define PTC_CCCAL_MASK              0x3FFFu      /**< \brief (PTC_CCCAL) MASK Register */

/* -------- PTC_CICAL : (PTC Offset: 0x1A) (R/W  8) CTS Integrator Capacitor Calibration Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  CIF:4;            /*!< bit:  0.. 3  Integration Capacitor Fine Value   */
    uint8_t  CIC:2;            /*!< bit:  4.. 5  Integration Capacitor Coarse Value */
    uint8_t  :2;               /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_CICAL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_CICAL_OFFSET            0x1A         /**< \brief (PTC_CICAL offset) CTS Integrator Capacitor Calibration Register */
#define PTC_CICAL_RESETVALUE        0x00         /**< \brief (PTC_CICAL reset_value) CTS Integrator Capacitor Calibration Register */

#define PTC_CICAL_CIF_Pos           0            /**< \brief (PTC_CICAL) Integration Capacitor Fine Value */
#define PTC_CICAL_CIF_Msk           (0xFu << PTC_CICAL_CIF_Pos)
#define PTC_CICAL_CIF(value)        ((PTC_CICAL_CIF_Msk & ((value) << PTC_CICAL_CIF_Pos)))
#define PTC_CICAL_CIC_Pos           4            /**< \brief (PTC_CICAL) Integration Capacitor Coarse Value */
#define PTC_CICAL_CIC_Msk           (0x3u << PTC_CICAL_CIC_Pos)
#define PTC_CICAL_CIC(value)        ((PTC_CICAL_CIC_Msk & ((value) << PTC_CICAL_CIC_Pos)))
#define PTC_CICAL_MASK              0x3Fu        /**< \brief (PTC_CICAL) MASK Register */

/* -------- PTC_CTSRS : (PTC Offset: 0x1B) (R/W  8) CTS Resistor Selection Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  RSEL:2;           /*!< bit:  0.. 1  Resistor Select                    */
    uint8_t  :6;               /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_CTSRS_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_CTSRS_OFFSET            0x1B         /**< \brief (PTC_CTSRS offset) CTS Resistor Selection Register */
#define PTC_CTSRS_RESETVALUE        0x00         /**< \brief (PTC_CTSRS reset_value) CTS Resistor Selection Register */

#define PTC_CTSRS_RSEL_Pos          0            /**< \brief (PTC_CTSRS) Resistor Select */
#define PTC_CTSRS_RSEL_Msk          (0x3u << PTC_CTSRS_RSEL_Pos)
#define PTC_CTSRS_RSEL(value)       ((PTC_CTSRS_RSEL_Msk & ((value) << PTC_CTSRS_RSEL_Pos)))
#define PTC_CTSRS_MASK              0x03u        /**< \brief (PTC_CTSRS) MASK Register */

/* -------- PTC_RESULT : (PTC Offset: 0x1C) (R/W 16) Conversion Result Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t RES:16;           /*!< bit:  0..15  Result                             */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_RESULT_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_RESULT_OFFSET           0x1C         /**< \brief (PTC_RESULT offset) Conversion Result Register */
#define PTC_RESULT_RESETVALUE       0x0000       /**< \brief (PTC_RESULT reset_value) Conversion Result Register */

#define PTC_RESULT_RES_Pos          0            /**< \brief (PTC_RESULT) Result */
#define PTC_RESULT_RES_Msk          (0xFFFFu << PTC_RESULT_RES_Pos)
#define PTC_RESULT_RES(value)       ((PTC_RESULT_RES_Msk & ((value) << PTC_RESULT_RES_Pos)))
#define PTC_RESULT_MASK             0xFFFFu      /**< \brief (PTC_RESULT) MASK Register */

/* -------- PTC_CTSCTRLB : (PTC Offset: 0x20) (R/W  8) CTS Control Register B -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  :2;               /*!< bit:  0.. 1  Reserved                           */
    uint8_t  CTSLP:1;          /*!< bit:      2  CTS Low Power Mode                 */
    uint8_t  :1;               /*!< bit:      3  Reserved                           */
    uint8_t  CTSM:4;           /*!< bit:  4.. 7  CTS Mode                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_CTSCTRLB_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_CTSCTRLB_OFFSET         0x20         /**< \brief (PTC_CTSCTRLB offset) CTS Control Register B */
#define PTC_CTSCTRLB_RESETVALUE     0x00         /**< \brief (PTC_CTSCTRLB reset_value) CTS Control Register B */

#define PTC_CTSCTRLB_CTSLP_Pos      2            /**< \brief (PTC_CTSCTRLB) CTS Low Power Mode */
#define PTC_CTSCTRLB_CTSLP          (0x1u << PTC_CTSCTRLB_CTSLP_Pos)
#define PTC_CTSCTRLB_CTSM_Pos       4            /**< \brief (PTC_CTSCTRLB) CTS Mode */
#define PTC_CTSCTRLB_CTSM_Msk       (0xFu << PTC_CTSCTRLB_CTSM_Pos)
#define PTC_CTSCTRLB_CTSM(value)    ((PTC_CTSCTRLB_CTSM_Msk & ((value) << PTC_CTSCTRLB_CTSM_Pos)))
#define PTC_CTSCTRLB_MASK           0xF4u        /**< \brief (PTC_CTSCTRLB) MASK Register */

/* -------- PTC_WINCCTRL : (PTC Offset: 0x21) (R/W  8) Window Comparator Control Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  WINCM:3;          /*!< bit:  0.. 2  Window Comparator Mode             */
    uint8_t  :5;               /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} PTC_WINCCTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_WINCCTRL_OFFSET         0x21         /**< \brief (PTC_WINCCTRL offset) Window Comparator Control Register */
#define PTC_WINCCTRL_RESETVALUE     0x00         /**< \brief (PTC_WINCCTRL reset_value) Window Comparator Control Register */

#define PTC_WINCCTRL_WINCM_Pos      0            /**< \brief (PTC_WINCCTRL) Window Comparator Mode */
#define PTC_WINCCTRL_WINCM_Msk      (0x7u << PTC_WINCCTRL_WINCM_Pos)
#define PTC_WINCCTRL_WINCM(value)   ((PTC_WINCCTRL_WINCM_Msk & ((value) << PTC_WINCCTRL_WINCM_Pos)))
#define PTC_WINCCTRL_MASK           0x07u        /**< \brief (PTC_WINCCTRL) MASK Register */

/* -------- PTC_WCHT : (PTC Offset: 0x24) (R/W 16) Window Comparator High Threshold Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t WCHT:16;          /*!< bit:  0..15  Window Comparator High Threshold   */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_WCHT_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_WCHT_OFFSET             0x24         /**< \brief (PTC_WCHT offset) Window Comparator High Threshold Register */
#define PTC_WCHT_RESETVALUE         0x0000       /**< \brief (PTC_WCHT reset_value) Window Comparator High Threshold Register */

#define PTC_WCHT_WCHT_Pos           0            /**< \brief (PTC_WCHT) Window Comparator High Threshold */
#define PTC_WCHT_WCHT_Msk           (0xFFFFu << PTC_WCHT_WCHT_Pos)
#define PTC_WCHT_WCHT(value)        ((PTC_WCHT_WCHT_Msk & ((value) << PTC_WCHT_WCHT_Pos)))
#define PTC_WCHT_MASK               0xFFFFu      /**< \brief (PTC_WCHT) MASK Register */

/* -------- PTC_WCLT : (PTC Offset: 0x26) (R/W 16) Window Comparator Low Threshold Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t WCLT:16;          /*!< bit:  0..15  Window Comparator Low Threshold    */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} PTC_WCLT_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define PTC_WCLT_OFFSET             0x26         /**< \brief (PTC_WCLT offset) Window Comparator Low Threshold Register */
#define PTC_WCLT_RESETVALUE         0x0000       /**< \brief (PTC_WCLT reset_value) Window Comparator Low Threshold Register */

#define PTC_WCLT_WCLT_Pos           0            /**< \brief (PTC_WCLT) Window Comparator Low Threshold */
#define PTC_WCLT_WCLT_Msk           (0xFFFFu << PTC_WCLT_WCLT_Pos)
#define PTC_WCLT_WCLT(value)        ((PTC_WCLT_WCLT_Msk & ((value) << PTC_WCLT_WCLT_Pos)))
#define PTC_WCLT_MASK               0xFFFFu      /**< \brief (PTC_WCLT) MASK Register */

/** \brief PTC hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  __IO PTC_CTRLA_Type            CTRLA;       /**< \brief Offset: 0x00 (R/W  8) Control Register A */
  __I  PTC_STATUS_Type           STATUS;      /**< \brief Offset: 0x01 (R/   8) Status Register */
       RoReg8                    Reserved1[0x2];
  __IO PTC_CTRLB_Type            CTRLB;       /**< \brief Offset: 0x04 (R/W  8) Control Register B */
  __IO PTC_EVCTRL_Type           EVCTRL;      /**< \brief Offset: 0x05 (R/W  8) Event Control Register */
       RoReg8                    Reserved2[0x2];
  __IO PTC_INTENCLR_Type         INTENCLR;    /**< \brief Offset: 0x08 (R/W  8) Interrupt Enable Clear Register */
  __IO PTC_INTENSET_Type         INTENSET;    /**< \brief Offset: 0x09 (R/W  8) Interrupt Enable Set Register */
  __IO PTC_INTFLAG_Type          INTFLAG;     /**< \brief Offset: 0x0A (R/W  8) Interrupt Flag Status and Clear Register */
       RoReg8                    Reserved3[0x1];
  __IO PTC_CTSCTRLA_Type         CTSCTRLA;    /**< \brief Offset: 0x0C (R/W  8) CTS Control Register A */
  __IO PTC_CTSCTRLC_Type         CTSCTRLC;    /**< \brief Offset: 0x0D (R/W  8) CTS Control Register C */
       RoReg8                    Reserved4[0x2];
  __IO PTC_YSEL_Type             YSEL;        /**< \brief Offset: 0x10 (R/W 16) Y Line Select Register */
  __IO PTC_XSEL_Type             XSEL;        /**< \brief Offset: 0x12 (R/W 16) X Line Select Register */
  __IO PTC_YEN_Type              YEN;         /**< \brief Offset: 0x14 (R/W 16) Y Line Enable Register */
  __IO PTC_XEN_Type              XEN;         /**< \brief Offset: 0x16 (R/W 16) X Line Enable Register */
  __IO PTC_CCCAL_Type            CCCAL;       /**< \brief Offset: 0x18 (R/W 16) CTS Compensation Capacitor Calibration Register */
  __IO PTC_CICAL_Type            CICAL;       /**< \brief Offset: 0x1A (R/W  8) CTS Integrator Capacitor Calibration Register */
  __IO PTC_CTSRS_Type            CTSRS;       /**< \brief Offset: 0x1B (R/W  8) CTS Resistor Selection Register */
  __IO PTC_RESULT_Type           RESULT;      /**< \brief Offset: 0x1C (R/W 16) Conversion Result Register */
       RoReg8                    Reserved5[0x2];
  __IO PTC_CTSCTRLB_Type         CTSCTRLB;    /**< \brief Offset: 0x20 (R/W  8) CTS Control Register B */
  __IO PTC_WINCCTRL_Type         WINCCTRL;    /**< \brief Offset: 0x21 (R/W  8) Window Comparator Control Register */
       RoReg8                    Reserved6[0x2];
  __IO PTC_WCHT_Type             WCHT;        /**< \brief Offset: 0x24 (R/W 16) Window Comparator High Threshold Register */
  __IO PTC_WCLT_Type             WCLT;        /**< \brief Offset: 0x26 (R/W 16) Window Comparator Low Threshold Register */
} Ptc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAMD20_PTC_COMPONENT_ */
