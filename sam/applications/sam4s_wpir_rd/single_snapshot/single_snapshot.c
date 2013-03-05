/**
 * \file
 *
 * \brief Single snapshot example.
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
 * \mainpage Single snapshot example.
 *
 * \section Purpose
 *
 * This example demonstrates how to configure the OV7740 sensor to capture a
 * picture and display it on the LCD using low power mode.
 *
 * \section Requirements
 *
 * This package can be used with sam4s_wpir_rd board.
 *
 * \section Description
 *
 * This example first configures PLLB to clock the system core and PLLA to clock
 * the PCK0 signal (used to get data from image sensor). The next step is to
 * configure the LCD controller and display information to the user.
 * Then external SRAM is then configured to store data after acquisition.
 * And finally this example configures the OV7740 CMOS image sensor and
 * PIO capture.
 * When global configuration is done, this example takes a picture, stores it
 * in the external SRAM and display it on the LCD.
 * The MCU enters in backup mode and the push button is configured as wake up
 * source.
 * When the user presses the push button, the MCU wakes up from backup mode and
 * starts the previous process.
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

/* Uncomment this macro to work in black and white mode */
#define DEFAULT_MODE_COLORED

/* TWI clock frequency in Hz (400KHz) */
#define TWI_CLK     400000

/* Customized value which will be stored in GPBR0 register to know if it's */
/* the first entry in this example. */
#define BACKUP_FLAG 0xFA5F5AA5

/* Macro to control the staying in each state permanently, comment for
 * continuous transition */
/* #define STATE_STAYED */

/* Display information on the LCD during Step by Step mode (uncomment
 * STATE_STAYED). */
/* #define LCD_DISPLAY_INFORMATION */

/* Macro to reset test pin, stay in current state (until the user presses the
 * push button), display info on the LCD. */
/* (if LCD_DISPLAY_INFORMATION is uncomment) and finally set test pin. */
#ifdef STATE_STAYED
static void display_info(uint32_t state);

	#ifdef LCD_DISPLAY_INFORMATION
		#define WAIT_FOR_STATE(STATE) \
	pio_set_pin_low(PIN_TEST_GPIO);	\
	display_info(STATE); \
	wait_for_state(STATE); \
	pio_set_pin_high(PIN_TEST_GPIO);
	#else
		#define WAIT_FOR_STATE(STATE) \
	pio_set_pin_low(PIN_TEST_GPIO);	\
	wait_for_state(STATE); \
	pio_set_pin_high(PIN_TEST_GPIO);
	#endif
#else
	#define WAIT_FOR_STATE(STATE)
#endif

/* Enumeration of all state */
typedef enum {
	STATE_INIT, /* initalized state */
	STATE_START, /* start state */
	STATE_WAKE_UP, /* state before LCD initialization */
	STATE_LCD_INIT, /* state before SRAM initialization */
	STATE_SRAM_INIT, /* state before capture initialization */
	STATE_CAPTURE_INIT, /* state before capture */
	STATE_CAPTURE_DONE, /* state before image displaying and enter in wait
	                     * mode */
	STATE_END /* reserved */
} STATE_TYPE;

/* Pointer to the image data destination buffer */
uint8_t *g_p_uc_cap_dest_buf;

/* Row size of capturing picture */
uint16_t g_us_cap_rows = IMAGE_HEIGHT;

/* Define display function and line size of captured picture according to the */
/* selected color mode. */
#ifdef DEFAULT_MODE_COLORED
	#define display_buffer() draw_frame_yuv_color_int()

/* (IMAGE_WIDTH *2 ) because ov7740 use YUV422 format in color mode */
/* (draw_frame_yuv_color_int for more details) */
uint16_t g_us_cap_line = (IMAGE_WIDTH * 2);
#else
	#define display_buffer() draw_frame_yuv_bw8()

uint16_t g_us_cap_line = (IMAGE_WIDTH);
#endif

/* Vsync signal information (true if it's triggered and false otherwise) */
static volatile uint32_t g_ul_vsync_flag = false;

/* Define the current state for step by step mode (uncomment STATE_STAYED) */
volatile STATE_TYPE ul_cur_state = STATE_INIT;

/**
 * \brief Handler for vertical synchronisation used by the OV7740 image sensor.
 */
