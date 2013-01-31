/**
 * @file main.c
 *
 * @brief Single button controller application
 *
 * $Id: main.c 33395 2012-11-06 10:49:31Z mathivanan.e $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <asf.h>
#include "conf_board.h"
#include "common_sw_timer.h"
#include "keyboard.h"
#include "app_config.h"
#include "rf4ce.h"
#include "zrc.h"
#include "pb_pairing.h"
#include "vendor_data.h"


/* === TYPES =============================================================== */

typedef enum node_status_tag
{
    IDLE,
    WARM_START,
    COLD_START,
    TRANSMITTING,
   
} SHORTENUM node_status_t;

/* === MACROS ============================================================== */

#define INTER_FRAME_DURATION_US    200000
#define TX_OPTIONS  (TXO_UNICAST | TXO_DST_ADDR_NET | \
                     TXO_ACK_REQ | TXO_SEC_REQ | TXO_MULTI_CH | \
                     TXO_CH_NOT_SPEC | TXO_VEND_NOT_SPEC)

/* === GLOBALS ============================================================= */

FLASH_DECLARE(uint16_t VendorIdentifier) = (uint16_t)NWKC_VENDOR_IDENTIFIER;
FLASH_DECLARE(uint8_t vendor_string[7]) = NWKC_VENDOR_STRING;
FLASH_DECLARE(uint8_t app_user_string[15]) = APP_USER_STRING;
FLASH_DECLARE(uint8_t supported_cec_cmds[32]) = SUPPORTED_CEC_CMDS;

static node_status_t node_status;
static uint8_t pairing_ref = 0xFF;

/* === PROTOTYPES ========================================================== */

static void app_task(void);
static key_state_t key_state_read(key_id_t key_no);
static void indicate_fault_behavior(void);
#ifdef ZRC_CMD_DISCOVERY
static void start_cmd_disc_cb(void *callback_parameter);
#endif



#ifdef RF4CE_CALLBACK_PARAM
static void zrc_cmd_confirm(nwk_enum_t Status, uint8_t PairingRef, cec_code_t RcCmd);
static void zrc_cmd_disc_confirm(nwk_enum_t Status, uint8_t PairingRef, uint8_t *SupportedCmd);
static void nlme_reset_confirm(nwk_enum_t Status);
void nlme_rx_enable_confirm(nwk_enum_t Status);
static void pbp_org_pair_confirm(nwk_enum_t Status, uint8_t PairingRef);
static void nlme_start_confirm(nwk_enum_t Status);

#ifdef ZRC_CMD_DISCOVERY
static void zrc_cmd_disc_indication(uint8_t PairingRef);
#endif

#ifdef VENDOR_DATA
extern void vendor_data_ind(uint8_t PairingRef, uint16_t VendorId,
                            uint8_t nsduLength, uint8_t *nsdu, uint8_t RxLinkQuality,
                            uint8_t RxFlags);
#endif
zrc_indication_callback_t zrc_ind;
uint8_t app_timer;

#endif
/* === IMPLEMENTATION ====================================================== */



/**
 * @brief Main function of the Single Button Controller application
 */
int main(void)
{

    irq_initialize_vectors();

	/* Initialize the board.
	 * The board-specific conf_board.h file contains the configuration of
	 * the board initialization.
	 */
	board_init();
	sysclk_init();

	sw_timer_init();
        
    /* Initialize all layers */
    if (nwk_init()!= NWK_SUCCESS)
    {
         //something went wrong during initialization
        while (1)
        {
            indicate_fault_behavior();
        }
    }
    cpu_irq_enable();
    sw_timer_get_id(&app_timer);
#ifdef RF4CE_CALLBACK_PARAM
#ifdef VENDOR_DATA
    zrc_ind.vendor_data_ind_cb = vendor_data_ind;
#endif
#ifdef ZRC_CMD_DISCOVERY
    zrc_ind.zrc_cmd_disc_indication_cb =  zrc_cmd_disc_indication;
#endif

    register_zrc_indication_callback(&zrc_ind);
#endif
    /*
     * The stack is initialized above,
     * hence the global interrupts are enabled here.
     */
    //pal_global_irq_enable();

    

    key_state_t key_state = key_state_read(SELECT_KEY);
    // For debugging: Force button press
    //key_state = KEY_PRESSED;
    if (key_state == KEY_PRESSED)
    {
        // Force push button pairing
        /* Cold start */
        LED_On(LED0);
        node_status = COLD_START;
        nlme_reset_request(true
#ifdef RF4CE_CALLBACK_PARAM
                           , (FUNC_PTR)nlme_reset_confirm
#endif
                          );        
    }
    else
    {
        /* Warm start */
        node_status = WARM_START;
        nlme_reset_request(false
#ifdef RF4CE_CALLBACK_PARAM
                           , (FUNC_PTR)nlme_reset_confirm
#endif
                          );
    }
#if 1
    /* Endless while loop */
    while (1)
    {
        app_task(); /* Application task */
        nwk_task(); /* RF4CE network layer task */
    }
#endif
}


