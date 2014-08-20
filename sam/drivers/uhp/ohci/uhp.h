/* ---------------------------------------------------------------------------- */
/*                  Atmel Microcontroller Software Support                      */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) 2014, Atmel Corporation                                        */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Atmel's name may not be used to endorse or promote products derived from     */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR   */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE   */
/* DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,      */
/* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,  */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    */
/* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING         */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/* ---------------------------------------------------------------------------- */

#ifndef _SAMG55_UHP_COMPONENT_
#define _SAMG55_UHP_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR USB Host Port */
/* ============================================================================= */
/** \addtogroup SAMG55_UHP USB Host Port */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Uhp hardware registers */
typedef struct {
  __I  uint32_t HcRevision;   /**< \brief (Udp Offset: 0x000) Frame Number Register */
  __IO uint32_t HcControl;  /**< \brief (Udp Offset: 0x004) Global State Register */
  __IO uint32_t HcCommandStatus;     /**< \brief (Udp Offset: 0x008) Function Address Register */
  __IO uint32_t HcInterruptStatus;
  __IO uint32_t HcInterruptEnable;       /**< \brief (Udp Offset: 0x010) Interrupt Enable Register */
  __IO uint32_t HcInterruptDisable;       /**< \brief (Udp Offset: 0x014) Interrupt Disable Register */
  __IO uint32_t HcHCCA;       /**< \brief (Udp Offset: 0x018) Interrupt Mask Register */
  __I  uint32_t HcPeriodCurrentED;       /**< \brief (Udp Offset: 0x01C) Interrupt Status Register */
  __IO uint32_t HcControlHeadED;       /**< \brief (Udp Offset: 0x020) Interrupt Clear Register */
  __IO uint32_t HcControlCurrentED;
  __IO uint32_t HcBulkHeadED;    /**< \brief (Udp Offset: 0x028) Reset Endpoint Register */
  __IO uint32_t HcBulkCurrentED;
  __I  uint32_t HcDoneHead;    /**< \brief (Udp Offset: 0x030) Endpoint Control and Status Register */
  __IO uint32_t HcFmInterval;
  __I  uint32_t HcFmRemaining;    /**< \brief (Udp Offset: 0x050) Endpoint FIFO Data Register */
  __I  uint32_t HcFmNumber;
  __IO uint32_t HcPeriodicStart;      /**< \brief (Udp Offset: 0x074) Transceiver Control Register */
  __IO uint32_t HcLSThreshold;
  __IO uint32_t HcRhDescriptorA;  /**< \brief (Udp Offset: 0x0F8) USB FS Device Features Register */
  __IO uint32_t HcRhDescriptorB;   /**< \brief (Udp Offset: 0x0FC) Version Register */
  __IO uint32_t HcRhStatus;  /**< \brief (Udp Offset: 0x0F8) USB FS Device Features Register */
  __IO uint32_t HcRhPortStatus;   /**< \brief (Udp Offset: 0x0FC) Version Register */
} Uhp;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
//! \brief  HcControl register fields
#define HC_CONTROL_PLE                  (1 << 2)
#define HC_CONTROL_IE                   (1 << 3)
#define HC_CONTROL_CLE                  (1 << 4)
#define HC_CONTROL_BLE                  (1 << 5)
#define HC_CONTROL_HCFS                 (3 << 6)
#define HC_CONTROL_HCFS_USBRESET        (0 << 6)
#define HC_CONTROL_HCFS_USBRESUME       (1 << 6)
#define HC_CONTROL_HCFS_USBOPERATIONAL  (2 << 6)
#define HC_CONTROL_HCFS_USBSUSPEND      (3 << 6)
#define HC_CONTROL_IR	                (1 << 8)	/* interrupt routing */
#define HC_CONTROL_RWC	                (1 << 9)	/* remote wakeup connected */
#define HC_CONTROL_RWE	                (1 << 10)	/* remote wakeup enable */

