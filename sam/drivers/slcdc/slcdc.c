/**
 * \file
 *
 * \brief SAM Segment LCD Controller (SLCDC).
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

#include "slcdc.h"
#include "sysclk.h"
#include "sleepmgr.h"

/**
 * \ingroup group_sam_drivers_slcdc
 *
 * @{
 */

/*each bit mask of uint32 */
static uint32_t mask[32];

const enum symbol_dot Symbol_D0[7] = PATTERN_SYMBOL_D0;
const enum symbol_dot Symbol_D1[7] = PATTERN_SYMBOL_D1;
const enum symbol_dot Symbol_D2[7] = PATTERN_SYMBOL_D2;
const enum symbol_dot Symbol_D3[7] = PATTERN_SYMBOL_D3;

const enum symbol_dot Symbol_A2[14] = PATTERN_SYMBOL_A2;	
const enum symbol_dot Symbol_A3[14] = PATTERN_SYMBOL_A3;	
const enum symbol_dot Symbol_A4[14] = PATTERN_SYMBOL_A4;	
const enum symbol_dot Symbol_A5[14] = PATTERN_SYMBOL_A5;	
const enum symbol_dot Symbol_A6[14] = PATTERN_SYMBOL_A6;	

const uint8_t Plot_Number[10] = {PLOT_NUMBER_0, PLOT_NUMBER_1, PLOT_NUMBER_2,
		PLOT_NUMBER_3, PLOT_NUMBER_4, PLOT_NUMBER_5, PLOT_NUMBER_6, PLOT_NUMBER_7,
		PLOT_NUMBER_8, PLOT_NUMBER_9};
const uint32_t Plot_Letter[26] = {PLOT_LETTER_A, PLOT_LETTER_B, PLOT_LETTER_C, PLOT_LETTER_D,
		PLOT_LETTER_E, PLOT_LETTER_F, PLOT_LETTER_G, PLOT_LETTER_H, PLOT_LETTER_I,
		PLOT_LETTER_J, PLOT_LETTER_K, PLOT_LETTER_L, PLOT_LETTER_M, PLOT_LETTER_N,
		PLOT_LETTER_O, PLOT_LETTER_P, PLOT_LETTER_Q, PLOT_LETTER_R, PLOT_LETTER_S,
		PLOT_LETTER_T, PLOT_LETTER_U, PLOT_LETTER_V, PLOT_LETTER_W, PLOT_LETTER_X,
		PLOT_LETTER_Y, PLOT_LETTER_Z};
const uint32_t Plot_Letter_Num[10] = {PLOT_LETTER_0, PLOT_LETTER_1, PLOT_LETTER_2, PLOT_LETTER_3,
		PLOT_LETTER_4, PLOT_LETTER_5, PLOT_LETTER_6, PLOT_LETTER_7, PLOT_LETTER_8, PLOT_LETTER_9};

/**
 * \internal
 * \brief SLCDC callback function pointer.
 */
slcdc_callback_t slcdc_callback_pointer = NULL;

static void init_mask(void)
{
	uint32_t tmp = 1;
	uint8_t index = 0;
	for (index =0; index <32; index ++) {
		mask[index] = tmp;
		tmp <<=1;
	}
}

status_code_t slcdc_init(Slcdc *p_slcdc, struct slcdc_config *slcdc_cfg)
{
	if(slcdc_get_status(p_slcdc) == SLCDC_SR_ENA) {
		return ERR_BUSY;
	}
	init_mask();
	supc_set_slcd_power_mode(SUPC, slcdc_cfg->power_mode);
	supc_set_slcd_vol(SUPC, slcdc_cfg->vlotage);
	sysclk_enable_peripheral_clock(ID_SLCDC);
	slcdc_reset(p_slcdc);
	p_slcdc->SLCDC_MR = (SLCDC_MR_BIAS_Msk & (CONF_SLCDC_BIAS << SLCDC_MR_BIAS_Pos))	|
			(SLCDC_MR_COMSEL_Msk & (CONF_SLCDC_COM_NUM << SLCDC_MR_COMSEL_Pos)) |
			SLCDC_MR_SEGSEL(CONF_SLCDC_SEG_NUM) |
			slcdc_cfg->buf_time | slcdc_cfg->is_low_power;
	p_slcdc->SLCDC_FRR = slcdc_cfg->pres | slcdc_cfg->div;
	p_slcdc->SLCDC_DR = SLCDC_DR_LCDBLKFREQ(slcdc_cfg->blink_freq) | slcdc_cfg->disp_mode;
	return STATUS_OK;
}