/*
 * The NLME-RESET.confirm primitive allows the NLME to notify the application of
 * the status of its request to reset the NWK layer.
 */
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nlme_reset_confirm(nwk_enum_t Status)
{
  
   if (Status != NWK_SUCCESS)
    {
        while (1)
        {
            // endless while loop!
            indicate_fault_behavior();
        }
    }

    if (node_status == COLD_START)
    {
        pairing_ref = 0xFF;
       nlme_start_request(
#ifdef RF4CE_CALLBACK_PARAM
            (FUNC_PTR)nlme_start_confirm
#endif
        );
    }
    else    // warm start
    {
        pairing_ref = 0;
        /* Set power save mode */
#ifdef ENABLE_PWR_SAVE_MODE
        nlme_rx_enable_request(nwkcMinActivePeriod
#ifdef RF4CE_CALLBACK_PARAM
                               , (FUNC_PTR)nlme_rx_enable_confirm
#endif
                              );
#else
        nlme_rx_enable_request(RX_DURATION_OFF
#ifdef RF4CE_CALLBACK_PARAM
                               , (FUNC_PTR)nlme_rx_enable_confirm
#endif
                              );
#endif
    }

}



/*
 * The NLME-START.confirm primitive allows the NLME to notify the application of
 * the status of its request to start a network.
 */
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nlme_start_confirm(nwk_enum_t Status)
{
    if (Status != NWK_SUCCESS)
    {
        while (1)
        {
            indicate_fault_behavior();
        }
    }

    
    LED_On(LED0);

    dev_type_t OrgDevTypeList[1];
    profile_id_t OrgProfileIdList[1];
    profile_id_t DiscProfileIdList[1];

    OrgDevTypeList[0] = DEV_TYPE_REMOTE_CONTROL;
    OrgProfileIdList[0] = PROFILE_ID_ZRC;
    DiscProfileIdList[0] = PROFILE_ID_ZRC;

    pbp_org_pair_request(APP_CAPABILITIES, OrgDevTypeList, OrgProfileIdList,
                         DEV_TYPE_WILDCARD, NUM_SUPPORTED_PROFILES, DiscProfileIdList
#ifdef RF4CE_CALLBACK_PARAM
                         , (FUNC_PTR)pbp_org_pair_confirm
#endif
                        );
}


/**
 * @brief Push button pairing confirm; target and controller use
 *
 * The push button pairing confirm is a callback that provides the status of the
 * push pairing request.
 *
 * @param Status        Status of the last pairing transaction
 * @param PairingRef    If pairing was successful, PairingRef contains assigned
 *                      pairing reference.
 */
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void pbp_org_pair_confirm(nwk_enum_t Status, uint8_t PairingRef)
{
    if (Status != NWK_SUCCESS)
    {
      indicate_fault_behavior();
    }

    pairing_ref = PairingRef;

#ifdef ZRC_CMD_DISCOVERY
    /* Start timer to send the cmd discovery request */
   sw_timer_start(app_timer,
                    aplcMinTargetBlackoutPeriod_us,
                    SW_TIMEOUT_RELATIVE,
                    (FUNC_PTR)start_cmd_disc_cb,
                    NULL);
#else
    /* Set power save mode */
#ifdef ENABLE_PWR_SAVE_MODE
    nlme_rx_enable_request(nwkcMinActivePeriod);
#else
    nlme_rx_enable_request(RX_DURATION_OFF);
#endif
#endif
}


/*
 * Callback funtion indication that the timer expired for sending the command
 * discovery request.
 */
#ifdef ZRC_CMD_DISCOVERY
static void start_cmd_disc_cb(void *callback_parameter)
{
    zrc_cmd_disc_request(pairing_ref
#ifdef RF4CE_CALLBACK_PARAM
                         , (FUNC_PTR)zrc_cmd_disc_confirm
#endif
                        );

    /* Keep compiler happy */
    callback_parameter = callback_parameter;
}
#endif


/*
 * The command discovery confirm callback provides information about the command
 * discovery reqest.
 */
#ifdef ZRC_CMD_DISCOVERY
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void zrc_cmd_disc_confirm(nwk_enum_t Status, uint8_t PairingRef, uint8_t *SupportedCmd)
{
    /* Enable transceiver Power Save Mode */
#ifdef ENABLE_PWR_SAVE_MODE
    nlme_rx_enable_request(nwkcMinActivePeriod
#ifdef RF4CE_CALLBACK_PARAM
                           , (FUNC_PTR)nlme_rx_enable_confirm
#endif
                          );
#else
    nlme_rx_enable_request(RX_DURATION_OFF
#ifdef RF4CE_CALLBACK_PARAM
                           , (FUNC_PTR)nlme_rx_enable_confirm
#endif
                          );
#endif

    /* Keep compiler happy */
    Status = Status;
    PairingRef = PairingRef;
    SupportedCmd = SupportedCmd;
}
#endif


/*
 * The command discovery indication callback indicates that a command discovery
 * request command was receiced.
 */
#ifdef ZRC_CMD_DISCOVERY
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void zrc_cmd_disc_indication(uint8_t PairingRef)
{
    /* Send back the response */
    uint8_t cec_cmds[32];
    PGM_READ_BLOCK(cec_cmds, supported_cec_cmds, 32);
    zrc_cmd_disc_response(PairingRef, cec_cmds);
}
#endif

