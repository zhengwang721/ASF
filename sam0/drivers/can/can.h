/**
 * \file
 *
 * \brief SAM Control Area Network (CAN) Low Level Driver
 * 
 * \note Since "The Control Area Network (CAN) performs communication according
 * to ISO 11898-1 (Bosch CAN specification 2.0 part A,B) and to Bosch CAN FD
 * specification V1.0", the driver is focus on the MAC layer and try to offer
 * the APIs which can be used by upper application layer. It's not comply with
 * the document "New API convention".
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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
/**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CAN_H_INCLUDED
#define CAN_H_INCLUDED

#include <compiler.h>
#include <system.h>
#include <conf_can.h>

/**
 * \name Message RAM 
 * @{
 */

/* -------- CAN_RX_ELEMENT_R0 : (CAN RX element: 0x00) (R/W 32) Rx Element R0 Configuration -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t ID:29;            /*!< bit:  0..28  Identifier           */
    uint32_t RTR:1;            /*!< bit:  29  Remote Transmission Request                    */
    uint32_t XTD:1;            /*!< bit:  30  Extended Identifier                           */
    uint32_t ESI:1;            /*!< bit:  31  Error State Indicator              */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} CAN_RX_ELEMENT_R0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define CAN_RX_ELEMENT_R0_ID_Pos          0
#define CAN_RX_ELEMENT_R0_ID_Msk          (0x1FFFFFFFul << CAN_RX_ELEMENT_R0_ID_Pos)
#define CAN_RX_ELEMENT_R0_ID(value)       ((CAN_RX_ELEMENT_R0_ID_Msk & ((value) << CAN_RX_ELEMENT_R0_ID_Pos)))
#define CAN_RX_ELEMENT_R0_RTR_Pos         29
#define CAN_RX_ELEMENT_R0_RTR             (0x1ul << CAN_RX_ELEMENT_R0_RTR_Pos)
#define CAN_RX_ELEMENT_R0_XTD_Pos         30
#define CAN_RX_ELEMENT_R0_XTD             (0x1ul << CAN_RX_ELEMENT_R0_XTD_Pos)
#define CAN_RX_ELEMENT_R0_ESI_Pos         31
#define CAN_RX_ELEMENT_R0_ESI             (0x1ul << CAN_RX_ELEMENT_R0_ESI_Pos)

/* -------- CAN_RX_ELEMENT_R1 : (CAN RX element: 0x01) (R/W 32) Rx Element R1 Configuration -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t RXTS:16;            /*!< bit:  0..15  Rx Timestamp           */
    uint32_t DLC:4;            /*!< bit:  16..19  Data Length Code                    */
    uint32_t BRS:1;            /*!< bit:  20  Bit Rate Switch                          */
    uint32_t FDF:1;            /*!< bit:  21  FD Format              */
	uint32_t :2;               /*!< bit: 22..23  Reserved*/
	uint32_t FIDX:7;            /*!< bit:  24..30  Filter Index                         */
    uint32_t ANMF:1;            /*!< bit:  31  Accepted Non-matching Frame             */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} CAN_RX_ELEMENT_R1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define CAN_RX_ELEMENT_R1_RXTS_Pos        0
#define CAN_RX_ELEMENT_R1_RXTS_Msk        (0xFFFFul << CAN_RX_ELEMENT_R1_RXTS_Pos)
#define CAN_RX_ELEMENT_R1_RXTS(value)     ((CAN_RX_ELEMENT_R1_RXTS_Msk & ((value) << CAN_RX_ELEMENT_R1_RXTS_Pos)))
#define CAN_RX_ELEMENT_R1_DLC_Pos         16
#define CAN_RX_ELEMENT_R1_DLC_Msk         (0xFul << CAN_RX_ELEMENT_R1_DLC_Pos)
#define CAN_RX_ELEMENT_R1_DLC(value)      ((CAN_RX_ELEMENT_R1_DLC_Msk & ((value) << CAN_RX_ELEMENT_R1_DLC_Pos)))
#define CAN_RX_ELEMENT_R1_BRS_Pos         20
#define CAN_RX_ELEMENT_R1_BRS             (0x1ul << CAN_RX_ELEMENT_R1_BRS_Pos)
#define CAN_RX_ELEMENT_R1_FDF_Pos         21
#define CAN_RX_ELEMENT_R1_FDF             (0x1ul << CAN_RX_ELEMENT_R1_FDF_Pos)
#define CAN_RX_ELEMENT_R1_FIDX_Pos        24
#define CAN_RX_ELEMENT_R1_FIDX_Msk        (0x7Ful << CAN_RX_ELEMENT_R1_FIDX_Pos)
#define CAN_RX_ELEMENT_R1_FIDX(value)     ((CAN_RX_ELEMENT_R1_FIDX_Msk & ((value) << CAN_RX_ELEMENT_R1_FIDX_Pos)))
#define CAN_RX_ELEMENT_R1_ANMF_Pos        31
#define CAN_RX_ELEMENT_R1_ANMF            (0x1ul << CAN_RX_ELEMENT_R1_ANMF_Pos)

/**
 * \brief CAN receive element structure.
 *
 *  Common element structure for receive buffer and FIFO.
 */
struct can_rx_element {
	__IO CAN_RX_ELEMENT_R0_Type R0;
	__IO CAN_RX_ELEMENT_R1_Type R1;
//question here: 64 is the max value, if it's depend on the config in RXESC
	uint8_t    data[CONF_CAN_RX_ELEMENT_DATA_SIZA];
};

