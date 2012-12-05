/**
 
 * \file
 *
 * \brief SAM0+ SERCOM USART Driver
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
#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED

#include <sercom.h>

#ifdef USART_ASYNC
#include <sercom_interrupts.h>
#endif

#define USART_DEFAULT_TIMEOUT  0xFFFF

/**
 * \brief USART Callback enum
 *
 * Callbacks for the Asynchronous USART driver
 *
 */
/* TODO: Add support for RX started interrupt. */
#ifdef USART_ASYNC
enum usart_callback {
	/** Callback for buffer transmitted. */
	USART_CALLBACK_BUFFER_TRANSMITTED,
	/** Callback for buffer received. */
	USART_CALLBACK_BUFFER_RECEIVED,
	/** Callback for error. */
	USART_CALLBACK_ERROR,
#if !defined(__DOXYGEN__)
	/** Number of available callbacks. */
	USART_CALLBACK_N,
#endif
};
#endif

/**
 * \brief USART Dataorder enum
 *
 * The dataorder decides which of MSB or LSB
 * is shifted out first when data is transferred
 */
enum usart_dataorder {
	USART_DATAORDER_MSB = SERCOM_USART_DORD_bm,
	USART_DATAORDER_LSB = 0,
};

/**
 * \brief USART Sample mode enum
 *
 *
 */
enum usart_sample_mode {
	/* Sampling is synchronized with the internal clock */
	USART_SAMPLE_MODE_SYNC_MASTER = (SERCOM_USART_CSRC_bm | SERCOM_USART_CMODE_bm),
	/* Sampling is synchronized with the  external clock */
	USART_SAMPLE_MODE_SYNC_SLAVE = (SERCOM_USART_CMODE_bm),
	/* Sampling is asynchronous and USART is clocked by the internal clock */
	USART_SAMPLE_MODE_ASYNC_INTERNAL_CLOCK = (SERCOM_USART_CSRC_bm),
	/* Sampling is asynchronous and USART is clocked by an external clock.
	 * The clock source is applied to the XCK pin
	 */
	USART_SAMPLE_MODE_ASYNC_EXTERNAL_CLOCK = 0,
};

/**
 * \brief USART Parity enum
 *
 * Parity is enabled for error checking of the data transferred. If even parity
 * is selected, the parity bit will be set if number of ones being transferred
 * is odd. If odd parity is selected, the parity bit will be set if number of
 * ones being transferred is even.
 *
 */
enum usart_parity {
	USART_PARITY_ODD  = SERCOM_USART_PMODE_bm,
	USART_PARITY_EVEN = 0,
	USART_PARITY_NONE = 0xFF,
};

/**
 * \brief USART signal mux settings
 *
 * Set the functionality of the SERCOM pins.
 * TODO: As not all settings can be used in different modes of operation, proper
 * settings must be chosen according to the rest of the configuration.
 *
 */
//TODO: rename to mux_settings_a-h
enum usart_signal_mux_settings {
	/** See \ref mux_setting_a */
	USART_RX_0_TX_0_XCK_1 = (SERCOM_USART_RXPO_0_gc | SERCOM_USART_TXPO_0_gc),
	/** See \ref mux_setting_b */
	USART_RX_0_TX_2_XCK_3 = (SERCOM_USART_RXPO_0_gc | SERCOM_USART_TXPO_2_gc),
	/** See \ref mux_setting_c */
	USART_RX_1_TX_0_XCK_1 = (SERCOM_USART_RXPO_1_gc | SERCOM_USART_TXPO_0_gc),
	/** See \ref mux_setting_d */
	USART_RX_1_TX_2_XCK_3 = (SERCOM_USART_RXPO_1_gc | SERCOM_USART_TXPO_2_gc),
	/** See \ref mux_setting_e */
	USART_RX_2_TX_0_XCK_1 = (SERCOM_USART_RXPO_2_gc | SERCOM_USART_TXPO_0_gc),
	/** See \ref mux_setting_f */
	USART_RX_2_TX_2_XCK_3 = (SERCOM_USART_RXPO_2_gc | SERCOM_USART_TXPO_2_gc),
	/** See \ref mux_setting_g */
	USART_RX_3_TX_0_XCK_1 = (SERCOM_USART_RXPO_3_gc | SERCOM_USART_TXPO_0_gc),
	/** See \ref mux_setting_h */
	USART_RX_3_TX_2_XCK_3 = (SERCOM_USART_RXPO_3_gc | SERCOM_USART_TXPO_2_gc),
};

