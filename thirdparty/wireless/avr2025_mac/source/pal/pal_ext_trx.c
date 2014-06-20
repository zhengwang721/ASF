/*
 * @file pal.c
 *
 * @brief Performs interface functionalities between the TAL layer and ASF
 *drivers
 *  Copyright (c) 2013 Atmel Corporation. All rights reserved.
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
 */

/*
 * Copyright (c) 2013, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#include "board.h"
#include "spi_master.h"
#include "pal_ext_trx.h"
#include "pal.h"
# include "usart.h"
# include "sio2ncp.h"
# include "conf_sio2ncp.h"
#include "delay.h"
#include "interrupt.h"
#include "conf_pal.h"
static irq_handler_t irq_hdl_trx = NULL;
struct spi_device SPI_AT86RFX_DEVICE = {
	/* ! Board specific select id */
	.id = AT86RFX_SPI_CS
};

AT86RFX_ISR()
{
	/*Clearing the RF interrupt*/
	pal_trx_irq_flag_clr();

	/*Calling the interrupt routines*/
	if (irq_hdl_trx) {
		irq_hdl_trx();
	}
}

void pal_spi_init(void)
{
	/* Initialize SPI in master mode to access the transceiver */
	spi_master_init(AT86RFX_SPI);
	spi_master_setup_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE, SPI_MODE_0,
			AT86RFX_SPI_BAUDRATE, 0);
	spi_enable(AT86RFX_SPI);
	AT86RFX_INTC_INIT();
}
#define TAL_DEFAULT_BB_IRQ_MASK     (BB_IRQ_TXFE | BB_IRQ_RXFE)
#define TAL_DEFAULT_RF_IRQ_MASK     RF_IRQ_ALL_IRQ

/*Temporary set of definitions to test parallel PER Test in 215*/
//todo : Implement spi using dma to reduce cpu dependency for writing 2000bytes payload for spi
#define DISABLE_TRX_INTERRUPT()     pal_trx_reg_write( RG_BBC0_IRQM, 0);\
								    pal_trx_reg_write(BB_BASE_ADDR_OFFSET + RG_BBC0_IRQM, 0);\
								    pal_trx_reg_write( RG_RF09_IRQM, 0);\
								    pal_trx_reg_write(RF_BASE_ADDR_OFFSET + RG_RF09_IRQM, 0);


#define ENABLE_TRX_INTERRUPT()         pal_trx_reg_write( RG_BBC0_IRQM, TAL_DEFAULT_BB_IRQ_MASK);\
									   pal_trx_reg_write(BB_BASE_ADDR_OFFSET + RG_BBC0_IRQM, TAL_DEFAULT_BB_IRQ_MASK);\
									   pal_trx_reg_write ( RG_RF09_IRQM, TAL_DEFAULT_RF_IRQ_MASK);\
									   pal_trx_reg_write(RF_BASE_ADDR_OFFSET + RG_RF09_IRQM, TAL_DEFAULT_RF_IRQ_MASK);							  
									   
void pal_trx_read(uint16_t addr,uint8_t *data, uint16_t length)
{
	uint8_t register_value = 0;

	/*Saving the current interrupt status & disabling the global interrupt
	 **/
	//ENTER_CRITICAL_REGION();
	//ENTER_TRX_REGION();
    DISABLE_TRX_INTERRUPT();

	/* Prepare the command byte */
	addr |= 0X00; //Read Command
	
	
	uint8_t reg_addr;
	
	/* Start SPI transaction by pulling SEL low */
	spi_select_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);
	
	reg_addr = addr>>8;
	
	/* Send the Read command byte */
	spi_write_packet(AT86RFX_SPI, &reg_addr, 1);	

	reg_addr = addr;

	/* Send the Read command byte */
	spi_write_packet(AT86RFX_SPI, &reg_addr, 1);

	/* Do dummy read for initiating SPI read */
	spi_read_packet(AT86RFX_SPI, data, length);
	
	/* Stop the SPI transaction by setting SEL high */
	spi_deselect_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);

	/*Restoring the interrupt status which was stored & enabling the global
	 *interrupt */
	//LEAVE_CRITICAL_REGION();
    ENABLE_TRX_INTERRUPT();

}

