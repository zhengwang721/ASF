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
#ifndef SLCDC_H_INCLUDED
#define SLCDC_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "compiler.h"
#include "supc.h"

/**
 * \defgroup group_sam_drivers_slcdc Segment Liquid Crystal Display Controller (SLCDC)
 *
 * This is a driver for configuring, enabling/disabling and use of the on-chip SLCDC.
 *
 * \section dependencies Dependencies
 *
 * The SLCDC module depends on the following modules:
 * - \ref sysclk_group for SLCDC clock control.
 * - \ref supc_group for SLCDC power management.
 * - \ref interrupt_group for enabling or disabling interrupts.
 * - \ref sleepmgr_group to unlock SLCDC
 * @{
 */

#define LCD_SEG_NB    50
#define LCD_COM_NB    6

/** Selection of the Number of Commons */
enum slcdc_com_num {
	/** COM0 is driven by SLCDC, COM1:5 are driven by digital function */
	SLCDC_COM_NUM_0 = SLCDC_MR_COMSEL_COM_0,
	/** COM0:1 are driven by SLCDC, COM2:5 are driven by digital function */
	SLCDC_COM_NUM_1 = SLCDC_MR_COMSEL_COM_0TO1,
	/** COM0:2 are driven by SLCDC, COM3:5 are driven by digital function */
	SLCDC_COM_NUM_2 = SLCDC_MR_COMSEL_COM_0TO2,
	/** COM0:3 are driven by SLCDC, COM4:5 are driven by digital function */
	SLCDC_COM_NUM_3 = SLCDC_MR_COMSEL_COM_0TO3,
	/** COM0:4 are driven by SLCDC, COM5 is driven by digital function */
	SLCDC_COM_NUM_4 = SLCDC_MR_COMSEL_COM_0TO4,
	/** COM0:5 are driven by SLCDC, No COM pin driven by digital function */
	SLCDC_COM_NUM_5 = SLCDC_MR_COMSEL_COM_0TO5,
};

/** Buffer On-Time */
enum slcdc_buf_time {
	/** Nominal drive time is 0% of SCLK period */
	SLCDC_BUF_TIME_0 = SLCDC_MR_BUFTIME_OFF,
	/** Nominal drive time is 2 periods of SCLK clock */
	SLCDC_BUF_TIME_1 = SLCDC_MR_BUFTIME_X2_SCLK_PERIOD,
	/** Nominal drive time is 4 periods of SCLK clock */
	SLCDC_BUF_TIME_2 = SLCDC_MR_BUFTIME_X4_SCLK_PERIOD,
	/** Nominal drive time is 8 periods of SCLK clock */
	SLCDC_BUF_TIME_3 = SLCDC_MR_BUFTIME_X8_SCLK_PERIOD,
	/** Nominal drive time is 16 periods of SCLK clock */
	SLCDC_BUF_TIME_4 = SLCDC_MR_BUFTIME_X16_SCLK_PERIOD,
	/** Nominal drive time is 32 periods of SCLK clock */
	SLCDC_BUF_TIME_5 = SLCDC_MR_BUFTIME_X32_SCLK_PERIOD,
	/** Nominal drive time is 64 periods of SCLK clock */
	SLCDC_BUF_TIME_6 = SLCDC_MR_BUFTIME_X64_SCLK_PERIOD,
	/** Nominal drive time is 128 periods of SCLK clock */
	SLCDC_BUF_TIME_7 = SLCDC_MR_BUFTIME_X128_SCLK_PERIOD,
	/** Nominal drive time is 50% of SCLK period */
	SLCDC_BUF_TIME_8 = SLCDC_MR_BUFTIME_PERCENT_50,
	/** Nominal drive time is 100% of SCLK period */
	SLCDC_BUF_TIME_9 = SLCDC_MR_BUFTIME_PERCENT_100,
};

/** LCD Display Configuration */
enum slcdc_bias {
	/** static */
	SLCDC_BIAS_0 = SLCDC_MR_BIAS_STATIC,
	/** bias 1/2 */
	SLCDC_BIAS_1 = SLCDC_MR_BIAS_BIAS_1_2,
	/** bias 1/3 */
	SLCDC_BIAS_2 = SLCDC_MR_BIAS_BIAS_1_3,
};

