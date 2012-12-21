/**
 * @file main.c
 *
 * @brief RF4CE Serial Interface Application
 *
 * $Id: main.c 32889 2012-08-31 10:40:36Z agasthian.s $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ============================================================ */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <asf.h>
#include "conf_board.h"
#include "pal.h"
#include "tal.h"
#include "serial_interface.h"
#include "app_config.h"


/* === Macros ============================================================== */

/* === Globals ============================================================= */

/* Write application specific values into flash memory */
FLASH_DECLARE(uint16_t VendorIdentifier) = (uint16_t)NWKC_VENDOR_IDENTIFIER;
FLASH_DECLARE(uint8_t vendor_string[7]) = NWKC_VENDOR_STRING;
FLASH_DECLARE(uint8_t app_user_string[15]) = APP_USER_STRING;
#ifdef ZRC_CMD_DISCOVERY
FLASH_DECLARE(uint8_t supported_cec_cmds[32]) = SUPPORTED_CEC_CMDS;
#endif

/* === EXTERNALS =========================================================== */
#ifdef RF4CE_CALLBACK_PARAM
extern void nwk_indication_callback_init(void);
#endif


/* === Prototypes ========================================================== */
static void app_alert(void);
/* === Implementation ====================================================== */

/**
 * Main function, initialization and main message loop
 *
 * @return error code
 */
int main (void)
{
    irq_initialize_vectors();

	/* Initialize the board.
	 * The board-specific conf_board.h file contains the configuration of
	 * the board initialization.
	 */
	board_init();
	sysclk_init();

	sw_timer_init();
        
    if (nwk_init(RF_BAND) != NWK_SUCCESS)
    {
        app_alert();
    }

#ifdef RF4CE_CALLBACK_PARAM
    nwk_indication_callback_init();
#endif

#ifdef FLASH_NVRAM
    pal_ps_set(EE_IEEE_ADDR, 8, &tal_pib.IeeeAddress);
#endif
    /* Initialize LEDs */
    //pal_led_init();
    cpu_irq_enable();

    /*
     * The global interrupt has to be enabled here as TAL uses the timer
     * delay which in turn requires interrupt to be enabled
     */
    //pal_global_irq_enable();

    serial_interface_init();

    /* Loop forever, the interrupts are doing the rest */
    while (1)
    {
        nwk_task();
        serial_data_handler();
    }
    /* No return statement here, because this code is unreachable */
}


bool pbp_allow_pairing(nwk_enum_t Status, uint64_t SrcIEEEAddr, uint16_t OrgVendorId,
                       uint8_t OrgVendorString[7], uint8_t OrgUserString[15],
                       uint8_t KeyExTransferCount)
{
    /* Keep compiler happy */
    Status = Status;
    SrcIEEEAddr = SrcIEEEAddr;
    OrgVendorId = OrgVendorId;
    OrgVendorString[0] = OrgVendorString[0];
    OrgUserString[0] = OrgUserString[0];
    KeyExTransferCount = KeyExTransferCount;

    return true;
}

static void app_alert(void)
{
    while (1)
    {
     
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
/* EOF */
