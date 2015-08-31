/**
 ****************************************************************************************
 *
 * @file timer.h
 *
 * @brief SAMB11 Timer API for Applications
 *
 * This module contains the public API and the necessary enumerations and structures that are  
 * required for SAMB11 Application Developers for using platform driver like GPIO,SPI,I2C etc.
 *
 *
 *  Copyright (c) 2014 Atmel Corporation. All rights reserved.
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 *  3. The name of Atmel may not be used to endorse or promote products derived from this software 
 *  without specific prior written permission.
 *
 *  4. This software may only be redistributed and used in connection with an Atmel microcontroller product.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
  * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
 *  THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 *  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************************
 */

#ifndef TIMER_DRV_H_INCLUDED
#define TIMER_DRV_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */
typedef void (*timer_callback) ();

#define USER_TIMER_CALLBACK		0x41

/** Enum for the possible callback types for the timer module. */
enum timer_callback_type
{
	/** Callback for timer expiry*/
	TIMER_EXPIRED_CALLBACK_TYPE_DETECT = 1,
};

/** Enum for the possible state for timer module. */
typedef enum TIMER_STATE
{
	STATE_NOT_INITIALIZED = 0,
	STATE_IDLE	= 1,
	STATE_SCHEDULED,
}timer_state;

/** Enum for the possible status values returned from timer module. */
typedef enum TIMER_STATUS
{
    /** Timer Initialized and ready */
    TIMER_STATUS_OK = 0x00,
    /** Timer is shceduled and busy */
    TIMER_STATUS_BUSY,
		/** Timer not Initialized */
		TIMER_STATUS_NOT_INITIALIZED,
		/** Failed to initialize the Timer */
		TIMER_STATUS_INITIALIZATION_FAILED,
		/** Status Failure */
		TIMER_STATUS_FAILURE,
		/** Invalid Argument */
		TIMER_STATUS_INVALID_ARGUMENT,
}timer_drv_status;
	
/**
@defgroup timer-drv Timer Driver API

@{
*/
	
/** \name Configuration and initialization
 * @{
 */

/**
 * \brief Initializes the Timer.
 *
 * Initializes the Timer module for application usage.
 * \note Call this API before calling anyother timer module related API.
 *
 * \return Status of scheduling the timer.
 * \retval TIMER_STATUS_OK           			Timer Initialized successfully.
 */
timer_drv_status timer_init(void);

/**
 * \brief Stops the timer.
 *
 * Stops the timer module.
 *
 */
void timer_stop();

/**
 * \brief Starts the timer with give timeout value.
 *
 * Starts the timer with given timeout value. Timeout value is specified in 
 * units of seconds. 
 *
 * \param[in] timeout  timeout value in units of seconds.
 * \param[in] periodic whether timer needs to run in periodic or one shot.
 *
 * \return Status of scheduling the timer.
 * \retval TIMER_STATUS_OK        Timer scheduled successfully.
 * \retval TIMER_STATUS_BUSY			Another timer is already scheduled.
 */
timer_drv_status timer_start(uint32_t timeout, uint8_t periodic);

/**
 * \brief Registers a callback function for timer expiry.
 *
 * Registers a callback function for timer expiry that will be called when
 * when timer is expired.
 * \param[in] fp Callback function of type \ref timer_callback.
 *
 * \return Status of registering the callback.
 * \retval TIMER_STATUS_OK                Callback registered successfully.
 * \retval TIMER_STATUS_INVALID_ARGUMENT	Not a valid callback function pointer.
 */
timer_drv_status timer_register_callback(timer_callback fp);

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif	//TIMER_DRV_H_INCLUDED
