/**
 * \file main.c
 *
 * \brief Main of TAL example - CHAT application
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
 */
/*
 * Copyright (c) 2013, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>
#include "pal.h"
#include "tal.h"
#include "app_config.h"
#include "sio2host.h"
#include "app_common.h"
#include "asf.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/** Enumerations used to identify LEDs */
typedef enum led_id_tag
{
	LED_0,
	LED_1,
	LED_2,
	LED_3,
	LED_4
} SHORTENUM led_id_t;


#define LED_APP         LED0
#define LED_TX_1GHZ     LED0
#define LED_RX_1GHZ     LED0
#define LED_TX_2GHZ     LED0
#define LED_RX_2GHZ     LED0

#define LED_TIME        500000UL

/* === EXTERNALS =========================================================== */

/* === GLOBALS ============================================================= */

#ifdef MULTI_TRX_SUPPORT
app_state_t app_state[2] = {APP_IDLE, APP_IDLE};
#else
app_state_t app_state[1] = {APP_IDLE};
#endif
trx_id_t current_trx_id = RF09;

/* === PROTOTYPES ========================================================== */

static void app_task(void);
static void app_init(void);
void app_alert(void);

/**
 * \brief Button debounce routine.
 *
 * Helper function for debouncing the transmit button.
 * \return ret 1 if a button event is detected, 0 otherwise.
 */
bool app_debounce_button(void);
static void app_timers_init(void);
/**
 * Determine if button is pressed
 *
 * \return true if button is pressed, else false
 */
bool button_pressed(void);

static void led_tx_off(void *parameter);
static void led_rx_off(void *parameter);
#ifdef MULTI_TRX_SUPPORT
static void switch_tx_band(trx_id_t id);
#endif


/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Main function of the Chat application
 */
int main(void)
{
	
	irq_initialize_vectors();
	sysclk_init();

	/* Initialize the board.
	 * The board-specific conf_board.h file contains the configuration of
	 * the board initialization.
	 */
	board_init();
	
	sw_timer_init();
	cpu_irq_enable();
    /* Initialize the TAL layer */
    if (tal_init() != MAC_SUCCESS)
    {
        // something went wrong during initialization
        app_alert();
    }

	app_timers_init();
	
	cpu_irq_enable();

	sio2host_init();

    LED_On(LED_APP);     // indicating application is started

    app_init();

    print_chat_menu();

    /* Endless while loop */
    while (1)
    {
        pal_task(); /* Handle platform specific tasks, like serial interface */
        tal_task(); /* Handle transceiver specific tasks */
        app_task(); /* Application task */
    }
}


/**
 * @brief Initializes the application
 */
static void app_init(void)
{

    /* Configure the TAL PIBs; e.g. set short address */
    uint16_t pan_id = OWN_PAN_ID;
#ifdef MULTI_TRX_SUPPORT
    tal_pib_set(RF09, macPANId, (pib_value_t *)&pan_id);
    tal_pib_set(RF24, macPANId, (pib_value_t *)&pan_id);
    uint16_t addr = OWN_SHORT_ADDR;
    tal_pib_set(RF09, macShortAddress, (pib_value_t *)&addr);
    tal_pib_set(RF24, macShortAddress, (pib_value_t *)&addr);

    /* Configure PHY for sub-1GHz */
    phy_t phy;
    phy.modulation = LEG_OQPSK;
    phy.phy_mode.leg_oqpsk.chip_rate  = CHIP_RATE_1000;
    phy.freq_band = US_915;
    phy.ch_spacing = LEG_915_CH_SPAC; ;
    phy.freq_f0 = LEG_915_F0;
    if (tal_pib_set(RF09, phySetting, (pib_value_t *)&phy) != MAC_SUCCESS)
    {
        app_alert();
    }

    /* Configure PHY for 2.4GHz */
#if 0
    phy.modulation = OFDM;
    phy.phy_mode.ofdm.interl = true;
    phy.phy_mode.ofdm.option = OFDM_OPT_1;
    phy.phy_mode.ofdm.mcs_val = MCS3;
    phy.freq_band = WORLD_2450;
    phy.ch_spacing = OFDM_2450_OPT1_CH_SPAC;
    phy.freq_f0 = OFDM_2450_OPT1_F0;
#else
    phy.modulation = LEG_OQPSK;
    phy.phy_mode.leg_oqpsk.chip_rate = CHIP_RATE_2000;
    phy.freq_band = WORLD_2450;
    phy.ch_spacing = LEG_2450_CH_SPAC;
    phy.freq_f0 = LEG_2450_F0;
#endif
    if (tal_pib_set(RF24, phySetting, (pib_value_t *)&phy) != MAC_SUCCESS)
    {
        app_alert();
    }

#else /* #ifdef MULTI_TRX_SUPPORT */
    tal_pib_set(macPANId, (pib_value_t *)&pan_id);
    uint16_t addr = OWN_SHORT_ADDR;
    tal_pib_set(macShortAddress, (pib_value_t *)&addr);
#endif

    /* Init tx frame info structure value that do not change during program execution */
    init_tx_frame();

    /* Switch receiver(s) on */
#ifdef MULTI_TRX_SUPPORT

   tal_rx_enable(RF09, PHY_RX_ON);
   tal_rx_enable(RF24, PHY_RX_ON);
  
  
#else
    tal_rx_enable(PHY_RX_ON);
#endif
}


