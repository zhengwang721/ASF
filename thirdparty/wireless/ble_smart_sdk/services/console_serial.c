#include <asf.h>
#include <conf_uart_serial.h>
#include <string.h>
#include <stdarg.h>

#include "console_serial.h"

struct uart_module uart_instance;

volatile static bool read_complete_flag = false;

static void uart_read_complete_callback(struct uart_module *const module)
{
	read_complete_flag = true;
}


//! [setup]
void serial_console_init(void)
{

	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);

	//! [setup_config]
	struct uart_config config_uart;
	//! [setup_config]
	//! [setup_config_defaults]
	uart_get_config_defaults(&config_uart);
	//! [setup_config_defaults]

	//! [setup_change_config]
	//config_uart.baud_rate = 38400;
	config_uart.baud_rate = 115200;
	config_uart.pin_number_pad[0] = EDBG_CDC_SERCOM_PIN_PAD0;
	config_uart.pin_number_pad[1] = EDBG_CDC_SERCOM_PIN_PAD1;
	config_uart.pin_number_pad[2] = EDBG_CDC_SERCOM_PIN_PAD2;
	config_uart.pin_number_pad[3] = EDBG_CDC_SERCOM_PIN_PAD3;
	
	config_uart.pinmux_sel_pad[0] = EDBG_CDC_SERCOM_MUX_PAD0;
	config_uart.pinmux_sel_pad[1] = EDBG_CDC_SERCOM_MUX_PAD1;
	config_uart.pinmux_sel_pad[2] = EDBG_CDC_SERCOM_MUX_PAD2;
	config_uart.pinmux_sel_pad[3] = EDBG_CDC_SERCOM_MUX_PAD3;
	//! [setup_change_config]

	stdio_serial_init(&uart_instance, CONF_STDIO_USART_MODULE, &config_uart);

	//! [setup_set_config]
	/*
	while (uart_init(&uart_instance,
	EDBG_CDC_MODULE, &config_uart) != STATUS_OK) {
	}
	*/
	//! [setup_set_config]
}
//! [setup]

/*
int getchar_b11()
{

	uint8_t string_input[1];
	read_complete_flag = false;
	
	uart_register_callback(&uart_instance, uart_read_complete_callback,UART_RX_COMPLETE);
	uart_enable_callback(&uart_instance, UART_RX_COMPLETE);
	uart_read_buffer_job(&uart_instance, string_input, sizeof(string_input));
	
	while (!read_complete_flag);
	
	return string_input[0];
	
}
*/
/*
int printf_b11( const char* format, ... )
{
	
	//uart_write_buffer_wait(&uart_instance, format, strlen(format));
	
	
	
	
	
	
	//chris.choi : seems like below codes have no error but if i use below code, then pxp scanning is not working in B0. I don't know why...
	int size;
	
	char temp[255] = "\0";
	va_list arg;
	va_start(arg,format);
	
	size = vsprintf(temp,format,arg);
	va_end(arg);
	
	uart_write_buffer_wait(&uart_instance, temp, size);
	
	
}
*/