static void vsync_handler(uint32_t id, uint32_t mask)
{
	g_ul_vsync_flag = true;
}

/**
 * \brief Handler for PIO capture. It will happen at the end of the capture.
 */
void PIO_capture_handler(void)
{
	if (OV7740_DATA_BUS_PIO->PIO_PCISR != 0) {
		OV7740_DATA_BUS_PIO->PIO_PCIDR |= PIO_PCIDR_RXBUFF;
	}
}

#ifdef STATE_STAYED

/**
 * \brief Handler for Button rising edge interrupt. It also changes the current
 * state.
 */
static void button_handler(uint32_t id, uint32_t mask)
{
	switch (ul_cur_state) {
	case STATE_INIT:
		ul_cur_state = STATE_START;
		break;

	case STATE_START:
		ul_cur_state = STATE_WAKE_UP;
		break;

	case STATE_WAKE_UP:
		ul_cur_state = STATE_LCD_INIT;
		break;

	case STATE_LCD_INIT:
		ul_cur_state = STATE_SRAM_INIT;
		break;

	case STATE_SRAM_INIT:
		ul_cur_state = STATE_CAPTURE_INIT;
		break;

	case STATE_CAPTURE_INIT:
		ul_cur_state = STATE_CAPTURE_DONE;
		break;

	case STATE_CAPTURE_DONE:
		ul_cur_state = STATE_INIT;
		break;

	default:
		ul_cur_state = STATE_END;
		break;
	}
}

#endif

#ifdef STATE_STAYED
/* static __ramfunc */

/**
 * \brief Wait a state changes (in step by step mode) using the push button
 * trigger.
 */
static void wait_for_state(uint32_t ul_state)
{
	while (ul_cur_state != ul_state) {
	}
}

/**
 * \brief Display information about the current state (STATE_STAYED must be
 * uncommented).
 */
static void display_info(uint32_t ul_state)
{
	switch (ul_state) {
	case STATE_LCD_INIT:
		ili9325_fill(COLOR_TURQUOISE);
		ili9325_draw_string(0, 10, "LCD Initialized");
		ili9325_draw_string(0, 50,
				"Press BP1 button to\nconfigure external\nsram");
		break;

	case STATE_SRAM_INIT:
		ili9325_fill(COLOR_TURQUOISE);
		ili9325_draw_string(0, 10, "External sram\nconfigured");
		ili9325_draw_string(0, 60,
				"Press BP1 button to\ninitialize image\nsensor");
		break;

	case STATE_CAPTURE_INIT:
		ili9325_fill(COLOR_TURQUOISE);
		ili9325_draw_string(0, 10, "Capture Initialized");
		ili9325_draw_string(0, 50,
				"Press PB1 button to\ntake a picture");
		break;

	case STATE_CAPTURE_DONE:
		ili9325_fill(COLOR_TURQUOISE);
		ili9325_draw_string(0, 10, "Capture done");
		ili9325_draw_string(0, 50,
				"Press PB1 button to\ndisplay picture and\nenter in backup mode");
		break;

	default:
		/* do nothing */
		break;
	}
}

#endif

/**
 * \brief Intialize Vsync_Handler
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

#ifdef STATE_STAYED

/**
 * \brief Configure push button and initialize button_handler interrupt.
 */
static void configure_button(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(PUSH_BUTTON_ID);

	/* Adjust PIO debounce filter parameters using a 10 Hz filter. */
	pio_set_debounce_filter(PUSH_BUTTON_PIO, PUSH_BUTTON_PIN_MSK, 10);

	/* Initialize PIO interrupt handlers, see PIO definition in conf_board.h
	**/
	pio_handler_set(PUSH_BUTTON_PIO, PUSH_BUTTON_ID, PUSH_BUTTON_PIN_MSK,
			PUSH_BUTTON_ATTR, button_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)PUSH_BUTTON_ID);

	/* Enable PIO line interrupts. */
	pio_enable_interrupt(PUSH_BUTTON_PIO, PUSH_BUTTON_PIN_MSK);
}

#endif

/**
 * \brief prepare chip for low power mode
 */
