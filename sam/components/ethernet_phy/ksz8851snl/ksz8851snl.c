/**
 *
 * \file
 *
 * \brief KS8851SNL driver for SAM.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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

#include "spi_master.h"
#include "ksz8851snl.h"
#include "ksz8851snl_reg.h"
#include "delay.h"
#include "pdc.h"
#include "conf_eth.h"

/* Clock polarity. */
#define SPI_CLK_POLARITY 0

/* Clock phase. */
#define SPI_CLK_PHASE 0

/* SPI PDC register base. */
Pdc *g_p_spi_pdc = 0;

static void ksz8851_reg_setbits(uint16_t reg, uint16_t bits_to_set)
{
   uint16_t	temp;

   temp = ksz8851_reg_read(reg);
   temp |= bits_to_set;
   ksz8851_reg_write(reg, temp);
}

static void ksz8851_reg_clrbits(uint16_t reg, uint16_t bits_to_clr)
{
   uint16_t	temp;

   temp = ksz8851_reg_read(reg);
   temp &= ~bits_to_clr;
   ksz8851_reg_write(reg, temp);
}

uint16_t ksz8851_reg_read(uint16_t reg)
{
	pdc_packet_t g_pdc_spi_tx_packet;
	pdc_packet_t g_pdc_spi_rx_packet;
	uint8_t	inbuf[5] = {'0xA5', '0xA5', '0xA5', '0xA5', '0xA5'};
	uint8_t	outbuf[5];
	uint16_t cmd = 0;
	volatile uint16_t res = 0;

	/* Move register address to cmd bits 9-2, make 32-bit address. */
	cmd = (reg << 2) & REG_ADDR_MASK;

	/* Last 2 bits still under "don't care bits" handled with byte enable. */
	/* Select byte enable for command. */
	if (reg & 2) {
		/* Odd word address writes bytes 2 and 3 */
		cmd |= (0xc << 10);
	} else {
		/* Even word address write bytes 0 and 1 */
		cmd |= (0x3 << 10);
	}

	/* Add command read code. */
	cmd |= CMD_READ;
	outbuf[0] = cmd >> 8;
	outbuf[1] = cmd & 0xff;
	outbuf[2] = CONFIG_SPI_MASTER_DUMMY;
	outbuf[3] = CONFIG_SPI_MASTER_DUMMY;

	/* Write 2 bytes for command and read 2 dummy bytes. */
	g_pdc_spi_tx_packet.ul_addr = (uint32_t) outbuf;
	g_pdc_spi_tx_packet.ul_size = 4;
	g_pdc_spi_rx_packet.ul_addr = (uint32_t) inbuf;
	g_pdc_spi_rx_packet.ul_size = 4;
	pdc_tx_init(g_p_spi_pdc, &g_pdc_spi_tx_packet, NULL);
	pdc_rx_init(g_p_spi_pdc, &g_pdc_spi_rx_packet, NULL);
	while (!(spi_read_status(KSZ8851SNL_SPI) & SPI_SR_ENDRX))
		;

	res = (inbuf[3] << 8) | inbuf[2];
	return res;
}

void ksz8851_reg_write(uint16_t reg, uint16_t wrdata)
{
	pdc_packet_t g_pdc_spi_tx_packet;
	pdc_packet_t g_pdc_spi_rx_packet;
	uint8_t	inbuf[4];
	uint8_t	outbuf[4];
	uint16_t cmd = 0;

   //printf("ksz8851_regwr: writing 0x%.4x  to  0x%.4x\n", wrdata, reg);

	/* Move register address to cmd bits 9-2, make 32-bit address. */
	cmd = (reg << 2) & REG_ADDR_MASK;

	/* Last 2 bits still under "don't care bits" handled with byte enable. */
	/* Select byte enable for command. */
	if (reg & 2) {
		/* Odd word address writes bytes 2 and 3 */
		cmd |= (0xc << 10);
	} else {
		/* Even word address write bytes 0 and 1 */
		cmd |= (0x3 << 10);
	}

	/* Add opcode to cmd */
	cmd |= CMD_WRITE;
	outbuf[0] = cmd >> 8;
	outbuf[1] = cmd & 0xff;
	outbuf[2] = wrdata & 0xff;
	outbuf[3] = wrdata >> 8;

	/* Write 2 bytes for command and read 2 dummy bytes. */
	g_pdc_spi_tx_packet.ul_addr = (uint32_t) outbuf;
	g_pdc_spi_tx_packet.ul_size = 4;
	g_pdc_spi_rx_packet.ul_addr = (uint32_t) inbuf;
	g_pdc_spi_rx_packet.ul_size = 4;
	pdc_tx_init(g_p_spi_pdc, &g_pdc_spi_tx_packet, NULL);
	pdc_rx_init(g_p_spi_pdc, &g_pdc_spi_rx_packet, NULL);
	while (!(spi_read_status(KSZ8851SNL_SPI) & SPI_SR_ENDTX))
		;
}

