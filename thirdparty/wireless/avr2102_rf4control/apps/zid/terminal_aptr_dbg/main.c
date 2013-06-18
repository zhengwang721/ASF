/**
 * @file main.c
 *
 * @brief RF4CE Serial Interface Application
 *
 * $Id: main.c 33720 2012-12-04 09:34:59Z agasthian.s $
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
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include "conf_usb.h"
#include "conf_board.h"
#include <asf.h>
#include "app_config.h"
#include "pal.h"
#include "led.h"
#include "delay.h"
#include "tal.h"
#include "vendor_data.h"
#include "pb_pairing.h"
#include "common_sw_timer.h"
#include "zid.h"


/* === Macros ============================================================== */
#define MAX_PAIRED_DEVICES  NWKC_MAX_PAIRING_TABLE_ENTRIES

#if (NO_OF_LEDS >= 3)
#define LED_START                       (LED0)
#define LED_NWK_SETUP                   (LED1)
#define LED_DATA                        (LED2)
#elif (NO_OF_LEDS == 2)
#define LED_START                       (LED0)
#define LED_NWK_SETUP                   (LED0)
#define LED_DATA                        (LED1)
#else
#define LED_START                       (LED0)
#define LED_NWK_SETUP                   (LED0)
#define LED_DATA                        (LED0)
#endif

#define MOUSE_MOVE_RANGE 3
/* === TYPES =============================================================== */
typedef enum node_status_tag
{
    IDLE = 0,
    RESETTING,
    POWER_SAVE,
    STARTING,
    ZID_CONNECTING,
    ALL_IN_ONE_START,
    WARM_STARTING,
    PRINTING_PAIRING_TABLE,
    UNPAIRING,
    CH_AGILITY_EXECUTION,
    BASE_CHANNEL_CHANGE,
    GETTING_CH_AG_NIBS
} SHORTENUM node_status_t;

/* === Globals ============================================================= */

#ifdef RF4CE_CALLBACK_PARAM
static zid_indication_callback_t zid_ind;
static nwk_indication_callback_t nwk_ind;
#endif

static uint8_t number_of_paired_dev = 0;
static node_status_t node_status;
/* This is used to find out the duplicate entry
 on receiving the pairing confirmation */
static bool duplicate_pair_entry = false;
static uint8_t target_auto_start = true;
static bool ch_ag_enabled = true;
static volatile bool main_b_kbd_enable = false;
static bool main_b_mouse_enable = false;
#define PAIR_WAIT_PERIOD    500000
/* Write application specific values into flash memory */
FLASH_DECLARE(uint16_t VendorIdentifier) = (uint16_t)NWKC_VENDOR_IDENTIFIER;
FLASH_DECLARE(uint8_t vendor_string[7]) = NWKC_VENDOR_STRING;
FLASH_DECLARE(uint8_t app_user_string[15]) = APP_USER_STRING;

static uint8_t led_timer;

/* === Prototypes ========================================================== */
#ifdef RF4CE_CALLBACK_PARAM
static void zid_indication_callback_init(void);
static void nlme_unpair_indication(uint8_t PairingRef);
static void zid_heartbeat_indication(uint8_t PairingRef);
static void zid_connect_confirm(nwk_enum_t Status, uint8_t PairingRef);
static void zid_report_data_indication(uint8_t PairingRef, uint8_t num_report_records,
                                                zid_report_data_record_t *zid_report_data_record_ptr, uint8_t RxLinkQuality, uint8_t RxFlags);

static void nlme_rx_enable_confirm(nwk_enum_t Status);
void zid_standby_leave_indication(void);
static void nlme_reset_confirm(nwk_enum_t Status);
static void nlme_start_confirm(nwk_enum_t Status);
static void nlme_set_confirm(nwk_enum_t Status, nib_attribute_t NIBAttribute, uint8_t NIBAttributeIndex);
static void nlme_get_confirm(nwk_enum_t Status, nib_attribute_t NIBAttribute,
                             uint8_t NIBAttributeIndex, void *NIBAttributeValue);
static void zid_standby_confirm(nwk_enum_t Status, bool StdbyEnable);
#endif