/* -------- CAN_TX_ELEMENT_T0 : (CAN TX element: 0x00) (R/W 32) Tx Element T0 Configuration -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t ID:29;            /*!< bit:  0..28  Identifier           */
    uint32_t RTR:1;            /*!< bit:  29  Remote Transmission Request                    */
    uint32_t XTD:1;            /*!< bit:  30  Extended Identifier                           */
    uint32_t ESI:1;            /*!< bit:  31  Error State Indicator              */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} CAN_TX_ELEMENT_T0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define CAN_TX_ELEMENT_T0_ID_Pos          0
#define CAN_TX_ELEMENT_T0_ID_Msk          (0x1FFFFFFFul << CAN_TX_ELEMENT_T0_ID_Pos)
#define CAN_TX_ELEMENT_T0_ID(value)       ((CAN_TX_ELEMENT_T0_ID_Msk & ((value) << CAN_TX_ELEMENT_T0_ID_Pos)))
#define CAN_TX_ELEMENT_T0_RTR_Pos         29
#define CAN_TX_ELEMENT_T0_RTR             (0x1ul << CAN_TX_ELEMENT_T0_RTR_Pos)
#define CAN_TX_ELEMENT_T0_XTD_Pos         30
#define CAN_TX_ELEMENT_T0_XTD             (0x1ul << CAN_TX_ELEMENT_T0_XTD_Pos)
#define CAN_TX_ELEMENT_T0_ESI_Pos         31
#define CAN_TX_ELEMENT_T0_ESI             (0x1ul << CAN_TX_ELEMENT_T0_ESI_Pos)

/* -------- CAN_TX_ELEMENT_T1 : (CAN TX element: 0x01) (R/W 32) Tx Element T1 Configuration -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t :16;               /*!< bit:  0..15  Reserved           */
    uint32_t DLC:4;            /*!< bit:  16..19  Data Length Code                    */
    uint32_t BRS:1;            /*!< bit:  20  Bit Rate Switch                          */
    uint32_t FDF:1;            /*!< bit:  21  FD Format              */
	uint32_t :1;               /*!< bit: 22  Reserved*/
	uint32_t EFC:1;            /*!< bit:  23  Event FIFO Control              */
	uint32_t MM:8;            /*!< bit:  24..31  Message Marker                         */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} CAN_TX_ELEMENT_T1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define CAN_TX_ELEMENT_T1_DLC_Pos         16
#define CAN_TX_ELEMENT_T1_DLC_Msk         (0xFul << CAN_TX_ELEMENT_T1_DLC_Pos)
#define CAN_TX_ELEMENT_T1_DLC(value)      ((CAN_TX_ELEMENT_T1_DLC_Msk & ((value) << CAN_TX_ELEMENT_T1_DLC_Pos)))
#define CAN_TX_ELEMENT_T1_BRS_Pos         20
#define CAN_TX_ELEMENT_T1_BRS             (0x1ul << CAN_TX_ELEMENT_T1_BRS_Pos)
#define CAN_TX_ELEMENT_T1_FDF_Pos         21
#define CAN_TX_ELEMENT_T1_FDF             (0x1ul << CAN_TX_ELEMENT_T1_FDF_Pos)
#define CAN_TX_ELEMENT_T1_EFC_Pos         23
#define CAN_TX_ELEMENT_T1_EFC             (0x1ul << CAN_TX_ELEMENT_T1_EFC_Pos)
#define CAN_TX_ELEMENT_T1_MM_Pos          24
#define CAN_TX_ELEMENT_T1_MM_Msk          (0xFFul << CAN_TX_ELEMENT_T1_MM_Pos)
#define CAN_TX_ELEMENT_T1_MM(value)       ((CAN_TX_ELEMENT_T1_MM_Msk & ((value) << CAN_TX_ELEMENT_T1_MM_Pos)))

/**
 * \brief CAN transfer element structure.
 *
 *  Common element structure for transfer buffer and FIFO/QUEUE.
 */
struct can_tx_element {
	__IO CAN_TX_ELEMENT_T0_Type T0;
	__IO CAN_TX_ELEMENT_T1_Type T1;
//question here: 64 is the max value, if it's depend on the config in RXESC
	uint8_t    data[CONF_CAN_TX_ELEMENT_DATA_SIZA];
};

/* -------- CAN_TX_EVENT_ELEMENT_E0 : (CAN TX event element: 0x00) (R/W 32) Tx Event Element E0 Configuration -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t ID:29;            /*!< bit:  0..28  Identifier           */
    uint32_t RTR:1;            /*!< bit:  29  Remote Transmission Request                    */
    uint32_t XTD:1;            /*!< bit:  30  Extended Identifier                           */
    uint32_t ESI:1;            /*!< bit:  31  Error State Indicator              */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} CAN_TX_EVENT_ELEMENT_E0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define CAN_TX_EVENT_ELEMENT_E0_ID_Pos          0
#define CAN_TX_EVENT_ELEMENT_E0_ID_Msk          (0x1FFFFFFFul << CAN_TX_EVENT_ELEMENT_E0_ID_Pos)
#define CAN_TX_EVENT_ELEMENT_E0_ID(value)       ((CAN_TX_EVENT_ELEMENT_E0_ID_Msk & ((value) << CAN_TX_EVENT_ELEMENT_E0_ID_Pos)))
#define CAN_TX_EVENT_ELEMENT_E0_RTR_Pos         29
#define CAN_TX_EVENT_ELEMENT_E0_RTR             (0x1ul << CAN_TX_EVENT_ELEMENT_E0_RTR_Pos)
#define CAN_TX_EVENT_ELEMENT_E0_XTD_Pos         30
#define CAN_TX_EVENT_ELEMENT_E0_XTD             (0x1ul << CAN_TX_EVENT_ELEMENT_E0_XTD_Pos)
#define CAN_TX_EVENT_ELEMENT_E0_ESI_Pos         31
#define CAN_TX_EVENT_ELEMENT_E0_ESI             (0x1ul << CAN_TX_EVENT_ELEMENT_E0_ESI_Pos)

