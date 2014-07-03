/**
 * \file
 *
 * \brief Component description for DSUSTANDBY
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

#ifndef _SAML21_DSUSTANDBY_COMPONENT_
#define _SAML21_DSUSTANDBY_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR DSUSTANDBY */
/* ========================================================================== */
/** \addtogroup SAML21_DSUSTANDBY Device Service Unit */
/*@{*/

#define DSUSTANDBY_U2209
#define REV_DSUSTANDBY              0x100

/* -------- DSUSTANDBY_DCFG : (DSUSTANDBY Offset: 0xF0) (R/W 32) Device Configuration -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} DSUSTANDBY_DCFG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSUSTANDBY_DCFG_OFFSET      0xF0         /**< \brief (DSUSTANDBY_DCFG offset) Device Configuration */
#define DSUSTANDBY_DCFG_RESETVALUE  0x00000000   /**< \brief (DSUSTANDBY_DCFG reset_value) Device Configuration */

#define DSUSTANDBY_DCFG_MASK        0x00000000u  /**< \brief (DSUSTANDBY_DCFG) MASK Register */

/** \brief DSUSTANDBY hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
       RoReg8                    Reserved1[0xF0];
  __IO DSUSTANDBY_DCFG_Type      DCFG[2];     /**< \brief Offset: 0xF0 (R/W 32) Device Configuration */
} Dsustandby;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAML21_DSUSTANDBY_COMPONENT_ */
