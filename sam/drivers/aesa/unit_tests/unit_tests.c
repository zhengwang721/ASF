/**
 * \file
 *
 * \brief Unit tests for AESA driver.
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

#include <asf.h>
#include <conf_test.h>

/**
 * \mainpage
 *
 * \section intro Introduction
 * This is the unit test application for the AESA driver.
 * It contains two test case for the AESA module:
 * - Test alarm interrupt and wakeup functions in calendar/counter mode.
 * - Test periodic interrupt and wakeup functions in counter mode.
 *
 * \section files Main Files
 * - \ref unit_tests.c
 * - \ref conf_test.h
 * - \ref conf_board.h
 * - \ref conf_clock.h
 * - \ref conf_uart_serial.h
 *
 * \section device_info Device Info
 * Only SAM4L devices can be used.
 * This example has been tested with the following setup:
 * - sam4lc4c_sam4l_ek
 *
 * \section compinfo Compilation info
 * This software was written for the GNU GCC and IAR for ARM. Other compilers
 * may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit <a href="http://www.atmel.com/">Atmel</a>.\n
 * Support and FAQ: http://support.atmel.no/
 */

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

volatile uint32_t flag = false;

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
 * \brief Test ECB mode encryption and decryption..
 *
 * \param test Current test case.
 */
static void run_ecb_mode_test(const struct test_case *test)
{
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
		flag = false;
	} else {
		flag = true;
	}

	test_assert_true(test, flag == true, "ECB mode encryption not work!");

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
		flag = false;
	} else {
		flag = true;
	}

	test_assert_true(test, flag == true, "ECB mode decryption not work!");
}

/**
 * \brief Test CBC mode encryption and decryption.
 *
 * \param test Current test case.
 */
static void run_cbc_mode_test(const struct test_case *test)
{
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
		flag = false;
	} else {
		flag = true;
	}

	test_assert_true(test, flag == true, "CBC mode encryption not work!");

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
		flag = false;
	} else {
		flag = true;
	}

	test_assert_true(test, flag == true, "CBC mode decryption not work!");
}

/**
 * \brief Test CFB128 mode encryption and decryption..
 *
 * \param test Current test case.
 */
static void run_cfb128_mode_test(const struct test_case *test)
{
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
		flag = false;
	} else {
		flag = true;
	}

	test_assert_true(test, flag == true, "CFB128 mode encryption not work!");

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
		flag = false;
	} else {
		flag = true;
	}

	test_assert_true(test, flag == true, "CFB128 mode decryption not work!");
}

/**
 * \brief Test OFB mode encryption and decryption..
 *
 * \param test Current test case.
 */
static void run_ofb_mode_test(const struct test_case *test)
{
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
		flag = false;
	} else {
		flag = true;
	}

	test_assert_true(test, flag == true, "OFB mode encryption not work!");

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
		flag = false;
	} else {
		flag = true;
	}

	test_assert_true(test, flag == true, "OFB mode decryption not work!");
}

/**
 * \brief Test CTR mode encryption and decryption..
 *
 * \param test Current test case.
 */
static void run_ctr_mode_test(const struct test_case *test)
{
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
		flag = false;
	} else {
		flag = true;
	}

	test_assert_true(test, flag == true, "CTR mode encryption not work!");

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
		flag = false;
	} else {
		flag = true;
	}

	test_assert_true(test, flag == true, "CTR mode decryption not work!");
}

/**
 * \brief Run AESA driver unit tests.
 */
int main(void)
{
	const usart_serial_options_t usart_serial_options = {
		.baudrate = CONF_TEST_BAUDRATE,
		.charlength = CONF_TEST_CHARLENGTH,
		.paritytype = CONF_TEST_PARITY,
		.stopbits = CONF_TEST_STOPBITS
	};

	sysclk_init();
	board_init();
	stdio_serial_init(CONF_TEST_USART, &usart_serial_options);

	/* Enable the AESA module. */
	aesa_enable(AESA);

	/* Enable AESA interrupt. */
	aesa_enable_interrupt(AESA, AESA_INTERRUPT_INPUT_BUFFER_READY);
	NVIC_EnableIRQ(AESA_IRQn);

	/* Define all the test cases. */
	DEFINE_TEST_CASE(ecb_mode_test, NULL, run_ecb_mode_test, NULL,
			"SAM AESA ECB mode encryption and decryption test.");
	DEFINE_TEST_CASE(cbc_mode_test, NULL, run_cbc_mode_test, NULL,
			"SAM AESA CBC mode encryption and decryption test.");
	DEFINE_TEST_CASE(cfb128_mode_test, NULL, run_cfb128_mode_test, NULL,
			"SAM AESA CFB128 mode encryption and decryption test.");
	DEFINE_TEST_CASE(ofb_mode_test, NULL, run_ofb_mode_test, NULL,
			"SAM AESA OFB mode encryption and decryption test.");
	DEFINE_TEST_CASE(ctr_mode_test, NULL, run_ctr_mode_test, NULL,
			"SAM AESA CTR mode encryption and decryption test.");

	/* Put test case addresses in an array. */
	DEFINE_TEST_ARRAY(aesa_tests) = {
		&ecb_mode_test,
		&cbc_mode_test,
		&cfb128_mode_test,
		&ofb_mode_test,
		&ctr_mode_test,
	};

	/* Define the test suite. */
	DEFINE_TEST_SUITE(aesa_suite, aesa_tests, "SAM AESA driver test suite");

	/* Run all tests in the test suite. */
	test_suite_run(&aesa_suite);

	while (1) {
		/* Busy-wait forever. */
	}
}
