/**
 * \file
 *
 * \brief OV7740 continuous capture demonstration without external SRAM.
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
 * \mainpage OV7740 continuous capture demonstration without external SRAM.
 *
 * \section Purpose
 *
 * This example demonstrates how to configure the OV7740 to continually capture
 * a picture into internal SRAM and display it on the LCD.
 *
 * This example demonstrates how to configure the OV7740 CMOS sensor
 * in order to create a camera application which will continually take a
 * picture, store it in internal SRAM (splitting the picture) and display it
 * on the LCD.
 *
 * \section Description
 *
 * This example first sets the PLLB to the system core clock and PLLA to clock
 * PCK0 (used to get data from image sensor). Next step is to configure the
 * LCD controller and display information to the user.
 * Finally this example configures the OV7740 CMOS image sensor and the PIO
 * capture.
 * So this example will continually take picture, splitting it into internal
 * SRAM
 * and display it on the LCD as a camera.
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
 *
 */

#include "asf.h"
#include "conf_board.h"
#include "conf_clock.h"

/* TWI clock frequency in Hz (100KHz) */
#define TWI_CLK     100000

/* Macro to measure Frame rate, to be commented if not used */
/* #define FRAME_RATE_MEASURE */

/* Pointer to the image data destination buffer */
uint8_t *g_p_uc_cap_dest_buf;

/* Row size of captured picture */
uint16_t g_us_cap_rows = IMAGE_HEIGHT;

/* Line size of captured picture (according to default color mode) */
uint16_t g_us_cap_line = (IMAGE_WIDTH * 2);

/* Buffer which contains data of the sliced picture */
uint8_t uc_capture_buffer_1[51200];     /* First buffer for image capture from
                                         * image Sensor */
uint8_t uc_capture_buffer_2[51200];     /* Second buffer for image capture from
                                         * image Sensor */

/* Vsync signal information (true if it's triggered and false otherwise) */
static volatile uint32_t g_ul_vsync_flag = false;

/* Information about the end of the first displaying (true if it's triggered and
 * false otherwise) */
static volatile uint32_t ul_first_display_finished = false;

/**
 * \brief Handler for vertical synchronisation used by the OV7740 image sensor.
 */
static void vsync_handler(uint32_t id, uint32_t mask)
{
	g_ul_vsync_flag = true;
}

/**
 * \brief Intialize Vsync_Handler.
 */
static void init_vsync_interrupts(void)
{
	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(OV7740_VSYNC_PIO, OV7740_VSYNC_ID, OV7740_VSYNC_MASK,
			OV7740_VSYNC_TYPE, vsync_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)OV7740_VSYNC_ID);
}

/**
 * \brief Initialize PIO capture for the OV7740 image sensor communication.
 *
 * \param p_pio PIO instance to be configured in PIO capture mode.
 * \param ul_id Corresponding PIO ID.
 */
static void pio_capture_init(Pio *p_pio, uint32_t ul_id)
{
	/* Enable peripheral clock*/
	pmc_enable_periph_clk(ul_id);

	/* Disable PIO capture */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_PCEN);

	/* Disable rxbuff interrupt */
	p_pio->PIO_PCIDR |= PIO_PCIDR_RXBUFF;

	/* 32bits width */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_DSIZE_Msk);
	p_pio->PIO_PCMR |= PIO_PCMR_DSIZE(2);

	/* Only HSYNC and VSYNC enabled */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_ALWYS);
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_HALFS);
}

/**
 * \brief Capture OV7740 data to a buffer.
 *
 * \param p_pio PIO instance.
 * \param p_uc_buf Buffer address where the first third of captured data must be
 * sent.
 * \param p_uc_nextbuf Buffer address where the second third of captured data
 * must be sent.
 * \param ul_size Data frame size.
 */
static uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *p_uc_buf,
		uint8_t *p_uc_nextbuf, uint32_t ul_size)
{
	/* Link the current PDC buffer to the buffer which contains the first
	 * third of captured data from the OV7740 */
	p_pio->PIO_RPR = (uint32_t)p_uc_buf;
	p_pio->PIO_RCR = ul_size / 4;

	/* Link the next PDC buffer to the buffer which contains the second
	 * third of captured data from the OV7740 */
	p_pio->PIO_RNPR = (uint32_t)p_uc_nextbuf;
	p_pio->PIO_RNCR = ul_size / 4;

	/* Enable channel */
	p_pio->PIO_PTCR = PIO_PTCR_RXTEN;

	return(0);
}

/**
 * \brief Intialize LCD display.
 */
static void display_init(void)
{
	struct ili9325_opt_t g_ili9325_display_opt;

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
			| SMC_MODE_WRITE_MODE);

	/* Initialize display parameter */
	g_ili9325_display_opt.ul_width = ILI9325_LCD_WIDTH;
	g_ili9325_display_opt.ul_height = ILI9325_LCD_HEIGHT;
	g_ili9325_display_opt.foreground_color = COLOR_BLACK;
	g_ili9325_display_opt.background_color = COLOR_WHITE;

	/* Switch off backlight */
	aat31xx_disable_backlight();

	/* Initialize LCD */
	ili9325_init(&g_ili9325_display_opt);

	/* Set backlight level */
	aat31xx_set_backlight(AAT31XX_MAX_BACKLIGHT_LEVEL);

	/* Turn on LCD */
	ili9325_display_on();
}

/**
 * \brief Initialize PIO capture and the OV7740 image sensor.
 */
static void capture_init(void)
{
	twi_options_t opt;

	/* Init Vsync handler*/
	init_vsync_interrupts();

	/* Init PIO capture*/
	pio_capture_init(OV7740_DATA_BUS_PIO, OV7740_DATA_BUS_ID);

	/* Turn on OV7740 image sensor using power pin*/
	ov_power(true, OV_POWER_PIO, OV_POWER_MASK);

	/* Init PCK0 to work at 24 Mhz */
	/* 96/4=24 Mhz */
	PMC->PMC_PCK[0] = (PMC_PCK_PRES_CLK_4 | PMC_PCK_CSS_PLLA_CLK);
	PMC->PMC_SCER = PMC_SCER_PCK0;
	while (!(PMC->PMC_SCSR & PMC_SCSR_PCK0)) {
	}

	/* Enable TWI peripheral */
	pmc_enable_periph_clk(ID_BOARD_TWI);

	/* Init TWI peripheral */
	opt.master_clk = sysclk_get_cpu_hz();
	opt.speed      = TWI_CLK;
	twi_master_init(BOARD_TWI, &opt);

	/* Configure TWI interrupts */
	NVIC_DisableIRQ(BOARD_TWI_IRQn);
	NVIC_ClearPendingIRQ(BOARD_TWI_IRQn);
	NVIC_SetPriority(BOARD_TWI_IRQn, 0);
	NVIC_EnableIRQ(BOARD_TWI_IRQn);

	/* OV7740 Initialization*/
	while (ov_init(BOARD_TWI) == 1) {
	}

	/* OV7740 configuration */
	ov_configure(BOARD_TWI, QVGA_YUV422_30FPS);

	/* Wait 3 seconds to let the image sensor to adapt to the environment */
	delay_ms(3000);
}

/**
 * \brief Configure SMC interface for SRAM.
 */
static void board_configure_sram( void )
{
	/* Enable peripheral clock */
	pmc_enable_periph_clk( ID_SMC );

	/* Configure SMC interface for SRAM */
	smc_set_setup_timing(SMC, SRAM_CS, SMC_SETUP_NWE_SETUP(2)
			| SMC_SETUP_NCS_WR_SETUP(0)
			| SMC_SETUP_NRD_SETUP(3)
			| SMC_SETUP_NCS_RD_SETUP(0));

	smc_set_pulse_timing(SMC, SRAM_CS, SMC_PULSE_NWE_PULSE(4)
			| SMC_PULSE_NCS_WR_PULSE(5)
			| SMC_PULSE_NRD_PULSE(4)
			| SMC_PULSE_NCS_RD_PULSE(6));

	smc_set_cycle_timing(SMC, SRAM_CS, SMC_CYCLE_NWE_CYCLE(6)
			| SMC_CYCLE_NRD_CYCLE(7));

	smc_set_mode(SMC, SRAM_CS, SMC_MODE_READ_MODE
			| SMC_MODE_WRITE_MODE);
}

