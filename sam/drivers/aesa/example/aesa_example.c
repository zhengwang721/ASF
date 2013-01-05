/**
 *
 * \file
 *
 * \brief AESA software driver for SAM.
 *
 * This file defines a useful set of functions for the AESA on SAM devices.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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
 * defines, enums, and typedefs for the AESA driver. <BR>It also gives several
 * examples of usage of the AESA module: <BR>
 * - ECB ciphering in manual mode with no counter measures,
 * - ECB ciphering in manual mode with all counter measures activated,
 * - demonstration of the URAD security feature,
 * - ECB deciphering in auto mode with no counter measures,
 * - CBC ciphering in auto mode with no counter measures.<BR>
 *
 * \section files Main Files
 * - aesa.c : AESA driver
 * - aesa.h : AESA header file
 * - aesa_example.c : AESA code example
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR Systems compiler.
 * Other compilers may or may not work.
 *
 * \section deviceinfo Device Info
 * All SAM devices with an AESA module can be used. This example has been
 * tested with the following setup:<BR>
 * - SAM4L evaluation kit.
 *
 * \section setupinfo Setup Information
 * <BR>CPU speed: <i> 12 MHz </i>
 * - Connect the SAM4L USB virtual port com to a PC
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

#define AESA_EXAMPLE_REFBUF_SIZE 4

/**
 * \name sample data from NIST-800-38A appendix F
 */
/* @{ */
/** Reference plain data. */
const uint32_t ref_plain_text[AESA_EXAMPLE_REFBUF_SIZE] = {
	0xe2bec16b,
	0x969f402e,
	0x117e3de9,
	0x2a179373
};

/** Reference ECB cipher data. */
const uint32_t ref_cipher_text_ecb[AESA_EXAMPLE_REFBUF_SIZE] = {
	0xb47bd73a,
	0x60367a0d,
	0xf3ca9ea8,
	0x97ef6624
};

/** Reference CBC cipher data. */
const uint32_t ref_cipher_text_cbc[AESA_EXAMPLE_REFBUF_SIZE] = {
	0xacab4976,
	0x46b21981,
	0x9b8ee9ce,
	0x7d19e912
};

/** Reference CFB128 cipher data. */
const uint32_t ref_cipher_text_cfb128[AESA_EXAMPLE_REFBUF_SIZE] = {
	0x2ed93f3b,
	0x20ad2db7,
	0xf8493433,
	0x4afb3ce8
};

/** Reference OFB cipher data. */
const uint32_t ref_cipher_text_ofb[AESA_EXAMPLE_REFBUF_SIZE] = {
	0x2ed93f3b,
	0x20ad2db7,
	0xf8493433,
	0x4afb3ce8
};

/** Reference CTR cipher data. */
const uint32_t ref_cipher_text_ctr[AESA_EXAMPLE_REFBUF_SIZE] = {
	0x91614d87,
	0x26e320b6,
	0x6468ef1b,
	0xceb60d99
};

/** Cipher 128 bits key array. */
const uint32_t key128[4] = {
	0x16157e2b,
	0xa6d2ae28,
	0x8815f7ab,
	0x3c4fcf09
};

/** Init vector array for ECB, CBC, CFB128 and OFB mode. */
const uint32_t init_vector[4] = {
	0x03020100,
	0x07060504,
	0x0b0a0908,
	0x0f0e0d0c
};

/** Init vector array for CTR mode. */
const uint32_t init_vector_ctr[4] = {
	0xf3f2f1f0,
	0xf7f6f5f4,
	0xfbfaf9f8,
	0xfffefdfc
};

/* @} */

/* Output data array */
static uint32_t output_data[AESA_EXAMPLE_REFBUF_SIZE];

/* Config variable */
static aesa_config_t aesa_config;

/* State indicate */
volatile uint32_t state = false;

/**
 * \brief The AESA interrupt handler.
 */
void AESA_Handler(void)
{
	/* Read the output(this will clear the DATRDY flag). */
	output_data[0] = aesa_read_output_data(AESA);
	output_data[1] = aesa_read_output_data(AESA);
	output_data[2] = aesa_read_output_data(AESA);
	output_data[3] = aesa_read_output_data(AESA);
	state = true;
}

