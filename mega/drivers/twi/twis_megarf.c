/**
 * \file
 *
 * \brief megaRF TWI driver as slave.
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

#include "twis_megarf.h"


/** Slave Transfer Descriptor */
static struct {
	slave_data_buffer_t *data_buffer;              /*  data buffer */
	unsigned int data_count;        /* Bus transfer payload data counter */
	uint8_t state;                  /* Transfer state */
	int status;           /* Transfer status */
}
transfer;

/**
 * \brief Enable TWI as slave
 *
 * \param none.
 */
static inline void twi_slave_enable(void)
{
	TWCR = (1 << TWEN) |                                 /* Enable
	                                                      * TWI-interface
	                                                      * and release TWI
	                                                      * pins */
			(1 << TWIE) | (1 << TWINT) | /* Keep interrupt enabled
	                                              * and clear the flag */
			(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | /*
	                                                             * Acknowledge
	                                                             * on any
	                                                             * new
	                                                             * requests. */
			(0 << TWWC);
}

/**
 * \brief  writing an byte to TWI.
 *
 * \param data - contains the data to transmitted ro master.
 */
static inline void twi_slave_write_byte(uint8_t data)
{
	TWDR =  data;
	twi_slave_enable();
}

/**
 * \brief Sending data to twi bus.
 *
 * \param none.
 */
static void twi_slave_data_write(void)
{
	twi_slave_write_byte(transfer.data_buffer->tx_buffer[transfer.data_count
			++]);
}

/**
 * \brief Notification of Last byte is transmitted
 *
 * \param none.
 */
static void twi_slave_last_byte_write_done(void)
{
	transfer.state = TWI_STATE_IDLE;
	transfer.status = TWI_STATUS_TX_COMPLETE;

	twi_slave_enable();
}

/**
 * \brief Read received data
 *
 * \param data - Contains data read from twi bus to be written to data receive
 *buffer.
 */
static void twi_slave_data_read(uint8_t data)
{
	transfer.data_buffer->rx_buffer[transfer.data_count++] = data;
	twi_slave_enable();
}

/**
 * \brief Reset TWI bus
 *
 * \param none.
 */
static void twi_slave_bus_reset(void)
{
	transfer.state = TWI_STATE_IDLE;
	twi_reset();
}

/**
 * \brief TWI interrupt handler
 */
void twi_slave_interrupt_handler(void)
{
	switch (TWI_TWSR_STATUS_MASK) {
	case TWS_ST_SLA_ACK:        /* Own SLA+R has been received; ACK has been
		                     * returned */
		transfer.data_count   = 0; /* Set buffer pointer to first data
		                            * location */

	case TWS_ST_DATA_ACK:       /* Data byte in TWDR has been transmitted;
		                     * ACK has been received */
		twi_slave_data_write();
		transfer.state = TWI_STATE_BUSY;
		break;

	case TWS_ST_DATA_NACK:      /* Data byte in TWDR has been transmitted;
		                     * NACK has been received. */
		                    /* I.e. this could be the end of the
		                     * transmission. */
		twi_slave_last_byte_write_done();
		break;

	case TWS_SR_GEN_ACK:        /* General call address has been received;
		                     * ACK has been returned */
	case TWS_SR_SLA_ACK:        /* Own SLA+W has been received ACK has been
		                     * returned */
		transfer.data_count   = 0; /* Set buffer pointer to first data
		                            * location */
		twi_slave_enable();
		transfer.state = TWI_STATE_BUSY;

		break;

	case TWS_SR_SLA_DATA_ACK:   /* Previously addressed with own SLA+W; data
		                     * has been received; ACK has been returned */
	case TWS_SR_GEN_DATA_ACK:   /* Previously addressed with general call;
		                     * data has been received; ACK has been
		                     * returned */
		twi_slave_data_read(twi_read_byte());
		transfer.state = TWI_STATE_BUSY;
		break;

	case TWS_SR_STOP_RESTART:   /* A STOP condition or repeated START
		                     * condition has been received while still
		                     * addressed as Slave */
		/* Enter not addressed mode and listen to address match */
		transfer.state = TWI_STATE_IDLE;
		transfer.status = TWI_STATUS_RX_COMPLETE;
		twi_slave_enable();
		break;

	case TWS_SR_SLA_DATA_NACK:  /* Previously addressed with own SLA+W; data
		                     * has been received; NOT ACK has been
		                     * returned */
	case TWS_SR_GEN_DATA_NACK:  /* Previously addressed with general call;
		                     * data has been received; NOT ACK has been
		                     * returned */
	case TWS_ST_DATA_ACK_LAST_BYTE: /* Last data byte in TWDR has been
		                         * transmitted (TWEA = “0”); ACK has
		                         * been received */
	case TWS_BUS_ERROR:     /* Bus error due to an illegal START or STOP
		                * condition */
		twi_slave_bus_reset();
		transfer.status = TWI_STATUS_IO_ERROR;
		break;

	default:
		transfer.status = TWI_STATUS_PROTOCOL_ERROR;           /* Store TWI State as
		                                           * errormessage,
		                                           * operation also
		                                           * clears the Success
		                                           * bit. */
		transfer.state = TWI_STATE_IDLE;
		twi_slave_enable();
		break;
	}
}


int twi_slave_status_get(void)
{
	return(transfer.status);
}

int twi_slave_state_get(void)
{
	return(transfer.state);
}

status_code_t twi_slave_init(uint8_t twi_slave_ownadd)
{
	TWAR = twi_slave_ownadd;                      /* Set own TWI slave
	                                               * address. Accept TWI
	                                               * General Calls. */
	TWCR = (1 << TWEN) |                        /* Enable TWI-interface and
	                                             * release TWI pins. */
			(0 << TWIE) | (0 << TWINT) | /* Disable TWI Interupt. */
			(0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | /* Do not
	                                                             * ACK on
	                                                             * any
	                                                             * requests,
	                                                             * yet. */
			(0 << TWWC);
			
	return STATUS_OK;		
}

status_code_t twi_slave_start(slave_data_buffer_t *package)
{
	if (TWI_STATE_IDLE != transfer.state) {
		return ERR_BUSY;
	}

	/* Initiate a transaction when the bus is ready. */
	transfer.data_buffer     = (slave_data_buffer_t *)package;
	transfer.data_count  = 0;

	twi_slave_status_reset();

	return STATUS_OK;
}

void twi_slave_status_reset(void)
{
	transfer.state       = TWI_STATE_IDLE;
	transfer.status      = TWI_STATUS_NO_STATE;

	twi_slave_enable();
}
