/**
 * \file
 *
 * \brief SAM4L USB Host Mass Storage Bootloader Application with CRC Check and 
 * AES decryption support.
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
#include <string.h>
#include <stdio.h>
#include "conf_usb_host.h"
#include "conf_bootloader.h"
#include "main.h"
#include "uhc.h"
#include "uhi_msc.h"
#include "uhi_msc_mem.h"
#include "ff.h"
#include "crccu.h"

#if FIRMWARE_AES_ENABLED
#include "aesa.h"
#endif

#define GPIO_BOOT_PIN_PORT        ((volatile GpioPort *)(GPIO_ADDR + (MSC_BOOT_LOAD_PIN >> 5) * sizeof(GpioPort)))
#define GPIO_BOOT_PIN_MASK        (1U << (MSC_BOOT_LOAD_PIN & 0x1F))

/*****************************************************************************/
/*                              GLOBAL VARIABLES                             */
/*****************************************************************************/
/* Flag to track connected LUNs */
volatile bool lun_connected = false;

/* File object management */
static FIL file_object;

/* Input file name */
char input_file_name[] = {
	FIRMWARE_IN_FILE_NAME
};

/* CRC32 Value of the firmware */
static uint32_t firmware_crc = 0;

/* Revision of the application firmware */
static uint16_t firmware_rev = *(uint16_t *)FIRMWARE_REV_ADDRESS;

/* Force Boot Option */
static bool force_boot = (((*(uint16_t *) BOOT_CONF_ADDRESS) & FORCE_BOOT_MASK) == 1); 

/* CRC descriptor - Should be 512 byte aligned */
__attribute__ ((aligned(512)))
static crccu_dscr_type_t crc_dscr;

/* File Data Buffer */
__attribute__ ((aligned(4)))
volatile uint8_t buffer[FLASH_BUFFER_SIZE];

#if FIRMWARE_AES_ENABLED
/* AES Output Buffer */
__attribute__ ((aligned(4)))
volatile uint32_t aes_output[FLASH_BUFFER_SIZE/4];
#endif

/*****************************************************************************/
/*                             FUNCTION DECLARATIONS                         */
/*****************************************************************************/
static void bootloader_activation_check(void);

static void start_application(void);

static void start_application_with_WDT(void);

static bool program_memory(void);

static bool integrity_check(void);

static void crc32_calculate(uint32_t address, uint32_t size);

static void bootloader_system_init(void);

#if FIRMWARE_AES_ENABLED

static void aes_init(void);

static void aes_decrypt(uint32_t *encrypted_data, uint32_t size);

#endif

#if CONSOLE_OUTPUT_ENABLED

static void console_init(void);

#endif

/*****************************************************************************/
/*                             FUNCTION DEFINITIONS                          */
/*****************************************************************************/

/**
 * \brief Function to start the application.
 */
static void start_application(void)
{
	/* Pointer to the Application Section */
	void (*application_code_entry)(void);
	unsigned *p;
	
	/* Rebase the Stack Pointer */
	__set_MSP(*(uint32_t *) APP_START_ADDRESS);
		
	/* Rebase the vector table base address */
	SCB->VTOR = ((uint32_t) APP_START_ADDRESS & SCB_VTOR_TBLOFF_Msk);

	/* Load the Reset Handler address of the application */
	application_code_entry = (void (*)(void))(unsigned *)(*(unsigned *)(APP_START_ADDRESS + 4));

	/* Jump to user Reset Handler in the application */
	application_code_entry();
}


/**
 * \brief Function to check the bootloader mode activaton.
 */