/* -------- CAN_TX_EVENT_ELEMENT_E1 : (CAN TX event element: 0x01) (R/W 32) Tx Event Element E1 Configuration -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t TXTS:16;            /*!< bit:  0..15  Tx Timestamp           */
    uint32_t DLC:4;            /*!< bit:  16..19  Data Length Code                    */
    uint32_t BRS:1;            /*!< bit:  20  Bit Rate Switch                          */
    uint32_t FDF:1;            /*!< bit:  21  FD Format              */
	uint32_t ET:2;            /*!< bit:  22..23  Event Type              */
	uint32_t MM:8;            /*!< bit:  24..31  Message Marker                         */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} CAN_TX_EVENT_ELEMENT_E1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define CAN_TX_EVENT_ELEMENT_E1_TXTS_Pos        0
#define CAN_TX_EVENT_ELEMENT_E1_TXTS_Msk        (0xFFFFul << CAN_TX_EVENT_ELEMENT_E1_TXTS_Pos)
#define CAN_TX_EVENT_ELEMENT_E1_TXTS(value)     ((CAN_TX_EVENT_ELEMENT_E1_TXTS_Msk & ((value) << CAN_TX_EVENT_ELEMENT_E1_TXTS_Pos)))
#define CAN_TX_EVENT_ELEMENT_E1_DLC_Pos         16
#define CAN_TX_EVENT_ELEMENT_E1_DLC_Msk         (0xFul << CAN_TX_EVENT_ELEMENT_E1_DLC_Pos)
#define CAN_TX_EVENT_ELEMENT_E1_DLC(value)      ((CAN_TX_EVENT_ELEMENT_E1_DLC_Msk & ((value) << CAN_TX_EVENT_ELEMENT_E1_DLC_Pos)))
#define CAN_TX_EVENT_ELEMENT_E1_BRS_Pos         20
#define CAN_TX_EVENT_ELEMENT_E1_BRS             (0x1ul << CAN_TX_EVENT_ELEMENT_E1_BRS_Pos)
#define CAN_TX_EVENT_ELEMENT_E1_FDF_Pos         21
#define CAN_TX_EVENT_ELEMENT_E1_FDF             (0x1ul << CAN_TX_EVENT_ELEMENT_E1_FDF_Pos)
#define CAN_TX_EVENT_ELEMENT_E1_ET_Pos          22
#define CAN_TX_EVENT_ELEMENT_E1_ET_Msk          (0x3ul << CAN_TX_EVENT_ELEMENT_E1_ET_Pos)
#define CAN_TX_EVENT_ELEMENT_E1_ET(value)       ((CAN_TX_EVENT_ELEMENT_E1_ET_Msk & ((value) << CAN_TX_EVENT_ELEMENT_E1_ET_Pos)))
#define CAN_TX_EVENT_ELEMENT_E1_MM_Pos          24
#define CAN_TX_EVENT_ELEMENT_E1_MM_Msk          (0xFFul << CAN_TX_EVENT_ELEMENT_E1_MM_Pos)
#define CAN_TX_EVENT_ELEMENT_E1_MM(value)       ((CAN_TX_EVENT_ELEMENT_E1_MM_Msk & ((value) << CAN_TX_EVENT_ELEMENT_E1_MM_Pos)))

/**
 * \brief CAN transfer event  FIFO element structure.
 *
 *  Common element structure for transfer event FIFO.
 */
struct can_tx_event_element {
	__IO CAN_TX_EVENT_ELEMENT_E0_Type E0;
	__IO CAN_TX_EVENT_ELEMENT_E1_Type E1;
};

/* -------- CAN_SD_MESSAGE_FILTER_ELEMENT_S0 : (CAN standard message ID filter element: 0x00) (R/W 32) Standard Message ID Filter Element S0 Configuration -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t SFID2:11;         /*!< bit:  0..10  Standard Filter ID 2           */
	uint32_t :5;               /*!< bit: 11..15  Reserved*/
	uint32_t SFID1:11;         /*!< bit:  16..26  Standard Filter ID 1           */
    uint32_t SFEC:3;            /*!< bit:  27..29  Standard Filter Element Configuration                */
    uint32_t SFT:2;            /*!< bit:  30..31  Standard Filter Type                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} CAN_SD_MESSAGE_FILTER_ELEMENT_S0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFID2_Pos          0
#define CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFID2_Msk          (0x7FFul << CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFID2_Pos)
#define CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFID2(value)       ((CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFID2_Msk & ((value) << CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFID2_Pos)))
#define CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFID1_Pos          16
#define CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFID1_Msk          (0x7FFul << CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFID1_Pos)
#define CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFID1(value)       ((CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFID1_Msk & ((value) << CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFID1_Pos)))
#define CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC_Pos           27
#define CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC_Msk           (0x7ul << CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC_Pos)
#define CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC(value)        ((CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC_Msk & ((value) << CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC_Pos)))
#define   CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC_DISABLE     CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC(0)
#define   CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC_STF0M       CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC(1)
#define   CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC_STF1M       CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC(2)
#define   CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC_REJECT      CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC(3)
#define   CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC_PRIORITY    CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC(4)
#define   CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC_PRIF0M      CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC(5)
#define   CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC_PRIF1M      CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC(6)
#define   CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC_STRXBUF     CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC(7)
#define CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFT_Pos            30
#define CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFT_Msk            (0x3ul << CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFT_Pos)
#define CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFT(value)         ((CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFT_Msk & ((value) << CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFT_Pos)))
#define   CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFT_RANGE          CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFT(0)
#define   CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFT_DUAL           CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFT(1)
#define   CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFT_CLASSIC        CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFT(2)

/**
 * \brief CAN standard message ID filter element structure.
 *
 *  Common element structure for standard message ID filter element.
 */
