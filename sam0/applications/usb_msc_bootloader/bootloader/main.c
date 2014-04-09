/**
 * \file
 *
 * \brief SAM0 USB Host Mass Storage Bootloader Application with CRC Check.
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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
#include <string.h>
#include "conf_usb_host.h"
#include "conf_bootloader.h"
#if CONSOLE_OUTPUT_ENABLED
#include "conf_uart_serial.h"
#endif
#include "main.h"

/* SOF Event Counter */
static volatile uint32_t sof_count = 0;

/* Flag to track connected LUNs */
static volatile bool lun_connected = false;

/* File System Management object */
static FATFS fs; // Re-use fs for LUNs to reduce memory footprint

/* Directory management */
static DIR file_dir;

/* File object management */
static FIL file_object;

/* Input file name */
char input_file_name[] = {
	FIRMWARE_IN_FILE_NAME
};

/* File Data Buffer */
COMPILER_WORD_ALIGNED
volatile uint8_t buffer[FLASH_BUFFER_SIZE];

#if FIRMWARE_CRC_ENABLED
/* CRC32 Value of the firmware */
static uint32_t firmware_crc = 0;

/* CRC descriptor - Should be 512 byte aligned */
COMPILER_ALIGNED(512)
static crccu_dscr_type_t crc_dscr;
#endif

#if CONSOLE_OUTPUT_ENABLED
//! Console Strings
#define APP_HEADER                   \
					"ATMEL SAM D21 Firmware Generator for USB MSC BOOTLOADER\r\n"
#define TASK_PASSED                  "PASS\r\n"
#define TASK_FAILED                  "FAIL\r\n"

struct usart_module usart_instance;

static void console_init(void);
#endif

/**
 * \brief Function to start the application.
 */