/**
 * @brief Application task
 */
static void app_task(void)
{

    int input = sio2host_getchar_nowait();
	static uint8_t key_press;
	 if (input != -1)
	 {
		 switch (input)
		 {
			 #ifdef MULTI_TRX_SUPPORT
			 case SUB1_CHAR:
			 switch_tx_band(RF09);
			 break;
			 case TWO_G_CHAR:
			 switch_tx_band(RF24);
			 break;
			 #endif
			 default:
			 if (input != 0xFF)
			 {
				 get_chat_input(input);
			 }
			 break;
		 }
	 }	
			
/*
			/ * Check for any key press * /
			key_press = app_debounce_button();
			if(key_press != 0)
			{
			switch_tx_band();
			}*/

}


#ifdef MULTI_TRX_SUPPORT
/**
 * @brief Switch frequency band for transmission
 */
static void switch_tx_band(trx_id_t id)
{
led_id_t tx_led_id, rx_led_id;
current_trx_id = id;
printf("\nActive transmitter frequency band: ");

if (id == RF09)
{
	printf("sub-1GHz\n> ");
	tx_led_id = LED_TX_1GHZ;
	rx_led_id = LED_RX_1GHZ;
}
else
{
	printf("2.4GHz\n> ");
	tx_led_id = LED_TX_2GHZ;
	rx_led_id = LED_RX_2GHZ;
}

for (uint8_t i = 0; i < 20; i++)
{
	LED_Toggle(tx_led_id);
	LED_Toggle(rx_led_id);
	pal_timer_delay(50000);
	pal_timer_delay(50000);
}
LED_Off(tx_led_id);
LED_Off(rx_led_id);	
		
	}

#endif


#ifdef MULTI_TRX_SUPPORT
/**
 * @brief User call back function for frame reception
 *
 * @param trx_id   Transceiver identifier
 * @param rx_frame Pointer to received frame structure of type frame_info_t
 *                 or to received frame array
 */
void tal_rx_frame_cb(trx_id_t trx_id, frame_info_t *rx_frame)
#else
void tal_rx_frame_cb(frame_info_t *rx_frame)
#endif
{
#ifndef MULTI_TRX_SUPPORT
    trx_id_t trx_id = RF09;
#endif

    /* Indicate received frame by LED */

    if (trx_id == RF09)
    {
        LED_On(LED_RX_1GHZ);
    }
    else
    {
        LED_On(LED_RX_2GHZ);
    }

    sw_timer_start(T_APP_LED_RX, LED_TIME, TIMEOUT_RELATIVE,
                    (FUNC_PTR)led_rx_off, NULL);

    chat_handle_incoming_frame(trx_id, rx_frame);

    /* Free buffer of incoming frame */
    bmm_buffer_free(rx_frame->buffer_header);
}


#ifdef MULTI_TRX_SUPPORT
/**
 * @brief User call back function for frame transmission
 *
 * @param trx_id Transceiver identifier
 * @param status Status of frame transmission attempt
 * @param frame Pointer to frame structure of type frame_info_t
 */
void tal_tx_frame_done_cb(trx_id_t trx_id, retval_t status, frame_info_t *frame)
#else
void tal_tx_frame_done_cb(retval_t status, frame_info_t *frame)
#endif
{
#ifndef MULTI_TRX_SUPPORT
    trx_id_t trx_id = RF09;
#endif


    if (trx_id == RF09)
    {
        LED_Toggle(LED_TX_1GHZ);
    }
    else
    {
        LED_Toggle(LED_TX_2GHZ);
    }

    sw_timer_start(T_APP_LED_TX, LED_TIME, TIMEOUT_RELATIVE,
                    (FUNC_PTR)led_tx_off, NULL);

    chat_tx_done_cb(trx_id, status, frame);
}


/**
 * @brief User call back function for finished ED Scan
 */
#ifdef MULTI_TRX_SUPPORT
void tal_ed_end_cb(trx_id_t trx_id, uint8_t energy_level)
#else
void tal_ed_end_cb(uint8_t energy_level)
#endif
{
    /* Keep compiler happy */
#ifdef MULTI_TRX_SUPPORT
    trx_id = trx_id;
#endif
    energy_level = energy_level;
}


/**
 * @brief User call back function to switch RX LEDs off
 */
static void led_rx_off(void *parameter)
{

    LED_Off(LED_RX_1GHZ);
    LED_Off(LED_RX_2GHZ);

    /* Keep compiler happy. */
    parameter = parameter;
}


/**
 * @brief User call back function to switch TX LEDs off
 */
