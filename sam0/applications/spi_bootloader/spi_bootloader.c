/**
 * \file
 *
 * \brief SAMD20 Master SPI Bootloader
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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
#include <asf.h>
#include <at45dbx.h>
#include "conf_bootloader.h"

/* Function prototypes */
static uint32_t get_length(void);
static void fetch_data(uint32_t sector, uint8_t *buffer, uint16_t len);
static void program_memory(uint32_t address, uint8_t *buffer, uint16_t len);
static void start_application(void);
static void check_boot_mode(void);

/**
 * \brief Function for fetching length of file to be programmed
 *
 * This function will open sector 0 of AT45DBX dataflash component and read the
 * first 4 bytes which contain the length of the file to be programmed. It
 * closes the sector after reading
 */
static uint32_t get_length(void)
{
	uint32_t len = 0;

	/* Open sector 0 to get the file length */
	at45dbx_read_sector_open(0);

	/* Read the first 4 bytes which contain the length */
	MSB0W(len) = at45dbx_read_byte();
	MSB1W(len) = at45dbx_read_byte();
	MSB2W(len) = at45dbx_read_byte();
	MSB3W(len) = at45dbx_read_byte();

	/* Close sector */
	at45dbx_read_close();

	return len;
}

/**
 * \brief Function for fetching data to be programmed
 *
 * This function will fetch the data from the AT45DBX dataflash 
 * component to be programmed to Flash
 *
 * \param sector sector number of dataflash AT45DBX
 * \param buffer pointer to the buffer to store data from AT45DBX
 * \param len    length of the data to be read from AT45DBX
 */
static void fetch_data(uint32_t sector, uint8_t *buffer, uint16_t len)
{
	/* Open the dataflash sector */
	at45dbx_read_sector_open(sector);
	/* Read the contents to RAM */
	at45dbx_read_sector_to_ram(buffer);
	/* Close the sector */
	at45dbx_read_close();
}

/**
 * \brief Function for programming data to Flash
 *
 * This function will check whether the data is greater than Flash page size.
 * If it is greater, it splits and writes pagewise.
 *
 * \param address address of the Flash page to be programmed
 * \param buffer  pointer to the buffer containing data to be programmed
 * \param len     length of the data to be programmed to Flash
 */
static void program_memory(uint32_t address, uint8_t *buffer, uint16_t len)
{
	/* Check if length is greater than Flash page size */
	if (len > NVMCTRL_PAGE_SIZE) {
		uint32_t offset = 0;
		while (len > NVMCTRL_PAGE_SIZE) {
			/* Check if it is first page of a row */
			if ((address & 0xFF) == 0) {
				/* Erase row */
				nvm_erase_row(address);
			}
			/* Write one page data to flash */
			nvm_write_buffer(address, buffer + offset, NVMCTRL_PAGE_SIZE);
			/* Increment the address to be programmed */
			address += NVMCTRL_PAGE_SIZE;
			/* Increment the offset of the buffer containing data */
			offset += NVMCTRL_PAGE_SIZE;
			/* Decrement the length */
			len -= NVMCTRL_PAGE_SIZE;
		}
		/* Check if there is data remaining to be programmed*/
		if (len > 0) {
			/* Write the data to flash */
			nvm_write_buffer(address, buffer + offset, len);
		}
	} else {
		/* Check if it is first page of a row) */
		if ((address & 0xFF) == 0) {
			/* Erase row */
			nvm_erase_row(address);
		}
		/* Write the data to flash */
		nvm_write_buffer(address, buffer, len);
	}
}

/**
 * \brief Function for starting application
 *
 * This function will configure the WDT module and enable it. The LED is
 * kept toggling till WDT reset occurs.
 */
static void start_application(void)
{
	struct wdt_conf wdt_config;

	/* Turn off LED */
	port_pin_set_output_level(BOOT_LED, true);

	/* Get WDT default configuration */
	wdt_get_config_defaults(&wdt_config);

	/* Set the required clock source and timeout period */
	wdt_config.clock_source = GCLK_GENERATOR_4;
	wdt_config.timeout_period = WDT_PERIOD_256CLK;

	/* Initialize WDT */
	wdt_init(&wdt_config);

	wdt_enable();

	while (1) {
		port_pin_toggle_output_level(BOOT_LED);
	}
}

