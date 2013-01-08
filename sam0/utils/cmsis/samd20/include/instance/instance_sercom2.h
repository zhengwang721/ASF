/**
 * \file
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

#ifndef _SAMD20_SERCOM2_INSTANCE_
#define _SAMD20_SERCOM2_INSTANCE_

/* ========== Register definition for SERCOM2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_SERCOM2_I2CM_CTRLA     (0x42001000U) /**< \brief (SERCOM2) I2cm I2CM Control Register A */
#define REG_SERCOM2_I2CS_CTRLA     (0x42001000U) /**< \brief (SERCOM2) I2cs I2CS Control Register A */
#define REG_SERCOM2_SPI_CTRLA      (0x42001000U) /**< \brief (SERCOM2) Spi SPI Control Register A */
#define REG_SERCOM2_USART_CTRLA    (0x42001000U) /**< \brief (SERCOM2) Usart USART Control Register A */
#define REG_SERCOM2_I2CM_CTRLB     (0x42001004U) /**< \brief (SERCOM2) I2cm I2CM Control Register B */
#define REG_SERCOM2_I2CS_CTRLB     (0x42001004U) /**< \brief (SERCOM2) I2cs I2CS Control Register B */
#define REG_SERCOM2_SPI_CTRLB      (0x42001004U) /**< \brief (SERCOM2) Spi SPI Control Register B */
#define REG_SERCOM2_USART_CTRLB    (0x42001004U) /**< \brief (SERCOM2) Usart USART Control Register B */
#define REG_SERCOM2_I2CM_DEBUG     (0x42001008U) /**< \brief (SERCOM2) I2cm I2CM Debug Register */
#define REG_SERCOM2_I2CS_DEBUG     (0x42001008U) /**< \brief (SERCOM2) I2cs I2CS Debug Register */
#define REG_SERCOM2_SPI_DEBUG      (0x42001008U) /**< \brief (SERCOM2) Spi SPI Debug Register */
#define REG_SERCOM2_USART_DEBUG    (0x42001008U) /**< \brief (SERCOM2) Usart USART Debug Register */
#define REG_SERCOM2_I2CM_BAUD      (0x4200100AU) /**< \brief (SERCOM2) I2cm I2CM Baud Rate Register */
#define REG_SERCOM2_SPI_BAUD       (0x4200100AU) /**< \brief (SERCOM2) Spi SPI Baud Rate Register */
#define REG_SERCOM2_USART_BAUD     (0x4200100AU) /**< \brief (SERCOM2) Usart USART Baud Rate Register */
#define REG_SERCOM2_I2CM_INTENCLR  (0x4200100CU) /**< \brief (SERCOM2) I2cm I2CM Interrupt Enable Clear Register */
#define REG_SERCOM2_I2CS_INTENCLR  (0x4200100CU) /**< \brief (SERCOM2) I2cs I2CS Interrupt Enable Clear Register */
#define REG_SERCOM2_SPI_INTENCLR   (0x4200100CU) /**< \brief (SERCOM2) Spi SPI Interrupt Enable Clear Register */
#define REG_SERCOM2_USART_INTENCLR (0x4200100CU) /**< \brief (SERCOM2) Usart USART Interrupt Enable Clear Register */
#define REG_SERCOM2_I2CM_INTENSET  (0x4200100DU) /**< \brief (SERCOM2) I2cm I2CM Interrupt Enable Set Register */
#define REG_SERCOM2_I2CS_INTENSET  (0x4200100DU) /**< \brief (SERCOM2) I2cs I2CS Interrupt Enable Set Register */
#define REG_SERCOM2_SPI_INTENSET   (0x4200100DU) /**< \brief (SERCOM2) Spi SPI Interrupt Enable Set Register */
#define REG_SERCOM2_USART_INTENSET (0x4200100DU) /**< \brief (SERCOM2) Usart USART Interrupt Enable Set Register */
#define REG_SERCOM2_I2CM_INTFLAG   (0x4200100EU) /**< \brief (SERCOM2) I2cm I2CM Interrupt Flag Status and Clear Register */
#define REG_SERCOM2_I2CS_INTFLAG   (0x4200100EU) /**< \brief (SERCOM2) I2cs I2CS Interrupt Flag Status and Clear Register */
#define REG_SERCOM2_SPI_INTFLAG    (0x4200100EU) /**< \brief (SERCOM2) Spi SPI Interrupt Flag Status and Clear Register */
#define REG_SERCOM2_USART_INTFLAG  (0x4200100EU) /**< \brief (SERCOM2) Usart USART Interrupt Flag Status and Clear Register */
#define REG_SERCOM2_I2CM_STATUS    (0x42001010U) /**< \brief (SERCOM2) I2cm I2CM Status Register */
#define REG_SERCOM2_I2CS_STATUS    (0x42001010U) /**< \brief (SERCOM2) I2cs I2CS Status Register */
#define REG_SERCOM2_SPI_STATUS     (0x42001010U) /**< \brief (SERCOM2) Spi SPI Status Register */
#define REG_SERCOM2_USART_STATUS   (0x42001010U) /**< \brief (SERCOM2) Usart USART Status Register */
#define REG_SERCOM2_I2CM_ADDR      (0x42001014U) /**< \brief (SERCOM2) I2cm I2CM Address Register */
#define REG_SERCOM2_I2CS_ADDR      (0x42001014U) /**< \brief (SERCOM2) I2cs I2CS Address Register */
#define REG_SERCOM2_SPI_ADDR       (0x42001014U) /**< \brief (SERCOM2) Spi SPI Address Register */
#define REG_SERCOM2_I2CM_DATA      (0x42001018U) /**< \brief (SERCOM2) I2cm I2CM Data Register */
#define REG_SERCOM2_I2CS_DATA      (0x42001018U) /**< \brief (SERCOM2) I2cs I2CS Data Register */
#define REG_SERCOM2_SPI_DATA       (0x42001018U) /**< \brief (SERCOM2) Spi SPI Data Register */
#define REG_SERCOM2_USART_DATA     (0x42001018U) /**< \brief (SERCOM2) Usart USART Data Register */
#else
#define REG_SERCOM2_CTRLA          (*(RwReg  *)0x42001000U) /**< \brief (SERCOM2) I2CM Control Register A */
#define REG_SERCOM2_CTRLA          (*(RwReg  *)0x42001000U) /**< \brief (SERCOM2) I2CS Control Register A */
#define REG_SERCOM2_CTRLA          (*(RwReg  *)0x42001000U) /**< \brief (SERCOM2) SPI Control Register A */
#define REG_SERCOM2_CTRLA          (*(RwReg  *)0x42001000U) /**< \brief (SERCOM2) USART Control Register A */
#define REG_SERCOM2_CTRLB          (*(RwReg  *)0x42001004U) /**< \brief (SERCOM2) I2CM Control Register B */
#define REG_SERCOM2_CTRLB          (*(RwReg  *)0x42001004U) /**< \brief (SERCOM2) I2CS Control Register B */
#define REG_SERCOM2_CTRLB          (*(RwReg  *)0x42001004U) /**< \brief (SERCOM2) SPI Control Register B */
#define REG_SERCOM2_CTRLB          (*(RwReg  *)0x42001004U) /**< \brief (SERCOM2) USART Control Register B */
#define REG_SERCOM2_DEBUG          (*(RwReg8 *)0x42001008U) /**< \brief (SERCOM2) I2CM Debug Register */
#define REG_SERCOM2_DEBUG          (*(RwReg8 *)0x42001008U) /**< \brief (SERCOM2) I2CS Debug Register */
#define REG_SERCOM2_DEBUG          (*(RwReg8 *)0x42001008U) /**< \brief (SERCOM2) SPI Debug Register */
#define REG_SERCOM2_DEBUG          (*(RwReg8 *)0x42001008U) /**< \brief (SERCOM2) USART Debug Register */
#define REG_SERCOM2_BAUD           (*(RwReg16*)0x4200100AU) /**< \brief (SERCOM2) I2CM Baud Rate Register */
#define REG_SERCOM2_BAUD           (*(RwReg8 *)0x4200100AU) /**< \brief (SERCOM2) SPI Baud Rate Register */
#define REG_SERCOM2_BAUD           (*(RwReg16*)0x4200100AU) /**< \brief (SERCOM2) USART Baud Rate Register */
#define REG_SERCOM2_INTENCLR       (*(RwReg8 *)0x4200100CU) /**< \brief (SERCOM2) I2CM Interrupt Enable Clear Register */
#define REG_SERCOM2_INTENCLR       (*(RwReg8 *)0x4200100CU) /**< \brief (SERCOM2) I2CS Interrupt Enable Clear Register */
#define REG_SERCOM2_INTENCLR       (*(RwReg8 *)0x4200100CU) /**< \brief (SERCOM2) SPI Interrupt Enable Clear Register */
#define REG_SERCOM2_INTENCLR       (*(RwReg8 *)0x4200100CU) /**< \brief (SERCOM2) USART Interrupt Enable Clear Register */
#define REG_SERCOM2_INTENSET       (*(RwReg8 *)0x4200100DU) /**< \brief (SERCOM2) I2CM Interrupt Enable Set Register */
#define REG_SERCOM2_INTENSET       (*(RwReg8 *)0x4200100DU) /**< \brief (SERCOM2) I2CS Interrupt Enable Set Register */
#define REG_SERCOM2_INTENSET       (*(RwReg8 *)0x4200100DU) /**< \brief (SERCOM2) SPI Interrupt Enable Set Register */
#define REG_SERCOM2_INTENSET       (*(RwReg8 *)0x4200100DU) /**< \brief (SERCOM2) USART Interrupt Enable Set Register */
#define REG_SERCOM2_INTFLAG        (*(RwReg8 *)0x4200100EU) /**< \brief (SERCOM2) I2CM Interrupt Flag Status and Clear Register */
#define REG_SERCOM2_INTFLAG        (*(RwReg8 *)0x4200100EU) /**< \brief (SERCOM2) I2CS Interrupt Flag Status and Clear Register */
#define REG_SERCOM2_INTFLAG        (*(RwReg8 *)0x4200100EU) /**< \brief (SERCOM2) SPI Interrupt Flag Status and Clear Register */
#define REG_SERCOM2_INTFLAG        (*(RwReg8 *)0x4200100EU) /**< \brief (SERCOM2) USART Interrupt Flag Status and Clear Register */
#define REG_SERCOM2_STATUS         (*(RwReg16*)0x42001010U) /**< \brief (SERCOM2) I2CM Status Register */
#define REG_SERCOM2_STATUS         (*(RwReg16*)0x42001010U) /**< \brief (SERCOM2) I2CS Status Register */
#define REG_SERCOM2_STATUS         (*(RwReg16*)0x42001010U) /**< \brief (SERCOM2) SPI Status Register */
#define REG_SERCOM2_STATUS         (*(RwReg16*)0x42001010U) /**< \brief (SERCOM2) USART Status Register */
#define REG_SERCOM2_ADDR           (*(RwReg8 *)0x42001014U) /**< \brief (SERCOM2) I2CM Address Register */
#define REG_SERCOM2_ADDR           (*(RwReg  *)0x42001014U) /**< \brief (SERCOM2) I2CS Address Register */
#define REG_SERCOM2_ADDR           (*(RwReg  *)0x42001014U) /**< \brief (SERCOM2) SPI Address Register */
#define REG_SERCOM2_DATA           (*(RwReg8 *)0x42001018U) /**< \brief (SERCOM2) I2CM Data Register */
#define REG_SERCOM2_DATA           (*(RwReg8 *)0x42001018U) /**< \brief (SERCOM2) I2CS Data Register */
#define REG_SERCOM2_DATA           (*(RwReg16*)0x42001018U) /**< \brief (SERCOM2) SPI Data Register */
#define REG_SERCOM2_DATA           (*(RwReg16*)0x42001018U) /**< \brief (SERCOM2) USART Data Register */
#endif /* (defined(__ASSEMBLY__) || defines(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for SERCOM2 peripheral ========== */
#define SERCOM2_GCLK_ID_CORE        15
#define SERCOM2_GCLK_ID_SLOW        12
#define SERCOM2_INT_MSB             3
#define SERCOM2_PMSB                3

#endif /* _SAMD20_SERCOM2_INSTANCE_ */
