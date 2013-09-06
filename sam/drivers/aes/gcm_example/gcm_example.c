/**
 *
 * \file
 *
 * \brief AES-GCM example for SAM.
 *
 * This file defines a useful set of functions for the AES-GCM mode on SAM devices.
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

/** \mainpage
 * \section intro Introduction
 * This is the documentation for the data structures, functions, variables,
 * defines, enums, and typedefs for the AES driver. <BR>It also gives several
 * examples of usage of the AES-GCM mode: <BR>
 * - GCM ciphering and deciphering.
 *
 * \section files Main Files
 * - aes.c : AES driver
 * - aes.h : AES header file
 * - gcm_example.c : AES-GCM code example
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR Systems compiler.
 * Other compilers may or may not work.
 *
 * \section deviceinfo Device Info
 * All SAM devices with an AES-GCM feature can be used. This example has been
 * tested with the following setup:<BR>
 * - SAM4C evaluation kit.
 *
 * \section setupinfo Setup Information
 * <BR>CPU speed: <i> 120 MHz </i>
 * - Connect the DBGU port com to a PC
 * - PC terminal settings:
 *     - 115200 bps,
 *     - 8 data bits,
 *     - no parity bit,
 *     - 1 stop bit,
 *     - no flow control.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com">Atmel</A>.\n
 */

#include <asf.h>
#include <string.h>

/* Key Sizes in bytes - 256 bits */
#define AES_KEY_SIZE		32

/* IV Size in bytes - 96 bits */
#define AES_IV_SIZE		12

/* TAG size in bytes - 128 bits */
#define AES_TAG_SIZE		16

/* Max Plain Text Size (16 byte aligned for padding) used in bytes -  bits */
#define AES_PDATA_SIZE		32

/* Max AAD Size (16 byte aligned for padding) used in bytes -  bits */
#define AES_AAD_SIZE		32

/* Effective AAD Size */
#define AES_AAD_EFFECTIVE_SIZE		20

/* Effective Plain Text Size */
#define AES_PDATA_EFFECTIVE_SIZE	20

/**
 * \name reference data for AES-GCM mode
 */
/* @{ */

/* GCM Mode Test Key, 256-bits */
uint8_t aes_key[AES_KEY_SIZE] = {
	0xf8, 0xd4, 0x76, 0xcf, 0xd6, 0x46, 0xea, 0x6c, 
	0x23, 0x84, 0xcb, 0x1c, 0x27, 0xd6, 0x19, 0x5d, 
	0xfe, 0xf1, 0xa9, 0xf3, 0x7b, 0x9c, 0x8d, 0x21, 
	0xa7, 0x9c, 0x21, 0xf8, 0xcb, 0x90, 0xd2, 0x89
};

/* GCM Mode Test Iv, 96-bits */
uint8_t aes_iv[AES_IV_SIZE] = {
	0xdb, 0xd1, 0xa3, 0x63, 0x60, 0x24, 
	0xb7, 0xb4, 0x02, 0xda, 0x7d, 0x6f
};

/* GCM Mode Test plain text, 160-bits */ 
uint8_t aes_plain_text[AES_PDATA_SIZE] = {
	0x90, 0xae, 0x61, 0xcf, 0x7b, 0xae, 0xbd, 0x4c, 
	0xad, 0xe4, 0x94, 0xc5, 0x4a, 0x29, 0xae, 0x70, 
	0x26, 0x9a, 0xec, 0x71
};

/* GCM Mode Test AAD, 160-bits */
uint8_t aes_aad[AES_AAD_SIZE] = {
	0x7b, 0xd8, 0x59, 0xa2, 0x47, 0x96, 0x1a, 0x21, 
	0x82, 0x3b, 0x38, 0x0e, 0x9f, 0xe8, 0xb6, 0x50, 
	0x82, 0xba, 0x61, 0xd3
};

/* Reference GCM cipher data */
uint8_t aes_cipher_text[AES_PDATA_SIZE] = {
	0xce, 0x20, 0x27, 0xb4, 0x7a, 0x84, 0x32, 0x52, 
	0x01, 0x34, 0x65, 0x83, 0x4d, 0x75, 0xfd, 0x0f, 
	0x07, 0x29, 0x75, 0x2e
};

/* Reference GCM Tag data */
uint8_t aes_tag[AES_TAG_SIZE] = {
	0xac, 0xd8, 0x83, 0x38, 0x37, 0xab, 0x0e, 0xde, 
	0x84, 0xf4, 0x74, 0x8d, 0xa8, 0x89, 0x9c, 0x15
};

/* @} */

/* GCM Input Structure */
struct gcm_input{
	uint32_t iv_len;
	uint32_t text_len;
	uint32_t aad_len;
	uint8_t  key_len;
	uint8_t  *key;
	uint8_t  *iv;
	uint8_t  *input;
	uint8_t  *output;
	uint8_t  *aad;
	uint8_t  *tag;
};

/* Output data array */
static uint8_t output_data[AES_PDATA_SIZE];

/* Tag data array */
static uint8_t tag_data[AES_TAG_SIZE];