struct can_sd_message_filter_element {
	__IO CAN_SD_MESSAGE_FILTER_ELEMENT_S0_Type S0;
};

/* -------- CAN_ET_MESSAGE_FILTER_ELEMENT_F0 : (CAN extended message ID filter element: 0x00) (R/W 32) Extended Message ID Filter Element F0 Configuration -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
	uint32_t EFID1:29;         /*!< bit:  0..28  Extended Filter ID 1           */
    uint32_t EFEC:3;           /*!< bit:  29..31  Extended Filter Element Configuration                */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} CAN_ET_MESSAGE_FILTER_ELEMENT_F0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFID1_Pos          0
#define CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFID1_Msk          (0x1FFFFFFFul << CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFID1_Pos)
#define CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFID1(value)       ((CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFID1_Msk & ((value) << CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFID1_Pos)))
#define CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC_Pos           29
#define CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC_Msk           (0x7ul << CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC_Pos)
#define CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC(value)        ((CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC_Msk & ((value) << CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC_Pos)))
#  define CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC_DISABLE       CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC(0)
#  define CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC_STF0M         CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC(1)
#  define CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC_STF1M         CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC(2)
#  define CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC_REJECT        CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC(3)
#  define CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC_PRIORITY      CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC(4)
#  define CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC_PRIF0M        CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC(5)
#  define CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC_PRIF1M        CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC(6)
#  define CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC_STRXBUF       CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC(7)

/* -------- CAN_ET_MESSAGE_FILTER_ELEMENT_F1 : (CAN extended message ID filter element: 0x01) (R/W 32) Extended Message ID Filter Element F1 Configuration -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t EFID2:29;         /*!< bit:  0..28  Extended Filter ID 2           */
	uint32_t :1;               /*!< bit: 29  Reserved*/
    uint32_t EFT:2;            /*!< bit:  30..31  Extended Filter Type                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} CAN_ET_MESSAGE_FILTER_ELEMENT_F1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFID2_Pos          0
#define CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFID2_Msk          (0x1FFFFFFFul << CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFID2_Pos)
#define CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFID2(value)       ((CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFID2_Msk & ((value) << CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFID2_Pos)))
#define CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT_Pos            30
#define CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT_Msk            (0x3ul << CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT_Pos)
#define CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT(value)         ((CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT_Msk & ((value) << CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT_Pos)))
#define   CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT_RANGEM       CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT(0)
#define   CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT_DUAL         CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT(1)
#define   CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT_CLASSIC      CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT(2)
#define   CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT_RANGE        CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT(3)

/**
 * \brief CAN extended message ID filter element structure.
 *
 *  Common element structure for extended message ID filter element.
 */
struct can_et_message_filter_element {
	__IO CAN_ET_MESSAGE_FILTER_ELEMENT_F0_Type F0;
	__IO CAN_ET_MESSAGE_FILTER_ELEMENT_F1_Type F1;
};
/** @} */

/**
 * \defgroup asfdoc_sam0_can_group SAM0 Control Area Network (CAN) Low Level Driver
 *
 * This driver for SAM0 devices provides an low level interface for the configuration
 * and management of the device's Control Area Network functionality.
 * The scope of this specification is to define the MAC sublayer and a small part of the
 * LLC sublayer of the Data Link Layer as well a part of the Physical Layer and to describe
 * the consequences of the CAN protocol on the surrounding layers.
 *
 * The following peripherals are used by this module:
 *  - CAN (Control Area Network)
 *
 * The following devices can use this module:
 *  - SAM C21
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_sam0_can_prerequisites
 *  - \ref asfdoc_sam0_can_module_overview
 *  - \ref asfdoc_sam0_can_special_considerations
 *  - \ref asfdoc_sam0_can_extra_info
 *  - \ref asfdoc_sam0_can_examples
 *  - \ref asfdoc_sam0_can_api_overview
 *
 *
 * \section asfdoc_sam0_can_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_sam0_can_module_overview Module Overview
 *
 * This driver provides an interface for the Control Area Network Controller
 * functions on the device.
 *
 *
 * \section asfdoc_sam0_can_special_considerations Special Considerations
 *
 * There are no special considerations for this module.
 *
 *
 * \section asfdoc_sam0_can_extra_info Extra Information
 *
 * For extra information see \ref asfdoc_sam0_can_extra. This includes:
 *  - \ref asfdoc_sam0_can_extra_acronyms
 *  - \ref asfdoc_sam0_can_extra_dependencies
 *  - \ref asfdoc_sam0_can_extra_errata
 *  - \ref asfdoc_sam0_can_extra_history
 *
 *
 * \section asfdoc_sam0_can_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_sam0_can_exqsg.
 *
 *
 * \section asfdoc_sam0_can_api_overview API Overview
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name Module setting 
 * @{
 */

/**
 * \brief Can module operation modes.
 *
 */
