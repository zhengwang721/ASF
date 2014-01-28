/**
 * @file main.c
 *
 * @brief QTouch Device Application
 *
 * $Id: main.c 34021 2013-01-29 05:42:49Z agasthian.s $
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
#include "led.h"
#include "common_sw_timer.h"
#include "keyboard.h"
#include "app_config.h"
#include "rf4ce.h"
#include "vendor_data.h"
#include "pb_pairing.h"
#include "zid.h"
#include "zid_device.h"
#include "Avr477QTouch.h"

#define BUTTON_0 0
#define BUTTON_1 1
#define BUTTON_2 2

/* === TYPES =============================================================== */
/* ZID States for Tracking */
typedef enum node_status_tag
{
    IDLE,
    WARM_START,
    COLD_START,
    CONFIGURING_ATTRIBUTES,
    CONNECTING,
    TRANSMITTING
} SHORTENUM node_status_t;


/* === MACROS ============================================================== */

#define  BUTTON_PLAY       (0x0080)
#define  BUTTON_PAUSE      (0x0100)
#define  BUTTON_MPLAYER    (0x0008)
#define  BUTTON_STOP       (0x0010)
#define  BUTTON_NEXT       (0x0040)
#define  BUTTON_PREVIOUS   (0x0020)
#define  BUTTON_VOLUME_P   (0x0002)
#define  BUTTON_VOLUME_N   (0x0004)
#define  BUTTON_MUTE       (0x0001)
#define  BUTTON_MODE       (0x00FE)
#define  BUTTON_REPEAT     (0x0200)

#define  BUTTON_PPT_MODE   (0x01)
#define  BUTTON_MEDIA_MODE (0x02)


#define  BUTTON_ESC        (41)
#define  BUTTON_TAB        (43)
#define  BUTTON_UP         (82)
#define  BUTTON_DOWN       (81)
#define  BUTTON_LEFT       (80)
#define  BUTTON_RIGHT      (79)
#define  BUTTON_DELETE     (76)

#define  BUTTON_PAGE_UP     (75)
#define  BUTTON_PAGE_DOWN   (78)
#define  BUTTON_FUNCTION_F5 (62)

#define  LAST_BUTTON_INDEX (0xFF)


uint16_t key_mapping_media[] = {BUTTON_STOP, BUTTON_PREVIOUS, BUTTON_NEXT, BUTTON_MPLAYER, \
                               BUTTON_VOLUME_P, BUTTON_PLAY, BUTTON_PAUSE, BUTTON_VOLUME_N, \
                               LAST_BUTTON_INDEX, LAST_BUTTON_INDEX, BUTTON_MUTE, LAST_BUTTON_INDEX};

uint8_t key_mapping_ppt[] = {BUTTON_ESC, BUTTON_UP, BUTTON_LEFT, BUTTON_RIGHT, \
                             BUTTON_TAB, BUTTON_DOWN, BUTTON_DELETE, BUTTON_MODE, \
                             BUTTON_PAGE_UP, BUTTON_PAGE_DOWN, BUTTON_FUNCTION_F5, LAST_BUTTON_INDEX};

uint8_t button_mode = BUTTON_MEDIA_MODE;

#define INTER_FRAME_DURATION_US     200000 // 200 ms
#define TX_OPTIONS  (TXO_UNICAST | TXO_DST_ADDR_NET | \
                     TXO_ACK_REQ | TXO_SEC_REQ | TXO_MULTI_CH | \
                     TXO_CH_NOT_SPEC | TXO_VEND_NOT_SPEC)

/* === GLOBALS ============================================================= */

FLASH_DECLARE(uint16_t VendorIdentifier) = (uint16_t)NWKC_VENDOR_IDENTIFIER;
FLASH_DECLARE(uint8_t vendor_string[7]) = NWKC_VENDOR_STRING;
FLASH_DECLARE(uint8_t app_user_string[15]) = APP_USER_STRING;

static node_status_t node_status;
static uint8_t pairing_ref = 0xFF;
#if (defined  RF4CE_CALLBACK_PARAM)
static zid_indication_callback_t zid_ind;

#endif
static uint8_t report_id = 0;
static uint8_t set_attribute_index= 0;
static void app_alert(void);
/* === PROTOTYPES ========================================================== */

