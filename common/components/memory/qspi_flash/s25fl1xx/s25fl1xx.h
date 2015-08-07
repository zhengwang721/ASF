/**
 * \file
 *
 * \brief QSPI flash memory driver for S25FL116K.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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

/**
 * \file
 *
 * Interface for the S25fl1 Serialflash driver.
 *
 */

#ifndef S25FL1XX_H
#define S25FL1XX_H

#include "qspi.h"

/*----------------------------------------------------------------------------
 *        Macros
 *----------------------------------------------------------------------------*/

#define Size(pAt25)                      ((pAt25)->pDesc->size)
#define PageSize(pAt25)                  ((pAt25)->pDesc->pageSize)
#define BlockSize(pAt25)                 ((pAt25)->pDesc->blockSize)
#define Name(pAt25)                      ((pAt25)->pDesc->name)
#define ManId(pAt25)                     (((pAt25)->pDesc->jedecId) & 0xFF)
#define PageNumber(pAt25)                (Size(pAt25) / PageSize(pAt25))
#define BlockNumber(pAt25)               (Size(pAt25) / BlockSize(pAt25))
#define PagePerBlock(pAt25)              (BlockSize(pAt25) / PageSize(pAt25))
#define BlockEraseCmd(pAt25)             ((pAt25)->pDesc->blockEraseCmd)

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

/** Device is protected, operation cannot be carried out. */
#define ERROR_PROTECTED                  1
/** Device is busy executing a command. */
#define ERROR_BUSY                       2
/** There was a problem while trying to program page data. */
#define ERROR_PROGRAM                    3
/** There was an SPI communication error. */
#define ERROR_SPI                        4

/** Device ready/busy status bit. */
#define STATUS_RDYBSY                    (1 << 0)
/** Device is ready. */
#define STATUS_RDYBSY_READY              (0 << 0)
/** Device is busy with internal operations. */
#define STATUS_RDYBSY_BUSY               (1 << 0)
/** Write enable latch status bit. */
#define STATUS_WEL                       (1 << 1)
/** Device is not write enabled. */
#define STATUS_WEL_DISABLED              (0 << 1)
/** Device is write enabled. */
#define STATUS_WEL_ENABLED               (1 << 1)
/** Software protection status bit-field. */
#define STATUS_SWP                       (3 << 2)
/** All sectors are software protected. */
#define STATUS_SWP_PROTALL               (3 << 2)
/** Some sectors are software protected. */
#define STATUS_SWP_PROTSOME              (1 << 2)
/** No sector is software protected. */
#define STATUS_SWP_PROTNONE              (0 << 2)
/** Write protect pin status bit. */
#define STATUS_WPP                       (1 << 4)
/** Write protect signal is not asserted. */
#define STATUS_WPP_NOTASSERTED           (0 << 4)
/** Write protect signal is asserted. */
#define STATUS_WPP_ASSERTED              (1 << 4)
/** Erase/program error bit. */
#define STATUS_EPE                       (1 << 5)
/** Erase or program operation was successful. */
#define STATUS_EPE_SUCCESS               (0 << 5)
/** Erase or program error detected. */
#define STATUS_EPE_ERROR                 (1 << 5)
/** Sector protection registers locked bit. */
#define STATUS_SPRL                      (1 << 7)
/** Sector protection registers are unlocked. */
#define STATUS_SPRL_UNLOCKED             (0 << 7)
/** Sector protection registers are locked. */
#define STATUS_SPRL_LOCKED               (1 << 7)
/** Quad enable bit */
#define STATUS_QUAD_ENABLE               (1 << 1)
/** Quad enable bit */
#define STATUS_WRAP_ENABLE               (0 << 4)
/** Latency control bits */
#define STATUS_LATENCY_CTRL              (0xF << 0)
#define STATUS_WRAP_BYTE                 (1 << 5)
#define BLOCK_PROTECT_Msk                (7 << 2)
#define TOP_BTM_PROTECT_Msk              (1 << 5)
#define SEC_PROTECT_Msk                  (1 << 6)
#define CHIP_PROTECT_Msk                 (0x1F << 2)