static void led_handling(void *callback_parameter);
static void app_task(void);
static void print_pairing_table(bool start_from_scratch, uint8_t *table_entry, uint8_t index);
static void led_handling(void *callback_parameter);
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
     sysclk_init();
     board_init();
   
     sw_timer_init();
     udc_start();
    //udc_start();
    if (nwk_init()!= NWK_SUCCESS)
    {
        app_alert();
    }

#ifdef RF4CE_CALLBACK_PARAM
    zid_indication_callback_init();
#endif
    /*
     * The stack is initialized above, hence the global interrupts are enabled
     * here.
     */
    cpu_irq_enable();
    
   sw_timer_get_id(&led_timer);

    /* Endless while loop */
    while (1)
    {    
        app_task(); /* Application task */
        nwk_task(); /* RF4CE network layer task */
    }
}

/**
 * @brief Application task
 */
static void app_task(void)
{
    if (target_auto_start == true)
    {
        target_auto_start = false;
        node_status = ALL_IN_ONE_START;
        ch_ag_enabled = false;
       nlme_reset_request(true
 #ifdef RF4CE_CALLBACK_PARAM
                                , (FUNC_PTR)nlme_reset_confirm
 #endif
                               );
        return;
    }
    else 
    {
         
                
    }/*end of else*/
       
}


/**
 * @brief Notify the application of the status of its request to reset the NWK
 *        layer.
 *
 * @param Status              nwk status
 */
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nlme_reset_confirm(nwk_enum_t Status)
{
   if (node_status == ALL_IN_ONE_START)
    {
        nlme_start_request(
 #ifdef RF4CE_CALLBACK_PARAM
                 (FUNC_PTR)nlme_start_confirm
 #endif
             );
    }
}

#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nlme_start_confirm(nwk_enum_t Status)
{
  if (node_status == ALL_IN_ONE_START)
    {
       sw_timer_start(T_LED_TIMER,
                        PAIR_WAIT_PERIOD,
                        TIMEOUT_RELATIVE,
                        (FUNC_PTR)led_handling,
                        NULL);
       
       dev_type_t RecDevTypeList[DEVICE_TYPE_LIST_SIZE];
       profile_id_t RecProfileIdList[PROFILE_ID_LIST_SIZE];

       RecDevTypeList[0] = (dev_type_t)SUPPORTED_DEV_TYPE_0;
       RecProfileIdList[0] = SUPPORTED_PROFILE_ID_0;
           
       node_status = ZID_CONNECTING;


       zid_rec_connect_request(APP_CAPABILITIES, RecDevTypeList, RecProfileIdList
#ifdef RF4CE_CALLBACK_PARAM
                                     , (FUNC_PTR)zid_connect_confirm
#endif
                                    );
    }
}



static void led_handling(void *callback_parameter)
{
     switch (node_status)
     {
         case ZID_CONNECTING:
         case ALL_IN_ONE_START:
             pal_timer_start(T_LED_TIMER,
                             500000,
                             TIMEOUT_RELATIVE,
                             (FUNC_PTR)led_handling,
                             NULL);
             LED_Toggle(LED_NWK_SETUP);
             break;

         default:
             pal_timer_stop(T_LED_TIMER);
             LED_Off(LED_DATA);
             LED_Off(LED_NWK_SETUP);
             break;
     }

     /* Keep compiler happy */
     callback_parameter = callback_parameter;
}