void slcdc_enable(Slcdc *p_slcdc)
{
	sleepmgr_lock_mode(SLEEPMGR_WAIT);
	p_slcdc->SLCDC_CR = SLCDC_CR_LCDEN;
}

void slcdc_disable(Slcdc *p_slcdc)
{
	sleepmgr_unlock_mode(SLEEPMGR_WAIT);
	p_slcdc->SLCDC_CR = SLCDC_CR_LCDDIS;
}

/**
 * \brief Turn on a symbol dot.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param symbol_dot Symbol dot value.
 */
static void slcdc_symbol_dot_on(Slcdc *p_slcdc, uint8_t symbol_dot)
{
	uint8_t seg =  symbol_dot >> 0x2;
	if (seg < 32)
		slcdc_set_lsb_memory(p_slcdc,symbol_dot & 0x3, mask[seg]);
	else
		slcdc_set_msb_memory(p_slcdc,symbol_dot & 0x3, mask[seg & 0x1F]);
}

/**
 * \brief Turn off a symbol dot.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param symbol_dot Symbol dot value.
 */
static void slcdc_symbol_dot_off(Slcdc *p_slcdc, uint8_t symbol_dot)
{
	uint8_t seg =  symbol_dot >> 0x2;
	if (seg < 32)
		slcdc_clear_lsb_memory(p_slcdc, symbol_dot & 0x3, mask[seg]);
	else
		slcdc_clear_msb_memory(p_slcdc, symbol_dot & 0x3, mask[seg & 0x1F]);
}

void slcdc_set_pixel(uint8_t pix_com, uint8_t pix_seg)
{
	slcdc_symbol_dot_on(SLCDC, 4 * pix_seg + pix_com);
}

void slcdc_clear_pixel(uint8_t pix_com, uint8_t pix_seg)
{
	slcdc_symbol_dot_off(SLCDC, 4 * pix_seg + pix_com);
}

void slcdc_display_symbol(Slcdc *p_slcdc, const enum symbol_dot *p_symbol,
		uint8_t number, uint32_t plot)
{
	while(number--) {
		if (plot & mask[number])
			slcdc_symbol_dot_on(p_slcdc, *(p_symbol+number));
		else
			slcdc_symbol_dot_off(p_slcdc, *(p_symbol+number));
	}
}

void slcdc_efface_symbol(Slcdc *p_slcdc, const enum symbol_dot *p_symbol,
		uint8_t number)
{
	while(number --) {
		slcdc_symbol_dot_off(p_slcdc, *(p_symbol+number));
	}
}

void slcdc_set_display_memory(Slcdc *p_slcdc)
{
	uint8_t tmp =  LCD_COM_NB;
	while(tmp--) {
		slcdc_set_lsb_memory(p_slcdc, tmp, ~0UL);
		slcdc_set_msb_memory(p_slcdc, tmp, ~0UL);
	}
}

void slcdc_clear_display_memory(Slcdc *p_slcdc)
{
	uint8_t tmp =  LCD_COM_NB;
	while(tmp--) {
		slcdc_clear_lsb_memory(p_slcdc, tmp, ~0UL);
		slcdc_clear_msb_memory(p_slcdc, tmp, ~0UL);
	}
}

