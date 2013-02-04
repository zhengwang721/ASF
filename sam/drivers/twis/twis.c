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

#include "twis.h"
#include "sleepmgr.h"

/** Define all error conditions */
#define TWIS_SR_ERROR (TWIS_SR_BUSERR     \
		| TWIS_SR_SMBPECERR                  \
		| TWIS_SR_SMBTOUT                    \
		| TWIS_SR_ORUN                       \
		| TWIS_SR_URUN)

/**
 * \internal
 * \brief TWIS callback function pointer array
 */
twis_slave_fct_t *twis_fct_pointer[TWIS_INST_NUM];

/**
 * \brief Get TWIS channel number.
 *
 * \param twis  Base address of the TWIS
 *
 * \return      TWIS channel number
 */
static uint32_t find_twis_channel_num(Twis *twis)
{
#if defined(ID_TWIS1)
	if (twis == TWIS1) {
		return 1;
	}
#endif
#if defined(ID_TWIS0)
	if (twis == TWIS0) {
		return 0;
	}
#endif

	return 0;
}

/**
 * \brief Initialize the TWI Slave Module.
 *
 * \param twis Base address of the TWIS
 * \param *config   Basic configuration for the TWIS
 * \param slave_fct  Pointer on application functions
 * \param irq_level  Interrupt level
 */
void twis_slave_init(Twis *twis, struct twis_config *config, twis_slave_fct_t *slave_fct,
		uint8_t irq_level)
{
	uint32_t reg = 0;
	uint32_t twis_ch = find_twis_channel_num(twis);
	uint32_t irq_line = 0;

	if (config->ten_bit) {
		reg |= TWIS_CR_TENBIT;
	}
	if (config->smbus) {
		reg |= TWIS_CR_SMEN;
	}
	if (config->stretch_clk_data) {
		reg |= TWIS_CR_SODR;
	} else {
		reg &= ~TWIS_CR_SODR;
	}
	if (config->stretch_clk_addr) {
		reg |= TWIS_CR_SOAM;
	} else {
		reg &= ~TWIS_CR_SOAM;
	}
	if (config->stretch_clk_hr) {
		reg |= TWIS_CR_STREN;
	} else {
		reg &= ~TWIS_CR_STREN;
	}
	if (config->ack_general_call) {
		reg |= TWIS_CR_GCMATCH;
	} else {
		reg &= ~TWIS_CR_GCMATCH;
	}
	if (config->ack_slave_addr) {
		reg |= TWIS_CR_SMATCH;
	} else {
		reg &= ~TWIS_CR_SMATCH;
	}
	if (config->enable_pec) {
		reg |= TWIS_CR_PECEN;
	} else {
		reg &= ~TWIS_CR_PECEN;
	}
	if (config->ack_smbus_host_header) {
		reg |= TWIS_CR_SMHH;
	} else {
		reg &= ~TWIS_CR_SMHH;
	}
	if (config->ack_smbus_default_addr) {
		reg |= TWIS_CR_SMDA;
	} else {
		reg &= ~TWIS_CR_SMDA;
	}
	twis->TWIS_CR = TWIS_CR_SWRST;
	twis->TWIS_CR = reg | TWIS_CR_ADR(config->chip) | TWIS_CR_SEN;
	
	twis->TWIS_TR = TWIS_TR_SUDAT(config->sudat) |
			TWIS_TR_EXP(config->exp) |
			TWIS_TR_TTOUT(config->ttouts) |
			TWIS_TR_TLOWS(config->tlows);
	twis->TWIS_SRR = TWIS_SRR_DADRIVEL(config->fs_dadrivel) |
			TWIS_SRR_DASLEW(config->fs_daslew) |
			TWIS_SRR_FILTER(config->fs_filter);
	
	twis->TWIS_HSTR = TWIS_HSTR_HDDAT(config->hddat);
	twis->TWIS_HSSRR = TWIS_HSSRR_DADRIVEL(config->hs_dadrivel) |
			TWIS_HSSRR_DASLEW(config->hs_daslew) |
			TWIS_HSSRR_FILTER(config->hs_filter);

	#if defined(ID_TWIS0)
	if (twis == TWIS0) {
		irq_line = TWIS0_IRQn;
	}
	#endif
	#if defined(ID_TWIS1)
	if (twis == TWIS1) {
		irq_line = TWIS1_IRQn;
	}
	#endif
	twis_fct_pointer[twis_ch] = slave_fct;
	irq_register_handler((IRQn_Type)irq_line, irq_level);
}

