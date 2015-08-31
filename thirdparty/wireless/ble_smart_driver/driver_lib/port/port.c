/**
 * \file
 *
 * \brief SAMB11 Port driver
 *
 * Copyright (C) 2012-2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <stdint.h>
#include <string.h>
#include "CMSDK_CM0.h"
#include "port.h"
#include "gpio.h"
#include "common.h"

portint_callback_t	port_int_callback[PORTPIN_MAX];

void port_get_config_defaults(
		struct port_config *const config)
{
	/* Default configuration values */
	config->direction  = PORT_PIN_DIR_INPUT;
	config->input_pull = PORT_PIN_PULL_UP;
	config->powersave  = false;
}

enum port_status_code port_pin_set_config(
		const uint8_t gpio_pin,
		const struct port_config *const config)
{
	uint32_t regaddr,regval;
	enum port_status_code status = PORT_STATUS_OK;
	if(config == NULL) {
		//Following GPIO's should never be modified by user.
		//GPIO_0 & GPIO_1 are used for SWD
		//GPIO_14 is used by firmware for coex.
		if((gpio_pin == LPGPIO_0)
			 || (gpio_pin == LPGPIO_1)
			 || (gpio_pin == LPGPIO_14)) {
				 status = PORT_STATUS_ERR_INVALID_ARG;
		}
		else {
			port_list[gpio_pin].bit.available = 1;
			port_list[gpio_pin].bit.configured = 0;
		}
	}
	else if(!port_list[gpio_pin].bit.available) {
		status = PORT_STATUS_RESOURCE_NOT_AVAILABLE;
	}
	else {
		port_list[gpio_pin].bit.available = 0;
		port_list[gpio_pin].bit.configured = 1;
		if( gpio_pin <= 7) {
			regaddr = 0x4000b044;
		}
		else if(gpio_pin <= 15) {
			regaddr = 0x4000b048;
		}
		else if(gpio_pin <= 23) {
			regaddr = 0x4000b04c;
		}
		regval = REG_RD(regaddr);
		regval &= ~(7 << ((gpio_pin%8)*4));
		REG_WR(regaddr,regval);
		if(config->direction == PORT_PIN_DIR_INPUT) {
			if(gpio_pin < 16) {
				CMSDK_GPIO0->OUTENABLECLR |= (1 << gpio_pin);
			}
			else {
				CMSDK_GPIO1->OUTENABLECLR |= (1 << (gpio_pin % 16));
			}
			// pull_enable.
			switch(config->input_pull)
			{
				case PORT_PIN_PULL_NONE:
				{
					regval = REG_RD(0x4000B050);
					regval |= (1 << gpio_pin);
					REG_WR(0x4000B050,regval);
				}
					break;
				case PORT_PIN_PULL_UP:
				{
					regval = REG_RD(0x4000B050);
					regval &= ~(1 << gpio_pin);
					REG_WR(0x4000B050,regval);
				}
					break;
#ifdef CHIPVERSION_B0
				case PORT_PIN_PULL_DOWN:
				{
					// Set R-Type
					regval = REG_RD(0x4000B054);
					regval |= (1 << gpio_pin);
					REG_WR(0x4000B054,regval);
					// Sete REN
					regval = REG_RD(0x4000B050);
					regval &= ~(1 << gpio_pin);
					REG_WR(0x4000B050,regval);
				}
					break;
#endif	//CHIPVERSION_B0
				default:
				{
					port_list[gpio_pin].bit.available = 1;
					port_list[gpio_pin].bit.configured = 0;
					status = PORT_STATUS_ERR_INVALID_ARG;
				}
					break;
			}
		}
		else if(config->direction == PORT_PIN_DIR_OUTPUT) {
			if(gpio_pin < 16) {
				CMSDK_GPIO0->OUTENABLESET |= (1 << gpio_pin);
				//CMSDK_GPIO0->DATAOUT |= (1 << gpio_pin);
			}
			else {
				CMSDK_GPIO1->OUTENABLESET |= (1 << (gpio_pin % 16));
				//CMSDK_GPIO1->DATAOUT |= (1 << (gpio_pin % 16));
			}
		}
	}
	return status;
}

bool port_pin_get_input_level(
		const uint8_t gpio_pin)
{
	uint32_t regval;
	if(!port_list[gpio_pin].bit.configured)
		return 0;
	if(gpio_pin < 16) {
		regval = CMSDK_GPIO0->DATA;
		return (regval & (1 << gpio_pin));
	}
	else {
		regval = CMSDK_GPIO1->DATA;
		return (regval & (1 << (gpio_pin % 16)));
	}
}

