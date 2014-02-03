/**
 * @file main.c
 *
 * @brief ZID Remote Key RC  Application
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
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


/* === TYPES =============================================================== */

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

#define INTER_FRAME_DURATION_US        (200000) // 200 ms
#define INTER_FRAME_DURATION_MOUSE_US  (5000)  //5ms

#define TX_OPTIONS  (TXO_UNICAST | TXO_DST_ADDR_NET | \
                     TXO_ACK_REQ | TXO_SEC_REQ | TXO_MULTI_CH | \
                     TXO_CH_NOT_SPEC | TXO_VEND_NOT_SPEC)

#define TOTAL_NO_OF_ZID_KEY_RC (27)


/* === GLOBALS ============================================================= */

FLASH_DECLARE(uint16_t VendorIdentifier) = (uint16_t)NWKC_VENDOR_IDENTIFIER;
FLASH_DECLARE(uint8_t vendor_string[7]) = NWKC_VENDOR_STRING;
FLASH_DECLARE(uint8_t app_user_string[15]) = APP_USER_STRING;

static node_status_t node_status;
static uint8_t pairing_ref = 0xFF;


#define  BUTTON_PLAY       (0x0080)
#define  BUTTON_PAUSE      (0x0100)
#define  BUTTON_MPLAYER    (0x0008)
#define  BUTTON_STOP       (0x0010)
#define  BUTTON_NEXT       (0x0040)
#define  BUTTON_PREVIOUS   (0x0020)
#define  BUTTON_VOLUME_P   (0x0002)
#define  BUTTON_VOLUME_N   (0x0004)
#define  BUTTON_MUTE       (0x0001)
#define  BUTTON_REPEAT     (0x0200)

#define  BUTTON_MOUSE_MODE (0xFB)
#define  BUTTON_PPT_MODE   (0xFC)
#define  BUTTON_GAME_MODE  (0xFD)
#define  BUTTON_MEDIA_MODE (0xFE)

#define  BUTTON_INVALID    (0xFF)


#define  BUTTON_ESC        (41)
#define  BUTTON_TAB        (43)
#define  BUTTON_UP_E       (82)
#define  BUTTON_DOWN_E     (81)
#define  BUTTON_LEFT_E     (80)
#define  BUTTON_RIGHT_E    (79)
#define  BUTTON_DELETE     (76)

#define  BUTTON_PAGE_UP     (75)
#define  BUTTON_PAGE_DOWN   (78)
#define  BUTTON_FUNCTION_F5 (62)
#define  BUTTON_LEFT_SINGLE_CLK  (90)
#define  BUTTON_RIGHT_SINGLE_CLK (91)
#define  BUTTON_SCROLL_UP        (92)
#define  BUTTON_SCROLL_DOWN      (93)
#define  BUTTON_MIDDLE_CLK       (94)


#define  LAST_BUTTON_INDEX (0xFF)

static uint16_t key_mapping_media[TOTAL_NO_OF_ZID_KEY_RC] =     {  BUTTON_REPEAT, /* BUTTON_F2_RED    */
                                                                   BUTTON_MOUSE_MODE, /* BUTTON_F3_GREEN  */
                                                                   BUTTON_PPT_MODE, /* BUTTON_F4_YELLOW */
                                                                   BUTTON_GAME_MODE, /* BUTTON_F1_BLUE   */
                                                                   BUTTON_MEDIA_MODE, /* Not valid for SELECT KEY */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_0 */
                                                                   BUTTON_INVALID, /* BUTTON_POWER_OFF_FUNCTION */
                                                                   BUTTON_INVALID, /* Invalid */
                                                                   BUTTON_INVALID, /* Invalid */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_1 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_2 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_3 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_4 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_5 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_6 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_7 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_8 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_9 */
                                                                   BUTTON_PLAY,    /* BUTTON_UP       */
                                                                   BUTTON_PREVIOUS, /* BUTTON_LEFT     */
                                                                   BUTTON_MPLAYER, /* BUTTON_ENTER    */
                                                                   BUTTON_NEXT,    /* BUTTON_RIGHT    */
                                                                   BUTTON_PAUSE, /* BUTTON_DOWN     */
                                                                   BUTTON_VOLUME_P, /* BUTTON_LEFT_UP  */
                                                                   BUTTON_VOLUME_N, /* BUTTON_LEFT_DOWN */
                                                                   BUTTON_MUTE, /* ZRC_BUTTON_RIGHT_UP */
                                                                   BUTTON_STOP /* ZRC_BUTTON_RIGHT_DOWN */
                                                                 };