/** Prescaler of the clock source */
enum slcdc_clk_pres {
	/** Slow clock is divided by 8 */
	SLCDC_CLK_PRES_0 = SLCDC_FRR_PRESC_SCLK_DIV8,
	/** Slow clock is divided by 16 */
	SLCDC_CLK_PRES_1 = SLCDC_FRR_PRESC_SCLK_DIV16,
	/** Slow clock is divided by 32 */
	SLCDC_CLK_PRES_2 = SLCDC_FRR_PRESC_SCLK_DIV32,
	/** Slow clock is divided by 64 */
	SLCDC_CLK_PRES_3 = SLCDC_FRR_PRESC_SCLK_DIV64,
	/** Slow clock is divided by 128 */
	SLCDC_CLK_PRES_4 = SLCDC_FRR_PRESC_SCLK_DIV128,
	/** Slow clock is divided by 256 */
	SLCDC_CLK_PRES_5 = SLCDC_FRR_PRESC_SCLK_DIV256,
	/** Slow clock is divided by 512 */
	SLCDC_CLK_PRES_6 = SLCDC_FRR_PRESC_SCLK_DIV512,
	/** Slow clock is divided by 1024 */
	SLCDC_CLK_PRES_7 = SLCDC_FRR_PRESC_SCLK_DIV1024,
};

/** Divider of the prescaled clock source */
enum slcdc_clk_div {
	/** Clock output from prescaler is divided by 1 */
	SLCDC_CLK_DIV_0 = SLCDC_FRR_DIV_PRESC_CLK_DIV1,
	/** Clock output from prescaler is divided by 2 */
	SLCDC_CLK_DIV_1 = SLCDC_FRR_DIV_PRESC_CLK_DIV2,
	/** Clock output from prescaler is divided by 3 */
	SLCDC_CLK_DIV_2 = SLCDC_FRR_DIV_PRESC_CLK_DIV3,
	/** Clock output from prescaler is divided by 4 */
	SLCDC_CLK_DIV_3 = SLCDC_FRR_DIV_PRESC_CLK_DIV4,
	/** Clock output from prescaler is divided by 5 */
	SLCDC_CLK_DIV_4 = SLCDC_FRR_DIV_PRESC_CLK_DIV5,
	/** Clock output from prescaler is divided by 6 */
	SLCDC_CLK_DIV_5 = SLCDC_FRR_DIV_PRESC_CLK_DIV6,
	/** Clock output from prescaler is divided by 7 */
	SLCDC_CLK_DIV_6 = SLCDC_FRR_DIV_PRESC_CLK_DIV7,
	/** Clock output from prescaler is divided by 8 */
	SLCDC_CLK_DIV_7 = SLCDC_FRR_DIV_PRESC_CLK_DIV8,
};

/** Display Mode */
enum slcdc_disp_mode {
	/** Normal Mode */
	SLCDC_DISP_MODE_0 = SLCDC_DR_DISPMODE_NORMAL,
	/** Force Off Mode */
	SLCDC_DISP_MODE_1 = SLCDC_DR_DISPMODE_FORCE_OFF,
	/** Force On Mode */
	SLCDC_DISP_MODE_2 = SLCDC_DR_DISPMODE_FORCE_ON,
	/** Blinking Mode */
	SLCDC_DISP_MODE_3 = SLCDC_DR_DISPMODE_BLINKING,
	/** Inverted Mode */
	SLCDC_DISP_MODE_4 = SLCDC_DR_DISPMODE_INVERTED,
	/** Inverted Blinking Mode */
	SLCDC_DISP_MODE_5 = SLCDC_DR_DISPMODE_INVERTED_BLINK,
	/** User Buffer Only Load Mode */
	SLCDC_DISP_MODE_6 = SLCDC_DR_DISPMODE_USER_BUFFER_LOAD,
	/** Buffer Swap Mode */
	SLCDC_DISP_MODE_7 = SLCDC_DR_DISPMODE_BUFFERS_SWAP,
};