/** Sequential program mode command code 1. */
#define SEQUENTIAL_PROGRAM_1             0xAD
/** Sequential program mode command code 2. */
#define SEQUENTIAL_PROGRAM_2             0xAF
/** Protect sector command code. */
#define PROTECT_SECTOR                   0x36
/** Unprotected sector command code. */
#define UNPROTECT_SECTOR                 0x39
/** Read sector protection registers command code. */
#define READ_SECTOR_PROT                 0x3C
/** Resume from deep power-down command code. */
#define SOFT_RESET_ENABLE                0x66
/** Resume from deep power-down command code. */
#define SOFT_RESET                       0x99
/** Read status register command code. */
#define READ_STATUS_1                    0x05
/** Read status register command code. */
#define READ_STATUS_2                    0x35
/** Read status register command code. */
#define READ_STATUS_3                    0x33
/** Write enable command code. */
#define WRITE_ENABLE                     0x06
/** Write Enable for Volatile Status Register. */
#define WRITE_ENABLE_FOR_VOLATILE_STATUS 0x50
/** Write disable command code. */
#define WRITE_DISABLE                    0x04
/** Write status register command code. */
#define WRITE_STATUS                     0x01
/** Resume from deep power-down command code. */
#define WRAP_ENABLE                      0x77
/** Byte/page program command code. */
#define BYTE_PAGE_PROGRAM                0x02
/** Block erase command code (4K block). */
#define BLOCK_ERASE_4K                   0x20
/** Block erase command code (32K block). */
#define BLOCK_ERASE_32K                  0x52
/** Block erase command code (64K block). */
#define BLOCK_ERASE_64K                  0xD8
/** Chip erase command code 1. */
#define CHIP_ERASE_1                     0x60
/** Chip erase command code 2. */
#define CHIP_ERASE_2                     0xC7
/** Read array (low frequency) command code. */
#define READ_ARRAY_LF                    0x03
/** Read array command code. */
#define READ_ARRAY                       0x0B
/** Fast Read array  command code. */
#define READ_ARRAY_DUAL                  0x3B
/** Fast Read array  command code. */
#define READ_ARRAY_QUAD                  0x6B
/** Fast Read array  command code. */
#define READ_ARRAY_DUAL_IO               0xBB
/** Fast Read array  command code. */
#define READ_ARRAY_QUAD_IO               0xEB
/** Deep power-down command code. */
#define DEEP_PDOWN                       0xB9
/** Resume from deep power-down command code. */
#define RES_DEEP_PDOWN                   0xAB
/** Manufacturer/ Device ID command code. */
#define MANUFACTURER_DEVICE_ID           0x90
/** Read manufacturer and device ID command code. */
#define READ_JEDEC_ID                    0x9F

/** QSPI Flash Manufacturer JEDEC ID */
#define ATMEL_SPI_FLASH                  0x1F
#define ST_SPI_FLASH                     0x20
#define WINBOND_SPI_FLASH                0xEF
#define MACRONIX_SPI_FLASH               0xC2
#define SST_SPI_FLASH                    0xBF

enum block_size {
	SIZE_4K = 0,
	SIZE_8K,
	SIZE_16K,
	SIZE_32K,
	SIZE_64K,
	SIZE_128K,
	SIZE_256K,
	SIZE_512K,
	SIZE_1M,
	SIZE_2M
};

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
enum status_code s25fl1xx_initialize(Qspi *qspi, struct qspi_config_t *mode_config, uint32_t use_default_config);
uint32_t s25fl1xx_read_jedec_id(struct qspid_t *qspid);
void s25fl1xx_soft_reset(struct qspid_t *qspid);
uint8_t s25fl1xx_protect(struct qspid_t *qspid);
uint8_t s25fl1xx_unprotect(struct qspid_t *qspid);
uint8_t s25fl1xx_data_protect(struct qspid_t *qspid, bool dir, enum block_size protect_size);
uint8_t s25fl1xx_data_unprotect(struct qspid_t *qspid);
void s25fl1xx_set_quad_mode(struct qspid_t *qspid, uint8_t mode);
void s25fl1xx_enable_wrap(struct qspid_t *qspid, uint8_t byte_align);
void s25fl1xx_set_read_latency_control(struct qspid_t *qspid, uint8_t latency);
uint8_t s25fl1xx_erase_chip(struct qspid_t *qspid);
uint8_t s25fl1xx_erase_sector(struct qspid_t *qspid, uint32_t address);
uint8_t s25fl1xx_erase_64k_block(struct qspid_t *qspid, uint32_t address);
uint8_t s25fl1xx_write(struct qspid_t *qspid, uint32_t *pdata, uint32_t size, uint32_t address, uint8_t secure);
uint8_t s25fl1xx_read(struct qspid_t *qspid, uint32_t *data,  uint32_t size, uint32_t address);
uint8_t s25fl1xx_read_dual(struct qspid_t *qspid, uint32_t *data, uint32_t size, uint32_t address);
uint8_t s25fl1xx_read_quad(struct qspid_t *qspid, uint32_t *data, uint32_t size, uint32_t address);
uint8_t s25fl1xx_read_dual_io(struct qspid_t *qspid, uint32_t *data, uint32_t size, uint32_t address,
								   uint8_t cont_mode, uint8_t secure);
uint8_t s25fl1xx_read_quad_io(struct qspid_t *qspid, uint32_t *data, uint32_t size, uint32_t address,
								   uint8_t cont_mode, uint8_t secure);

#endif // #ifndef S25FL1_H

