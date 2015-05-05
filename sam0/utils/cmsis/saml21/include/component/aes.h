/**
 * \file
 *
 * \brief Component description for AES
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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

#ifndef _SAML21_AES_COMPONENT_
#define _SAML21_AES_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR AES */
/* ========================================================================== */
/** \addtogroup SAML21_AES Advanced Encryption Standard */
/*@{*/

#define AES_U2238
#define REV_AES                     0x100

/* -------- AES_CTRLA : (AES Offset: 0x00) (R/W 32) Control A -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint32_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint32_t AESMODE:3;        /*!< bit:  2.. 4  AES Modes of operation             */
    uint32_t CFBS:3;           /*!< bit:  5.. 7  CFB Types                          */
    uint32_t KEYSIZE:2;        /*!< bit:  8.. 9  Keysize                            */
    uint32_t CIPHER:1;         /*!< bit:     10  Cipher mode                        */
    uint32_t STARTMODE:1;      /*!< bit:     11  Start mode                         */
    uint32_t LOD:1;            /*!< bit:     12  LOD Enable                         */
    uint32_t KEYGEN:1;         /*!< bit:     13  Last key generation                */
    uint32_t XORKEY:1;         /*!< bit:     14  Xor Key operation                  */
    uint32_t :1;               /*!< bit:     15  Reserved                           */
    uint32_t CTYPE:4;          /*!< bit: 16..19  Counter measure types              */
    uint32_t :12;              /*!< bit: 20..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} AES_CTRLA_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_CTRLA_OFFSET            0x00         /**< \brief (AES_CTRLA offset) Control A */
#define AES_CTRLA_RESETVALUE        0x00000000ul /**< \brief (AES_CTRLA reset_value) Control A */

#define AES_CTRLA_SWRST_Pos         0            /**< \brief (AES_CTRLA) Software Reset */
#define AES_CTRLA_SWRST             (0x1ul << AES_CTRLA_SWRST_Pos)
#define AES_CTRLA_ENABLE_Pos        1            /**< \brief (AES_CTRLA) Enable */
#define AES_CTRLA_ENABLE            (0x1ul << AES_CTRLA_ENABLE_Pos)
#define AES_CTRLA_AESMODE_Pos       2            /**< \brief (AES_CTRLA) AES Modes of operation */
#define AES_CTRLA_AESMODE_Msk       (0x7ul << AES_CTRLA_AESMODE_Pos)
#define AES_CTRLA_AESMODE(value)    (AES_CTRLA_AESMODE_Msk & ((value) << AES_CTRLA_AESMODE_Pos))
#define AES_CTRLA_CFBS_Pos          5            /**< \brief (AES_CTRLA) CFB Types */
#define AES_CTRLA_CFBS_Msk          (0x7ul << AES_CTRLA_CFBS_Pos)
#define AES_CTRLA_CFBS(value)       (AES_CTRLA_CFBS_Msk & ((value) << AES_CTRLA_CFBS_Pos))
#define AES_CTRLA_KEYSIZE_Pos       8            /**< \brief (AES_CTRLA) Keysize */
#define AES_CTRLA_KEYSIZE_Msk       (0x3ul << AES_CTRLA_KEYSIZE_Pos)
#define AES_CTRLA_KEYSIZE(value)    (AES_CTRLA_KEYSIZE_Msk & ((value) << AES_CTRLA_KEYSIZE_Pos))
#define AES_CTRLA_CIPHER_Pos        10           /**< \brief (AES_CTRLA) Cipher mode */
#define AES_CTRLA_CIPHER            (0x1ul << AES_CTRLA_CIPHER_Pos)
#define AES_CTRLA_STARTMODE_Pos     11           /**< \brief (AES_CTRLA) Start mode */
#define AES_CTRLA_STARTMODE         (0x1ul << AES_CTRLA_STARTMODE_Pos)
#define AES_CTRLA_LOD_Pos           12           /**< \brief (AES_CTRLA) LOD Enable */
#define AES_CTRLA_LOD               (0x1ul << AES_CTRLA_LOD_Pos)
#define AES_CTRLA_KEYGEN_Pos        13           /**< \brief (AES_CTRLA) Last key generation */
#define AES_CTRLA_KEYGEN            (0x1ul << AES_CTRLA_KEYGEN_Pos)
#define AES_CTRLA_XORKEY_Pos        14           /**< \brief (AES_CTRLA) Xor Key operation */
#define AES_CTRLA_XORKEY            (0x1ul << AES_CTRLA_XORKEY_Pos)
#define AES_CTRLA_CTYPE_Pos         16           /**< \brief (AES_CTRLA) Counter measure types */
#define AES_CTRLA_CTYPE_Msk         (0xFul << AES_CTRLA_CTYPE_Pos)
#define AES_CTRLA_CTYPE(value)      (AES_CTRLA_CTYPE_Msk & ((value) << AES_CTRLA_CTYPE_Pos))
#define AES_CTRLA_MASK              0x000F7FFFul /**< \brief (AES_CTRLA) MASK Register */