static void led_tx_off(void *parameter)
{

    LED_Off(LED_TX_1GHZ);
    LED_Off(LED_TX_2GHZ);

    /* Keep compiler happy. */
    parameter = parameter;
}



/**
 * @brief Get text for return value
 *
 * @param ret return value
 */
char *get_retval_text(retval_t ret)
{
    char *text;

    switch (ret)
    {
        case MAC_SUCCESS:
            text = "MAC_SUCCESS";
            break;
        case TAL_TRX_ASLEEP:
            text = "TAL_TRX_ASLEEP";
            break;
        case TAL_TRX_AWAKE:
            text = "TAL_TRX_AWAKE";
            break;
        case FAILURE:
            text = "FAILURE";
            break;
        case TAL_BUSY:
            text = "TAL_BUSY";
            break;
        case TAL_FRAME_PENDING:
            text = "TAL_FRAME_PENDING";
            break;
        case MAC_CHANNEL_ACCESS_FAILURE:
            text = "MAC_CHANNEL_ACCESS_FAILURE";
            break;
        case MAC_DISABLE_TRX_FAILURE:
            text = "MAC_DISABLE_TRX_FAILURE";
            break;
        case MAC_FRAME_TOO_LONG:
            text = "MAC_FRAME_TOO_LONG";
            break;
        case MAC_INVALID_PARAMETER:
            text = "MAC_INVALID_PARAMETER";
            break;
        case MAC_NO_ACK:
            text = "MAC_NO_ACK";
            break;
        case MAC_UNSUPPORTED_ATTRIBUTE:
            text = "MAC_UNSUPPORTED_ATTRIBUTE";
            break;
        case MAC_READ_ONLY:
            text = "MAC_READ_ONLY";
            break;
        default:
            text = "unknown retval";
            break;
    }
    return text;
}


/**
 * @brief Get text for TAL type
 *
 * @param tal_type TAL type
 */
char *get_tal_type_text(uint8_t tal_type)
{
    char *text;

	text = "AT86RF215";

    return text;
}


/**
 * @brief Get text for PAL type
 *
 * @param pal_type PAL type
 */
char *get_pal_type_text(uint8_t pal_type)
{
    char *text;

            text = "SAM4LC4C";


    return text;
}


/**
 * @brief Get text for board type
 *
 * @param board Board type
 */
char *get_board_text(uint8_t board)
{
    char *text;

    
            text = "SAM4L Xpro-RF215";

    return text;
}


#ifdef MULTI_TRX_SUPPORT
/**
 * @brief Get text for transceiver identifier
 *
 * @param id Transceiver identifier
 */
char *get_trx_id_text(trx_id_t id)
{
    char *text;

    if (id == RF09)
    {
        text = "RF09";
    }
    else if (id == RF24)
    {
        text = "RF24";
    }
    else
    {
        text = "unknown trx id";
    }

    return text;
}
#endif

void app_alert()
{
	while (1) {
		#if LED_COUNT > 0
		LED_Toggle(LED0);
		#endif

		#if LED_COUNT > 1
		LED_Toggle(LED1);
		#endif

		#if LED_COUNT > 2
		LED_Toggle(LED2);
		#endif

		#if LED_COUNT > 3
		LED_Toggle(LED3);
		#endif

		#if LED_COUNT > 4
		LED_Toggle(LED4);
		#endif

		#if LED_COUNT > 5
		LED_Toggle(LED5);
		#endif

		#if LED_COUNT > 6
		LED_Toggle(LED6);
		#endif

		#if LED_COUNT > 7
		LED_Toggle(LED7);
		#endif
		delay_us(0xFFFF);
	}
}

bool app_debounce_button(void)
{
	uint8_t ret = 0;
	static uint8_t key_cnt;
	/*Read the current state of the button*/

	if (button_pressed()) { /* Button Pressed */
		if (key_cnt != 20) {
			key_cnt++;
		}
	} else if (!(button_pressed()) &&
			(key_cnt == 20)) {           /*
		                                                             * Button
		                                                             *
		                                                             *released
		                                                             **/
		ret = 1;
		key_cnt = 0;
	} else {
		key_cnt = 0;
	}

	return ret;
}

/*
 * Determine if button is pressed
 *
 * \return true if button is pressed, else false
 */
bool button_pressed(void)
{
#if defined GPIO_PUSH_BUTTON_0
	/*Read the current state of the button*/
	if (ioport_get_pin_level(GPIO_PUSH_BUTTON_0)) {
		return false;
	} else {
		return true;
	}

#else
	return false;
#endif
}

static void app_timers_init(void)
{
	if (STATUS_OK != sw_timer_get_id(&T_APP_TX_DELAY)) {
		app_alert();
	}

	if (STATUS_OK != sw_timer_get_id(&T_APP_LED_TX)) {
		app_alert();
	}

	if (STATUS_OK != sw_timer_get_id(&T_APP_LED_RX)) {
		app_alert();
	}


}


/* EOF */