/**
 * \brief Enable NACK transfer in Slave Receiver Mode
 *
 * \param twis   Base address of the TWIS
 */
void twis_send_data_nack(Twis *twis)
{
	twis->TWIS_CR |= TWIS_CR_ACK;
}

/**
 * \brief Enable ACK transfer in Slave Receiver Mode
 *
 * \param twis   Base address of the TWIS
 */
void twis_send_data_ack(Twis *twis)
{
	twis->TWIS_CR &= ~TWIS_CR_ACK;
}

/**
 * \brief Get the last byte data received from TWI bus.
 *
 * \param twis   Base address of the TWIS
 *
 * \retval Last byte data received from TWI bus
 */
uint8_t twi_slave_read(Twis *twis)
{
	return twis->TWIS_RHR;
}

/**
 * \brief Write one byte data to TWI bus.
 *
 * \param twis       Base address of the TWIS
 * \param byte       The byte data to write
 */
void twi_slave_write(Twis *twis, uint8_t byte)
{
	twis->TWIS_THR = byte;
}

/**
 * \brief Get the calculated PEC value. Only for SMBus mode.
 *
 * \param twis   Base address of the TWIS
 *
 * \retval Calculated PEC value
 */
uint8_t twis_get_smbus_pec(Twis *twis)
{
	return (uint8_t)(twis->TWIS_PECR & 0xFF);
}

/**
 * \brief Set the total number of data bytes in the transmission. Only for SMBus mode.
 *
 * \param twis       Base address of the TWIS
 * \param nb         Total number of data bytes in the transmission
 * \param increment  Count up per byte transferred if true, otherwise count down
 */
void twis_set_smbus_transfer_nb(Twis *twis, uint8_t nb, bool increment)
{
	if (increment) {
		twis->TWIS_CR |= TWIS_CR_CUP;
	} else {
		twis->TWIS_CR &= ~TWIS_CR_CUP;
	}

	twis->TWIS_NBYTES = nb;
}

/**
 * \brief Get the progress of the transfer in SMBus mode.
 *
 * \param twis   Base address of the TWIS
 *
 * \retval The left number of data bytes in the transmission.
 */
uint8_t twis_get_smbus_transfer_nb(Twis *twis)
{
	return (uint8_t)(twis->TWIS_NBYTES & 0xFF);
}

/**
 * \brief Enable the TWIS interrupts
 *
 * \param twis              Base address of the TWIS
 * \param interrupt_source  The TWIS interrupt to be enabled
 */
void twis_enable_interrupt(Twis *twis, uint32_t interrupt_source)
{
	/* Set the interrupt flags */
	twis->TWIS_IER = interrupt_source;
}

/**
 * \brief Disable the TWIS interrupts and clear their status
 *
 * \param twis              Base address of the TWIS.
 * \param interrupt_source  The TWIS interrupt to be disabled
 */
void twis_disable_interrupt(Twis *twis, uint32_t interrupt_source)
{
	/* Clear the interrupt flags */
	twis->TWIS_IDR = interrupt_source;
	/* Clear the status flags */
	twis->TWIS_SCR = interrupt_source;
}

/**
 * \brief Get the TWIS interrupt mask
 *
 * \param twis   Base address of the TWIS
 *
 * \retval TWIS interrupt mask
 */
uint32_t twis_get_interrupt_mask(Twis *twis)
{
	return twis->TWIS_IMR;
}

/**
 * \brief Information about the current status of the TWIS
 *
 * \param twis   Base address of the TWIS
 */
uint32_t twis_get_status(Twis *twis)
{
	return twis->TWIS_SR;
}