enum can_mode {
	/** Software Initialization Mode. */
	CAN_MODE_SOFTWARE_INITIALIZATION,
	/** Normal Operation Mode. */
	CAN_MODE_NORMAL_OPERATION,
	/** CAN FD Operation Mode. */
	CAN_MODE_FD_OPERATION,
	/** Restricted Operation Mode. */
	CAN_MODE_RESTRICTED_OPERATION,
	/** Bus Monitoring Mode. */
	CAN_MODE_BUS_MONITOR,
	/** Sleep Mode. */
	CAN_MODE_SLEEP,
	/** Test Mode. */
	CAN_MODE_TEST,
};

/**
 * \brief Can time out modes.
 */
enum can_timeout_mode {
	/** Continuout operation. */
	CAN_TIMEOUT_CONTINUES = CAN_TOCC_TOS_CONT,
	/** Timeout controlled by TX Event FIFO. */
	CAN_TIMEOUT_TX_EVEN_FIFO = CAN_TOCC_TOS_TXEF,
	/** Timeout controlled by Rx FIFO 0. */
	CAN_TIMEOUT_RX_FIFO_0 = CAN_TOCC_TOS_RXF0,
	/** Timeout controlled by Rx FIFO 1. */
	CAN_TIMEOUT_RX_FIFO_1 = CAN_TOCC_TOS_RXF1,
};



/**
 * \brief CAN software device instance structure.
 *
 * CAN software instance structure, used to retain software state information
 * of an associated hardware module instance.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct can_module {
#if !defined(__DOXYGEN__)
	/** Pointer to CAN hardware module. */
	Can *hw;
#endif
};

/**
 * \brief CAN configuration structure.
 *
 * Configuration structure for an CAN instance. This structure should be
 * initialized by the \ref can_get_config_defaults()
 * function before being modified by the user application.
 */
struct can_config {
	/** GCLK generator used to clock the peripheral. */
	enum gclk_generator clock_source;
	/** CAN run in standby control. */
	bool run_in_standby;
	/**Start value of the Message RAM Watchdog Counter */
	uint8_t watchdog_configuration;
	/** Transmit Pause. */
	bool transmit_pause;
	/** Edge Filtering during Bus Integration. */
	bool edge_filtering;
	/** Protocol Exception Handling. */
	bool protocol_exception_handling;
	/** Automatic Retransmission. */
	bool automatic_retransmission;
	/** Clock Stop Request. */
	bool clock_stop_request;
	/** Clock Stop Acknowledge. */
	bool clock_stop_acknowledge;

};

/**
 * \brief Initializes an CAN configuration structure to defaults
 *
 * Initializes a given CAN configuration struct to a set of known default
 * values. This function should be called on any new instance of the
 * configuration struct before being modified by the user application.
 *
 * The default configuration is as follows:
 *  \li GCLK generator 8 (GCLK main) clock source
 *  \li Not run in standby mode
 *  \li Watchdog value with 0xFF
 *  \li Transmit pause enabled
 *  \li Edge filtering during bus integration enabled
 *  \li Protocol exception handling enabled
 *  \li Automatic retransmission enabled
 *  \li Clock stop request disabled
 *  \li Clock stop acknowledge disabled
 *
 * \param[out] config  Pointer to configuration struct to initialize to
 *                     default values
 */
static inline void can_get_config_defaults(
		struct can_config *const config)
{
	/* Sanity check arguments */
	Assert(config);

	/* Default configuration values */
	config->clock_source = GCLK_GENERATOR_8;
	config->run_in_standby = false;
	config->watchdog_configuration = 0xFF;
	config->transmit_pause = true;
	config->edge_filtering = true;
	config->protocol_exception_handling = true;
	config->automatic_retransmission = true;
	config->clock_stop_request = false;
	config->clock_stop_acknowledge = false;
}


/**
 * \brief Initializes CAN module.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in]  hw          Pointer to the CAN module instance
 * \param[in]  config      Pointer to the configuration struct
 */
void can_module_init(struct can_module *const module_inst, Can *hw,
		struct can_config *config);

/**
 * \brief Can operation mode switch.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] mode      Can operation mode type
 */
void can_switch_mode(struct can_module *const module_inst,
		const enum can_mode mode);

/**
 * \brief Can set timestamp prescale.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] prescale      Prescale value
 */
static inline void can_set_timestamp_prescale(
		struct can_module *const module_inst, uint32_t prescale)
{
	module_inst->hw->TSCC.reg = CAN_TSCC_TCP(prescale) | CAN_TSCC_TSS_INC_Val;
}

/**
 * \brief Can read timestamp count value.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 *
 */
static inline uint16_t can_read_timestamp_count_value(
		struct can_module *const module_inst)
{
	return module_inst->hw->TSCV.bit.TSC;
}

/**
 * \brief Can set timeout.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] period      Period value
 * \param[in] mode      Timeout mode type
 */
static inline void can_set_timeout(struct can_module *const module_inst,
		uint16_t period, const enum can_timeout_mode mode)
{
	module_inst->hw->TOCC.reg = CAN_TOCC_TOP(period) | mode | CAN_TOCC_ETOC;
}

/**
 * \brief Can read timeout  count value.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 *
 */
static inline uint16_t can_read_timeout_count_value(
		struct can_module *const module_inst)
{
	return module_inst->hw->TOCV.bit.TOC;
}

/**
 * \brief Can read error count.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 *
 */
static inline uint32_t can_read_error_count(
		struct can_module *const module_inst)
{
	return module_inst->hw->ECR.reg;
}

/**
 * \brief Can read protocal status.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 *
 */
static inline uint32_t can_read_protocal_status(
		struct can_module *const module_inst)
{
	return module_inst->hw->PSR.reg;
}

