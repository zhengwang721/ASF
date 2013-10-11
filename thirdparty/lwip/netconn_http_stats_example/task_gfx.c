/**
 * \file
 *
 * \brief GFX task from the HTTP Netconn Example.
 *
 * Copyright (c) 2011-2012 Atmel Corporation. All rights reserved.
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

#include "task_defs.h"
#include "arm_math.h"
#include "logo_atmel.h"

#include <FreeRTOS.h>
#include <FreeRTOS_CLI.h>
#include <StackMacros.h>
#include <croutine.h>
#include <list.h>
#include <mpu_wrappers.h>
#include <portable.h>
#include <projdefs.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>
#include <timers.h>

#include <pmc.h>
#include <sysclk.h>

#include "ili9325.h"
#include "aat31xx.h"
#include "gpio.h"
#include "pio_handler.h"
#include "smc.h"
#include "ethernet.h"
#include "rtouch.h"

static void gfx_task(void *pvParameters);
static void gfx_init(void);
static void gfx_draw_bmpfile(const uint8_t *bmpImage);

/* Chip select number to be set */
#define ILI9325_LCD_CS       1

/** IRQ priority for PIO (The lower the value, the greater the priority) */
#define IRQ_PRIOR_PIO        13

const portTickType instructions_delay = 50UL / portTICK_RATE_MS;
static const portTickType presentation_delay = 2000UL / portTICK_RATE_MS;
static const portTickType touch_delay = 10UL / portTICK_RATE_MS;
static const portTickType gfx_refresh_rate = 1000UL / portTICK_RATE_MS;

/* Hold the instructions screen and prevent other task from running. */
uint32_t app_hold = 1;

uint32_t g_ip_mode = 0; // 0 unselected, 1 static, 2 dhcp.

int8_t g_c_ipconfig[25];

/* BMP (Windows) Header Format. */
COMPILER_PACK_SET(1)
struct bmpfile_header {
	uint16_t type;		/* signature, must be 4D42 hex */
	uint32_t file_size; /* size of BMP file in bytes (unreliable) */
	uint16_t reserved1; /* reserved, must be zero */
	uint16_t reserved2; /* reserved, must be zero */
	uint32_t offset;	/* offset to start of image data in bytes */
	uint32_t header_size; /* size of BITMAPINFOHEADER structure, must be 40 */
	uint32_t width;		/* image width in pixels */
	uint32_t height;	/* image height in pixels */
	uint16_t planes;	/* number of planes in the image, must be 1 */
	uint16_t bits;		/* number of bits per pixel (1, 4, 8, 16, 24, 32) */
	uint32_t compression; /* compression type (0=none, 1=RLE-8, 2=RLE-4) */
	uint32_t inage_size; /* size of image data in bytes (including padding) */
	uint32_t h_resolution;	/* horizontal resolution in pixels per meter */
							/* (unreliable) */
	uint32_t v_resolution;	/* vertical resolution in pixels per meter */
							/* (unreliable) */
	uint32_t colours;		/* number of colors in image, or zero */
	uint32_t important_colors;	/* number of important colors, or zero */
};
COMPILER_PACK_RESET()

/**
 * \brief Callback called when the pen is pressed on the touchscreen.
 *
 * \param x horizontal position (in pixel if ts calibrated).
 * \param y vertical position (in pixel if ts calibrated).
 */
