/**
 * \file tal_irq_handler.c
 *
 * \brief This file handles the interrupts generated by the transceiver.
 *
 * Copyright (c) 2013-2014 Atmel Corporation. All rights reserved.
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

/*
 * Copyright (c) 2013-2014, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "stack_config.h"
#include "bmm.h"
#include "qmm.h"
#include "tal_irq_handler.h"
#include "tal_rx.h"
#include "atmega256rfr2.h"
#include "tal_internal.h"
#include "tal_constants.h"
#include "tal_tx.h"
#include "mac_build_config.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/** Conversion factor: us per symbol for 2,4 GHz. */
#define PAL_US_PER_SYMBOLS              (16)

#if defined(__DOXYGEN__)

/**
 * \brief Reads the 32-bit timer register in the required order of bytes
 *
 * \param hh hh octet of 32-bit register
 * \param hl hl octet of 32-bit register
 * \param lh lh octet of 32-bit register
 * \param ll ll octet of 32-bit register
 *
 * \returns uint32_t Value of timer register
 */
static inline uint32_t sc_read32(volatile uint8_t *hh,
		volatile uint8_t *hl,
		volatile uint8_t *lh,
		volatile uint8_t *ll);

#else
static inline uint32_t sc_read32(volatile uint8_t *hh,
		volatile uint8_t *hl,
		volatile uint8_t *lh,
		volatile uint8_t *ll)
{
	union {
		uint8_t a[4];
		uint32_t rv;
	}
	x;

	x.a[0] = *ll;
	x.a[1] = *lh;
	x.a[2] = *hl;
	x.a[3] = *hh;

	return x.rv;
}

#endif /* __DOXYGEN__ */

/** String concatenation by preprocessor used to create proper register names.
 **/
#define CONCAT(a, b) a ## b

/** Creates proper subregister names and reads the corresponding values. */
#define SC_READ32(reg)                  sc_read32(&CONCAT(reg, HH), \
		&CONCAT(reg, HL), \
		&CONCAT(reg, LH), \
		&CONCAT(reg, LL))

/** Creates proper subregister names and writes the corresponding values. */
#define SC_WRITE32(reg, val) \
	do { \
		union { uint8_t a[4]; uint32_t v; } \
		x; \
		x.v = val; \
		CONCAT(reg, HH) = x.a[3]; \
		CONCAT(reg, HL) = x.a[2]; \
		CONCAT(reg, LH) = x.a[1]; \
		CONCAT(reg, LL) = x.a[0]; \
	} \
	while (0)

/* === Globals ============================================================= */

/*
 * Function pointers to store the callback function of
 * the transceiver interrupt
 */
/** Function pointer to store callback for transceiver TX_END interrupt. */
static irq_handler_t irq_hdl_trx_tx_end;
/** Function pointer to store callback for transceiver RX_END interrupt. */
static irq_handler_t irq_hdl_trx_rx_end;
/** Function pointer to store callback for transceiver CCA_ED interrupt. */
static irq_handler_t irq_hdl_trx_cca_ed;
/** Function pointer to store callback for transceiver AWAKE interrupt. */
static irq_handler_t irq_hdl_trx_awake;

#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) || (defined __DOXYGEN__)
/** Function pointer to store callback for transceiver timestamp (RX_START)
 *interrupt. */
static irq_handler_t irq_hdl_trx_tstamp;
#endif

#if (defined ENABLE_ALL_TRX_IRQS) || (defined __DOXYGEN__)
/** Function pointer to store callback for transceiver AMI interrupt. */
static irq_handler_t irq_hdl_trx_ami;
/** Function pointer to store callback for transceiver BATMON interrupt. */
static irq_handler_t irq_hdl_trx_batmon;
/** Function pointer to store callback for transceiver PLL_LOCK interrupt. */
static irq_handler_t irq_hdl_trx_pll_lock;
/** Function pointer to store callback for transceiver PLL_UNLOCK interrupt. */
static irq_handler_t irq_hdl_trx_pll_unlock;
/** Function pointer to store callback for transceiver AES_READY interrupt. */
static irq_handler_t irq_hdl_trx_aes_ready;
/** Function pointer to store callback for transceiver TX_START interrupt. */
static irq_handler_t irq_hdl_trx_tx_start;
/** Function pointer to store callback for transceiver AMI0 interrupt. */
static irq_handler_t irq_hdl_trx_ami0;
/** Function pointer to store callback for transceiver AMI1 interrupt. */
static irq_handler_t irq_hdl_trx_ami1;
/** Function pointer to store callback for transceiver AMI2 interrupt. */
static irq_handler_t irq_hdl_trx_ami2;
/** Function pointer to store callback for transceiver AMI3 interrupt. */
static irq_handler_t irq_hdl_trx_ami3;
#endif  /* ENABLE_ALL_TRX_IRQS */

#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) || (defined __DOXYGEN__)
/** Timestamp of the last frame transmission (including auto-ACK) */
uint32_t pal_tx_timestamp;
#endif

/* === Prototypes ========================================================== */