static void app_task(void);
static void extended_delay_ms(uint16_t delay_ms);
static void indicate_fault_behavior(void);
#ifdef RF4CE_CALLBACK_PARAM
static void nlme_reset_confirm(nwk_enum_t Status);
static void nlme_start_confirm(nwk_enum_t Status);
static void app_nlme_rx_enable_confirm(nwk_enum_t Status);
static void zid_connect_confirm(nwk_enum_t Status, uint8_t PairingRef);
static void zid_report_data_confirm(nwk_enum_t Status, uint8_t PairingRef);
static void zid_data_confirm(nwk_enum_t Status, uint8_t PairingRef);
static void zid_report_data_indication(uint8_t PairingRef, uint8_t num_report_records,
                                                zid_report_data_record_t *zid_report_data_record_ptr, uint8_t RxLinkQuality, uint8_t RxFlags);
static void zid_get_report_indication(uint8_t PairingRef,zid_report_types_t zid_report_type, zid_report_desc_t zid_report_desc,
                                                uint8_t RxLinkQuality, uint8_t RxFlags);

static void zid_set_attribute_confirm(nwk_enum_t status,uint8_t PairingRef, zid_attribute_t ZIDAttribute, uint8_t ZIDAttributeIndex);

#endif

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Main function of the Single Button Controller application
 */
uint8_t b_event = 0xFF;
uint8_t b_state = 0xFF;

int main(void)
{
    irq_initialize_vectors();

    /* Initialize the board.
     * The board-specific conf_board.h file contains the configuration of
     * the board initialization.
     */
    board_init();    

    //sysclk_init();

    sw_timer_init();
    
    BSP_InitQTouch(appButtonsInd);
#ifdef QDEBUG 
//    while(1)
//    {
//      //If Touch Detected return will be true else false
//      if(Touch_measurement(&b_event, &b_state))
//      {
//        ;
//      }      
//    }
#endif
    
       
       
    if (nwk_init()!= NWK_SUCCESS)
    {
        app_alert();
    }

#ifdef RF4CE_CALLBACK_PARAM
    zid_ind.zid_report_data_indication_cb = zid_report_data_indication;
    zid_ind.zid_get_report_indication_cb = zid_get_report_indication;
    register_zid_indication_callback(&zid_ind);
#endif
    /*
     * The stack is initialized above,
     * hence the global interrupts are enabled here.
     */
     cpu_irq_enable();

    
    // any button press after power on
    
    if(b_event)
    {
        // Force push button pairing
        /* Cold start */
        LED_On(LED0);
        node_status = COLD_START;
        nlme_reset_request(true
#ifdef RF4CE_CALLBACK_PARAM
                          ,(FUNC_PTR)nlme_reset_confirm
#endif
                           );
    }
    else
    {
        /* Warm start */
        node_status = WARM_START;
        nlme_reset_request(false
#ifdef RF4CE_CALLBACK_PARAM
                          ,(FUNC_PTR)nlme_reset_confirm
#endif
                           );
    }

    /* Endless while loop */
    while (1)
    {
        //Touch_measurement(&b_event, &b_state);
        app_task(); /* Application task */
        nwk_task(); /* RF4CE network layer task */
    }
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
        /* Set power save mode: sleep */
        nlme_rx_enable_request(0/*nwkcMinActivePeriod*/
#ifdef RF4CE_CALLBACK_PARAM
                           ,(FUNC_PTR)app_nlme_rx_enable_confirm
#endif

                           );
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
          app_alert();
        }
    }

    if(node_status == COLD_START)
    {
        uint8_t value= 10;
        zid_set_attribute_request(0xFF, aplHIDNumStdDescComps, 0, &value
#ifdef RF4CE_CALLBACK_PARAM
                               , (FUNC_PTR)zid_set_attribute_confirm
#endif
                              );
        node_status = CONFIGURING_ATTRIBUTES;
    }

}