//
/*
 * The NLME-RX-ENABLE.confirm primitive reports the results of the attempt to
 * enable or disable the receiver.
 */
void nlme_rx_enable_confirm(nwk_enum_t Status)
{
    if (Status != NWK_SUCCESS)
    {
        while (1)
        {
            indicate_fault_behavior();
        }
    }

    if (node_status == COLD_START)
    {
        node_status = IDLE;

        /* LED handling */
        LED_On(LED0);
        delay_ms(1000);
        LED_Off(LED0);
      
    }
    else if (node_status == WARM_START)
    {
        node_status = IDLE;

        /* LED handling */
        LED_On(LED0 );
        delay_ms(250);
        LED_Off(LED0);
        
    }
}


/**
 * @brief Application task and state machine
 */
static void app_task(void)
{
    static uint32_t current_time;
    static uint32_t previous_button_time;
    switch (node_status)
    {
        case IDLE:
            {
                static key_state_t key_state;
                key_state = key_state_read(SELECT_KEY);
                if (key_state == KEY_PRESSED)
                {
                    /* Check time to previous transmission. */
                    current_time=sw_timer_get_time();
                    if ((current_time - previous_button_time) < INTER_FRAME_DURATION_US)
                    {
                        return;
                    }
                    else
                    {
                        /* Store current time */
                        previous_button_time = current_time;
                    }
                    LED_On(LED0);
                    uint8_t cmd = POWER_TOGGLE_FUNCTION;  // 0x6b
                     if (zrc_cmd_request(pairing_ref, 0x0000, USER_CONTROL_PRESSED,
                                            1, &cmd, TX_OPTIONS
    #ifdef RF4CE_CALLBACK_PARAM
                                            , (FUNC_PTR)zrc_cmd_confirm
    #endif
                                           ))
                    {
                        node_status = TRANSMITTING;
                    }
                }
                else //(button == BUTTON_OFF)
                {
                   /* if (nwk_ready_to_sleep())
                    {
                        / * Set MCU to sleep * /                     
                        sleepmgr_init();
                        sleepmgr_lock_mode(SLEEPMGR_IDLE);
                        sleepmgr_enter_sleep();
                        / * MCU is awake again * /
                    }*/
                }
            }
            break;

        default:
            break;
    }
    
}
//

/**
 * @brief ZRC command confirm
 *
 * The ZRC comand confirm callback provides information about the status of the
 * last command request/transmission.
 *
 * @param Status        NWK_SUCCESS if transmission was successful
 * @param PairingRef    Pairing reference
 * @param RcCmd         Sent RC command
 */
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void zrc_cmd_confirm(nwk_enum_t Status, uint8_t PairingRef, cec_code_t RcCmd)
{
    node_status = IDLE;

    if (Status == NWK_SUCCESS)
    {
        LED_Off(LED0);
    }
    else
    {
        indicate_fault_behavior();
    }

    /* Keep compiler happy. */
    PairingRef = PairingRef;
    RcCmd = RcCmd;
}

//
/*
 * The NLDE-DATA.confirm primitive is generated by the NWK layer entity in
 * response to an NLDE-DATA.request primitive.
 */
#ifndef RF4CE_CALLBACK_PARAM
void nlde_data_confirm(nwk_enum_t Status, uint8_t PairingRef, profile_id_t ProfileId
#ifdef NLDE_HANDLE
                       , uint8_t Handle
#endif
                      )
{
    node_status = IDLE;

    if (Status == NWK_SUCCESS)
    {
        LED_Off(LED0);
    }
    else
    {
        indicate_fault_behavior();
    }

    /* Keep compiler happy. */
    PairingRef = PairingRef;
    ProfileId = ProfileId;
#ifdef NLDE_HANDLE
    Handle = Handle;
#endif
}
#endif


/* --- Helper functions ---------------------------------------------------- */





/**
 * @brief Indicating malfunction
 */
static void indicate_fault_behavior(void)
{
    uint8_t i;

    for (i = 0; i < 10; i++)
    {

       
        LED_On(LED0);
        delay_ms(200);
        LED_Off(LED0);
        delay_ms(200);
    }
}


/**
 * @brief Vendor-spefic callback; handles reaction to incoming alive request
 */
void vendor_app_alive_req(void)
{
    /* Variant to demonstrate FOTA featue */
#if 1
    LED_On(LED0);
    delay_ms(500);
    LED_Off(LED0);
#else
    LED_On(LED0);
    delay_ms(500);
    LED_Off(LED0);
#endif
}

/**
 * @brief Read key_state
 *
 * @param button_no Button ID
 */
static key_state_t key_state_read(key_id_t key_no)
{
    key_state_t key_val = KEY_RELEASED;
    switch (key_no)
    {
        case SELECT_KEY:
          if(!ioport_get_pin_level(GPIO_PUSH_BUTTON_0))
          {
            key_val = KEY_PRESSED; 
          }
        break;
        default:
        break;
    }
  return key_val;
}



/* EOF */
