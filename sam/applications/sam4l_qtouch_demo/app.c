/**
 * \file
 *
 * \brief Application layer for low power and full features demo initialization.
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
 
#include "app.h"

// By default, the Full Demo Mode is on, used in main.c
volatile bool lowpower_mode = false;
// Internal counter for the QTouch library
volatile uint32_t touch_sensors_idle_count = 0u;
// Push button PB0 event
volatile bool pbEvent = false; 

// External reference to SAM4L Status
extern volatile sam4l_status_t sam4l_status;
static void wdt_clear(void);

/**
 *  \brief Asynchronous timer (ASF) handler for the QTouch acquisition.
 *  and clear Watchdog counter - generates interrupt every 100ms
 */
void AST_PER_Handler (void)
{
	touch_sensors_update_time();
	touch_sensors_idle_count++;
	AST->AST_SCR = 0x00010000;
	wdt_clear();
}

/**
 *  \brief External interrupt handler, used by PB0 push button
 */
void EIC_5_Handler(void)
{
	sysclk_enable_peripheral_clock(EIC);
	
	if(eic_is_interrupt_line_pending(EIC,GPIO_PUSH_BUTTON_EIC_LINE))
	{
		eic_clear_interrupt_line(EIC,GPIO_PUSH_BUTTON_EIC_LINE);
		pbEvent = true;
	}
	sysclk_disable_peripheral_clock(EIC);
}

/**
 *  \brief Enable external interrupt to wake up the MCU from sleep modes
 */
static void eic_enable_wkup(void)
{
	// Structure holding the configuration parameters
	// of the EIC module.
	eic_options_t eic_options;

	// Enable level-triggered interrupt.
	eic_options.eic_mode   = EIC_MODE_EDGE_TRIGGERED;
	// Interrupt will trigger on low-level.
	eic_options.eic_level  = EIC_LEVEL_LOW_LEVEL;
	// Edge on falling edge
	eic_options.eic_edge = EIC_EDGE_FALLING_EDGE;
	// Enable filter.
	eic_options.eic_filter = EIC_FILTER_DISABLED;
	// For Wake Up mode, initialize in asynchronous mode
	eic_options.eic_async  = EIC_ASYNCH_MODE;
	// Choose External Interrupt Controller Line
	eic_options.eic_line   = 5;

	// Init the EIC controller with the options
	eic_init(EIC, &eic_options,1);

	// Enable External Interrupt Controller Line
	eic_enable_line(EIC, eic_options.eic_line);

	// Enable the interrupt for each EIC line.
	eic_enable_interrupt_lines(EIC, (1<<eic_options.eic_line));

	// Register EIC interrupt to the NVIC
	NVIC_ClearPendingIRQ(EIC_5_IRQn);
	NVIC_SetPriority(    EIC_5_IRQn, 1);
	NVIC_EnableIRQ(      EIC_5_IRQn);

	//EIC can wake the device from backup mode
	BPM->BPM_BKUPWEN |= (1<<BPM_BKUPWEN_EIC) | (1<<BPM_BKUPWEN_AST);
	//EIC can wake the device from backup mode
	BPM->BPM_BKUPPMUX = (1<<GPIO_PUSH_BUTTON_EIC_LINE);
	BPM->BPM_IORET = 0;
	//Retain I/O lines after wakeup from backup
	BPM->BPM_IORET = 1; 
}

/**
 *  \brief Set Control register function for WDT
 */
static void wdt_set_ctrl(uint32_t ctrl)
{
	WDT->WDT_CTRL = ctrl | WDT_CTRL_KEY((uint32_t)0x55);
	WDT->WDT_CTRL = ctrl | WDT_CTRL_KEY((uint32_t)0xAA);
}

/**
 *  \brief Clear WDT
 */
static void wdt_clear(void)
{
	while (!(WDT->WDT_SR & WDT_SR_CLEARED));
	WDT->WDT_CTRL = WDT_CLR_WDTCLR | WDT_CLR_KEY((uint32_t)0x55);
	WDT->WDT_CTRL = WDT_CLR_WDTCLR | WDT_CLR_KEY((uint32_t)0xAA);
}

/**
 *  \brief Enable WDT
 */