/* -------- AES_CTRLB : (AES Offset: 0x04) (R/W  8) Control B -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  START:1;          /*!< bit:      0  Manual Start                       */
    uint8_t  NEWMSG:1;         /*!< bit:      1  New message                        */
    uint8_t  EOM:1;            /*!< bit:      2  End of message                     */
    uint8_t  GFMUL:1;          /*!< bit:      3  GF Multiplication                  */
    uint8_t  :4;               /*!< bit:  4.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AES_CTRLB_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_CTRLB_OFFSET            0x04         /**< \brief (AES_CTRLB offset) Control B */
#define AES_CTRLB_RESETVALUE        0x00ul       /**< \brief (AES_CTRLB reset_value) Control B */

#define AES_CTRLB_START_Pos         0            /**< \brief (AES_CTRLB) Manual Start */
#define AES_CTRLB_START             (0x1ul << AES_CTRLB_START_Pos)
#define AES_CTRLB_NEWMSG_Pos        1            /**< \brief (AES_CTRLB) New message */
#define AES_CTRLB_NEWMSG            (0x1ul << AES_CTRLB_NEWMSG_Pos)
#define AES_CTRLB_EOM_Pos           2            /**< \brief (AES_CTRLB) End of message */
#define AES_CTRLB_EOM               (0x1ul << AES_CTRLB_EOM_Pos)
#define AES_CTRLB_GFMUL_Pos         3            /**< \brief (AES_CTRLB) GF Multiplication */
#define AES_CTRLB_GFMUL             (0x1ul << AES_CTRLB_GFMUL_Pos)
#define AES_CTRLB_MASK              0x0Ful       /**< \brief (AES_CTRLB) MASK Register */

/* -------- AES_INTENCLR : (AES Offset: 0x05) (R/W  8) Interrupt Enable Clear -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  ENCCMP:1;         /*!< bit:      0  Encryption Complete                */
    uint8_t  GFMCMP:1;         /*!< bit:      1  GF Multiplication Complete         */
    uint8_t  :6;               /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AES_INTENCLR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_INTENCLR_OFFSET         0x05         /**< \brief (AES_INTENCLR offset) Interrupt Enable Clear */
#define AES_INTENCLR_RESETVALUE     0x00ul       /**< \brief (AES_INTENCLR reset_value) Interrupt Enable Clear */

#define AES_INTENCLR_ENCCMP_Pos     0            /**< \brief (AES_INTENCLR) Encryption Complete */
#define AES_INTENCLR_ENCCMP         (0x1ul << AES_INTENCLR_ENCCMP_Pos)
#define   AES_INTENCLR_ENCCMP_0_Val       0x0ul  /**< \brief (AES_INTENCLR) 1 (no division) */
#define   AES_INTENCLR_ENCCMP_1_Val       0x1ul  /**< \brief (AES_INTENCLR) 2 */
#define   AES_INTENCLR_ENCCMP_2_Val       0x2ul  /**< \brief (AES_INTENCLR) 4 */
#define   AES_INTENCLR_ENCCMP_3_Val       0x3ul  /**< \brief (AES_INTENCLR) 8 */
#define AES_INTENCLR_ENCCMP_0       (AES_INTENCLR_ENCCMP_0_Val     << AES_INTENCLR_ENCCMP_Pos)
#define AES_INTENCLR_ENCCMP_1       (AES_INTENCLR_ENCCMP_1_Val     << AES_INTENCLR_ENCCMP_Pos)
#define AES_INTENCLR_ENCCMP_2       (AES_INTENCLR_ENCCMP_2_Val     << AES_INTENCLR_ENCCMP_Pos)
#define AES_INTENCLR_ENCCMP_3       (AES_INTENCLR_ENCCMP_3_Val     << AES_INTENCLR_ENCCMP_Pos)
#define AES_INTENCLR_GFMCMP_Pos     1            /**< \brief (AES_INTENCLR) GF Multiplication Complete */
#define AES_INTENCLR_GFMCMP         (0x1ul << AES_INTENCLR_GFMCMP_Pos)
#define AES_INTENCLR_MASK           0x03ul       /**< \brief (AES_INTENCLR) MASK Register */

