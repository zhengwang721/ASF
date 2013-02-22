/**
 * \file
 *
 * \brief Starter Kit Demo.
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
 * \mainpage Starter Kit Demo
 *
 * \section Purpose
 *
 * The Starter Kit Demo will help new users get familiar with Atmel's
 * SAM family of microcontrollers. This demo features the IO1 and OLED1
 * extension boards for the SAM4 Xplained Pro.
 *
 * \section Requirements
 *
 * This package can be used with SAM Xplained Pro evaluation kits.
 *
 * \section Description
 *
 * The demonstration program can operate in 3 different modes; temperature
 * information, light sensor information and SD card status. Each of these
 * being selectable using Button1 Button2 and Button3 from the OLED1
 * extension.
 * IO1 extension must be connected on EXT2.
 * OLED1 extension must be connected on EXT3.
 *
 */

#include "board.h"
#include "sysclk.h"
#include "ssd1306.h"
#include "at30tse75x.h"
#include "twi_master.h"
#include "adc.h"
#include "ioport.h"
#include "pio_handler.h"
#include "sd_mmc.h"
#include "sd_mmc_spi.h"

/* These settings will force to set and refresh the temperature mode. */
volatile uint32_t app_mode = 0;
volatile uint32_t app_mode_switch = 1;

volatile uint32_t sd_status_update = 0;

/**
 * \brief Process Buttons Events.
 *
 * \param uc_button The button number.
 */
static void ProcessButtonEvt(uint8_t uc_button)
{
	/* Switch to temperature mode. */
	if ((uc_button == 1) && (app_mode != 0))
	{
		app_mode_switch = 1;
	}
	/* Switch to light mode. */
	else if ((uc_button == 2) && (app_mode != 1))
	{
		app_mode_switch = 2;
	}
	/* Switch to SD mode. */
	else if ((uc_button == 3) && (app_mode != 2))
	{
		app_mode_switch = 3;
	}
}

/**
 * \brief Handler for Button 1 rising edge interrupt.
 * \param id The button ID.
 * \param mask The button mask.
 */
static void Button1_Handler(uint32_t id, uint32_t mask)
{
	if ((PIN_PUSHBUTTON_1_ID == id) && (PIN_PUSHBUTTON_1_MASK == mask))
		ProcessButtonEvt(1);
}

/**
 * \brief Handler for Button 2 rising edge interrupt.
 * \param id The button ID.
 * \param mask The button mask.
 */
static void Button2_Handler(uint32_t id, uint32_t mask)
{
	if ((PIN_PUSHBUTTON_2_ID == id) && (PIN_PUSHBUTTON_2_MASK == mask))
		ProcessButtonEvt(2);
}

/**
 * \brief Handler for Button 3 rising edge interrupt.
 * \param id The button ID.
 * \param mask The button mask.
 */
static void Button3_Handler(uint32_t id, uint32_t mask)
{
	if ((PIN_PUSHBUTTON_3_ID == id) && (PIN_PUSHBUTTON_3_MASK == mask))
		ProcessButtonEvt(3);
}

/**
 * \brief Handler for SD card detect rising edge interrupt.
 * \param id The button ID.
 * \param mask The button mask.
 */
static void SD_Detect_Handler(uint32_t id, uint32_t mask)
{
	if ((SD_MMC_0_CD_ID == id) && (SD_MMC_0_CD_MASK == mask))
		sd_status_update = 1;
}

/* IRQ priority for PIO (The lower the value, the greater the priority) */
#define IRQ_PRIOR_PIO    0

/**
 * \brief Configure the Pushbuttons.
 *
 * Configure the PIO as inputs and generate corresponding interrupt when
 * pressed or released.
 */
static void configure_buttons(void)
{
	/* Configure Pushbutton 1. */
	pmc_enable_periph_clk(PIN_PUSHBUTTON_1_ID);
	pio_set_debounce_filter(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_ID,
			PIN_PUSHBUTTON_1_MASK, PIN_PUSHBUTTON_1_ATTR, Button1_Handler);
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_1_ID);
	pio_handler_set_priority(PIN_PUSHBUTTON_1_PIO, (IRQn_Type) PIN_PUSHBUTTON_1_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK);

	/* Configure Pushbutton 2. */
	pmc_enable_periph_clk(PIN_PUSHBUTTON_2_ID);
	pio_set_debounce_filter(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_ID,
			PIN_PUSHBUTTON_2_MASK, PIN_PUSHBUTTON_2_ATTR, Button2_Handler);
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_2_ID);
	pio_handler_set_priority(PIN_PUSHBUTTON_2_PIO, (IRQn_Type) PIN_PUSHBUTTON_2_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_MASK);

	/* Configure Pushbutton 3. */
	pmc_enable_periph_clk(PIN_PUSHBUTTON_3_ID);
	pio_set_debounce_filter(PIN_PUSHBUTTON_3_PIO, PIN_PUSHBUTTON_3_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_3_PIO, PIN_PUSHBUTTON_3_ID,
			PIN_PUSHBUTTON_3_MASK, PIN_PUSHBUTTON_3_ATTR, Button3_Handler);
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_3_ID);
	pio_handler_set_priority(PIN_PUSHBUTTON_3_PIO, (IRQn_Type) PIN_PUSHBUTTON_3_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_PUSHBUTTON_3_PIO, PIN_PUSHBUTTON_3_MASK);

	/* Configure SD card detection. */
	pmc_enable_periph_clk(SD_MMC_0_CD_ID);
	pio_set_debounce_filter(SD_MMC_0_CD_PIO, SD_MMC_0_CD_MASK, 10);
	pio_handler_set(SD_MMC_0_CD_PIO, SD_MMC_0_CD_ID, SD_MMC_0_CD_MASK,
			SD_MMC_0_CD_ATTR, SD_Detect_Handler);
	NVIC_EnableIRQ((IRQn_Type) SD_MMC_0_CD_ID);
	pio_handler_set_priority(SD_MMC_0_CD_PIO, (IRQn_Type) SD_MMC_0_CD_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SD_MMC_0_CD_PIO, SD_MMC_0_CD_MASK);
}