static void zid_set_attribute_confirm(nwk_enum_t status,uint8_t PairingRef, zid_attribute_t ZIDAttribute, uint8_t ZIDAttributeIndex)
{
   if(status == NWK_SUCCESS)
   {
       if(ZIDAttribute == aplHIDStdDescCompsList)
       {
          if(ZIDAttributeIndex >= 9)
          {
              set_attribute_index = 0;
              LED_Off(LED0);
              

              node_status = CONNECTING;

              dev_type_t OrgDevTypeList[1];;
              profile_id_t OrgProfileIdList[1];
              profile_id_t DiscProfileIdList[1];

              OrgDevTypeList[0] = DEV_TYPE_REMOTE_CONTROL;
              OrgProfileIdList[0] = PROFILE_ID_ZID;
              DiscProfileIdList[0] = PROFILE_ID_ZID;

              zid_org_connect_request(APP_CAPABILITIES, OrgDevTypeList, OrgProfileIdList,
                                   DEV_TYPE_WILDCARD, NUM_SUPPORTED_PROFILES, DiscProfileIdList
#ifdef RF4CE_CALLBACK_PARAM
                                    ,(FUNC_PTR)zid_connect_confirm
#endif
                                     );
          }
          else
          {
              set_attribute_index++;
          }
       }
       else
       {

           //node_status = IDLE;
       }
   }
   else
   {
        while (1)
        {
            app_alert();
        }
   }
   PairingRef = PairingRef;
   ZIDAttribute = ZIDAttribute;
   ZIDAttributeIndex =ZIDAttributeIndex;
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
void zid_connect_confirm(nwk_enum_t Status, uint8_t PairingRef)
{
    if (Status != NWK_SUCCESS)
    {
        while(1)
        {
           app_alert();
        }
    }

    pairing_ref = PairingRef;

    /* Set power save mode */
    //nlme_rx_enable_request(0x00/*nwkcMinActivePeriod*/);

    if (Status != NWK_SUCCESS)
    {
        while(1)
        {
            indicate_fault_behavior();
        }
    }

    if (node_status == CONNECTING)
    {
        node_status = IDLE;
        zid_state = ZID_STATE_IDLE;

        /* LED handling */
        LED_On(LED0);
        extended_delay_ms(1000);
        LED_Off(LED0);
    }

}



void zid_heartbeat_confirm(nwk_enum_t Status, uint8_t PairingRef)
{
    /* Keep compiler happy. */
    Status = Status;
    PairingRef = PairingRef;
}

#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void zid_report_data_indication(uint8_t PairingRef,uint8_t num_report_records,
                                                zid_report_data_record_t *zid_report_data_record_ptr, uint8_t RxLinkQuality, uint8_t RxFlags)
{

    PairingRef = PairingRef;
    num_report_records = num_report_records;
    zid_report_data_record_ptr = zid_report_data_record_ptr;
    RxLinkQuality = RxLinkQuality;
    RxFlags = RxFlags;

}

#if 1
/*
 * The NLME-RX-ENABLE.confirm primitive reports the results of the attempt to
 * enable or disable the receiver.
 */
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void app_nlme_rx_enable_confirm(nwk_enum_t Status)
{
    if (Status != NWK_SUCCESS)
    {
        while(1)
        {
            indicate_fault_behavior();
        }
    }

    if (node_status == COLD_START)
    {
        node_status = IDLE;

        /* LED handling */
        LED_On(LED0);
        extended_delay_ms(1000);
        LED_Off(LED0);
    }
    else if (node_status == WARM_START)
    {
        node_status = IDLE;

         LED_On(LED0);
         extended_delay_ms(250);
         LED_Off(LED0);
    }
}
#endif


/**
 * @brief Application task and state machine
 */
static void app_task(void)
{
    switch (node_status)
    {
        case IDLE:
            {                
                static uint32_t current_time;
                static uint32_t previous_button_time;
                uint8_t num_records = 1;
                           
                if (Touch_measurement(&b_event, &b_state)/*num_records != 1*/)
                {                  

                    current_time= sw_timer_get_time();
                    if ((current_time - previous_button_time) < INTER_FRAME_DURATION_US)
                    {
                        return;
                    }
                    else
                    {
                        /* Store current time */
                        previous_button_time = current_time;
                    }              

                    
                    if(key_mapping_media[b_state] != LAST_BUTTON_INDEX)
                    {
                      zid_report_data_record_t zid_report_data[2];
                      uint8_t report_data_buffer[80];
                      uint8_t *msg_ptr = &report_data_buffer[0];

                      zid_report_data[0].report_type = INPUT;
                      zid_report_data[0].report_desc_identifier = KEYBOARD;
                      zid_report_data[0].report_data = (void *)msg_ptr;

                      keyboard_input_desc_t *keyboard_input_desc;
                      keyboard_input_desc = (keyboard_input_desc_t *)msg_ptr;

                      keyboard_input_desc->modifier_keys = 0x00;
                      keyboard_input_desc->key_code[0] = 0x00;
                      keyboard_input_desc->key_code[1] = 0x00;
                      keyboard_input_desc->key_code[2] = 0x00;
                      keyboard_input_desc->key_code[3] = 0x00;
                      keyboard_input_desc->key_code[4] = (uint8_t)key_mapping_media[b_state];
                      keyboard_input_desc->key_code[5] = (uint8_t)(key_mapping_media[b_state] >> 8);
                      //report_id = 1;
                      num_records = 1;
                      
                      if (zid_report_data_request(pairing_ref,num_records, zid_report_data, TX_OPTIONS
  #ifdef RF4CE_CALLBACK_PARAM
                                                ,(FUNC_PTR)zid_report_data_confirm
  #endif
                        ))

                      {
                          node_status = TRANSMITTING;
                      }
                    }
                }
                else //(button == BUTTON_OFF)
                {
                    if (nwk_stack_idle())
                    {
                        /* Set MCU to sleep */
                       // pal_sleep_mode(SLEEP_MODE_PWR_SAVE);
                        /* MCU is awake again */
                    }
                }
            }
            break;
        case CONFIGURING_ATTRIBUTES:
          {
               uint8_t value[10]= {MOUSE,KEYBOARD,CONTACT_DATA,TAP_GESTURE,SCROLL_GESTURE,PINCH_GESTURE,ROTATE_GESTURE,SYNC,TOUCH_SENSOR_PROPERTIES,TAP_SUPPORT_PROPERTIES};
               zid_set_attribute_request(0xFF, aplHIDStdDescCompsList, set_attribute_index, &value[set_attribute_index]
#ifdef RF4CE_CALLBACK_PARAM
                               , (FUNC_PTR) zid_set_attribute_confirm
#endif
                              );
          }

        default:
            break;
    }
}