/**
 * \brief ECB mode encryption and decryption test.
 */
static void ecb_mode_test(void)
{
	printf("\r\n-----------------------------------\r\n");
	printf("- 128bit cryptographic key\r\n");
	printf("- ECB cipher mode\r\n");
	printf("- all counter measures\r\n");
	printf("- input of 4 32bit words\r\n");
	printf("-----------------------------------\r\n");

	state = false;

	/* Configure the AESA. */
	aesa_config.encrypt_mode = AESA_ENCRYPTION;
	aesa_config.key_size = AESA_KEY_SIZE_128;
	aesa_config.dma_mode = AESA_MANUAL_MODE;
	aesa_config.opmode = AESA_ECB_MODE;
	aesa_config.cfb_size = AESA_CFB_SIZE_128;
	aesa_config.countermeasure_mask = 0xF;
	aesa_set_config(AESA, &aesa_config);

	/* Beginning of a new message. */
	aesa_set_new_message(AESA);

	/* Set the cryptographic key. */
	aesa_write_key(AESA, key128);

	/* The initialization vector is not used by the ECB cipher mode. */

	/* Write the data to be ciphered to the input data registers. */
	aesa_write_input_data(AESA, ref_plain_text[0]);
	aesa_write_input_data(AESA, ref_plain_text[1]);
	aesa_write_input_data(AESA, ref_plain_text[2]);
	aesa_write_input_data(AESA, ref_plain_text[3]);

	/* Wait for the end of the encryption process. */
	while (false == state) {
	}

	if ((ref_cipher_text_ecb[0] != output_data[0]) ||
			(ref_cipher_text_ecb[1] != output_data[1]) ||
			(ref_cipher_text_ecb[2] != output_data[2]) ||
			(ref_cipher_text_ecb[3] != output_data[3])) {
		printf("\r\nKO!!!\r\n");
	} else {
		printf("\r\nOK!!!\r\n");
	}

	printf("\r\n-----------------------------------\r\n");
	printf("- 128bit cryptographic key\r\n");
	printf("- ECB decipher mode\r\n");
	printf("- all counter measures\r\n");
	printf("- input of 4 32bit words\r\n");
	printf("-----------------------------------\r\n");

	state = false;

	/* Configure the AESA. */
	aesa_config.encrypt_mode = AESA_DECRYPTION;
	aesa_config.key_size = AESA_KEY_SIZE_128;
	aesa_config.dma_mode = AESA_MANUAL_MODE;
	aesa_config.opmode = AESA_ECB_MODE;
	aesa_config.cfb_size = AESA_CFB_SIZE_128;
	aesa_config.countermeasure_mask = 0xF;
	aesa_set_config(AESA, &aesa_config);

	/* Beginning of a new message. */
	aesa_set_new_message(AESA);

	/* Set the cryptographic key. */
	aesa_write_key(AESA, key128);

	/* The initialization vector is not used by the ECB cipher mode. */

	/* Write the data to be deciphered to the input data registers. */
	aesa_write_input_data(AESA, ref_cipher_text_ecb[0]);
	aesa_write_input_data(AESA, ref_cipher_text_ecb[1]);
	aesa_write_input_data(AESA, ref_cipher_text_ecb[2]);
	aesa_write_input_data(AESA, ref_cipher_text_ecb[3]);

	/* Wait for the end of the decryption process. */
	while (false == state) {
	}

	/* check the result. */
	if ((ref_plain_text[0] != output_data[0]) ||
			(ref_plain_text[1] != output_data[1]) ||
			(ref_plain_text[2] != output_data[2]) ||
			(ref_plain_text[3] != output_data[3])) {
		printf("\r\nKO!!!\r\n");
	} else {
		printf("\r\nOK!!!\r\n");
	}
}

/**
 * \brief CBC mode encryption and decryption test.
 */