void pal_trx_write(uint16_t addr, uint8_t *data,uint16_t length)
{
	/*Saving the current interrupt status & disabling the global interrupt
	 **/
	//ENTER_TRX_REGION();

	DISABLE_TRX_INTERRUPT();

	/* Prepare the command byte */
	addr |= 0X8000; //Write Command
	
	
	uint8_t reg_addr;

	/* Start SPI transaction by pulling SEL low */
	spi_select_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);

	reg_addr = addr>>8;
	
	/* Send the Read command byte */
	spi_write_packet(AT86RFX_SPI, &reg_addr, 1);

	reg_addr = addr;
	
	/* Send the Read command byte */
	spi_write_packet(AT86RFX_SPI, &reg_addr, 1);

	uint32_t i = 0;
	uint8_t val;

	while (length) {
		val = data[i];
		while (!spi_is_tx_ready(AT86RFX_SPI));
		spi_write_single(AT86RFX_SPI, val);
		i++;
		length--;
	}
	/* Stop the SPI transaction by setting SEL high */
	spi_deselect_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);

	/*Restoring the interrupt status which was stored & enabling the global
	 *interrupt */ 
	 ENABLE_TRX_INTERRUPT();
	//LEAVE_TRX_REGION();
}

uint8_t pal_trx_reg_read(uint16_t addr)
{
	uint8_t register_value = 0;

	/*Saving the current interrupt status & disabling the global interrupt
	 **/
	ENTER_CRITICAL_REGION();

	/* Prepare the command byte */
	addr |= 0X00; //Read Command
	
	
	uint8_t reg_addr;
	
	/* Start SPI transaction by pulling SEL low */
	spi_select_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);
	
	reg_addr = addr>>8;
	
	/* Send the Read command byte */
	spi_write_packet(AT86RFX_SPI, &reg_addr, 1);	

	reg_addr = addr;

	/* Send the Read command byte */
	spi_write_packet(AT86RFX_SPI, &reg_addr, 1);

	/* Do dummy read for initiating SPI read */
	spi_read_packet(AT86RFX_SPI, &register_value, 1);
	
	/* Stop the SPI transaction by setting SEL high */
	spi_deselect_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);

	/*Restoring the interrupt status which was stored & enabling the global
	 *interrupt */
	LEAVE_CRITICAL_REGION();

	return register_value;
}

void pal_trx_reg_write(uint16_t addr, uint8_t data)
{
	/*Saving the current interrupt status & disabling the global interrupt
	 **/
	ENTER_CRITICAL_REGION();

	/* Prepare the command byte */
	addr |= 0X8000; //Write Command
	
	
	uint8_t reg_addr;

	/* Start SPI transaction by pulling SEL low */
	spi_select_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);

	reg_addr = addr>>8;
	
	/* Send the Read command byte */
	spi_write_packet(AT86RFX_SPI, &reg_addr, 1);

	reg_addr = addr;
	
	/* Send the Read command byte */
	spi_write_packet(AT86RFX_SPI, &reg_addr, 1);

	/* Write the byte in the transceiver data register */
	spi_write_packet(AT86RFX_SPI, &data, 1);

	/* Stop the SPI transaction by setting SEL high */
	spi_deselect_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);

	/*Restoring the interrupt status which was stored & enabling the global
	 *interrupt */
	LEAVE_CRITICAL_REGION();
}

void pal_trx_irq_init(FUNC_PTR trx_irq_cb)
{
	/*
	 * Set the handler function.
	 * The handler is set before enabling the interrupt to prepare for
	 *spurious
	 * interrupts, that can pop up the moment they are enabled
	 */
	irq_hdl_trx = (irq_handler_t)trx_irq_cb;
}

uint8_t pal_trx_bit_read(uint16_t addr, uint8_t mask, uint8_t pos)
{
	uint8_t ret;
	ret = pal_trx_reg_read(addr);
	ret &= mask;
	ret >>= pos;
	return ret;
}

void pal_trx_bit_write(uint16_t reg_addr, uint8_t mask, uint8_t pos,
		uint8_t new_value)
{
	uint8_t current_reg_value;
	current_reg_value = pal_trx_reg_read(reg_addr);
	current_reg_value &= ~mask;
	new_value <<= pos;
	new_value &= mask;
	new_value |= current_reg_value;
	pal_trx_reg_write(reg_addr, new_value);
}