#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void zid_report_data_confirm(nwk_enum_t Status, uint8_t PairingRef)
{
    node_status = IDLE;

    if(report_id < 2)
    {
      report_id++;
    }
    else
    {
       report_id = 0;
    }

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
}

#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void zid_data_confirm(nwk_enum_t Status, uint8_t PairingRef)
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
}


static void zid_get_report_indication(uint8_t PairingRef,zid_report_types_t zid_report_type, zid_report_desc_t zid_report_desc,
                                                uint8_t RxLinkQuality, uint8_t RxFlags)
{
    PairingRef = PairingRef;
    RxLinkQuality = RxLinkQuality;
    zid_report_type = zid_report_type;
    zid_report_desc = zid_report_desc;
    RxFlags = RxFlags;
    /* zid_report_data_request should be used to respond to this indication */
    /* num_report_data_records will be one */
    mouse_desc_t mouse_desc;
    zid_report_data_record_t zid_report_data;
    //zid_report_data_record_t *zid_report_data_ptr[1];
    zid_report_data.report_type = INPUT;
    zid_report_data.report_desc_identifier = MOUSE;
    zid_report_data.report_data = (void *)&mouse_desc;
    mouse_desc.button0 = true;
    mouse_desc.button1 = true;
    mouse_desc.button2 = true;
    mouse_desc.x_coordinate = 0x11;
    mouse_desc.y_coordinate = 0x22;
    //zid_report_data_ptr[0] = &zid_report_data;

                    ///cmd[0] = INPUT; // report type
                    ///cmd[1] = KEYBOARD;   // report identifier
    if(node_status == IDLE)
    {
        if (zid_report_data_request(pairing_ref,1, &zid_report_data, TX_OPTIONS
#ifdef RF4CE_CALLBACK_PARAM
                                              ,(FUNC_PTR)zid_data_confirm
#endif
                                      ))

        {
                        node_status = TRANSMITTING;
        }

    }
}
/*
 * The NLDE-DATA.confirm primitive is generated by the NWK layer entity in
 * response to an NLDE-DATA.request primitive.
 */
void nlde_data_confirm(nwk_enum_t Status, uint8_t PairingRef, profile_id_t ProfileId)
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
}

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
/* --- Helper functions ---------------------------------------------------- */


/**
 * @brief Extended blocking delay
 *
 * @param delay_ms Delay value in ms
 */
static void extended_delay_ms(uint16_t delay_ms)
{
    uint16_t i;
    uint16_t timer_delay;

    timer_delay = delay_ms / 50;
    for (i = 0; i < timer_delay; i++)
    {
        delay_ms(5);
    }
}


/**
 * @brief Indicating malfunction
 */
static void indicate_fault_behavior(void)
{
    uint8_t i;

    for (i = 0; i < 10; i++)
    {
          LED_On(LED0);
          extended_delay_ms(200);
          LED_Off(LED0);
        
    }
}


/**
 * @brief Vendor-spefic callback; handles reaction to incoming alive request
 */
void vendor_app_alive_req(void)
{
    /* Variant to demonstrate FOTA featue */

    LED_On(LED0);
    delay_ms(500);
    LED_Off(LED0);
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