static void cbc_mode_test(void)
{
	printf("\r\n-----------------------------------\r\n");
	printf("- 128bit cryptographic key\r\n");
	printf("- CBC cipher mode\r\n");
	printf("- all counter measures\r\n");
	printf("- input of 4 32bit words\r\n");
	printf("-----------------------------------\r\n");

	state = false;

	/* Configure the AESA. */
	aesa_config.encrypt_mode = AESA_ENCRYPTION;
	aesa_config.key_size = AESA_KEY_SIZE_128;
	aesa_config.dma_mode = AESA_MANUAL_MODE;
	aesa_config.opmode = AESA_CBC_MODE;
	aesa_config.cfb_size = AESA_CFB_SIZE_128;
	aesa_config.countermeasure_mask = 0xF;
	aesa_set_config(AESA, &aesa_config);

	/* Beginning of a new message. */
	aesa_set_new_message(AESA);

	/* Set the cryptographic key. */
	aesa_write_key(AESA, key128);

	/* Set the initialization vector. */
	aesa_write_initvector(AESA, init_vector);

	/* Write the data to be ciphered to the input data registers. */
	aesa_write_input_data(AESA, ref_plain_text[0]);
	aesa_write_input_data(AESA, ref_plain_text[1]);
	aesa_write_input_data(AESA, ref_plain_text[2]);
	aesa_write_input_data(AESA, ref_plain_text[3]);

	/* Wait for the end of the encryption process. */
	while (false == state) {
	}

	if ((ref_cipher_text_cbc[0] != output_data[0]) ||
			(ref_cipher_text_cbc[1] != output_data[1]) ||
			(ref_cipher_text_cbc[2] != output_data[2]) ||
			(ref_cipher_text_cbc[3] != output_data[3])) {
		printf("\r\nKO!!!\r\n");
	} else {
		printf("\r\nOK!!!\r\n");
	}

	printf("\r\n-----------------------------------\r\n");
	printf("- 128bit cryptographic key\r\n");
	printf("- CBC decipher mode\r\n");
	printf("- all counter measures\r\n");
	printf("- input of 4 32bit words\r\n");
	printf("-----------------------------------\r\n");

	state = false;

	/* Configure the AESA. */
	aesa_config.encrypt_mode = AESA_DECRYPTION;
	aesa_config.key_size = AESA_KEY_SIZE_128;
	aesa_config.dma_mode = AESA_MANUAL_MODE;
	aesa_config.opmode = AESA_CBC_MODE;
	aesa_config.cfb_size = AESA_CFB_SIZE_128;
	aesa_config.countermeasure_mask = 0xF;
	aesa_set_config(AESA, &aesa_config);

	/* Beginning of a new message. */
	aesa_set_new_message(AESA);

	/* Set the cryptographic key. */
	aesa_write_key(AESA, key128);

	/* Set the initialization vector. */
	aesa_write_initvector(AESA, init_vector);

	/* Write the data to be deciphered to the input data registers. */
	aesa_write_input_data(AESA, ref_cipher_text_cbc[0]);
	aesa_write_input_data(AESA, ref_cipher_text_cbc[1]);
	aesa_write_input_data(AESA, ref_cipher_text_cbc[2]);
	aesa_write_input_data(AESA, ref_cipher_text_cbc[3]);

	/* Wait for the end of the decryption process. */
	while (false == state) {
	}

	if ((ref_plain_text[0] != output_data[0]) ||
			(ref_plain_text[1] != output_data[1]) ||
			(ref_plain_text[2] != output_data[2]) ||
			(ref_plain_text[3] != output_data[3])) {
		printf("\r\nKO!!!\r\n");
	} else {
		printf("\r\nOK!!!\r\n");
	}
}

/**
 * \brief CFB128 mode encryption and decryption test.
 */
