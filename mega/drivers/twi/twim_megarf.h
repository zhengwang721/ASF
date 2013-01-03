/**
 *
 * \file
 *
 * \brief TWI Master driver for megarf.
 *
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

#ifndef _TWI_MEGARF_MASTER_H_
#define _TWI_MEGARF_MASTER_H_


#include "compiler.h"
#include "status_codes.h"
#include "twi_common.h"
#include <conf_twim.h>
/**
 * \ingroup group_megarf_drivers_twi
 * \defgroup group_megarf_drivers_twim twi master Driver
 *
 * This driver is the implementation of TWI module of megaRF device as master.
 * Master initiates and terminates a data transmission. The transmission is
 * initiated when the Master issues a START condition on the bus, and it is 
 * terminated when the Master issues a STOP condition. This uses two
 * bi-directional bus lines(PD0 and PD1) one for clock (SCL) and one for 
 * data (SDA)
 *
 * \section megarf_twim_configuration Configuration
 * Configuration is done in the configuration file : conf_twim.h
 *
 * Configuration defines:
 * - \ref SLAVE_BUS_ADDR   : TWI slave bus address
 *
 * - \ref SLAVE_MEM_ADDR_LENGTH : TWI slave memory address length
 *
 * - \ref TWI_SPEED_HZ   : TWI data transfer rate
 *
 * \section megarf_twim_interface Interface
 * The twi module should be configured as master. The SCL period is
 * controlled by settings in the TWI Bit Rate Register (TWBR) and the
 * Prescaler bits in the TWI Status Register (TWSR). Enabling of TWI
 * clock source and interrupt is done by calling the function
 * \code twi_master_setup() \endcode
 *
 * \subsection megarf_twim_usage Usage
 * The include file is needed:
 * \code #include "twim_megarf.h" \endcode
 * \code #include "twi_common.h" \endcode
 *
 * TWI transmission is to transmit a START condition. This is done by
 * writing a specific value into TWCR, instructing the TWI hardware to transmit
 * START condition and then transmit the data. Similarly in receive mode
 * This is done by calling:
 * \code twi_master_write() and twi_master_read() \endcode
 *
 * After initiating the transmission or reception the completion status will be
 * indicated to the application code through the following function
 * \code twi_master_get_status() and twi_master_read() \endcode
 * @{
 */
/**
 * \internal
 * \brief internal address size
 */

typedef enum {
	TWI_SLAVE_NO_INTERNAL_ADDRESS,
	TWI_SLAVE_ONE_BYTE_SIZE,
	TWI_SLAVE_TWO_BYTE_SIZE,
	TWI_SLAVE_THREE_BYTE_SIZE
} twiInternalAddrSize_t;

/** \brief TWI Master status codes. */
enum {
	TWS_BUSERROR      = 0x00,
	TWS_START         = 0x08,
	TWS_RSTART        = 0x10,
	TWS_MT_SLA_ACK    = 0x18,
	TWS_MT_SLA_NACK   = 0x20,
	TWS_MT_DATA_ACK   = 0x28,
	TWS_MT_DATA_NACK  = 0x30,
	TWS_M_ARB_LOST    = 0x38,
	TWS_MR_SLA_ACK    = 0x40,
	TWS_MR_SLA_NACK   = 0x48,
	TWS_MR_DATA_ACK   = 0x50,
	TWS_MR_DATA_NACK  = 0x58
};


/**
 * \internal
 * \def TWI_STATE
 * \brief different states of TWI process
 */
/* states of the i2c transaction */
#define TWI_IDLE                     1
#define TWI_WRITE_IADDR_WRITE_DATA   2
#define TWI_WRITE_IADDR_READ_DATA    3
#define TWI_WRITE_DATA               4
#define TWI_READ_DATA                5
#define TWI_TRANSAC_SUCCESS          6
#define TWI_TRANSAC_FAIL             7
#define TWI_PROCESS                  8

/**
 * \brief Perform a TWI master write transfer.
 *
 * This function is a TWI Master write transaction.
 *
 * \param package -  Package information and data
 *                  (see \ref twi_package_t)
 */
status_code_t twi_master_write(volatile void *twi,const twi_package_t *package);

/**
 * \brief Reads the series of bytes from the TWI bus
 * \param package -  Package information and data
 *                  (see \ref twi_package_t)
 */
status_code_t twi_master_read(volatile void *twi,const twi_package_t *package);

/**
 * \brief returns the status of TWI bus
 *
 * \param none
 */
status_code_t twi_master_get_status(void);

/**
 * \brief Inits TWI module as master
 *
 * This function is a TWI Master initialisation.
 *
 * \param opt twi setting options
 *                  (see \ref twi_master_options_t)
 */
status_code_t twi_master_init(volatile void *twi, twi_master_options_t *opt);

/** @} */
#endif  /* _TWI_MEGARF_MASTER_H_*/