/** LCD Voltage Regulator Output */
enum slcdc_voltage {
	/** 2.92V */
	SLCDC_VOL_292 = 0,
	/** 2.85V */
	SLCDC_VOL_285,
	/** 2.77V */
	SLCDC_VOL_277,
	/** 2.70V */
	SLCDC_VOL_270,
	/** 2.63V */
	SLCDC_VOL_263,
	/** 2.55V */
	SLCDC_VOL_255,
	/** 2.48V */
	SLCDC_VOL_248,
	/** 2.41V */
	SLCDC_VOL_241,
	/** 3.51V */
	SLCDC_VOL_351,
	/** 3.44V */
	SLCDC_VOL_344,
	/** 3.36V */
	SLCDC_VOL_336,
	/** 3.29V */
	SLCDC_VOL_329,
	/** 3.22V */
	SLCDC_VOL_322,
	/** 3.14V */
	SLCDC_VOL_314,
	/** 3.07V */
	SLCDC_VOL_307,
	/** 3V */
	SLCDC_VOL_300,	
};

/**
 * Basic configuration for SLCDC.
 */
struct slcdc_config {
	/** Buffer On-Time */
	enum slcdc_buf_time buf_time;
	/** LCD Display Configuration */
	bool is_low_power;
	/** prescaler of the clock source. */
	enum slcdc_clk_pres pres;
	/** Divider of the prescaled clock source. */
	enum slcdc_clk_div div;
	/** Display Mode. */
	enum slcdc_disp_mode disp_mode;
	/** LCD Blinking Frequency Selection */
	uint8_t blink_freq;
	/** Power Mode */
	enum slcdc_power_mode power_mode;
	/** LCD Voltage Regulator Output */
	enum slcdc_voltage vlotage;
};

/**
 * \brief Interrupt event callback function type
 *
 * The interrupt handler can be configured to do a function callback,
 * the callback function must match the slcdc_callback_t type.
 *
 */
typedef void (*slcdc_callback_t)(void);

/**
 * \brief Initialize SLCDC with specified configuration.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param slcdc_cfg  Pointer to SLCDC Configuration.
 */
status_code_t slcdc_init(Slcdc *p_slcdc, struct slcdc_config *slcdc_cfg);

/**
 * \brief  SLCDC enter low power mode.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 */
static inline void slcdc_enter_lp_mode(Slcdc *p_slcdc)
{
	p_slcdc->SLCDC_MR |= SLCDC_MR_LPMODE;
}

/**
 * \brief  SLCDC exit from low power mode.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 */
static inline void slcdc_exit_lp_mode(Slcdc *p_slcdc)
{
	p_slcdc->SLCDC_MR &= ~SLCDC_MR_LPMODE;
}

/**
 * \brief  Set display mode for SLCDC.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param mode Display mode.
 */
static inline void slcdc_set_display_mode(Slcdc *p_slcdc, enum slcdc_disp_mode mode)
{
	p_slcdc->SLCDC_DR |= mode;
}

/**
  * \brief  Set blinking frequency for SLCDC.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param freq  Blinking frequency.
 */
static inline void slcdc_set_blink_freq(Slcdc *p_slcdc, uint8_t freq)
{
	p_slcdc->SLCDC_DR |= SLCDC_DR_LCDBLKFREQ(freq);
}

/**
 * \brief  Set Buffer On-Time for SLCDC.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param buf_time Buffer time value .
 */
static inline void slcdc_set_buf_time(Slcdc *p_slcdc, enum slcdc_buf_time buf_time)
{
	p_slcdc->SLCDC_MR |= buf_time;
}

/**
 * \brief  Set frame rate for SLCDC.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param buf_time Buffer time value .
 */
static inline void slcdc_set_frame_rate(Slcdc *p_slcdc, enum slcdc_clk_pres pres,
		enum slcdc_clk_div div)
{
	p_slcdc->SLCDC_FRR = pres | div;
}

/**
 * \brief Enable SLCDC.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 */
