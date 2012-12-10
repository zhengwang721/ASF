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

#ifndef __SAM0XX_INSTANCE_TC__
#define __SAM0XX_INSTANCE_TC__


#define TC0_ADDR          (0x42002000U)                  /**< \brief (TC0) APB Base Address */
#define TC0               CAST(Tc,        TC0_ADDR)     
#define TC1_ADDR          (0x42002400U)                  /**< \brief (TC1) APB Base Address */
#define TC1               CAST(Tc,        TC1_ADDR)     
#define TC2_ADDR          (0x42002800U)                  /**< \brief (TC2) APB Base Address */
#define TC2               CAST(Tc,        TC2_ADDR)     
#define TC3_ADDR          (0x42002C00U)                  /**< \brief (TC3) APB Base Address */
#define TC3               CAST(Tc,        TC3_ADDR)     
#define TC4_ADDR          (0x42003000U)                  /**< \brief (TC4) APB Base Address */
#define TC4               CAST(Tc,        TC4_ADDR)     
#define TC5_ADDR          (0x42003400U)                  /**< \brief (TC5) APB Base Address */
#define TC5               CAST(Tc,        TC5_ADDR)     
#define TC6_ADDR          (0x42003800U)                  /**< \brief (TC6) APB Base Address */
#define TC6               CAST(Tc,        TC6_ADDR)     
#define TC7_ADDR          (0x42003C00U)                  /**< \brief (TC7) APB Base Address */
#define TC7               CAST(Tc,        TC7_ADDR)     
#define TC_INST_NUM       8                              /**< \brief (TC) Number of instances */
#define TC_INSTS          { TC0, TC1, TC2, TC3, TC4, TC5, TC6, TC7 } /**< \brief (TC) Instances List */

#endif /* __SAM0XX_INSTANCE_TC__ */
