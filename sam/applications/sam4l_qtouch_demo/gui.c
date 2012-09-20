/**
 * \file
 *
 * \brief User Interface.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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
//_____  I N C L U D E S ___________________________________________________
#include "gui.h"

//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N S ______________________________________________
#define TOUCH_SENSORS_IDLE_TIME         (156u)

//_____ D E C L A R A T I O N S ____________________________________________

// SAM4L status structure for the board monitor
volatile sam4l_status_t sam4l_status = {
	.voltage_scaling = VOLTAGE_SCALING_12V,
	.sleep_mode = SLEEP_MODE_RUN,
	.cpu_freq = 12000000,
	.cpu_src = CPU_SRC_RC4M,
};

extern volatile bool lowpower_mode;
extern volatile bool pbEvent;
extern volatile uint32_t touch_sensors_idle_count;

static void prime_number_demo(void);

/**
 *  \brief Prime Number function
 */
static void prime_number_demo(void)
{
	#define PRIM_NUMS 64
	uint32_t i, d, n;
	uint32_t primes[PRIM_NUMS];


	/* Find prime numbers forever */
	primes[0] = 1;
	for (i = 1; i < PRIM_NUMS;) {
		for (n = primes[i - 1] + 1; ;n++) {
			for (d = 2; d <= n; d++) {
				if (n == d) {
					primes[i] = n;
					goto nexti;
				}
				if (n%d == 0) break;
			}
		}
		nexti:
		i++;
	}
}

/** \brief User Interface Initialization :
 *  - Initialize LCDCA Controller,
 *  - Initialize board monitor and send SAM4L status.
 */
void gui_init(void)
{
	uint8_t  string_info[8];
	uint32_t voltage_scaling, sleep_mode, cpu_freq, cpu_src;
	
	// Initialize LCDCA
	sysclk_enable_peripheral_clock(LCDCA);

	// LCDCA Controller Initialization and display SAM4L-EK DEMO texts on 
	// segment LCD
	ioport_set_pin_level(LCD_BL_GPIO, IOPORT_PIN_LEVEL_HIGH);
	lcdca_clk_init();
	lcdca_connection_init(PORT_MASK, X_BIAS);
	lcdca_blink_init(LCDCA_TIMER_FC0, true);
	lcdca_csr_init(LCDCA_TIMER_FC0,LCD_CSR_RIGHT,7,0x03);
	lcdca_enable();
	lcdca_timer_init(0,3,2,2,1);
	lcdca_enable_timer(LCDCA_TIMER_FC0);
	lcdca_enable_timer(LCDCA_TIMER_FC1);
	lcdca_enable_timer(LCDCA_TIMER_FC2);
	c42364a_set_contrast(31);
	c42364a_start_anim();
	lcdca_set_pixel(ICON_ARM);
	lcdca_scrolling_init(LCDCA_TIMER_FC1,LCDCA_SCROLL_INV, TDG_14SEG4COM, 
		FIRST_14SEG_4C,7, 7);
	lcdca_scrolling_start((const uint8_t *)"SAM4L-EK DEMO", 13); 
	while(touch_sensors_idle_count<TOUCH_SENSORS_IDLE_TIME){}
	touch_sensors_idle_count = 0;
	lcdca_scrolling_stop();

	if (sam4l_status.voltage_scaling == VOLTAGE_SCALING_18V){
		sprintf((char*)string_info, "RUN PS0");
	} else {
		sprintf((char*)string_info, "RUN PS1");
	}
	c42364a_write_alpha_packet(string_info);

	// Initialize Board Monitor and send first status
	sysclk_enable_peripheral_clock(REMOTE_TASK_USART);
	bm_init();
	voltage_scaling = sam4l_status.voltage_scaling;
	sleep_mode = sam4l_status.sleep_mode;
	cpu_freq = sam4l_status.cpu_freq;
	cpu_src = sam4l_status.cpu_src;
	bm_send_mcu_status(voltage_scaling, sleep_mode, cpu_freq, cpu_src);
	sysclk_disable_peripheral_clock(REMOTE_TASK_USART);
}

// Boolean value to detect if the QTouch button is pressed or not
bool qTouchButtonPressed = false;

/*! \brief User Interface Task
 *
 */