/**
 * \brief Clear the current status of the TWIS
 *
 * \param twis   Base address of the TWIS
 * \param clear_status  The TWIS status to be clear
 */
void twis_clear_status(Twis *twis, uint32_t clear_status)
{
	twis->TWIS_SCR = clear_status;
}

/**
 * \internal
 * \brief TWIS interrupt handler
 *
 * \param Base address of the TWIS
 */
static void twis_slave_int_handler(Twis *twis)
{
	uint32_t twis_ch = find_twis_channel_num(twis);

	/* Get status and interrupt mask register values */
	uint32_t status  = twis_get_status(twis);
	uint32_t enabled = twis_get_interrupt_mask(twis);
	uint32_t pending = status & enabled;

	/* An error has occurred, set only address match active and return */
	if (status & TWIS_SR_ERROR) {
		twis_disable_interrupt(twis, ~0UL);
		twis_clear_status(twis, ~0UL);
		twis_enable_interrupt(twis, TWIS_IER_SAM);
		twis_fct_pointer[twis_ch]->stop();
		return;
	}
	/* Check if the slave address match flag is raised */
	if (pending & TWIS_IER_SAM) {
		/* Ignore repeated start and transmission complete flags */
		if (pending & TWIS_SR_REP) {
			twis_clear_status(twis, TWIS_SCR_REP);
		}
		if (pending & TWIS_SR_TCOMP) {
			twis_clear_status(twis, TWIS_SCR_TCOMP);
		}
		/* Enable error handling */
		twis_enable_interrupt(twis, TWIS_SR_ERROR);

		/* Check if the slave should be in receive or transmit mode */
		if (status & TWIS_SR_TRA) {
			/* Transmit mode */
			twis_clear_status(twis, TWIS_SR_BTF);
			twis_enable_interrupt(twis, TWIS_IER_BTF |TWIS_IER_TCOMP);
			twi_slave_write(twis, twis_fct_pointer[twis_ch]->tx());
		} else {
			/* Receive mode */
			twis_enable_interrupt(twis, TWIS_IER_RXRDY | TWIS_IER_TCOMP
					| TWIS_IER_REP);
		}
	}

	/* Check if there is data ready to be read in the data receive register */
	if (pending & TWIS_IER_RXRDY) {
		/* Call user specific receive function */
		twis_fct_pointer[twis_ch]->rx(twi_slave_read(twis));
	}

	/* Check if the transmit ready flag is raised */
	if (pending & TWIS_SR_BTF) {
		if (status & TWIS_SR_NAK) {
			twis_disable_interrupt(twis, TWIS_IDR_BTF);
			twis_clear_status(twis, TWIS_SCR_BTF);
			twis_enable_interrupt(twis, TWIS_IER_TCOMP |TWIS_IER_REP);
			/* Clear the NAK */
			twis_clear_status(twis, TWIS_SCR_NAK);
		} else {
			twi_slave_write(twis, twis_fct_pointer[twis_ch]->tx());
		}
	}

	/* Check if the transmission complete or repeated start flags raised */
	if (pending & (TWIS_IER_TCOMP | TWIS_IER_REP)) {
		/* Clear transmit complete and repeated start flags */
		twis_clear_status(twis, TWIS_SCR_TCOMP | TWIS_SCR_REP | TWIS_SCR_NAK);
		/* Disable transmission ready interrupt */
		twis_disable_interrupt(twis, TWIS_IDR_BTF
			| TWIS_IDR_RXRDY
			| TWIS_IDR_TCOMP
			| TWIS_IDR_REP);
		/* Enable slave address match interrupt */
		twis_enable_interrupt(twis, TWIS_IER_SAM);
		/* Call user specific stop function */
		twis_fct_pointer[twis_ch]->stop();
	}
	twis_clear_status(twis, pending);
}

#if defined(ID_TWIS0)
void TWIS0_Handler(void)
{
	twis_slave_int_handler(TWIS0);
}
#endif

#if defined(ID_TWIS1)
void TWIS1_Handler(void)
{
	twis_slave_int_handler(TWIS1);
}
#endif