/**
 * \brief USART Stop Bits enum
 *
 * Number of stop bits for a frame.
 *
 */
enum usart_stopbits {
	USART_STOPBITS_1 = SERCOM_USART_SBMODE_1_bm,
	USART_STOPBITS_2 = SERCOM_USART_SBMODE_2_bm,
};

/**
 * \brief USART Character Size
 *
 * Number of bits for the character sent in a frame.
 *
 */
enum usart_char_size {
	USART_CHAR_SIZE_5BIT = SERCOM_USART_CHSIZE_5BIT_gc,
	USART_CHAR_SIZE_6BIT = SERCOM_USART_CHSIZE_6BIT_gc,
	USART_CHAR_SIZE_7BIT = SERCOM_USART_CHSIZE_7BIT_gc,
	USART_CHAR_SIZE_8BIT = SERCOM_USART_CHSIZE_8BIT_gc,
	USART_CHAR_SIZE_9BIT = SERCOM_USART_CHSIZE_9BIT_gc,
};

/**
 * \brief USART Status Flags
 *
 * Status flags for the USART module
 *
 */
enum usart_status_flag {
	USART_STATUS_FLAG_PERR        = SERCOM_USART_STATUS_PERR,
	USART_STATUS_FLAG_FERR        = SERCOM_USART_STATUS_FERR,
	USART_STATUS_FLAG_BUFOVF      = SERCOM_USART_STATUS_BUFOVF,
	USART_STATUS_FLAG_SYNCBUSY    = SERCOM_USART_STATUS_SYNCBUSY,
};

/**
 * \brief USART Interrupt Flags
 *
 * Interrupt flags for the USART module
 *
 */
enum usart_interrupt_flag {
	USART_INTERRUPT_FLAG_DREIF,
	USART_INTERRUPT_FLAG_TXCIF,
	USART_INTERRUPT_FLAG_RXCIF,
};

/**
 * \name USART configuration struct
 * \note Configuration options for USART
 */
struct usart_conf {
	/** USART bit order (MSB or LSB first) */
	enum usart_dataorder data_order;
	/** USART in asynchronous or synchronous mode */
	enum usart_sample_mode sample_mode;
	/** USART parity */
	enum usart_parity parity;
	/** Number of stop bits */
	enum usart_stopbits stopbits;
	/** USART character size */
	enum usart_char_size char_size;
	/** USART pin out */
	enum usart_signal_mux_settings mux_settings;
	/** USART baud rate */
	uint32_t baudrate;
	/** USART Clock Polarity
	 * If true, data changes on falling XCK edge and
	 * is sampled at rising edge
	 * If false, data changes on rising XCK edge and
	 * is sampled at falling edge
	 * */
	bool clock_polarity_inverted;
	/** External clock frequency in synchronous mode.
	 * Must be given if clock source (XCK) is set to external. */
	uint32_t ext_clock_freq;
	//TODO: clock_setup, config_struct?;
	enum gclk_generator generator_source;
	/*  */
	bool run_in_standby;
};

#ifdef USART_ASYNC
/* Prototype for the device instance */
struct usart_dev_inst;

/* Type of the callback functions */
typedef void (*usart_async_callback_t)(const struct usart_dev_inst *const dev_inst);
#endif

/**
 * \name USART device struct
 * \note Device information for USART
 */
struct usart_dev_inst {
#ifdef USART_ASYNC
	/** Mode for the SERCOM module */
	SERCOM_MODE_t sercom_mode;
#endif
	/** Pointer to the hardware instance */
	SERCOM_t *hw_dev;
	/** Character size of the data being transferred */
	enum usart_char_size char_size;
#ifdef USART_ASYNC
	/** Array to store callback function pointers in */
	usart_async_callback_t *callback[USART_CALLBACK_N];
	/** Buffer pointer to where the next received character will be put */
	uint8_t *rx_buffer_ptr;
	/** Buffer pointer to where the next character will be transmitted from */
	uint8_t *tx_buffer_ptr;
	/** Remaining characters to receive */
	volatile uint16_t remaining_rx_buffer_length;
	/** Remaining characters to transmit */
	volatile uint16_t remaining_tx_buffer_length;
	/** Bit mask for callbacks registered */
	uint8_t callback_reg_mask;
	/** Bit mask for callbacks enabled */
	uint8_t callback_enable_mask;
	/** Returns 'true' if asynchronous reception is ongoing */
	bool async_rx_ongoing;
	/** Returns 'false' if asynchronous transmission is ongoing */
	bool async_tx_ongoing;
	/** Holds the status of the last asynchronous operation */
	enum status_code status;
#endif
};

