/**
 * \file
 *
 * \brief SAM Liquid Crystal Display driver (LCDCA).
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
#ifndef _LCDCA_H_
#define _LCDCA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "compiler.h"
#include "sysclk.h"
#include "bpm.h"
#include "bscif.h"	
#include "conf_lcdca.h"
	
/**
 * \defgroup lcdca_group Liquid Crystal Display (LCDCA)
 *
 * This is a driver for configuring, enabling/disabling and use of the on-chip
 * LCD controller.
 *
 * \section dependencies Dependencies
 *
 * The LCD module depends on the following modules:
 * - \ref sysclk_group for LCD clock control.
 * - \ref interrupt_group for enabling or disabling interrupts.
 * - \ref sleepmgr_group to unlock LCD controller
 * @{
 */


//! \name LCD addressing limits
//@{
//! Maximum number of common lines.
#define LCD_MAX_NR_OF_COM  4
//! Maximum number of segment lines.
# define LCD_MAX_NBR_OF_SEG  40
//@}

//! \name LCD Blink Mode
//@{
//! Blink mode for LCD: blink all lcd.
#define LCD_BLINK_FULL       1
//! Blink mode for LCD: only selected segment will blink.
#define LCD_BLINK_SELECTED   0
//@}

//! \name LCD Shift Register Direction
//@{
//! Right Direction.
#define LCD_CSR_RIGHT       1
//! Left Direction.
#define LCD_CSR_LEFT        0
//@}

//! \name LCD Type of digit
//@{
//! 7-segment with 3 common terminals.
#define TDG_7SEG3COM        0
//! 7-segment with 4 common terminals.
#define TDG_7SEG4COM        1
//! 14-segment with 4 common terminals.
#define TDG_14SEG4COM       2
//! 16-segment with 3 common terminals.
#define TDG_16SEG3COM       3
//@}

//! \name LCDCA Timer Ressource
//@{
//! Timer FCO ressource.
#define LCDCA_TIMER_FC0        0
//! Timer FC1 ressource.
#define LCDCA_TIMER_FC1        1
//! Timer FC2 ressource.
#define LCDCA_TIMER_FC2        2
//@}

//! \name LCDCA CMCFG
//@{
#define LCDCA_CMCFG_DREV_LEFT   1
#define LCDCA_CMCFG_DREV_RIGHT  0
//@}

//! \name LCDCA Scrolling DMA Channel
//@{
#ifndef LCDCA_SCROLLING_DMA_CH
#define LCDCA_SCROLLING_DMA_CH  3
#endif
//@}

//! \name LCDCA Scrolling Mode
//@{
//! Inversed Scrolling.
#define LCDCA_SCROLL_INV        1
//! Non nversed Scrolling.
#define LCDCA_SCROLL_NON_INV    0
//@}

/**
 * \brief LCDCA glass connection initialization.
 *
 * This function sets the LCDCA glass connection.
 *
 * The function enables COM & SEG swapping, LCD port mask and the using (or no)
 * of an External Resistor Bias Generation. This function also clears
 * all the display memory (=> all pixels "off") and lets the LCD disabled.
 *
 * \param  port_mask Number of SEG used.
 * \param  x_bias    External bias (false: internal gen., true: external gen.).
 */
static inline void lcdca_connection_init(uint8_t port_mask, bool x_bias)
{
	LCDCA->LCDCA_CFG = LCDCA_CFG_NSU(port_mask) | (x_bias ? LCDCA_CFG_XBIAS : 0);
}


/**
 * \brief LCD timer initialization.
 *
 * This function initialize the LCD timer.
 *
 * The function sets the prescaler, the dividor and the duty cycle of the LCD.
 *
 * \param  lcd_pres    prescaler of the clock source.
 * \param  lcd_clkdiv  Divider of the prescaled clock source.
 * \param  fc0         Frame Counter 0.
 * \param  fc1         Frame Counter 1.
 * \param  fc2         Frame Counter 2.
 * \endcode
 */
static inline void lcdca_timer_init(bool lcd_pres,
		uint8_t lcd_clkdiv,
		uint8_t fc0, uint8_t fc1, uint8_t fc2)
{
	LCDCA->LCDCA_TIM = LCDCA_TIM_FC2(fc2) | LCDCA_TIM_FC1(fc1) | LCDCA_TIM_FC0(fc0) |\
		LCDCA_TIM_CLKDIV(lcd_clkdiv) | lcd_pres;
}