static void cfb128_mode_test(void)
{
	printf("\r\n-----------------------------------\r\n");
	printf("- 128bit cryptographic key\r\n");
	printf("- CFB128 cipher mode\r\n");
	printf("- all counter measures\r\n");
	printf("- input of 4 32bit words\r\n");
	printf("-----------------------------------\r\n");

	state = false;

	/* Configure the AESA. */
	aesa_config.encrypt_mode = AESA_ENCRYPTION;
	aesa_config.key_size = AESA_KEY_SIZE_128;
	aesa_config.dma_mode = AESA_MANUAL_MODE;
	aesa_config.opmode = AESA_CFB_MODE;
	aesa_config.cfb_size = AESA_CFB_SIZE_128;
	aesa_config.countermeasure_mask = 0xF;
	aesa_set_config(AESA, &aesa_config);

	/* Beginning of a new message. */
	aesa_set_new_message(AESA);

	/* Set the cryptographic key. */
	aesa_write_key(AESA, key128);

	/* Set the initialization vector. */
	aesa_write_initvector(AESA, init_vector);

	/* Write the data to be ciphered to the input data registers. */
	aesa_write_input_data(AESA, ref_plain_text[0]);
	aesa_write_input_data(AESA, ref_plain_text[1]);
	aesa_write_input_data(AESA, ref_plain_text[2]);
	aesa_write_input_data(AESA, ref_plain_text[3]);

	/* Wait for the end of the encryption process. */
	while (false == state) {
	}

	/* check the result. */
	if ((ref_cipher_text_cfb128[0] != output_data[0]) ||
			(ref_cipher_text_cfb128[1] != output_data[1]) ||
			(ref_cipher_text_cfb128[2] != output_data[2]) ||
			(ref_cipher_text_cfb128[3] != output_data[3])) {
		printf("\r\nKO!!!\r\n");
	} else {
		printf("\r\nOK!!!\r\n");
	}

	printf("\r\n-----------------------------------\r\n");
	printf("- 128bit cryptographic key\r\n");
	printf("- CFB128 decipher mode\r\n");
	printf("- all counter measures\r\n");
	printf("- input of 4 32bit words\r\n");
	printf("-----------------------------------\r\n");

	state = false;

	/* Configure the AESA. */
	aesa_config.encrypt_mode = AESA_DECRYPTION;
	aesa_config.key_size = AESA_KEY_SIZE_128;
	aesa_config.dma_mode = AESA_MANUAL_MODE;
	aesa_config.opmode = AESA_CFB_MODE;
	aesa_config.cfb_size = AESA_CFB_SIZE_128;
	aesa_config.countermeasure_mask = 0xF;
	aesa_set_config(AESA, &aesa_config);

	/* Beginning of a new message. */
	aesa_set_new_message(AESA);

	/* Set the cryptographic key. */
	aesa_write_key(AESA, key128);

	/* Set the initialization vector. */
	aesa_write_initvector(AESA, init_vector);

	/* Write the data to be deciphered to the input data registers. */
	aesa_write_input_data(AESA, ref_cipher_text_cfb128[0]);
	aesa_write_input_data(AESA, ref_cipher_text_cfb128[1]);
	aesa_write_input_data(AESA, ref_cipher_text_cfb128[2]);
	aesa_write_input_data(AESA, ref_cipher_text_cfb128[3]);

	/* Wait for the end of the decryption process. */
	while (false == state) {
	}

	/* check the result. */
	if ((ref_plain_text[0] != output_data[0]) ||
			(ref_plain_text[1] != output_data[1]) ||
			(ref_plain_text[2] != output_data[2]) ||
			(ref_plain_text[3] != output_data[3])) {
		printf("\r\nKO!!!\r\n");
	} else {
		printf("\r\nOK!!!\r\n");
	}
}

/**
 * \brief OFB mode encryption and decryption test.
 */
