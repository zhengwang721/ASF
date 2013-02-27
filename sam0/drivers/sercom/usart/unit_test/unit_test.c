#include <asf.h>
#include <stdio_serial.h>

/* USART for STDIO */
#define CONF_TEST_USART    SERCOM4

struct usart_module edbg_mod;



static void run_transfer_single_8bit_char_test(const struct test_case *test)
{
	uint8_t tx_char = 0xF;
	uint8_t rx_char;


	test_assert_true(test, 2==2, "jattjatt");
	
}

static void run_transfer_single_9bit_char_test(const struct test_case *test)
{
	uint16_t tx_char = 0x1F;
	uint16_t rx_char;
	
}

static void test_system_init(void)
{
	/* Variables */
	struct usart_config usart_conf;

	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_settings = USART_RX_3_TX_2_XCK_3;
	usart_conf.pinout_pad3 = EDBG_CDC_RX_PINMUX;
	usart_conf.pinout_pad2 = EDBG_CDC_TX_PINMUX;
	usart_conf.baudrate = 38400;
//	usart_init(&edbg_mod, CONF_TEST_USART, &usart_conf);

	stdio_serial_init(&edbg_mod, CONF_TEST_USART, &usart_conf);
	usart_enable(&edbg_mod);
	/* Enable transmitter */
	usart_enable_transceiver(&edbg_mod, USART_TRANSCEIVER_TX);
	usart_enable_transceiver(&edbg_mod, USART_TRANSCEIVER_RX);
}
void write_string(struct usart_module *const mod, uint8_t *string)
{
	do {
		//while (usart_write_wait(mod, *string) != STATUS_OK) {
		usart_write_wait(mod, *string);
		//}
	} while (*(++string) != 0);
}
int main(void)
{
	system_init();
	system_clock_source_write_calibration(SYSTEM_CLOCK_SOURCE_OSC8M, 1081, 1);
	test_system_init();
	
	/* Variables */

	/* Define Test Cases */
	DEFINE_TEST_CASE(transfer_single_8bit_char_test, NULL,
			run_transfer_single_8bit_char_test, NULL,
			"Transfer single 8-bit character result:");
	DEFINE_TEST_CASE(transfer_single_9bit_char_test, NULL,
			run_transfer_single_9bit_char_test, NULL,
			"Transfer single 9-bit character result:");

	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(usart_tests) = {
			&transfer_single_8bit_char_test,
			&transfer_single_9bit_char_test,
			};

	/* Define the test suite */
	DEFINE_TEST_SUITE(usart_suite, usart_tests,
			"SAM0 USART driver test suite");

	/* Run all tests in the suite*/
	test_suite_run(&usart_suite);
#if 0
#endif
	while(1) {
		/* Intentionally left empty */
	}

}