void slcdc_enable(Slcdc *p_slcdc);

/**
 * \brief Disable SLCDC.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 */
void slcdc_disable(Slcdc *p_slcdc);

/**
 * \brief Reset SLCDC.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 */
static inline void slcdc_reset(Slcdc *p_slcdc)
{
	p_slcdc->SLCDC_CR = SLCDC_CR_SWRST;
}

/**
 * \brief Get the SLCDC status.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 *
 * \return return bit OR of the SLCDC status register.
 */
static inline uint32_t slcdc_get_status(Slcdc *p_slcdc)
{
	return p_slcdc->SLCDC_SR;
}

/**
 * \brief SLCDC interrupt callback function
 *
 * This function allows the caller to set and change the interrupt callback
 * function. Without setting a callback function the interrupt handler in the
 * driver will only clear the interrupt flags.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param callback Reference to a callback function.
 * \param ul_sources Interrupts to be enabled.
 * \param irq_line  interrupt line.
 * \param irq_level interrupt level.
 */
void slcdc_set_callback(Slcdc *p_slcdc, slcdc_callback_t callback,
		uint32_t ul_sources, uint8_t irq_line, uint8_t irq_level);

/**
 * \brief SLCDC enable interrupt.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param ul_sources Interrupts to be enabled.
 */
static inline void slcdc_enable_interrupt(Slcdc *p_slcdc, uint32_t ul_sources)
{
	p_slcdc->SLCDC_IER = ul_sources;
}

/**
 * \brief SLCDC disable interrupt.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param ul_sources Interrupts to be disabled.
 */
static inline void slcdc_disable_interrupt(Slcdc *p_slcdc, uint32_t ul_sources)
{
	p_slcdc->SLCDC_IDR = ul_sources;
}

/**
 * \brief Get SLCDC interrupt mask.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 *
 * \return The interrupt mask value.
 */
static inline uint32_t slcdc_get_interrupt_mask(Slcdc *p_slcdc)
{
	return p_slcdc->SLCDC_IMR;
}

/**
 * \brief Get SLCDC interrupt status.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 *
 * \return The interrupt status value.
 */
static inline uint32_t slcdc_get_interrupt_status(Slcdc *p_slcdc)
{
	return p_slcdc->SLCDC_ISR;
}

/**
 * \brief SLCDC set segment map0.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param ul_sources Segments to be mapped.
 */
static inline void slcdc_set_segmap0(Slcdc *p_slcdc, uint32_t ul_sources)
{
	p_slcdc->SLCDC_SMR0 = ul_sources;
}

/**
 * \brief SLCDC set segment map1.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param ul_sources Segments to be mapped.
 */
static inline void slcdc_set_segmap1(Slcdc *p_slcdc, uint32_t ul_sources)
{
	p_slcdc->SLCDC_SMR1 = ul_sources;
}

/**
 * \brief Enable or disable write protection of SLCDC registers.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param ul_enable 1 to enable, 0 to disable.
 */
static inline void slcdc_set_writeprotect(Slcdc *p_slcdc, uint32_t ul_enable)
{
	if (ul_enable) {
		p_slcdc->SLCDC_WPMR = SLCDC_WPMR_WPKEY(0x4c4344) | SLCDC_WPMR_WPEN;
	} else {
		p_slcdc->SLCDC_WPMR = SLCDC_WPMR_WPKEY(0x4c4344);
	}
}

/**
 * \brief Indicate write protect status.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 *
 * \return SLCDC_WPSR value.
 */
static inline uint32_t slcdc_get_writeprotect_status(Slcdc *p_slcdc)
{
	return p_slcdc->SLCDC_WPSR;
}

/**
 * \brief Set LSB Pixels memory associated to COMx terminal.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param index COM terminal index.
 * \param val    SEG terminal value
 */
static inline void slcdc_set_lsb_memory(Slcdc *p_slcdc, uint32_t index,uint32_t val)
{
    p_slcdc->SLCDC_COM[index].SLCDC_LMEMR |= val;
}