static uint8_t key_mapping_ppt[TOTAL_NO_OF_ZID_KEY_RC] =        {  BUTTON_ESC, /* BUTTON_F2_RED    */
                                                                   BUTTON_MOUSE_MODE, /* BUTTON_F3_GREEN  */
                                                                   BUTTON_PPT_MODE, /* BUTTON_F4_YELLOW */
                                                                   BUTTON_GAME_MODE, /* BUTTON_F1_BLUE   */
                                                                   BUTTON_MEDIA_MODE, /* Not valid for SELECT KEY */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_0 */
                                                                   BUTTON_INVALID, /* BUTTON_POWER_OFF_FUNCTION */
                                                                   BUTTON_INVALID, /* Invalid */
                                                                   BUTTON_INVALID, /* Invalid */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_1 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_2 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_3 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_4 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_5 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_6 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_7 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_8 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_9 */
                                                                   BUTTON_UP_E, /* BUTTON_UP       */
                                                                   BUTTON_LEFT_E, /* BUTTON_LEFT     */
                                                                   BUTTON_FUNCTION_F5, /* BUTTON_ENTER    */
                                                                   BUTTON_RIGHT_E, /* BUTTON_RIGHT    */
                                                                   BUTTON_DOWN_E, /* BUTTON_DOWN     */
                                                                   BUTTON_TAB, /* BUTTON_LEFT_UP  */
                                                                   BUTTON_DELETE, /* BUTTON_LEFT_DOWN */
                                                                   BUTTON_PAGE_UP, /* ZRC_BUTTON_RIGHT_UP */
                                                                   BUTTON_PAGE_DOWN /* ZRC_BUTTON_RIGHT_DOWN */
                                                                 };

static uint8_t key_mapping_mouse[TOTAL_NO_OF_ZID_KEY_RC] =        {BUTTON_INVALID, /* BUTTON_F2_RED    */
                                                                   BUTTON_MOUSE_MODE, /* BUTTON_F3_GREEN  */
                                                                   BUTTON_PPT_MODE, /* BUTTON_F4_YELLOW */
                                                                   BUTTON_GAME_MODE, /* BUTTON_F1_BLUE   */
                                                                   BUTTON_MEDIA_MODE, /* Not valid for SELECT KEY */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_0 */
                                                                   BUTTON_INVALID, /* BUTTON_POWER_OFF_FUNCTION */
                                                                   BUTTON_INVALID, /* Invalid */
                                                                   BUTTON_INVALID, /* Invalid */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_1 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_2 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_3 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_4 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_5 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_6 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_7 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_8 */
                                                                   BUTTON_INVALID, /* BUTTON_NUMBER_9 */
                                                                   BUTTON_UP_E, /* BUTTON_UP       */
                                                                   BUTTON_LEFT_E, /* BUTTON_LEFT     */
                                                                   BUTTON_MIDDLE_CLK, /* BUTTON_ENTER    */
                                                                   BUTTON_RIGHT_E, /* BUTTON_RIGHT    */
                                                                   BUTTON_DOWN_E, /* BUTTON_DOWN     */
                                                                   BUTTON_LEFT_SINGLE_CLK, /* BUTTON_LEFT_UP  */
                                                                   BUTTON_RIGHT_SINGLE_CLK, /* BUTTON_LEFT_DOWN */
                                                                   BUTTON_SCROLL_UP, /* ZRC_BUTTON_RIGHT_UP */
                                                                   BUTTON_SCROLL_DOWN /* ZRC_BUTTON_RIGHT_DOWN */
                                                                 };