/**
 * \brief LCD timer enable.
 *
 * This function enables the LCD timer and wait until it is correctly enabled.
 *
 * \param  lcd_timer   Timer number to enable.
 * \endcode
 */
static inline void lcdca_enable_timer(uint8_t lcd_timer)
{
	if (lcd_timer == LCDCA_TIMER_FC0) {
		LCDCA->LCDCA_CR = LCDCA_CR_FC0EN;
		while(!(LCDCA->LCDCA_SR&LCDCA_SR_FC0S));
	}
	else if (lcd_timer == LCDCA_TIMER_FC1) {
		LCDCA->LCDCA_CR = LCDCA_CR_FC1EN;
		while(!(LCDCA->LCDCA_SR&LCDCA_SR_FC1S));
	}
	else if (lcd_timer == LCDCA_TIMER_FC2) {
		LCDCA->LCDCA_CR = LCDCA_CR_FC2EN;
		while(!(LCDCA->LCDCA_SR&LCDCA_SR_FC2S));
	}
}

/**
 * \brief LCD blinking initialization.
 *
 * This function initializes the hardware display blinking.
 *
 * The function defines the blinking rate. In the same time, the hardware
 * display blinking is disabled.
 *
 * \param  lcd_blink_timer LCD Blink Timer Selected.
 * \param  lcd_blink_mode  Blink Mode selected.
 */
static inline void lcdca_blink_init(uint8_t lcd_blink_timer, 
	bool lcd_blink_mode)
{
	LCDCA->LCDCA_CR = LCDCA_CR_BSTOP;
	LCDCA->LCDCA_BCFG = LCDCA_BCFG_FCS(lcd_blink_timer) | (lcd_blink_mode);
}

/**
 * \brief LCD Shift Register initialization.
 *
 * This function initializes the hardware display shift register.
 *
 * The function defines the shift register rate. In the same time, the hardware
 * display shift register is disabled.
 *
 * \param  lcd_csr_timer LCD Shift register Timer Selected.
 * \param  lcd_csr_dir   Direction.
 * \param  size          Defines the size of the circular shift register.
 * \param  data          Circular Shift Register Value.
 */
static inline void lcdca_csr_init(uint8_t lcd_blink_timer, 
	uint8_t lcd_csr_dir, uint8_t size, uint8_t data)
{
	LCDCA->LCDCA_CR = LCDCA_CR_CSTOP;
	LCDCA->LCDCA_CSRCFG = LCDCA_CSRCFG_FCS(lcd_blink_timer) | (lcd_csr_dir) |\
		LCDCA_CSRCFG_SIZE(size) | LCDCA_CSRCFG_DATA(data);
}

/**
 * \brief LCD Scrolling initialization.
 *
 * This function initializes the hardware scrolling.
 *
 * The function defines the scrolling rate. In the same time, the hardware
 * display scrolling is disabled.
 *
 * \param  lcd_scroll_timer LCD Scrolling register Timer Selected.
 * \param  lcd_scroll_dir   Direction (1: Reversed / 0: Not reversed).
 * \param  lcd_tdg          Type of digit selected.
 * \param  stseg            Defines the start segment.
 * \param  steps            Defines the step.
 * \param  dign             Defines the dign.
 */
static inline void lcdca_scrolling_init(uint8_t lcd_scroll_timer, 
	uint8_t lcd_scroll_dir, uint8_t lcd_tdg, uint8_t stseg, uint8_t steps,
	uint8_t dign)
{
	sysclk_enable_peripheral_clock(PDCA);
	// Clear register first
	LCDCA->LCDCA_ACMCFG = 0;
	// Write configuration then
	LCDCA->LCDCA_ACMCFG = LCDCA_ACMCFG_FCS(lcd_scroll_timer ) | (LCDCA_ACMCFG_MODE) |\
		(lcd_scroll_dir ? LCDCA_ACMCFG_DREV : 0) | LCDCA_ACMCFG_TDG(lcd_tdg) | LCDCA_ACMCFG_STSEG(stseg) |\
		LCDCA_ACMCFG_STEPS(steps) | LCDCA_ACMCFG_DIGN(dign);
}