/**
 * \brief Configure the ADC for the light sensor.
 */
static void configure_adc(void)
{
	/* Configure ADC pin for light sensor. */
	gpio_configure_pin(LIGHT_SENSOR_GPIO, LIGHT_SENSOR_FLAGS);

	/* Enable ADC clock. */
	pmc_enable_periph_clk(ID_ADC);

	/* Configure ADC. */
	adc_init(ADC, sysclk_get_cpu_hz(), 1000000, ADC_MR_STARTUP_SUT0);
	adc_enable_channel(ADC, ADC_CHANNEL_4);
	adc_configure_trigger(ADC, ADC_TRIG_SW, 1);
}

/**
 * \brief Show SD card status on the OLED screen.
 */
static void display_sd_info(void)
{
	uint8_t card_check;
	uint8_t sd_card_type;
	uint8_t sd_card_version;
	uint32_t sd_card_size;
	uint8_t size[10];

	// Is SD card present?
	if (gpio_pin_is_low(SD_MMC_0_CD_GPIO) == false)
	{
		ssd1306_write_text("Please insert SD card...");
	}
	else
	{
		ssd1306_write_text("SD card information:");

		sd_mmc_init();
		card_check = sd_mmc_check(0);
		while (card_check != SD_MMC_OK)
		{
			card_check = sd_mmc_check(0);
			delay_ms(1);
		}

		if (card_check == SD_MMC_OK)
		{
			sd_card_type = sd_mmc_get_type(0);
			sd_card_version = sd_mmc_get_version(0);
			sd_card_size = sd_mmc_get_capacity(0);

			ssd1306_set_page_address(1);
			ssd1306_set_column_address(0);

			// Card type
			switch(sd_card_type)
			{
				case CARD_TYPE_SD:
				ssd1306_write_text("- Type: Normal SD card");
				break;
				case CARD_TYPE_SDIO:
				ssd1306_write_text("- Type: SDIO card");
				break;
				case CARD_TYPE_HC:
				ssd1306_write_text("- Type: High Capacity card");
				break;
				case CARD_TYPE_SD_COMBO:
				ssd1306_write_text("- Type: SDIO/Memory card");
				break;
				default:
				ssd1306_write_text("- Type: unknown");
			}

			ssd1306_set_page_address(2);
			ssd1306_set_column_address(0);

			// SD card version
			switch(sd_card_version)
			{
				case CARD_VER_SD_1_0:
				ssd1306_write_text("- Version: 1.0x");
				break;
				case CARD_VER_SD_1_10:
				ssd1306_write_text("- Version: 1.10");
				break;
				case CARD_VER_SD_2_0:
				ssd1306_write_text("- Version: 2.00");
				break;
				case CARD_VER_SD_3_0:
				ssd1306_write_text("- Version: 3.0x");
				break;
				default:
				ssd1306_write_text("- Version: unknown");
			}

			ssd1306_set_page_address(3);
			ssd1306_set_column_address(0);

			sprintf(size, "- Total size: %lu KB", sd_card_size);
			ssd1306_write_text(size);
		}
	}
}

/**
 * \brief Draw graph on the OLED screen using the provided point array.
 * \param col X coordinate.
 * \param page Y coordinate (please refer to OLED datasheet for page description).
 * \param width Graph width.
 * \param height Graph height.
 * \param tab Data to draw. Must contain width elements.
 */
static void ssd1306_draw_graph(uint8_t col, uint8_t page, uint8_t width, uint8_t height, uint8_t *tab)
{
	volatile register uint8_t page_start;
	volatile register uint8_t i, j, k, s;
	uint8_t scale;

	for (i = 0; i < width; ++i) {
		for (page_start = page; page_start <= height; ++page_start) {
			ssd1306_write_command(SSD1306_CMD_SET_PAGE_START_ADDRESS(page_start));
			ssd1306_set_column_address(i);
			j = tab[i];
			scale = 8 * (height - page_start + 1);
			if (j > scale)
				j = 8;
			else
				j -= (scale - 8);

			for (k = 0, s = j; j > 0; --j)
				k = (k << 1) + 1;
			for (s = 8 - s; s > 0; --s)
				k <<= 1;
			ssd1306_write_data(k);
		}
	}
}