uint16_t button_mode = BUTTON_MEDIA_MODE;

uint32_t zid_interframe_duration = INTER_FRAME_DURATION_US;

#if (defined  RF4CE_CALLBACK_PARAM)
static zid_indication_callback_t zid_ind;
#endif

static uint8_t report_id = 0;
static uint8_t set_attribute_index= 0;
/* === PROTOTYPES ========================================================== */
static void app_alert(void);
uint8_t get_zid_keyrc_button(button_id_t button_id);
void key_rc_board_init(void);
static void app_task(void);
static void extended_delay_ms(uint16_t delay_ms);
static void indicate_fault_behavior(void);
//static key_state_t key_state_read(key_id_t key_no);
#ifdef RF4CE_CALLBACK_PARAM
static void nlme_reset_confirm(nwk_enum_t Status);
static void nlme_start_confirm(nwk_enum_t Status);
static void app_nlme_rx_enable_confirm(nwk_enum_t Status);
static void zid_connect_confirm(nwk_enum_t Status, uint8_t PairingRef);
static void zid_report_data_confirm(nwk_enum_t Status, uint8_t PairingRef);
static void zid_data_confirm(nwk_enum_t Status, uint8_t PairingRef);
//static void zid_heart_beat_confirm(nwk_enum_t Status, uint8_t PairingRef);
static void zid_report_data_indication(uint8_t PairingRef, uint8_t num_report_records,
                                                zid_report_data_record_t *zid_report_data_record_ptr, uint8_t RxLinkQuality, uint8_t RxFlags);
static void zid_get_report_indication(uint8_t PairingRef,zid_report_types_t zid_report_type, zid_report_desc_t zid_report_desc,
                                                uint8_t RxLinkQuality, uint8_t RxFlags);

static void zid_set_attribute_confirm(nwk_enum_t status,uint8_t PairingRef, zid_attribute_t ZIDAttribute, uint8_t ZIDAttributeIndex);

#endif

/* === IMPLEMENTATION ====================================================== */


button_id_t button_val;
/**
 * @brief Main function of the Single Button Controller application
 */
