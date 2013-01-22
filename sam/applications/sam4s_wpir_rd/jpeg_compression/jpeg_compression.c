/**
 * \file
 *
 * \brief JPEG compression example.
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

/**
 * \mainpage JPEG compression example.
 *
 * \section Purpose
 *
 * This example demonstrates how to use JPEG library to compress and uncompress
 * a picture captured by the image sensor.
 *
 * \section Requirements
 *
 * This package can be used with sam4s_wpir_rd board.
 *
 * \section Description
 *
 * This example first sets the PLLB to the system core clock and PLLA to clock
 * PCK0 (used to get data from image sensor). The systick then gets configured
 * for time measurement.
 * Next step is to configure the LCD controller and display information to the
 * user and configure the push button to allow the user to switch between
 * color or black and white mode.
 * Then the external SRAM, which is used to store data after acquisition, is
 * configured.
 * Finally this example configures the OV7740 CMOS image sensor and the PIO
 * capture.
 * So this example will continually take picture and display it on the LCD as a
 * camera.
 * When the user presses the push button, a picture is be captured and stored
 * in the external SRAM.Then the JPEG library is used to compress this picture.
 * Finally this picture is uncompressed and displayed on the LCD with
 * compression information.
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
 */

#include "asf.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "libjpeg.h"

/* TWI clock frequency in Hz (400KHz) */
#define TWI_CLK     400000

/* Define image size according to image configuration */
#define IMAGE_SIZE_RGB_QVGA    (240 * 320 * 3)
#define IMAGE_SIZE_RGB_VGA     (480 * 640 * 3)
#define IMAGE_SIZE_YUV422_QVGA (240 * 320 * 2)

/* Define image size source and destination for this example */
#define IMAGE_SIZE_DST  IMAGE_SIZE_YUV422_QVGA
#define IMAGE_SIZE_SRC   IMAGE_SIZE_RGB_VGA

/* Define string which contains information about compress/uncompress time */
#define STRING_RESULT \
	"compress:%u ms\n\nuncompress:%u ms\n\nimage size after\ncompress:%u bytes\n\nimage size ratio\nof compress:%u%%"

/* Structure for JPEG compression/uncompression */
typedef struct jpeg_test {
	const uint8_t *p_uc_data; /* image buffer */
	uint32_t ul_data_length; /* length of data */
	uint32_t ul_time_c; /* compression time */
	uint32_t ul_time_d; /* uncompression time */
	uint32_t ul_final_length; /* final length */
} jpeg_test_t;

/* Row size of captured picture */
uint16_t g_us_cap_rows = IMAGE_HEIGHT;

/* Line size of captured picture (according to color mode by default) */
uint16_t g_us_cap_line = IMAGE_WIDTH * 2;

/* Vsync signal information (true if it's triggered and false otherwise) */
static volatile uint32_t g_ul_vsync_flag = false;

/* Push button information (true if it's triggered and false otherwise) */
static volatile uint32_t g_ul_push_button_trigger = false;

/* Image to be compressed/uncompressed */
static jpeg_test_t g_p_image;

/* Source and destination buffer of the image to be compressed/uncompressed */
static uint8_t *g_p_uc_image_source;
static uint8_t *g_p_uc_image_destination;

/* Time information to fill LCD's RAM with an empty wallpaper and with a picture
 **/
static uint32_t g_ul_time_empty_wallpaper;
static uint32_t g_ul_time_display_picture;

/* Systick increased every 1 ms */
static uint32_t g_ul_time_systick;

/* String used to store information before the use of ili9325_draw_string
 * function */
static uint8_t g_uc_size_result[200];

/* Captured image data buffer */
static uint8_t *g_p_uc_capture_buf;

/* Data used for JPEG compression */
static uint8_t *g_p_uc_index;
static uint8_t g_uc_line[2048];

/**
 * \brief Handler for vertical synchronisation using by OV7740 image sensor.
 */
static void vsync_handler(uint32_t id, uint32_t mask)
{
	g_ul_vsync_flag = true;
}

/**
 * \brief Handler for button rising edge interrupt.
 */
static void button_handler(uint32_t id, uint32_t mask)
{
	g_ul_push_button_trigger = true;
}

/**
 * \brief Interrupt Handler for systick.
 */
void SysTick_Handler( void )
{
	g_ul_time_systick++;
}

/**
 * \brief Copying uncompressed data to display buffer.
 */
static uint32_t ijg_callback( uint8_t *p_uc_rows, uint32_t ul_length )
{
	memcpy( g_p_uc_index, p_uc_rows, ul_length );
	g_p_uc_index += ul_length;

	return 0;
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
 * \brief Configure push button and initialize button_handler interrupt.
 */
static void configure_button(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(PUSH_BUTTON_ID);

	/* Adjust PIO debounce filter parameters, uses 10 Hz filter. */
	pio_set_debounce_filter(PUSH_BUTTON_PIO, PUSH_BUTTON_PIN_MSK, 10);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(PUSH_BUTTON_PIO, PUSH_BUTTON_ID, PUSH_BUTTON_PIN_MSK,
			PUSH_BUTTON_ATTR, button_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)PUSH_BUTTON_ID);

	/* Enable PIO line interrupts. */
	pio_enable_interrupt(PUSH_BUTTON_PIO, PUSH_BUTTON_PIN_MSK);
}

