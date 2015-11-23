/**
 * \file
 *
 * \brief BNO055 extension board example for SAME70.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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

#include "asf.h"
#include "conf_board.h"
#include "conf_bno055.h"
#include "bno055_porting_i2c.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

#define STRING_EOL    "\r"
#define STRING_HEADER "-- MMA7341L  Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL


/* Sleep State Macros */
#define SLEEP_STATE_SLEEP		UINT8_C(1)
#define SLEEP_STATE_AWAKE		UINT8_C(0)

/* name RGB LED Macros */
#define RGB_LED_R				PIO_PB2_IDX
#define RGB_LED_G				PIO_PA0_IDX
#define RGB_LED_B				PIO_PC30_IDX
#define RGB_LED_G_ON			false
#define RGB_LED_G_OFF			true
#define RGB_LED_B_VALUE			(0xFFFF - ((bno055_euler_data.h) * 0xFFFF / 5759))
#define RBG_LED_B_OFF			UINT16_C(0xFFFF)
#define RGB_LED_R_VALUE			(0xFFFF - ((bno055_euler_data.p) * 0xFFFF / 5759))
#define RBG_LED_R_OFF			UINT16_C(0xFFFF)

/*----------------------------------------------------------------------------*
 *  struct bno055_t parameters can be accessed by using BNO055
 *	BNO055_t having the following parameters
 *	Bus write function pointer: BNO055_WR_FUNC_PTR
 *	Bus read function pointer: BNO055_RD_FUNC_PTR
 *	Burst read function pointer: BNO055_BRD_FUNC_PTR
 *	Delay function pointer: delay_msec
 *	I2C address: dev_addr
 *	Chip id of the sensor: chip_id
*---------------------------------------------------------------------------*/
struct bno055_t bno055;

/*! holds BNO055 Euler data which consists of three Euler angles (Heading, Roll and Pitch) */
struct bno055_euler_t			bno055_euler_data;
/*! Holds sleep state of the system */
uint8_t sleep_state;

/*! the flag is set by the external interrupt callback function */
volatile bool extint_callback_detect_flag;
volatile bool read_sensor_data = false;
uint32_t systick_count = 0;

/************************************************************************/
/**\name Function Prototype Declaration                                 */
/************************************************************************/
void extint_configure(void);
//void extint_configure_callbacks(void);
void extint_detection_callback(void);
void (*extint_handler_function_ptr)(void);
void extint_initialize(void (*extint_handler_function)(void));
static void bno055_interrupt_handler_no_motion(void);
static void bno055_interrupt_handler_any_motion(void);



static void bno055_interrupt_handler(void)
{
	uint8_t accel_no_motion_status = 0;
	uint8_t accel_any_motion_status = 0;
	uint8_t gyro_any_motion_status = 0;
	
	bno055_get_intr_stat_accel_no_motion(&accel_no_motion_status);
	bno055_get_intr_stat_accel_any_motion(&accel_any_motion_status);
	bno055_get_intr_stat_gyro_any_motion(&gyro_any_motion_status);
	
	switch (sleep_state)
	{
		case SLEEP_STATE_AWAKE:
		if (accel_no_motion_status)
		{
			sleep_state = SLEEP_STATE_SLEEP;
			bno055_interrupt_handler_no_motion();
		}
		break;
		case SLEEP_STATE_SLEEP:
		if (accel_any_motion_status || gyro_any_motion_status)
		{
			sleep_state = SLEEP_STATE_AWAKE;
			bno055_interrupt_handler_any_motion();
		}
		break;
		default:
		break;
	}
	
	bno055_set_intr_rst(ENABLED);
}

void extint_configure(void)
{
#if 0
	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);
	
	config_extint_chan.gpio_pin = EXT1_IRQ_PIN;
	config_extint_chan.gpio_pin_mux = EXT1_IRQ_PINMUX;
	config_extint_chan.gpio_pin_pull = EXTINT_PULL_DOWN;
	config_extint_chan.detection_criteria = EXTINT_DETECT_RISING;
	
	extint_chan_set_config(EXT1_IRQ_INPUT, &config_extint_chan);
#endif
}