static void low_power_prepare(void)
{
	/* Disable all peripherique*/
	pmc_disable_all_periph_clk();

	/* Put pull-up on input io to reduce power consumption*/
	gpio_configure_group(PIOA, PIOA_INPUT, PIO_INPUT | PIO_PULLUP);
	gpio_configure_group(PIOB, PIOB_INPUT, PIO_INPUT | PIO_PULLUP);
	gpio_configure_group(PIOC, PIOC_INPUT, PIO_INPUT | PIO_PULLUP);

	/* Configure output io in default mode to reduce power consumption*/
	gpio_configure_group(PIOA, PIOA_OUTPUT, PIO_OUTPUT_0 | PIO_DEFAULT);
	gpio_configure_group(PIOB, PIOB_OUTPUT, PIO_OUTPUT_0 | PIO_DEFAULT);
	gpio_configure_group(PIOC, PIOC_OUTPUT, PIO_OUTPUT_0 | PIO_DEFAULT);

	/* Disable USB Clock */
	REG_PMC_SCDR = PMC_SCER_UDP;
}

/**
 * \brief Configure microcontroller to enter sleep mode.
 * Enter condition: WFI + (SLEEPDEEP bit = 0) + (LPM bit = 0)
 */
static void enter_sleep_mode( void )
{
	/* enable sleep mode*/
	/*PMC->PMC_FSMR &= (uint32_t)~PMC_FSMR_LPM ;*/

	/* clear DEEPSLEEP bit*/
	SCB->SCR &= (uint32_t) ~SCB_SCR_SLEEPDEEP_Msk;

	/* Call WFI instruction to enter in sleep mode*/
	__WFI();
}

/**
 * \brief prepare chip for low power mode
 */
static void enter_backup_mode(void)
{
	/* Enable the PIO for wake-up */
	SUPC->SUPC_WUIR = SUPC_WUIR_WKUPEN2;
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	/* Set VROFF bit  */
	SUPC->SUPC_CR |= SUPC_CR_KEY(0xA5u) | SUPC_CR_VROFF_STOP_VREG;
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

#if !defined(DEFAULT_MODE_COLORED)
	/* Samples only data with even index */
	p_pio->PIO_PCMR |= PIO_PCMR_HALFS;
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_FRSTS);
#endif
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
 * \brief Intialize the LCD display.
 */
static void display_init(void)
{
	struct ili9325_opt_t g_ili9325display_buffer_opt;

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
	g_ili9325display_buffer_opt.ul_width = ILI9325_LCD_WIDTH;
	g_ili9325display_buffer_opt.ul_height = ILI9325_LCD_HEIGHT;
	g_ili9325display_buffer_opt.foreground_color = COLOR_BLACK;
	g_ili9325display_buffer_opt.background_color = COLOR_WHITE;

	/* Switch off backlight */
	aat31xx_disable_backlight();

	/* Initialize LCD */
	ili9325_init(&g_ili9325display_buffer_opt);

	/* Set backlight level */
	aat31xx_set_backlight(AAT31XX_MAX_BACKLIGHT_LEVEL);

	/* Turn on LCD */
	ili9325_display_on();
}

/**
 * \brief Initialize PIO capture and OV7740 component.
 */
static void capture_init(void)
{
	twi_options_t opt;

	/* Init Vsync handler*/
	init_vsync_interrupts();

	/* Init PIO capture */
	pio_capture_init(OV7740_DATA_BUS_PIO, OV7740_DATA_BUS_ID);

	/* Turn on OV7740 image sensor using power pin */
	ov_power(true, OV_POWER_PIO, OV_POWER_MASK);

	/* Init PCK0 to work at 24 Mhz*/
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

	/* Check if it is the first init*/
	if (!(gpbr_read(GPBR0) == BACKUP_FLAG)) {
		/* Wait 3 seconds to let the image sensor to adapt to the
		 * environment */
		delay_ms(3000);
		/* Save configuration */
		ov_store_manual(BOARD_TWI, &(GPBR->SYS_GPBR[2]), 2);
		gpbr_write(GPBR0, BACKUP_FLAG);
	} else {
		/* Use the previously saved configuration from first
		 * initialization */
		ov_restore_manual(&(GPBR->SYS_GPBR[2]), 2);
		ov_configure_manual(BOARD_TWI);

		/* Wait 5 ms to let the image sensor to adapt to the environment
		**/
		delay_ms(5);
	}
}