static void ofb_mode_test(void)
{
	printf("\r\n-----------------------------------\r\n");
	printf("- 128bit cryptographic key\r\n");
	printf("- OFB cipher mode\r\n");
	printf("- all counter measures\r\n");
	printf("- input of 4 32bit words\r\n");
	printf("-----------------------------------\r\n");

	state = false;

	/* Configure the AESA. */
	aesa_config.encrypt_mode = AESA_ENCRYPTION;
	aesa_config.key_size = AESA_KEY_SIZE_128;
	aesa_config.dma_mode = AESA_MANUAL_MODE;
	aesa_config.opmode = AESA_OFB_MODE;
	aesa_config.cfb_size = AESA_CFB_SIZE_128;
	aesa_config.countermeasure_mask = 0xF;
	aesa_set_config(AESA, &aesa_config);

	/* Beginning of a new message. */
	aesa_set_new_message(AESA);

	/* Set the cryptographic key. */
	aesa_write_key(AESA, key128);

	/* Set the initialization vector. */
	aesa_write_initvector(AESA, init_vector);

	/* Write the data to be ciphered to the input data registers. */
	aesa_write_input_data(AESA, ref_plain_text[0]);
	aesa_write_input_data(AESA, ref_plain_text[1]);
	aesa_write_input_data(AESA, ref_plain_text[2]);
	aesa_write_input_data(AESA, ref_plain_text[3]);

	/* Wait for the end of the encryption process. */
	while (false == state) {
	}

	/* check the result. */
	if ((ref_cipher_text_ofb[0] != output_data[0]) ||
			(ref_cipher_text_ofb[1] != output_data[1]) ||
			(ref_cipher_text_ofb[2] != output_data[2]) ||
			(ref_cipher_text_ofb[3] != output_data[3])) {
		printf("\r\nKO!!!\r\n");
	} else {
		printf("\r\nOK!!!\r\n");
	}

	printf("\r\n-----------------------------------\r\n");
	printf("- 128bit cryptographic key\r\n");
	printf("- OFB decipher mode\r\n");
	printf("- all counter measures\r\n");
	printf("- input of 4 32bit words\r\n");
	printf("-----------------------------------\r\n");

	state = false;

	/* Configure the AESA. */
	aesa_config.encrypt_mode = AESA_DECRYPTION;
	aesa_config.key_size = AESA_KEY_SIZE_128;
	aesa_config.dma_mode = AESA_MANUAL_MODE;
	aesa_config.opmode = AESA_OFB_MODE;
	aesa_config.cfb_size = AESA_CFB_SIZE_128;
	aesa_config.countermeasure_mask = 0xF;
	aesa_set_config(AESA, &aesa_config);

	/* Beginning of a new message. */
	aesa_set_new_message(AESA);

	/* Set the cryptographic key. */
	aesa_write_key(AESA, key128);

	/* Set the initialization vector. */
	aesa_write_initvector(AESA, init_vector);

	/* Write the data to be deciphered to the input data registers. */
	aesa_write_input_data(AESA, ref_cipher_text_ofb[0]);
	aesa_write_input_data(AESA, ref_cipher_text_ofb[1]);
	aesa_write_input_data(AESA, ref_cipher_text_ofb[2]);
	aesa_write_input_data(AESA, ref_cipher_text_ofb[3]);

	/* Wait for the end of the decryption process. */
	while (false == state) {
	}

	/* check the result. */
	if ((ref_plain_text[0] != output_data[0]) ||
			(ref_plain_text[1] != output_data[1]) ||
			(ref_plain_text[2] != output_data[2]) ||
			(ref_plain_text[3] != output_data[3])) {
		printf("\r\nKO!!!\r\n");
	} else {
		printf("\r\nOK!!!\r\n");
	}
}

/**
 * \brief CTR mode encryption and decryption test.
 */
