/**
 * \file
 *
 * \brief I2C Driver for SAMB11
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "i2c_common.h"

enum status_code _i2c_set_config(
								I2C *const i2c_module,
								uint16_t *pinmux_pad,
								uint32_t baud_rate,
								uint8_t clock_source)
{
	uint8_t idx = 0;
	
	/* Set the pinmux for this i2c module. */
	for(idx = 0; idx < 2; idx++) {
		gpio_pinmux_cofiguration(pinmux_pad[idx], GPIO_PINMUX_SEL_2);
	}
	
	/* Find and set baudrate. */
	i2c_module->CLOCK_SOURCE_SELECT.reg = clock_source;
	i2c_module->I2C_CLK_DIVIDER.reg = 0x10;
	
	return STATUS_OK;
}

enum status_code _i2c_reset(I2C *const i2c_module)
{
	if (i2c_module == (I2C *)I2C0) {
		LPMCU_MISC_REGS0->LPMCU_GLOBAL_RESET_0.reg &= 
								~(LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_CORTUS_I2C0_CORE_RSTN |
								LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_CORTUS_I2C0_IF_RSTN);
		LPMCU_MISC_REGS0->LPMCU_GLOBAL_RESET_0.reg |=
								(LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_CORTUS_I2C0_CORE_RSTN |
								LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_CORTUS_I2C0_IF_RSTN);						
	}
#ifdef CHIPVERSION_B0
	else if(i2c_module == (I2C *)I2C1) {
		LPMCU_MISC_REGS0->LPMCU_GLOBAL_RESET_1.reg &=
								~(LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_1_CORTUS_I2C1_CORE_RSTN |
								LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_1_CORTUS_I2C1_IF_RSTN);
		LPMCU_MISC_REGS0->LPMCU_GLOBAL_RESET_1.reg |=
								(LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_1_CORTUS_I2C1_CORE_RSTN |
								LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_1_CORTUS_I2C1_IF_RSTN);
	}
#endif

	return STATUS_OK;
}
