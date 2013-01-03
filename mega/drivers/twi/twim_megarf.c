/**
 * \file
 *
 * \brief megaRF TWI master driver.
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

#include "twim_megarf.h"


/** Master Transfer Descriptor */
static struct {
	twi_package_t *pkg;             /* Bus message descriptor */
	int addr_count;                 /* Bus transfer address data counter */
	unsigned int data_count;        /* Bus transfer payload data counter */
	uint8_t state;                  /* Transfer state */
	status_code_t status;           /* Transfer status */
}transfer;

static volatile bool twi_master_busy = false;

/**
 * \brief Reset TWI bus
 *
 * \param none.
 */
static void twi_master_bus_reset(void)
{
	transfer.state = TWI_IDLE;
	twi_master_busy      = false;
	twi_reset();
}

/**
 * \brief Notification that last byte was read from the TWI.
 *  Needs to send STOP condition
 *
 * \param data - contains byte that was read.
 */
static void twi_master_read_last_byte(uint8_t data)
{
	if (TWI_READ_DATA == transfer.state) {
		transfer.pkg->buffer[transfer.data_count++] = data;
		twi_send_stop();
		transfer.state = TWI_IDLE;
		transfer.status = STATUS_OK;
		twi_master_busy  = false;
	} else { /* abnormal */
		twi_master_bus_reset();
		transfer.status = ERR_PROTOCOL;
	}
}

/**
 * \brief Notification that byte was read by the TWI.
 *
 * \param data - contains byte that was read.
 */
static void twi_master_read_done(uint8_t data)
{
	if (TWI_READ_DATA == transfer.state) {
		transfer.pkg->buffer[transfer.data_count++] = data;
		if (transfer.data_count < (transfer.pkg->length - 1)) {
			twi_send_ack(true); /* send ACK */
		} else {
			twi_send_ack(false); /* send NACK */
		}
	} else { /* abnormal */
		twi_master_bus_reset();
		transfer.status = ERR_PROTOCOL;
	}
}

/**
 * \brief Notification that address byte was written to the TWI and need to send
 *ACK or NACK.
 *
 * \param none.
 */
static void twi_master_addr_ack(void)
{
	if (TWI_READ_DATA == transfer.state) {
		if (transfer.data_count == (transfer.pkg->length - 1)) {
			twi_send_ack(false); /* send nack */
		} else {
			twi_send_ack(true); /* send ack */
		}
	} else { /* abnormal */
		twi_master_bus_reset();
		transfer.status = ERR_PROTOCOL;
	}
}

/**
 * \brief Sending internal device address to twi bus.
 *
 * \param none.
 */
static void twi_master_internal_addr_write(void)
{
	uint8_t data;

	data = transfer.pkg->addr[transfer.addr_count];
	transfer.addr_count++;
	twi_write_byte(data);

	if (transfer.pkg->addr_length == transfer.addr_count) {
		if (TWI_WRITE_IADDR_WRITE_DATA == transfer.state) {
			transfer.state = TWI_WRITE_DATA;
		} else {
			transfer.state = TWI_READ_DATA;
		}
	}
}

/**
 * \brief wait for some time for bus idle condition after the transfer complete.
 *
 * \param none.
 */
static void twi_bus_release_delay(void)
{
	for (volatile int i = 0; i < 10000; i++) {
	}
}

/**
 * \brief Sending data to twi bus. If last byte then send stop condition.
 *
 * \param none.
 */
static void twi_master_data_write(void)
{
	if (transfer.data_count < transfer.pkg->length) {
		twi_write_byte(transfer.pkg->buffer[transfer.data_count++]);
	} else {
		twi_send_stop();
		transfer.state = TWI_IDLE;
		transfer.status = STATUS_OK;
		twi_master_busy = false;
		twi_bus_release_delay();
	}
}

/**
 * \brief Notification that byte was written to the TWI.
 *
 * \param result - contains result of previous operation.
 */

static void twi_master_write_done(void)
{
	if (TWI_WRITE_DATA == transfer.state) {
		twi_master_data_write();
	} else if ((TWI_WRITE_IADDR_WRITE_DATA == transfer.state) ||
			(TWI_WRITE_IADDR_READ_DATA == transfer.state)) {
		twi_master_internal_addr_write();
	} else if (TWI_READ_DATA == transfer.state) {
		twi_send_start();
	} else { /* abnormal */
		twi_master_bus_reset();
		transfer.status = ERR_PROTOCOL;
	}
}

/**
 * \brief Notification about the start condition was sent.
 *
 * This function is a TWI Master start indication.
 *
 * \param none
 */
