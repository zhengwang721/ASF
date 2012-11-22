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

#include <stdint.h>
#include <sercom.h>
#include <stdbool.h>

//TODO: Add support for RX started interrupt.
#ifdef USART_ASYNC
enum usart_callback_type {
	USART_CALLBACK_TYPE_BUFFER_TRANSMITTED,
	USART_CALLBACK_TYPE_BUFFER_RECEIVED,
	USART_CALLBACK_N,
};
#endif

enum usart_dataorder {
	USART_DATAORDER_MSB = SERCOM_USART_DORD_bm,
	USART_DATAORDER_LSB = 0,
};

enum usart_com_mode {
	USART_COM_MODE_ASYNC_CLOCK_INTERNAL,
	USART_COM_MODE_ASYNC_CLOCK_EXTERNAL,
	USART_COM_MODE_SYNC_CLOCK_INTERNAL,
	USART_COM_MODE_SYNC_CLOCK_EXTERNAL,
};

//TODO: remove as there will be no half duplex for now..?
enum usart_frame_format {
	USART_FRAME_FORMAT_FULL_DUPLEX   = SERCOM_USART_FORM_1_gc,
	USART_FRAME_FORMAT_HALF_DUPLEX   = SERCOM_USART_FORM_5_gc,
};

enum usart_parity {
	USART_PARITY_ODD  = SERCOM_USART_PMODE_bm,
	USART_PARITY_EVEN = 0,
	USART_PARITY_NONE = 0xFF,
};

enum usart_mux_settings {
	USART_RX_0_TX_0_XCK_1 = (SERCOM_USART_RXPO_0_gc | SERCOM_USART_TXPO_0_gc),
	USART_RX_0_TX_2_XCK_3 = (SERCOM_USART_RXPO_0_gc | SERCOM_USART_TXPO_2_gc),
	USART_RX_1_TX_0_XCK_1 = (SERCOM_USART_RXPO_1_gc | SERCOM_USART_TXPO_0_gc),
	USART_RX_1_TX_2_XCK_3 = (SERCOM_USART_RXPO_1_gc | SERCOM_USART_TXPO_2_gc),
	USART_RX_2_TX_0_XCK_1 = (SERCOM_USART_RXPO_2_gc | SERCOM_USART_TXPO_0_gc),
	USART_RX_2_TX_2_XCK_3 = (SERCOM_USART_RXPO_2_gc | SERCOM_USART_TXPO_2_gc),
	USART_RX_3_TX_0_XCK_1 = (SERCOM_USART_RXPO_3_gc | SERCOM_USART_TXPO_0_gc),
	USART_RX_3_TX_2_XCK_3 = (SERCOM_USART_RXPO_3_gc | SERCOM_USART_TXPO_2_gc),
};

enum usart_stopbits {
	USART_STOPBITS_1 = 0,
	USART_STOPBITS_2 = SERCOM_USART_BMODE_bm,
};

enum usart_char_size {
	USART_CHAR_SIZE_5BIT = SERCOM_USART_CHSIZE_5BIT_gc,
	USART_CHAR_SIZE_6BIT = SERCOM_USART_CHSIZE_6BIT_gc,
	USART_CHAR_SIZE_7BIT = SERCOM_USART_CHSIZE_7BIT_gc,
	USART_CHAR_SIZE_8BIT = SERCOM_USART_CHSIZE_8BIT_gc,
	USART_CHAR_SIZE_9BIT = SERCOM_USART_CHSIZE_9BIT_gc,
};

/**
 * \name USART configuration struct
 * \note Configuration options for USART
 */
struct usart_config {
	/** USART bit order (MSB or LSB first) */
	enum usart_dataorder data_order;
	/** USART in asynchronous or synchronous mode */
	//TODO: fix
	enum usart_com_mode com_mode;
	/** Address match */
	bool address_match;
	/*! USART parity */
	enum usart_parity parity;
	/*! Number of stop bits */
	enum usart_stopbits stopbits;
	/*! USART character size */
	enum usart_char_size char_size;
	/*! USART pin out */
	enum usart_mux_settings mux_settings;
	/** USART frame format */
	enum usart_frame_format frame_format;
	/*! USART baud rate */
	uint32_t baudrate;
	/*! External clock frequency in synchronous mode */
	/* TODO: this should refer to the GCLK somehow, transparent to user */
	uint32_t ext_clock_freq;
	//TODO: clock_setup, config_struct?;
};

/* Prototype for the device instance */
struct usart_dev_inst;

typedef void (*usart_async_callback_t)(const struct usart_dev_inst *const dev_inst);

/**
 * \name USART device struct
 * \note Device information for USART
 */
struct usart_dev_inst {
#ifdef USART_ASYNC
	SERCOM_MODE_t sercom_mode;
#endif
	SERCOM_t *hw_dev;
	enum usart_char_size char_size;
#ifdef USART_ASYNC
	usart_async_callback_t *callback[USART_CALLBACK_N];
	uint8_t *rx_buffer_ptr;
	uint8_t *tx_buffer_ptr;
	volatile uint16_t remaining_rx_buffer_length;
	volatile uint16_t remaining_tx_buffer_length;
	uint8_t callback_reg_mask;
	uint8_t callback_enable_mask;
#endif
};