static void event_handler(rtouch_event_t const *event)
{
	switch(event->type) {
	case RTOUCH_MOVE:
	case RTOUCH_PRESS:
	case RTOUCH_RELEASE:
		if (event->panel.y < 160)
		{
			/* Static IP button selected. */
			/* Button 1. */
			ili9325_set_foreground_color(COLOR_BLACK);
			ili9325_draw_rectangle(20,70,220,130);
			ili9325_set_foreground_color(0xE4E4E4u);
			ili9325_draw_rectangle(22,72,222,132);
			ili9325_set_foreground_color(COLOR_LIGHTGREY);
			ili9325_draw_filled_rectangle(21,71,221,131);
			ili9325_set_foreground_color(COLOR_BLACK);
			ili9325_draw_string(92, 91, (uint8_t *)"Static");

			/* Button 2. */
			ili9325_set_foreground_color(0xE4E4E4u);
			ili9325_draw_rectangle(20,170,220,230);
			ili9325_set_foreground_color(COLOR_BLACK);
			ili9325_draw_rectangle(22,172,222,232);
			ili9325_set_foreground_color(COLOR_LIGHTGREY);
			ili9325_draw_filled_rectangle(21,171,221,231);
			ili9325_set_foreground_color(COLOR_BLACK);
			ili9325_draw_string(102, 191, (uint8_t *)"DHCP");
		}
		else
		{
			/* DHCP IP button selected. */
			/* Button 1. */
			ili9325_set_foreground_color(0xE4E4E4u);
			ili9325_draw_rectangle(20,70,220,130);
			ili9325_set_foreground_color(COLOR_BLACK);
			ili9325_draw_rectangle(22,72,222,132);
			ili9325_set_foreground_color(COLOR_LIGHTGREY);
			ili9325_draw_filled_rectangle(21,71,221,131);
			ili9325_set_foreground_color(COLOR_BLACK);
			ili9325_draw_string(92, 91, (uint8_t *)"Static");

			/* Button 2. */
			ili9325_set_foreground_color(COLOR_BLACK);
			ili9325_draw_rectangle(20,170,220,230);
			ili9325_set_foreground_color(0xE4E4E4u);
			ili9325_draw_rectangle(22,172,222,232);
			ili9325_set_foreground_color(COLOR_LIGHTGREY);
			ili9325_draw_filled_rectangle(21,171,221,231);
			ili9325_set_foreground_color(COLOR_BLACK);
			ili9325_draw_string(102, 191, (uint8_t *)"DHCP");
		}
		break;
	default:
		break;
	}

	/* If released, freeze buttons and perform IP settings. */
	if (event->type == RTOUCH_RELEASE)
	{
		ili9325_set_foreground_color(COLOR_GRAY);
		ili9325_draw_string(92, 91, (uint8_t *)"Static");
		ili9325_draw_string(102, 191, (uint8_t *)"DHCP");
		ili9325_set_foreground_color(COLOR_BLACK);
		if (event->panel.y < 160)
			g_ip_mode = 1;
		else
			g_ip_mode = 2;
	}
}

/**
 * \brief Create the GFX task.
 *
 * \param stack_depth_words Task stack size in 32 bits word.
 * \param task_priority Task priority.
 */
void create_gfx_task(uint16_t stack_depth_words,
		unsigned portBASE_TYPE task_priority)
{
	/* Initialize display. */
	gfx_init();

	/* Create the task as described above. */
	xTaskCreate(gfx_task, (const signed char *const) "GFX",
			stack_depth_words, NULL, task_priority,
			NULL);
}

/**
 * \brief GFX task core function.
 *
 * \param pvParameters Junk parameter.
 */
