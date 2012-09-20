/**
 * \file
 *
 * \brief SAM Liquid Crystal Display driver (LCDCA).
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

#include "compiler.h"
#include "lcdca.h"

/**
 * \ingroup lcdca_group
 *
 * @{
 */


/**
 * \brief LCD clock initialization.
 *
 * This function enables the specified LCD clock (RC32K or OSC32K).
 *
 * The function enables the LCD input clock.
 * Then the module source clock for the LCD will be the same than for the AST.
 *
 */
void lcdca_clk_init(void)
{
	//Enable APB clock
	sysclk_enable_peripheral_clock(LCDCA);

	//Switch LCD input to selected source clock
	bpm_set_clk32_source(BPM,LCDCA_SOURCE_CLK);

#if (LCDCA_SOURCE_CLK == BPM_CLK32_SOURCE_OSC32K)
	bscif_enable_osc32_crystal(BSCIF,true,false);
#else
	bscif_enable_rc32k(BSCIF);
#endif
}

/**
 * \internal
 * \brief Get pixel register from coordinates.
 *
 * Driver-internal function for getting the address of the register which
 * controls the state for the specified pixel coordinate pair.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 *
 * \return Address of register controlling specified pixel.
 */
static uint64_t lcdca_get_pixel_register(uint8_t pix_com)
{
	uint64_t register_value = 0;
	switch (pix_com){
		case 0:
			register_value = (uint64_t)LCDCA->LCDCA_DRL0;
			register_value |= ((uint64_t)LCDCA->LCDCA_DRH0<<32);
		break;
		case 1:
			register_value = (uint64_t)LCDCA->LCDCA_DRL1;
			register_value |= ((uint64_t)LCDCA->LCDCA_DRH1<<32);
		break; 
		case 2:
			register_value = (uint64_t)LCDCA->LCDCA_DRL2;
			register_value |= ((uint64_t)LCDCA->LCDCA_DRH2<<32);
		break;
		case 3:
			register_value = (uint64_t)LCDCA->LCDCA_DRL3;
			register_value |= ((uint64_t)LCDCA->LCDCA_DRH3<<32);
		break;
	}
	return register_value;
}

/**
 * \brief Send a sequence of ASCII characters to LCD device.
 *
 * This function enables LCD segments via the digit decoder.
 * The function will write the maximum number of byte passed as argument,
 * and will stop writing if a NULL character is found.
 *
 * \param  lcd_tdg    Type of digit decoder.
 * \param  first_seg  First SEG where the first data will be writen.
 * \param  data       Data buffer.
 * \param  width      Maximum Number of data.
 * \param  dir        Direction (==0: Left->Right, !=0: Left<-Right).
 */
void lcdca_write_packet(uint8_t lcd_tdg, uint8_t first_seg,
		const uint8_t *data, size_t width, uint8_t dir)
{
	LCDCA->LCDCA_CMCFG = LCDCA_CMCFG_TDG(lcd_tdg) 
		| LCDCA_CMCFG_STSEG(first_seg) 
		| (dir ? LCDCA_CMCFG_DREV_LEFT: LCDCA_CMCFG_DREV_RIGHT);

	while (width--) {
		if (*data == '\0') {
			break; // Stop on NULL char
		}
		LCDCA->LCDCA_CMDR = (*data++);
	}

}

/**
 * \brief Set pixel (icon) in LCD display memory.
 *
 * This function sets a pixel in LCD (icon) display memory. If a parameter
 * is out of range, then the function doesn't set any bit.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 */
void lcdca_set_pixel(uint8_t pix_com, uint8_t pix_seg)
{
	if ((pix_com < LCD_MAX_NR_OF_COM) && (pix_seg < LCD_MAX_NBR_OF_SEG)) {
		uint64_t register_value = lcdca_get_pixel_register(pix_com);
		register_value |= ((uint64_t)1 << pix_seg);
		switch (pix_com){
			case 0:
				LCDCA->LCDCA_DRL0 = register_value;
				LCDCA->LCDCA_DRH0 = (register_value>>32);
			break;
			case 1:
				LCDCA->LCDCA_DRL1 = register_value;
				LCDCA->LCDCA_DRH1 = (register_value>>32);
			break; 
			case 2:
				LCDCA->LCDCA_DRL2 = register_value;
				LCDCA->LCDCA_DRH2 = (register_value>>32);
			break;
			case 3:
				LCDCA->LCDCA_DRL3 = register_value;
				LCDCA->LCDCA_DRH3 = (register_value>>32);
			break;
		}
	}
}


/**
 * \brief Clear pixel (icon) in LCD display memory.
 *
 * This function clears a pixel in LCD (icon) display memory. If a parameter
 * is out of range, then the function doesn't clear any bit.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 */
void lcdca_clear_pixel(uint8_t pix_com, uint8_t pix_seg)
{
	if ((pix_com < LCD_MAX_NR_OF_COM) && (pix_seg < LCD_MAX_NBR_OF_SEG)) {
		uint64_t register_value = lcdca_get_pixel_register(pix_com);
		register_value &= ~((uint64_t)1 << pix_seg);
		switch (pix_com){
			case 0:
				LCDCA->LCDCA_DRL0 = register_value;
				LCDCA->LCDCA_DRH0 = (register_value>>32);
			break;
			case 1:
				LCDCA->LCDCA_DRL1 = register_value;
				LCDCA->LCDCA_DRH1 = (register_value>>32);
			break; 
			case 2:
				LCDCA->LCDCA_DRL2 = register_value;
				LCDCA->LCDCA_DRH2 = (register_value>>32);
			break;
			case 3:
				LCDCA->LCDCA_DRL3 = register_value;
				LCDCA->LCDCA_DRH3 = (register_value>>32);
			break;
		}
	}
}


/**
 * \brief Toggle pixel (icon) in LCD display memory.
 *
 * This function toggles a pixel in LCD (icon) display memory. If a parameter
 * is out of range, then the function doesn't toggle any bit.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 */
void lcdca_tgl_pixel(uint8_t pix_com, uint8_t pix_seg)
{
	if ((pix_com < LCD_MAX_NR_OF_COM) && (pix_seg < LCD_MAX_NBR_OF_SEG)) {
		uint64_t register_value = lcdca_get_pixel_register(pix_com);

		if (register_value & ((uint64_t)1 << pix_seg )) {
			lcdca_clear_pixel(pix_com, pix_seg);
		} else {
			lcdca_set_pixel(pix_com, pix_seg);
		}
	}
}


/**
 * \brief Get pixel value (icon) in LCD display memory.
 *
 * This function returns the pixel value in LCD (icon) display memory.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 */
bool lcdca_get_pixel(uint8_t pix_com, uint8_t pix_seg)
{
	uint64_t register_value = lcdca_get_pixel_register(pix_com);
	return register_value & ((uint64_t)1 << pix_seg );
}

/**
 * @}
 */