/**
 * \brief Set MSB Pixels memory associated to COMx terminal.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param index COM terminal index.
 * \param val    SEG terminal value
 */
static inline void slcdc_set_msb_memory(Slcdc *p_slcdc, uint32_t index,uint32_t val)
{
    p_slcdc->SLCDC_COM[index].SLCDC_MMEMR |= val;
}

/**
 * \brief Clear LSB Pixels memory associated to COMx terminal.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param index COM terminal index.
 * \param val    SEG terminal value
 */
static inline void slcdc_clear_lsb_memory(Slcdc *p_slcdc, uint32_t index,uint32_t val)
{
    p_slcdc->SLCDC_COM[index].SLCDC_LMEMR &= ~val;
}

/**
 * \brief Clear MSB Pixels memory associated to COMx terminal.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param index COM terminal index.
 * \param val    SEG terminal value
 */
static inline void slcdc_clear_msb_memory(Slcdc *p_slcdc, uint32_t index,uint32_t val)
{
    p_slcdc->SLCDC_COM[index].SLCDC_MMEMR &= ~val;
}

/* LCD symbol dot mapping */
enum symbol_dot{
//LCD seg remap to chip seg
G1  = 4 * 48,G0,G2,G3,            //0 ==>   48
G4  = 4 * 49,G5,G6,G7,            // 1 ==>   49
/*
E0 = 4 *15,E2,E4,E6,		//  2 ==> 15
E1 = 4 *16,E3,E5,E7,		 // 3 ==> 16
*/
D3A = 4 * 27,D3B,D3C,B9,        //  4 ==> 27    
D3F = 4 * 26,D3G,D3E,D3D,    //  5 ==> 26
D2A = 4 * 25,D2B,D2C,D2P,     // 6 ==> 25
D2F = 4 * 24,D2G,D2E,D2D,     // 7 ==> 24
D1A = 4 * 22,D1B,D1C,D1P,     // 8 ==> 22
D1F = 4 * 21,D1G,D1E,D1D,     // 9 ==> 21
D0A = 4 * 20,D0B,D0C,D0P,     // 10 ==> 20
D0F = 4 * 17,D0G,D0E,D0D,      // 11 ==> 17
A6H = 4 * 47,A6I,A6K,A6N,      // 12 ==> 47
B14 = 4 * 46,A6F,A6E,A6D,       // 13 ==> 46
A6A = 4 * 45,A6B,A6C,B13,       // 14 ==> 45
A6G = 4 * 44,A6J,A6L,A6M,       // 15 ==> 44
A5H = 4 * 43,A5I,A5K,A5N,       // 16 ==> 43
B5  = 4 * 42,A5F,A5E,A5D,         // 17 ==> 42
A5A = 4 * 41,A5B,A5C,B12,       // 18 ==> 41
A5G = 4 * 40,A5J,A5L,A5M,       // 19 ==> 40
A4H = 4 * 39,A4I,A4K,A4N,       // 20 ==> 39
B6  = 4 * 38,A4F,A4E,A4D,         // 21 ==> 38
A4A = 4 * 37,A4B,A4C,B11,       // 22 ==> 37
A4G = 4 * 36,A4J,A4L,A4M,       // 23 ==> 36
A3H = 4 * 35,A3I,A3K,A3N,       // 24 ==> 35
B4  = 4 * 34,A3F,A3E,A3D,         // 25 ==> 34
A3A = 4 * 33,A3B,A3C,B10,        // 26 ==> 33
A3G = 4 * 32,A3J,A3L,A3M,        // 27 ==> 32
A2H = 4 * 31,A2I,A2K,A2N,        // 28 ==> 31
B3  = 4 * 30,A2F,A2E,A2D,          // 29 ==> 30
A2A = 4 * 29,A2B,A2C,B1,          // 30 ==> 29
A2G = 4 * 28,A2J,A2L,A2M,        // 31 ==> 28
/*
A1H = 4 * 27,A1I,A1K,A1N,	    //  32 ==> 27
B2 = 4 * 26,A1F,A1E,A1D,           //  33 ==> 26
A1A = 4 * 25,A1B,A1C,B8,	    //  34 ==> 25
A1G = 4 * 24,A1J,A1L,A1M,	    //  35 ==> 24
A0H = 4 * 22,A0I,A0K,A0N,          //  36 ==> 22
B0 = 4 * 21,A0F,A0E,A0D,	   //  37 ==> 21
A0A = 4 * 20,A0B,A0C,B7,          //  38 ==> 20 
A0G = 4 * 17,A0J,A0L,A0M,	   // 39 ==> 17
*/
};

