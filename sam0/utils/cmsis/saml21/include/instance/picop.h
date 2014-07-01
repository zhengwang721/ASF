/**
 * \file
 *
 * \brief Instance description for PICOP
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

#ifndef _SAML21_PICOP_INSTANCE_
#define _SAML21_PICOP_INSTANCE_

/* ========== Register definition for PICOP peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PICOP_ID0              (0x44000C00U) /**< \brief (PICOP) ID 0 */
#define REG_PICOP_ID1              (0x44000C04U) /**< \brief (PICOP) ID 1 */
#define REG_PICOP_ID2              (0x44000C08U) /**< \brief (PICOP) ID 2 */
#define REG_PICOP_ID3              (0x44000C0CU) /**< \brief (PICOP) ID 3 */
#define REG_PICOP_ID4              (0x44000C10U) /**< \brief (PICOP) ID 4 */
#define REG_PICOP_ID5              (0x44000C14U) /**< \brief (PICOP) ID 5 */
#define REG_PICOP_ID6              (0x44000C18U) /**< \brief (PICOP) ID 6 */
#define REG_PICOP_ID7              (0x44000C1CU) /**< \brief (PICOP) ID 7 */
#define REG_PICOP_CONFIG           (0x44000C20U) /**< \brief (PICOP) Configuration */
#define REG_PICOP_CTRL             (0x44000C24U) /**< \brief (PICOP) Control */
#define REG_PICOP_CMD              (0x44000C28U) /**< \brief (PICOP) Command */
#define REG_PICOP_PC               (0x44000C2CU) /**< \brief (PICOP) Program Counter */
#define REG_PICOP_HF               (0x44000C30U) /**< \brief (PICOP) Host Flags */
#define REG_PICOP_HFCTRL           (0x44000C34U) /**< \brief (PICOP) Host Flag Control */
#define REG_PICOP_HFSETCLR0        (0x44000C38U) /**< \brief (PICOP) Host Flags Set/Clr */
#define REG_PICOP_HFSETCLR1        (0x44000C3CU) /**< \brief (PICOP) Host Flags Set/Clr */
#define REG_PICOP_OCDCONFIG        (0x44000C50U) /**< \brief (PICOP) OCD Configuration */
#define REG_PICOP_OCDCONTROL       (0x44000C54U) /**< \brief (PICOP) OCD Control */
#define REG_PICOP_OCDSTATUS        (0x44000C58U) /**< \brief (PICOP) OCD Status and Command */
#define REG_PICOP_OCDPC            (0x44000C5CU) /**< \brief (PICOP) ODC Program Counter */
#define REG_PICOP_OCDFEAT          (0x44000C60U) /**< \brief (PICOP) OCD Features */
#define REG_PICOP_OCDCCNT          (0x44000C68U) /**< \brief (PICOP) OCD Cycle Counter */
#define REG_PICOP_OCDBPGEN0        (0x44000C70U) /**< \brief (PICOP) OCD Breakpoint Generator 0 */
#define REG_PICOP_OCDBPGEN1        (0x44000C74U) /**< \brief (PICOP) OCD Breakpoint Generator 1 */
#define REG_PICOP_OCDBPGEN2        (0x44000C78U) /**< \brief (PICOP) OCD Breakpoint Generator 2 */
#define REG_PICOP_OCDBPGEN3        (0x44000C7CU) /**< \brief (PICOP) OCD Breakpoint Generator 3 */
#define REG_PICOP_R3R0             (0x44000C80U) /**< \brief (PICOP) R3 to 0 */
#define REG_PICOP_R7R4             (0x44000C84U) /**< \brief (PICOP) R7 to 4 */
#define REG_PICOP_R11R8            (0x44000C88U) /**< \brief (PICOP) R11 to 8 */
#define REG_PICOP_R15R12           (0x44000C8CU) /**< \brief (PICOP) R15 to 12 */
#define REG_PICOP_R19R16           (0x44000C90U) /**< \brief (PICOP) R19 to 16 */
#define REG_PICOP_R23R20           (0x44000C94U) /**< \brief (PICOP) R23 to 20 */
#define REG_PICOP_R27R24           (0x44000C98U) /**< \brief (PICOP) R27 to 24: XH, XL, R25, R24 */
#define REG_PICOP_R31R28           (0x44000C9CU) /**< \brief (PICOP) R31 to 28: ZH, ZL, YH, YL */
#define REG_PICOP_S1S0             (0x44000CA0U) /**< \brief (PICOP) System Regs 1 to 0: SR */
#define REG_PICOP_S3S2             (0x44000CA4U) /**< \brief (PICOP) System Regs 3 to 2: CTRL */
#define REG_PICOP_S5S4             (0x44000CA8U) /**< \brief (PICOP) System Regs 5 to 4: SREG, CCR */
#define REG_PICOP_S11S10           (0x44000CB4U) /**< \brief (PICOP) System Regs 11 to 10: Immediate */
#define REG_PICOP_LINK             (0x44000CB8U) /**< \brief (PICOP) Link */
#define REG_PICOP_SP               (0x44000CBCU) /**< \brief (PICOP) Stack Pointer */
#define REG_PICOP_MMUFLASH         (0x44000D00U) /**< \brief (PICOP) MMU mapping for flash */
#define REG_PICOP_MMU0             (0x44000D18U) /**< \brief (PICOP) MMU mapping user 0 */
#define REG_PICOP_MMU1             (0x44000D1CU) /**< \brief (PICOP) MMU mapping user 1 */
#define REG_PICOP_MMUCTRL          (0x44000D20U) /**< \brief (PICOP) MMU Control */
#define REG_PICOP_ICACHE           (0x44000D80U) /**< \brief (PICOP) Instruction Cache Control */
#define REG_PICOP_ICACHELRU        (0x44000D84U) /**< \brief (PICOP) Instruction Cache LRU */
#define REG_PICOP_QOSCTRL          (0x44000E00U) /**< \brief (PICOP) QOS Control */
#else
#define REG_PICOP_ID0              (*(RwReg  *)0x44000C00U) /**< \brief (PICOP) ID 0 */
#define REG_PICOP_ID1              (*(RwReg  *)0x44000C04U) /**< \brief (PICOP) ID 1 */
#define REG_PICOP_ID2              (*(RwReg  *)0x44000C08U) /**< \brief (PICOP) ID 2 */
#define REG_PICOP_ID3              (*(RwReg  *)0x44000C0CU) /**< \brief (PICOP) ID 3 */
#define REG_PICOP_ID4              (*(RwReg  *)0x44000C10U) /**< \brief (PICOP) ID 4 */
#define REG_PICOP_ID5              (*(RwReg  *)0x44000C14U) /**< \brief (PICOP) ID 5 */
#define REG_PICOP_ID6              (*(RwReg  *)0x44000C18U) /**< \brief (PICOP) ID 6 */
#define REG_PICOP_ID7              (*(RwReg  *)0x44000C1CU) /**< \brief (PICOP) ID 7 */
#define REG_PICOP_CONFIG           (*(RwReg  *)0x44000C20U) /**< \brief (PICOP) Configuration */
#define REG_PICOP_CTRL             (*(RwReg  *)0x44000C24U) /**< \brief (PICOP) Control */
#define REG_PICOP_CMD              (*(RwReg  *)0x44000C28U) /**< \brief (PICOP) Command */
#define REG_PICOP_PC               (*(RwReg  *)0x44000C2CU) /**< \brief (PICOP) Program Counter */
#define REG_PICOP_HF               (*(RwReg  *)0x44000C30U) /**< \brief (PICOP) Host Flags */
#define REG_PICOP_HFCTRL           (*(RwReg  *)0x44000C34U) /**< \brief (PICOP) Host Flag Control */
#define REG_PICOP_HFSETCLR0        (*(RwReg  *)0x44000C38U) /**< \brief (PICOP) Host Flags Set/Clr */
#define REG_PICOP_HFSETCLR1        (*(RwReg  *)0x44000C3CU) /**< \brief (PICOP) Host Flags Set/Clr */
#define REG_PICOP_OCDCONFIG        (*(RwReg  *)0x44000C50U) /**< \brief (PICOP) OCD Configuration */
#define REG_PICOP_OCDCONTROL       (*(RwReg  *)0x44000C54U) /**< \brief (PICOP) OCD Control */
#define REG_PICOP_OCDSTATUS        (*(RwReg  *)0x44000C58U) /**< \brief (PICOP) OCD Status and Command */
#define REG_PICOP_OCDPC            (*(RwReg  *)0x44000C5CU) /**< \brief (PICOP) ODC Program Counter */
#define REG_PICOP_OCDFEAT          (*(RwReg  *)0x44000C60U) /**< \brief (PICOP) OCD Features */
#define REG_PICOP_OCDCCNT          (*(RwReg  *)0x44000C68U) /**< \brief (PICOP) OCD Cycle Counter */
#define REG_PICOP_OCDBPGEN0        (*(RwReg  *)0x44000C70U) /**< \brief (PICOP) OCD Breakpoint Generator 0 */
#define REG_PICOP_OCDBPGEN1        (*(RwReg  *)0x44000C74U) /**< \brief (PICOP) OCD Breakpoint Generator 1 */
#define REG_PICOP_OCDBPGEN2        (*(RwReg  *)0x44000C78U) /**< \brief (PICOP) OCD Breakpoint Generator 2 */
#define REG_PICOP_OCDBPGEN3        (*(RwReg  *)0x44000C7CU) /**< \brief (PICOP) OCD Breakpoint Generator 3 */
#define REG_PICOP_R3R0             (*(RwReg  *)0x44000C80U) /**< \brief (PICOP) R3 to 0 */
#define REG_PICOP_R7R4             (*(RwReg  *)0x44000C84U) /**< \brief (PICOP) R7 to 4 */
#define REG_PICOP_R11R8            (*(RwReg  *)0x44000C88U) /**< \brief (PICOP) R11 to 8 */
#define REG_PICOP_R15R12           (*(RwReg  *)0x44000C8CU) /**< \brief (PICOP) R15 to 12 */
#define REG_PICOP_R19R16           (*(RwReg  *)0x44000C90U) /**< \brief (PICOP) R19 to 16 */
#define REG_PICOP_R23R20           (*(RwReg  *)0x44000C94U) /**< \brief (PICOP) R23 to 20 */
#define REG_PICOP_R27R24           (*(RwReg  *)0x44000C98U) /**< \brief (PICOP) R27 to 24: XH, XL, R25, R24 */
#define REG_PICOP_R31R28           (*(RwReg  *)0x44000C9CU) /**< \brief (PICOP) R31 to 28: ZH, ZL, YH, YL */
#define REG_PICOP_S1S0             (*(RwReg  *)0x44000CA0U) /**< \brief (PICOP) System Regs 1 to 0: SR */
#define REG_PICOP_S3S2             (*(RwReg  *)0x44000CA4U) /**< \brief (PICOP) System Regs 3 to 2: CTRL */
#define REG_PICOP_S5S4             (*(RwReg  *)0x44000CA8U) /**< \brief (PICOP) System Regs 5 to 4: SREG, CCR */
#define REG_PICOP_S11S10           (*(RwReg  *)0x44000CB4U) /**< \brief (PICOP) System Regs 11 to 10: Immediate */
#define REG_PICOP_LINK             (*(RwReg  *)0x44000CB8U) /**< \brief (PICOP) Link */
#define REG_PICOP_SP               (*(RwReg  *)0x44000CBCU) /**< \brief (PICOP) Stack Pointer */
#define REG_PICOP_MMUFLASH         (*(RwReg  *)0x44000D00U) /**< \brief (PICOP) MMU mapping for flash */
#define REG_PICOP_MMU0             (*(RwReg  *)0x44000D18U) /**< \brief (PICOP) MMU mapping user 0 */
#define REG_PICOP_MMU1             (*(RwReg  *)0x44000D1CU) /**< \brief (PICOP) MMU mapping user 1 */
#define REG_PICOP_MMUCTRL          (*(RwReg  *)0x44000D20U) /**< \brief (PICOP) MMU Control */
#define REG_PICOP_ICACHE           (*(RwReg  *)0x44000D80U) /**< \brief (PICOP) Instruction Cache Control */
#define REG_PICOP_ICACHELRU        (*(RwReg  *)0x44000D84U) /**< \brief (PICOP) Instruction Cache LRU */
#define REG_PICOP_QOSCTRL          (*(RwReg  *)0x44000E00U) /**< \brief (PICOP) QOS Control */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */


#endif /* _SAML21_PICOP_INSTANCE_ */
