/**
 * \file
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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

#ifndef __SAM0XX_COMPONENT_NVM__
#define __SAM0XX_COMPONENT_NVM__

#include <io.h>

/* WDT module */
typedef struct {
		__REG16(CTRLA, __IO);
		__REG16(reserved_0x02, __I);
		__REG16(CTRLB, __IO);
		__REG16(reserved_0x06, __I);
		__REG32(PARAM, __IO);
		__REG8(INTENCLR, __IO);
		__REG8(INTENSET, __IO);
		__REG8(INTFLAG, __IO);
		__REG8(reserved_0x0f, __I);
		__REG16(STATUS, __IO);
		__REG16(reserved_0x12, __I);
		__REG32(ADDR, __IO);
		__REG16(LOCK, __IO);
		__REG16(reserved_0x1a, __I);
} NVM_t;

#define NVM_CMDEX_EXECUTION_KEY 0xA5

#define NVM_CMD_ER_gc     (0x02 << 0x00)
#define NVM_CMD_WP_gc     (0x04 << 0x00)
#define NVM_CMD_EAR_gc    (0x05 << 0x00)
#define NVM_CMD_WAP_gc    (0x06 << 0x00)
#define NVM_CMD_WL_gc     (0x0f << 0x00)
#define NVM_CMD_LR_gc     (0x40 << 0x00)
#define NVM_CMD_UR_gc     (0x41 << 0x00)
#define NVM_CMD_SPRM_gc   (0x42 << 0x00)
#define NVM_CMD_CPRM_gc   (0x43 << 0x00)
#define NVM_CMD_PBC_gc    (0x44 << 0x00)
#define NVM_CMD_SSB_gc    (0x45 << 0x00)

#define NVM_WAKEUP_gp         8
#define NVM_WAKEONACCESS_gc  (0x00 << NVM_WAKEUP_gp)
#define NVM_WAKEUPINSTANT_gc (0x01 << NVM_WAKEUP_gp)
#define NVM_DISABLED_gc      (0x11 << NVM_WAKEUP_gp)

#define NVM_AUTO_WAITSTATES_DISABLED_bm 0x00


#define NVM_SLEEP_ENABLE_gp     8
#define NVM_MAN_PAGE_WRITE_bp   7
#define NVM_MAN_PAGE_WRITE_bm   0x00000008
#define NVM_AUTO_WAIT_STATE_bp  1
#define NVM_WAIT_STATES_gp      0
#define NVM_SECURITY_BIT_bm     0x0100
#define NVM_AUX_ADDRESS         0x00200000
#define NVM_AUX_BOOTPROT_gp     1
#define NVM_AUX_EEPROM_gp       4


#define NVM_COMMAND_gp          0
#define NVM_CMDEX_gp            8

#define NVM_PSZ_gm              0x00070000
#define NVM_NVMP_gm             0x00001111
#define NVM_PSZ_gp              16
#define NVM_NVMP_gp             0
#define NVM_INTFLAG_READY_bm    0x0001

#define NVM_COMMAND_KEY         0xA5
#define NVM_ERRORS_gp           2
#define NVM_ERRORS_gm           (0x3 << NVM_ERRORS_gp)

#endif /* __SAM0XX_COMPONENT_NVM__ */