#define BUFFER_SIZE 128

int main(void)
{
	uint8_t i;
	uint8_t temperature[BUFFER_SIZE];
	uint8_t light[BUFFER_SIZE];
	uint8_t value_disp[5];
	uint32_t adc_value;
	double temp;

	// Initialize clocks.
	sysclk_init();

	// Initialize GPIO states.
	board_init();

	// Configure ADC for light sensor.
	configure_adc();

	// Initialize at30tse.
	at30tse_init();

	// Configure IO1 buttons.
	configure_buttons();

	// Initialize SPI and SSD1306 controller.
	ssd1306_init();
	ssd1306_clear();

	// Clear internal buffers.
	for (i = 0; i < BUFFER_SIZE; ++i)
	{
		temperature[i] = 0;
		light[i] = 0;
	}

	while (true)
	{
		/* Refresh page title only if necessary. */
		if (app_mode_switch > 0)
		{
			app_mode = app_mode_switch - 1;

			// Clear screen.
			ssd1306_clear();
			ssd1306_set_page_address(0);
			ssd1306_set_column_address(0);

			/* Temperature mode. */
			if (app_mode == 0)
			{
				ioport_set_pin_level(IO1_LED1_PIN, IO1_LED1_ACTIVE);
				ioport_set_pin_level(IO1_LED2_PIN, !IO1_LED2_ACTIVE);
				ioport_set_pin_level(IO1_LED3_PIN, !IO1_LED3_ACTIVE);
				ssd1306_write_text("Temperature sensor:");
			}
			/* Light mode. */
			else if (app_mode == 1)
			{
				ioport_set_pin_level(IO1_LED2_PIN, IO1_LED2_ACTIVE);
				ioport_set_pin_level(IO1_LED1_PIN, !IO1_LED1_ACTIVE);
				ioport_set_pin_level(IO1_LED3_PIN, !IO1_LED3_ACTIVE);
				ssd1306_write_text("Light sensor:");
			}
			/* SD mode. */
			else
			{
				ioport_set_pin_level(IO1_LED3_PIN, IO1_LED3_ACTIVE);
				ioport_set_pin_level(IO1_LED1_PIN, !IO1_LED1_ACTIVE);
				ioport_set_pin_level(IO1_LED2_PIN, !IO1_LED2_ACTIVE);

				display_sd_info();
			}
			app_mode_switch = 0;
		}

		// Shift graph buffers.
		for (i = 0; i < BUFFER_SIZE - 1; ++i)
		{
			temperature[i] = temperature[i + 1];
			light[i] = light[i + 1];
		}

		// Get temperature in a range from 0 to 40 degrees.
		if (at30tse_read_temperature(&temp) == TWI_SUCCESS)
		{
			// Don't care about negative temperature.
			if (temp < 0)
				temp = 0;

			// Update temperature for display.
			// Note: -12 in order to rescale for better rendering.
			if (temp < 12)
				temperature[BUFFER_SIZE - 1] = 0;
			else
				temperature[BUFFER_SIZE - 1] = temp - 12;
		}
		else
		{
			// Error print zero values.
			temperature[BUFFER_SIZE - 1] = 0;
		}

		// Get light sensor information.
		// Rescale for better rendering.
		adc_start(ADC);
		adc_value = adc_get_channel_value(ADC, ADC_CHANNEL_4);
		light[BUFFER_SIZE - 1] = 24 - adc_value * 24 / 4096;

		// Print temperature in text format.
		if (app_mode == 0)
		{
			sprintf(value_disp, "%d", (uint8_t)temp);
			ssd1306_set_column_address(95);
			ssd1306_write_command(SSD1306_CMD_SET_PAGE_START_ADDRESS(0));
			ssd1306_write_text(" ");
			ssd1306_write_text(value_disp);
			// Display degree symbol.
			ssd1306_write_data(0x06);
			ssd1306_write_data(0x06);
			ssd1306_write_text("c");

			// Refresh graph.
			ssd1306_draw_graph(0, 1, BUFFER_SIZE, 3, temperature);
		}
		else if (app_mode == 1)
		{
			sprintf(value_disp, "%lu", 100 - (adc_value * 100 / 4096));
			ssd1306_set_column_address(98);
			ssd1306_write_command(SSD1306_CMD_SET_PAGE_START_ADDRESS(0));
			ssd1306_write_text(" ");
			ssd1306_write_text(value_disp);
			ssd1306_write_text("%");

			// Refresh graph.
			ssd1306_draw_graph(0, 1, BUFFER_SIZE, 3, light);
		}
		else
		{
			// Is card has been inserted or removed?
			if (sd_status_update == 1)
			{
				// Clear screen.
				ssd1306_clear();
				ssd1306_set_page_address(0);
				ssd1306_set_column_address(0);

				// Show SD card info.
				display_sd_info();

				sd_status_update = 0;
			}

		}

		/* Wait and stop screen flickers. */
		delay_ms(50);
	}
}