/**
 * \brief LCD Scrolling start.
 *
 * This function start the hardware scrolling.
 *
 * \param  data          Data string buffer.
 * \param  width         Data string length.
 */
static inline void lcdca_scrolling_start(const uint8_t *data, size_t width)
{
	// Configure DMA
	PDCA->PDCA_CHANNEL[LCDCA_SCROLLING_DMA_CH].PDCA_MAR  = (uint32_t)data;
	PDCA->PDCA_CHANNEL[LCDCA_SCROLLING_DMA_CH].PDCA_MARR = (uint32_t)data;
	PDCA->PDCA_CHANNEL[LCDCA_SCROLLING_DMA_CH].PDCA_PSR  = LCDCA_PDCA_ID_TX_ACM;
	PDCA->PDCA_CHANNEL[LCDCA_SCROLLING_DMA_CH].PDCA_TCR  = width;
	PDCA->PDCA_CHANNEL[LCDCA_SCROLLING_DMA_CH].PDCA_TCRR = width;
	PDCA->PDCA_CHANNEL[LCDCA_SCROLLING_DMA_CH].PDCA_MR   = (PDCA_MR_RING) | (PDCA_MR_SIZE_BYTE);
	PDCA->PDCA_CHANNEL[LCDCA_SCROLLING_DMA_CH].PDCA_CR   = (PDCA_CR_TEN);
	// Enable Scrolling
	LCDCA->LCDCA_ACMCFG |= LCDCA_ACMCFG_EN;
}

/**
 * \brief LCD Scrolling reload.
 *
 * This function start the hardware scrolling.
 *
 * \param  data          Data string buffer.
 * \param  width         Data string length.
 */
static inline void lcdca_scrolling_reload(const uint8_t *data, size_t width)
{
  PDCA->PDCA_CHANNEL[LCDCA_SCROLLING_DMA_CH].PDCA_MARR = (uint32_t)data;
  PDCA->PDCA_CHANNEL[LCDCA_SCROLLING_DMA_CH].PDCA_TCRR = width;  
}

/**
 * \brief LCD Scrolling stop.
 *
 * This function stop the hardware scrolling.
 *
 */
static inline void lcdca_scrolling_stop(void)
{
	// Disable Scrolling
	LCDCA->LCDCA_ACMCFG &= ~LCDCA_ACMCFG_EN;
}

/**
 * \brief Set the LCD contrast.
 *
 * This function finely sets the LCD contrast.
 *
 * Transfer function: VLCD = 3.0 V + (fcont[5:0] * 0.016 V)
 *
 * \param  fcont  -32 <= signed contrast value <= 31.
 */
static inline void lcdca_set_contrast(int8_t fcont)
{
	// Clear previous contrast value
	LCDCA->LCDCA_CFG &= ~LCDCA_CFG_FCST(0x3Fu);
	// Write new value
	LCDCA->LCDCA_CFG |= LCDCA_CFG_FCST(fcont);
}


/**
 * \brief Enable LCD.
 *
 * This function enables the LCD module.
 */
static inline void lcdca_enable(void)
{
	// Enable all display memory
	LCDCA->LCDCA_CFG &= ~LCDCA_CFG_BLANK;
	LCDCA->LCDCA_CR = LCDCA_CR_EN;
	// Clear all display memory
	LCDCA->LCDCA_CR = LCDCA_CR_CDM;
	// Wait enable signal
	while(!(LCDCA->LCDCA_SR&LCDCA_SR_EN));
}


/**
 * \brief Disable LCD.
 *
 * This function disables the LCD module.
 *
 */
static inline void lcdca_disable(void)
{
	LCDCA->LCDCA_CR = LCDCA_CR_DIS;
}


/**
 * \brief Set the blinking of pixels.
 *
 * This function sets the blinking of pixels in LCD module on SEG0 & SEG1.
 * Each bit position corresponds to one pixel.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 *
 */
static inline void lcdca_set_blink_pixel(uint8_t pix_com, uint8_t pix_seg)
{
	if (pix_seg == 0){
		LCDCA->LCDCA_BCFG |= LCDCA_BCFG_BSS0(1<<pix_com);
	}
	if (pix_seg == 1){
		LCDCA->LCDCA_BCFG |= LCDCA_BCFG_BSS1(1<<pix_com);
	}
}

