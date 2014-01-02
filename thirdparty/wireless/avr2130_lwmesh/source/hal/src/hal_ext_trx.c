 /*
 * @file pal.c
 *
 * @brief Performs interface functionalities between the TAL layer and ASF drivers
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
#if SAMD20
#include "spi.h"
#else
#include "spi_master.h"
#endif
#include "hal_ext_trx.h"
#include "hal.h"
#include "delay.h"
#include "interrupt.h"
#include "conf_board.h"

static irq_handler_t irq_hdl_trx = NULL;

#if SAMD20
struct spi_slave_inst_config slave_dev_config;
struct spi_config config;
struct spi_module master;
struct spi_slave_inst slave;
uint16_t dummy_read;
#else
struct spi_device SPI_AT86RFX_DEVICE = {
	//! Board specific select id
	.id = AT86RFX_SPI_CS
};
#endif

#if SAMD20
void AT86RFX_ISR(uint32_t i);
void AT86RFX_ISR(uint32_t i)
#else
AT86RFX_ISR()
#endif
 
{
    /*Clearing the RF interrupt*/
    pal_trx_irq_flag_clr();

    /*Calling the interrupt routines*/
    if(irq_hdl_trx)
    {
      irq_hdl_trx();
    }
}

void pal_spi_init(void)
{
	/* Initialize SPI in master mode to access the transceiver */
#if SAMD20
	spi_slave_inst_get_config_defaults(&slave_dev_config);
	slave_dev_config.ss_pin = AT86RFX_SPI_CS;
	spi_attach_slave(&slave, &slave_dev_config);
	spi_get_config_defaults(&config);
	AT86RFX_SPI_CONFIG(config);
	spi_init(&master, AT86RFX_SPI, &config);
	spi_enable(&master);
	AT86RFX_INTC_INIT();
#else
	spi_master_init(AT86RFX_SPI);
	spi_master_setup_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE, SPI_MODE_0,
			AT86RFX_SPI_BAUDRATE, 0);
	spi_enable(AT86RFX_SPI);
	AT86RFX_INTC_INIT();
#endif
}

uint8_t pal_trx_reg_read(uint8_t addr)
{
#if SAMD20	
  uint16_t register_value = 0;
#else
  uint8_t register_value = 0;  
#endif  

	/*Saving the current interrupt status & disabling the global interrupt */
	ENTER_CRITICAL_REGION();

	/* Prepare the command byte */
	addr |= READ_ACCESS_COMMAND;
    
#if SAMD20
    /* Start SPI transaction by pulling SEL low */
	spi_select_slave(&master, &slave, true);

	/* Send the Read command byte */
	while(!spi_is_ready_to_write(&master));
	spi_write(&master,addr);
	while(!spi_is_write_complete(&master));
	/* Dummy read since SPI RX is double buffered */
	while(!spi_is_ready_to_read(&master));
	spi_read(&master, &dummy_read);

	while(!spi_is_ready_to_write(&master));
	spi_write(&master,0);
	while(!spi_is_write_complete(&master));
	while(!spi_is_ready_to_read(&master));
	spi_read(&master, &register_value);

	/* Stop the SPI transaction by setting SEL high */
	spi_select_slave(&master, &slave, false);
    
   
#else

	/* Start SPI transaction by pulling SEL low */
	spi_select_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);

	/* Send the Read command byte */
	spi_write_packet(AT86RFX_SPI, &addr, 1);

	/* Do dummy read for initiating SPI read */
	spi_read_packet(AT86RFX_SPI, &register_value, 1);

	/* Stop the SPI transaction by setting SEL high */
	spi_deselect_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);
 
#endif
    	/*Restoring the interrupt status which was stored & enabling the global interrupt */
	LEAVE_CRITICAL_REGION();

	return register_value; 

}

void HAL_PhyReset(void)
{
  /* Ensure control lines have correct levels. */
  PAL_RST_HIGH();
  PAL_SLP_TR_LOW();        
  
  /* Wait typical time of timer TR1. */
  HAL_Delay(330);
  
  RST_LOW();
  HAL_Delay(10);
  RST_HIGH();
}

void pal_trx_reg_write(uint8_t addr, uint8_t data)
{
	/*Saving the current interrupt status & disabling the global interrupt */
	ENTER_CRITICAL_REGION();

	/* Prepare the command byte */
	addr |= WRITE_ACCESS_COMMAND;

#if SAMD20
    /* Start SPI transaction by pulling SEL low */
	spi_select_slave(&master, &slave, true);

	/* Send the Read command byte */
	while(!spi_is_ready_to_write(&master));
	spi_write(&master,addr);
	while(!spi_is_write_complete(&master));
	/* Dummy read since SPI RX is double buffered */
	while(!spi_is_ready_to_read(&master));
	spi_read(&master, &dummy_read);

	/* Write the byte in the transceiver data register */
	while(!spi_is_ready_to_write(&master));
	spi_write(&master,data);
	while(!spi_is_write_complete(&master));
	/* Dummy read since SPI RX is double buffered */
	while(!spi_is_ready_to_read(&master));
	spi_read(&master, &dummy_read);

	/* Stop the SPI transaction by setting SEL high */
	spi_select_slave(&master, &slave, false);
#else
	/* Start SPI transaction by pulling SEL low */
	spi_select_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);

	/* Send the Read command byte */
	spi_write_packet(AT86RFX_SPI, &addr, 1);

	/* Write the byte in the transceiver data register */
	spi_write_packet(AT86RFX_SPI, &data, 1);

	/* Stop the SPI transaction by setting SEL high */
	spi_deselect_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);