static void gfx_task(void *pvParameters)
{
	uint32_t blink = 0;

	/* Get rid of this compiler warning. */
	pvParameters = pvParameters;

	/* Display ATMEL logo. */
	ili9325_set_cursor_position(0,0);
	gfx_draw_bmpfile(logo_atmel_bmp);
	vTaskDelay(presentation_delay);

	/* Initialize touchscreen without calibration */
	pmc_enable_periph_clk(ID_PIOA);
	pio_handler_set_priority(PIOA, PIOA_IRQn, IRQ_PRIOR_PIO);
	pmc_enable_periph_clk(ID_SPI);
	rtouch_init(ILI9325_LCD_WIDTH, ILI9325_LCD_HEIGHT);
	rtouch_enable();
	rtouch_set_event_handler(event_handler);
	/*
	 * Set touch screen calibration parameters which are got
	 * from manual calibration when developing.
	 */
	rtouch_set_calibration_parameter(7432, -5637, 318, 1845);

	/* Draw IP config menu. */
	ili9325_set_foreground_color(COLOR_WHITE);
	ili9325_draw_filled_rectangle(0, 0, ILI9325_LCD_WIDTH, ILI9325_LCD_HEIGHT);

	/* Button 1. */
	ili9325_set_foreground_color(0xE4E4E4u);
	ili9325_draw_rectangle(20,70,220,130);
	ili9325_set_foreground_color(COLOR_BLACK);
	ili9325_draw_rectangle(22,72,222,132);
	ili9325_set_foreground_color(COLOR_LIGHTGREY);
	ili9325_draw_filled_rectangle(21,71,221,131);
	ili9325_set_foreground_color(COLOR_BLACK);
	ili9325_draw_string(92, 91, (uint8_t *)"Static");

	/* Button 2. */
	ili9325_set_foreground_color(0xE4E4E4u);
	ili9325_draw_rectangle(20,170,220,230);
	ili9325_set_foreground_color(COLOR_BLACK);
	ili9325_draw_rectangle(22,172,222,232);
	ili9325_set_foreground_color(COLOR_LIGHTGREY);
	ili9325_draw_filled_rectangle(21,171,221,231);
	ili9325_set_foreground_color(COLOR_BLACK);
	ili9325_draw_string(102, 191, (uint8_t *)"DHCP");
	ili9325_draw_string(22, 30, (uint8_t *)"IP Configuration");
	ili9325_draw_string(20, 260, (uint8_t *)"Assigned IP:");
	ili9325_draw_rectangle(20,280,220,310);

	while (g_ip_mode == 0) {
		rtouch_process();
		vTaskDelay(touch_delay);
	}

	/* Bring up the ethernet interface & initializes timer0, channel0 */
	ili9325_draw_string(30, 290, (uint8_t *)"Please wait...");
	init_ethernet();
	app_hold = 0;

	/* Wait for IP to be configured. Done in LwIP. */
	while (g_ip_mode != 3) {
		vTaskDelay(touch_delay);
	}

	/* Show configured IP and unlock all other waiting tasks. */
	ili9325_set_foreground_color(COLOR_WHITE);
	ili9325_draw_filled_rectangle(20,280,220,310);
	ili9325_draw_filled_rectangle(0,0,240,60);
	ili9325_set_foreground_color(COLOR_BLACK);
	ili9325_draw_rectangle(20,280,220,310);
	ili9325_draw_string(30, 290, (uint8_t const*)g_c_ipconfig);

	/* GFX task Loop. */
	while (1)
	{
		/* Make HTTP text to blink to show GFX task activity. */
		if (blink == 0)
		{
			ili9325_draw_string(5, 30, (uint8_t *)"HTTP server running!");
			blink = 1;
		}
		else
		{
			ili9325_set_foreground_color(COLOR_WHITE);
			ili9325_draw_filled_rectangle(0,0,240,60);
			ili9325_set_foreground_color(COLOR_BLACK);
			blink = 0;
		}

		/* Sleep to reach the expected blink rate. */
		/* Leave some CPU time for other tasks. */
		vTaskDelay(gfx_refresh_rate);
	}
}

/**
 * \brief Initialize the LCD.
 */
static void gfx_init(void)
{
	struct ili9325_opt_t g_ili9325_display_opt;

	/* Enable peripheral clock */
	pmc_enable_periph_clk(ID_SMC);

	/* Configure SMC interface for Lcd */
	smc_set_setup_timing(SMC,ILI9325_LCD_CS,SMC_SETUP_NWE_SETUP(2)
	| SMC_SETUP_NCS_WR_SETUP(2)
	| SMC_SETUP_NRD_SETUP(2)
	| SMC_SETUP_NCS_RD_SETUP(2));
	smc_set_pulse_timing(SMC, ILI9325_LCD_CS , SMC_PULSE_NWE_PULSE(4)
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
	g_ili9325_display_opt.foreground_color = COLOR_WHITE;
	g_ili9325_display_opt.background_color = COLOR_WHITE;

	/* Initialize LCD */
	if (1 == ili9325_init(&g_ili9325_display_opt)) {
		while(1);
	}

	/* Set backlight level */
	aat31xx_set_backlight(AAT31XX_MAX_BACKLIGHT_LEVEL);

	/* Turn on LCD */
	ili9325_display_on();
}

/**
 * \brief Draw a bitmap image.
 * Expected file format is 320x240, 24 bits, RGB888.
 *
 * \param bmpImage Pointer to the bmp file image.
 */
static void gfx_draw_bmpfile(const uint8_t *bmpImage)
{
	struct bmpfile_header *bmp_header;
	uint32_t length;
	uint32_t i = 0;
	uint32_t offset;

	bmp_header = (struct bmpfile_header*) bmpImage;
	length = bmp_header->height * bmp_header->width * 3;
	offset = sizeof(struct bmpfile_header);

	ili9325_set_cursor_position(0,0);

	/* Prepare to write in GRAM */
	LCD_IR(0);
	LCD_IR(ILI9325_GRAM_DATA_REG);

	for (i = offset; i < length; i += 3) {
	  	/* Invert red and blue. */
		LCD_WD(bmpImage[i+2]);
		LCD_WD(bmpImage[i+1]);
		LCD_WD(bmpImage[i]);
	}
}