static void twi_master_start(void)
{
	uint8_t chip_add;

	if (TWI_WRITE_IADDR_WRITE_DATA == transfer.state || TWI_WRITE_DATA ==
			transfer.state || TWI_WRITE_IADDR_READ_DATA ==
			transfer.state) {
		chip_add = TWI_WRITE_ENABLE(transfer.pkg->chip);
		twi_write_byte(chip_add);
	} else if (TWI_READ_DATA == transfer.state) {
		chip_add = TWI_READ_ENABLE(transfer.pkg->chip);
		twi_write_byte(chip_add);
	} else { /* abnormal */
		twi_master_bus_reset();
		transfer.status = ERR_PROTOCOL;
	}
}

/**
 * \internal
 *
 * \brief TWI Master Interrupt Vectors
 */
static void twi_master_interrupt_handler(void)
{
	uint8_t status_mask;
	status_mask = TWI_TWSR_STATUS_MASK;

	switch (status_mask) {
	case TWS_START:
	case TWS_RSTART:
		twi_master_start();
		break;

	case TWS_MT_SLA_ACK:
	case TWS_MT_DATA_ACK:
		twi_master_write_done();
		break;

	case TWS_BUSERROR:
	case TWS_MT_SLA_NACK:
	case TWS_MT_DATA_NACK:
	case TWS_MR_SLA_NACK:
		twi_master_bus_reset();
		transfer.status = ERR_IO_ERROR;
		break;

	case TWS_MR_SLA_ACK:
		twi_master_addr_ack();
		break;

	case TWS_MR_DATA_ACK:
		twi_master_read_done(twi_read_byte());
		break;

	case TWS_MR_DATA_NACK:
		twi_master_read_last_byte(twi_read_byte());
		break;

	case TWS_M_ARB_LOST:
		twi_master_bus_reset();
		transfer.status = ERR_BUSY;
		break;

	default:
		transfer.state = TWI_IDLE;
		transfer.status = ERR_PROTOCOL;
		twi_master_busy = false;
		break;
	}
}

/**
 * \brief TWI interrupt handler
 */
ISR(TWI_vect)
{
	twi_master_interrupt_handler();
}	



status_code_t twi_master_write(volatile void *twi,const twi_package_t *package)
{
	/* Do a sanity check on the arguments. */
	if (package == NULL) {
		return ERR_INVALID_ARG;
	}
	
	if (twi_master_busy == true) {
		return ERR_BUSY;
	}

	/* Initiate a transaction when the bus is ready. */
	transfer.pkg         = (twi_package_t *)package;
	transfer.addr_count  = 0;
	transfer.data_count  = 0;
	twi_master_busy      = true;

	if (TWI_SLAVE_NO_INTERNAL_ADDRESS == transfer.pkg->addr_length) {
		transfer.state  = TWI_WRITE_DATA;
	} else {
		transfer.state  = TWI_WRITE_IADDR_WRITE_DATA;
	}

	twi_send_start();

	/* Wait for the transaction to complete */
	while(twi_master_busy){
		
	}

	
	return twi_master_get_status();
}

status_code_t twi_master_read(volatile void *twi,const twi_package_t *package)
{
	/* Do a sanity check on the arguments. */
	if ((package == NULL) || transfer.pkg->length == 0) {
		return ERR_INVALID_ARG;
	}

	if (true == twi_master_busy) {
		return ERR_BUSY;
	}

	/* Initiate a transaction when the bus is ready. */
	/* twi = NULL;     megaRF has only one twi     */
	transfer.pkg         = (twi_package_t *)package;
	transfer.addr_count  = 0;
	transfer.data_count  = 0;
	twi_master_busy      = true;

	if (TWI_SLAVE_NO_INTERNAL_ADDRESS == transfer.pkg->addr_length) {
		transfer.state = TWI_READ_DATA;
	} else {
		transfer.state = TWI_WRITE_IADDR_READ_DATA;
	}

	twi_send_start();
    
	/* Wait for the transaction to complete */
	while(twi_master_busy);
	
	return twi_master_get_status();
}


status_code_t twi_master_get_status(void)
{
	return transfer.status;
}

/**
 * \brief Inits TWI module as master
 *
 * This function is a TWI Master initialisation.
 *
 * \param opt twi setting options
 *                  (see \ref twi_master_options_t)
 */
status_code_t twi_master_init(volatile void *twi, twi_master_options_t *opt)
{
	cpu_irq_disable();
	
	TWCR = 0x00;
	/* ! prescaler */
	TWSR = TWI_PRESCALE_REG;
	/* ! Set bit rate */
	TWBR = opt->baud_reg;

	twi_interrupt_enable();
	
	cpu_irq_enable();

	return STATUS_OK;
}