/* -------- AES_INTENSET : (AES Offset: 0x06) (R/W  8) Interrupt Enable Set -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  ENCCMP:1;         /*!< bit:      0  Encryption Complete                */
    uint8_t  GFMCMP:1;         /*!< bit:      1  GF Multiplication Complete         */
    uint8_t  :6;               /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AES_INTENSET_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_INTENSET_OFFSET         0x06         /**< \brief (AES_INTENSET offset) Interrupt Enable Set */
#define AES_INTENSET_RESETVALUE     0x00ul       /**< \brief (AES_INTENSET reset_value) Interrupt Enable Set */

#define AES_INTENSET_ENCCMP_Pos     0            /**< \brief (AES_INTENSET) Encryption Complete */
#define AES_INTENSET_ENCCMP         (0x1ul << AES_INTENSET_ENCCMP_Pos)
#define AES_INTENSET_GFMCMP_Pos     1            /**< \brief (AES_INTENSET) GF Multiplication Complete */
#define AES_INTENSET_GFMCMP         (0x1ul << AES_INTENSET_GFMCMP_Pos)
#define AES_INTENSET_MASK           0x03ul       /**< \brief (AES_INTENSET) MASK Register */

/* -------- AES_INTFLAG : (AES Offset: 0x07) (R/W  8) Interrupt Flag Status -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { // __I to avoid read-modify-write on write-to-clear register
  struct {
    __I uint8_t  ENCCMP:1;         /*!< bit:      0  Encryption Complete                */
    __I uint8_t  GFMCMP:1;         /*!< bit:      1  GF Multiplication Complete         */
    __I uint8_t  :6;               /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AES_INTFLAG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_INTFLAG_OFFSET          0x07         /**< \brief (AES_INTFLAG offset) Interrupt Flag Status */
#define AES_INTFLAG_RESETVALUE      0x00ul       /**< \brief (AES_INTFLAG reset_value) Interrupt Flag Status */

#define AES_INTFLAG_ENCCMP_Pos      0            /**< \brief (AES_INTFLAG) Encryption Complete */
#define AES_INTFLAG_ENCCMP          (0x1ul << AES_INTFLAG_ENCCMP_Pos)
#define AES_INTFLAG_GFMCMP_Pos      1            /**< \brief (AES_INTFLAG) GF Multiplication Complete */
#define AES_INTFLAG_GFMCMP          (0x1ul << AES_INTFLAG_GFMCMP_Pos)
#define AES_INTFLAG_MASK            0x03ul       /**< \brief (AES_INTFLAG) MASK Register */

/* -------- AES_DATABUFPTR : (AES Offset: 0x08) (R/W  8) Data buffer pointer -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  INDATAPTR:2;      /*!< bit:  0.. 1  Input Data Pointer                 */
    uint8_t  :6;               /*!< bit:  2.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AES_DATABUFPTR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_DATABUFPTR_OFFSET       0x08         /**< \brief (AES_DATABUFPTR offset) Data buffer pointer */
#define AES_DATABUFPTR_RESETVALUE   0x00ul       /**< \brief (AES_DATABUFPTR reset_value) Data buffer pointer */

#define AES_DATABUFPTR_INDATAPTR_Pos 0            /**< \brief (AES_DATABUFPTR) Input Data Pointer */
#define AES_DATABUFPTR_INDATAPTR_Msk (0x3ul << AES_DATABUFPTR_INDATAPTR_Pos)
#define AES_DATABUFPTR_INDATAPTR(value) (AES_DATABUFPTR_INDATAPTR_Msk & ((value) << AES_DATABUFPTR_INDATAPTR_Pos))
#define AES_DATABUFPTR_MASK         0x03ul       /**< \brief (AES_DATABUFPTR) MASK Register */

/* -------- AES_DBGCTRL : (AES Offset: 0x09) ( /W  8) Debug control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  DBGRUN:1;         /*!< bit:      0  Debug Run                          */
    uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} AES_DBGCTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_DBGCTRL_OFFSET          0x09         /**< \brief (AES_DBGCTRL offset) Debug control */
