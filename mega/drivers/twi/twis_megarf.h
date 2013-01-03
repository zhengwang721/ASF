/**
 *
 * \file
 *
 * \brief Definition for TWI Slave driver for megarf.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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
#ifndef _TWI_MEGARF_SLAVE_H_
#define _TWI_MEGARF_SLAVE_H_

#include "status_codes.h"
#include "twi_common.h"
/**
 * \ingroup group_megarf_drivers_twi
 * \defgroup group_megarf_drivers_twis twi slave Driver
 *
 * This driver is the implementation of TWI module of megaRF device as slave.
 * Master initiates and terminates a data transmission.In the Slave Transmitter
 * mode, a number of data bytes are transmitted to a Master. In the Slave Receiver
 * mode, a number of data bytes are received from a Master. This uses two 
 * bi-directional bus lines(PD0 and PD1), one to receive clock (SCL) from 
 * master and one for data (SDA)
 *
 * \section megarf_twis_configuration Configuration
 * Configuration is done in the configuration file : conf_twis.h
 *
 * Configuration defines:
 * - \ref SLAVE_BUS_ADDR   : TWI slave bus address to respond to master
 *
 * - \ref TWI_GCE : TWI slave general call enable. TWI will respond to the
 *                  general call address (0x00), otherwise it will ignore
 *                  the general call address
 *
 * \section megarf_twis_interface Interface
 * The twi module should be configured as slave. The slave address is set
 * in TWAR register. The upper 7 bits are the address to which the 2-wire Serial
 * Interface will respond when addressed by a Master. If the LSB is set(TWI_GCE), 
 * the TWI will respond to the general call address (0x00), otherwise it will
 * ignore the general call address.
 * Enabling of TWI as slave is done by calling the function:
 * \code twi_slave_start() \endcode
 *
 * \subsection megarf_twis_usage Usage
 * The include file needed is:
 * \code #include "twis_megarf.h" \endcode
 * \code #include "twi_common.h" \endcode
 *
 * After initiating the twi as slave,the TWI waits until it is addressed by its
 * own slave address (or the general call address if enabled) followed by the
 * data direction bit. If the direction bit is “1” (read), the TWI will operate in Slave
 * transmit mode, otherwise Slave receive mode is entered
 *
 * The status and state is indicated to the application code through the
 * following functions
 * \code twi_slave_status_get() and twi_slave_state_get() \endcode
 * @{
 */

/**
 * \internal
 * \def TWI_STATE
 * \brief states of the twi slave transaction
 */
#define TWI_STATE_IDLE                    0
#define TWI_STATE_BUSY                    1

/**
 * \internal
 * \def TWI_STATUS_CODES
 * \brief TWI Slave status codes
 */
enum {
	TWS_BUS_ERROR                  = 0x00, /* ! Bus error due to an illegal
	                                       * START or STOP condition */
	TWS_ST_SLA_ACK                                = 0xA8, /* ! Own SLA+R has
	                                                       * been received;
	                                                       * ACK has been
	                                                       * returned */
	TWS_ST_SLA_ACK_M_ARB_LOST         = 0xB0, /* ! Arbitration lost in
	                                           * SLA+R/W as Master; own
	                                           * SLA+R has been received;
	                                           * ACK has been returned */
	TWS_ST_DATA_ACK                       = 0xB8, /* ! Data byte in TWDR has
	                                               * been transmitted; ACK
	                                               * has been received */
	TWS_ST_DATA_NACK              = 0xC0, /* ! Data byte in TWDR has been
	                                       * transmitted; NOT ACK has been
	                                       * received */
	TWS_ST_DATA_ACK_LAST_BYTE     = 0xC8, /* ! Last data byte in TWDR has
	                                       * been transmitted (TWEA = “0”);
	                                       * ACK has been received */
	TWS_SR_SLA_ACK                            = 0x60, /* ! Own SLA+W has
	                                                   * been received ACK
	                                                   * has been returned */
	TWS_SR_SLA_ACK_M_ARB_LOST         = 0x68, /* ! Arbitration lost in
	                                           * SLA+R/W as Master; own
	                                           * SLA+W has been received;
	                                           * ACK has been returned */
	TWS_SR_GEN_ACK                            = 0x70, /* ! General call
	                                                   * address has been
	                                                   * received; ACK has
	                                                   * been returned */
	TWS_SR_GEN_ACK_M_ARB_LOST     = 0x78, /* ! Arbitration lost in SLA+R/W
	                                       * as Master; General call address
	                                       * has been received; ACK has been
	                                       * returned */
	TWS_SR_SLA_DATA_ACK                   = 0x80, /* ! Previously addressed
	                                               * with own SLA+W; data
	                                               * has been received; ACK
	                                               * has been returned */
	TWS_SR_SLA_DATA_NACK              = 0x88, /* ! Previously addressed with
	                                           * own SLA+W; data has been
	                                           * received; NOT ACK has been
	                                           * returned */
	TWS_SR_GEN_DATA_ACK           = 0x90, /* ! Previously addressed with
	                                       * general call; data has been
	                                       * received; ACK has been returned */
	TWS_SR_GEN_DATA_NACK          = 0x98, /* ! Previously addressed with
	                                       * general call; data has been
	                                       * received; NOT ACK has been
	                                       * returned */
	TWS_SR_STOP_RESTART           = 0xA0 /* ! A STOP condition or repeated
	                                      * START condition has been
	                                      * received while still addressed
	                                      * as Slave */
};



/**
 * \brief Get the State of TWI transceiver
 *
 * \return int - status information
 */
int twi_slave_status_get(void);

/**
 * \brief Get the status of TWI transceiver
 *
 * \return int - state information
 */
int twi_slave_state_get(void);

/**
 * \brief Initialize TWI as Slave
 *
 * \param TWI_ownAddress - contains Slave own Address
 */
status_code_t twi_slave_init(uint8_t twi_slave_ownadd);

/**
 * \brief Start the slave Transceiver
 *
 * \return status_code_t - status of twi slave
 */
status_code_t twi_slave_start(slave_data_buffer_t *package);

/**
 * \brief Resets the slave state and status to initial for next
 *transmission/reception
 */
void twi_slave_status_reset(void);

/** 
 * \brief slave interrupt service routine.
 *  Interrupt service routine for the TWI slave. 
 */
void twi_slave_interrupt_handler(void);

/*! TWI slave interrupt service routine.
 *
 *  Interrupt service routine for the TWI slave. Copy the interrupt vector
 *  into your code if needed.
 *
     ISR(TWI_vect)
    {
      twi_slave_interrupt_handler();
    }
 *
 */

/** @} */
#endif  /* _TWI_MEGARF_SLAVE_H_*/