#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nlme_rx_enable_confirm(nwk_enum_t Status)
{
   

    /* Keep compiler happy */
    Status = Status;
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


#ifdef RF4CE_CALLBACK_PARAM
static void zid_indication_callback_init(void)
{
    zid_ind.zid_heartbeat_indication_cb = zid_heartbeat_indication;

    zid_ind.zid_report_data_indication_cb = zid_report_data_indication;
    register_zid_indication_callback(&zid_ind);
    nwk_ind.nlme_unpair_indication_cb = nlme_unpair_indication;
    zid_ind.zid_standby_leave_indication_cb = zid_standby_leave_indication;
    register_nwk_indication_callback(&nwk_ind);
}
#endif

#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void nlme_unpair_indication(uint8_t PairingRef)
{
  number_of_paired_dev--;
}

#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void zid_heartbeat_indication(uint8_t PairingRef)
{
    //printf("ZID-heartbeat from pairing ref:%d\r\n\r\n",PairingRef);
}


#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void zid_connect_confirm(nwk_enum_t Status, uint8_t PairingRef)
{  
  node_status = IDLE;
  if (Status == NWK_SUCCESS)
  {
        if (duplicate_pair_entry == false)
        {
            number_of_paired_dev++;
        }
        LED_On(LED0);
      // zid_standby_request(0x01,(FUNC_PTR)zid_standby_confirm);
  }
 
 // printf("ZID-connect status code:0x%x pairing ref:%d\r\n\r\n",Status,PairingRef);
}
//static void zid_standby_confirm(nwk_enum_t Status, bool StdbyEnable)
//{       
//  Status=Status;
//  StdbyEnable=StdbyEnable;
//  
//}
//void zid_standby_leave_indication(void)
//{
//  LED_Off(LED0);
//}
#ifdef RF4CE_CALLBACK_PARAM
static
#endif
void zid_report_data_indication(uint8_t PairingRef, uint8_t num_report_records,
                                                zid_report_data_record_t *zid_report_data_record_ptr, uint8_t RxLinkQuality, uint8_t RxFlags)
{          
   
                  
                  
     for(uint8_t i=0;i<num_report_records;i++)
     {  
    
         switch(zid_report_data_record_ptr->report_desc_identifier)
         {
         case MOUSE:
         {
            
             mouse_desc_t *mouse_desc;
             mouse_desc = (mouse_desc_t *)zid_report_data_record_ptr->report_data;
             uint8_t value;
             udi_hid_mouse_moveX((mouse_desc->x_coordinate));
             udi_hid_mouse_moveY((mouse_desc->y_coordinate));
             if(value=(mouse_desc->button0))
             { 
               udi_hid_mouse_btnleft(value);
             }
             if(value=(mouse_desc->button1))
             { 
               udi_hid_mouse_btnright(value);
             }
             if(value=(mouse_desc->button2))
             { 
               udi_hid_mouse_btnmiddle(value);
             }
             
          
             break;
         }
         case KEYBOARD:
         {
             if(zid_report_data_record_ptr->report_type == INPUT)
             {
                 //printf("Keyboard input desc.:\r\n");
                 keyboard_input_desc_t *keyboard_input_desc;
                 keyboard_input_desc = (keyboard_input_desc_t *)zid_report_data_record_ptr->report_data;
                    uint8_t k_value;
                    if(k_value = (keyboard_input_desc->modifier_keys))
                    {
                        udi_hid_kbd_modifier_down(k_value);
                    }  
                    for(uint8_t j=0;j<4;j++)
                    {  
                        if(k_value = (keyboard_input_desc->key_code[j]))
                        {    
                            udi_hid_kbd_down(k_value);
                            udi_hid_kbd_up(k_value);
                        }   
                   
                    }
                    uint16_t u_value;
                    u_value= (keyboard_input_desc->key_code[4])|(keyboard_input_desc->key_code[5]);
                    if(u_value)
                    {   
                        udi_hid_mkbd_modifier_down(u_value);
                    }
              
             }
             else
             {
               
             }
             break;
         }
         
         }
         zid_report_data_record_ptr++;
     }
//
    RxLinkQuality = RxLinkQuality;
    RxFlags = RxFlags;

}
void main_suspend_action(void)
{
	//ui_powerdown();
}

void main_resume_action(void)
{
	//ui_wakeup();
}
void main_sof_action(void)
{
	if (!main_b_kbd_enable)
		return;
	//ui_process(udd_get_frame_number());
}

void main_remotewakeup_enable(void)
{
	//ui_wakeup_enable();
}
bool main_mouse_enable(void)
{
	main_b_mouse_enable = true;
	return true;
}

void main_mouse_disable(void)
{
	main_b_mouse_enable = false;
}
void main_remotewakeup_disable(void)
{
	//ui_wakeup_disable();
}

bool main_kbd_enable(void)
{
	main_b_kbd_enable = true;
	return true;
}

void main_kbd_disable(void)
{
	main_b_kbd_enable = false;
}

/* Alert to indicate something has gone wrong in the application */
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