/**
 * \brief Can set timeout.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] offset  Offset value
 * \param[in] filter_window_length  Filter length
 */
static inline void can_set_tx_delay_compensation(
		struct can_module *const module_inst,
		uint8_t offset, uint8_t filter_window_length)
{
	module_inst->hw->TDCR.reg = CAN_TDCR_TDCO(offset) |
			CAN_TDCR_TDCF(filter_window_length);
}

/** @} */

/**
 * \name Rx Handling 
 * @{
 */

/**
 * \brief Can nonmatching frames action.
 */
enum can_nonmatching_frames_action {
	/** Accept in Rx FIFO 0. */
	CAN_NONMATCHING_FRAMES_FIFO_0,
	/** Accept in Rx FIFO 1. */
	CAN_NONMATCHING_FRAMES_FIFO_1,
	/** Reject. */
	CAN_NONMATCHING_FRAMES_REJECT,
};

/**
 * \brief Can global filter setting.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] standard  Action for nonmatching standard frames
 * \param[in] extended  Action for nonmatching extended frames
 * \param[in] reject_remote_frame_standard  True for reject standard remote frame request
 * \param[in] reject_remote_frame_extended  True for reject extended remote frame request
 */
static inline void can_set_global_filter(struct can_module *const module_inst,
		const enum can_nonmatching_frames_action standard,
		const enum can_nonmatching_frames_action extended,
		bool reject_remote_frame_standard,
		bool reject_remote_frame_extended)
{
	module_inst->hw->GFC.reg = CAN_GFC_ANFS(standard) |	CAN_GFC_ANFE(extended);
	if (reject_remote_frame_standard) {
		module_inst->hw->GFC.reg |= CAN_GFC_RRFS;
	}
	if (reject_remote_frame_extended) {
		module_inst->hw->GFC.reg |= CAN_GFC_RRFE;
	}
}

/**
 * \brief Read high priority message status.
 *
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 */
static inline uint32_t can_read_high_priority_message_status(
		struct can_module *const module_inst)
{
	return module_inst->hw->HPMS.reg;
}

/**
 * \brief Set Rx FIFO operation mode.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] fifo_number  FIFO 0 or 1
 * \param[in] overwrite  True for overwrite mode, false for block mode
 */
static inline void can_rx_set_fifo_mode(
		struct can_module *const module_inst,
		bool fifo_number, bool overwrite)
{
	if (!fifo_number) {
		if (overwrite) {
			module_inst->hw->RXF0C.reg |= CAN_RXF0C_F0OM;
		} else {
			module_inst->hw->RXF0C.reg &= ~CAN_RXF0C_F0OM;
		}
	} else {
		if (overwrite) {
			module_inst->hw->RXF1C.reg |= CAN_RXF1C_F1OM;
		} else {
			module_inst->hw->RXF1C.reg &= ~CAN_RXF1C_F1OM;
		}
	}
}

/**
 * \brief Get Rx FIFO status.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] fifo_number  FIFO 0 or 1
 */
static inline uint32_t can_rx_get_fifo_status(
		struct can_module *const module_inst, bool fifo_number)
{
	if (!fifo_number) {
		return module_inst->hw->RXF0S.reg;
	} else {
		return module_inst->hw->RXF1S.reg;
	}
}

/**
 * \brief Set Rx acknowledge.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] fifo_number  FIFO 0 or 1
 * \param[in] index  Index offset in FIFO
 */
static inline void can_rx_fifo_acknowledge(
		struct can_module *const module_inst, bool fifo_number, uint32_t index)
{
	if (!fifo_number) {
		module_inst->hw->RXF0A.reg = CAN_RXF0A_F0AI(index);
	} else {
		module_inst->hw->RXF1A.reg = CAN_RXF1A_F1AI(index);
	}
}

/**
 * \brief Get the standard message filter default value.
 *
 * \param[out] sd_filter  Pointer to standard filter element struct to initialize to default values
 */
static inline void can_get_sd_message_filter_element_default(
		struct can_sd_message_filter_element *sd_filter)
{
	sd_filter->S0.reg = CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFID2_Msk |
			CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFID1(0) |
			CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFEC_STF0M |
			CAN_SD_MESSAGE_FILTER_ELEMENT_S0_SFT_CLASSIC;			
}

/**
 * \brief Set the standard message filter.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] sd_filter  Pointer to standard filter element struct
 * \param[in] index  Index offset in standard filter element
 */
void can_set_rx_standand_filter(struct can_module *const module_inst,
		struct can_sd_message_filter_element *sd_filter, uint32_t index);

/**
 * \brief Get the extended message filter default value.
 *
 * \param[out] et_filter  Pointer to extended filter element struct to initialize to default values
 */
static inline void can_get_et_message_filter_element_default(
		struct can_et_message_filter_element *et_filter)
{
	et_filter->F0.reg = CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFID1(0) |
			CAN_ET_MESSAGE_FILTER_ELEMENT_F0_EFEC_STF1M;			
	et_filter->F1.reg = CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFID2_Msk |
			CAN_ET_MESSAGE_FILTER_ELEMENT_F1_EFT_CLASSIC;
}

/**
 * \brief Set the extended message filter.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] et_filter  Pointer to extended filter element struct
 * \param[in] index  Index offset in extended filter element
 */
void can_set_rx_extended_filter(struct can_module *const module_inst,
		struct can_et_message_filter_element *et_filter, uint32_t index);

/**
 * \brief Get the point to the receive buffer element.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] rx_element  Pointer to receive buffer element
 * \param[in] index  Index offset in receive buffer
 */
