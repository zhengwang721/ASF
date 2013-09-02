/**
 * \file
 *
 * \brief Real-Time Clock (RTC) tamper example for SAM.
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
 * \mainpage RTC Tamper Example
 *
 * \section Purpose
 *
 * This basic example shows how to use the Real-Time Clock (RTC) tamper
 * function in SAM.
 *
 * \section Requirements
 *
 * This example can be used with SAM4C evaluation kits.
 *
 * \section Description
 *
 * Upon startup, the system set the tamper pin TMP0 and TMP2 as wake-up
 * source. After press any key, the system will enter backup mode. By pressing the
 * BP3(TMP0) or BP6(TMP2) button, the system will wake up and clear GPBR0 ~7
 * register automatically, display the tamper event happen time/date.
 *
 * \note Because this example need use RTCOUT pin, which is common PIO with TDO,
 * To debug normally, please select SWD interface.
 *
 * \section Usage
 *
 * -# Build the program and download it into the evaluation board.
 * -# On the computer, open and configure a terminal application
 *    (e.g., HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *    \code
 *     -- RTC Tamper Example xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *    \endcode
 */

#include "asf.h"
#include "conf_example.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/** Flash wait state number. */
#define FLASH_WAIT_STATE_NBR         6

/** Backup mode flag. */
#define BACKUP_MODE_FLAG             0xAA55AA55

/** Normal mode flag. */
#define NORMAL_MODE_FLAG             0xffffffff

/** GPBR const written data */
#define GPBR_CONST_DATA              (0xdeadbeef)

/** The initial RTC time */
#define RTC_TIME_HOUR                13
#define RTC_TIME_MIN                 30
#define RTC_TIME_SEC                 0

/** The RTC output config */
#define RTC_OUT_CHN                  0
#define RTC_OUT_SRC                  3

#define STRING_EOL    "\r"
#define STRING_HEADER "-- RTC Tamper Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/**
 *  Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief Application entry point for RTC tamper example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint32_t ul_read_value[8] = {0, 0 ,0, 0, 0, 0, 0, 0};
	uint32_t ul_last_page_addr = LAST_PAGE_ADDRESS;
	uint32_t *ul_back_mode_flag_addr = (uint32_t *) ul_last_page_addr;
	uint32_t ul_normal_mode_flag = NORMAL_MODE_FLAG;
	uint32_t ul_backup_mode_flag = BACKUP_MODE_FLAG;
	uint32_t ul_hour, ul_minute, ul_second;
	uint32_t ul_year, ul_month, ul_day, ul_week;
	uint32_t tmp_src;
	uint8_t uc_key;

	/* Initialize the SAM system */
	sysclk_init();
	board_init();

	/* Initialize the console uart */
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);

	/* Default RTC configuration, 24-hour mode */
	rtc_set_hour_mode(RTC, 0);
	rtc_set_time(RTC, RTC_TIME_HOUR, RTC_TIME_MIN, RTC_TIME_SEC);
	rtc_set_waveform(RTC, RTC_OUT_CHN, RTC_OUT_SRC);

	/* Initialize flash: 6 wait states for flash writing. */
	flash_init(FLASH_ACCESS_MODE_128, FLASH_WAIT_STATE_NBR);

	/* Unlock flash page. */
	flash_unlock(ul_last_page_addr,
			ul_last_page_addr + IFLASH_PAGE_SIZE - 1, NULL, NULL);

	if ((*ul_back_mode_flag_addr) == BACKUP_MODE_FLAG) {
		/* Read the data from GPBR0 ~ 7 */
		ul_read_value[0] = gpbr_read(GPBR0);
		ul_read_value[1] = gpbr_read(GPBR1);
		ul_read_value[2] = gpbr_read(GPBR2);
		ul_read_value[3] = gpbr_read(GPBR3);
		ul_read_value[4] = gpbr_read(GPBR4);
		ul_read_value[5] = gpbr_read(GPBR5);
		ul_read_value[6] = gpbr_read(GPBR6);
		ul_read_value[7] = gpbr_read(GPBR7);

		if((ul_read_value[0] != GPBR_CONST_DATA) &&
				(ul_read_value[1] != GPBR_CONST_DATA) &&
				(ul_read_value[2] != GPBR_CONST_DATA) &&
				(ul_read_value[3] != GPBR_CONST_DATA) &&
				(ul_read_value[4] != GPBR_CONST_DATA) &&
				(ul_read_value[5] != GPBR_CONST_DATA) &&
				(ul_read_value[6] != GPBR_CONST_DATA) &&
				(ul_read_value[7] != GPBR_CONST_DATA)) {
			printf("The backup register is cleared when tamper event happen!\r\n");
		}

		/* Retrieve tamper date and time */
		rtc_get_tamper_time(RTC, &ul_hour, &ul_minute, &ul_second, 0);
		rtc_get_tamper_date(RTC, &ul_year, &ul_month, &ul_day, &ul_week, 0);
		tmp_src = rtc_get_tamper_source(RTC, 0);
		printf("The tamper event TMP%u happen in %02u:%02u:%02u, %02u/%02u/%04u",
				tmp_src, ul_hour, ul_minute, ul_second, ul_month, ul_day, ul_year);

		/* Erase flag page */
		flash_erase_page(ul_last_page_addr, IFLASH_ERASE_PAGES_8);

		/* Clear backup mode flag */
		if (flash_write(ul_last_page_addr, (uint8_t *)&ul_normal_mode_flag,
				sizeof(uint32_t), 0) != FLASH_RC_OK) {
			printf("Flash Write failed!\r\n");
		}
	}

	printf("Press any key to Enter Backup Mode!\r\n");

	while (uart_read(CONSOLE_UART, &uc_key));

	/* Write the data to the backup register GPBR0 ~ 7 */
	gpbr_write(GPBR0, GPBR_CONST_DATA);
	gpbr_write(GPBR1, GPBR_CONST_DATA);
	gpbr_write(GPBR2, GPBR_CONST_DATA);
	gpbr_write(GPBR3, GPBR_CONST_DATA);
	gpbr_write(GPBR4, GPBR_CONST_DATA);
	gpbr_write(GPBR5, GPBR_CONST_DATA);
	gpbr_write(GPBR6, GPBR_CONST_DATA);
	gpbr_write(GPBR7, GPBR_CONST_DATA);

	/* Enable TMP0 and TMP2 wake up */
	supc_set_wakeup_mode(SUPC, SUPC_WUMR_LPDBCEN0_ENABLE |
			SUPC_WUMR_LPDBCCLR_ENABLE | SUPC_WUMR_LPDBCEN2_ENABLE |
			SUPC_WUMR_LPDBC_2_RTCOUT0);
	supc_set_wakeup_inputs(SUPC, SUPC_WUIR_WKUPEN0_ENABLE |
			SUPC_WUIR_WKUPEN14_ENABLE,
			SUPC_WUIR_WKUPT0_LOW | SUPC_WUIR_WKUPT14_LOW);

	if(flash_erase_page(ul_last_page_addr, IFLASH_ERASE_PAGES_8) != FLASH_RC_OK) {
		printf("Erase page failed!\r\n");
	}

	/* Write backup mode flag */
	if (flash_write(ul_last_page_addr, (uint8_t *)&ul_backup_mode_flag,
					sizeof(uint32_t), 0) != FLASH_RC_OK) {
		printf("Flash Write failed!\r\n");
	}

	printf("Enter Backup Mode!\r\n");

	/* Enter backup mode */
	pmc_enable_backupmode();

	while (1) {
	}
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