/* === Implementation ====================================================== */

/*
 * \brief Initializes the transceiver TX END interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver TX END interrupt
 *
 * \param trx_irq_cb Callback function for the transceiver TX END interrupt
 */
void pal_trx_irq_init_tx_end(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_tx_end = (irq_handler_t)trx_irq_cb;
}

/*
 * \brief Initializes the transceiver RX END interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver RX END interrupt
 *
 * \param trx_irq_cb Callback function for the transceiver RX END interrupt
 */
void pal_trx_irq_init_rx_end(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_rx_end = (irq_handler_t)trx_irq_cb;
}

/*
 * \brief Initializes the transceiver CCA ED END interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver CCA ED END interrupt
 *
 * \param trx_irq_cb Callback function for the transceiver CCA ED END interrupt
 */
void pal_trx_irq_init_cca_ed(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_cca_ed = (irq_handler_t)trx_irq_cb;
}

#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) || defined(__DOXYGEN__)

/*
 * \brief Initializes the transceiver timestamp interrupt (RX START interrupt)
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver timestamp interrupt (RX START
 *interrupt)
 *
 * \param trx_irq_cb Callback function for the transceiver
 * timestamp interrupt (RX START interrupt)
 */
void pal_trx_irq_init_tstamp(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_tstamp = (irq_handler_t)trx_irq_cb;
}

#endif  /* #if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) */

/*
 * \brief Initializes the transceiver AWAKE interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver AWAKE interrupt
 *
 * \param trx_irq_cb Callback function for the transceiver AWAKE interrupt
 */
void pal_trx_irq_init_awake(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_awake = (irq_handler_t)trx_irq_cb;
}

#if defined(ENABLE_ALL_TRX_IRQS) || defined(__DOXYGEN__)

/*
 * \brief Initializes the transceiver AMI interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver AMI interrupt
 *
 * \param trx_irq_cb Callback function for the transceiver AMI interrupt
 */
void pal_trx_irq_init_ami(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_ami = (irq_handler_t)trx_irq_cb;
}

/*
 * \brief Initializes the transceiver BATMON interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver BATMON interrupt
 *
 * \param trx_irq_cb Callback function for the transceiver BATMON interrupt
 */
void pal_trx_irq_init_batmon(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_batmon = (irq_handler_t)trx_irq_cb;
}

/*
 * \brief Initializes the transceiver PLL_LOCK interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver PLL_LOCK interrupt
 *
 * \param trx_irq_cb Callback function for the transceiver PLL_LOCK interrupt
 */
void pal_trx_irq_init_pll_lock(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_pll_lock = (irq_handler_t)trx_irq_cb;
}

/*
 * \brief Initializes the transceiver PLL_UNLOCK interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver PLL_UNLOCK interrupt
 *
 * \param trx_irq_cb Callback function for the transceiver PLL_UNLOCK interrupt
 */
void pal_trx_irq_init_pll_unlock(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_pll_unlock = (irq_handler_t)trx_irq_cb;
}

/*
 * \brief Initializes the transceiver AES_READY interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver AES_READY interrupt
 *
 * \param trx_irq_cb Callback function for the transceiver AES_READY interrupt
 */
void pal_trx_irq_init_aes_ready(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_aes_ready = (irq_handler_t)trx_irq_cb;
}

/*
 * \brief Initializes the transceiver TX_START interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver TX_START interrupt
 *
 * \param trx_irq_cb Callback function for the transceiver TX_START interrupt
 */
void pal_trx_irq_init_tx_start(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_tx_start = (irq_handler_t)trx_irq_cb;
}

/*
 * \brief Initializes the transceiver MAF AMI0 interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver MAF AMI0 interrupt
 *
 * \param trx_irq_cb Callback function for the transceiver MAF AMI0 interrupt
 */
void pal_trx_irq_init_maf_0_ami(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_ami0 = (irq_handler_t)trx_irq_cb;
}

/*
 * \brief Initializes the transceiver MAF AMI1 interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver MAF AMI1 interrupt
 *
 * \param trx_irq_cb Callback function for the transceiver MAF AMI1 interrupt
 */
void pal_trx_irq_init_maf_1_ami(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_ami1 = (irq_handler_t)trx_irq_cb;
}

/*
 * \brief Initializes the transceiver MAF AMI2 interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver MAF AMI2 interrupt
 *
 * \param trx_irq_cb Callback function for the transceiver MAF AMI2 interrupt
 */
void pal_trx_irq_init_maf_2_ami(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_ami2 = (irq_handler_t)trx_irq_cb;
}

/*
 * \brief Initializes the transceiver MAF AMI3 interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver MAF AMI3 interrupt
 *
 * \param trx_irq_cb Callback function for the transceiver MAF AMI3 interrupt
 */
void pal_trx_irq_init_maf_3_ami(FUNC_PTR trx_irq_cb)
{
	irq_hdl_trx_ami3 = (irq_handler_t)trx_irq_cb;
}