/* Number */
#define PLOT_NUMBER_0 	0x3F
#define PLOT_NUMBER_1      0x06
#define PLOT_NUMBER_2      0x5B
#define PLOT_NUMBER_3 	0x4F
#define PLOT_NUMBER_4 	0x66
#define PLOT_NUMBER_5 	0x6D
#define PLOT_NUMBER_6 	0x7D
#define PLOT_NUMBER_7 	0x07
#define PLOT_NUMBER_8 	0x7F
#define PLOT_NUMBER_9 	0x6F

/* Letter */
#define PLOT_LETTER_0 	0x003F
#define PLOT_LETTER_1 	0x1080
#define PLOT_LETTER_2 	0x061B
#define PLOT_LETTER_3 	0x060F
#define PLOT_LETTER_4 	0x0626
#define PLOT_LETTER_5 	0x062D
#define PLOT_LETTER_6 	0x063B
#define PLOT_LETTER_7 	0x0007
#define PLOT_LETTER_8 	0x063F
#define PLOT_LETTER_9 	0x062F
#define PLOT_LETTER_A 	0x0637
#define PLOT_LETTER_B        0x148F
#define PLOT_LETTER_C        0x0039
#define PLOT_LETTER_D	0x10B9
#define PLOT_LETTER_E        	0x0639
#define PLOT_LETTER_F 	0x0631
#define PLOT_LETTER_G 	0x043D
#define PLOT_LETTER_H 	0x0636
#define PLOT_LETTER_I 	0x1089
#define PLOT_LETTER_J 	0x001E
#define PLOT_LETTER_K 	0x2330
#define PLOT_LETTER_L 	0x0038
#define PLOT_LETTER_M 	0x0176
#define PLOT_LETTER_N 	0x2076
#define PLOT_LETTER_O 	0x003F
#define PLOT_LETTER_P 	0x0633
#define PLOT_LETTER_Q 	0x203F
#define PLOT_LETTER_R 	0x2633
#define PLOT_LETTER_S 	0x062D
#define PLOT_LETTER_T 	0x1081
#define PLOT_LETTER_U 	0x003E
#define PLOT_LETTER_V 	0x0930
#define PLOT_LETTER_W 	0x2836
#define PLOT_LETTER_X 	0x2940
#define PLOT_LETTER_Y 	0x1140
#define PLOT_LETTER_Z 	0x0909

/*Primary*/
#define PLOT_PRIM_NEG1 	0x7
#define PLOT_PRIM_POS1  	0x6
#define PLOT_PRIM_EMPTY  0x0
#define PLOT_PRIM_NEG 	0x1

/*Battery*/
#define PLOT_BAT_0 		0x1
#define PLOT_BAT_1  		0x3
#define PLOT_BAT_2  		0x7
#define PLOT_BAT_3 		0xF

/*multiple dot symbols*/
#define PATTERN_SYMBOL_D0				{D0A,D0B,D0C,D0D,D0E,D0F,D0G}
#define PATTERN_SYMBOL_D1				{D1A,D1B,D1C,D1D,D1E,D1F,D1G}
#define PATTERN_SYMBOL_D2				{D2A,D2B,D2C,D2D,D2E,D2F,D2G}
#define PATTERN_SYMBOL_D3				{D3A,D3B,D3C,D3D,D3E,D3F,D3G}