/**
 * \brief Take a 32 bit variable in parameters and returns a value between 0 and
 * 255.
 *
 * \param i Enter value .
 * \return i if 0<i<255, 0 if i<0 and 255 if i>255
 */
static inline uint8_t clip32_to_8( int32_t i )
{
	if (i > 255) {
		return 255;
	}

	if (i < 0) {
		return 0;
	}

	return (uint8_t)i;
}

/**
 * \brief Draw LCD in color with integral algorithm.
 *
 * \param p_uc_cap_data_buffer Buffer address which contains data to display.
 */
static void draw_frame_yuv_color_int( uint8_t *p_uc_cap_data_buffer )
{
	uint32_t ul_cursor;
	int32_t l_y1;
	int32_t l_y2;
	int32_t l_v;
	int32_t l_u;
	int32_t l_blue;
	int32_t l_green;
	int32_t l_red;
	uint8_t *p_uc_data;

	p_uc_data = (uint8_t *)p_uc_cap_data_buffer;

	if (ul_first_display_finished == false) {
		LCD_IR(0);
		LCD_IR(ILI9325_ENTRY_MODE);
		LCD_WD(((ILI9325_ENTRY_MODE_BGR | ILI9325_ENTRY_MODE_AM |
				ILI9325_ENTRY_MODE_DFM |
				ILI9325_ENTRY_MODE_TRI |
				ILI9325_ENTRY_MODE_ORG) >> 8) & 0xFF);
		LCD_WD((ILI9325_ENTRY_MODE_BGR | ILI9325_ENTRY_MODE_AM |
				ILI9325_ENTRY_MODE_DFM |
				ILI9325_ENTRY_MODE_TRI |
				ILI9325_ENTRY_MODE_ORG) & 0xFF);
	ili9325_draw_prepare(0, 0, IMAGE_HEIGHT, IMAGE_WIDTH);
	}

	/* OV7740 color format is YUV422. In this format pixel has 4 bytes
	 * length(Y1,U,Y2,V).
	 * To display it on LCD, these pixel need to be converted in RGB format.
	 * The output of this conversion is two 3 bytes pixels in (B,G,R)
	 * format.
	 * First one is calculed using
	 * Y1,U,V and the other one with Y2,U,V. For that reason cap_line is
	 * twice bigger in color mode than in black and white mode.
	 */
	for (ul_cursor = (IMAGE_WIDTH * IMAGE_HEIGHT) / 3; ul_cursor != 0;
			ul_cursor -= 2, p_uc_data += 4) {
		l_y1 = p_uc_data[0]; /* Y1 */
		l_y1 -= 16;
		l_v = p_uc_data[3]; /* V */
		l_v -= 128;
		l_u = p_uc_data[1]; /* U */
		l_u -= 128;

		l_blue = 516 * l_v + 128;
		l_green = -100 * l_v - 208 * l_u + 128;
		l_red = 409 * l_u + 128;

		/* BLUE */
		LCD_WD( clip32_to_8((298 * l_y1 + l_blue) >> 8));
		/* GREEN */
		LCD_WD( clip32_to_8((298 * l_y1 + l_green) >> 8));
		/* RED */
		LCD_WD( clip32_to_8((298 * l_y1 + l_red) >> 8));

		l_y2 = p_uc_data[2]; /* Y2 */
		l_y2 -= 16;
		LCD_WD( clip32_to_8((298 * l_y2 + l_blue) >> 8));
		LCD_WD( clip32_to_8((298 * l_y2 + l_green) >> 8));
		LCD_WD( clip32_to_8((298 * l_y2 + l_red) >> 8));
	}
}