static void ctr_mode_test(void)
{
	printf("\r\n-----------------------------------\r\n");
	printf("- 128bit cryptographic key\r\n");
	printf("- CTR cipher mode\r\n");
	printf("- all counter measures\r\n");
	printf("- input of 4 32bit words\r\n");
	printf("-----------------------------------\r\n");

	state = false;

	/* Configure the AESA. */
	aesa_config.encrypt_mode = AESA_ENCRYPTION;
	aesa_config.key_size = AESA_KEY_SIZE_128;
	aesa_config.dma_mode = AESA_MANUAL_MODE;
	aesa_config.opmode = AESA_CTR_MODE;
	aesa_config.cfb_size = AESA_CFB_SIZE_128;
	aesa_config.countermeasure_mask = 0xF;
	aesa_set_config(AESA, &aesa_config);

	/* Beginning of a new message. */
	aesa_set_new_message(AESA);

	/* Set the cryptographic key. */
	aesa_write_key(AESA, key128);

	/* Set the initialization vector. */
	aesa_write_initvector(AESA, init_vector_ctr);

	/* Write the data to be ciphered to the input data registers. */
	aesa_write_input_data(AESA, ref_plain_text[0]);
	aesa_write_input_data(AESA, ref_plain_text[1]);
	aesa_write_input_data(AESA, ref_plain_text[2]);
	aesa_write_input_data(AESA, ref_plain_text[3]);

	/* Wait for the end of the encryption process. */
	while (false == state) {
	}

	/* check the result. */
	if ((ref_cipher_text_ctr[0] != output_data[0]) ||
			(ref_cipher_text_ctr[1] != output_data[1]) ||
			(ref_cipher_text_ctr[2] != output_data[2]) ||
			(ref_cipher_text_ctr[3] != output_data[3])) {
		printf("\r\nKO!!!\r\n");
	} else {
		printf("\r\nOK!!!\r\n");
	}

	printf("\r\n-----------------------------------\r\n");
	printf("- 128bit cryptographic key\r\n");
	printf("- CTR decipher mode\r\n");
	printf("- all counter measures\r\n");
	printf("- input of 4 32bit words\r\n");
	printf("-----------------------------------\r\n");

	state = false;

	/* Configure the AESA. */
	aesa_config.encrypt_mode = AESA_DECRYPTION;
	aesa_config.key_size = AESA_KEY_SIZE_128;
	aesa_config.dma_mode = AESA_MANUAL_MODE;
	aesa_config.opmode = AESA_CTR_MODE;
	aesa_config.cfb_size = AESA_CFB_SIZE_128;
	aesa_config.countermeasure_mask = 0xF;
	aesa_set_config(AESA, &aesa_config);

	/* Beginning of a new message. */
	aesa_set_new_message(AESA);

	/* Set the cryptographic key. */
	aesa_write_key(AESA, key128);

	/* Set the initialization vector. */
	aesa_write_initvector(AESA, init_vector_ctr);

	/* Write the data to be deciphered to the input data registers. */
	aesa_write_input_data(AESA, ref_cipher_text_ctr[0]);
	aesa_write_input_data(AESA, ref_cipher_text_ctr[1]);
	aesa_write_input_data(AESA, ref_cipher_text_ctr[2]);
	aesa_write_input_data(AESA, ref_cipher_text_ctr[3]);

	/* Wait for the end of the decryption process. */
	while (false == state) {
	}

	/* check the result. */
	if ((ref_plain_text[0] != output_data[0]) ||
			(ref_plain_text[1] != output_data[1]) ||
			(ref_plain_text[2] != output_data[2]) ||
			(ref_plain_text[3] != output_data[3])) {
		printf("\r\nKO!!!\r\n");
	} else {
		printf("\r\nOK!!!\r\n");
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
			"  1: ECB mode test. \n\r"
			"  2: CBC mode test. \n\r"
			"  3: CFB128 mode test. \n\r"
			"  4: OFB mode test. \n\r"
			"  5: CTR mode test. \n\r"
			"\n\r\n\r");
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
	printf("-- AESA Example --\r\n");
	printf("-- %s\n\r", BOARD_NAME);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

	/* Enable the AESA module. */
	aesa_enable(AESA);

	/* Enable AESA interrupt. */
	aesa_enable_interrupt(AESA, AESA_INTERRUPT_INPUT_BUFFER_READY);
	NVIC_EnableIRQ(AESA_IRQn);

	/* Display menu */
	display_menu();

	while (1) {
		scanf("%c", (char *)&key);

		switch (key) {
		case 'h':
			display_menu();
			break;

		case '1':
			printf("ECB mode encryption and decryption test.\r\n");
			ecb_mode_test();
			break;

		case '2':
			printf("CBC mode encryption and decryption test.\r\n");
			cbc_mode_test();
			break;

		case '3':
			printf("CFB128 mode encryption and decryption test.\r\n");
			cfb128_mode_test();
			break;

		case '4':
			printf("OFB mode encryption and decryption test.\r\n");
			ofb_mode_test();
			break;

		case '5':
			printf("CTR mode encryption and decryption test.\r\n");
			ctr_mode_test();
			break;

		default:
			break;
		}
	}
}