static void start_application(void)
{
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

/**
 * \brief Function to issue a WDT reset to start the application.
 * This will reset the clock & peripheral configurations.
 */
static void start_application_with_wdt(void)
{
	/* Enable the Watchdog module */
	WDT->CTRL.reg = WDT_CTRL_ENABLE;

//	NVIC_SystemReset();

	while (1) {
		/* Wait indefinitely for a WDT reset */
	}
}

/* 
 * This function check if the applicatino or bootloade should be run
 */
static void check_boot_mode(void)
{
	uint32_t app_check_address;
	uint32_t *app_check_address_ptr;

	/* Check if WDT is locked */
	if (!(WDT->CTRL.reg & WDT_CTRL_ALWAYSON)) {
		/* Disable the Watchdog module */
		WDT->CTRL.reg &= ~WDT_CTRL_ENABLE;
	}

	app_check_address = APP_START_ADDRESS;
	app_check_address_ptr = (uint32_t *)app_check_address;

	board_init();

	if (port_pin_get_input_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
		/* Button is pressed, run bootloader */
		return;
	}

	if (*app_check_address_ptr == 0xFFFFFFFF) {
		/* No application; run bootloader */
		return;
	}

	/* Enters application mode*/
	start_application();
}

/**
 * \brief Function to program the Flash. Decrypt the firmware and program it.
 */
static bool program_memory(void)
{
	uint32_t buffer_size = 0;
	uint32_t file_size = 0;
	uint32_t curr_address = 0;
	uint32_t current_page = 0;
	uint16_t rows_clear, i;
	enum status_code error_code;
	uint8_t page_buffer[NVMCTRL_PAGE_SIZE];

#if VERIFY_PROGRAMMING_ENABLED
	uint32_t firmware_crc_output;
#endif

	/* Open the input file */
	f_open(&file_object,
			(char const *)input_file_name,
			FA_OPEN_EXISTING | FA_READ);

	file_size = file_object.fsize;

#if FIRMWARE_CRC_ENABLED
	/* Read the CRC data & Signature from the firmware */
	f_read(&file_object, (void *)buffer, APP_BINARY_OFFSET, &buffer_size);

	/* Check if there is any buffer */
	if (!buffer_size) {
		return false;
	}
#endif

	/* Set the busy LED */
	port_pin_set_output_level(LED0_PIN, LED0_ACTIVE);

	current_page = APP_START_ADDRESS /
			NVMCTRL_PAGE_SIZE;
	curr_address = 0;
	
	/* Erase flash rows to fit new firmware */
	rows_clear = file_size / NVMCTRL_ROW_SIZE;
	for (i = 0; i < rows_clear; i++) {
		do {
			error_code =
			nvm_erase_row(
			(APP_START_ADDRESS) +
			(NVMCTRL_ROW_SIZE * i));
		} while (error_code == STATUS_BUSY);
	}

	/* Program the flash memory page by page */
	do {
		/* Open the input file */
		f_read(&file_object, (void *)page_buffer, FLASH_BUFFER_SIZE, &buffer_size);
		/* Check if there is any buffer */
		if (!buffer_size) {
			break;
		}

		curr_address +=  buffer_size;

		/* Disable the global interrupts */
		cpu_irq_disable();

		/* Program the Flash Memory. */
		/* Write page buffer to flash */
		do {
			error_code =
			nvm_write_buffer(
				current_page *
				NVMCTRL_PAGE_SIZE,
				page_buffer,
				buffer_size);
		} while (error_code == STATUS_BUSY);

		/* Enable the global interrupts */
		cpu_irq_enable();

		current_page++;
	} while (curr_address < file_size);

	/* Close the File after operation. */
	f_close(&file_object);

#if VERIFY_PROGRAMMING_ENABLED
	/* Update the buffer size */
	buffer_size = address_offset;
	/* Re-initialize offset to zero */
	address_offset = 0;

	/* Reset the CRCCU to start fresh calculation */
	crccu_reset(CRCCU);

	/* CRC calculation with max 0xFFF since the CRCCU size limit is 12-bit */
	do {
		/* Calculate the CRC32 of the programmed memory */
		crc32_calculate((uint32_t)(APP_START_ADDRESS + address_offset),
							min(0xFFF, buffer_size));
		buffer_size -= min(0xFFF, buffer_size);
		address_offset += 0xFFF;
	} while (buffer_size!=0);

	/* Get the CRC32 value */
	firmware_crc_output = crccu_read_crc_value(CRCCU);

	if (APP_CRC_POLYNOMIAL_TYPE == CRCCU_MR_PTYPE_CCITT16) {
		/* 16-bit CRC */
		firmware_crc_output &= 0xFFFF;
	}

	/* Compare the calculated CRC Value */
	if (firmware_crc != firmware_crc_output) {
		/* Verification Failed */
		return false;
	}
#endif

	/* Clear the busy LED */
	port_pin_set_output_level(LED0_PIN, LED0_INACTIVE);

	/* return true */
	return true;
}


#if FIRMWARE_CRC_ENABLED
/**
 * \brief Function to check the integrity of the firmware.
 */
static bool integrity_check()
{
	uint8_t *buf = NULL;
	uint32_t firmware_crc_output = 0;
	uint32_t buffer_size = 0;
	uint8_t *signature_bytes = APP_SIGNATURE;

	/* Reset the CRCCU */
	crccu_reset(CRCCU);

	/* Open the input file */
	f_open(&file_object,
			(char const *)input_file_name,
			FA_OPEN_EXISTING | FA_READ);

	/* Routine to check the signature for decryption
	 * Firmware - First 4 bytes - CRC32, 12 bytes - Firmware Signature/Password
	 * Remaining bytes - Application binary file
	 */

	/* Read the CRC data & Signature from the firmware */
	f_read(&file_object, (void *)buffer, APP_BINARY_OFFSET, &buffer_size);
	/* Check if there is any buffer */
	if (!buffer_size) {
		/* Close the File after operation. */
		f_close(&file_object);
		return false;
	}

	/* Update the buffer */
	buf = (uint8_t *) buffer;

	/* Store the firmware CRC */
	firmware_crc = *(uint32_t *)buf;
	/* Update the buffer size */
	buffer_size-=4;
	buf+=4;
	/* Validate the Signature bytes */
	while (buffer_size--) {
		if (*buf++ != *signature_bytes++) {
			/* Signature verification failed */
			/* Close the File after operation. */
			f_close(&file_object);
			return false;
		}
	}

	/*
	 * File pointer didnt get updated at this point to the binary offset value.
	 * Calling F_SEEK again.
	 */
	f_lseek(&file_object, APP_BINARY_OFFSET);
	/* Verify the CRC32 of the entire decrypted file before programming */
	while (true) {
		/* Read the data from the firmware */
		f_read(&file_object, (void *)buffer, FLASH_BUFFER_SIZE, &buffer_size);
		/* Check if there is any buffer */
		if (!buffer_size) {
			break;
		}

		/* Update the output buffer */
		buf = (uint8_t *) buffer;

		/* Calculate the CRC32 for the buffer */
		crc32_calculate((uint32_t) buf, buffer_size);
	}

	/* Get the CRC32 value */
	firmware_crc_output = crccu_read_crc_value(CRCCU);

	if (APP_CRC_POLYNOMIAL_TYPE == CRCCU_MR_PTYPE_CCITT16) {
		/* 16-bit CRC */
		firmware_crc_output &= 0xFFFF;
	}

	/* Compare the calculated CRC Value */
	if (firmware_crc != firmware_crc_output) {
		/* Close the File after operation. */
		f_close(&file_object);
		/* Return false on compare match fail */
		return false;
	}

	/* Close the File after operation. */
	f_close(&file_object);

	/* Return true on compare match pass */
	return true;
}
#endif

#if FIRMWARE_CRC_ENABLED
static void crc32_calculate(uint32_t address, uint32_t size)
{
	/* Set the memory address for CRCCU DMA transfer */
	crc_dscr.ul_tr_addr = address;

	/* Transfer width: byte, interrupt disable(here interrupt mask enabled) */
	crc_dscr.ul_tr_ctrl =
	CRCCU_TR_CTRL_TRWIDTH_BYTE | size |
	CRCCU_TR_CTRL_IEN_DISABLE;

	/* Configure the CRCCU descriptor */
	crccu_configure_descriptor(CRCCU, (uint32_t) &crc_dscr);

	/* Configure CRCCU mode */
	crccu_configure_mode(CRCCU, CRCCU_MR_ENABLE | APP_CRC_POLYNOMIAL_TYPE);

	/* Start the CRC calculation */
	crccu_enable_dma(CRCCU);

	/* Wait for calculation ready */
	while ((crccu_get_dma_status(CRCCU) == CRCCU_DMA_SR_DMASR)) {
	}
}
#endif

#if CONSOLE_OUTPUT_ENABLED
/**
 * \brief Initializes the console output
 */
static void console_init(void)
{
	struct usart_config usart_conf;
	struct usart_module cdc_uart_module;

	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = CONF_STDIO_MUX_SETTING;
	usart_conf.pinmux_pad0 = CONF_STDIO_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = CONF_STDIO_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = CONF_STDIO_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = CONF_STDIO_PINMUX_PAD3;
	usart_conf.baudrate    = CONF_STDIO_BAUDRATE;

	stdio_serial_init(&cdc_uart_module, CONF_STDIO_USART_MODULE, &usart_conf);
	usart_enable(&cdc_uart_module);
}
#endif


/**
 * \brief Initializes the device for the bootloader
 */
static void bootloader_system_init(void)
{
	struct nvm_config nvm_cfg;
	struct port_config pin;

	/* Initialize the system */
	system_init();

    /*Configures PORT for LED0*/
	port_get_config_defaults(&pin);
	pin.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LED0_PIN, &pin);

	/* Initialize the NVM */
	nvm_get_config_defaults(&nvm_cfg);
	nvm_set_config(&nvm_cfg);

	/* Initialize the sleep manager */
	sleepmgr_init();

#if CONSOLE_OUTPUT_ENABLED
	/* Initialize the console */
	console_init();
	/* Print a header */
	printf(APP_HEADER);
#endif

	/* Enable the global interrupts */
	cpu_irq_enable();

	/* Start USB host stack */
	uhc_start();
}