void slcdc_display_alphanum_string(const uint8_t *data)
{
	if(data[0] == 0x20) {
		slcdc_efface_symbol(SLCDC, Symbol_A2, sizeof(Symbol_A2));
	} else if((0x30 < data[0]) && (data[0] < 0x41)) {
		slcdc_display_symbol(SLCDC, Symbol_A2, sizeof(Symbol_A2), Plot_Letter_Num[data[0] - 0x30]);
	} else if((0x40 < data[0]) && (data[0] < 0x61)) {
		slcdc_display_symbol(SLCDC, Symbol_A2, sizeof(Symbol_A2), Plot_Letter[data[0] - 0x41]);
	} else {
		slcdc_display_symbol(SLCDC, Symbol_A2, sizeof(Symbol_A2), Plot_Letter[data[0] - 0x61]);
	}
	if(data[1] == 0x20) {
		slcdc_efface_symbol(SLCDC, Symbol_A3, sizeof(Symbol_A3));
	} else if((0x30 < data[1]) && (data[1] < 0x41)) {
		slcdc_display_symbol(SLCDC, Symbol_A3, sizeof(Symbol_A3), Plot_Letter_Num[data[1] - 0x30]);
	} else if((0x40 < data[1]) && (data[1] < 0x61)) {
		slcdc_display_symbol(SLCDC, Symbol_A3, sizeof(Symbol_A3), Plot_Letter[data[1] - 0x41]);
	} else {
		slcdc_display_symbol(SLCDC, Symbol_A3, sizeof(Symbol_A3), Plot_Letter[data[1] - 0x61]);
	} 
	if(data[2] == 0x20) {
		slcdc_efface_symbol(SLCDC, Symbol_A4, sizeof(Symbol_A4));
	} else if((0x30 < data[2]) && (data[2] < 0x41)) {
		slcdc_display_symbol(SLCDC, Symbol_A4, sizeof(Symbol_A4), Plot_Letter_Num[data[2] - 0x30]);
	} else if((0x40 < data[2]) && (data[2] < 0x61)) {
		slcdc_display_symbol(SLCDC, Symbol_A4, sizeof(Symbol_A4), Plot_Letter[data[2] - 0x41]);
	} else {
		slcdc_display_symbol(SLCDC, Symbol_A4, sizeof(Symbol_A4), Plot_Letter[data[2] - 0x61]);
	} 
	if(data[3] == 0x20) {
		slcdc_efface_symbol(SLCDC, Symbol_A5, sizeof(Symbol_A5));
	} else if((0x30 < data[3]) && (data[3] < 0x41)) {
		slcdc_display_symbol(SLCDC, Symbol_A5, sizeof(Symbol_A5), Plot_Letter_Num[data[3] - 0x30]);
	} else if((0x40 < data[3]) && (data[3] < 0x61)) {
		slcdc_display_symbol(SLCDC, Symbol_A5, sizeof(Symbol_A5), Plot_Letter[data[3] - 0x41]);
	} else {
		slcdc_display_symbol(SLCDC, Symbol_A5, sizeof(Symbol_A5), Plot_Letter[data[3] - 0x61]);
	} 
	if(data[4] == 0x20) {
		slcdc_efface_symbol(SLCDC, Symbol_A6, sizeof(Symbol_A6));
	} else if((0x30 < data[4]) && (data[4] < 0x41)) {
		slcdc_display_symbol(SLCDC, Symbol_A6, sizeof(Symbol_A6), Plot_Letter_Num[data[4] - 0x30]);
	} else if((0x40 < data[4]) && (data[4] < 0x61)) {
		slcdc_display_symbol(SLCDC, Symbol_A6, sizeof(Symbol_A6), Plot_Letter[data[4] - 0x41]);
	} else {
		slcdc_display_symbol(SLCDC, Symbol_A6, sizeof(Symbol_A6), Plot_Letter[data[4] - 0x61]);
	} 	
}

void slcdc_display_num_string(const uint8_t *data)
{
	if(data[0] == 0x20) {
		slcdc_efface_symbol(SLCDC, Symbol_D0, sizeof(Symbol_D0));
	} else {	
		slcdc_display_symbol(SLCDC, Symbol_D0, sizeof(Symbol_D0), Plot_Number[data[0] - 0x30]);
	}
	if(data[1] == 0x20) {
		slcdc_efface_symbol(SLCDC, Symbol_D1, sizeof(Symbol_D1));
	} else {	
		slcdc_display_symbol(SLCDC, Symbol_D1, sizeof(Symbol_D1), Plot_Number[data[1] - 0x30]);
	}
	if(data[2] == 0x20) {
		slcdc_efface_symbol(SLCDC, Symbol_D2, sizeof(Symbol_D2));
	} else {	
		slcdc_display_symbol(SLCDC, Symbol_D2, sizeof(Symbol_D2), Plot_Number[data[2] - 0x30]);
	}
	if(data[3] == 0x20) {
		slcdc_efface_symbol(SLCDC, Symbol_D3, sizeof(Symbol_D3));
	} else {	
		slcdc_display_symbol(SLCDC, Symbol_D3, sizeof(Symbol_D3), Plot_Number[data[3] - 0x30]);
	}	
}

void slcdc_set_callback(Slcdc *p_slcdc, slcdc_callback_t callback,
	uint32_t ul_sources, uint8_t irq_line, uint8_t irq_level)
{
	slcdc_callback_pointer = callback;
	NVIC_ClearPendingIRQ((IRQn_Type)irq_line);
	NVIC_SetPriority((IRQn_Type)irq_line, irq_level);
	NVIC_EnableIRQ((IRQn_Type)irq_line);
	slcdc_enable_interrupt(p_slcdc, ul_sources);
}

void SLCDC_Handler(void)
{
	/* Interrupt handler */
	if (slcdc_callback_pointer != NULL) {
		slcdc_callback_pointer();
	}
}

/**
 * @}
 */
