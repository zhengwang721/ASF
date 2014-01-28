/**
 * \file
 *
 * \brief Example specific board configuration file
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
#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define CONF_BOARD_ENABLE_USARTA1

/* Taking the battery voltage for TFA */
#define GET_BATTERY_VOLTAGE()    tfa_get_batmon_voltage()  








/* Enumerations used to identify LEDs */
typedef enum led_id_tag
{
    LED_1,
    LED_2,
    LED_3,
    LED_4,
    LED_5
} SHORTENUM led_id_t;

/* Enumerations used to idenfify ADC Channels */
typedef enum adc_channel_tag
{
    X_AXIS,
    Y_AXIS,
    Z_AXIS,
    ADC_REF
} SHORTENUM adc_channel_t;

/* Enumerations states for enabling or disabling the Accelerometer */
typedef enum acc_status_tag
{
    ACC_OFF,
    ACC_ON
} SHORTENUM acc_status_t;

/**
 * @brief LED action
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


/**
 * @brief Button action
 */
typedef enum button_state_tag
{
    /** Button is pressed. */
    BUTTON_PRESSED,
    /** Button is released. */
    BUTTON_OFF
} SHORTENUM button_state_t;


/* === Externals ============================================================*/


/* === Macros ===============================================================*/

/**
 * The default CPU clock
 *
 * The board must be configured for 16 MHz clock, which can be fused
 * for either the 16 MHz internal RC oscillator, or for the 16 MHz
 * crystal oscillator that is required for the transceiver operation.
 * If F_CPU is set to 4 or 8 MHz, the prescaler reduces the CPU clock.
 */
#ifndef F_CPU
#define F_CPU                           (16000000UL)
#endif

/**
 * This board does not support antenna diversity.
 */
#define ANTENNA_DIVERSITY               (0)


#define NO_OF_LEDS                      (5)

/* Definitions to identify buttons, reference HDMI specification */
#define BUTTON_UNKNOWN            0x00000000
#define BUTTON_F2_RED             0x00000001
#define BUTTON_F3_GREEN           0x00000002
#define BUTTON_F4_YELLOW          0x00000004
#define BUTTON_F1_BLUE            0x00000008
#define BUTTON_SELECT             0x00000010
#define BUTTON_NUMBER_0           0x00000020
#define BUTTON_POWER_OFF_FUNCTION 0x00000040
#define BUTTON_NUMBER_1           0x00000200
#define BUTTON_NUMBER_2           0x00000400
#define BUTTON_NUMBER_3           0x00000800
#define BUTTON_NUMBER_4           0x00001000
#define BUTTON_NUMBER_5           0x00002000
#define BUTTON_NUMBER_6           0x00004000
#define BUTTON_NUMBER_7           0x00008000
#define BUTTON_NUMBER_8           0x00010000
#define BUTTON_NUMBER_9           0x00020000
#define BUTTON_UP                 0x00040000
#define BUTTON_LEFT               0x00080000
#define BUTTON_ENTER              0x00100000
#define BUTTON_RIGHT              0x00200000
#define BUTTON_DOWN               0x00400000
#define BUTTON_LEFT_UP            0x00800000
#define BUTTON_LEFT_DOWN          0x01000000
#define BUTTON_RIGHT_UP           0x02000000
#define BUTTON_RIGHT_DOWN         0x04000000


#define button_id_t             uint32_t
#define NO_OF_BUTTONS           (24)

/*
 * PORT where LEDs are connected
 */
#define LED_PORT                        (PORTB)
#define LED_PORT_DIR                    (DDRB)

/*
 * PINs where LEDs are connected
 */
#define LED_1_PIN                       (PB0)
#define LED_2_PIN                       (PB1)
#define LED_3_PIN                       (PB2)
#define LED_4_PIN                       (PB3)
#define LED_5_PIN                       (PB4)
#define LED_MASK                        ((1 << LED_1_PIN) | (1 << LED_2_PIN) \
                                         | (1 << LED_3_PIN) | (1 << LED_4_PIN) \
                                         | (1 << LED_5_PIN))

/*
 * PINs where Accelerometer power is connected
 */
#define ACC_PWR                         (PB5)