/**
 * \brief Main function. Execution starts here.
 */
int main(void)
{
	uint32_t file_size = 0;
	uint8_t lun;
	FRESULT res;

	/* Check whether the bootloader mode is activated */
	check_boot_mode();

	/* Device initialization for the bootloader */
	bootloader_system_init();

#if CONSOLE_OUTPUT_ENABLED
	/* Print a header */
	printf("Insert device\r\n");
#endif
	/* The USB management is entirely managed by interrupts. */
	while (true) {
		/* Check if a MSC device and its LUN are configured. */
		if (!lun_connected) {
			continue;
		}
		/* Adding a small delay to ensure that the MSC device is ready */
		while (sof_count < 1000);

#if CONSOLE_OUTPUT_ENABLED
		/* Print a header */
		printf("Device Connected\r\n");
#endif
		/* Go through the different LUN and check for the file. */
		for (lun = 0; (lun < uhi_msc_mem_get_lun()) && (lun < 8); lun++) {

			TCHAR root_directory[3] = "0:";
			root_directory[0] = '0' + lun;

			/* Initialize the file system object */
			memset(&fs, 0, sizeof(FATFS));

#if CONSOLE_OUTPUT_ENABLED
			/* Print the current task */
			printf("Mounting the device...\r\n");
#endif
			/* 
			 * Set and mount the currently selected LUN as a drive
			 * in Navigator
			 */
			res = f_mount(lun, &fs);
			/* Check the return status */
			if (res != FR_OK) {
				continue;
			}

			/* Open the root directory */
			res = f_opendir(&file_dir, root_directory);
			/* Check the return status */
			if (res != FR_OK) {
#if CONSOLE_OUTPUT_ENABLED
				/* Print task output */
				printf(TASK_FAILED);
#endif
				/* Check the next LUN */
				continue;
			}

#if CONSOLE_OUTPUT_ENABLED
			/* Print the current task */
			printf("Search firmware...\r\n");
#endif
			/* Firmware files on the disk */
			input_file_name[0] = lun + '0';
			/* Open the input file */
			res = f_open(&file_object,
				(char const *)input_file_name,
				FA_OPEN_EXISTING | FA_READ);
			/* Get the file length */
			file_size = file_object.fsize;
			/* Check if file size is greater than the possible flash size */
			/* Open the firmware upgrade file in READ mode from the disk */
			if (!(file_size && (file_size < APP_MAX_SIZE)
					&& (res == FR_OK))) {
#if CONSOLE_OUTPUT_ENABLED
				/* Print task output */
				printf(TASK_FAILED);
#endif
				/* LUN error */
				f_close(&file_object);
				continue;
			}

			/* Close the File after operation. */
			f_close(&file_object);

#if CONSOLE_OUTPUT_ENABLED
			/* Print the current task */
			printf(TASK_PASSED);
			printf("Integrity check...\r\n");
#endif

#if FIRMWARE_CRC_ENABLED
			/* Validate the firmware file */
			if (!integrity_check()) {
#if CONSOLE_OUTPUT_ENABLED
				/* Print a header */
				printf(TASK_FAILED);
#endif
				continue;
			}
#endif

#if CONSOLE_OUTPUT_ENABLED
			/* Print the current task */
			printf(TASK_PASSED);
#  if VERIFY_PROGRAMMING_ENABLED
			printf("Programming & verifying...\r\n");
#  else
			printf("Programming...\r\n");
#  endif
#endif

			/* Program the memory */
			if (!program_memory()) {
#if CONSOLE_OUTPUT_ENABLED
				/* Print task output */
				printf(TASK_FAILED);
#endif
				continue;
			}

			/* Unmount the MSC device */
			f_mount(lun, NULL);

			/* Stop the USB Host */
			uhc_stop(true);

			/* Disable global interrupts. */
			cpu_irq_disable();

#if CONSOLE_OUTPUT_ENABLED
			/* Print the current task */
			printf(TASK_PASSED);
			printf("Starting application...\r\n");
#endif

			/* Start the application with a WDT Reset */
			start_application_with_wdt();
		}
		/* None of the connected LUN has the upgrade file. */
		lun_connected = false;
	}
}