#endif  /* ENABLE_ALL_TRX_IRQS */

#ifdef PAL_XTD_IRQ_API

/*
 * \brief Returns the current callback function for the transceiver RX END
 *interrupt
 *
 * \return Current callback function for the transceiver RX END interrupt
 */
FUNC_PTR pal_trx_irq_get_hdlr_rx_end(void)
{
	return irq_hdl_trx_rx_end;
}

/*
 * \brief Returns the current callback function for the transceiver TX END
 *interrupt
 *
 * \return Current callback function for the transceiver TX END interrupt
 */
FUNC_PTR pal_trx_irq_get_hdlr_tx_end(void)
{
	return irq_hdl_trx_tx_end;
}

#endif  /* PAL_XTD_IRQ_API */

/**
 * \brief ISR for transceiver's transmit end interrupt
 */
ISR(TRX24_TX_END_vect)
{
	irq_hdl_trx_tx_end();
}

/**
 * \brief ISR for transceiver's receive end interrupt
 */
ISR(TRX24_RX_END_vect)
{
	irq_hdl_trx_rx_end();
}

/**
 * \brief ISR for transceiver's CCA/ED measurement done interrupt
 */
ISR(TRX24_CCA_ED_DONE_vect)
{
	irq_hdl_trx_cca_ed();
}

/**
 * \brief ISR for transceiver's Awake interrupt
 */
ISR(TRX24_AWAKE_vect)
{
	irq_hdl_trx_awake();
}

/**
 * \brief ISR for transceiver's rx start interrupt
 *
 * By the time the SFD is detected, the hardware timestamps the
 * current frame in the SCTSR register.
 */
ISR(TRX24_RX_START_vect)
{
#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)
	irq_hdl_trx_tstamp();
#endif  /* #if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) */
}

/**
 * \brief ISR for transceiver's PLL lock interrupt
 */
ISR(TRX24_PLL_LOCK_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
	irq_hdl_trx_pll_lock();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * \brief ISR for transceiver's PLL unlock interrupt
 */
ISR(TRX24_PLL_UNLOCK_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
	irq_hdl_trx_pll_unlock();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * \brief ISR for transceiver's address match interrupt
 */
ISR(TRX24_XAH_AMI_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
	irq_hdl_trx_ami();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * \brief ISR for transceiver's battery low interrupt
 */
ISR(BAT_LOW_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
	irq_hdl_trx_batmon();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * \brief ISR for transceiver's AES interrupt
 */
ISR(AES_READY_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
	irq_hdl_trx_aes_ready();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * \brief ISR for transceiver's TX_START interrupt
 */
ISR(TRX24_TX_START_vect)
{
#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)
	pal_tx_timestamp = PAL_US_PER_SYMBOLS * SC_READ32(SCCNT);
#endif
#ifdef ENABLE_ALL_TRX_IRQS
	irq_hdl_trx_tx_start();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * \brief ISR for transceiver's AMI0 interrupt
 */
ISR(TRX24_AMI0_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
	irq_hdl_trx_ami0();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * \brief ISR for transceiver's AMI1 interrupt
 */
ISR(TRX24_AMI1_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
	irq_hdl_trx_ami1();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * \brief ISR for transceiver's AMI2 interrupt
 */
ISR(TRX24_AMI2_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
	irq_hdl_trx_ami2();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * \brief ISR for transceiver's AMI3 interrupt
 */
ISR(TRX24_AMI3_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
	irq_hdl_trx_ami3();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/*
 * \brief Transceiver interrupt handler
 *
 * This function handles the transceiver generated interrupts for RX end.
 */
void trx_rx_end_handler_cb(void)
{
	ENTER_CRITICAL_REGION();

	/* Handle rx interrupt. */
	handle_received_frame_irq(); /* see tal_rx.c */

	LEAVE_CRITICAL_REGION();
} /* trx_rx_end_handler_cb() */

/*
 * \brief Transceiver interrupt handler
 *
 * This function handles the transceiver generated interrupts for TX end.
 */
void trx_tx_end_handler_cb(void)
{
	ENTER_CRITICAL_REGION();

	/* Get the result and push it to the queue. */
	handle_tx_end_irq();        /* see tal_tx.c */

	LEAVE_CRITICAL_REGION();
} /* trx_tx_end_handler_cb() */

#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)

/*
 * \brief Timestamp interrupt handler
 *
 * This function handles the interrupts handling the timestamp.
 * The timestamping is only required for beaconing networks
 * or if timestamping is explicitly enabled.
 */
void trx_irq_timestamp_handler_cb(void)
{
	/* The timestamping is only required for beaconing networks
	 * or if timestamping is explicitly enabled.
	 */
	pal_trx_read_timestamp(&tal_rx_timestamp);
}

#endif /* (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) */

/*
 * \brief Transceiver interrupt handler for awake end IRQ
 *
 * This function handles the transceiver awake end interrupt.
 */
void trx_awake_handler_cb(void)
{
	/* Set the wake-up flag. */
	tal_awake_end_flag = true;
}

/* EOF */
