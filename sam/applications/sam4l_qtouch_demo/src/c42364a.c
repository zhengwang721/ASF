/**
 * \file
 *
 * \brief Management of C42364A LCD Glass component.
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

#include "compiler.h"
#include "c42364a.h"
#include "lcdca.h"

void c42364a_init(void)
{
	// LCD_initialization
	lcdca_clk_init();
	lcdca_connection_init(PORT_MASK, X_BIAS);
	lcdca_blink_init(LCDCA_TIMER_FC0, true);;
	lcdca_csr_init(LCDCA_TIMER_FC0,LCD_CSR_RIGHT,7,0x03);
	lcdca_enable();
	lcdca_timer_init(0,3,2,2,1);
	lcdca_enable_timer(LCDCA_TIMER_FC0);
	lcdca_enable_timer(LCDCA_TIMER_FC1);
	lcdca_enable_timer(LCDCA_TIMER_FC2);
}

void c42364a_battery_graph(uint8_t val)
{
	lcdca_clear_pixel(ICON_BAT_LEVEL_1);
	lcdca_clear_pixel(ICON_BAT_LEVEL_2);
	lcdca_clear_pixel(ICON_BAT_LEVEL_3);
	lcdca_set_pixel(ICON_BAT);
	if (val>2) {
		lcdca_set_pixel(ICON_BAT_LEVEL_3);
	}
	if (val>1) {
		lcdca_set_pixel(ICON_BAT_LEVEL_2);
	}
	if (val>0) {
		lcdca_set_pixel(ICON_BAT_LEVEL_1);
	}
}

void c42364a_set_numeric_dec(uint16_t val)
{
	uint8_t lcd_num[5];

	sprintf((char*)lcd_num, "%d", val);

	//! Write to character generator
	c42364a_write_num_packet((uint8_t const*)&lcd_num);
}