//! \brief  HcCommandStatus register fields
#define HC_COMMANDSTATUS_HCR            (1 << 0)
#define HC_COMMANDSTATUS_CLF            (1 << 1)
#define HC_COMMANDSTATUS_BLF            (1 << 2)

/*
 * masks used with interrupt registers:
 * HcInterruptStatus (intrstatus)
 * HcInterruptEnable (intrenable)
 * HcInterruptDisable (intrdisable)
 */
//! \brief  HcInterruptEnable/Disable/Status registers fields
#define HC_INTERRUPT_SO                 (1 << 0)  /* scheduling overrun */
#define HC_INTERRUPT_WDH                (1 << 1)  /* writeback of done_head */
#define HC_INTERRUPT_SF                 (1 << 2)  /* start frame */
#define HC_INTERRUPT_RD                 (1 << 3)  /* resume detect */
#define HC_INTERRUPT_UE                 (1 << 4)  /* unrecoverable error */
#define HC_INTERRUPT_FNO                (1 << 5)  /* frame number overflow */
#define HC_INTERRUPT_RHSC               (1 << 6)  /* root hub status change */
#define HC_INTERRUPT_OC                 (1 << 30) /* ownership change */ 
#define HC_INTERRUPT_MIE                (1 << 31) /* master interrupt enable */

//! \brief  HcFmInterval register fields
#define HC_FMINTERVAL_FI                0x00003FFF

//! \brief  HcRhDescriptorA register fields
#define HC_RHDESCRIPTORA_NDP            0x000000FF

//! \brief  HcRhPortStatus register fields
#define HC_RHPORTSTATUS_CCS             (1 << 0)
#define HC_RHPORTSTATUS_PRS             (1 << 4)
#define HC_RHPORTSTATUS_LSDA            (1 << 9)


/* OHCI ROOT HUB REGISTER MASKS */

/* roothub.portstatus [i] bits */
#define RH_PS_CCS            0x00000001		/* current connect status */
#define RH_PS_PES            0x00000002		/* port enable status*/
#define RH_PS_PSS            0x00000004		/* port suspend status */
#define RH_PS_POCI           0x00000008		/* port over current indicator */
#define RH_PS_PRS            0x00000010		/* port reset status */
#define RH_PS_PPS            0x00000100		/* port power status */
#define RH_PS_LSDA           0x00000200		/* low speed device attached */
#define RH_PS_CSC            0x00010000		/* connect status change */
#define RH_PS_PESC           0x00020000		/* port enable status change */
#define RH_PS_PSSC           0x00040000		/* port suspend status change */
#define RH_PS_OCIC           0x00080000		/* over current indicator change */
#define RH_PS_PRSC           0x00100000		/* port reset status change */

/* roothub.status bits */
#define RH_HS_LPS	     0x00000001		/* local power status */
#define RH_HS_OCI	     0x00000002		/* over current indicator */
#define RH_HS_DRWE	     0x00008000		/* device remote wakeup enable */
#define RH_HS_LPSC	     0x00010000		/* local power status change */
#define RH_HS_OCIC	     0x00020000		/* over current indicator change */
#define RH_HS_CRWE	     0x80000000		/* clear remote wakeup enable */

/* roothub.b masks */
#define RH_B_DR		0x0000ffff		/* device removable flags */
#define RH_B_PPCM	0xffff0000		/* port power control mask */

/* roothub.a masks */
#define	RH_A_NDP	(0xff << 0)		/* number of downstream ports */
#define	RH_A_PSM	(1 << 8)		/* power switching mode */
#define	RH_A_NPS	(1 << 9)		/* no power switching */
#define	RH_A_DT		(1 << 10)		/* device type (mbz) */
#define	RH_A_OCPM	(1 << 11)		/* over current protection mode */
#define	RH_A_NOCP	(1 << 12)		/* no over current protection */
#define	RH_A_POTPGT	(0xff << 24)		/* power on to power good time */


/*@}*/


#endif /* _SAMG55_UHP_COMPONENT_ */