/**
 * \brief Application entry point for image sensor capture example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	board_init();
	sysclk_init();

	/* OV7740 send image sensor data at 24 Mhz. For best performances, PCK0
	 * which will capture OV7740 data, has to work at 24Mhz. It's easier
	 * and optimum to use one PLL for core (PLLB) and one other for PCK0
	 * (PLLA).
	 */
	pmc_enable_pllack(7, 0x1, 1);

	/* LCD display initialization */
	display_init();

	/* Display information */
	ili9325_fill(COLOR_TURQUOISE);
	ili9325_draw_string(0, 20,
			(uint8_t *)"Continuous capture\ndemo without\nexternal sram");
	ili9325_draw_string(0, 100,
			(uint8_t *)"Please wait during\ninitialization");

	/* Configure SMC interface for external SRAM. This SRAM will be used
	 * to store picture during image sensor acquisition.
	 */
	board_configure_sram();

	/* OV7740 image sensor initialization. */
	capture_init();

#ifdef FRAME_RATE_MEASURE
	/* configure test pin */
	pmc_enable_periph_clk(ID_PIN_TEST);
	pio_configure_pin(PIN_TEST, (PIO_OUTPUT_1 | PIO_DEFAULT));
#endif

	while (1) {
		/* Enable vsync interrupt */
		pio_enable_interrupt(OV7740_VSYNC_PIO, OV7740_VSYNC_MASK);

		/* Capture acquisition will start on rising edge of Vsync
		 * signal.
		 * So wait vsync_flag = 1 before starting process
		 */
		while (!g_ul_vsync_flag) {
		}

#ifdef FRAME_RATE_MEASURE
		pio_set_pin_high(PIN_TEST); /* set test pin to high level */
#endif

		/* Enable vsync interrupt */
		pio_disable_interrupt(OV7740_VSYNC_PIO, OV7740_VSYNC_MASK);

		/* Reset vsync flag */
		g_ul_vsync_flag = false;

		/* Link the first PDC banks to uc_capture_buffer_1 and the
		 * second PDC bank to uc_capture_buffer_1 */
		pio_capture_to_buffer(OV7740_VSYNC_PIO, uc_capture_buffer_1,
				uc_capture_buffer_2,
				(g_us_cap_rows * g_us_cap_line / 3));

		/* Enable PIO capture */
		pio_capture_enable(OV7740_DATA_BUS_PIO);

		/* Wait end of capture of first third of picture into
		 * uc_capture_buffer_1 */
		while (!((OV7740_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_ENDRX) ==
				PIO_PCIMR_ENDRX)) {
		}

		/* uc_capture_buffer_1 is full with data of first third of
		 * picture. So uc_capture_buffer_2 is automatically
		 * linked to the first PDC bank in order to receive the second
		 * third of picture. uc_capture_buffer_1 will be
		 * used to reload next pdc buffer.
		 */
		/* Reload Next Buffer */
		PIOA->PIO_RNPR = (uint32_t)uc_capture_buffer_1;

		/* Size of buffer is /4 due to 32-bit word transfer */
		PIOA->PIO_RNCR = (g_us_cap_rows * g_us_cap_line / 3) / 4;

		/* Display the first third of picture during the second third is
		 * captured into uc_capture_buffer_2 */
		draw_frame_yuv_color_int( uc_capture_buffer_1 );

		/* Set ul_first_display_finished flag */
		ul_first_display_finished = true;

		/* Wait end of capture of second third of picture into
		 * uc_capture_buffer_2 */
		while (!((OV7740_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_ENDRX) ==
				PIO_PCIMR_ENDRX)) {
		}

		/* uc_capture_buffer_2 is full with data of second third of
		 * picture. So uc_capture_buffer_1 is automatically
		 * linked to the first PDC bank in order to receive the last
		 * third of picture . Display uc_capture_buffer_2
		 * data during the last third of picture of picture is captured
		 * into uc_capture_buffer_1.
		 */
		draw_frame_yuv_color_int( uc_capture_buffer_2 );

		/* Display the last third of picture */
		draw_frame_yuv_color_int( uc_capture_buffer_1 );

#ifdef FRAME_RATE_MEASURE
		pio_set_pin_low(PIN_TEST); /* set test pin to low level */
#endif

		/* Reset the ul_first_display_finished flag */
		ul_first_display_finished = false;
	}
}
