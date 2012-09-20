/**
 * \file
 *
 * \brief BPM driver.
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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

#ifndef BPM_H_INCLUDED
#define BPM_H_INCLUDED

#include "compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup group_sam_drivers_bpm BPM - Backup Power Manager
 *
 * Driver for the BPM (Backup Power Manager).
 * This driver provides access to the main features of the BPM controller.
 * It provides functions for different power mode management.
 *
 * @{
 */

/** BPM unlock macro */
#define BPM_UNLOCK(reg) \
	do { \
		BPM->BPM_UNLOCK = BPM_UNLOCK_KEY(0xAAu)                          \
			| BPM_UNLOCK_ADDR((uint32_t)&BPM->BPM_##reg - (uint32_t)BPM);\
	} while (0)

/** \name Sleep mode definitions */
/* @{ */
#define BPM_SM_ACTIVE    0    /**< Active mode */
#define BPM_SM_SLEEP_0   1    /**< Sleep mode 0 */
#define BPM_SM_SLEEP_1   2    /**< Sleep mode 1 */
#define BPM_SM_SLEEP_2   3    /**< Sleep mode 2 */
#define BPM_SM_SLEEP_3   4    /**< Sleep mode 3 */
#define BPM_SM_WAIT      5    /**< Wait mode */
#define BPM_SM_RET       6    /**< Retention mode */
#define BPM_SM_BACKUP    7    /**< Backup mode */
/* @} */

/** \name Power scaling change mode */
/* @{ */
/** Power scaling change mode: halting the CPU execution */
#define BPM_PSCM_CPU_HALT           0
/** Power scaling change mode: CPU execution not halted */
#define BPM_PSCM_CPU_NOT_HALT       1
/* @} */

/** \name Power scaling mode value */
/* @{ */
/** Power scaling mode 0 */
#define BPM_PS_0    0
/** Power scaling mode 1 */
#define BPM_PS_1    1
/* @} */

/** \name CLK32 32Khz-1Khz clock source selection */
/* @{ */
/** OSC32K : Low frequency crystal oscillator */
#define BPM_CLK32_SOURCE_OSC32K  0
/** RC32K : Internal Low frequency RC oscillator */
#define BPM_CLK32_SOURCE_RC32K   1
/* @} */

/** \name Backup wake up sources */
/* @{ */
/** EIC wake up */
#define BPM_BKUP_WAKEUP_SRC_EIC       (1UL << BPM_BKUPWEN_EIC)
/** AST wake up */
#define BPM_BKUP_WAKEUP_SRC_AST       (1UL << BPM_BKUPWEN_AST)
/** WDT wake up */
#define BPM_BKUP_WAKEUP_SRC_WDT       (1UL << BPM_BKUPWEN_WDT)
/** BOD33 wake up */
#define BPM_BKUP_WAKEUP_SRC_BOD33     (1UL << BPM_BKUPWEN_BOD33)
/** BOD18 wake up */
#define BPM_BKUP_WAKEUP_SRC_BOD18     (1UL << BPM_BKUPWEN_BOD18)
/** PICOUART wake up */
#define BPM_BKUP_WAKEUP_SRC_PICOUART  (1UL << BPM_BKUPWEN_PICOUART)
/* @} */

/** \name Backup pin muxing */
/* @{ */
#define BPM_BKUP_PIN_PB01_EIC0    BPM_BKUPPMUX_BKUPPMUX(0)
#define BPM_BKUP_PIN_PA06_EIC1    BPM_BKUPPMUX_BKUPPMUX(1)
#define BPM_BKUP_PIN_PA04_EIC2    BPM_BKUPPMUX_BKUPPMUX(2)
#define BPM_BKUP_PIN_PA05_EIC3    BPM_BKUPPMUX_BKUPPMUX(3)
#define BPM_BKUP_PIN_PA07_EIC4    BPM_BKUPPMUX_BKUPPMUX(4)
#define BPM_BKUP_PIN_PC03_EIC5    BPM_BKUPPMUX_BKUPPMUX(5)
#define BPM_BKUP_PIN_PC04_EIC6    BPM_BKUPPMUX_BKUPPMUX(6)
#define BPM_BKUP_PIN_PC05_EIC7    BPM_BKUPPMUX_BKUPPMUX(7)
#define BPM_BKUP_PIN_PC06_EIC8    BPM_BKUPPMUX_BKUPPMUX(8)
/* @} */

/**
 * \name Power management
 */
/* @{ */

/**
 * \brief Configure power scaling mode.
 *
 * \param bpm  Base address of the BPM instance.
 * \param ps_value  Power scaling value, see \ref Power scaling mode value.
 * \param pscm      Power scaling change mode, see \ref Power scaling change mode.
 */
void bpm_configure_power_scaling(Bpm *bpm, uint32_t ps_value, uint32_t pscm);

/**
 * \brief Enable fast wakeup for analog modules.
 *
 * \param bpm  Base address of the BPM instance.
 */
void bpm_enable_fast_wakeup(Bpm *bpm);

/**
 * \brief Disable fast wakeup for analog modules.
 *
 * \param bpm  Base address of the BPM instance.
 */
void bpm_disable_fast_wakeup(Bpm *bpm);

/**
 * \brief Set clock source for 32KHz clock.
 *
 * \param bpm  Base address of the BPM instance.
 * \param source  Clock source, see \ref CLK32 32Khz-1Khz clock source selection.
 */
void bpm_set_clk32_source(Bpm *bpm, uint32_t source);

/**
 * \brief Get wakeup cause from backup mode.
 *
 * \param bpm  Base address of the BPM instance.
 */
uint32_t bpm_get_backup_wakeup_cause(Bpm *bpm);

/**
 * \brief Enable wakeup source.
 *
 * \param bpm  Base address of the BPM instance.
 * \param sources  Wakeup source mask, see \ref Backup wake up sources.
 */
void bpm_enable_wakeup_source(Bpm *bpm, uint32_t sources);

/**
 * \brief Disable wakeup source.
 *
 * \param bpm  Base address of the BPM instance.
 * \param sources  Wakeup source mask, see \ref Backup wake up sources.
 */
void bpm_disable_wakeup_source(Bpm *bpm, uint32_t sources);

/**
 * \brief Enable backup pin for wakeup.
 *
 * \param bpm  Base address of the BPM instance.
 * \param backup_pins  Backup pin mask, see \ref Backup pin muxing.
 */
void bpm_enable_backup_pin(Bpm *bpm, uint32_t backup_pins);

/**
 * \brief Disable backup pin for wakeup.
 *
 * \param bpm  Base address of the BPM instance.
 * \param backup_pins  Backup pin mask, see \ref Backup pin muxing.
 */
void bpm_disable_backup_pin(Bpm *bpm, uint32_t backup_pins);

/**
 * \brief Enable IO retention for backup mode.
 *
 * \param bpm  Base address of the BPM instance.
 */
void bpm_enable_io_retention(Bpm *bpm);

/**
 * \brief Disable IO retention for backup mode.
 *
 * \param bpm  Base address of the BPM instance.
 */
void bpm_disable_io_retention(Bpm *bpm);
/* @} */

/**
 * \name Interrupt and status management
 */
/* @{ */

/**
 * \brief Enable interrupt with given sources mask.
 *
 * \param bpm  Base address of the BPM instance.
 * \param sources BPM interrupt source mask.
 */
void bpm_enable_interrupt(Bpm *bpm, uint32_t sources);

/**
 * \brief Disable interrupt with given sources mask.
 *
 * \param bpm  Base address of the BPM instance.
 * \param sources BPM interrupt source mask.
 */
void bpm_disable_interrupt(Bpm *bpm, uint32_t sources);

/**
 * \brief Get BPM interrupt mask.
 *
 * \param bpm  Base address of the BPM instance.
 *
 * \return BPM interrupt mask.
 */
uint32_t bpm_get_interrupt_mask(Bpm *bpm);

/**
 * \brief Get BPM interrupt status.
 *
 * \param bpm  Base address of the BPM instance.
 *
 * \return BPM interrupt status.
 */
uint32_t bpm_get_interrupt_status(Bpm *bpm);

/**
 * \brief Clear BPM interrupt.
 *
 * \param bpm  Base address of the BPM instance.
 */
void bpm_clear_interrupt(Bpm *bpm, uint32_t sources);

/**
 * \brief Get BPM status.
 *
 * \param bpm  Base address of the BPM instance.
 *
 * \return BPM status.
 */
uint32_t bpm_get_status(Bpm *bpm);

/**
 * \brief Get version of BPM module.
 *
 * \param bpm  Base address of the BPM instance.
 *
 * \return Version of BPM module.
 */
uint32_t bpm_get_version(Bpm *bpm);
/* @} */

/* @} */
#ifdef __cplusplus
}
#endif

#endif /* BPM_H_INCLUDED */