/* J0 data array */
static uint8_t j0[16];

/** AES configuration */
struct aes_config   g_aes_cfg;

/** GCM Input */
struct gcm_input    gcm_input_data;


/**
 * \brief Read the GCM Authentication Tag "T" generated in AES_TAGRx registers.
 *
 * \param p_aes             Base address of the AES instance.
 * \param p_auth_tag_buffer Pointer to output buffer of 4 contiguous 32-bit words.
 *
 */
static void aes_read_gcm_auth_tag(Aes *const p_aes, uint32_t *p_auth_tag_buffer)
{
        uint8_t i;
        for (i = 0; i < 4 ;i++) {
            p_auth_tag_buffer[i] = aes_read_tag(p_aes, i);
        }
}

/**
 * \brief GCM mode encryption and generate tag automatically.
 */
static void gcm_mode_encryption_test(void)
{
	printf("\r\n-----------------------------------\r\n");
	printf("- 256bit cryptographic key\r\n");
	printf("- GCM Encryption\r\n");
	printf("- Auto start mode\r\n");
	printf("- input of 160-bit effective words\r\n");
	printf("-----------------------------------\r\n");

	/* Configure the AES. */
	g_aes_cfg.encrypt_mode = AES_ENCRYPTION;
	g_aes_cfg.key_size = AES_KEY_SIZE_256;
	g_aes_cfg.start_mode = AES_AUTO_START;
	g_aes_cfg.opmode = AES_GCM_MODE;
	g_aes_cfg.cfb_size = AES_CFB_SIZE_128;
	g_aes_cfg.lod = false;

	/* AES-GCM Input Configuration */
	gcm_input_data.key = aes_key;
	gcm_input_data.key_len = AES_KEY_SIZE;
	gcm_input_data.iv = aes_iv;
	gcm_input_data.iv_len = AES_IV_SIZE;
	gcm_input_data.input = aes_plain_text;
	gcm_input_data.text_len = AES_PDATA_EFFECTIVE_SIZE;
	gcm_input_data.aad = aes_aad;
	gcm_input_data.aad_len = AES_AAD_EFFECTIVE_SIZE;
	gcm_input_data.output = output_data;
	gcm_input_data.tag = tag_data;

	/* Set GTAGEN to '1' and generate Tag automatically */
	g_aes_cfg.gtag_en = true;
	aes_set_config(AES, &g_aes_cfg);

	/* Write the key to generate GCMH Subkey */
	aes_write_key(AES, (const uint32_t *)gcm_input_data.key);
	
	/* Wait for the GCMH to generate */
	while(!(aes_read_interrupt_status(AES)& AES_ISR_DATRDY));

	/* Generate J0 using IV(96 Bits) */
	uint32_t i;
	for (i = 0; i < 12; i++) {
		j0[i] = gcm_input_data.iv[i];
	}
	j0[15] = 1;

	/* Write the j0 + 1 in IV register */
	j0[15] += 1;
	aes_write_initvector(AES, (uint32_t *)j0);

	/* set AADLEN */
	aes_write_authen_datalength(AES, gcm_input_data.aad_len);

	/* Set CLEN */
	aes_write_pctext_length(AES, gcm_input_data.text_len);

	/* Write AAD Data for TAG generation */
	uint32_t offset = 0;

	for (i = 0; i < (AES_AAD_SIZE/16); i++) {
	/* write the input data */
	aes_write_input_data(AES, (const uint32_t *)(gcm_input_data.aad + offset)); 
	/* Wait till TAG is ready */
	while(!(aes_read_interrupt_status(AES)& AES_ISR_DATRDY));
	/* 16-Byte Boundaries */
	offset += 16;  
	}

	/* Reset offset to zero */
	offset = 0;

	/* Write plain text for cipher text generation */
	for (i = 0; i < (AES_PDATA_SIZE/16); i++) {
		/* write the input data */
		aes_write_input_data(AES, (const uint32_t *)(gcm_input_data.input + offset)); 
		/* Wait for the operation to complete */
		while(!(aes_read_interrupt_status(AES)& AES_ISR_DATRDY));
		aes_read_output_data(AES, (uint32_t *)(gcm_input_data.output + offset));
		offset += 16;
	}

	/* Wait for the operation to complete */
	while(!(aes_read_interrupt_status(AES)& AES_ISR_TAGRDY));
	/* Read the generated tag */
	aes_read_gcm_auth_tag(AES, (uint32_t *)gcm_input_data.tag);

	if( memcmp((uint8_t *)tag_data,(uint8_t *)aes_tag, AES_TAG_SIZE) != 0) {
		printf("\n\rTAG GENERATE FAILED! ");
	} else {
		printf("\n\rTAG GENERATE SUCCESS! ");
	}	

	if ( memcmp((uint8_t *)output_data,(uint8_t *)aes_cipher_text,AES_PDATA_EFFECTIVE_SIZE) != 0) {
		printf("\n\rENC COMPARE FAILED! ");
	} else {
		printf("\n\rENC COMPARE SUCCESS! ");
	}
}

/**
 * \brief GCM mode decryption test.
 */