#define AES_DBGCTRL_RESETVALUE      0x00ul       /**< \brief (AES_DBGCTRL reset_value) Debug control */

#define AES_DBGCTRL_DBGRUN_Pos      0            /**< \brief (AES_DBGCTRL) Debug Run */
#define AES_DBGCTRL_DBGRUN          (0x1ul << AES_DBGCTRL_DBGRUN_Pos)
#define AES_DBGCTRL_MASK            0x01ul       /**< \brief (AES_DBGCTRL) MASK Register */

/* -------- AES_KEYWORD0 : (AES Offset: 0x0C) ( /W 32) Keyword 0 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_KEYWORD0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_KEYWORD0_OFFSET         0x0C         /**< \brief (AES_KEYWORD0 offset) Keyword 0 */
#define AES_KEYWORD0_RESETVALUE     0x00000000ul /**< \brief (AES_KEYWORD0 reset_value) Keyword 0 */
#define AES_KEYWORD0_MASK           0xFFFFFFFFul /**< \brief (AES_KEYWORD0) MASK Register */

/* -------- AES_KEYWORD1 : (AES Offset: 0x10) ( /W 32) Keyword 1 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_KEYWORD1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_KEYWORD1_OFFSET         0x10         /**< \brief (AES_KEYWORD1 offset) Keyword 1 */
#define AES_KEYWORD1_RESETVALUE     0x00000000ul /**< \brief (AES_KEYWORD1 reset_value) Keyword 1 */
#define AES_KEYWORD1_MASK           0xFFFFFFFFul /**< \brief (AES_KEYWORD1) MASK Register */

/* -------- AES_KEYWORD2 : (AES Offset: 0x14) ( /W 32) Keyword 2 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_KEYWORD2_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_KEYWORD2_OFFSET         0x14         /**< \brief (AES_KEYWORD2 offset) Keyword 2 */
#define AES_KEYWORD2_RESETVALUE     0x00000000ul /**< \brief (AES_KEYWORD2 reset_value) Keyword 2 */
#define AES_KEYWORD2_MASK           0xFFFFFFFFul /**< \brief (AES_KEYWORD2) MASK Register */

/* -------- AES_KEYWORD3 : (AES Offset: 0x18) ( /W 32) Keyword 3 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_KEYWORD3_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_KEYWORD3_OFFSET         0x18         /**< \brief (AES_KEYWORD3 offset) Keyword 3 */
#define AES_KEYWORD3_RESETVALUE     0x00000000ul /**< \brief (AES_KEYWORD3 reset_value) Keyword 3 */
#define AES_KEYWORD3_MASK           0xFFFFFFFFul /**< \brief (AES_KEYWORD3) MASK Register */

/* -------- AES_KEYWORD4 : (AES Offset: 0x1C) ( /W 32) Keyword 4 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_KEYWORD4_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_KEYWORD4_OFFSET         0x1C         /**< \brief (AES_KEYWORD4 offset) Keyword 4 */
#define AES_KEYWORD4_RESETVALUE     0x00000000ul /**< \brief (AES_KEYWORD4 reset_value) Keyword 4 */
#define AES_KEYWORD4_MASK           0xFFFFFFFFul /**< \brief (AES_KEYWORD4) MASK Register */

/* -------- AES_KEYWORD5 : (AES Offset: 0x20) ( /W 32) Keyword 5 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_KEYWORD5_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_KEYWORD5_OFFSET         0x20         /**< \brief (AES_KEYWORD5 offset) Keyword 5 */
#define AES_KEYWORD5_RESETVALUE     0x00000000ul /**< \brief (AES_KEYWORD5 reset_value) Keyword 5 */
#define AES_KEYWORD5_MASK           0xFFFFFFFFul /**< \brief (AES_KEYWORD5) MASK Register */

/* -------- AES_KEYWORD6 : (AES Offset: 0x24) ( /W 32) Keyword 6 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_KEYWORD6_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_KEYWORD6_OFFSET         0x24         /**< \brief (AES_KEYWORD6 offset) Keyword 6 */
#define AES_KEYWORD6_RESETVALUE     0x00000000ul /**< \brief (AES_KEYWORD6 reset_value) Keyword 6 */
#define AES_KEYWORD6_MASK           0xFFFFFFFFul /**< \brief (AES_KEYWORD6) MASK Register */

