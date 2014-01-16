/************************************************************************//**
  \file avr477QTouch.h

  \brief
    The header file is for the AVR477 QTouch.c

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
    History:
    10.09.21 N. FOMIN - Created.
******************************************************************************/

#ifndef _AVR477QTOUCH_H
#define _AVR477QTOUCH_H

/******************************************************************************
                   Includes section
******************************************************************************/


/**************************************************************************//**
                    Types section
******************************************************************************/
typedef enum _BSP_TouchEvent_t
{
  TOUCHKEY_NO_EVENT,
  TOUCHKEY_CHANGED_EVENT,
  TOUCHKEY_PRESSED_EVENT,
  TOUCHKEY_RELEASED_EVENT,
  TOUCHKEY_THRESHOLD_REACHED_EVENT,
  TOUCHKEY_LEVEL_CHANGED_EVENT,
} BSP_TouchEvent_t;

typedef enum _BSP_ButtonEvent_t
{
  BUTTON_NO_EVENT,
  BUTTON_CHANGED_EVENT,
  BUTTON_PRESSED_EVENT,
  BUTTON_RELEASED_EVENT,
  BUTTON_THRESHOLD_REACHED_EVENT,
  BUTTON_LEVEL_CHANGED_EVENT,
} BSP_ButtonEvent_t;

/* Key-Button Mapping 
   These defines are basically a mapping between the keys on the AVR477 RF Touch
   Board and the buttons on the Red Key Remote Control Board.

   For Example, in present implementation user wants to send 'ON' command   
   when key SW16 is touched on AVR477 board. Since in red key remote control board,
   BUTTON_LEFT_P is used for this function, the KEY_SW16 is mapped to BUTTON_LEFT_P;

   This mapping needs to be changed, if user wants to change the intended 
   functionality when a key is touched.
*/

#define KEY_SW16        (0x00)
#define KEY_SW09        (0x01)
#define KEY_SW10        (0x02) 
#define KEY_SW17        (0x03) 
#define KEY_SW14        (0x04)
#define KEY_SW11        (0x05)
#define KEY_SW12        (0x06)
#define KEY_SW15        (0x07)
#define KEY_SW13        (0x08)
#define KEY_ROT_CW      (0x09)
#define KEY_ROT_CCW     (0x0A)

//#define KEY_SW01        (0x0C)
//#define KEY_SW02        (0x0D)
//#define KEY_SW03        (0x0E)
//#define KEY_SW04        (0x0F)
//#define KEY_SW05        (0x10)
//#define KEY_SW06        (0x11)
//#define KEY_SW07        (0x12)
//#define KEY_SW08        (0x13)
#define INVALID_KEY     (0x0B)


/** Type of buttons event handler */
typedef void (* BSP_TouchEventHandler_t)(BSP_TouchEvent_t event, uint8_t button, uint8_t data);

uint8_t Touch_measurement(uint8_t *button_event, uint8_t *button_state);
void BSP_InitQTouch(BSP_TouchEventHandler_t handler);
void appButtonsInd(BSP_TouchEvent_t event, uint8_t button, uint8_t data);

/**************************************************************************//**
\brief Set threshold value for a button
\param[in] button - button index
\param[in] threshold - value to set
******************************************************************************/
void BSP_SetButtonThreshold(uint8_t button, uint8_t threshold);

/**************************************************************************//**
\brief Determine if specified button is currently pressed.
\param[in] button - button index
\returns true - button is pressed
\returns false - button is released
******************************************************************************/
bool BSP_IsButtonPressed(uint8_t button);

/** Type of buttons event handler */
typedef void (* BSP_ButtonEventHandler_t)(BSP_ButtonEvent_t event, uint8_t button, uint8_t data);

#endif// _AVR477QTOUCH_H

//eof bspDbg.h