uint32_t ksz8851snl_init(void)
{
	uint32_t count = 0;
	uint16_t dev_id = 0;
	board_spi_select_id_t spi_select_id = 3;
	struct spi_device device = {
		.id = KSZ8851SNL_CS_PIN,
	};

	/* Configure the SPI peripheral. */
	spi_enable_clock(KSZ8851SNL_SPI);
	spi_disable(KSZ8851SNL_SPI);
	spi_reset(KSZ8851SNL_SPI);
	spi_set_lastxfer(KSZ8851SNL_SPI);
	spi_set_master_mode(KSZ8851SNL_SPI);
	spi_disable_mode_fault_detect(KSZ8851SNL_SPI);
	spi_set_peripheral_chip_select_value(KSZ8851SNL_SPI, ~(1 << KSZ8851SNL_CS_PIN));
	spi_set_clock_polarity(KSZ8851SNL_SPI, KSZ8851SNL_CS_PIN, SPI_CLK_POLARITY);
	spi_set_clock_phase(KSZ8851SNL_SPI, KSZ8851SNL_CS_PIN, SPI_CLK_PHASE);
	spi_set_bits_per_transfer(KSZ8851SNL_SPI, KSZ8851SNL_CS_PIN,
			SPI_CSR_BITS_8_BIT);
	spi_set_baudrate_div(KSZ8851SNL_SPI, KSZ8851SNL_CS_PIN, 3);
	spi_set_transfer_delay(KSZ8851SNL_SPI, KSZ8851SNL_CS_PIN, CONFIG_SPI_MASTER_DELAY_BS,
			CONFIG_SPI_MASTER_DELAY_BCT);
	//spi_configure_cs_behavior(KSZ8851SNL_SPI, KSZ8851SNL_CS_PIN, SPI_CS_RISE_FORCED);
	spi_enable(KSZ8851SNL_SPI);

	/* Get pointer to UART PDC register base. */
	g_p_spi_pdc = spi_get_pdc_base(KSZ8851SNL_SPI);
	pdc_enable_transfer(g_p_spi_pdc, PERIPH_PTCR_RXTEN | PERIPH_PTCR_TXTEN);

	/* Reset the Micrel in a proper state. */
	do {
		/* Configure RSTN pin and perform hardware reset. */
		gpio_configure_pin(KSZ8851SNL_RSTN_GPIO, KSZ8851SNL_RSTN_FLAGS);
		gpio_set_pin_low(KSZ8851SNL_RSTN_GPIO);
		delay_ms(10);
		gpio_set_pin_high(KSZ8851SNL_RSTN_GPIO);
		/* Reset timing should be 10 at minimum. */
		delay_ms(10);

		/* Perform Global Soft Reset */
		ksz8851_reg_setbits(REG_RESET_CTRL, GLOBAL_SOFTWARE_RESET);
		ksz8851_reg_clrbits(REG_RESET_CTRL, GLOBAL_SOFTWARE_RESET);

		/* Read chip ID. */
		dev_id = ksz8851_reg_read(REG_CHIP_ID);
		if (++count > 10)
			return 1;
	} while ((dev_id & 0xFFF0) != CHIP_ID_8851_16);


	return 0;
}
