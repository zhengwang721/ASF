/**
 * @file main.c
 *
 * @brief RF4CE Serial Interface Application
 *
 * $Id: main.c 34021 2013-01-29 05:42:49Z agasthian.s $
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
#include "common_sw_timer.h"
#include "serial_api.h"
#include "app_config.h"
#include "pb_pairing.h"

/* === Macros ============================================================== */

/* === Globals ============================================================= */
#ifdef RF4CE_CALLBACK_PARAM
extern void zid_indication_callback_init(void);
#endif

/* Write application specific values into flash memory */
FLASH_DECLARE(uint16_t VendorIdentifier) = (uint16_t)NWKC_VENDOR_IDENTIFIER;
FLASH_DECLARE(uint8_t vendor_string[7]) = NWKC_VENDOR_STRING;
FLASH_DECLARE(uint8_t app_user_string[15]) = APP_USER_STRING;
#ifdef ZRC_CMD_DISCOVERY
FLASH_DECLARE(uint8_t supported_cec_cmds[32]) = SUPPORTED_CEC_CMDS;
#endif
/* === Prototypes ========================================================== */
static void app_alert(void);
/* === Implementation ====================================================== */

/* === EXTERNALS =========================================================== */

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
        
    if (nwk_init() != NWK_SUCCESS)
    {
        app_alert();
    }

#ifdef RF4CE_CALLBACK_PARAM
    zid_indication_callback_init();
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

#ifdef PBP_REC
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
#endif

#ifdef ZID_DEVICE
bool check_zid_adaptor_compatibility(uint8_t PairingRef,uint8_t payload_length,uint8_t *payload)
{
    // Application need to find out the compatibility with the adaptor
    // It needs to extract the adaptor attributes from the following payloas
    // Payload format is as per the GET_ATTRIBUTES_RESPONSE packet format excluding the header
    // payload[0] = attr_id...........
    PairingRef = PairingRef;
    payload_length = payload_length;
    payload = payload;
    return true;

}
#endif

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