static void bootloader_activation_check(void)
{
	/*
	 * Check the following parameters for loading the bootloader
	 * Bootloader Force bit is set
	 * No watchdog Reset
	 * Activation of the boot pin
	 * Absence of Application by checking the firmware revision data
	 */
	 if (!force_boot) {
		/* Enable the input mode in Boot GPIO Pin */
		GPIO_BOOT_PIN_PORT->GPIO_ODERC = GPIO_BOOT_PIN_MASK;
		GPIO_BOOT_PIN_PORT->GPIO_STERS = GPIO_BOOT_PIN_MASK;
		boot_en = (uint32_t) (GPIO_ADDR + (MSC_BOOT_LOAD_PIN >> 5) * sizeof(GpioPort));
		boot_en = ((GpioPort *)(boot_en))->GPIO_PVR & GPIO_BOOT_PIN_MASK;
		/* In absence of firmware or activation of boot pin or on absence of WDT reset enter bootloader mode */
		if (!((firmware_rev == 0xFFFF) || (boot_en) || (PM->PM_RCAUSE & PM_RCAUSE_WDT))) {
			/* Start the application - No return*/
			start_application();
		}
	}
	/* Enters bootloader mode */
}


/**
 * \brief Function to issue a WDT reset to start the application.
 * This will reset the clock & peripheral configurations.
 */
static void start_application_with_WDT(void)
{
	/* Disable the global interrupts */
	cpu_irq_disable();

	/* Switch off the LED */
	ioport_set_pin_level(BOOT_LED, 0);

	/* Store the WDT Configuration value */
	uint32_t wdt_ctrl_val = (WDT_CTRL_EN | WDT_CTRL_PSEL(5) | WDT_CTRL_CEN	| WDT_CTRL_DAR) ;

	/* Set the WDT to trigger a reset - Two times for the two keys*/
	WDT->WDT_CTRL = wdt_ctrl_val | WDT_CTRL_KEY(0x55);
	WDT->WDT_CTRL = wdt_ctrl_val | WDT_CTRL_KEY(0xAA);

	/* Wait indefinitely for a WDT reset */
	while (1) {
		/* Toggle the LED */
		ioport_toggle_pin_level(BOOT_LED);
	}
}


/**
 * \brief Function to program the Flash. Decrypt the firmware and program it.
 */
static bool program_memory(void)
{
	uint32_t address_offset = 0;
	void *buf = NULL;
	uint32_t buffer_size = 0;
	uint32_t firmware_crc_output = 0;

	/* Open the input file */
	f_open(&file_object,
			(char const *)input_file_name,
			FA_OPEN_EXISTING | FA_READ);

	/* Read the CRC data & Signature from the firmware */
	f_read(&file_object, (void *)buffer, APP_BINARY_OFFSET, &buffer_size);

	/* Check if there is any buffer */
	if (!buffer_size) {
		return false;
	}

#if FIRMWARE_AES_ENABLED
	/* Decrypt as a new message */
	aes_init();
	/* Decrypt the AES data */
	aes_decrypt((uint32_t *)buffer, APP_BINARY_OFFSET/4);
#endif

	/* Set the busy LED */
	ioport_set_pin_level(BOOT_LED, 0);

	/* Program the flash memory page by page */
	while(true) {
		/* Open the input file */
		f_read(&file_object, (void *)buffer, FLASH_BUFFER_SIZE, &buffer_size);

		/* Check if there is any buffer */
		if (!buffer_size) {
			break;
		}

#if FIRMWARE_AES_ENABLED
		/* Decrypt the binary data */
		aes_decrypt((uint32_t *)buffer, buffer_size/4);
		/* Update the output buffer */
		buf = (void *)aes_output;
#else
		/* Update the output buffer */
		buf = (void *)buffer;
#endif

		/* Disable the global interrupts */
		cpu_irq_disable();

		/* Program the Flash Memory. */
		flashcalw_memcpy((void *)(APP_START_ADDRESS + address_offset),
		buf , buffer_size, true);
		
		/* Enable the global interrupts */
		cpu_irq_enable();

		/* Update the address and page offset values */
		address_offset += buffer_size;
	}

	/* Close the File after operation. */
	f_close(&file_object);

#if VERIFY_PROGRAMMING_ENABLED
		/* Update the buffer size */
		buffer_size = address_offset;
		/* Re-initialize offset to zero */
		address_offset = 0;
		/* Reset the CRCCU to start fresh calculation */
		crccu_reset(CRCCU);
		/* Do it in a loop of max size 12-bit since the CRCCU can operate at a max of 12-bit only*/
		do {
			/* Calculate the CRC32 of the programmed memory with a max limit of 0xFFF*/
			crc32_calculate((uint32_t)(APP_START_ADDRESS + address_offset), min(0xFFF, buffer_size));
			buffer_size -= min(0xFFF, buffer_size);
			address_offset += 0xFFF;
		} while (buffer_size!=0);

		/* Get the CRC32 value */
		firmware_crc_output = crccu_read_crc_value(CRCCU);

		if (APP_CRC_POLYNOMIAL_TYPE == CRCCU_MR_PTYPE_CCITT16) {
			/* 16-bit CRC */
			firmware_crc_output &= 0xFF;
		}

		/* Compare the calculated CRC Value */
		if (firmware_crc != firmware_crc_output) {
			/* Verification Failed */
			return false;
		}

#endif

	/* Clear the busy LED */
	ioport_set_pin_level(BOOT_LED, 1);

	/* return true */
	return true;
}