void gui_task(void)
{
	int touch_position;
	char  string_info[8];
	enum sleepmgr_mode  current_sleep_mode;
	uint32_t voltage_scaling, sleep_mode, cpu_freq, cpu_src;

	// Manage QTouch sensors and LCD in Full Mode Demo
	if ( lowpower_mode == false) {
		// Perform Capacitance Touch measurement on Sensors.
		touch_sensors_measure();
		// Use Rotor/Slider Position.
		if (GET_QT_SENSOR_STATE(1) || GET_QT_SENSOR_STATE(2) 
			|| GET_QT_SENSOR_STATE(3) ) {
			touch_position = GET_QT_ROTOR_SLIDER_POSITION(0);
			touch_position = 255 - touch_position;
			sprintf(string_info, "%d", touch_position);
			if (touch_position < 100) {
				string_info[2] = ' ';
				string_info[3] = ' ';
				string_info[4] = '\0';
			}
			// display slider position on segment LCD
			c42364a_write_num_packet((uint8_t const*)&string_info);
		}
		else {
			// Clear digit area
			string_info[0] = ' ';
			string_info[1] = ' ';
			string_info[2] = ' ';
			string_info[3] = ' ';
			string_info[4] = '\0';
			// display slider position on segment LCD
			c42364a_write_num_packet((uint8_t const*)&string_info);
		}
		/* Use QTouch button status and update Power Scaling value: CS0 
		    is pressed */
		if (GET_QT_SENSOR_STATE(0)&&(qTouchButtonPressed==false)) {
			qTouchButtonPressed = true;
		}
		/* Use QTouch button status and update Power Scaling value: 
		   CS0 is released  */
		else if (!GET_QT_SENSOR_STATE(0)&&(qTouchButtonPressed==true)) {
			qTouchButtonPressed = false;
			// Change to PS0
			if (sam4l_status.voltage_scaling == VOLTAGE_SCALING_12V){
				sam4l_status.voltage_scaling = VOLTAGE_SCALING_18V;
				bpm_configure_power_scaling(BPM, BPM_PS_0, 
					BPM_PSCM_CPU_NOT_HALT);
				while(!(bpm_get_status(BPM)&BPM_SR_PSOK));
			}
			else {
				// Change to PS1
				sam4l_status.voltage_scaling = VOLTAGE_SCALING_12V;
				bpm_configure_power_scaling(BPM, BPM_PS_1, 
					BPM_PSCM_CPU_NOT_HALT);
				while(!(bpm_get_status(BPM)&BPM_SR_PSOK));
			}
			// Display Power Scaling mode on segment LCD
			if (sam4l_status.voltage_scaling == VOLTAGE_SCALING_18V){
				sprintf((char*)string_info, "RUN PS0");
			} else {
				sprintf((char*)string_info, "RUN PS1");
			}
			c42364a_write_alpha_packet((uint8_t const*)&string_info);
			/* Send Board Monitor Update (power scarling, sleep mode, 
			   CPU frequency, CPU clock source) */
			sysclk_enable_peripheral_clock(REMOTE_TASK_USART);
			voltage_scaling = sam4l_status.voltage_scaling;
			sleep_mode = sam4l_status.sleep_mode;
			cpu_freq = sam4l_status.cpu_freq;
			cpu_src = sam4l_status.cpu_src;
			bm_send_mcu_status(voltage_scaling, sleep_mode, cpu_freq, cpu_src);
			sysclk_disable_peripheral_clock(REMOTE_TASK_USART);
			// Initialize touch sensing after Power Scaling mode change
			touch_sensors_deinit();
			touch_sensors_init();
		}
		// Clear QTouch library internal counter
		if(p_qt_measure_data->acq_status & TOUCH_STATUS_CHANGE)
		{
		  touch_sensors_idle_count = 0;
		}
	}
	else {
		/* Demo runs in full mode (QTouch and segment LCD are active) , 
		   runs prime number algorithm */
		prime_number_demo();
		
		/* If PB0 is pressed, switch to a deeper sleep mode: 
		   RUN->WAIT->RET->BACKUP (then go back to RUN) */
		if (pbEvent == true) {
			pbEvent = false;
			switch(sam4l_status.sleep_mode){
				case SLEEP_MODE_WAIT:
					sam4l_status.sleep_mode = SLEEP_MODE_RETENTION;
					current_sleep_mode = SLEEPMGR_RET;
				break;
				case SLEEP_MODE_RETENTION:
					sam4l_status.sleep_mode = SLEEP_MODE_BACKUP;
					current_sleep_mode = SLEEPMGR_BACKUP;
				break;
				case SLEEP_MODE_BACKUP:
					sam4l_status.sleep_mode = SLEEP_MODE_RUN;
					current_sleep_mode = SLEEPMGR_ACTIVE;
				break;
				case SLEEP_MODE_RUN:
				default:
					sam4l_status.sleep_mode = SLEEP_MODE_WAIT;
					current_sleep_mode = SLEEPMGR_WAIT;
				break;
			}
			// Send status to Board Monitor before switching in sleep mode
			sysclk_enable_peripheral_clock(REMOTE_TASK_USART);
			voltage_scaling = sam4l_status.voltage_scaling;
			sleep_mode = sam4l_status.sleep_mode;
			cpu_freq = sam4l_status.cpu_freq;
			cpu_src = sam4l_status.cpu_src;
			bm_send_mcu_status(voltage_scaling, sleep_mode, cpu_freq, cpu_src);
			sysclk_disable_peripheral_clock(REMOTE_TASK_USART);
			
			// Now we're ready to enter the selected sleep mode 
			// (RUN->WAIT->RET->BACKUP)
			sleepmgr_sleep(current_sleep_mode);
		}
	}
}