bool port_pin_get_output_level(
		const uint8_t gpio_pin)
{
	uint32_t regval;
	if(!port_list[gpio_pin].bit.configured)
		return 0;
	if(gpio_pin < 16) {
		regval = CMSDK_GPIO0->DATAOUT;
		return (regval & (1 << gpio_pin));
	}
	else {
		regval = CMSDK_GPIO1->DATAOUT;
		return (regval & (1 << (gpio_pin % 16)));
	}
}

void port_pin_set_output_level(
		const uint8_t gpio_pin,
		const bool level)
{
	if(!port_list[gpio_pin].bit.configured)
		return;
	if(gpio_pin < 16) {
		if(level)
			CMSDK_GPIO0->DATAOUT |= (1 << gpio_pin);
		else
			CMSDK_GPIO0->DATAOUT &= ~(1 << gpio_pin);
	}
	else {
		if(level)
			CMSDK_GPIO1->DATAOUT |= (1 << (gpio_pin % 16));
		else
			CMSDK_GPIO1->DATAOUT &= ~(1 << (gpio_pin % 16));
	}
}

void port_pin_toggle_output_level(
		const uint8_t gpio_pin)
{
	if(!port_list[gpio_pin].bit.configured)
		return;
	if(gpio_pin < 16) {
		CMSDK_GPIO0->DATAOUT ^= (1 << gpio_pin);
	}
	else {
		CMSDK_GPIO1->DATAOUT ^= (1 << (gpio_pin % 16));
	}
}

void gpio1_combined_isr_handler(void)
{
	uint8_t index = 0;
	portint_callback_t callback;
	uint8_t port = LPGPIO_16;
	uint32_t intstatus = CMSDK_GPIO1->INTSTATUS;
	do {
#ifdef CHIPVERSION_B0
		intstatus &= 0x1FF;
#else
		intstatus &= 0x1F;
#endif
		index = 0;
		if(intstatus != 0) {
			port = (intstatus & ~(intstatus - 1));
			while(!(port == 1)) {
				port = port >> 1;
				index++;
			}
			if(port_int_callback[index+LPGPIO_16] != NULL) {
				callback = port_int_callback[index+LPGPIO_16];
				callback();
			}
			CMSDK_GPIO1->INTCLEAR = (1 << index);
		}
		else {
			CMSDK_GPIO1->INTCLEAR = CMSDK_GPIO1->INTSTATUS;
		}
		intstatus = CMSDK_GPIO1->INTSTATUS;
	}while(intstatus != 0);
}

void gpio0_combined_isr_handler(void)
{
	uint8_t port = LPGPIO_0;
	uint8_t index = 0;
	portint_callback_t callback;
	uint32_t intstatus = CMSDK_GPIO0->INTSTATUS;
	do {
		intstatus &= 0xFFFF;
		if(intstatus != 0) {
			index = 0;
			port = (intstatus & ~(intstatus - 1));
			while(!(port == 1)) {
				port = port >> 1;
				index++;
			}
			if(port_int_callback[index] != NULL) {
				callback = port_int_callback[index];
				callback();
			}
			CMSDK_GPIO0->INTCLEAR = (1 << index);
		}
		else {
			CMSDK_GPIO0->INTCLEAR = CMSDK_GPIO0->INTSTATUS;
		}
		intstatus = CMSDK_GPIO0->INTSTATUS;
	}while(intstatus != 0);
}

#ifdef CHIPVERSION_B0
void PORT1_COMB_Handler(void)
{
	if(CMSDK_GPIO1->INTSTATUS & ((1<<15) | (1<<14) | (1<<13))) {
		handle_ext_wakeup_isr();
		
		// clear specific int pin status that caused the Interrupt
		CMSDK_GPIO1->INTCLEAR |= CMSDK_GPIO1->INTSTATUS & ((1<<15) | (1<<14) | (1<<13));
		NVIC_ClearPendingIRQ(PORT1_COMB_IRQn);
	}
	else
	{
		gpio1_combined_isr_handler();
	}
}

#endif	//CHIPVERSION_B0

void portint_get_config_defaults(
		struct port_int_chan_conf *const config)
{
	if(config != NULL){
		config->port_pin 	= PORTPIN_0;
		config->pin_mux		= PINMUX_VAL_0;
		config->port_pin_pull = PORT_PIN_PULL_UP;
		config->wake_if_sleeping = false;
		config->detection_type = PORT_INT_DETECT_FALLING;
	}
}

enum port_status_code port_pin_set_int_config(
		const uint8_t gpio_pin,
		const struct port_int_chan_conf *const config)
{
	uint32_t regaddr,regval;
	CMSDK_GPIO_TypeDef *gpio_core;
	enum port_status_code status = PORT_STATUS_OK;
	if(gpio_pin < 16)
		gpio_core = CMSDK_GPIO0;
	else
		gpio_core = CMSDK_GPIO1;
	if(config == NULL) {
		//Following GPIO's should never be modified by user.
		//GPIO_0 & GPIO_1 are used for SWD
		//GPIO_14 is used by firmware for coex.
		if((gpio_pin == LPGPIO_0)
			 || (gpio_pin == LPGPIO_1)
			 || (gpio_pin == LPGPIO_14)) {
				 status = PORT_STATUS_ERR_INVALID_ARG;
		}
		else {
			port_list[gpio_pin].bit.available = 1;
			port_list[gpio_pin].bit.configured = 0;
			// Interrupt
			gpio_core->INTENCLR = (1 << (gpio_pin % LPGPIO_16));
			gpio_core->INTPOLCLR = (1 << (gpio_pin % LPGPIO_16));
			gpio_core->INTTYPECLR = (1 << (gpio_pin % LPGPIO_16));
		}
	}
	else if(!port_list[gpio_pin].bit.available) {
		status = PORT_STATUS_RESOURCE_NOT_AVAILABLE;
	}
	else {
		port_list[gpio_pin].bit.available = 0;
		port_list[gpio_pin].bit.configured = 1;
		// Set the pinmux.
		if( gpio_pin <= 7) {
			regaddr = 0x4000b044;
		}
		else if(gpio_pin <= 15) {
			regaddr = 0x4000b048;
		}
		else if(gpio_pin <= 23) {
			regaddr = 0x4000b04c;
		}
		regval = REG_RD(regaddr);
		regval &= ~(7 << ((gpio_pin%8)*4));
		REG_WR(regaddr,regval);
		// Set as Input pin
		gpio_core->OUTENABLECLR |= (1 << (gpio_pin % LPGPIO_16));
		switch(config->detection_type)
		{
			case PORT_INT_DETECT_RISING:
				gpio_core->INTPOLSET = (1 << gpio_pin % (LPGPIO_16));
				gpio_core->INTTYPESET = (1 << gpio_pin % (LPGPIO_16));
				break;
			case PORT_INT_DETECT_FALLING:
				gpio_core->INTPOLCLR = (1 << (gpio_pin % LPGPIO_16));
				gpio_core->INTTYPESET = (1 << gpio_pin % (LPGPIO_16));
				break;
			case PORT_INT_DETECT_HIGH:
				gpio_core->INTPOLSET = (1 << gpio_pin % LPGPIO_16);
				gpio_core->INTTYPECLR = (1 << gpio_pin % (LPGPIO_16));
				break;
			case PORT_INT_DETECT_LOW:
				gpio_core->INTPOLCLR = (1 << (gpio_pin % LPGPIO_16));
				gpio_core->INTTYPECLR = (1 << gpio_pin % (LPGPIO_16));
				break;
			default:
				gpio_core->INTPOLCLR = (1 << (gpio_pin % LPGPIO_16));
				gpio_core->INTTYPECLR = (1 << (gpio_pin % LPGPIO_16));
				gpio_core->INTENCLR = (1 << (gpio_pin % LPGPIO_16));
				break;
		}
	}
	return status;	
}

enum port_status_code port_int_register_callback(uint8_t gpio_pin,
																	portint_callback_t fp)
{
	enum port_status_code status = PORT_STATUS_OK;
	if((fp != NULL) && ((gpio_pin != LPGPIO_0) || (gpio_pin != LPGPIO_1) || (gpio_pin != LPGPIO_14))) {
			port_int_callback[gpio_pin] = fp;
	}
	else {
		status = PORT_STATUS_ERR_INVALID_ARG;
	}
	return status;
}

enum port_status_code port_int_enable(uint8_t gpio_pin)
{
	enum port_status_code status = PORT_STATUS_OK;
	if((gpio_pin == LPGPIO_0) || (gpio_pin == LPGPIO_1) || (gpio_pin == LPGPIO_14)) {
		status = PORT_STATUS_ERR_INVALID_ARG;
	}
	else {
		if(gpio_pin < 16) {
			CMSDK_GPIO0->INTENSET = (1 << gpio_pin);
		}
		else {
			CMSDK_GPIO1->INTENSET = (1 << (gpio_pin % LPGPIO_16));
		}
	}
	return status;
}

enum port_status_code port_int_disable(uint8_t gpio_pin)
{
	enum port_status_code status = PORT_STATUS_OK;
	if((gpio_pin == LPGPIO_0) || (gpio_pin == LPGPIO_1) || (gpio_pin == LPGPIO_14)) {
		status = PORT_STATUS_ERR_INVALID_ARG;
	}
	else {
		if(gpio_pin < 16) {
			CMSDK_GPIO0->INTENCLR = (1 << gpio_pin);
		}
		else {
			CMSDK_GPIO1->INTENCLR = (1 << (gpio_pin % LPGPIO_16));
		}
	}
	return status;
}
