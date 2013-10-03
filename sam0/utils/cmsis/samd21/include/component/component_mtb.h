/**
 * \file
 *
 * \brief Component description for MTB
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

#ifndef _SAMD21_MTB_COMPONENT_
#define _SAMD21_MTB_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR MTB */
/* ========================================================================== */
/** \addtogroup SAMD21_MTB Cortex-M0+ Micro-Trace Buffer */
/*@{*/

#define MTB_U2002
#define REV_MTB                     0x100

/* -------- MTB_POSITION : (MTB Offset: 0x000) (R/W 32) MTB Position Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t :2;               /*!< bit:  0.. 1  Reserved                           */
    uint32_t WRAP:1;           /*!< bit:      2                                     */
    uint32_t POINTER:29;       /*!< bit:  3..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_POSITION_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_POSITION_OFFSET         0x000        /**< \brief (MTB_POSITION offset) MTB Position Register */

#define MTB_POSITION_WRAP_Pos       2            /**< \brief (MTB_POSITION)  */
#define MTB_POSITION_WRAP           (0x1u << MTB_POSITION_WRAP_Pos)
#define MTB_POSITION_POINTER_Pos    3            /**< \brief (MTB_POSITION)  */
#define MTB_POSITION_POINTER_Msk    (0x1FFFFFFFu << MTB_POSITION_POINTER_Pos)
#define MTB_POSITION_POINTER(value) ((MTB_POSITION_POINTER_Msk & ((value) << MTB_POSITION_POINTER_Pos)))
#define MTB_POSITION_MASK           0xFFFFFFFCu  /**< \brief (MTB_POSITION) MASK Register */

/* -------- MTB_MASTER : (MTB Offset: 0x004) (R/W 32) MTB Master Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t MASK:5;           /*!< bit:  0.. 4                                     */
    uint32_t TSTARTEN:1;       /*!< bit:      5                                     */
    uint32_t TSTOPEN:1;        /*!< bit:      6                                     */
    uint32_t SFRWPRIV:1;       /*!< bit:      7                                     */
    uint32_t RAMPRIV:1;        /*!< bit:      8                                     */
    uint32_t HALTREQ:1;        /*!< bit:      9                                     */
    uint32_t :21;              /*!< bit: 10..30  Reserved                           */
    uint32_t EN:1;             /*!< bit:     31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_MASTER_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_MASTER_OFFSET           0x004        /**< \brief (MTB_MASTER offset) MTB Master Register */
#define MTB_MASTER_RESETVALUE       0x00000000   /**< \brief (MTB_MASTER reset_value) MTB Master Register */

#define MTB_MASTER_MASK_Pos         0            /**< \brief (MTB_MASTER)  */
#define MTB_MASTER_MASK_Msk         (0x1Fu << MTB_MASTER_MASK_Pos)
#define MTB_MASTER_MASK(value)      ((MTB_MASTER_MASK_Msk & ((value) << MTB_MASTER_MASK_Pos)))
#define MTB_MASTER_TSTARTEN_Pos     5            /**< \brief (MTB_MASTER)  */
#define MTB_MASTER_TSTARTEN         (0x1u << MTB_MASTER_TSTARTEN_Pos)
#define MTB_MASTER_TSTOPEN_Pos      6            /**< \brief (MTB_MASTER)  */
#define MTB_MASTER_TSTOPEN          (0x1u << MTB_MASTER_TSTOPEN_Pos)
#define MTB_MASTER_SFRWPRIV_Pos     7            /**< \brief (MTB_MASTER)  */
#define MTB_MASTER_SFRWPRIV         (0x1u << MTB_MASTER_SFRWPRIV_Pos)
#define MTB_MASTER_RAMPRIV_Pos      8            /**< \brief (MTB_MASTER)  */
#define MTB_MASTER_RAMPRIV          (0x1u << MTB_MASTER_RAMPRIV_Pos)
#define MTB_MASTER_HALTREQ_Pos      9            /**< \brief (MTB_MASTER)  */
#define MTB_MASTER_HALTREQ          (0x1u << MTB_MASTER_HALTREQ_Pos)
#define MTB_MASTER_EN_Pos           31           /**< \brief (MTB_MASTER)  */
#define MTB_MASTER_EN               (0x1u << MTB_MASTER_EN_Pos)
#define MTB_MASTER_MASK_            0x800003FFu  /**< \brief (MTB_MASTER) MASK Register */