void can_get_rx_buffer_element(struct can_module *const module_inst,
		struct can_rx_element *rx_element, uint32_t index);

/**
 * \brief Get the point to the receive FIFO 0 element.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] rx_element  Pointer to receive FIFO 0
 * \param[in] index  Index offset in receive FIFO 0
 */
void can_get_rx_fifo_0_element(struct can_module *const module_inst,
		 struct can_rx_element *rx_element, uint32_t index);

/**
 * \brief Get the point to the receive FIFO 1 element.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] rx_element  Pointer to receive FIFO 1
 * \param[in] index  Index offset in receive FIFO 1
 */
void can_get_rx_fifo_1_element(struct can_module *const module_inst,
		 struct can_rx_element *rx_element, uint32_t index);

/** @} */

/**
 * \name Tx Handling 
 * @{
 */

/**
 * \brief Set Tx FIFO operation.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 */
static inline void can_tx_set_fifo_mode(struct can_module *const module_inst)
{
	module_inst->hw->TXBC.reg &= ~CAN_TXBC_TFQM;
}

/**
 * \brief Set Tx Queue operation.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 */
static inline void can_tx_set_queue_mode(struct can_module *const module_inst)
{
	module_inst->hw->TXBC.reg |= CAN_TXBC_TFQM;
}

/**
 * \brief Get Tx FIFO/Queue status.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 */
static inline uint32_t can_tx_get_fifo_queue_status(
		struct can_module *const module_inst)
{
	return module_inst->hw->TXFQS.reg;
}

/**
 * \brief Get Tx FIFO/Queue status.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 */
static inline uint32_t can_tx_get_pending_status(
		struct can_module *const module_inst)
{
	return module_inst->hw->TXBRP.reg;
}

/**
 * \brief Set Tx Queue operation.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] trig_mask  The mask value to trigger tansfer buffer 
 */
static inline void can_tx_add_request(struct can_module *const module_inst,
		uint32_t trig_mask)
{
	module_inst->hw->TXBAR.reg = trig_mask;
}

/**
 * \brief Set Tx Queue operation.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] trig_mask  The mask value to cancel tansfer buffer 
 */
static inline void can_tx_cancel_request(struct can_module *const module_inst,
		uint32_t trig_mask)
{
	module_inst->hw->TXBCR.reg = trig_mask;
}

/**
 * \brief Get Tx FIFO/Queue transmission status.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 */
static inline uint32_t can_tx_get_transmission_status(
		struct can_module *const module_inst)
{
	return module_inst->hw->TXBTO.reg;
}

/**
 * \brief Get Tx FIFO/Queue cancellationion status.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 */
static inline uint32_t can_tx_get_cancellationion_status(
		struct can_module *const module_inst)
{
	return module_inst->hw->TXBCF.reg;
}

/**
 * \brief Get Tx event FIFO status.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 */
static inline uint32_t can_tx_get_event_fifo_status(
		struct can_module *const module_inst)
{
	return module_inst->hw->TXEFS.reg;
}

/**
 * \brief Set Tx Queue operation.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] index  Index for the transfer FIFO
 */
static inline void can_tx_event_fifo_acknowledge(
		struct can_module *const module_inst, uint32_t index)
{
	module_inst->hw->TXEFA.reg = CAN_TXEFA_EFAI(index);
}

/**
 * \brief Get the default transfer buffer element.
 *
 * \param[out] tx_element  Pointer to transfer element struct to initialize to default values
 */
static inline void can_get_tx_buffer_element_defaults(
		struct can_tx_element *tx_element)
{
	tx_element->T0.reg = 0;
	tx_element->T1.reg = CAN_TX_ELEMENT_T1_DLC(8);
}

/**
 * \brief Set the transfer buffer element.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] tx_element  Pointer to transfer element struct
 * \param[in] index  Index for the transfer buffer
 */
void can_set_tx_buffer_element(struct can_module *const module_inst,
		struct can_tx_element *tx_element, uint32_t index);

/**
 * \brief Get the point to the transfer event FIFO element.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] tx_event_element  Pointer to transfer event element
 * \param[in] index  Index offset in transfer event FIFO
 */
void can_get_tx_event_fifo_element(struct can_module *const module_inst,
		 struct can_tx_event_element *tx_event_element, uint32_t index);

/** @} */

/**
 * \name Interrupt Handling 
 * @{
 */

/**
 * \brief Can module interrupt source.
 *
 * Enum for the interrupt source.
 */