#if 0
void extint_configure_callbacks(void)
{
	extint_register_callback(extint_detection_callback, EXT1_IRQ_INPUT, EXTINT_CALLBACK_TYPE_DETECT);
	extint_callback_detect_flag = false;
	extint_chan_enable_callback(EXT1_IRQ_INPUT, EXTINT_CALLBACK_TYPE_DETECT);
}
#endif

void extint_detection_callback(void)
{
	extint_callback_detect_flag = true;
	extint_handler_function_ptr();
}

void extint_initialize(void (*extint_handler_function)(void))
{
	extint_handler_function_ptr = extint_handler_function;
	
	/* Enable the peripheral clock for the BNO055 extension board interrupt pin. */
	pmc_enable_periph_clk(PIN_BNO055_EXT_INIERRUPT_ID);
	/* Configure PIOs as input pins. */
	pio_configure(PIN_BNO055_EXT_INIERRUPT_PIO, PIN_BNO055_EXT_INIERRUPT_TYPE, 
					PIN_BNO055_EXT_INIERRUPT_MASK, PIN_BNO055_EXT_INIERRUPT_ATTR);
	/* Initialize PIO interrupt handler, interrupt on rising edge. */
	pio_handler_set(PIN_BNO055_EXT_INIERRUPT_PIO, PIN_BNO055_EXT_INIERRUPT_ID, PIN_BNO055_EXT_INIERRUPT_MASK,
					PIN_BNO055_EXT_INIERRUPT_ATTR, bno055_interrupt_handler);
	/* Initialize and enable push button (PIO) interrupt. */
	pio_handler_set_priority(PIN_BNO055_EXT_INIERRUPT_PIO, PIOD_IRQn, 0);
}

static void bno055_gpio_config(void)
{
	//ioport_set_pin_dir(PIO_PB3_IDX, IOPORT_DIR_OUTPUT);
	//ioport_set_pin_dir(PIO_PB2_IDX, IOPORT_DIR_OUTPUT);
	//ioport_set_pin_dir(PIO_PC17_IDX, IOPORT_DIR_OUTPUT);
	//ioport_set_pin_dir(PIO_PD25_IDX, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode(PIO_PB3_IDX, IOPORT_DIR_OUTPUT | IOPORT_MODE_PULLUP | IOPORT_MODE_MUX_MASK);
	ioport_set_pin_mode(PIO_PB2_IDX, IOPORT_DIR_OUTPUT | IOPORT_MODE_PULLUP | IOPORT_MODE_MUX_MASK);
	ioport_set_pin_mode(PIO_PC17_IDX, IOPORT_DIR_OUTPUT | IOPORT_MODE_PULLUP | IOPORT_MODE_MUX_MASK);
	ioport_set_pin_mode(PIO_PD25_IDX, IOPORT_DIR_OUTPUT | IOPORT_MODE_PULLUP | IOPORT_MODE_MUX_MASK);
	
	//pio_configure(PIO_PB3_IDX, PIO_OUTPUT_0, PIO_PD28, PIO_PULLUP);
	//pio_configure(PIO_PB2_IDX, PIO_OUTPUT_0, PIO_PB2, PIO_PULLUP);
	//pio_configure(PIO_PC17_IDX, PIO_OUTPUT_0, PIO_PC17, PIO_PULLUP);
	//pio_configure(PIO_PD25_IDX, PIO_OUTPUT_0, PIO_PD25, PIO_PULLUP);
}

/**
 *  \brief Handler for System Tick interrupt.
 */
void SysTick_Handler(void)
{
	systick_count ++;
	if(systick_count == 500) {
		read_sensor_data = true;
		systick_count = 0;
	}
}