#define PATTERN_SYMBOL_A0				{A0A,A0B,A0C,A0D,A0E,A0F,A0G,A0H,A0I,A0J,A0K,A0L,A0M,A0N}
#define PATTERN_SYMBOL_A1				{A1A,A1B,A1C,A1D,A1E,A1F,A1G,A1H,A1I,A1J,A1K,A1L,A1M,A1N}
#define PATTERN_SYMBOL_A2				{A2A,A2B,A2C,A2D,A2E,A2F,A2G,A2H,A2I,A2J,A2K,A2L,A2M,A2N}
#define PATTERN_SYMBOL_A3				{A3A,A3B,A3C,A3D,A3E,A3F,A3G,A3H,A3I,A3J,A3K,A3L,A3M,A3N}
#define PATTERN_SYMBOL_A4				{A4A,A4B,A4C,A4D,A4E,A4F,A4G,A4H,A4I,A4J,A4K,A4L,A4M,A4N}
#define PATTERN_SYMBOL_A5				{A5A,A5B,A5C,A5D,A5E,A5F,A5G,A5H,A5I,A5J,A5K,A5L,A5M,A5N}
#define PATTERN_SYMBOL_A6				{A6A,A6B,A6C,A6D,A6E,A6F,A6G,A6H,A6I,A6J,A6K,A6L,A6M,A6N}

#define PATTERN_SYMBOLl_CYCLE			{E0,E1,E2,E3,E4,E5,E6,E7}
#define PATTERN_SYMBOL_BATTERY		        {G4,B4,B5,B6}
#define PATTERN_SYMBOL_PRIMARY		        {B0,B1,B2,B3}
#define PATTERN_SYMBOL_USB 		                 {G0}
#define PATTERN_SYMBOL_COLON 		         {G1}
#define PATTERN_SYMBOL_WLESS 		         {G2}
#define PATTERN_SYMBOL_AUDIO 		         {G3}
#define PATTERN_SYMBOL_AM 		                 {B9}
#define PATTERN_SYMBOL_PM 		                 {B10}
#define PATTERN_SYMBOL_C_DEG 		         {B11}
#define PATTERN_SYMBOL_F_DEG 		         {B12}
#define PATTERN_SYMBOL_VOL 		                 {B13}
#define PATTERN_SYMBOL_MVOL 		         {B14}
#define PATTERN_SYMBOL_ERROR_CHN 		 {G6}
#define PATTERN_SYMBOL_ERROR_EN 		 {G5}
#define PATTERN_SYMBOL_LOG 		                 {G7}
#define PATTERN_SYMBOL_START 		         {B8}
#define PATTERN_SYMBOL_PAUSE 		         {B7}

/**
 * \brief Set pixel (icon) in SLCDC display memory.
 *
 * This function sets a pixel in LCD (icon) display memory. If a parameter
 * is out of range, then the function doesn't set any bit.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 */
void slcdc_set_pixel(uint8_t pix_com, uint8_t pix_seg);

/**
 * \brief Clear pixel (icon) in SLCDC display memory.
 *
 * This function clears a pixel in LCD (icon) display memory. If a parameter
 * is out of range, then the function doesn't clear any bit.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel (icon).
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel (icon).
 */
void slcdc_clear_pixel(uint8_t pix_com, uint8_t pix_seg);

/**
 * \brief Turn on a symbol dot array according to symbol plot definition.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param p_symbol  Pointer to an symbol_dot array.
 * \param number Symbol dot array number.
 * \param plot   Symbol plot definition value
 */
void slcdc_display_symbol(Slcdc *p_slcdc, const enum symbol_dot *p_symbol,
		uint8_t number, uint32_t plot);

/**
 * \brief Turn off a symbol dot array.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param p_symbol  Pointer to an symbol_dot array.
 * \param number Symbol dot array number.
 */
void slcdc_efface_symbol(Slcdc *p_slcdc, const enum symbol_dot *p_symbol,
		uint8_t number);

/**
 * \brief Set all SLCDC display memory.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 */
void slcdc_set_display_memory(Slcdc *p_slcdc);

/**
 * \brief Clear all SLCDC display memory.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 */
 void slcdc_clear_display_memory(Slcdc *p_slcdc);