/**
 * \brief Function to check the integrity of the firmware.
 */
static bool integrity_check(void)
{
	uint32_t *buf = NULL;
	uint32_t firmware_crc_output = 0;
	uint32_t buffer_size = 0;

	/* Enable CRCCU peripheral clock */
	sysclk_enable_peripheral_clock(CRCCU);

	/* Reset the CRCCU */
	crccu_reset(CRCCU);

	/* Open the input file */
	f_open(&file_object,
			(char const *)input_file_name,
			FA_OPEN_EXISTING | FA_READ);

	/* Routine to check the signature for decryption
	 * Firmware - First 4 bytes - CRC32, Next 12 bytes - Firmware Signature/Password
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

#if FIRMWARE_AES_ENABLED
	/* Initialize the AES Module */
	aes_init();
	/* Decrypt the AES data */
	aes_decrypt((uint32_t *)buffer, APP_BINARY_OFFSET/4);
	
	/* Get the firmware CRC32 value */
	firmware_crc = aes_output[0];

	/* Check the signature bytes */
	if ((aes_output[1] != APP_SIGNATURE_WORD0) || (aes_output[2] != APP_SIGNATURE_WORD1) || (aes_output[3] != APP_SIGNATURE_WORD2)) {
		/* Signature verification failed */
		/* Close the File after operation. */
		f_close(&file_object);
		return false;
	}
#else
	/* Update the buffer */
	buf = (uint32_t *) buffer;
	/* Store the firmware CRC */
	firmware_crc = buf[0];
	/* Check the signature bytes */
	if ((buf[1] != APP_SIGNATURE_WORD0) || (buf[2] != APP_SIGNATURE_WORD1) || (buf[3] != APP_SIGNATURE_WORD2)) {
		/* Signature verification failed */
		/* Close the File after operation. */
		f_close(&file_object);
		return false;
	}
#endif
	/* File pointer didnt get updated at this point to the binary offset value. Calling F_SEEK again. */
	f_lseek(&file_object, APP_BINARY_OFFSET);
	/* Verify the CRC32 of the entire decrypted file before starting the programming sequence */
	while(true) {
		/* Read the data from the firmware */
		f_read(&file_object, (void *)buffer, FLASH_BUFFER_SIZE, &buffer_size);
		/* Check if there is any buffer */
		if (!buffer_size) {
			break;
		}
#if FIRMWARE_AES_ENABLED
		/* Decrypt the application binary */
		aes_decrypt((uint32_t *)(buffer), buffer_size/4);
		/* Update the output buffer */
		buf = (uint32_t *) aes_output;
#else
		/* Update the output buffer */
		buf = (uint32_t *) buffer;
#endif

		/* Calculate the CRC32 for the buffer */
		crc32_calculate((uint32_t) buf, buffer_size);
	}

	/* Get the CRC32 value */
	firmware_crc_output = crccu_read_crc_value(CRCCU);

	if (APP_CRC_POLYNOMIAL_TYPE == CRCCU_MR_PTYPE_CCITT16) {
		/* 16-bit CRC */
		firmware_crc_output &= 0xFF;
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


#if FIRMWARE_AES_ENABLED
/**
 * \brief AES Initialization routine
 */
static void aes_init()
{
	aesa_config_t aesa_config;
	uint32_t aesa_key[FIRMWARE_AES_KEY_SIZE * 2 + 4] = {
		 FIRMWARE_AES_KEY_WORD0
		,FIRMWARE_AES_KEY_WORD1
		,FIRMWARE_AES_KEY_WORD2
		,FIRMWARE_AES_KEY_WORD3
		#if FIRMWARE_AES_KEY_SIZE > AESA_KEY_SIZE_128
		,FIRMWARE_AES_KEY_WORD4
		,FIRMWARE_AES_KEY_WORD5
		#endif
		#if FIRMWARE_AES_KEY_SIZE > AESA_KEY_SIZE_192
		,FIRMWARE_AES_KEY_WORD6
		,FIRMWARE_AES_KEY_WORD7
		#endif
	};

	uint32_t aesa_initvect[4] = {
		FIRMWARE_AES_INITVECT_WORD0,
		FIRMWARE_AES_INITVECT_WORD1,
		FIRMWARE_AES_INITVECT_WORD2,
		FIRMWARE_AES_INITVECT_WORD3
	};

	/* Reset the AES */
	aesa_reset(AESA);
	
	/* Enable the AES Module */
	aesa_enable(AESA);

	/* Configure the AESA. */
	aesa_config.encrypt_mode = AESA_DECRYPTION;
	aesa_config.key_size = FIRMWARE_AES_KEY_SIZE;
	aesa_config.dma_mode = AESA_MANUAL_MODE;
	aesa_config.opmode = AESA_CBC_MODE;
	aesa_config.cfb_size = AESA_CFB_SIZE_128;
	aesa_config.countermeasure_mask = 0xF;
	aesa_set_config(AESA, &aesa_config);

	/* Beginning of a new message. */
	aesa_set_new_message(AESA);

	/* Set the cryptographic key. */
	aesa_write_key(AESA, aesa_key);

	/* Set the initialization vector. */
	aesa_write_initvector(AESA, aesa_initvect);
}


/**
 * \brief AES Decryption routine
 */
static void aes_decrypt(uint32_t *encrypted_data, uint32_t size)
{
	uint16_t i;
	for (i = 0; i < size ; i+=4) {
		/* Write the data to be ciphered to the input data registers. */
		aesa_write_input_data(AESA, (encrypted_data[i]));
		aesa_write_input_data(AESA, (encrypted_data[i+1]));
		aesa_write_input_data(AESA, (encrypted_data[i+2]));
		aesa_write_input_data(AESA, (encrypted_data[i+3]));

		/* Wait until the output data is ready */
		while(!(aesa_read_status(AESA) & AESA_SR_ODATARDY));

		/* Read the decrypted output data */
		aes_output[i]     = aesa_read_output_data(AESA);
		aes_output[i + 1] = aesa_read_output_data(AESA);
		aes_output[i + 2] = aesa_read_output_data(AESA);
		aes_output[i + 3] = aesa_read_output_data(AESA);
	}
}

#endif

#if CONSOLE_OUTPUT_ENABLED
/**
 * \brief Initializes the console output
 */
static void console_init(void)
{
		const sam_usart_opt_t usart_serial_options = {
			.baudrate     = CONSOLE_UART_BAUDRATE,
			.parity_type  = CONSOLE_UART_PARITY,
			.char_length  = CONSOLE_UART_CHAR_LENGTH,
			.stop_bits    = CONSOLE_UART_STOPBITS,
			.channel_mode = US_MR_CHMODE_NORMAL
		};
		/* Enable the clock for the console UART */
		sysclk_enable_peripheral_clock(CONSOLE_UART);
		/* Initialize the UART Module for console output */
		usart_init_rs232(CONSOLE_UART, &usart_serial_options, sysclk_get_peripheral_bus_hz(CONSOLE_UART));
		/* Enable the transmitter. */
		usart_enable_tx(CONSOLE_UART);
}
#endif


/**
 * \brief Initializes the device for the bootloader
 */
static void bootloader_system_init(void)
{
	/* Initialize the bootloader system */
	bootloader_system_init();

	/* Initialize the system clocks */
	sysclk_init();

	/* Initialize the board components */
	board_init();

	/* Initialize the sleep manager */
	sleepmgr_init();

#if CONSOLE_OUTPUT_ENABLED
	/* Initialize the console */
	console_init();
	/* Print a header */
	CONSOLE_PUTS(APP_HEADER);
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
	/* File System Management object */
	FATFS fs; // Re-use fs for LUNs to reduce memory footprint

	/* Check the bootloader activation condition */
	bootloader_activation_check();

	/* Device initialization for the bootloader */
	bootloader_system_init();

#if CONSOLE_OUTPUT_ENABLED
		/* Print a header */
		CONSOLE_PUTS("\n\rInsert device");
#endif
	/* The USB management is entirely managed by interrupts. */
	while (true) {

		/* Check if a MSC device and its LUN are configured. */
		if (!lun_connected) {
			continue;
		}

		FRESULT res;
		/* Directory management */
		DIR file_dir;
#if CONSOLE_OUTPUT_ENABLED
		/* Print a header */
		CONSOLE_PUTS("\n\r Device Connected");
#endif
		/* Go through the different LUN and check for the file. */
		for (lun = 0; (lun < uhi_msc_mem_get_lun()) && (lun < 8); lun++) {

			TCHAR root_directory[3] = "0:";
			root_directory[0] = '0' + lun;

			/* Initialize the file system object */
			memset(&fs, 0, sizeof(FATFS));

#if CONSOLE_OUTPUT_ENABLED
			/* Print the current task */
			CONSOLE_PUTS("\n\rMounting the device...");
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
				CONSOLE_PUTS(TASK_FAILED);
#endif
				/* Check the next LUN */
				continue;
			}

#if CONSOLE_OUTPUT_ENABLED
			/* Print the current task */
			CONSOLE_PUTS("\n\rSearch firmware...");
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
				CONSOLE_PUTS(TASK_FAILED);
#endif
				/* LUN error */
				f_close(&file_object);
				continue;
			}

			/* Close the File after operation. */
			f_close(&file_object);

#if CONSOLE_OUTPUT_ENABLED
			/* Print the current task */
			CONSOLE_PUTS(TASK_PASSED);
			CONSOLE_PUTS("\n\rIntegrity check...");
#endif
			/* Validate the firmware file */
			if (!integrity_check()) {
#if CONSOLE_OUTPUT_ENABLED
				/* Print a header */
				CONSOLE_PUTS(TASK_FAILED);
#endif
				continue;
			}

#if CONSOLE_OUTPUT_ENABLED
			/* Print the current task */
			CONSOLE_PUTS(TASK_PASSED);
#  if VERIFY_PROGRAMMING_ENABLED
			CONSOLE_PUTS("\n\rProgramming & verifying...");
#  else
			CONSOLE_PUTS("\n\rProgramming...");
#  endif
#endif
			/* Program the memory */
			if (!program_memory()) {
#if CONSOLE_OUTPUT_ENABLED
				/* Print task output */
				CONSOLE_PUTS(TASK_FAILED);
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
			CONSOLE_PUTS(TASK_PASSED);
			CONSOLE_PUTS("\n\rStarting application...");
#endif

			/* Start the application with a WDT Reset */
			start_application_with_WDT();
		}
		/* None of the connected LUN has the upgrade file. */
		lun_connected = false;
	}
}

void main_usb_connection_event(uhc_device_t * dev, bool b_present)
{
	lun_connected = b_present;
}

/**
 * \mainpage ASF SAM4L USB Host Mass Storage Bootloader Solution
 *
 * \section intro Introduction
 * SAM4L USB Host Mass Storage Bootloader application is to facilitate firmware upgrade
 * using a USB MSC drives. The application includes firmware revision, CRC check,
 * Signature verification, AES decryption and memory verification functionality offering
 * safe and secure firmware upgradation.
 *
 * \section startup Procedure
 * - Do complete chip erase and Userpage erase.
 * - Program the bootloader
 * - Load the application firmware into U-disk. Connect it to the SAM4L-EK USB MSC Host.
 * - Press PB0 on RESET to start the bootloader.
 *
 * \section config Configuration Options
 * - conf_bootloader.h -> Bootloader Configurations
 *   Important configuration options
 *   - FIRMWARE_AES_ENABLED       -> Enable/disable the AES Decryption
 *   - CONSOLE_OUTPUT_ENABLED     -> Enable/disable the Console message output
 *   - VERIFY_PROGRAMMING_ENABLED -> Enable/disable the verification of programmed memory
 *   - APP_START_OFFSET           -> Application starting offset from Flash origin
 *   - FIRMWARE_IN_FILE_NAME      -> Application Firmware file to be programmed
 *   - APP_SIGNATURE              -> Signature bytes to be verified
 *   - MSC_BOOT_LOAD_PIN          -> IO Pin used for bootloader activation
 *   - MSC_BOOT_LOAD_PIN_ACTIVE_LVL -> Active level to be monitored for the pin
 *   - BOOT_CONF_ADDRESS          -> Bootloader settings in Userpage (Last word in Userpage)
 * 
 * \section board Board Setup
 * - SAM4L-EK -> Has an IO configured for VBUS Detect. VBUS Pin jumper PA06/USB should be set
 *   - conf_board.h -> USB Pin configuration
 *   - CONF_BOARD_USB_PORT           -> Enable USB interface
 *   - CONF_BOARD_USB_VBUS_CONTROL   -> VBUS control enabled, jumper PC08/USB should be set
 *   - CONF_BOARD_USB_VBUS_ERR_DETECT-> VBUS error control enabled, jumper PC07/USB should be set
 *   - An external power supply should be used since the VBUS is powered only through the external
 *     power supply controlled by the VBUS Control(VBOF) pin. Refer the SAM4L-EK schematics for
 *     more details.
 *   - Console message output is sent through the Embedded Debugger(onboard)'s COM PORT.
 * 
 * \section func Bootloader Operation
 * The bootloader will decrypt the first block (24 bytes of data). It verifies the
 * Signature data. If the verification is successful, it decrypts the entire firmware and
 * generates CRC32 value and compares it with the stored CRC32 value. If it matches, it
 * starts to program the application. Then, verification of the programmed memory (CRC32
 * check again) is performed. Then it programs the Firmware Revision into the User jumps
 * to the application section with WDT reset.
 * Output Firmware Structure with AES:
 * - 4 bytes   -> Encrypted CRC32
 * - 2 bytes   -> Encrypted Firmware Revision
 * - 18 bytes  -> Encrypted Signature Data
 * - Rest data -> Encrypted Input Firmware
 * Output Firmware Structure without AES:
 * - 4 bytes   -> CRC32
 * - 2 bytes   -> Firmware Revision
 * - 18 bytes  -> Signature Data
 * - Rest data -> Input Firmware
 *
 * \copydoc UI
 *
 * \section  dependencies Application Dependencies
 *
 * The application uses the following module groups:
 * - Basic modules:
 *   Startup, board, clock, interrupt, power management
 * - USB host stack and MSC modules:
 *   <br>services/usb/
 *   <br>services/usb/uhc/
 *   <br>services/usb/class/msc/host/
 * - Thirdparty modules:
 *   <br>thirdparty/fatfs
 * - Specific implementation:
 *    - main.c,
 *      <br>initializes clock
 *      <br>initializes interrupt
 *      <br>initializes USB, FATFS, AES, CRCCU
 *      <br>Search, validate, program & verify the firmware
 */
