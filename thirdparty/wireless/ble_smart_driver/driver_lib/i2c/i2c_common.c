/**
 * \file
 *
 * \brief I2C Driver
 *
 * Copyright (C) 2012-2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <string.h>
#include "i2c.h"
#include "CMSDK_CM0.h"
#include "dma_if.h"
#include "i2c_hw.h"
#include "common.h"
#ifdef CHIPVERSION_B0
#include "reg_common.h"
#endif

enum i2c_status_code _i2c_set_config(
									I2C *const i2c_module,
									uint16_t *pinmux_pad,
									uint32_t baud_rate,
									uint8_t clock_source)
{
	/* Temporary variables. */
	uint8_t idx = 0;
	uint8_t pinnum = 0;
	uint8_t muxval = 0;
	uint32_t regaddr = 0;
	uint32_t regval = 0;
	//enum i2c_status_code tmp_status_code = STATUS_OK;

#if 1
	// Pin Mux programming
	for(idx=0;idx<2;idx++) {
		pinnum = (uint8_t)((pinmux_pad[idx] >> 8) & 0xFF);
		muxval = (uint8_t)(pinmux_pad[idx] & 0xFF);
		if(!port_list[pinnum].bit.available) {
			return I2C_STATUS_ERR_NOT_INITIALIZED;
		} 
		else {
			port_list[pinnum].bit.available = 0;
		}
		if( pinnum <= 7) {
			regaddr = 0x4000b044;
		}
		else if(pinnum <= 15) {
			regaddr = 0x4000b048;
		}
		else if(pinnum <= 23) {
			regaddr = 0x4000b04c;
		}
		regval = REG_RD(regaddr);
		regval &= ~(7 << ((pinnum%8)*4));
		regval |= (muxval << ((pinnum%8)*4));
		REG_WR(regaddr,regval);
	}
#else
	regaddr = 0x4000b048;
	regval = REG_RD(regaddr);
	regval &= ~((0x7 << 24) | (0x7 << 28));
	regval |= ((1 << 24) | (1 << 28));
	REG_WR(regaddr,regval);
	regaddr = 0x4000b1AC;
	regval = REG_RD(regaddr);
	regval &= ~((0x1f << 24) | (0x1f << 16));
	regval |= ((9 << 24) | (8 << 16));
	REG_WR(regaddr,regval);
#endif
	
	/* Find and set baudrate. */
	i2c_module->CLKSEL.reg = I2C_CLK_INPUT_0;
#ifdef CHIPVERSION_B0
	i2c_module->CLKDIVIDER.reg = 0x09;
#else
	i2c_module->CLKDIVIDER.reg = 0x10;
#endif
	
	
	return I2C_STATUS_OK;
}

enum i2c_status_code _i2c_reset(I2C *const i2c_module)
{
	uint32_t regval;
	if(i2c_module == (I2C *)I2C1_BASE) {
		regval = REG_RD(0x4000B004);
		regval &= ~(((1 << 8)) | (1 << 9));
		REG_WR(0x4000B004,regval);
		regval |= ((1 << 8) | (1 << 9));
		REG_WR(0x4000B004,regval);
	}
#ifdef CHIPVERSION_B0
	else if(i2c_module == (I2C *)I2C2_BASE) {
		regval = REG_RD(LPMCU_CORTEX_MISC_REGS_LPMCU_GLOBAL_RESET_1);
		regval &= ~((LPMCU_CORTEX_MISC_REGS_LPMCU_GLOBAL_RESET_1_CORTUS_I2C1_CORE_RSTN) |
								(LPMCU_CORTEX_MISC_REGS_LPMCU_GLOBAL_RESET_1_CORTUS_I2C1_IF_RSTN));
		REG_WR(LPMCU_CORTEX_MISC_REGS_LPMCU_GLOBAL_RESET_1,regval);
		regval |= ((LPMCU_CORTEX_MISC_REGS_LPMCU_GLOBAL_RESET_1_CORTUS_I2C1_CORE_RSTN) |
								(LPMCU_CORTEX_MISC_REGS_LPMCU_GLOBAL_RESET_1_CORTUS_I2C1_IF_RSTN));
		REG_WR(LPMCU_CORTEX_MISC_REGS_LPMCU_GLOBAL_RESET_1,regval);
	}
#endif
	return I2C_STATUS_OK;
}