/**
 * \brief Clear the blinking of pixels.
 *
 * This function clears the blinking of pixels in LCD module on SEG0 & SEG1.
 * Each bit position corresponds to one pixel.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 *
 */
static inline void lcdca_clear_blink_pixel(uint8_t pix_com, uint8_t pix_seg)
{
	if (pix_seg == 0){
		LCDCA->LCDCA_BCFG &= ~LCDCA_BCFG_BSS0(1<<pix_com);
	}
	if (pix_seg == 1){
		LCDCA->LCDCA_BCFG &= ~LCDCA_BCFG_BSS1(1<<pix_com);
	}
}

/**
 * \brief Clear the blinking of all pixels.
 *
 * This function clears the blinking of all pixels in LCD module on SEG0 & SEG1.
 *
 */
static inline void lcdca_clear_blink_all_pixel(void)
{
	lcdca_clear_blink_pixel(0, 0|1);
	lcdca_clear_blink_pixel(1, 0|1);
	lcdca_clear_blink_pixel(2, 0|1);
	lcdca_clear_blink_pixel(3, 0|1);
}

/**
 * \brief LCD blink enable.
 *
 * This function enables the blinking mode in LCD module.
 */
static inline void lcdca_blink_enable(void)
{
	// Blinking "on"
	LCDCA->LCDCA_CR  = LCDCA_CR_BSTART;
}


/**
 * \brief LCD blink disable.
 *
 * This function disables the blinking mode in LCD module.
 */
static inline void lcdca_blink_disable(void)
{
	// Blinking "off"
	LCDCA->LCDCA_CR  = LCDCA_CR_BSTOP;
}

/**
 * \brief LCD CSR enable.
 *
 * This function enables the csr mode in LCD module.
 */
static inline void lcdca_csr_enable(void)
{
	// CSR "on"
	LCDCA->LCDCA_CR  = LCDCA_CR_CSTART;
}


/**
 * \brief LCD CSR disable.
 *
 * This function disables the csr mode in LCD module.
 */
static inline void lcdca_csr_disable(void)
{
	// CSR "off"
	LCDCA->LCDCA_CR  = LCDCA_CR_CSTOP;
}


/**
 * \brief LCD clock initialization.
 *
 * This function enables the specified LCD clock (RC32K or OSC32K).
 *
 * The function enables the LCD input clock.
 * Then the module source clock for the LCD will be the same than for the AST.
 *
 */
void lcdca_clk_init(void);

/**
 * \brief Send a sequence of ASCII characters to LCDCA device.
 *
 * This function enables/disables LCD pixels via the digit decoder.
 *
 * \param  lcdca_tdg  Type of digit decoder.
 * \param  first_seg  First SEG where the first data will be writen.
 * \param  data       Data buffer.
 * \param  width      Number of data.
 * \param  dir        Direction (==0: Left->Right, ==1: Left<-Right).
 */
void lcdca_write_packet(uint8_t lcd_tdg, uint8_t first_seg,
		const uint8_t *data, size_t width, uint8_t dir);

/**
 * \brief Set pixel (icon) in LCD display memory.
 *
 * This function sets a pixel in LCD (icon) display memory. If a parameter
 * is out of range, then the function doesn't set any bit.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 */
void lcdca_set_pixel(uint8_t pix_com, uint8_t pix_seg);


/**
 * \brief Clear pixel (icon) in LCD display memory.
 *
 * This function clears a pixel in LCD (icon) display memory. If a parameter
 * is out of range, then the function doesn't clear any bit.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 */
void lcdca_clear_pixel(uint8_t pix_com, uint8_t pix_seg);


/**
 * \brief Toggle pixel (icon) in LCD display memory.
 *
 * This function toggles a pixel in LCD (icon) display memory. If a parameter
 * is out of range, then the function doesn't toggle any bit.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 */
void lcdca_tgl_pixel(uint8_t pix_com, uint8_t pix_seg);


/**
 * \brief Get pixel value (icon) in LCD display memory.
 *
 * This function returns the pixel value in LCD (icon) display memory.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 */
bool lcdca_get_pixel(uint8_t pix_com, uint8_t pix_seg);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // _LCDCA_H_