static void gcm_mode_decryption_test(void)
{
	printf("\r\n-----------------------------------\r\n");
	printf("- 256bit cryptographic key\r\n");
	printf("- GCM Decryption\r\n");
	printf("- Auto start mode\r\n");
	printf("- input of 160-bit effective words\r\n");
	printf("-----------------------------------\r\n");

	/* Configure the AES. */
	g_aes_cfg.encrypt_mode = AES_DECRYPTION;
	g_aes_cfg.key_size = AES_KEY_SIZE_256;
	g_aes_cfg.start_mode = AES_AUTO_START;
	g_aes_cfg.opmode = AES_GCM_MODE;
	g_aes_cfg.cfb_size = AES_CFB_SIZE_128;
	g_aes_cfg.lod = false;
	g_aes_cfg.gtag_en = false;
	aes_set_config(AES, &g_aes_cfg);

	/* AES-GCM Input Configuration */
	gcm_input_data.key = aes_key;
	gcm_input_data.key_len = AES_KEY_SIZE;
	gcm_input_data.iv = aes_iv;
	gcm_input_data.iv_len = AES_IV_SIZE;
	gcm_input_data.input = aes_cipher_text;
	gcm_input_data.text_len = AES_PDATA_EFFECTIVE_SIZE;
	gcm_input_data.aad = aes_aad;
	gcm_input_data.aad_len = AES_AAD_EFFECTIVE_SIZE;
	gcm_input_data.output = output_data;
	gcm_input_data.tag = tag_data;

	/* Write the key to generate GCMH Subkey */
	aes_write_key(AES, (const uint32_t *)gcm_input_data.key);

	/* Wait for the GCMH to generate */
	while(!(aes_read_interrupt_status(AES)& AES_ISR_DATRDY));

	/* Generate J0 using IV(96 Bits) */
	uint32_t i;
	for (i = 0; i < 12; i++) {
		j0[i] = gcm_input_data.iv[i];
	}
	j0[15] = 1;

	/* Write the j0 + 1 in IV register */
	j0[15] += 1;
	aes_write_initvector(AES, (uint32_t *)j0);
 
	/* set AADLEN */
	aes_write_authen_datalength(AES, gcm_input_data.aad_len);

	/* Set CLEN */
	aes_write_pctext_length(AES, gcm_input_data.text_len);
 
	/* Write AAD Data for TAG generation */
	uint32_t offset = 0;
	for (i = 0; i < (AES_AAD_SIZE/16); i++) {
		/* write the input data */
		aes_write_input_data(AES, (const uint32_t *)(gcm_input_data.aad + offset)); 
		/* Wait till TAG is ready */
		while(!(aes_read_interrupt_status(AES)& AES_ISR_DATRDY));
		/* 16-Byte Boundaries */
		offset += 16;  
	}
 
	/* Reset offset to zero */
	offset = 0;

	/* Write plain text for cipher text generation */
	for (i = 0; i < (AES_PDATA_SIZE/16); i++) {
		/* write the input data */
		aes_write_input_data(AES, (const uint32_t *)(gcm_input_data.input + offset)); 
		/* Wait for the operation to complete */
		while(!(aes_read_interrupt_status(AES)& AES_ISR_DATRDY));
		aes_read_output_data(AES, (uint32_t *)(gcm_input_data.output + offset));
		offset += 16;
	}

	if ( memcmp((uint8_t *)output_data,(uint8_t *)aes_plain_text,AES_PDATA_EFFECTIVE_SIZE) != 0) {
		printf("\n\rDEC COMPARE FAILED! ");
	} else {
		printf("\n\rDEC COMPARE SUCCESS! ");
	}
}

/**
 *  Configure serial console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console. */
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief Display the user menu on the terminal.
 */
static void display_menu(void)
{
	printf("Menu :\n\r"
			"  -- Select operation:\n\r"
			"  h: Display menu \n\r"
			"  e: GCM mode encryption test. \n\r"
			"  d: GCM mode decryption test. \n\r"
			"  \n\r\n\r");
}

/**
 * \brief The main function.
 */
int main(void)
{
	uint8_t key;

	/* Initialize the SAM system */
	sysclk_init();
	board_init();

	/* Initialize the console  */
	configure_console();

	/* Output example information */
	printf("-- AES Example --\r\n");
	printf("-- %s\n\r", BOARD_NAME);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

	/* Enable the AES module. */
	aes_get_config_defaults(&g_aes_cfg);
	aes_init(AES, &g_aes_cfg);
	aes_enable();

	/* Enable AES interrupt. */
	aes_enable_interrupt(AES,AES_INTERRUPT_DATA_READY);

	/* Display menu */
	display_menu();

	while (1) {
		scanf("%c", (char *)&key);

		switch (key) {
		case 'h':
			display_menu();
			break;

		case 'e':
			printf("GCM mode encryption test.\r\n");
			gcm_mode_encryption_test();
			break;

		case 'd':
			printf("GCM mode decryption test.\r\n");
			gcm_mode_decryption_test();
			break;

		default:
			break;
		}
	}
}