/* -------- AES_KEYWORD7 : (AES Offset: 0x28) ( /W 32) Keyword 7 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_KEYWORD7_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_KEYWORD7_OFFSET         0x28         /**< \brief (AES_KEYWORD7 offset) Keyword 7 */
#define AES_KEYWORD7_RESETVALUE     0x00000000ul /**< \brief (AES_KEYWORD7 reset_value) Keyword 7 */
#define AES_KEYWORD7_MASK           0xFFFFFFFFul /**< \brief (AES_KEYWORD7) MASK Register */

/* -------- AES_INDATA : (AES Offset: 0x38) (R/W 32) Indata -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_INDATA_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_INDATA_OFFSET           0x38         /**< \brief (AES_INDATA offset) Indata */
#define AES_INDATA_RESETVALUE       0x00000000ul /**< \brief (AES_INDATA reset_value) Indata */
#define AES_INDATA_MASK             0xFFFFFFFFul /**< \brief (AES_INDATA) MASK Register */

/* -------- AES_INTVECTV0 : (AES Offset: 0x3C) ( /W 32) Initialisation Vector 0 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_INTVECTV0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_INTVECTV0_OFFSET        0x3C         /**< \brief (AES_INTVECTV0 offset) Initialisation Vector 0 */
#define AES_INTVECTV0_RESETVALUE    0x00000000ul /**< \brief (AES_INTVECTV0 reset_value) Initialisation Vector 0 */
#define AES_INTVECTV0_MASK          0xFFFFFFFFul /**< \brief (AES_INTVECTV0) MASK Register */

/* -------- AES_INTVECTV1 : (AES Offset: 0x40) ( /W 32) Initialisation Vector 1 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_INTVECTV1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_INTVECTV1_OFFSET        0x40         /**< \brief (AES_INTVECTV1 offset) Initialisation Vector 1 */
#define AES_INTVECTV1_RESETVALUE    0x00000000ul /**< \brief (AES_INTVECTV1 reset_value) Initialisation Vector 1 */
#define AES_INTVECTV1_MASK          0xFFFFFFFFul /**< \brief (AES_INTVECTV1) MASK Register */

/* -------- AES_INTVECTV2 : (AES Offset: 0x44) ( /W 32) Initialisation Vector 2 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_INTVECTV2_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_INTVECTV2_OFFSET        0x44         /**< \brief (AES_INTVECTV2 offset) Initialisation Vector 2 */
#define AES_INTVECTV2_RESETVALUE    0x00000000ul /**< \brief (AES_INTVECTV2 reset_value) Initialisation Vector 2 */
#define AES_INTVECTV2_MASK          0xFFFFFFFFul /**< \brief (AES_INTVECTV2) MASK Register */

/* -------- AES_INTVECTV3 : (AES Offset: 0x48) ( /W 32) Initialisation Vector 3 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_INTVECTV3_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_INTVECTV3_OFFSET        0x48         /**< \brief (AES_INTVECTV3 offset) Initialisation Vector 3 */
#define AES_INTVECTV3_RESETVALUE    0x00000000ul /**< \brief (AES_INTVECTV3 reset_value) Initialisation Vector 3 */
#define AES_INTVECTV3_MASK          0xFFFFFFFFul /**< \brief (AES_INTVECTV3) MASK Register */

/* -------- AES_HASHKEY0 : (AES Offset: 0x5C) (R/W 32) Hash key 0 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_HASHKEY0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_HASHKEY0_OFFSET         0x5C         /**< \brief (AES_HASHKEY0 offset) Hash key 0 */
#define AES_HASHKEY0_RESETVALUE     0x00000000ul /**< \brief (AES_HASHKEY0 reset_value) Hash key 0 */
#define AES_HASHKEY0_MASK           0xFFFFFFFFul /**< \brief (AES_HASHKEY0) MASK Register */

/* -------- AES_HASHKEY1 : (AES Offset: 0x60) (R/W 32) Hash key 1 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_HASHKEY1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_HASHKEY1_OFFSET         0x60         /**< \brief (AES_HASHKEY1 offset) Hash key 1 */
#define AES_HASHKEY1_RESETVALUE     0x00000000ul /**< \brief (AES_HASHKEY1 reset_value) Hash key 1 */
#define AES_HASHKEY1_MASK           0xFFFFFFFFul /**< \brief (AES_HASHKEY1) MASK Register */