enum can_interrupt_source {
	/** Rx FIFO 0 New Message Interrupt Enable. */
	CAN_RX_FIFO_0_NEW_MESSAGE = CAN_IE_RF0NE,
	/** Rx FIFO 0 Watermark Reached Interrupt Enable. */
	CAN_RX_FIFO_0_WATERMARK = CAN_IE_RF0WE,
	/** Rx FIFO 0 Full Interrupt Enable. */
	CAN_RX_FIFO_0_FULL = CAN_IE_RF0FE,
	/** Rx FIFO 0 Message Lost Interrupt Enable. */
	CAN_RX_FIFO_0_LOST_MESSAGE = CAN_IE_RF0LE,
	/** Rx FIFO 1 New Message Interrupt Enable. */
	CAN_RX_FIFO_1_NEW_MESSAGE = CAN_IE_RF1NE,
	/** Rx FIFO 1 Watermark Reached Interrupt Enable. */
	CAN_RX_FIFO_1_WATERMARK = CAN_IE_RF1WE,
	/** Rx FIFO 1 Full Interrupt Enable. */
	CAN_RX_FIFO_1_FULL = CAN_IE_RF1FE,
	/** Rx FIFO 1 Message Lost Interrupt Enable. */
	CAN_RX_FIFO_1_MESSAGE_LOST = CAN_IE_RF1LE,
	/** High Priority Message Interrupt Enable. */
	CAN_RX_HIGH_PRIORITY_MESSAGE = CAN_IE_HPME,
	/** Timestamp Completed Interrupt Enable. */
	CAN_TIMESTAMP_COMPLETE = CAN_IE_TCE,
	/** Transmission Cancellation Finished Interrupt Enable. */
	CAN_TX_CANCELLATION_FINISH = CAN_IE_TCFE,
	/** Tx FIFO Empty Interrupt Enable. */
	CAN_TX_FIFO_EMPTY = CAN_IE_TFEE,
	/** Tx Event FIFO New Entry Interrupt Enable. */
	CAN_TX_EVENT_FIFO_NEW_ENTRY = CAN_IE_TEFNE,
	/** Tx Event FIFO Watermark Reached Interrupt Enable. */
	CAN_TX_EVENT_FIFO_WATERMARK = CAN_IE_TEFWE,
	/** Tx Event FIFO Full Interrupt Enable. */
	CAN_TX_EVENT_FIFO_FULL = CAN_IE_TEFFE,
	/** Tx Event FIFO Element Lost Interrupt Enable. */
	CAN_TX_EVENT_FIFO_ELEMENT_LOST = CAN_IE_TEFLE,
	/** Timestamp Wraparound Interrupt Enable. */
	CAN_TIMESTAMP_WRAPAROUND = CAN_IE_TSWE,
	/** Message RAM Access Failure Interrupt Enable. */
	CAN_MESSAGE_RAM_ACCESS_FAILURE = CAN_IE_MRAFE,
	/** Timeout Occurred Interrupt Enable. */
	CAN_TIMEOUT_OCCURRED = CAN_IE_TOOE,
	/** Message stored to Dedicated Rx Buffer Interrupt Enable. */
	CAN_RX_BUFFER_NEW_MESSAGE = CAN_IE_DRXE,
	/** Bit Error Corrected Interrupt Enable. */
	CAN_BIT_ERROR_CORRECTED = CAN_IE_BECE,
	/** Bit Error Uncorrected Interrupt Enable. */
	CAN_BIT_ERROR_UNCORRECTED = CAN_IE_BEUE,
	/** Error Logging Overflow Interrupt Enable. */
	CAN_ERROR_LOGGING_OVERFLOW = CAN_IE_ELOE,
	/** Error Passive Interrupt Enable. */
	CAN_ERROR_PASSIVE = CAN_IE_EPE,
	/** Warning Status Interrupt Enable. */
	CAN_WARNING_STATUS = CAN_IE_EWE,
	/** Bus_Off Status Interrupt Enable. */
	CAN_BUS_OFF = CAN_IE_BOE,
	/** Watchdog Interrupt Interrupt Enable. */
	CAN_WATCHDOG = CAN_IE_WDIE,
	/** Protocol Error in Arbitration Phase Enable. */
	CAN_PROTOCAL_ERROR_ARBITRATION = CAN_IE_PEAE,
	/** Protocol Error in Data Phase Enable. */
	CAN_PROTOCAL_ERROR_DATA = CAN_IE_PEDE,
	/** Access to Reserved Address Enable. */
	CAN_ACCESS_RESERVED_ADDRESS = CAN_IE_ARAE,
};

/**
 * \brief Set CAN interrupt.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] source  Interrupt source type
 */
static inline void can_set_interrupt(struct can_module *const module_inst,
		const enum can_interrupt_source source)
{
	module_inst->hw->IE.reg |= source;
}

/**
 * \brief Get CAN interrupt status.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 */
static inline uint32_t can_read_interrupt_status(
		struct can_module *const module_inst)
{
	return module_inst->hw->IR.reg;
}

/**
 * \brief Clear CAN interrupt status.
 *
 * \param[in] module_inst  Pointer to the CAN software instance struct
 * \param[in] source  Interrupt source type
 */
static inline void can_clear_interrupt_status(
		struct can_module *const module_inst,
		const enum can_interrupt_source source)
{
	module_inst->hw->IR.reg = source;
}

/** @} */



#ifdef __cplusplus
}
#endif

/** @} */

/**
 * \page asfdoc_sam0_can_extra Extra Information for CAN Driver
 *
 * \section asfdoc_sam0_can_extra_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *	<tr>
 *		<th>Acronym</th>
 *		<th>Description</th>
 *	</tr>
 *  <tr>
 *		<td>CAN</td>
 *		<td>Operational Amplifier Controller</td>
 *	</tr>
 * </table>
 *
 *
 * \section asfdoc_sam0_can_extra_dependencies Dependencies
 * This driver has no dependencies.
 *
 *
 * \section asfdoc_sam0_can_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_sam0_can_extra_history Module History
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in
 * the table.
 *
 * <table>
 *	<tr>
 *		<th>Changelog</th>
 *	</tr>
  *	<tr>
 *		<td>Initial Release</td>
 *	</tr>
 * </table>
 */

/**
 * \page asfdoc_sam0_can_exqsg Examples for CAN Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_sam0_can_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_sam0_can_basic_use_case
 *
 * \page asfdoc_sam0_can_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Doc. Rev.</td>
 *		<th>Date</td>
 *		<th>Comments</td>
 *	</tr>
 *	<tr>
 *		<td>A</td>
 *		<td>01/2015</td>
 *		<td>Initial release</td>
 *	</tr>
 * </table>
 */

#endif /* CAN_H_INCLUDED */