/* -------- MTB_FLOW : (MTB Offset: 0x008) (R/W 32) MTB Flow Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t AUTOSTOP:1;       /*!< bit:      0                                     */
    uint32_t AUTOHALT:1;       /*!< bit:      1                                     */
    uint32_t :1;               /*!< bit:      2  Reserved                           */
    uint32_t WATERMARK:29;     /*!< bit:  3..31                                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_FLOW_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_FLOW_OFFSET             0x008        /**< \brief (MTB_FLOW offset) MTB Flow Register */
#define MTB_FLOW_RESETVALUE         0x00000000   /**< \brief (MTB_FLOW reset_value) MTB Flow Register */

#define MTB_FLOW_AUTOSTOP_Pos       0            /**< \brief (MTB_FLOW)  */
#define MTB_FLOW_AUTOSTOP           (0x1u << MTB_FLOW_AUTOSTOP_Pos)
#define MTB_FLOW_AUTOHALT_Pos       1            /**< \brief (MTB_FLOW)  */
#define MTB_FLOW_AUTOHALT           (0x1u << MTB_FLOW_AUTOHALT_Pos)
#define MTB_FLOW_WATERMARK_Pos      3            /**< \brief (MTB_FLOW)  */
#define MTB_FLOW_WATERMARK_Msk      (0x1FFFFFFFu << MTB_FLOW_WATERMARK_Pos)
#define MTB_FLOW_WATERMARK(value)   ((MTB_FLOW_WATERMARK_Msk & ((value) << MTB_FLOW_WATERMARK_Pos)))
#define MTB_FLOW_MASK               0xFFFFFFFBu  /**< \brief (MTB_FLOW) MASK Register */

/* -------- MTB_BASE : (MTB Offset: 0x00C) (R/  32) MTB Base Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_BASE_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_BASE_OFFSET             0x00C        /**< \brief (MTB_BASE offset) MTB Base Register */
#define MTB_BASE_MASK               0xFFFFFFFFu  /**< \brief (MTB_BASE) MASK Register */

/* -------- MTB_ITCTRL : (MTB Offset: 0xF00) (R/W 32) MTB Integration Mode Control Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_ITCTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_ITCTRL_OFFSET           0xF00        /**< \brief (MTB_ITCTRL offset) MTB Integration Mode Control Register */
#define MTB_ITCTRL_MASK             0xFFFFFFFFu  /**< \brief (MTB_ITCTRL) MASK Register */

/* -------- MTB_CLAIMSET : (MTB Offset: 0xFA0) (R/W 32) MTB Claim Set Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_CLAIMSET_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_CLAIMSET_OFFSET         0xFA0        /**< \brief (MTB_CLAIMSET offset) MTB Claim Set Register */
#define MTB_CLAIMSET_MASK           0xFFFFFFFFu  /**< \brief (MTB_CLAIMSET) MASK Register */

/* -------- MTB_CLAIMCLR : (MTB Offset: 0xFA4) (R/W 32) MTB Claim Clear Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_CLAIMCLR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_CLAIMCLR_OFFSET         0xFA4        /**< \brief (MTB_CLAIMCLR offset) MTB Claim Clear Register */
#define MTB_CLAIMCLR_MASK           0xFFFFFFFFu  /**< \brief (MTB_CLAIMCLR) MASK Register */

/* -------- MTB_LOCKACCESS : (MTB Offset: 0xFB0) (R/W 32) MTB Lock Access Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_LOCKACCESS_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_LOCKACCESS_OFFSET       0xFB0        /**< \brief (MTB_LOCKACCESS offset) MTB Lock Access Register */
#define MTB_LOCKACCESS_MASK         0xFFFFFFFFu  /**< \brief (MTB_LOCKACCESS) MASK Register */

/* -------- MTB_LOCKSTATUS : (MTB Offset: 0xFB4) (R/  32) MTB Lock Status Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_LOCKSTATUS_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_LOCKSTATUS_OFFSET       0xFB4        /**< \brief (MTB_LOCKSTATUS offset) MTB Lock Status Register */
#define MTB_LOCKSTATUS_MASK         0xFFFFFFFFu  /**< \brief (MTB_LOCKSTATUS) MASK Register */

/* -------- MTB_AUTHSTATUS : (MTB Offset: 0xFB8) (R/  32) MTB Authentication Status Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_AUTHSTATUS_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_AUTHSTATUS_OFFSET       0xFB8        /**< \brief (MTB_AUTHSTATUS offset) MTB Authentication Status Register */
#define MTB_AUTHSTATUS_MASK         0xFFFFFFFFu  /**< \brief (MTB_AUTHSTATUS) MASK Register */

