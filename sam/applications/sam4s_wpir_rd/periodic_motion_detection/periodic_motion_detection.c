/**
 * \file
 *
 * \brief Periodic motion detection example.
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
 * \mainpage Periodic motion detection example.
 *
 * \section Purpose
 *
 * This example demonstrates how to configure the OV7740 CMOS sensor, RE200B PIR
 * sensor in order to create an application which will take a picture, store it
 * in the external SRAM and display it on the LCD when a motion is detected.
 * This application is designed to use MCU low power mode to reduce power
 * consumption.
 *
 * \section Requirements
 *
 * This package can be used with sam4s_wpir_rd board.
 *
 * \section Description
 *
 * At startup, the system clock stays in reset state (PLLA/PLLB disabled with a
 * 4Mhz core clock provided by the internal oscillator) to reduce power
 * consumption. This application first configures the LCD, displays information
 *about
 * application and initializes the RE200B PIR sensor. The application
 * prepares the MCU to enter in wait mode, configures a 200ms RTT alarm
 * which will wake it up and launch the wait mode sequence.
 * Once waked up, the MCU enables the PIR sensor, configures a 500ms RTT
 * alarm and enters in sleep mode. The MCU can be waked up from this
 * sleep mode only when a motion is detected or if the RTT alarm has timed out.
 * If the wake up source is the RTT alarm, the MCU will come back in previous
 * wait mode. However, if wake up source is a motion detection the application
 *will
 * trigger the capture and display process.
 * This process first sets the PLLB to the system core clock and PLLA to clock
 *PCK0
 * (used to get data from image sensor). Next step is to configure external
 * SRAM, which is used to store data after acquisition. Finally, it configures
 *PIO
 * capture and OV7740 image sensor. Then a picture is taken, stored in external
 * sram and displayed on LCD by loading data from external sram. Finnaly system
 * clock is set to the reset state (plla/pllb disable and core clock provide
 * by 4 Mhz internal oscillator) and the microcontroller come back in wait mode.
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

/* Uncomment this macro to work in black and white mode. */
#define DEFAULT_MODE_COLORED

/* TWI clock frequency in Hz (400KHz) */
#define TWI_CLK     400000

/* Macro to control the staying in each state permanently, comment for
 * continuous transition. */
/* #define STATE_STAYED */

/* Display information on the LCD during Step by Step mode (uncomment
 * STATE_STAYED). */
/* #define LCD_DISPLAY_INFORMATION */

/* Macro to reset test pin, stay in current state (until the user presses the
 * push button), display info on the LCD. */
/* (if LCD_DISPLAY_INFORMATION is uncomment) and finally set test pin. */
#ifdef STATE_STAYED
static void display_info(uint32_t state);
static void display_init(void);

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

/* enumeration of all state */
typedef enum {
	STATE_INIT, /* initalized state */
	STATE_START, /* start state*/
	STATE_WAIT, /* state before to enter in wait mode */
	STATE_SLEEP_ENTER, /* state before to enter in sleep mode */
	STATE_ACTIVE_MODE, /* state after exit from sleep mode */
	STATE_SRAM_CONFIGURE, /* state before sram configuration */
	STATE_CAPTURE_INIT, /* state before capture initialization */
	STATE_CAPTURE, /* state before capturation */
	STATE_DISPLAY, /* state before image displaying */
	STATE_END /* reserved */
} STATE_TYPE;

/* Pointer to the image data destination buffer */
uint8_t *g_p_uc_cap_dest_buf;

/* Row size of captured picture */
uint16_t g_us_cap_rows = IMAGE_HEIGHT;

/* Define display function and line size of captured picture according to the */
/* selected color mode. */
#ifdef DEFAULT_MODE_COLORED
	#define _display() draw_frame_yuv_color_int()

/* (IMAGE_WIDTH *2 ) because ov7740 use YUV422 format in color mode */
/* (draw_frame_yuv_color_int for more details) */
uint16_t g_us_cap_line = (IMAGE_WIDTH * 2);
#else
	#define _display() draw_frame_yuv_bw8()

uint16_t g_us_cap_line = (IMAGE_WIDTH);
#endif

/* CKGR_MOR register define which allow to enter in wait mode */
#define CKGR_MOR_WAITMODE    (0x1u << 2)

/* Vsync signal information (true if it's triggered and false otherwise) */
static volatile uint32_t g_ul_vsync_flag = false;

/* RTT alarm signal information (true if it's triggered and false otherwise) */
static volatile uint32_t g_ul_alarm_flag = false;

/* Motion detect information (true if it's triggered and false otherwise) */
static volatile uint32_t ul_motion_detected = false;

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

/**
 * \brief Handler for the RTT alarm event.
 */