/**
 * \brief Start picture capture.
 */
static void start_capture(void)
{
	/* Set capturing destination address */
	g_p_uc_cap_dest_buf = (uint8_t *)CAP_DEST;

	/* Set cap_rows value */
	g_us_cap_rows = IMAGE_HEIGHT;

	/* Capture acquisition will start on rising edge of Vsync signal.
	 * So wait vsync_flag = 1 before starting process
	 */
	while (!g_ul_vsync_flag) {
		/* Enable vsync interrupt */
		pio_enable_interrupt(OV7740_VSYNC_PIO, OV7740_VSYNC_MASK);

		/* Enter in sleep to reduce consumption. The vsync interrupt
		 * will wake up the core */
		enter_sleep_mode();

		/* Disable vsync interrupt */
		pio_disable_interrupt(OV7740_VSYNC_PIO, OV7740_VSYNC_MASK);
	}

	/* Enable PIO capture */
	pio_capture_enable(OV7740_DATA_BUS_PIO);

	/* Capture data and send it to external SRAM memory using PDC */
	pio_capture_to_buffer(OV7740_DATA_BUS_PIO, g_p_uc_cap_dest_buf,
			(g_us_cap_line * g_us_cap_rows) >> 2);

	/* Enable interrupt to wake up the core */
	OV7740_DATA_BUS_PIO->PIO_PCIER |= PIO_PCIER_RXBUFF;

	/* Enter in sleep to reduce consumption. The PIO_PCIMR_RXBUFF signal
	 * will wake up the core. */
	enter_sleep_mode();

	/* Wait end of capture */
	while (!((OV7740_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_RXBUFF) ==
			PIO_PCIMR_RXBUFF)) {
	}

	/* Disable pio capture */
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

#ifdef DEFAULT_MODE_COLORED

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
 */
static void draw_frame_yuv_color_int( void )
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

	p_uc_data = (uint8_t *)g_p_uc_cap_dest_buf;

	/* Configure LCD to draw captured picture */
	LCD_IR(0);
	LCD_IR(ILI9325_ENTRY_MODE);
	LCD_WD(((ILI9325_ENTRY_MODE_BGR | ILI9325_ENTRY_MODE_AM |
			ILI9325_ENTRY_MODE_DFM | ILI9325_ENTRY_MODE_TRI |
			ILI9325_ENTRY_MODE_ORG) >> 8) & 0xFF);
	LCD_WD((ILI9325_ENTRY_MODE_BGR | ILI9325_ENTRY_MODE_AM |
			ILI9325_ENTRY_MODE_DFM | ILI9325_ENTRY_MODE_TRI |
			ILI9325_ENTRY_MODE_ORG) & 0xFF);
	ili9325_draw_prepare(0, 0, IMAGE_HEIGHT, IMAGE_WIDTH);

	/* OV7740 Color format is YUV422. In this format pixel has 4 bytes
	 * length (Y1,U,Y2,V).
	 * To display it on the LCD, these pixel need to be converted in RGB
	 * format. The output
	 * of this conversion is two 3 bytes pixels in (B,G,R) format. First one
	 * is calculed using
	 * Y1,U,V and the other one with Y2,U,V. For that reason cap_line is
	 * twice bigger in color mode than in black and white mode.
	 */
	for (ul_cursor = IMAGE_WIDTH * IMAGE_HEIGHT; ul_cursor != 0;
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

#else

/**
 * \brief Draw LCD in black and white with integral algorithm.
 */
static void draw_frame_yuv_bw8( void )
{
	volatile uint32_t ul_cursor;
	uint8_t *p_uc_data;

	p_uc_data = (uint8_t *)g_p_uc_cap_dest_buf;

	/* Configure LCD to draw captured picture */
	LCD_IR(0);
	LCD_IR(ILI9325_ENTRY_MODE);
	LCD_WD(((ILI9325_ENTRY_MODE_BGR | ILI9325_ENTRY_MODE_AM |
			ILI9325_ENTRY_MODE_DFM | ILI9325_ENTRY_MODE_TRI |
			ILI9325_ENTRY_MODE_ORG) >> 8) & 0xFF);
	LCD_WD((ILI9325_ENTRY_MODE_BGR | ILI9325_ENTRY_MODE_AM |
			ILI9325_ENTRY_MODE_DFM | ILI9325_ENTRY_MODE_TRI |
			ILI9325_ENTRY_MODE_ORG) & 0xFF);
	ili9325_draw_prepare(0, 0, IMAGE_HEIGHT, IMAGE_WIDTH);

	/* LCD pixel has 24bit data. In black and White mode data has 8bit only
	 * so
	 * this data must be written three time in LCD memory.
	 */
	for (ul_cursor = IMAGE_WIDTH * IMAGE_HEIGHT; ul_cursor != 0;
			ul_cursor--, p_uc_data++) {
		/* Black and White using Y */
		LCD_WD( *p_uc_data );
		LCD_WD( *p_uc_data );
		LCD_WD( *p_uc_data );
	}
}

#endif

/**
 * \brief Application entry point for image sensor capture example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
#ifdef STATE_STAYED
	/* Configure test pin */
	pmc_enable_periph_clk(PIN_TEST_ID);
	pio_configure_pin(PIN_TEST_GPIO, PIN_TEST_FLAGS);
#endif

	/* Configure system clock. ov7740 send image sensor data to 24 Mhz so
	 * for best performance PCK0, which will capture ov7740 data, have to
	 * work at 24Mhz.
	 * It's more easy and optimum to use one PLL for core (PLLB) and
	 * others one for PCK0 (PLLA).
	 */
	sysclk_init(); /* System clock use PLLB */
	pmc_enable_pllack(7, 0x1, 1);

	board_init();

#ifdef STATE_STAYED
	/* Configure push button */
	configure_button();
#endif

	/* Write information only for power up sequence */
	if (!(gpbr_read(GPBR0) == BACKUP_FLAG)) {
		/* LCD display initialization */
		display_init();

		/* LCD display information */
		ili9325_fill(COLOR_TURQUOISE);
		ili9325_draw_string(0, 20, (uint8_t *)"Single snapshot demo");
		ili9325_draw_string(0, 80,
				(uint8_t *)"Please wait during\ninitialization");

		/* OV7740 image sensor initialization */
		capture_init();

		#ifdef STATE_STAYED
		/* LCD display information */
		ili9325_fill(COLOR_TURQUOISE);
		ili9325_draw_string(0, 10, (uint8_t *)"Single snapshot demo");
		ili9325_draw_string(0, 30, (uint8_t *)"Step by step mode");
		ili9325_draw_string(0, 80,
				(uint8_t *)"Please press PB1\nbutton to go at the\nnext state");
		#else
		/* LCD display information */
		ili9325_fill(COLOR_TURQUOISE);
		ili9325_draw_string(0, 10, (uint8_t *)"Single snapshot demo");
		ili9325_draw_string(0, 80,
				(uint8_t *)"Please press PB1\n button to take\na picture");
		#endif

		/* Prepare to enter in backup mode*/
		low_power_prepare();

		/* Enter Backup Mode */
		enter_backup_mode();
	}

	/* Wait for a state change (in step by step mode) */
	WAIT_FOR_STATE(STATE_WAKE_UP);

	/* LCD display initialization */
	display_init();

	/* Wait for a state change (in step by step mode) */
	WAIT_FOR_STATE(STATE_LCD_INIT);

	/* Configure SMC interface for external SRAM. This SRAM will be used
	 * to store picture during image sensor acquisition.
	 */
	board_configure_sram();

	/* Wait for a state change (in step by step mode) */
	WAIT_FOR_STATE(STATE_SRAM_INIT);

	/* OV7740 image sensor initialization */
	capture_init();

	/* Wait for a state change (in step by step mode) */
	WAIT_FOR_STATE(STATE_CAPTURE_INIT);

	/* Capture a picture and send corresponding data to external memory */
	start_capture();

	/* Wait for a state change (in step by step mode) */
	WAIT_FOR_STATE(STATE_CAPTURE_DONE);

	/* Load picture data from external memory and display it on the LCD */
	display_buffer();

	/* Prepare to enter in backup mode */
	low_power_prepare();

	/* Enter Backup Mode */
	enter_backup_mode();
}
