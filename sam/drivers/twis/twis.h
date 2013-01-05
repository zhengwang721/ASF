/**
 * \file
 *
 * \brief TWIS driver for SAM.
 *
 * This file defines a useful set of functions for the TWIS on SAM4L devices.
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


#ifndef TWIS_H_INCLUDED
#define TWIS_H_INCLUDED

/**
 * \defgroup sam_drivers_twis_group TWIS - Two-Wire Slave Interface
 *
 * Driver for the TWIS (Two-Wire Slave Interface).
 * This driver provides access to the main features of the TWIS controller.
 * The TWIS interconnects components on a unique two-wire bus.
 * The TWIS is programmable as a slave with sequential or single-byte access.
 * High speed mode capability is supported.
 *
 * \{
 */

#include "compiler.h"
#include "status_codes.h"
#include "sysclk.h"

/**
 * \brief Input parameters when initializing the TWIS module mode
 */
struct twis_config {
	/** Ten-bit addressing */
	bool ten_bit;
	/** The desired address. */
	uint32_t chip;
	/** SMBUS mode */
	bool smbus;
	/** Stretch clock on data byte reception */
	bool stretch_clk_data;
	/** Stretch clock on address match */
	bool stretch_clk_addr;
	/** Stretch clock if RHR is full or THR is empty */
	bool stretch_clk_hr;
	/** Acknowledge the general call address */
	bool ack_general_call;
	/** Acknowledge the specified slave address */
	bool ack_slave_addr;
	/** Enable packet error checking */
	bool enable_pec;
	/** Acknowledge the SMBus host header */
	bool ack_smbus_host_header;
	/** Acknowledge the SMBus default address */
	bool ack_smbus_default_addr;
	/** Data Setup Cycles */
	uint8_t sudat;
	/** Input Spike Filter Control in F/S mode*/
	uint8_t fs_filter;
	/** Data Slew Limit in F/S mode */
	uint8_t fs_daslew;
	/** Data Drive Strength Low in F/S mode */
	uint8_t fs_dadrivel;
	/** Data Hold Cycles */
	uint8_t hddat;
	/** Input Spike Filter Control in HS mode*/
	uint8_t hs_filter;
	/** Data Slew Limit in HS mode */
	uint8_t hs_daslew;
	/** Data Drive Strength Low in HS mode*/
	uint8_t hs_dadrivel;
	/** Clock Prescaler */
	uint8_t exp;
	/** SMBus TIMEOUT Cycles */
	uint8_t ttouts;
	/** SMBus Low:Sext Cycles */
	uint8_t tlows;
};

/*
 * \brief Pointer on TWI slave user specific application routines
 */
typedef struct
{
	/** Routine to receive data from TWI master */
	void (*rx) (uint8_t);
	/** Routine to transmit data to TWI master */
	uint8_t (*tx) (void);
	/** Routine to signal a TWI STOP */
	void (*stop) (void);
} twis_slave_fct_t;

/**
 * \brief Enable Slave Mode of the TWI.
 *
 * \param *twis   Base address of the TWIS instance.
 */
static inline void twis_enable(Twis *twis)
{
	sysclk_enable_peripheral_clock(twis);
	twis->TWIS_CR = TWIS_CR_SEN;
}

/**
 * \brief Disable Slave Mode of the TWI.
 *
 * \param *twis   Base address of the TWIS instance.
 */
static inline void twis_disable(Twis *twis)
{
	sysclk_disable_peripheral_clock(twis);
	twis->TWIS_CR &= ~TWIS_CR_SEN;
}

void twis_slave_init(Twis *twis, struct twis_config *config, twis_slave_fct_t *slave_fct,
		uint8_t irq_level);

uint8_t twi_slave_read(Twis *twis);
void twi_slave_write(Twis *twis, uint8_t byte);

void twis_send_data_nack(Twis *twis);
void twis_send_data_ack(Twis *twis);

void twis_enable_interrupt(Twis *twis, uint32_t interrupt_source);
void twis_disable_interrupt(Twis *twis, uint32_t interrupt_source);
uint32_t twis_get_interrupt_mask(Twis *twis);

uint32_t twis_get_status(Twis *twis);
void twis_clear_status(Twis *twis, uint32_t clear_status);

uint8_t twis_get_smbus_pec(Twis *twis);
void twis_set_smbus_transfer_nb(Twis *twis, uint8_t nb, bool increment);
uint8_t twis_get_smbus_transfer_nb(Twis *twis);

/**
 * \}
 */

#endif /* TWIS_H_INCLUDED */