#if !defined (__DOXYGEN__)

/**
 * \internal Wait until synchronization is complete
 */
static inline void _usart_wait_for_sync(const struct usart_dev_inst
		*const dev_inst)
{
	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Wait until the synchronization is complete */
	while (usart_module->STATUS & SERCOM_USART_SYNCBUSY_bm) {
		/* Intentionally left empty */
	}
}

#endif

/**
 * \brief Initializes the device to predefined defaults
 *
 * Initialize the USART device to predefined defaults:
 * - 8-bit asynchronous USART
 * - no parity
 * - 1 stop bit
 * - 115200 baud
 *
 * Both the device and the config struct will be updated with the predefined
 * settings.
 *
 * \param[in] dev_inst Pointer to USART software instance struct
 *
 */
static inline void usart_get_config_defaults(struct usart_conf *const config)
{
	Assert(config);
	config->data_order = USART_DATAORDER_MSB;
	config->sample_mode = USART_SAMPLE_MODE_ASYNC_INTERNAL_CLOCK;
	config->parity = USART_PARITY_NONE;
	config->stopbits = USART_STOPBITS_1;
	config->char_size = USART_CHAR_SIZE_8BIT;
	config->baudrate = 9600;
	config->mux_settings = USART_RX_1_TX_2_XCK_3;
}

enum status_code usart_init(struct usart_dev_inst *const dev_inst,
		SERCOM_t *const hw_dev, const struct usart_conf *const config);

/**
 * \brief Enable the module
 *
 * Enables the USART module
 *
 * \param[in] dev_inst Pointer to USART software instance struct
 *
 */
static inline void usart_enable(const struct usart_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Enable USART module */
	usart_module->CTRLA |= SERCOM_USART_ENABLE_bm;
}

/**
 * /brief Disable module
 *
 * Disables the USART module
 *
 * \param[in] dev_inst Pointer to USART software instance struct
 */
static inline void usart_disable(const struct usart_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Disable USART module */
	usart_module->CTRLA &= ~SERCOM_USART_ENABLE_bm;
}

/**
 * \brief Resets the USART module
 *
 * Disables and resets the USART module.
 *
 * \param[in] dev_inst Pointer to the USART software instance struct
 *
 */
static inline void usart_reset(const struct usart_dev_inst *const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	usart_disable(dev_inst);

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Reset module */
	usart_module->CTRLA = SERCOM_USART_RESET_bm;
}

/**
 * \name Writing and reading
 * {@
 */
enum status_code usart_write(struct usart_dev_inst *const dev_inst,
		const uint16_t tx_data);

enum status_code usart_read(struct usart_dev_inst *const dev_inst,
		uint16_t *const rx_data);

enum status_code usart_write_buffer(struct usart_dev_inst *const dev_inst,
		const uint8_t *tx_data, uint16_t length);

enum status_code usart_read_buffer(struct usart_dev_inst *const dev_inst,
		const uint8_t *rx_data, uint16_t length);

/**
 * @}
 */

/**
 * \brief poll for data ready
 *
 * Returns non-zero value if data is received and ready
 *
 * The Receiver starts data reception when it detects a valid start bit.
 * When the first stop bit is received and a complete serial frame is present
 * in the Receive Shift Register, the contents of the Shift Register will be
 * moved into the receive buffer. The Receive Complete Interrupt Flag (RXCIF)
 * is set, and the optional interrupt is generated.
 *
 * \param[in] dev_inst Pointer to USART software instance struct
 *
 */
static inline bool usart_is_data_received(const struct usart_dev_inst
		*const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Variable for interrupt flag */
	uint8_t rxcif;

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Read out Transmit Complete Interrupt Flag */
	rxcif = usart_module->INTFLAGS & SERCOM_USART_RXCIF_bm;

#ifdef USART_ASYNC
	return rxcif && !(dev_inst->remaining_rx_buffer_length);
#endif
	return rxcif;
}