/**
 * \brief Compression and uncompression routine.
 */
static void jpeg_benchmark_pirrd_raw( jpeg_test_t *p_image )
{
	uint8_t *p_uc_cursor;
	uint32_t i;
	SJpegData jpeg_data;

	p_image->ul_time_c = g_ul_time_systick;

	/* Compress image */
	JpegData_Init( &jpeg_data );

	/* Use captured buffer as direct source */
	JpegData_SetSource( &jpeg_data, g_p_uc_capture_buf, 320 * 240 * 3 );
	JpegData_SetDestination( &jpeg_data, g_p_uc_image_destination,
			320 * 240 * 3 );
	JpegData_SetDimensions( &jpeg_data, 320, 240, 3 );
	JpegData_SetParameters( &jpeg_data, 75, JPG_DATA_YCbCr,
			JPG_METHOD_IFAST );

	if (ijg_compress_raw_no_padding( &jpeg_data ) == 0) {
		p_image->ul_time_c = 1 + g_ul_time_systick - p_image->ul_time_c;
		p_image->ul_final_length = jpeg_data.dwDstLength;

		ili9325_draw_prepare(0, 0, IMAGE_HEIGHT, IMAGE_WIDTH);

		ili9325_fill(COLOR_TURQUOISE);

		p_image->ul_time_d = g_ul_time_systick;
		/* Set uncompression */
		JpegData_Init( &jpeg_data );
		JpegData_SetSource( &jpeg_data, g_p_uc_image_destination,
				320 * 240 * 3 );
		JpegData_SetDestination( &jpeg_data, g_uc_line,
				sizeof(g_uc_line));
		JpegData_SetDimensions( &jpeg_data, 320, 240, 3 );
		JpegData_SetParameters( &jpeg_data, 75, JPG_DATA_RGB,
				JPG_METHOD_IFAST );
		JpegData_SetCallback( &jpeg_data, ijg_callback );

		if (ijg_decompress( &jpeg_data ) == 0) {
			p_image->ul_time_d = 1 + g_ul_time_systick -
					p_image->ul_time_d;

			p_uc_cursor = g_p_uc_image_source;
			g_ul_time_display_picture = g_ul_time_systick;

			LCD_IR(0);
			LCD_IR(ILI9325_ENTRY_MODE);
			LCD_WD(((ILI9325_ENTRY_MODE_BGR |
					ILI9325_ENTRY_MODE_AM |
					ILI9325_ENTRY_MODE_DFM |
					ILI9325_ENTRY_MODE_TRI |
					ILI9325_ENTRY_MODE_ORG) >> 8) & 0xFF);
			LCD_WD((ILI9325_ENTRY_MODE_BGR | ILI9325_ENTRY_MODE_AM |
					ILI9325_ENTRY_MODE_DFM |
					ILI9325_ENTRY_MODE_TRI |
					ILI9325_ENTRY_MODE_ORG) & 0xFF);
			ili9325_draw_prepare(0, 0, IMAGE_HEIGHT, IMAGE_WIDTH);

			for (i = 0; i < IMAGE_SIZE_RGB_QVGA; i++) {
				LCD_WD( *(p_uc_cursor++));
			}
			g_ul_time_display_picture = 1 + g_ul_time_systick -
					g_ul_time_display_picture;
		}
	}
}

/**
 * \brief Initialize PIO capture for the OV7740 image sensor communication.
 *
 * \param p_pio PIO instance to be configured in PIO capture mode.
 * \param ul_id Corresponding PIO ID.
 */
static void pio_capture_init(Pio *p_pio, uint32_t ul_id)
{
	/* Enable peripheral clock */
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
 * \param ul_size Data frame size.
 */
static uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf,
		uint32_t ul_size)
{
	/* Check if the first PDC bank is free */
	if ((p_pio->PIO_RCR == 0) && (p_pio->PIO_RNCR == 0)) {
		p_pio->PIO_RPR = (uint32_t)uc_buf;
		p_pio->PIO_RCR = ul_size;
		p_pio->PIO_PTCR = PIO_PTCR_RXTEN;
		return 1;
	} else if (p_pio->PIO_RNCR == 0) {
		p_pio->PIO_RNPR = (uint32_t)uc_buf;
		p_pio->PIO_RNCR = ul_size;
		return 1;
	} else {
		return 0;
	}
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

	/* Init Vsync handler */
	init_vsync_interrupts();

	/* Init PIO capture */
	pio_capture_init(OV7740_DATA_BUS_PIO, OV7740_DATA_BUS_ID);

	/* Turn on OV7740 image sensor using power pin */
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

	/* OV7740 Initialization */
	while (ov_init(BOARD_TWI) == 1) {
	}

	/* OV7740 configuration */
	ov_configure(BOARD_TWI, QVGA_YUV422_20FPS);

	/* Wait 3 seconds to let the image sensor to adapt to environment */
	delay_ms(3000);
}