/*
 * PORT where button is connected
 */
#define BUTTON_IRQ_PORT                 (PORTD)
#define BUTTON_IRQ_PORT_DIR             (DDRD)
#define BUTTON_IRQ_PORT_IN              (PIND)
#define BUTTON_PORT1                    (PORTB)
#define BUTTON_PORT1_DIR                (DDRB)
#define BUTTON_PORT1_IN                 (PINB)
#define BUTTON_PORT2                    (PORTD)
#define BUTTON_PORT2_DIR                (DDRD)
#define BUTTON_PORT2_IN                 (PIND)

/*
 * PINs where buttons are connected
 */
#define BUTTON_IRQ_PIN_1                (PD1)
#define BUTTON_IRQ_PIN_2                (PD2)
#define BUTTON_IRQ_PIN_3                (PD3)
#define BUTTON_PIN_0                    (PB0)
#define BUTTON_PIN_1                    (PB1)
#define BUTTON_PIN_2                    (PB2)
#define BUTTON_PIN_3                    (PB3)
#define BUTTON_PIN_4                    (PB4)
#define BUTTON_PIN_5                    (PB5)
#define BUTTON_PIN_6                    (PB6)
#define BUTTON_PIN_7                    (PD5)
#define BUTTON_PIN_8                    (PD7)

/*
 * ISR vectors for buttons
 */
#define BUTTON_1_ISR_vect               (INT1_vect)
#define BUTTON_2_ISR_vect               (INT2_vect)
#define BUTTON_3_ISR_vect               (INT3_vect)

/*
 * ISR mask for buttons
 */
#define BUTTON_1_ISR_MASK               (1 << INT1)
#define BUTTON_2_ISR_MASK               (1 << INT2)
#define BUTTON_3_ISR_MASK               (1 << INT3)

/*
 * ISR flag for buttons
 */
#define BUTTON_FLAG_REG                 (EIFR)
#define BUTTON_1_ISR_FLAG               (1 << INTF1)
#define BUTTON_2_ISR_FLAG               (1 << INTF2)
#define BUTTON_3_ISR_FLAG               (1 << INTF3)

/*
 * Button input mask
 */
#define BUTTON_IRQ_1_IN_MASK            (1 << PIND1)
#define BUTTON_IRQ_2_IN_MASK            (1 << PIND2)
#define BUTTON_IRQ_3_IN_MASK            (1 << PIND3)


/* Disable all button interrupts */
#define DISABLE_ALL_BUTTON_IRQS()       EIMSK &= ~(BUTTON_1_ISR_MASK | BUTTON_2_ISR_MASK | BUTTON_3_ISR_MASK)
/* Enable all button interrupts */
#define ENABLE_ALL_BUTTON_IRQS()        EIMSK |= (BUTTON_1_ISR_MASK | BUTTON_2_ISR_MASK | BUTTON_3_ISR_MASK)
/* Clear all button IRQ flags */
#define CLEAR_ALL_BUTTON_IRQ_FLAGS()    BUTTON_FLAG_REG = BUTTON_1_ISR_FLAG | BUTTON_2_ISR_FLAG | BUTTON_3_ISR_FLAG

/*
 * Macros controlling the latch
 */
#define LATCH_PORT                      PORTE
#define LATCH_PORT_DIR                  DDRE
#define LATCH_PIN                       PE5
#define LATCH_HIGH()                    LATCH_PORT |= 1 << LATCH_PIN
#define LATCH_LOW()                     LATCH_PORT &= ~(1 << LATCH_PIN)
#define LATCH_PULSE()                   LATCH_HIGH(); LATCH_LOW()
#define LATCH_DATA                      PORTB
#define LATCH_DATA_DIR                  DDRB
#define LATCH_INIT()                    do { \
        LATCH_PORT &= ~(1 << LATCH_PIN); \
        LATCH_PORT_DIR |= 1 << LATCH_PIN; \
    } while (0)   
   

button_id_t pal_button_scan(void);
void pal_button_init(void);
void pal_led(led_id_t led_no, led_action_t led_setting);
void pal_led_init(void);








#endif /* CONF_BOARD_H */
