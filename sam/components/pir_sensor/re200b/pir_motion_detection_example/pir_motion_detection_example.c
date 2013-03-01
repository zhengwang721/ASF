/**
 * \file
 *
 * \brief re200b pir motion detect example.
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

/**
 * \mainpage re200b pir motion detect example
 *
 * \section Purpose
 *
 * This example demonstrates how to configure re200b pir sensor in order to
 * detect a motion and display information about it on LCD.
 *
 * \section Requirements
 *
 * This package can be used with sam4s_wpir_rd board.
 *
 * \section Description
 *
 * This example first configure LCD controller to display information and re200b
 * pir sensor to motion detection. After that, microcontroller enter in sleep
 * mode in order to reduce power comsumption. A motion detection will trig
 * ACC_Handler which will wake up microcontroller from sleep mode and display
 * information on LCD.Finally microcontroller will come back in sleep mode to
 * wait an others motion detection.
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a> application note or to the
 *    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *    IAR EWARM User Guide</a>, depending on your chosen solution.
 * -# Some text, image and basic shapes should be displayed on the LCD.
 *
 */
/********************************************************************/
/*                      Headers                                     */
/********************************************************************/
#include "asf.h"
#include "conf_board.h"
#include "conf_clock.h"

/********************************************************************/
/*                      Local Handler functions                     */
/********************************************************************/

/**
 * \brief Handler for acc comparison event
 */
void ACC_Handler(void)
{
	re200b_motion_detect_handler();
}

/********************************************************************/
/*                       Local functions                            */
/********************************************************************/

/**
 * \brief Configure microcontroller to enter in Sleep mode.
 * Enter condition: WFI + (SLEEPDEEP bit = 0) + (LPM bit = 0)
 */
static void _enter_sleep_mode( void )
{
	/* enable sleep mode*/
	PMC->PMC_FSMR &= (uint32_t) ~PMC_FSMR_LPM;

	/* clear DEEPSLEEP bit*/
	SCB->SCR &= (uint32_t) ~SCB_SCR_SLEEPDEEP_Msk;

	/* Call WFI instruction to enter in sleep mode*/
	__WFI();
}

/**
 * \brief Intialize LCD display
 */
static void _display_init(void)
{
	struct ili9325_opt_t ili9325_display_opt;

	/* Enable peripheral clock */
	pmc_enable_periph_clk( ID_SMC );

	/* Configure SMC interface for LCD */
	smc_set_setup_timing(SMC, ILI9325_LCD_CS, SMC_SETUP_NWE_SETUP(2)
			| SMC_SETUP_NCS_WR_SETUP(2)
			| SMC_SETUP_NRD_SETUP(2)
			| SMC_SETUP_NCS_RD_SETUP(2));

	smc_set_pulse_timing(SMC, ILI9325_LCD_CS, SMC_PULSE_NWE_PULSE(4)
			| SMC_PULSE_NCS_WR_PULSE(4)
			| SMC_PULSE_NRD_PULSE(10)
			| SMC_PULSE_NCS_RD_PULSE(10));

	smc_set_cycle_timing(SMC, ILI9325_LCD_CS, SMC_CYCLE_NWE_CYCLE(10)
			| SMC_CYCLE_NRD_CYCLE(22));

	smc_set_mode(SMC, ILI9325_LCD_CS, SMC_MODE_READ_MODE
			| SMC_MODE_WRITE_MODE
			| SMC_MODE_DBW_8_BIT);

	/* Initialize display parameter */
	ili9325_display_opt.ul_width = ILI9325_LCD_WIDTH;
	ili9325_display_opt.ul_height = ILI9325_LCD_HEIGHT;
	ili9325_display_opt.foreground_color = COLOR_BLACK;
	ili9325_display_opt.background_color = COLOR_WHITE;

	/* Switch off backlight */
	aat31xx_disable_backlight();

	/* Initialize LCD */
	ili9325_init(&ili9325_display_opt);

	/* Set backlight level */
	aat31xx_set_backlight(AAT31XX_MAX_BACKLIGHT_LEVEL);

	/* Turn on LCD */
	ili9325_display_on();
}

/********************************************************************/
/*                      Exported functins                           */
/********************************************************************/

/**
 * \brief Application entry point for smc_lcd example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint32_t ul_motion_detected = false;
	uint32_t i = 0;
	uint8_t uc_string_display[30];

	sysclk_init();
	board_init();

	/* LCD display initialization*/
	_display_init();

	/* Clear LCD */
	ili9325_fill(COLOR_TURQUOISE);

	/* Initialize Pir sensor*/
	re200b_motion_detect_init();

	while (1) {
		/* Reset motion detection (enable interrupt, clear variable,
		 * clear status register) */
		re200b_motion_detect_enable();
		re200b_motion_detect_reset();
		acc_get_interrupt_status(ACC);

		/* Enter sleep mode */
		_enter_sleep_mode();

		/* Check if a motion was detected */
		ul_motion_detected = re200b_motion_detection();
		if (ul_motion_detected) {
			/* Disable motion detection */
			re200b_motion_detect_disable();

			/*Display information about motion detection on LCD*/
			if ((i * 20) < BOARD_LCD_HEIGHT) {
				sprintf(uc_string_display, "Motion Detected:%d",
						i);
				ili9325_draw_string(0, i * 20,
						uc_string_display);
				i++;
			} else {
				i = 0;
				ili9325_fill(COLOR_TURQUOISE);
				sprintf(uc_string_display, "Motion Detected:%d",
						i);
				ili9325_draw_string(0, i * 20,
						uc_string_display);
				i++;
			}
		}
	}
}