/**
 * \brief Start picture capture.
 */
static void start_capture(uint8_t *p_uc_dest)
{
	uint8_t *p_uc_buf;

	p_uc_buf = p_uc_dest;

	/* Set cap_rows value */
	g_us_cap_rows = IMAGE_HEIGHT;

	/* Enable vsync interrupt */
	pio_enable_interrupt(OV7740_VSYNC_PIO, OV7740_VSYNC_MASK);

	/* Capture acquisition will start on rising edge of Vsync signal.
	 * So wait vsync_flag = 1 before starting process
	 */
	while (!g_ul_vsync_flag) {
	}

	/* Disable vsync interrupt */
	pio_disable_interrupt(OV7740_VSYNC_PIO, OV7740_VSYNC_MASK);

	/* Enable PIO capture */
	pio_capture_enable(OV7740_DATA_BUS_PIO);

	/* Capture data and send it to external SRAM memory using PDC */
	pio_capture_to_buffer(OV7740_DATA_BUS_PIO, p_uc_buf,
			(g_us_cap_line * g_us_cap_rows) >> 2);

	/* Wait end of capture */
	while (!((OV7740_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_RXBUFF) ==
			PIO_PCIMR_RXBUFF)) {
	}

	/* Disable PIO capture */
	pio_capture_disable(OV7740_DATA_BUS_PIO);

	/* Reset vsync flag */
	g_ul_vsync_flag = false;
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
 * \brief Application entry point for image sensor capture example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint8_t uc_ratio;

	board_init();
	sysclk_init();

	/* OV7740 send image sensor data at 24 Mhz. For best performances, PCK0
	 * which will capture OV7740 data, has to work at 24Mhz. It's easier
	 * and optimum to use one PLL for core (PLLB) and one other for PCK0
	 * (PLLA). */
	pmc_enable_pllack(7, 0x1, 1);

	/* Initialization using buffer */
	g_p_uc_capture_buf = (uint8_t *)CAP_DEST;
	g_p_uc_image_source = (uint8_t *)CAP_DEST;
	g_p_uc_image_destination =  (uint8_t *)(CAP_DEST + IMAGE_SIZE_SRC + 1);

	/* Configure systick timer to make time measurement */
	g_ul_time_systick = 0;
	SysTick_Config(sysclk_get_cpu_hz() / 1000);

	/* LCD display initialization */
	display_init();

	/* Display information */
	ili9325_fill(COLOR_TURQUOISE);
	ili9325_draw_string( 0, 10, "JPEG compression \nexample ");
	ili9325_draw_string(0, 60, "Please wait during\ninitialization");

	/* Configure push button as a interrupt source */
	configure_button();

	/* Configure external SRAM */
	board_configure_sram();

	/* OV7740 image sensor initialization */
	capture_init();

	/* Display information */
	ili9325_fill(COLOR_TURQUOISE);
	ili9325_draw_string( 0, 10, "JPEG compression \nexample ");
	ili9325_draw_string(0, 60,
			"Press BP1 button to\nstart capture and\ncompress/uncompress");

	while (1) {
		while (!g_ul_push_button_trigger) {
			/* Press push button to get out */
		}

		/* Reset push button trigger flag */
		g_ul_push_button_trigger = false;

		/* Capture a picture and send corresponding data to external
		 * memory */
		start_capture(g_p_uc_capture_buf);

		g_p_uc_index = g_p_uc_image_source;

		/* compression and uncompression */
		jpeg_benchmark_pirrd_raw( &g_p_image );

		delay_ms(1000);

		/* Measure time to fill the LCD's RAM with an empty wallpaper */
		g_ul_time_empty_wallpaper = g_ul_time_systick;
		ili9325_fill(COLOR_TURQUOISE);
		g_ul_time_empty_wallpaper = 1 + g_ul_time_systick -
				g_ul_time_empty_wallpaper;

		/* LCD display initialization */
		display_init();

		/* Display information about g_ul_time_empty_wallpaper */
		sprintf((char *)g_uc_size_result, "display a\nwallpaper:%u ms",
				g_ul_time_empty_wallpaper);
		ili9325_draw_string(0, 10, g_uc_size_result);

		/* Display information about g_ul_time_display_picture */
		sprintf((char *)g_uc_size_result, "display a\npicture:%u ms",
				g_ul_time_display_picture);
		ili9325_draw_string(0, 60, g_uc_size_result);

		/* Display information about compress/uncompress time and size */
		uc_ratio = (float)g_p_image.ul_final_length * 100.0 /
				(float)IMAGE_SIZE_YUV422_QVGA;
		snprintf((char *)g_uc_size_result, sizeof(g_uc_size_result),
				STRING_RESULT, g_p_image.ul_time_c,
				g_p_image.ul_time_d, g_p_image.ul_final_length,
				uc_ratio );
		ili9325_draw_string( 0, 110,
				(const uint8_t *)g_uc_size_result );
	}
}