/**
 * \brief Initializes the device
 *
 * Initializes the USART device based on the setting specified in the
 * configuration struct. This will leave the device in an enabled state
 * after initialization.
 *
 * \param dev_inst Pointer to USART device
 * \param config Pointer to configuration struct
 */
enum status_code usart_init(struct usart_dev_inst *dev_inst, SERCOM_t *hw_dev,
		struct usart_config *config);

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
 * \param config Pointer to the configuration struct
 */
static inline void usart_get_config_defaults(struct usart_config *const config)
{

	//TODO: look through defaults
	Assert(config);
	config->data_order = USART_DATAORDER_MSB;
	config->com_mode = USART_COM_MODE_ASYNC_CLOCK_INTERNAL;
	config->parity = USART_PARITY_NONE;
	config->stopbits = USART_STOPBITS_1;
	config->char_size = USART_CHAR_SIZE_8BIT;
	config->frame_format = USART_FRAME_FORMAT_FULL_DUPLEX;
	config->baudrate = 9600;
	config->mux_settings = USART_RX_1_TX_2_XCK_3;
}

/**
 * \brief Enable the module
 *
 * Enables the USART module
 *
 * \param dev_inst Pointer to USART device
 */
static inline void usart_enable(const struct usart_dev_inst *const dev_inst)
{
	Assert(dev_inst);
	dev_inst->hw_dev->USART.CTRLA |= SERCOM_USART_ENABLE_bm;
	/* Wait until the USART module has been enabled */
	while(!(dev_inst->hw_dev->USART.CTRLA & SERCOM_USART_ENABLE_bm));
}

/**
 * /brief Disable module
 *
 * Disables the USART module
 *
 * \param dev_inst Pointer to USART device
 */
static inline void usart_disable(const struct usart_dev_inst *const dev_inst)
{
	Assert(dev_inst);
	dev_inst->hw_dev->USART.CTRLA &= ~SERCOM_USART_ENABLE_bm;
	/* Wait until the USART module has been disabled */
	while((dev_inst->hw_dev->USART.CTRLA & SERCOM_USART_ENABLE_bm));
}


/**
 * \name Writing and reading
 * {@
 */
enum status_code usart_write(struct usart_dev_inst *const dev_inst,
		const uint16_t tx_data);

enum status_code usart_read(struct usart_dev_inst *const dev_inst,
		uint16_t *const rx_data);

enum status_code usart_write_buffer(const struct usart_dev_inst *const dev_inst,
		const uint16_t *tx_data, uint16_t length);

enum status_code usart_read_buffer(const struct usart_dev_inst *const dev_inst,
		const uint16_t *rx_data, uint16_t length);

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
 * \param dev_inst Pointer to USART device
 */
static inline bool usart_is_data_received(struct usart_dev_inst *dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	uint8_t rxcif;

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

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
 * \param dev_inst Pointer to USART device
 */
static inline bool usart_is_data_transmitted(struct usart_dev_inst *dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);

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
 * \param dev_inst pointer to USART device
 */
static inline bool usart_is_data_buffer_empty(struct usart_dev_inst *dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

#ifdef USART_ASYNC
	return dev_inst->remaining_tx_buffer_length &&
			(usart_module->INTFLAGS & SERCOM_USART_DREIF_bm);
#endif
	return usart_module->INTFLAGS & SERCOM_USART_DREIF_bm;
}

/**
 * \name Enabling/Disabling RX and TX
 * {@
 */

/**
 * \brief Disable RX
 *
 * Disable rx in module
 *
 * \param dev_inst Pointer to USART device
 */
static inline void usart_disable_rx(struct usart_dev_inst *dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	dev_inst->hw_dev->USART.CTRLB &= ~SERCOM_USART_RXEN_bm;
}

/**
 * \brief Enable RX
 *
 * Enable rx in module
 *
 * \param dev_inst Pointer to USART device
 */
static inline void usart_enable_rx(struct usart_dev_inst *dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	dev_inst->hw_dev->USART.CTRLB |= SERCOM_USART_RXEN_bm;
}

/**
 * \brief Disable TX
 *
 * Disable tx in module
 *
 * \param dev_inst_inst Pointer to USART device
 */
static inline void usart_disable_tx(struct usart_dev_inst *dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	dev_inst->hw_dev->USART.CTRLB &= ~SERCOM_USART_TXEN_bm;
}

/**
 * \brief Enable TX
 *
 * Enable tx in module
 *
 * \param dev_inst Pointer to USART device
 */
static inline void usart_enable_tx(struct usart_dev_inst *dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	dev_inst->hw_dev->USART.CTRLB |= SERCOM_USART_TXEN_bm;
}

/*
 * @}
 */

#endif /* USART_H_INCLUDED */