void RTT_Handler( void )
{
	uint32_t ul_status;

	/* Get RTT status */
	ul_status = rtt_get_status(RTT);

	/* Alarm */
	if ((ul_status & RTT_SR_ALMS) == RTT_SR_ALMS) {
		g_ul_alarm_flag = true;
	}
}

/**
 * \brief Handler for ACC comparison event.
 */
void ACC_Handler(void)
{
	re200b_motion_detect_handler();
}

#ifdef STATE_STAYED

/**
 * \brief Handler for Button rising edge interrupt. It also changes the current
 *state.
 */
static void button_handler(uint32_t id, uint32_t mask)
{
	switch (ul_cur_state) {
	case STATE_INIT:
		ul_cur_state = STATE_START;
		break;

	case STATE_START:
		ul_cur_state = STATE_WAIT;
		break;

	case STATE_WAIT:
		ul_cur_state = STATE_SLEEP_ENTER;
		break;

	case STATE_SLEEP_ENTER:
		ul_cur_state = STATE_ACTIVE_MODE;
		break;

	case STATE_ACTIVE_MODE:
		ul_cur_state = STATE_SRAM_CONFIGURE;
		break;

	case STATE_SRAM_CONFIGURE:
		ul_cur_state = STATE_CAPTURE_INIT;
		break;

	case STATE_CAPTURE_INIT:
		ul_cur_state = STATE_CAPTURE;
		break;

	case STATE_CAPTURE:
		ul_cur_state = STATE_DISPLAY;
		break;

	case STATE_DISPLAY:
		ul_cur_state = STATE_START;
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
 *trigger.
 */
static void wait_for_state(uint32_t ul_state)
{
	while (ul_cur_state != ul_state) {
	}
}

/**
 * \brief Display information about the current state (STATE_STAYED must be
 *uncommented).
 */
static void display_info(uint32_t ul_state)
{
	static uint32_t ul_first_display = true;
	switch (ul_state) {
	case STATE_WAIT:
		if (ul_first_display) {
			ili9325_draw_string(0, 140, "Initialization done");
			ili9325_draw_string(0, 160,
					"Press BP1 to enter\nin wait mode");
			ul_first_display = false;
		} else {
			if (ul_motion_detected) {
				display_init(); /* Initialize LCD to display
				                 * information */
				ili9325_fill(COLOR_TURQUOISE);
				ili9325_draw_string(0, 10,
						"Initialization done");
				ili9325_draw_string(0, 40,
						"Press BP1 button to\nenter in wait mode");
			} else {
				ili9325_fill(COLOR_TURQUOISE);
				ili9325_draw_string(0, 10, "Out of sleep mode");
				ili9325_draw_string(0, 40,
						"No motion detected\nsoftware come back\nin start state");
				ili9325_draw_string(0, 110,
						"Press BP1 button to\nenter in wait mode");
			}
		}

		break;

	case STATE_SLEEP_ENTER:
		ili9325_fill(COLOR_TURQUOISE);
		ili9325_draw_string(0, 10, "Out of wait mode");
		ili9325_draw_string(0, 50,
				"Press BP1 button to\nenter in sleep mode");
		break;

	case STATE_ACTIVE_MODE:
		ili9325_fill(COLOR_TURQUOISE);
		ili9325_draw_string(0, 10, "Out of sleep mode");
		ili9325_draw_string(0, 40, "motion detected");
		ili9325_draw_string(0, 80,
				"Press BP1 button to\nswitch mck from\ninternal xtal to pll");
		break;

	case STATE_SRAM_CONFIGURE:
		ili9325_fill(COLOR_TURQUOISE);
		ili9325_draw_string(0, 10, "PLL configuration\ndone");
		ili9325_draw_string(0, 60,
				"Press BP1 button to\nconfigure external\nsram");
		break;

	case STATE_CAPTURE_INIT:
		ili9325_fill(COLOR_TURQUOISE);
		ili9325_draw_string(0, 10, "external sram\nconfigured");
		ili9325_draw_string(0, 60,
				"Press BP1 button to go\nat the next state");
		break;

	case STATE_CAPTURE:
		ili9325_fill(COLOR_TURQUOISE);
		ili9325_draw_string(0, 10, "Capture initialized");
		ili9325_draw_string(0, 60,
				"Press BP1 button to\ncapture a picture");
		break;

	case STATE_DISPLAY:
		ili9325_fill(COLOR_TURQUOISE);
		ili9325_draw_string(0, 10, "Capture done");
		ili9325_draw_string(0, 60,
				"Press BP1 button to\ndisplay a picture");
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
 * \brief Configure microcontroller to enter sleep mode.
 * Enter condition: WFI + (SLEEPDEEP bit = 0) + (LPM bit = 0)
 */
static void enter_sleep_mode( void )
{
	/* enable sleep mode*/
	/* PMC->PMC_FSMR &= (uint32_t)~PMC_FSMR_LPM; */

	/* clear DEEPSLEEP bit*/
	SCB->SCR &= (uint32_t) ~SCB_SCR_SLEEPDEEP_Msk;

	/* Call WFI instruction to enter in sleep mode */
	__WFI();
}

/**
 * \brief Enter Wait Mode.
 * Enter condition: (WAITMODE bit = 1) + (SLEEPDEEP bit = 0) + (LPM bit = 1)
 */
static void enter_wait_mode( void )
{
	/* Disable Brownout Detector */
	SUPC->SUPC_MR |= (uint32_t)(0xA5 << 24) | (0x01 << 13);

	/* Enable RTT alarm wakeup and low power mode */
	PMC->PMC_FSMR |= (/*PMC_FSMR_LPM |*/ PMC_FSMR_RTTAL);

	/* Clear DEEPSLEEP bit */
	SCB->SCR &= (uint32_t) ~SCB_SCR_SLEEPDEEP_Msk;

	/* Enable RTT */
	pmc_enable_periph_clk(ID_RTT);

	/* Initialize RTT peripheral */
	rtt_init(RTT, 32);

	/* Set 200 alarm */
	rtt_write_alarm_time(RTT, 200);

	/* Restart RTT */
	rtt_enable_interrupt(RTT, RTT_MR_ALMIEN);

	/* Enter wait mode */
	PMC->CKGR_MOR |= (CKGR_MOR_KEY(0x37) | CKGR_MOR_WAITMODE);
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY)) {
	}

	while (!(PMC->CKGR_MOR & CKGR_MOR_MOSCRCEN)) {
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
 * \brief Prepare to enter in wait mode.
 */
static void wait_prepare(void)
{
	/* Disable all peripheral */
	pmc_disable_all_periph_clk();

	/* Disable USB Clock */
	REG_PMC_SCDR = PMC_SCER_UDP;
}

/**
 * \brief Intialize the LCD display.
 */
static void display_init(void)
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
			| SMC_MODE_WRITE_MODE);

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

/**
 * \brief Initialize PIO capture and OV7740 component.
 */
static void capture_init(void)
{
	/* Backup setting for future use */
	static uint32_t ul_backup_setting[2] = {0};
	static uint32_t ulcapture_initialized = false;
	twi_options_t opt;

	/* Init Vsync handler */
	init_vsync_interrupts();

	/* Init PIO capture */
	pio_capture_init(OV7740_DATA_BUS_PIO, OV7740_DATA_BUS_ID);

	/* Turn on OV7740 image sensor using power pin */
	ov_power(true, OV_POWER_PIO, OV_POWER_MASK);

	/* Init PCK0 to work at 24 Mhz*/
	PMC->PMC_PCK[0] = (PMC_PCK_PRES_CLK_4 | PMC_PCK_CSS_PLLA_CLK); /*
	                                                                * 96/4=24
	                                                                * Mhz */
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

	/* Check if it is the first init */
	if (!ulcapture_initialized) {
		/* Wait 3 seconds to let the image sensor to adapt to the
		 *environment */
		delay_ms(3000);
		ov_store_manual(BOARD_TWI, ul_backup_setting, 2); /* Save
		                                                   * configuration */
		ulcapture_initialized = true;
	} else {
		/* Use the previously saved configuration from first
		 *initialization */
		ov_restore_manual(ul_backup_setting, 2);
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

	/* Enable vsync interrupt */
	pio_enable_interrupt(OV7740_VSYNC_PIO, OV7740_VSYNC_MASK);

	/* Capture acquisition will start on rising edge of Vsync signal.
	 * So wait vsync_flag = 1 before starting process
	 */
	while (!g_ul_vsync_flag) {
		/* Enable vsync interrupt */
		pio_enable_interrupt(OV7740_VSYNC_PIO, OV7740_VSYNC_MASK);

		/* Enter in sleep to reduce consumption. The vsync interrupt
		 *will wake up the core */
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
	 *will wake up the core. */
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
	 *format. The output
	 * of this conversion is two 3 bytes pixels in (B,G,R) format. First one
	 *is calculed using
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
	board_init();

	/* LCD display initialization */
	display_init();
#ifdef STATE_STAYED
	/* Configure the push button*/
	configure_button();

	/* Configure test pin */
	pmc_enable_periph_clk(PIN_TEST_ID);
	pio_configure_pin(PIN_TEST_GPIO, PIN_TEST_FLAGS);

	/* LCD display information */
	ili9325_fill(COLOR_TURQUOISE);
	ili9325_draw_string(0, 10, (uint8_t *)"Periodic motion\ndetect demo");
	ili9325_draw_string(0, 50, (uint8_t *)"Step by Step mode");
#else
	ili9325_fill(COLOR_TURQUOISE);
	ili9325_draw_string(0, 10, (uint8_t *)"Periodic motion\ndetect demo");
	ili9325_draw_string(0, 80,
			(uint8_t *)"please wait during\ninitialization");
#endif
	/* Initialize PIR sensor */
	re200b_motion_detect_init();

	while (1) {
		ul_cur_state = STATE_START;

		/* Wait for a state change (in step by step mode) */
		WAIT_FOR_STATE(STATE_WAIT);

		/* Prepare to enter in wait mode */
		wait_prepare();

		/* Enter wait mode */
		enter_wait_mode();

		/* Enable ACC, RTT and SMC peripheral */
		pmc_enable_periph_clk( ID_ACC );
		pmc_enable_periph_clk( ID_RTT );
		pmc_enable_periph_clk( ID_SMC );

#ifdef STATE_STAYED
		/* Enable test pin and push button PIO ID*/
		pmc_enable_periph_clk(PIN_TEST_ID);
		pmc_enable_periph_clk(PUSH_BUTTON_ID);
#endif
		/* Wait for a state change (in step by step mode) */
		WAIT_FOR_STATE(STATE_SLEEP_ENTER);

		/* Reset RTT and initialize an 500ms RTT alarm */
		rtt_init(RTT, 32);
		rtt_write_alarm_time(RTT, 500);
		rtt_enable_interrupt(RTT, RTT_MR_ALMIEN);
		NVIC_EnableIRQ(RTT_IRQn);

		/* Reset motion detection */
		re200b_motion_detect_enable();
		re200b_motion_detect_reset();
		acc_get_interrupt_status(ACC);

		/* Enter sleep mode */
		enter_sleep_mode();

		ul_motion_detected = re200b_motion_detection();
		if ((ul_motion_detected) || (g_ul_alarm_flag)) {
			/* Disable RTT and ACC interrupt */
			re200b_motion_detect_disable();
			g_ul_alarm_flag = false;

			if (ul_motion_detected == 1) {
				/* Wait for a state change (in step by step
				 *mode) */
				WAIT_FOR_STATE(STATE_ACTIVE_MODE);

				/* To reduce power consumption system clock stay
				 * in reset state.
				 * In this state PLLA/PLLB are disabled and the
				 *system clock is provided
				 * by the 4Mhz internal oscillator. However
				 *during image capture and display
				 * it is necessary to work faster, so system
				 *clock will be configured
				 * to use PLLB at max frequency with external
				 *crystal as clock source.
				 */
				sysclk_init(); /* System clock use PLLB */

				/* OV7740 send image sensor data at 24 Mhz. For
				 * best performances, PCK0
				 * which will capture OV7740 data, has to work
				 *at 24Mhz. It's easier
				 * and optimum to use one PLL for core (PLLB)
				 *and one other for PCK0
				 * (PLLA).
				 */
				pmc_enable_pllack(7, 0x1, 1);

				/* Wait a state change (in step by step mode) */
				WAIT_FOR_STATE(STATE_SRAM_CONFIGURE);

				/* Configure SMC interface for external SRAM.
				 * This SRAM will be used
				 * to store picture during image sensor
				 *acquisition.
				 */
				board_configure_sram();

				/* wait for a state change (in step by step
				 *mode) */
				WAIT_FOR_STATE(STATE_CAPTURE_INIT);

				/* OV7740 image sensor initialization */
				capture_init();

				/* wait for a state change (in step by step
				 *mode) */
				WAIT_FOR_STATE(STATE_CAPTURE);

				/* Capture a picture and send corresponding data
				 *to the external memory */
				start_capture();

				/* wait for a state change (in step by step
				 *mode) */
				WAIT_FOR_STATE(STATE_DISPLAY);

				/* Load picture data from external memory and
				 *display it on LCD*/
				_display();

				/* wait for a state change (in step by step
				 *mode) */
				WAIT_FOR_STATE(STATE_START);

				/* Image capture and display are done, so system
				 * clock can restored in the
				 * reset state (PLLA/PLLB disabled).
				 */
				pmc_switch_mck_to_mainck(SYSCLK_PRES_1);
				pmc_switch_mainck_to_fastrc(
						CKGR_MOR_MOSCRCF_4_MHz);
				EFC0->EEFC_FMR = EEFC_FMR_FWS(0);
				pmc_disable_pllack();
				pmc_disable_pllbck();
			}
		}
	}
}
