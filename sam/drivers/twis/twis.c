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
 * \brief Initializes a TWIS configuration structure to defaults.
 *
 *  Initializes a given TWIS configuration structure to a set of
 *  known default values. This function should be called on all new
 *  instances of these configuration structures before being modified by the
 *  user application.
 *
 *  \param cfg    Configuration structure to initialize to default values.
 */
void twis_get_config_defaults(struct twis_config *const cfg)
{
	/* Sanity check arguments */
	Assert(cfg);

	cfg->ten_bit = false;
	cfg->chip = 0x50;
	cfg->smbus = false;
	cfg->stretch_clk_data = false;
	cfg->stretch_clk_addr = false;
	cfg->stretch_clk_hr = true;
	cfg->ack_general_call = false;
	cfg->ack_slave_addr = true;
	cfg->enable_pec = false;
	cfg->ack_smbus_host_header = false;
	cfg->ack_smbus_default_addr = false;
}

/**
 * \brief Initialize the TWI Slave Module.
 *
 * \param dev_inst  Device structure pointer.
 * \param twis  Base address of the TWIS instance.
 * \param cfg   Pointer to TWIS configuration.
 * \param slave_fct  Pointer on application functions
 * \param irq_level  Interrupt level
 */
void twis_slave_init(struct twis_dev_inst *const dev_inst, Twis *const twis, 
		struct twis_config *cfg, twis_slave_fct_t *slave_fct, uint8_t irq_level)
{
	uint32_t reg = 0;
	uint32_t twis_ch = find_twis_channel_num(twis);
	uint32_t irq_line = 0;

	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(twis);
	Assert(cfg);

	dev_inst->hw_dev = twis;
	dev_inst->twis_cfg = cfg;

	sysclk_enable_peripheral_clock(dev_inst->hw_dev);
	
	if (cfg->ten_bit) {
		reg |= TWIS_CR_TENBIT;
	}
	if (cfg->smbus) {
		reg |= TWIS_CR_SMEN;
	}
	if (cfg->stretch_clk_data) {
		reg |= TWIS_CR_SODR;
	} else {
		reg &= ~TWIS_CR_SODR;
	}
	if (cfg->stretch_clk_addr) {
		reg |= TWIS_CR_SOAM;
	} else {
		reg &= ~TWIS_CR_SOAM;
	}
	if (cfg->stretch_clk_hr) {
		reg |= TWIS_CR_STREN;
	} else {
		reg &= ~TWIS_CR_STREN;
	}
	if (cfg->ack_general_call) {
		reg |= TWIS_CR_GCMATCH;
	} else {
		reg &= ~TWIS_CR_GCMATCH;
	}
	if (cfg->ack_slave_addr) {
		reg |= TWIS_CR_SMATCH;
	} else {
		reg &= ~TWIS_CR_SMATCH;
	}
	if (cfg->enable_pec) {
		reg |= TWIS_CR_PECEN;
	} else {
		reg &= ~TWIS_CR_PECEN;
	}
	if (cfg->ack_smbus_host_header) {
		reg |= TWIS_CR_SMHH;
	} else {
		reg &= ~TWIS_CR_SMHH;
	}
	if (cfg->ack_smbus_default_addr) {
		reg |= TWIS_CR_SMDA;
	} else {
		reg &= ~TWIS_CR_SMDA;
	}
	twis->TWIS_CR = TWIS_CR_SWRST;
	twis->TWIS_CR = reg | TWIS_CR_ADR(cfg->chip) | TWIS_CR_SEN;
	
	twis->TWIS_TR = TWIS_TR_SUDAT(cfg->sudat) |
			TWIS_TR_EXP(cfg->exp) |
			TWIS_TR_TTOUT(cfg->ttouts) |
			TWIS_TR_TLOWS(cfg->tlows);
	twis->TWIS_SRR = TWIS_SRR_DADRIVEL(cfg->fs_dadrivel) |
			TWIS_SRR_DASLEW(cfg->fs_daslew) |
			TWIS_SRR_FILTER(cfg->fs_filter);
	
	twis->TWIS_HSTR = TWIS_HSTR_HDDAT(cfg->hddat);
	twis->TWIS_HSSRR = TWIS_HSSRR_DADRIVEL(cfg->hs_dadrivel) |
			TWIS_HSSRR_DASLEW(cfg->hs_daslew) |
			TWIS_HSSRR_FILTER(cfg->hs_filter);

	sysclk_disable_peripheral_clock(dev_inst->hw_dev);
	
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
 * \brief Enable Slave Mode of the TWI.
 *
 * \param dev_inst  Device structure pointer.
 */
void twis_enable(struct twis_dev_inst *const dev_inst)
{
	sysclk_enable_peripheral_clock(dev_inst->hw_dev);
	dev_inst->hw_dev->TWIS_CR = TWIS_CR_SEN;
}

/**
 * \brief Disable Slave Mode of the TWI.
 *
 * \param dev_inst  Device structure pointer.
 */
void twis_disable(struct twis_dev_inst *const dev_inst)
{
	sysclk_disable_peripheral_clock(dev_inst->hw_dev);
	dev_inst->hw_dev->TWIS_CR &= ~TWIS_CR_SEN;
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

