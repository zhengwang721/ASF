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

#ifndef _SAMD20_HMATRIX_INSTANCE_
#define _SAMD20_HMATRIX_INSTANCE_

/* ========== Register definition for HMATRIX peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_HMATRIX_MCFG           (0x41004800U) /**< \brief (HMATRIX) Master Configuration Register */
#define REG_HMATRIX_SCFG           (0x41004840U) /**< \brief (HMATRIX) Slave Configuration Register */
#define REG_HMATRIX_PRAS0          (0x41004880U) /**< \brief (HMATRIX) Priority Register A for Slave 0 */
#define REG_HMATRIX_PRBS0          (0x41004884U) /**< \brief (HMATRIX) Priority Register B for Slave 0 */
#define REG_HMATRIX_PRAS1          (0x41004888U) /**< \brief (HMATRIX) Priority Register A for Slave 1 */
#define REG_HMATRIX_PRBS1          (0x4100488CU) /**< \brief (HMATRIX) Priority Register B for Slave 1 */
#define REG_HMATRIX_PRAS2          (0x41004890U) /**< \brief (HMATRIX) Priority Register A for Slave 2 */
#define REG_HMATRIX_PRBS2          (0x41004894U) /**< \brief (HMATRIX) Priority Register B for Slave 2 */
#define REG_HMATRIX_PRAS3          (0x41004898U) /**< \brief (HMATRIX) Priority Register A for Slave 3 */
#define REG_HMATRIX_PRBS3          (0x4100489CU) /**< \brief (HMATRIX) Priority Register B for Slave 3 */
#define REG_HMATRIX_PRAS4          (0x410048A0U) /**< \brief (HMATRIX) Priority Register A for Slave 4 */
#define REG_HMATRIX_PRBS4          (0x410048A4U) /**< \brief (HMATRIX) Priority Register B for Slave 4 */
#define REG_HMATRIX_PRAS5          (0x410048A8U) /**< \brief (HMATRIX) Priority Register A for Slave 5 */
#define REG_HMATRIX_PRBS5          (0x410048ACU) /**< \brief (HMATRIX) Priority Register B for Slave 5 */
#define REG_HMATRIX_PRAS6          (0x410048B0U) /**< \brief (HMATRIX) Priority Register A for Slave 6 */
#define REG_HMATRIX_PRBS6          (0x410048B4U) /**< \brief (HMATRIX) Priority Register B for Slave 6 */
#define REG_HMATRIX_PRAS7          (0x410048B8U) /**< \brief (HMATRIX) Priority Register A for Slave 7 */
#define REG_HMATRIX_PRBS7          (0x410048BCU) /**< \brief (HMATRIX) Priority Register B for Slave 7 */
#define REG_HMATRIX_PRAS8          (0x410048C0U) /**< \brief (HMATRIX) Priority Register A for Slave 8 */
#define REG_HMATRIX_PRBS8          (0x410048C4U) /**< \brief (HMATRIX) Priority Register B for Slave 8 */
#define REG_HMATRIX_PRAS9          (0x410048C8U) /**< \brief (HMATRIX) Priority Register A for Slave 9 */
#define REG_HMATRIX_PRBS9          (0x410048CCU) /**< \brief (HMATRIX) Priority Register B for Slave 9 */
#define REG_HMATRIX_PRAS10         (0x410048D0U) /**< \brief (HMATRIX) Priority Register A for Slave 10 */
#define REG_HMATRIX_PRBS10         (0x410048D4U) /**< \brief (HMATRIX) Priority Register B for Slave 10 */
#define REG_HMATRIX_PRAS11         (0x410048D8U) /**< \brief (HMATRIX) Priority Register A for Slave 11 */
#define REG_HMATRIX_PRBS11         (0x410048DCU) /**< \brief (HMATRIX) Priority Register B for Slave 11 */
#define REG_HMATRIX_PRAS12         (0x410048E0U) /**< \brief (HMATRIX) Priority Register A for Slave 12 */
#define REG_HMATRIX_PRBS12         (0x410048E4U) /**< \brief (HMATRIX) Priority Register B for Slave 12 */
#define REG_HMATRIX_PRAS13         (0x410048E8U) /**< \brief (HMATRIX) Priority Register A for Slave 13 */
#define REG_HMATRIX_PRBS13         (0x410048ECU) /**< \brief (HMATRIX) Priority Register B for Slave 13 */
#define REG_HMATRIX_PRAS14         (0x410048F0U) /**< \brief (HMATRIX) Priority Register A for Slave 14 */
#define REG_HMATRIX_PRBS14         (0x410048F4U) /**< \brief (HMATRIX) Priority Register B for Slave 14 */
#define REG_HMATRIX_PRAS15         (0x410048F8U) /**< \brief (HMATRIX) Priority Register A for Slave 15 */
#define REG_HMATRIX_PRBS15         (0x410048FCU) /**< \brief (HMATRIX) Priority Register B for Slave 15 */
#define REG_HMATRIX_MRCR           (0x41004900U) /**< \brief (HMATRIX) Master Remap Control Register */
#define REG_HMATRIX_SFR            (0x41004910U) /**< \brief (HMATRIX) Special Function Register */
#else
#define REG_HMATRIX_MCFG           (*(RwReg  *)0x41004800U) /**< \brief (HMATRIX) Master Configuration Register */
#define REG_HMATRIX_SCFG           (*(RwReg  *)0x41004840U) /**< \brief (HMATRIX) Slave Configuration Register */
#define REG_HMATRIX_PRAS0          (*(RwReg  *)0x41004880U) /**< \brief (HMATRIX) Priority Register A for Slave 0 */
#define REG_HMATRIX_PRBS0          (*(RwReg  *)0x41004884U) /**< \brief (HMATRIX) Priority Register B for Slave 0 */
#define REG_HMATRIX_PRAS1          (*(RwReg  *)0x41004888U) /**< \brief (HMATRIX) Priority Register A for Slave 1 */
#define REG_HMATRIX_PRBS1          (*(RwReg  *)0x4100488CU) /**< \brief (HMATRIX) Priority Register B for Slave 1 */
#define REG_HMATRIX_PRAS2          (*(RwReg  *)0x41004890U) /**< \brief (HMATRIX) Priority Register A for Slave 2 */
#define REG_HMATRIX_PRBS2          (*(RwReg  *)0x41004894U) /**< \brief (HMATRIX) Priority Register B for Slave 2 */
#define REG_HMATRIX_PRAS3          (*(RwReg  *)0x41004898U) /**< \brief (HMATRIX) Priority Register A for Slave 3 */
#define REG_HMATRIX_PRBS3          (*(RwReg  *)0x4100489CU) /**< \brief (HMATRIX) Priority Register B for Slave 3 */
#define REG_HMATRIX_PRAS4          (*(RwReg  *)0x410048A0U) /**< \brief (HMATRIX) Priority Register A for Slave 4 */
#define REG_HMATRIX_PRBS4          (*(RwReg  *)0x410048A4U) /**< \brief (HMATRIX) Priority Register B for Slave 4 */
#define REG_HMATRIX_PRAS5          (*(RwReg  *)0x410048A8U) /**< \brief (HMATRIX) Priority Register A for Slave 5 */
#define REG_HMATRIX_PRBS5          (*(RwReg  *)0x410048ACU) /**< \brief (HMATRIX) Priority Register B for Slave 5 */
#define REG_HMATRIX_PRAS6          (*(RwReg  *)0x410048B0U) /**< \brief (HMATRIX) Priority Register A for Slave 6 */
#define REG_HMATRIX_PRBS6          (*(RwReg  *)0x410048B4U) /**< \brief (HMATRIX) Priority Register B for Slave 6 */
#define REG_HMATRIX_PRAS7          (*(RwReg  *)0x410048B8U) /**< \brief (HMATRIX) Priority Register A for Slave 7 */
#define REG_HMATRIX_PRBS7          (*(RwReg  *)0x410048BCU) /**< \brief (HMATRIX) Priority Register B for Slave 7 */
#define REG_HMATRIX_PRAS8          (*(RwReg  *)0x410048C0U) /**< \brief (HMATRIX) Priority Register A for Slave 8 */
#define REG_HMATRIX_PRBS8          (*(RwReg  *)0x410048C4U) /**< \brief (HMATRIX) Priority Register B for Slave 8 */
#define REG_HMATRIX_PRAS9          (*(RwReg  *)0x410048C8U) /**< \brief (HMATRIX) Priority Register A for Slave 9 */
#define REG_HMATRIX_PRBS9          (*(RwReg  *)0x410048CCU) /**< \brief (HMATRIX) Priority Register B for Slave 9 */
#define REG_HMATRIX_PRAS10         (*(RwReg  *)0x410048D0U) /**< \brief (HMATRIX) Priority Register A for Slave 10 */
#define REG_HMATRIX_PRBS10         (*(RwReg  *)0x410048D4U) /**< \brief (HMATRIX) Priority Register B for Slave 10 */
#define REG_HMATRIX_PRAS11         (*(RwReg  *)0x410048D8U) /**< \brief (HMATRIX) Priority Register A for Slave 11 */
#define REG_HMATRIX_PRBS11         (*(RwReg  *)0x410048DCU) /**< \brief (HMATRIX) Priority Register B for Slave 11 */
#define REG_HMATRIX_PRAS12         (*(RwReg  *)0x410048E0U) /**< \brief (HMATRIX) Priority Register A for Slave 12 */
#define REG_HMATRIX_PRBS12         (*(RwReg  *)0x410048E4U) /**< \brief (HMATRIX) Priority Register B for Slave 12 */
#define REG_HMATRIX_PRAS13         (*(RwReg  *)0x410048E8U) /**< \brief (HMATRIX) Priority Register A for Slave 13 */
#define REG_HMATRIX_PRBS13         (*(RwReg  *)0x410048ECU) /**< \brief (HMATRIX) Priority Register B for Slave 13 */
#define REG_HMATRIX_PRAS14         (*(RwReg  *)0x410048F0U) /**< \brief (HMATRIX) Priority Register A for Slave 14 */
#define REG_HMATRIX_PRBS14         (*(RwReg  *)0x410048F4U) /**< \brief (HMATRIX) Priority Register B for Slave 14 */
#define REG_HMATRIX_PRAS15         (*(RwReg  *)0x410048F8U) /**< \brief (HMATRIX) Priority Register A for Slave 15 */
#define REG_HMATRIX_PRBS15         (*(RwReg  *)0x410048FCU) /**< \brief (HMATRIX) Priority Register B for Slave 15 */
#define REG_HMATRIX_MRCR           (*(RwReg  *)0x41004900U) /**< \brief (HMATRIX) Master Remap Control Register */
#define REG_HMATRIX_SFR            (*(RwReg  *)0x41004910U) /**< \brief (HMATRIX) Special Function Register */
#endif /* (defined(__ASSEMBLY__) || defines(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for HMATRIX ========== */
#define HMATRIX_SLAVE_FLASH         0
#define HMATRIX_SLAVE_HPB0          1
#define HMATRIX_SLAVE_HPB1          2
#define HMATRIX_SLAVE_HPB2          3
#define HMATRIX_SLAVE_HRAMC0        4
#define HMATRIX_SLAVE_NUM           5

#define HMATRIX_MASTER_CM0P         0
#define HMATRIX_MASTER_DSU          1
#define HMATRIX_MASTER_NUM          2

#endif /* _SAMD20_HMATRIX_INSTANCE_ */