uint8_t b_state = 0xFF;
int main(void)
{
    uint8_t button_event = 0xFF;
    irq_initialize_vectors();

    /* Initialize the board.
     * The board-specific conf_board.h file contains the configuration of
     * the board initialization.
     */
    key_rc_board_init();
    

    sw_timer_init();
       
       
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

    
    if (button_event == 0xFF)
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
       pal_led(LED_1, LED_ON); 
       pal_led(LED_2, LED_OFF);
       pal_led(LED_3, LED_OFF);
       pal_led(LED_4, LED_OFF);
       pal_led(LED_5, LED_ON);       
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
                button_id_t button;                
                static uint32_t current_time;
                static uint32_t previous_button_time;
                uint8_t num_records = 1;
                static button_id_t previous_button;

                button = pal_button_scan();
                if ((button != BUTTON_UNKNOWN) && (button == previous_button))
                {
                    /* Check time to previous transmission. */

                     current_time= sw_timer_get_time();
                    if ((current_time - previous_button_time) < zid_interframe_duration)
                    {
                        return;
                    }
                    else
                    {
                       if(BUTTON_MOUSE_MODE == button_mode)
                       {
                         if(!((key_mapping_mouse[b_state] <= BUTTON_UP_E) &&  
                            (BUTTON_RIGHT_E >= key_mapping_mouse[b_state])))
                         {
                           if((current_time - previous_button_time) < INTER_FRAME_DURATION_MOUSE_US)
                           {
                            return;
                           }
                         }
                       }
                        /* Store current time */
                        previous_button_time = current_time;                        
                    }
                    
                    b_state = get_zid_keyrc_button(button);                    
                    
                    switch(key_mapping_media[b_state])
                    {
                    case BUTTON_MOUSE_MODE:
                       button_mode = BUTTON_MOUSE_MODE;
                       pal_led(LED_2, LED_ON);
                       pal_led(LED_3, LED_OFF);
                       pal_led(LED_4, LED_OFF);
                       pal_led(LED_5, LED_OFF);
                       zid_interframe_duration = INTER_FRAME_DURATION_MOUSE_US;
                       return;
                      break;
                      
                    case BUTTON_PPT_MODE:
                      button_mode = BUTTON_PPT_MODE;
                       pal_led(LED_2, LED_OFF);
                       pal_led(LED_3, LED_ON);
                       pal_led(LED_4, LED_OFF);
                       pal_led(LED_5, LED_OFF);
                       zid_interframe_duration = INTER_FRAME_DURATION_US;
                       return;
                      break;
                      
                    case BUTTON_GAME_MODE:
                      button_mode = BUTTON_GAME_MODE;
                       pal_led(LED_2, LED_OFF);
                       pal_led(LED_3, LED_OFF);
                       pal_led(LED_4, LED_ON);
                       pal_led(LED_5, LED_OFF);
                       zid_interframe_duration = INTER_FRAME_DURATION_US;
                       return;
                      break;
                      
                    case BUTTON_MEDIA_MODE:
                      button_mode = BUTTON_MEDIA_MODE;
                       pal_led(LED_2, LED_OFF);
                       pal_led(LED_3, LED_OFF);
                       pal_led(LED_4, LED_OFF);
                       pal_led(LED_5, LED_ON);
                       zid_interframe_duration = INTER_FRAME_DURATION_US;
                       return;
                      break;
                      
                    default:
                      pal_led(LED_1, LED_TOGGLE);
                      break;
                    } 
                    
                    if(button_mode == BUTTON_MEDIA_MODE)
                    {
                      zid_report_data_record_t zid_report_data[2];
                      uint8_t report_data_buffer[80];
                      uint8_t *msg_ptr = &report_data_buffer[0];
                      
                      if(key_mapping_media[b_state] == BUTTON_INVALID)
                      {
                        return;
                      }
                      
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
                    else if(button_mode == BUTTON_PPT_MODE)
                    {
                      zid_report_data_record_t zid_report_data[2];
                      uint8_t report_data_buffer[80];
                      uint8_t *msg_ptr = &report_data_buffer[0];
                      
                      if(key_mapping_ppt[b_state] == BUTTON_INVALID)
                      {
                        return;
                      }

                      zid_report_data[0].report_type = INPUT;
                      zid_report_data[0].report_desc_identifier = KEYBOARD;
                      zid_report_data[0].report_data = (void *)msg_ptr;

                      keyboard_input_desc_t *keyboard_input_desc;
                      keyboard_input_desc = (keyboard_input_desc_t *)msg_ptr;

                      keyboard_input_desc->modifier_keys = 0x00;
                      keyboard_input_desc->key_code[0] = key_mapping_ppt[b_state];
                      keyboard_input_desc->key_code[1] = 0x00;
                      keyboard_input_desc->key_code[2] = 0x00;
                      keyboard_input_desc->key_code[3] = 0x00;
                      keyboard_input_desc->key_code[4] = 0x00;
                      keyboard_input_desc->key_code[5] = 0x00;
                      num_records = 1;
                    if (zid_report_data_request(pairing_ref,num_records, zid_report_data, TX_OPTIONS
  #ifdef RF4CE_CALLBACK_PARAM
                                                ,(FUNC_PTR)zid_report_data_confirm
  #endif
                        ))

                      {
                          node_status = TRANSMITTING;
                          b_state = LAST_BUTTON_INDEX;
                      }
                    }
                    else if(button_mode == BUTTON_MOUSE_MODE)
                    {                         
                        zid_report_data_record_t zid_report_data[2];
                        uint8_t report_data_buffer[80];
                        uint8_t *msg_ptr = &report_data_buffer[0];
                        mouse_desc_t *mouse_desc;
                        
                        if(key_mapping_mouse[b_state] == BUTTON_INVALID)
                        {
                          return;
                        }
                      
                        zid_report_data[0].report_type = INPUT;
                        zid_report_data[0].report_desc_identifier = MOUSE;
                        zid_report_data[0].report_data = (void *)msg_ptr;                        
                        mouse_desc = (mouse_desc_t *)msg_ptr;
                        
                        mouse_desc->button0 = 0x00;
                        mouse_desc->button1 = 0x00;
                        mouse_desc->button2 = 0x00;
                        mouse_desc->x_coordinate = 0x00;
                        mouse_desc->y_coordinate = 0x00;

                          switch(key_mapping_mouse[b_state])
                          {
                          case BUTTON_UP_E:
                             mouse_desc->y_coordinate = -2;
                            break;
                          case BUTTON_LEFT_E:
                             mouse_desc->x_coordinate = -2;
                            break;
                          case BUTTON_RIGHT_E:
                            mouse_desc->x_coordinate = 2;
                            break;
                          case BUTTON_DOWN_E:
                            mouse_desc->y_coordinate = 2;
                            break;
                          case BUTTON_LEFT_SINGLE_CLK:
                            mouse_desc->button0 = 0x01;
                            break;
                          case BUTTON_RIGHT_SINGLE_CLK:
                            mouse_desc->button1 = 0x01;
                            break;
                          case BUTTON_MIDDLE_CLK:
                            mouse_desc->button2 = 0x01;
                            break;                            
                          case BUTTON_SCROLL_UP:
                            mouse_desc->y_coordinate = 1;
                            mouse_desc->button2 = 0x80;
                            break;
                          case BUTTON_SCROLL_DOWN:
                            mouse_desc->x_coordinate = -1;
                            mouse_desc->button2 = 0x80;
                            break;
                          default:
                            break;
                          }                   
                       

                        msg_ptr += sizeof(mouse_desc_t);
                        num_records = 1;
                    if (zid_report_data_request(pairing_ref, num_records, zid_report_data, TX_OPTIONS
  #ifdef RF4CE_CALLBACK_PARAM
                                                ,(FUNC_PTR)zid_report_data_confirm
  #endif
                        ))

                      {
                          node_status = TRANSMITTING;
                          b_state = LAST_BUTTON_INDEX;
                      }              
                    }
                    else if(button_mode == BUTTON_GAME_MODE)
                    {
                      
                      if(key_mapping_ppt[b_state] == BUTTON_INVALID)
                      {
                        return;
                      }
                      
                    }
                }
                else 
                {
                    previous_button = button;
                    previous_button_time = sw_timer_get_time();
                    if (nwk_stack_idle())
                    {
                        /* Set MCU to sleep */
                        /* Configure the wakeup source and sleep */
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
    zid_report_data.report_type = INPUT;
    zid_report_data.report_desc_identifier = MOUSE;
    zid_report_data.report_data = (void *)&mouse_desc;
    mouse_desc.button0 = true;
    mouse_desc.button1 = true;
    mouse_desc.button2 = true;
    mouse_desc.x_coordinate = 0x11;
    mouse_desc.y_coordinate = 0x22;
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
		//delay_us(0xFFFF);
	}
}

uint8_t get_zid_keyrc_button(button_id_t button_id)
{
    uint8_t i;
    for (i = 0; i < TOTAL_NO_OF_ZID_KEY_RC; i++)
    {
        if (button_id & ((button_id_t)1 << i))
        {
            return i;
        }
    }
    return (0xFF);
}


#include "pal.h"


void key_rc_board_init(void)
{ 
    /* Initialize buttons and LEDs */
    pal_button_init();
    pal_led_init();
}



/*******Atmega256RFR2 RCB *****************/
static uint8_t latch_status = 0xDF;
static inline void pulse_latch(void);
static void set_button_pins_for_normal_mode(void);

/**
 * @brief Pulse latch
 */
static inline void pulse_latch(void)
{
    uint8_t data_port;
    uint8_t data_dir;

    /* Store previous values */
    data_port = LATCH_DATA;
    data_dir = LATCH_DATA_DIR;

    /* Apply latch pulse to set LED status */

    LATCH_DATA_DIR = 0xFF;

    LATCH_DATA = latch_status;
    LATCH_PULSE();

    /* Re-store previous values */
    LATCH_DATA = data_port;
    LATCH_DATA_DIR = data_dir;
}


/**
 * @brief Initialize LEDs
 */
void pal_led_init(void)
{
    /* LCD initialization for inactive use */
    // LCD CS: out, high
    PORTE |= 1 << PE4;
    DDRE |=  1 << PE4;
    // LCD RST: out, high
    PORTE |= 1 << PE2;
    DDRE |= 1 << PE2;
    // PE3, PE6, PE7: out, high
    PORTE |= ((1 << PE3) | (1 << PE6) | (1 << PE7));
    DDRE |= (1 << PE3) | (1 << PE6) | (1 << PE7);

    // Unused pin
    DDRG &= ~(1 << PG2);
    PORTG |= 1 << PG2;

    LATCH_INIT();

    /* Switch all LEDs off, low active */
    latch_status |= (1 << LED_1_PIN) | (1 << LED_2_PIN) | (1 << LED_3_PIN)
                    | (1 << LED_4_PIN) | (1 << LED_5_PIN);

    /* Apply latch pulse to set LED status */
    pulse_latch();
}


/**
 * @brief Control LED status
 *
 * @param[in]  led_no LED ID
 * @param[in]  led_setting LED_ON, LED_OFF, LED_TOGGLE
 */
void pal_led(led_id_t led_no, led_action_t led_setting)
{
    switch (led_setting)
    {
            /* low active LEDs */
        case LED_ON:
            latch_status &= ~(1 << led_no);
            break;
        case LED_OFF:
            latch_status |= (1 << led_no);
            break;
        case LED_TOGGLE:
        default:
            if (latch_status & (1 << led_no))
            {
                latch_status &= ~(1 << led_no);
            }
            else
            {
                latch_status |= (1 << led_no);
            }
            break;
    }

    pulse_latch();
}

/**
 * @brief Initialize the buttons
 */
void pal_button_init(void)
{
    /* Button at IRQ lines are inputs */
    BUTTON_IRQ_PORT_DIR &= ~((1 << BUTTON_IRQ_PIN_1) | (1 << BUTTON_IRQ_PIN_2)
                             | (1 << BUTTON_IRQ_PIN_3));
    /* Enable pull-ups */
    BUTTON_IRQ_PORT |= ((1 << BUTTON_IRQ_PIN_1) | (1 << BUTTON_IRQ_PIN_2)
                        | (1 << BUTTON_IRQ_PIN_3));

    set_button_pins_for_normal_mode();

    /* Initialize the IRQ lines' interrupt behavior. */
    DISABLE_ALL_BUTTON_IRQS();
}


#define MAX_KEY_SCANS           6
#define NUM_OF_IDENTICAL_KEYS   3
#define INTER_BUTTON_SCAN_DELAY 100
#define WAKEUP_DEBOUNCE_DELAY   5000


/**
 * @brief Button handling
 *
 * @return button_no
 * @ingroup apiPalApi
 */
button_id_t pal_button_scan(void)
{
    uint32_t ret_val = 0;
    uint8_t i, k, r;
    uint8_t pin_no;
    uint8_t pin_b, pin_d;
    uint32_t result[NUM_OF_IDENTICAL_KEYS];
    for (r = 0; r < NUM_OF_IDENTICAL_KEYS; r++)
    {
        result[r] = 0x80000000;  // indicating unused entry
    }

    for (k = 0; k < MAX_KEY_SCANS; k++)
    {
        ret_val = 0;

        for (i = 1; i < 4; i++)
        {
            /* Set IRQ pin to output, low */
            BUTTON_IRQ_PORT_DIR |= (1 << i);
            BUTTON_IRQ_PORT &= ~(1 << i);
            nop();
            pal_timer_delay(1);  /* wait until next rising edge changes input state */
            pin_b = PINB;
            pin_d = PIND;
            if ((pin_b & 0x7F) != 0x7F) /* Any of the pins are pressed. */
            {

                if ((pin_b & (1 << BUTTON_PIN_0)) == 0)
                {
                    pin_no = 0;
                    ret_val |= (((uint32_t)1 << (((i - 1) * 9) + pin_no)));
                }
                if ((pin_b & (1 << BUTTON_PIN_1)) == 0)
                {
                    pin_no = 1;
                    ret_val |= (((uint32_t)1 << (((i - 1) * 9) + pin_no)));
                }
                if ((pin_b & (1 << BUTTON_PIN_2)) == 0)
                {
                    pin_no = 2;
                    ret_val |= (((uint32_t)1 << (((i - 1) * 9) + pin_no)));
                }
                if ((pin_b & (1 << BUTTON_PIN_3)) == 0)
                {
                    pin_no = 3;
                    ret_val |= (((uint32_t)1 << (((i - 1) * 9) + pin_no)));
                }
                if ((pin_b & (1 << BUTTON_PIN_4)) == 0)
                {
                    pin_no = 4;
                    ret_val |= (((uint32_t)1 << (((i - 1) * 9) + pin_no)));
                }
                if ((pin_b & (1 << BUTTON_PIN_5)) == 0)
                {
                    pin_no = 5;
                    ret_val |= (((uint32_t)1 << (((i - 1) * 9) + pin_no)));
                }
                if ((pin_b & (1 << BUTTON_PIN_6)) == 0)
                {
                    pin_no = 6;
                    ret_val |= (((uint32_t)1 << (((i - 1) * 9) + pin_no)));
                }
            }
            if ((pin_d & (1 << BUTTON_PIN_7)) == 0)
            {
                pin_no = 7;
                ret_val |= (((uint32_t)1 << (((i - 1) * 9) + pin_no)));
            }
            if ((pin_d & (1 << BUTTON_PIN_8)) == 0)
            {
                pin_no = 8;
                ret_val |= (((uint32_t)1 << (((i - 1) * 9) + pin_no)));
            }

            /* Reset current pin */
            BUTTON_IRQ_PORT_DIR &= ~(1 << i);
            BUTTON_IRQ_PORT |= (1 << i);

        }

        /* Debouncing: Check if key value is reproducible */
        for (r = 0; r < NUM_OF_IDENTICAL_KEYS; r++)
        {
            if (result[r] == ret_val)
            {
                if (r == (NUM_OF_IDENTICAL_KEYS - 1))
                {
                    return ret_val;
                }
            }
            else
            {
                result[r] = ret_val;
                break;
            }
        }

        //pal_timer_delay(INTER_BUTTON_SCAN_DELAY);
    }
    return ret_val;
}


/**
 * @brief Button pins setting for active/normal mode
 */
static void set_button_pins_for_normal_mode(void)
{
    /* input */
    BUTTON_PORT1_DIR &= 0x80;
    BUTTON_PORT2_DIR &= ~((1 << BUTTON_PIN_7) | (1 << BUTTON_PIN_8));
    /* pull-up */
    BUTTON_PORT1 |= 0x7F;
    BUTTON_PORT2 |= (1 << BUTTON_PIN_7) | (1 << BUTTON_PIN_8);
}


/* EOF */