/**
 * \brief Function for checking whether to enter boot mode or application mode
 *
 * This function will check the state of BOOT_LOAD_PIN. If it is pressed, it
 * continues execution in bootloader mode. Else, it jumps to the application
 * and starts execution from there
 * Access to direct peripheral registers are made in this routine to enable
 * quick decision on application or bootloader mode.
 */
static void check_boot_mode(void)
{
	/* Check if WDT is locked */
	if (!(WDT->CTRL.reg & WDT_CTRL_ALWAYSON)) {
		/* Disable the Watchdog module */
		WDT->CTRL.reg &= ~WDT_CTRL_ENABLE;
	}

	volatile PortGroup *boot_port = (volatile PortGroup *)
			(&(PORT->Group[BOOT_LOAD_PIN / 32]));
	volatile bool boot_en;

	/* Enable the input mode in Boot GPIO Pin */
	boot_port->DIRCLR.reg = GPIO_BOOT_PIN_MASK;
	boot_port->PINCFG[BOOT_LOAD_PIN & 0x1F].reg = 
			PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	boot_port->OUTSET.reg = GPIO_BOOT_PIN_MASK;

	/* Read the BOOT_LOAD_PIN status */
	boot_en = (boot_port->IN.reg) & GPIO_BOOT_PIN_MASK;

	/* Check the BOOT pin or the reset cause is Watchdog */
	if ((boot_en) || (PM->RCAUSE.reg & PM_RCAUSE_WDT)) {
		/* Pointer to the Application Section */
		void (*application_code_entry)(void);

		/* Rebase the Stack Pointer */
		__set_MSP(*(uint32_t *) APP_START_ADDRESS);

		/* Rebase the vector table base address */
		SCB->VTOR = ((uint32_t) APP_START_ADDRESS & SCB_VTOR_TBLOFF_Msk);

		/* Load the Reset Handler address of the application */
		application_code_entry = (void (*)(void))(unsigned *)(*(unsigned *)
				(APP_START_ADDRESS + 4));

		/* Jump to user Reset Handler in the application */
		application_code_entry();
	}
}

/**
 * \brief Main application
 */
int main(void)
{
	uint32_t len = 0;
	uint32_t curr_sector;
	uint32_t curr_prog_addr;
	uint8_t buff[AT45DBX_SECTOR_SIZE] = {0};
	struct nvm_config config;

	/* Check switch state to enter boot mode or application mode */
	check_boot_mode();

	/* Data to be programmed is available from sector 1 of AT45DBX */
	curr_sector = 1;
	/* 
	 * Application to be programmed from APP_START_ADDRESS defined in
	 * conf_bootloader.h
	 */
	curr_prog_addr = APP_START_ADDRESS;

	/* Initialize system */
	system_init();

	/* Initialize AT45DBX dataflash component */
	at45dbx_init();

	/* Get NVM default configuration and load the same */
	nvm_get_config_defaults(&config);
	nvm_set_config(&config);

	/* Turn on LED */
	port_pin_set_output_level(BOOT_LED, false);

	/* Check the dataflash component */
	if (at45dbx_mem_check() == true) {
		/* Get the length to be programmed */
		len = get_length();

		do {
			/* Read data of AT45DBX_SECTOR_SIZE */
			fetch_data(curr_sector, buff, min(AT45DBX_SECTOR_SIZE, len));

			/* Program the read data into Flash */
			program_memory(curr_prog_addr, buff, min(AT45DBX_SECTOR_SIZE, len));

			/* Increment the dataflash sector */
			curr_sector++;

			/* Increment the current programming address */
			curr_prog_addr += min(AT45DBX_SECTOR_SIZE, len);

			/* Update the length to remaining length to be programmed */
			len -= min(AT45DBX_SECTOR_SIZE, len);

			/* Do this for entire length */
		} while (len != 0);

		start_application();
	}

	while (1) {
		/* Inf loop */
	}

}