#endif
	/*Restoring the interrupt status which was stored & enabling the global interrupt */
	LEAVE_CRITICAL_REGION();
}


void pal_trx_irq_init(FUNC_PTR trx_irq_cb)
{
    /*
     * Set the handler function.
     * The handler is set before enabling the interrupt to prepare for spurious
     * interrupts, that can pop up the moment they are enabled
     */
    irq_hdl_trx = (irq_handler_t)trx_irq_cb;
}

uint8_t pal_trx_bit_read(uint8_t addr, uint8_t mask, uint8_t pos)
{
	uint8_t ret;
	ret = pal_trx_reg_read(addr);
	ret &= mask;
	ret >>= pos;
	return ret;
}

void pal_trx_bit_write(uint8_t reg_addr, uint8_t mask, uint8_t pos, uint8_t new_value)
{
	uint8_t current_reg_value;
	current_reg_value = pal_trx_reg_read(reg_addr);
	current_reg_value &= ~mask;
	new_value <<= pos;
	new_value &= mask;
	new_value |= current_reg_value;
	pal_trx_reg_write(reg_addr, new_value);
}

void pal_trx_frame_read(uint8_t *data, uint8_t length)
{
	

	/*Saving the current interrupt status & disabling the global interrupt */
	ENTER_CRITICAL_REGION();

#if SAMD20
    
    uint16_t temp;
        /* Start SPI transaction by pulling SEL low */
	spi_select_slave(&master, &slave, true);

	temp = TRX_CMD_FR;

	/* Send the command byte */
	while(!spi_is_ready_to_write(&master));
	spi_write(&master,temp);
	while(!spi_is_write_complete(&master));
	/* Dummy read since SPI RX is double buffered */
	while(!spi_is_ready_to_read(&master));
	spi_read(&master, &dummy_read);

	while(length--){
		while(!spi_is_ready_to_write(&master));
		spi_write(&master,0);
		while(!spi_is_write_complete(&master));
		/* Dummy read since SPI RX is double buffered */
		while(!spi_is_ready_to_read(&master));
		spi_read(&master, &temp);
		*data = temp;
		data++;
	}

	/* Stop the SPI transaction by setting SEL high */
	spi_select_slave(&master, &slave, false);
#else 
    uint8_t temp;
	/* Start SPI transaction by pulling SEL low */
	spi_select_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);

	temp = TRX_CMD_FR;

	/* Send the command byte */
	spi_write_packet(AT86RFX_SPI, &temp, 1);

	spi_read_packet(AT86RFX_SPI, data, length);

	/* Stop the SPI transaction by setting SEL high */
	spi_deselect_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);
#endif
	/*Restoring the interrupt status which was stored & enabling the global interrupt */
	LEAVE_CRITICAL_REGION();
}

void pal_trx_frame_write(uint8_t *data, uint8_t length)
{
	uint8_t temp;
	/*Saving the current interrupt status & disabling the global interrupt */
	ENTER_CRITICAL_REGION();

#if SAMD20
        /* Start SPI transaction by pulling SEL low */
	spi_select_slave(&master, &slave, true);

	temp = TRX_CMD_FW;

	/* Send the command byte */
	while(!spi_is_ready_to_write(&master));
	spi_write(&master,temp);
	while(!spi_is_write_complete(&master));
	/* Dummy read since SPI RX is double buffered */
	while(!spi_is_ready_to_read(&master));
	spi_read(&master, &dummy_read);
	
    length= length+2;
	while(!spi_is_ready_to_write(&master));
	spi_write(&master,length);
	while(!spi_is_write_complete(&master));
	/* Dummy read since SPI RX is double buffered */
	while(!spi_is_ready_to_read(&master));
	spi_read(&master, &dummy_read);
	
    length= length-2;
	while(length--){
		while(!spi_is_ready_to_write(&master));
		spi_write(&master,*data++);
		while(!spi_is_write_complete(&master));
		/* Dummy read since SPI RX is double buffered */
		while(!spi_is_ready_to_read(&master));
		spi_read(&master, &dummy_read);
	}

	/* Stop the SPI transaction by setting SEL high */
	spi_select_slave(&master, &slave, false);
#else
	/* Start SPI transaction by pulling SEL low */
	spi_select_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);

	temp = TRX_CMD_FW;

	/* Send the command byte */
	spi_write_packet(AT86RFX_SPI, &temp, 1);
        length= length+2;
        spi_write_packet(AT86RFX_SPI, &length, 1);
        length= length-2;
	spi_write_packet(AT86RFX_SPI, data, length);

	/* Stop the SPI transaction by setting SEL high */
	spi_deselect_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);
#endif
	/*Restoring the interrupt status which was stored & enabling the global interrupt */
	LEAVE_CRITICAL_REGION();
}