/**
 *  \brief Configure UART console.
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

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

static void sensor_init(void)
{
	I2C_routine(&bno055);
	bno055_init(&bno055);
	bno055_set_power_mode(POWER_MODE_LOWPOWER);
	bno055_set_intr_rst(ENABLED);
	
	/* Configure sensor interrupts */
	bno055_set_gyro_any_motion_axis_enable(0, ENABLED);
	bno055_set_gyro_any_motion_axis_enable(1, ENABLED);
	bno055_set_gyro_any_motion_axis_enable(2, ENABLED);
	
	bno055_set_intr_gyro_any_motion(ENABLED);
	bno055_set_intr_mask_gyro_any_motion(ENABLED);
	
	bno055_set_accel_any_motion_no_motion_axis_enable(0, ENABLED);
	bno055_set_accel_any_motion_no_motion_axis_enable(1, ENABLED);
	bno055_set_accel_any_motion_no_motion_axis_enable(2, ENABLED);
	
	bno055_set_accel_any_motion_thres(5);
	bno055_set_accel_any_motion_durn(1);
	bno055_set_intr_accel_any_motion(ENABLED);
	bno055_set_intr_mask_accel_any_motion(ENABLED);
	
	bno055_set_accel_slow_no_motion_thres(5);
	bno055_set_accel_slow_no_motion_durn(1);
	bno055_set_intr_accel_no_motion(ENABLED);
	bno055_set_intr_mask_accel_no_motion(ENABLED);
	
	bno055_set_operation_mode(OPERATION_MODE_NDOF);
}

/*!
* @brief	Reads output data of the sensor and sends them data via USART.
*
* This function reads output data of BNO055 (Three Euler angles) using
* sensor API functions and sends sensor data in ASCII Format via USART.
*
* @param[in]	NULL
*
* @param[out]	NULL
*
* @return		NULL
*
*/
static void sensors_data_print(void)
{
	uint8_t uart_tx_buf[49] = {0};
	uint8_t accel_calib = 0;
	uint8_t gyro_calib = 0;
	uint8_t mag_calib = 0;
	
	bno055_read_euler_hrp(&bno055_euler_data);
	bno055_get_accel_calib_stat(&accel_calib);
	bno055_get_gyro_calib_stat(&gyro_calib);
	bno055_get_mag_calib_stat(&mag_calib);
	
	printf("H: %4d\tR: %4d\tP: %4d\tCalib: %d%d%d\r\n", (bno055_euler_data.h)/16, 
			(bno055_euler_data.r)/16, (bno055_euler_data.p)/16, accel_calib, gyro_calib, mag_calib);
}

/*!
* @brief	No motion interrupt handler.
*
* This function is called when a no-motion interrupt is triggered
* by the accelerometer in BNO055,	turns the LED color to green,
* stops data stream and sends a message to the terminal window.
*/
static void bno055_interrupt_handler_no_motion(void)
{
	const uint8_t str_sleeping_message[] = "\r\nShake me to wake me!\r\n\r\n";
	
	ioport_set_pin_level(RGB_LED_G,  RGB_LED_G_ON);
	//RGB_LED_B_R_UPDATE(RBG_LED_B_OFF, RBG_LED_R_OFF);
	printf("%s", str_sleeping_message);
}

/*!
* @brief	Any motion interrupt handler.
*
* This function is called when an any-motion interrupt is triggered
* by the accelerometer or gyroscope in BNO055, turns off the green light,
* sends a message to the terminal window and starts data stream.
*/
static void bno055_interrupt_handler_any_motion(void)
{
	const uint8_t str_wakingup_message[] = "\r\nWell done!\r\n\r\n";
	
	printf("%s", str_wakingup_message);
	//RGB_LED_B_R_UPDATE(RGB_LED_B_VALUE, RGB_LED_R_VALUE);
	ioport_set_pin_level(RGB_LED_G,  RGB_LED_G_OFF);
}

int main(void)
{
	/* Initialize the system */
	sysclk_init();
	board_init();

	/* Initialize the console uart */
	configure_console();
	
	/* Systick configuration. */
	puts("Configure systick to get 1ms tick period.\r");
	if (SysTick_Config(sysclk_get_cpu_hz() / 1000)) {
		puts("-F- Systick configuration error\r");
	}

	puts(STRING_HEADER);
	
	bno055_gpio_config();
	BNO055_I2C_bus_init();
	
	/* (BNO055 external interrupt) */
	extint_initialize(&bno055_interrupt_handler);
	cpu_irq_enable();
	
	sensor_init();
	
	while (1) {
		if(read_sensor_data) {
			read_sensor_data = false;
			sensors_data_print();
		}
		
		/* Check sensor reset button (SW0 button on the main board) */
		if (ioport_get_pin_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			/* turn LED0 on */
			ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
			/* Reset BNO055 */
			sensor_init();
		} else {
			/* turn LED off if the button is not pressed */
			ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
		}
	}
}


/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond