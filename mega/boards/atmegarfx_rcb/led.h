/**
 * \file
 *
 * \brief  ATMEGA256RFR2 RCB board LEDs support package.
 *
 * This file contains definitions and services related to the LED features of
 * the ATMEGA256RFR2 XPLAINED PRO board.
 *
 * To use this board, define BOARD=ATMEGA256RFR2_XPLAINED_PRO.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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

#ifndef _LED_H_
#define _LED_H_

#include "gpio.h"
# include "board.h"

static uint8_t led_state = 0x00;

typedef enum led_id_tag
{
    LED_0,
    LED_1,
    LED_2
} SHORTENUM led_id_t;
/**
 * \brief LED action
 */
typedef enum led_action_tag
{
    /** Switch LED on. */
    LED_ON,
    /** Switch LED off. */
    LED_OFF,
    /** Toggle LED. */
    LED_TOGGLE
} SHORTENUM led_action_t;

/*
 * Bit numbers address where LEDs are mapped to.
 */
#define LED_BIT_0                       (0)
#define LED_BIT_1                       (1)

/*
 * LED bit mask
 */
#define LED_BIT_MASK                    ((1 << LED_BIT_0) | (1 << LED_BIT_1))
#define LED_PORT                        (PORTB)
#define LED_PORT_DIR                    (DDRB)
#ifdef SENSOR_TERMINAL_BOARD
/*! \brief Turns off the specified LEDs.
 *
 * \param led_gpio LED to turn off (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Off(led_gpio)     pal_led(led_gpio,LED_OFF)

                                  

/*! \brief Turns on the specified LEDs.
 *
 * \param led_gpio LED to turn on (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_On(led_gpio)      pal_led(led_gpio,LED_ON)

/*! \brief Toggles the specified LEDs.
 *
 * \param led_gpio LED to toggle (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Toggle(led_gpio)  pal_led(led_gpio,LED_TOGGLE)
//#endif

/**
 * \brief Helper function for LEDs
 */
static inline void led_helper_func(void)
{
    /*
     * Enable LED address decoding.
     * This is similar to USB, but with other settings.
     */
    LED_ADDR_DEC_PORT |= _BV(6);    // Different to USB
    LED_ADDR_DEC_DDR |= _BV(6);
    LED_ADDR_DEC_PORT &= ~_BV(7);
    LED_ADDR_DEC_DDR |= _BV(7);

    /* Enable desired LED state. */
    DDRE |= _BV(4);
    /* Set PE4 to 0 and back to 1. */
    PORTE &= ~_BV(4);
    PORTE |= _BV(4);

    /*
     * Disable LED address decoding.
     * This enables USB again.
     */
    LED_ADDR_DEC_PORT &= ~_BV(6);
    LED_ADDR_DEC_DDR |= _BV(6);
    LED_ADDR_DEC_PORT &= ~_BV(7);
    LED_ADDR_DEC_DDR |= _BV(7);
}


static inline void pal_led(led_id_t led_no, led_action_t led_setting)
{
uint8_t pin;
/* New values of LED pins based on new LED state. */
uint8_t led_pin_value;
/*
* Original value of LED port before writing new value.
* This value needs to be restored.
*/
uint8_t orig_led_port = LED_PORT & ~LED_BIT_MASK;

/* Both LEDs need to be updated, since several peripherals
* are dealing with the same port for this board
* (USB, EEPROM, LEDs, Button).
*/
LED_PORT_DIR |= (1 << LED_BIT_0);
LED_PORT_DIR |= (1 << LED_BIT_1);

switch (led_no)
{
case LED_0:
    pin = LED_BIT_0;
    break;
case LED_1:
    pin = LED_BIT_1;
    break;
default:
    return;
}

switch (led_setting)
{
case LED_ON:
    led_state |= _BV(pin);
    break;

case LED_OFF:
    led_state &= ~_BV(pin);
    break;

case LED_TOGGLE:
default:
    if (led_state & _BV(pin))
    {
        /*
         * LED is currently on,
         * Switch it off
         */
        led_state &= ~_BV(pin);
    }
    else
    {
        /*
         * LED is currently off,
         * Switch it on
         */
        led_state |= _BV(pin);
    }
    break;
}

led_pin_value = (uint8_t)(~(uint16_t)led_state);  // Implicit casting required to avoid IAR Pa091.
led_pin_value &= LED_BIT_MASK;

LED_PORT = orig_led_port | led_pin_value;

led_helper_func();
}

#else

/*! \brief Turns off the specified LEDs.
 *
 * \param led_gpio LED to turn off (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Off(led_gpio)     gpio_set_pin_high(led_gpio)

/*! \brief Turns on the specified LEDs.
 *
 * \param led_gpio LED to turn on (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_On(led_gpio)      gpio_set_pin_low(led_gpio)

/*! \brief Toggles the specified LEDs.
 *
 * \param led_gpio LED to toggle (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Toggle(led_gpio)  gpio_toggle_pin(led_gpio)

#endif //STB
#endif /* _LED_H_ */