static void enable_wdt(void)
{
	wdt_set_ctrl(WDT_CTRL_EN | WDT_CTRL_CEN | WDT_CTRL_PSEL(17) );
}


/**
 *  \brief Application initialization: at startup the application run in full 
 *  demo mode (all features on, includes QTouch and segment LCD). Initialize the 
 *  board IO configuration, clocks, QTouch library, xternal interrupts, NVIC and 
 *  GUI SAM4L is running at 12 MHz from internal RCFAST (configured at 12MHz).
 */
void app_init(void)
{
	// Initialize board features
	board_init();

	// Initialize clock
	sysclk_init();

	// Initialize EIC Controller
	sysclk_enable_peripheral_clock(EIC);
	eic_enable_wkup();
	sysclk_disable_peripheral_clock(EIC);

	// Initialize WDT Controller
	sysclk_enable_peripheral_clock(WDT);
	enable_wdt();

	// Enable Osc32
	osc_enable(OSC_ID_OSC32);
	while(!osc_is_ready(OSC_ID_OSC32));

	// Initialize AST Controller
	ast_init_counter(AST, AST_OSC_32KHZ, 1, 0);
	ast_set_periodic0_value(AST, 0x09);
	ast_enable_periodic0(AST);
	ast_enable_periodic_interrupt(AST, 0);
	ast_enable(AST);
	// Set up interrupt handler
	NVIC_ClearPendingIRQ(AST_PER_IRQn);
	NVIC_SetPriority(AST_PER_IRQn,0);
	NVIC_EnableIRQ(AST_PER_IRQn);

	//CATB & Related clocks - for QTouch Sensors
	sysclk_enable_peripheral_clock(CATB);
	sysclk_enable_peripheral_clock(PDCA);

	// Initialize QTouch Library.
	touch_sensors_init();
	
	// Initialize GUI
	gui_init();
	pbEvent = false;

}

/**
 *  \brief Initialize the app in low power: now PB0 push button has been pressed 
 *  once, the application switches in low power mode: Stop LCD controller, stop 
 *  LCD backlight, stop QTouch acquisition, switch SAM4L in power scaling PS1 
 *  mode. SAM4L is in RUN mode.
 */
void app_init_lowpower(void)
{
	uint32_t voltage_scaling, sleep_mode, cpu_freq, cpu_src;
	
	// The application is running in low power mode
	lowpower_mode = true;
	
	// Clear LCD
	LCDCA->LCDCA_CFG = LCDCA_CFG_BLANK;
	
	// Stop LCD
	LCDCA->LCDCA_CR = 0x1;

	// Clear QTouch Initialization
	touch_sensors_deinit();

	// Initialize board features
	board_init();

	// Clear LCD backlight
	ioport_set_pin_level(LCD_BL_GPIO, IOPORT_PIN_LEVEL_LOW);

	// Disable the peripheral that we do not use anymore
	sysclk_disable_peripheral_clock(CATB);
	sysclk_disable_peripheral_clock(PDCA);
	sysclk_disable_peripheral_clock(LCDCA);

	// Switch in PS1 mode
	bpm_configure_power_scaling(BPM, BPM_PS_1, BPM_PSCM_CPU_NOT_HALT);
	while(!(bpm_get_status(BPM)&BPM_SR_PSOK));

	// Initialize structure for the board monitor message over USART
	sam4l_status.voltage_scaling = VOLTAGE_SCALING_12V,
	sam4l_status.sleep_mode = SLEEP_MODE_RUN,
	sam4l_status.cpu_freq = 12000000,
	sam4l_status.cpu_src = CPU_SRC_RC4M,

	// The USART needs to be enabled when sending the message to the board monitor
	sysclk_enable_peripheral_clock(REMOTE_TASK_USART);
	voltage_scaling = sam4l_status.voltage_scaling;
	sleep_mode = sam4l_status.sleep_mode;
	cpu_freq = sam4l_status.cpu_freq;
	cpu_src = sam4l_status.cpu_src;
	bm_send_mcu_status(voltage_scaling, sleep_mode, cpu_freq, cpu_src);
	sysclk_disable_peripheral_clock(REMOTE_TASK_USART);

	// Clear the push button event now that it is processed
	pbEvent = false;
}