/* -------- AES_HASHKEY2 : (AES Offset: 0x64) (R/W 32) Hash key 2 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_HASHKEY2_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_HASHKEY2_OFFSET         0x64         /**< \brief (AES_HASHKEY2 offset) Hash key 2 */
#define AES_HASHKEY2_RESETVALUE     0x00000000ul /**< \brief (AES_HASHKEY2 reset_value) Hash key 2 */
#define AES_HASHKEY2_MASK           0xFFFFFFFFul /**< \brief (AES_HASHKEY2) MASK Register */

/* -------- AES_HASHKEY3 : (AES Offset: 0x68) (R/W 32) Hash key 3 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_HASHKEY3_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_HASHKEY3_OFFSET         0x68         /**< \brief (AES_HASHKEY3 offset) Hash key 3 */
#define AES_HASHKEY3_RESETVALUE     0x00000000ul /**< \brief (AES_HASHKEY3 reset_value) Hash key 3 */
#define AES_HASHKEY3_MASK           0xFFFFFFFFul /**< \brief (AES_HASHKEY3) MASK Register */

/* -------- AES_GHASH0 : (AES Offset: 0x6C) (R/W 32) Galois Hash 0 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_GHASH0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_GHASH0_OFFSET           0x6C         /**< \brief (AES_GHASH0 offset) Galois Hash 0 */
#define AES_GHASH0_RESETVALUE       0x00000000ul /**< \brief (AES_GHASH0 reset_value) Galois Hash 0 */
#define AES_GHASH0_MASK             0xFFFFFFFFul /**< \brief (AES_GHASH0) MASK Register */

/* -------- AES_GHASH1 : (AES Offset: 0x70) (R/W 32) Galois Hash 1 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_GHASH1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_GHASH1_OFFSET           0x70         /**< \brief (AES_GHASH1 offset) Galois Hash 1 */
#define AES_GHASH1_RESETVALUE       0x00000000ul /**< \brief (AES_GHASH1 reset_value) Galois Hash 1 */
#define AES_GHASH1_MASK             0xFFFFFFFFul /**< \brief (AES_GHASH1) MASK Register */

/* -------- AES_GHASH2 : (AES Offset: 0x74) (R/W 32) Galois Hash 2 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_GHASH2_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_GHASH2_OFFSET           0x74         /**< \brief (AES_GHASH2 offset) Galois Hash 2 */
#define AES_GHASH2_RESETVALUE       0x00000000ul /**< \brief (AES_GHASH2 reset_value) Galois Hash 2 */
#define AES_GHASH2_MASK             0xFFFFFFFFul /**< \brief (AES_GHASH2) MASK Register */

/* -------- AES_GHASH3 : (AES Offset: 0x78) (R/W 32) Galois Hash 3 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_GHASH3_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_GHASH3_OFFSET           0x78         /**< \brief (AES_GHASH3 offset) Galois Hash 3 */
#define AES_GHASH3_RESETVALUE       0x00000000ul /**< \brief (AES_GHASH3 reset_value) Galois Hash 3 */
#define AES_GHASH3_MASK             0xFFFFFFFFul /**< \brief (AES_GHASH3) MASK Register */

/* -------- AES_CIPLEN : (AES Offset: 0x80) (R/W 32) Cipher Length -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_CIPLEN_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_CIPLEN_OFFSET           0x80         /**< \brief (AES_CIPLEN offset) Cipher Length */
#define AES_CIPLEN_RESETVALUE       0x00000000ul /**< \brief (AES_CIPLEN reset_value) Cipher Length */
#define AES_CIPLEN_MASK             0xFFFFFFFFul /**< \brief (AES_CIPLEN) MASK Register */

/* -------- AES_RANDSEED : (AES Offset: 0x84) (R/W 32) Random Seed -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} AES_RANDSEED_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AES_RANDSEED_OFFSET         0x84         /**< \brief (AES_RANDSEED offset) Random Seed */
#define AES_RANDSEED_RESETVALUE     0x00000000ul /**< \brief (AES_RANDSEED reset_value) Random Seed */
#define AES_RANDSEED_MASK           0xFFFFFFFFul /**< \brief (AES_RANDSEED) MASK Register */