/* -------- MTB_DEVARCH : (MTB Offset: 0xFBC) (R/  32) MTB Device Architecture Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_DEVARCH_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_DEVARCH_OFFSET          0xFBC        /**< \brief (MTB_DEVARCH offset) MTB Device Architecture Register */
#define MTB_DEVARCH_MASK            0xFFFFFFFFu  /**< \brief (MTB_DEVARCH) MASK Register */

/* -------- MTB_DEVID : (MTB Offset: 0xFC8) (R/  32) MTB Device Configuration Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_DEVID_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_DEVID_OFFSET            0xFC8        /**< \brief (MTB_DEVID offset) MTB Device Configuration Register */
#define MTB_DEVID_MASK              0xFFFFFFFFu  /**< \brief (MTB_DEVID) MASK Register */

/* -------- MTB_DEVTYPE : (MTB Offset: 0xFCC) (R/  32) MTB Device Type Register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_DEVTYPE_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_DEVTYPE_OFFSET          0xFCC        /**< \brief (MTB_DEVTYPE offset) MTB Device Type Register */
#define MTB_DEVTYPE_MASK            0xFFFFFFFFu  /**< \brief (MTB_DEVTYPE) MASK Register */

/* -------- MTB_PID4 : (MTB Offset: 0xFD0) (R/  32) CoreSight register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_PID4_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_PID4_OFFSET             0xFD0        /**< \brief (MTB_PID4 offset) CoreSight register */
#define MTB_PID4_MASK               0xFFFFFFFFu  /**< \brief (MTB_PID4) MASK Register */

/* -------- MTB_PID5 : (MTB Offset: 0xFD4) (R/  32) CoreSight register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_PID5_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_PID5_OFFSET             0xFD4        /**< \brief (MTB_PID5 offset) CoreSight register */
#define MTB_PID5_MASK               0xFFFFFFFFu  /**< \brief (MTB_PID5) MASK Register */

/* -------- MTB_PID6 : (MTB Offset: 0xFD8) (R/  32) CoreSight register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_PID6_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_PID6_OFFSET             0xFD8        /**< \brief (MTB_PID6 offset) CoreSight register */
#define MTB_PID6_MASK               0xFFFFFFFFu  /**< \brief (MTB_PID6) MASK Register */

/* -------- MTB_PID7 : (MTB Offset: 0xFDC) (R/  32) CoreSight register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_PID7_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_PID7_OFFSET             0xFDC        /**< \brief (MTB_PID7 offset) CoreSight register */
#define MTB_PID7_MASK               0xFFFFFFFFu  /**< \brief (MTB_PID7) MASK Register */

/* -------- MTB_PID0 : (MTB Offset: 0xFE0) (R/  32) CoreSight register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_PID0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_PID0_OFFSET             0xFE0        /**< \brief (MTB_PID0 offset) CoreSight register */
#define MTB_PID0_MASK               0xFFFFFFFFu  /**< \brief (MTB_PID0) MASK Register */

/* -------- MTB_PID1 : (MTB Offset: 0xFE4) (R/  32) CoreSight register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_PID1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_PID1_OFFSET             0xFE4        /**< \brief (MTB_PID1 offset) CoreSight register */
#define MTB_PID1_MASK               0xFFFFFFFFu  /**< \brief (MTB_PID1) MASK Register */

/* -------- MTB_PID2 : (MTB Offset: 0xFE8) (R/  32) CoreSight register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_PID2_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_PID2_OFFSET             0xFE8        /**< \brief (MTB_PID2 offset) CoreSight register */
#define MTB_PID2_MASK               0xFFFFFFFFu  /**< \brief (MTB_PID2) MASK Register */

/* -------- MTB_PID3 : (MTB Offset: 0xFEC) (R/  32) CoreSight register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_PID3_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_PID3_OFFSET             0xFEC        /**< \brief (MTB_PID3 offset) CoreSight register */
#define MTB_PID3_MASK               0xFFFFFFFFu  /**< \brief (MTB_PID3) MASK Register */

/* -------- MTB_CID0 : (MTB Offset: 0xFF0) (R/  32) CoreSight register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_CID0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_CID0_OFFSET             0xFF0        /**< \brief (MTB_CID0 offset) CoreSight register */
#define MTB_CID0_MASK               0xFFFFFFFFu  /**< \brief (MTB_CID0) MASK Register */

/* -------- MTB_CID1 : (MTB Offset: 0xFF4) (R/  32) CoreSight register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_CID1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_CID1_OFFSET             0xFF4        /**< \brief (MTB_CID1 offset) CoreSight register */
#define MTB_CID1_MASK               0xFFFFFFFFu  /**< \brief (MTB_CID1) MASK Register */

/* -------- MTB_CID2 : (MTB Offset: 0xFF8) (R/  32) CoreSight register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_CID2_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_CID2_OFFSET             0xFF8        /**< \brief (MTB_CID2 offset) CoreSight register */
#define MTB_CID2_MASK               0xFFFFFFFFu  /**< \brief (MTB_CID2) MASK Register */

/* -------- MTB_CID3 : (MTB Offset: 0xFFC) (R/  32) CoreSight register -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} MTB_CID3_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MTB_CID3_OFFSET             0xFFC        /**< \brief (MTB_CID3 offset) CoreSight register */
#define MTB_CID3_MASK               0xFFFFFFFFu  /**< \brief (MTB_CID3) MASK Register */

/** \brief MTB hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  __IO MTB_POSITION_Type         POSITION;    /**< \brief Offset: 0x000 (R/W 32) MTB Position Register */
  __IO MTB_MASTER_Type           MASTER;      /**< \brief Offset: 0x004 (R/W 32) MTB Master Register */
  __IO MTB_FLOW_Type             FLOW;        /**< \brief Offset: 0x008 (R/W 32) MTB Flow Register */
  __I  MTB_BASE_Type             BASE;        /**< \brief Offset: 0x00C (R/  32) MTB Base Register */
       RoReg8                    Reserved1[0xEF0];
  __IO MTB_ITCTRL_Type           ITCTRL;      /**< \brief Offset: 0xF00 (R/W 32) MTB Integration Mode Control Register */
       RoReg8                    Reserved2[0x9C];
  __IO MTB_CLAIMSET_Type         CLAIMSET;    /**< \brief Offset: 0xFA0 (R/W 32) MTB Claim Set Register */
  __IO MTB_CLAIMCLR_Type         CLAIMCLR;    /**< \brief Offset: 0xFA4 (R/W 32) MTB Claim Clear Register */
       RoReg8                    Reserved3[0x8];
  __IO MTB_LOCKACCESS_Type       LOCKACCESS;  /**< \brief Offset: 0xFB0 (R/W 32) MTB Lock Access Register */
  __I  MTB_LOCKSTATUS_Type       LOCKSTATUS;  /**< \brief Offset: 0xFB4 (R/  32) MTB Lock Status Register */
  __I  MTB_AUTHSTATUS_Type       AUTHSTATUS;  /**< \brief Offset: 0xFB8 (R/  32) MTB Authentication Status Register */
  __I  MTB_DEVARCH_Type          DEVARCH;     /**< \brief Offset: 0xFBC (R/  32) MTB Device Architecture Register */
       RoReg8                    Reserved4[0x8];
  __I  MTB_DEVID_Type            DEVID;       /**< \brief Offset: 0xFC8 (R/  32) MTB Device Configuration Register */
  __I  MTB_DEVTYPE_Type          DEVTYPE;     /**< \brief Offset: 0xFCC (R/  32) MTB Device Type Register */
  __I  MTB_PID4_Type             PID4;        /**< \brief Offset: 0xFD0 (R/  32) CoreSight register */
  __I  MTB_PID5_Type             PID5;        /**< \brief Offset: 0xFD4 (R/  32) CoreSight register */
  __I  MTB_PID6_Type             PID6;        /**< \brief Offset: 0xFD8 (R/  32) CoreSight register */
  __I  MTB_PID7_Type             PID7;        /**< \brief Offset: 0xFDC (R/  32) CoreSight register */
  __I  MTB_PID0_Type             PID0;        /**< \brief Offset: 0xFE0 (R/  32) CoreSight register */
  __I  MTB_PID1_Type             PID1;        /**< \brief Offset: 0xFE4 (R/  32) CoreSight register */
  __I  MTB_PID2_Type             PID2;        /**< \brief Offset: 0xFE8 (R/  32) CoreSight register */
  __I  MTB_PID3_Type             PID3;        /**< \brief Offset: 0xFEC (R/  32) CoreSight register */
  __I  MTB_CID0_Type             CID0;        /**< \brief Offset: 0xFF0 (R/  32) CoreSight register */
  __I  MTB_CID1_Type             CID1;        /**< \brief Offset: 0xFF4 (R/  32) CoreSight register */
  __I  MTB_CID2_Type             CID2;        /**< \brief Offset: 0xFF8 (R/  32) CoreSight register */
  __I  MTB_CID3_Type             CID3;        /**< \brief Offset: 0xFFC (R/  32) CoreSight register */
} Mtb;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAMD21_MTB_COMPONENT_ */
