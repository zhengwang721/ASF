/**
 * \file
 *
 * \brief SAMD20 Master I2C Slave Bootloader
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

/*! \mainpage
 * \section intro Introduction
 * This application implements a I2C Slave bootloader. I2C master sends
 * the binary file to be programmed via I2C interface.
 *
 * \section files Main Files
 * - i2c_bootloader.c: I2C Slave bootloader implementation
 * - conf_board.h: board initialization process configuration
 * - conf_clocks.h: clock specific initialization
 * - conf_bootloader.h: Bootloader specific configuration
 *
 * \section deviceinfo Device Info
 * All devices with I2C can be used. This application has been tested
 * with the following setup:
 *   - SAM0+D Xplained Pro kit connected with I2C Master on PA14 and PA15.
 *
 * \section applicationdescription Description of the application
 *   - Check the status of BOOT_LOAD_PIN to continue executing bootloader or
 *     start executing application
 *   - Initialize board, NVM, system clock
 *   - Master initially transmits 4 bytes - which is the length of the
 *     following data
 *   - Master transmits the data to be programmed to the device in blocks of
 *     NVMCTRL_PAGE_SIZE
 *   - Program the data to flash from APP_START_ADDRESS
 *   - Send an acknowledgment byte 's' to the master to indicate it has
 *     received the data and finished programming it.
 *   - Repeat till entire length of data has been programmed to the device
 *   - Enable watchdog timer with timeout of 256 clock cycles and wait
 *     for watchdog reset
 *   - BOOT_LED is 'on' until the bootloader is executing
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for ARM.
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com/">Atmel</A>.\n
 */

#include <asf.h>
#include "conf_bootloader.h"

struct i2c_slave_module slave;

struct i2c_packet packet = {
	.address     = SLAVE_ADDRESS,
	.data_length = DATA_LENGTH,
	.data        = read_buffer,
};

/* Function prototypes */
static uint32_t get_length(void);
static void fetch_data(uint8_t *buffer, uint16_t len);
static void program_memory(uint32_t address, uint8_t *buffer, uint16_t len);
static void start_application(void);
static void check_boot_mode(void);
static void send_ack(void);

/**
 * \brief Function for fetching length of file to be programmed
 *
 * Master initially transmits 4 bytes - which is the length of the
 * the data to be programmed to the device.
 */
static uint32_t get_length(void)
{
	uint8_t read_buffer[4];
	uint32_t len = 0;

	packet.data_length = 4;
	packet.data = read_buffer;

	/* Read 4 bytes of data from master */
	while (i2c_slave_read_packet_wait(&slave, &packet) != STATUS_OK);
	MSB0W(len) = read_buffer[0];
	MSB1W(len) = read_buffer[1];
	MSB2W(len) = read_buffer[2];
	MSB3W(len) = read_buffer[3];

	return len;
}

/**
 * \brief Function for fetching data to be programmed
 *
 * This function will read \ref len number of bytes from master for
 * programming the device.
 *
 * \param buffer pointer to the buffer to store data from I2C master
 * \param len    length of the data that will be sent by I2C master
 */
static void fetch_data(uint8_t *buffer, uint16_t len)
{
	packet.data_length = len;
	packet.data = buffer;

	/* Read \ref len number of bytes from master */
	while (i2c_slave_read_packet_wait(&slave, &packet) != STATUS_OK);
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
 * \brief Function for sending acknowledgment to I2C Master
 *
 * This function will write an acknowledgement byte 's' on I2C bus to
 * indicate the master that it has received and programmed the data.
 */
static void send_ack(void)
{
	uint8_t ack = 's';
	packet.data_length = 1;
	packet.data = &ack;
	while (i2c_slave_write_packet_wait(&slave, &packet) != STATUS_OK);
}

/**
 * \brief Main application
 */
int main(void)
{
	static volatile uint32_t len = 0;
	uint32_t remaining_len = 0;
	uint32_t curr_sector;
	uint32_t curr_prog_addr;
	uint8_t buff[NVMCTRL_PAGE_SIZE];
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

	/* Create and initialize config structure */
	struct i2c_slave_config config_i2c;
	i2c_slave_get_config_defaults(&config_i2c);

	/* Change address and address_mode */
	config_i2c.address = SLAVE_ADDRESS;
	config_i2c.address_mode = I2C_SLAVE_ADDRESS_MODE_MASK;
	config_i2c.pinmux_pad0 = BOOT_I2C_PAD0;
	config_i2c.pinmux_pad1 = BOOT_I2C_PAD1;

	/* Initialize and enable device with config */
	i2c_slave_init(&slave, BOOT_SERCOM, &config_i2c);

	i2c_slave_enable(&slave);

	/* Get NVM default configuration and load the same */
	nvm_get_config_defaults(&config);
	nvm_set_config(&config);

	/* Turn on LED */
	port_pin_set_output_level(BOOT_LED, false);

	/* Get the length to be programmed */
	len = get_length();
	remaining_len = len;

	do {
		/* Read data of AT45DBX_SECTOR_SIZE */
		fetch_data(buff, min(DATA_LENGTH, len));

		/* Program the read data into Flash */
		program_memory(curr_prog_addr, buff, min(DATA_LENGTH, len));

		/* Increment the current programming address */
		curr_prog_addr += min(DATA_LENGTH, len);

		/* Calculate the remaining length */
		remaining_len -= min(DATA_LENGTH, len);

		/* Update the length to remaining length to be programmed */
		len = remaining_len;

		send_ack();

		/* Do this for entire length */
	} while (len != 0);

	start_application();

	while (1) {
		/* Inf loop */
	}

}