/**
 * \brief Notify that a SOF has been sent (each 1 ms)
 */
void main_usb_sof_event(void)
{
	sof_count++;
}

/**
 * \brief Callback on enumeration status change of a MSC device
 */
void main_usb_connection_event(uhc_device_t * dev, bool b_present)
{
	/* To remove compiler warning */
	UNUSED(dev);

	/* Enumeration status of the connected MSC device */
	lun_connected = b_present;

	/* Reset sof_count to start MSC delay*/
	sof_count = 0;
}



/**
 * \mainpage ASF USB host mass storage
 *
 * \section intro Introduction
 * This application demonstrates a very simple bootloader application for the
 * SAM D21. It use the USB Host capability to read data from a connected
 * USB mass storage device.
 *
 * \section startup Startup
 * After loading firmware, connect a FAT formatted mass storage device to the
 * SAM D20 Xplained Pro with a firmware file called firmware.bin.
 *
 * If the device is blank (never been bootloaded), the bootloader will always
 * start
 * If the device have been programmed already you need to press the SW0 button
 * on the SAM D21 Xplained Pro after reset to enter the bootloader.
 * When running, the bootloader will search for a file called firmware.bin .
 * If this file is found it will be programmed to the device and the new
 * firmware will be started.
 *
 * \copydoc UI
 *
 * \section example About example
 *
 * The example uses the following module groups:
 * - Basic modules:
 *   Startup, board, clock, interrupt, power management
 * - USB host stack and MSC modules:
 *   <br>services/usb/
 *   <br>services/usb/uhc/
 *   <br>services/usb/class/msc/host/
 * - Thirdparty modules:
 *   <br>thirdparty/fatfs

 */

