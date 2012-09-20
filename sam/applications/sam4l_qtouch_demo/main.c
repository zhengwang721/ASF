/**
 * \file
 *
 * \brief Low Power and QTouch Demo for SAM4L.
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

#include "asf.h"
#include "app.h"
#include "gui.h"

// Event for the PB0 push button
extern volatile bool pbEvent;

/**
 *  \brief Lower power and QTouch Demo for SAM4L entry point.
 *  \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	/*
	QTouch library:
	Use touch_config_sam4l.h file to configure Sensor Pins, number of Sensors
	and Sensor Global configuration information.

	Use touch.c touch_sensors_config() function to set Sensor specifc 
	configuration data such as the Sensor Threshold setting.
	*/

	/* 
	At startup the application run in full demo mode (all features on, includes 
	QTouch and segment LCD). Initialize the board IO configuration, clocks, 
	QTouch library, External interrupts, NVIC and GUI SAM4L is running at 12 MHz 
	from internal RCFAST (configured at 12MHz).
	*/
	app_init();

	// Stay in full demo mode until PB0 button is pressed
	while (pbEvent == false) {
		/* 
		Capture QTouch inputs (sliders and CS0 QTouch button): displays the 
		slider value (0..255) to the segment LCD, CS0 will change the SAM4L 
		Power Scaling mode (PS0 or PS1). 
		*/
		gui_task();
	}

	/* 
	Now PB0 push button has been pressed once, the application switches in low 
	power mode: Stop LCD controller, stop LCD backlight, stop QTouch 
	acquisition, switch SAM4L in power scaling PS1 mode.
	SAM4L is in RUN mode.
	*/
	app_init_lowpower();

	while(1u){
		/* 
		Run in low power mode: if PB0 is pressed, the SAM4L will enter one of 
		the sleep modes (from RUN to WAIT to RET to BACKUP, then restart to RUN).
		For each sleep mode transition, the SAM4L is sending the information for 
		the board monitor (over the USART). The current SAM4L sleep mode is 
		displayed by the board monitor on the OLED display.
		*/
		gui_task();
	}

}// end main function