/**
 * \brief Poll if data has been sent
 *
 * Returns non-zero value if data has been completely transmitted.
 *
 * A data transmission is initiated by loading the transmit buffer (DATA) with
 * the data to be sent. The data in the transmit buffer is moved to the
 * Shift Register when the Shift Register is empty and ready to send a new
 * frame. The Shift Register is loaded if it is idle (no ongoing transmission)
 * or immediately after the last stop bit of the previous frame is transmitted.
 * When the Shift Register is loaded with data, it will transmit one complete
 * frame.
 * The Transmit Complete Interrupt Flag (TXCIF) is set and the optional
 * interrupt is generated when the entire frame in the Shift Register has been
 * shifted out and there is no new data present in the transmit buffer.
 * The Transmit Data Register (DATA) can only be written when the
 * Data Register Empty Flag (DREIF) is set, indicating that the register is
 * empty and ready for new data.
 *
 * \param[in] dev_inst Pointer to USART software instance struct
 *
 */
static inline bool usart_is_data_transmitted(const struct usart_dev_inst
		*const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Variable for interrupt flag */
	uint8_t txcif;

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Read out Transmit Complete Interrupt Flag */
	txcif = usart_module->INTFLAGS & SERCOM_USART_TXCIF_bm;

#ifdef USART_ASYNC
	return txcif && !(dev_inst->remaining_tx_buffer_length);
#endif
	return txcif;
}

/**
 * \brief Check if transmit buffer is empty
 *
 * Check if the data buffer is empty and ready for new data
 *
 * \param[in] dev_inst Pointer to USART software instance struct
 *
 */
static inline bool usart_is_data_buffer_empty(const struct usart_dev_inst
		*const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

#ifdef USART_ASYNC
	return dev_inst->remaining_tx_buffer_length &&
			(usart_module->INTFLAGS & SERCOM_USART_DREIF_bm);
#endif
	return usart_module->INTFLAGS & SERCOM_USART_DREIF_bm;
}

/**
 * \name Enabling/Disabling receiver and transmitter
 * {@
 */

/**
 * \brief Enable RX
 *
 * Enable receiver in module.
 *
 * \param[in] dev_inst Pointer to USART software instance struct
 *
 */
static inline void usart_enable_rx(const struct usart_dev_inst
		*const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Enable RX */
	usart_module->CTRLB |= SERCOM_USART_RXEN_bm;
}

/**
 * \brief Disable RX
 *
 * Disable receiver in module.
 *
 * \param[in] dev_inst Pointer to USART software instance struct
 *
 */
static inline void usart_disable_rx(const struct usart_dev_inst
		*const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Disable RX */
	usart_module->CTRLB &= ~SERCOM_USART_RXEN_bm;
}

/**
 * \brief Enable TX
 *
 * Enable transmitter in module.
 *
 * \param[in] dev_inst Pointer to USART software instance struct
 *
 */
static inline void usart_enable_tx(const struct usart_dev_inst
		*const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Enable TX */
	usart_module->CTRLB |= SERCOM_USART_TXEN_bm;
}

/**
 * \brief Disable TX
 *
 * Disable transmitter in module.
 *
 * \param[in] dev_inst Pointer to USART software instance struct
 *
 */
static inline void usart_disable_tx(const struct usart_dev_inst
		*const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Disable TX */
	usart_module->CTRLB &= ~SERCOM_USART_TXEN_bm;
}

/*
 * @}
 */

/**
 * \name Interrupt and Status flags
 * {@
 */

static inline bool usart_is_status_flag_set(
		const struct usart_dev_inst *const dev_inst,
		enum usart_status_flag status_flag)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	return (usart_module->STATUS & (1 << status_flag));
}

static inline void usart_clear_status_flag(
		const struct usart_dev_inst *const dev_inst,
		enum usart_status_flag status_flag)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Clear requested status flag by writing 1 to it */
	usart_module->STATUS |= (1 << status_flag);
}

static inline bool usart_is_interrupt_flag_set(
		const struct usart_dev_inst *const dev_inst,
		enum usart_interrupt_flag interrupt_flag)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	return (usart_module->INTFLAGS & (1 << interrupt_flag));
}

static inline void usart_clear_interrupt_flag(
		const struct usart_dev_inst *const dev_inst,
		enum usart_interrupt_flag interrupt_flag)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Clear requested status flag by writing a one to it */
	usart_module->INTFLAGS |= (1 << interrupt_flag);
}

/**
 * @}
 */

#endif /* USART_H_INCLUDED */