/**
 * \brief Display a string in lcd glass alphanumeric field.
 *
 * This function will write the input string to the alphanumeric field of the
 * lcd glass.
 *
 * \param data Pointer to the input string(max length is 5)
 */
void slcdc_display_alphanum_string(const uint8_t *data);

/**
 * \brief Display a string in lcd glass numeric field.
 *
 * This function will write the input string to the numeric field of the
 * lcd glass.
 *
 * \param data Pointer to the input string(max length is 4)
 */
void slcdc_display_num_string(const uint8_t *data);

/**
 * \brief Clear a specific icon on the LCD glass.
 *
 * This function will clear a specific icon.
 *
 * \param icon_com  Pixel coordinate - COMx - of the icon.
 * \param icon_seg  Pixel coordinate - SEGy - of the icon.
 *
 * \note Use the icon define in header file.
 */
void slcdc_clear_icon(uint8_t icon_com, uint8_t icon_seg);

/**
 * \brief Show a specific icon on the LCD glass.
 *
 * This function will show a specific icon.
 *
 * \param icon_com  Pixel coordinate - COMx - of the icon.
 * \param icon_seg  Pixel coordinate - SEGy - of the icon.
 *
 * \note Use the icon define in header file.
 */
void slcdc_show_icon(uint8_t icon_com, uint8_t icon_seg);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

/**
 * \page sam_drivers_slcdc_quick_start Quick Start Guide for the SLCDC driver
 *
 * This is the quick start guide for the \ref group_sam_drivers_slcdc, with
 * step-by-step instructions on how to configure and use the driver for
 * a specific use case.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section slcdc_qs_use_cases Use cases
 * - \ref slcdc_basic
 *
 * \section slcdc_basic SLCDC basic usage
 *
 * This use case will demonstrate how to configure and use of the on-chip
 * SLCD controller to address an external LCD segment (C42048A).
 *
 * \section slcdc_basic_setup Setup steps
 *
 * \subsection slcdc_basic_prereq Prerequisites
 *
 * This module requires the following service
 * - \ref clk_group
 * - \ref supc_group
 * - \ref sleepmgr_group
 *
 * \subsection slcdc_basic_setup_code Setup Code Example
 *
 * Add this to the main loop or a setup function:
 * \code
 *
 *   struct slcdc_config slcdc_cfg;
 *
 *   // SLCDC Controller initialization
 *   // - Clock,
 *   // - Connect to C42364A glass LCD component,
 *   // - Timing:  64 Hz frame rate
 *   // - Interrupt: off
 *   slcdc_cfg.com_num = SLCDC_COM_NUM_3;
 *   slcdc_cfg.seg_num = 39;
 *   slcdc_cfg.buf_time = SLCDC_BUF_TIME_6;
 *   slcdc_cfg.bias = SLCDC_BIAS_2;
 *   slcdc_cfg.is_low_power = false;
 *   slcdc_cfg.pres = SLCDC_CLK_PRES_4;
 *   slcdc_cfg.div = SLCDC_CLK_DIV_3;
 *   slcdc_cfg.disp_mode = SLCDC_DISP_MODE_0;
 *   slcdc_cfg.blink_freq = 32;
 *   slcdc_cfg.power_mode = SLCDC_POWER_MODE_2;
 *   slcdc_cfg.vlotage = 4;
 *   slcdc_init(&slcdc_cfg);
 *   slcdc_enable();
 *
 *   // Turn on LCD back light
 *   ioport_set_pin_level(LCD_BL_GPIO, IOPORT_PIN_LEVEL_HIGH);
 * \endcode
 *
 * \subsection slcdc_basic_setup_workflow Basic Setup Workflow
 *
 * -# Initialize SLCDC with basic configuration
 *  - \code slcdc_init(&slcdc_cfg); \endcode
 * -# Enable SLCDC module
 *  - \code slcdc_enable(); \endcode
 * -# Turn on LCD back light
 *  - \code ioport_set_pin_level(LCD_BL_GPIO, IOPORT_PIN_LEVEL_HIGH); \endcode
 *
 */
#endif /* SLCDC_H_INCLUDED */
