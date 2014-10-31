/**
 * \file
 *
 * \brief SAM System Clock Driver Quick Start
 *
 * Copyright (C) 2012-2014 Atmel Corporation. All rights reserved.
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
 */
#include <asf.h>



int main(void)
{
    system_init();
    
    struct system_pinmux_config pin_conf;
    system_pinmux_get_config_defaults(&pin_conf);

    /*GCLK0 clock output*/
    pin_conf.mux_position = MUX_PB22H_GCLK_IO0;//PINMUX_PB14H_GCLK_IO0;
    pin_conf.direction    = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
    system_pinmux_pin_set_config(PIN_PB22, &pin_conf);
    
    /*GCLK1 clock output*/
    pin_conf.mux_position = MUX_PB23H_GCLK_IO1;//PINMUX_PB14H_GCLK_IO0;
    pin_conf.direction    = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
    system_pinmux_pin_set_config(PIN_PB23, &pin_conf);
    
    /*GCLK2 clock output*/
     pin_conf.mux_position = MUX_PB16H_GCLK_IO2;//PINMUX_PB14H_GCLK_IO0;
    pin_conf.direction    = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
    system_pinmux_pin_set_config(PIN_PB16, &pin_conf);
    
     /*GCLK3 clock output*/
    pin_conf.mux_position = MUX_PA17H_GCLK_IO3;//PINMUX_PB14H_GCLK_IO0;
    pin_conf.direction    = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
    system_pinmux_pin_set_config(PIN_PA17, &pin_conf);
    
   
    port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
	while (true) {

	}
}