/** \brief AES hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  __IO AES_CTRLA_Type            CTRLA;       /**< \brief Offset: 0x00 (R/W 32) Control A */
  __IO AES_CTRLB_Type            CTRLB;       /**< \brief Offset: 0x04 (R/W  8) Control B */
  __IO AES_INTENCLR_Type         INTENCLR;    /**< \brief Offset: 0x05 (R/W  8) Interrupt Enable Clear */
  __IO AES_INTENSET_Type         INTENSET;    /**< \brief Offset: 0x06 (R/W  8) Interrupt Enable Set */
  __IO AES_INTFLAG_Type          INTFLAG;     /**< \brief Offset: 0x07 (R/W  8) Interrupt Flag Status */
  __IO AES_DATABUFPTR_Type       DATABUFPTR;  /**< \brief Offset: 0x08 (R/W  8) Data buffer pointer */
  __O  AES_DBGCTRL_Type          DBGCTRL;     /**< \brief Offset: 0x09 ( /W  8) Debug control */
       RoReg8                    Reserved1[0x2];
  __O  AES_KEYWORD0_Type         KEYWORD0;    /**< \brief Offset: 0x0C ( /W 32) Keyword 0 */
  __O  AES_KEYWORD1_Type         KEYWORD1;    /**< \brief Offset: 0x10 ( /W 32) Keyword 1 */
  __O  AES_KEYWORD2_Type         KEYWORD2;    /**< \brief Offset: 0x14 ( /W 32) Keyword 2 */
  __O  AES_KEYWORD3_Type         KEYWORD3;    /**< \brief Offset: 0x18 ( /W 32) Keyword 3 */
  __O  AES_KEYWORD4_Type         KEYWORD4;    /**< \brief Offset: 0x1C ( /W 32) Keyword 4 */
  __O  AES_KEYWORD5_Type         KEYWORD5;    /**< \brief Offset: 0x20 ( /W 32) Keyword 5 */
  __O  AES_KEYWORD6_Type         KEYWORD6;    /**< \brief Offset: 0x24 ( /W 32) Keyword 6 */
  __O  AES_KEYWORD7_Type         KEYWORD7;    /**< \brief Offset: 0x28 ( /W 32) Keyword 7 */
       RoReg8                    Reserved2[0xC];
  __IO AES_INDATA_Type           INDATA;      /**< \brief Offset: 0x38 (R/W 32) Indata */
  __O  AES_INTVECTV0_Type        INTVECTV0;   /**< \brief Offset: 0x3C ( /W 32) Initialisation Vector 0 */
  __O  AES_INTVECTV1_Type        INTVECTV1;   /**< \brief Offset: 0x40 ( /W 32) Initialisation Vector 1 */
  __O  AES_INTVECTV2_Type        INTVECTV2;   /**< \brief Offset: 0x44 ( /W 32) Initialisation Vector 2 */
  __O  AES_INTVECTV3_Type        INTVECTV3;   /**< \brief Offset: 0x48 ( /W 32) Initialisation Vector 3 */
       RoReg8                    Reserved3[0x10];
  __IO AES_HASHKEY0_Type         HASHKEY0;    /**< \brief Offset: 0x5C (R/W 32) Hash key 0 */
  __IO AES_HASHKEY1_Type         HASHKEY1;    /**< \brief Offset: 0x60 (R/W 32) Hash key 1 */
  __IO AES_HASHKEY2_Type         HASHKEY2;    /**< \brief Offset: 0x64 (R/W 32) Hash key 2 */
  __IO AES_HASHKEY3_Type         HASHKEY3;    /**< \brief Offset: 0x68 (R/W 32) Hash key 3 */
  __IO AES_GHASH0_Type           GHASH0;      /**< \brief Offset: 0x6C (R/W 32) Galois Hash 0 */
  __IO AES_GHASH1_Type           GHASH1;      /**< \brief Offset: 0x70 (R/W 32) Galois Hash 1 */
  __IO AES_GHASH2_Type           GHASH2;      /**< \brief Offset: 0x74 (R/W 32) Galois Hash 2 */
  __IO AES_GHASH3_Type           GHASH3;      /**< \brief Offset: 0x78 (R/W 32) Galois Hash 3 */
       RoReg8                    Reserved4[0x4];
  __IO AES_CIPLEN_Type           CIPLEN;      /**< \brief Offset: 0x80 (R/W 32) Cipher Length */
  __IO AES_RANDSEED_Type         RANDSEED;    /**< \brief Offset: 0x84 (R/W 32) Random Seed */
} Aes;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAML21_AES_COMPONENT_ */
