/**
 * \file
 *
 * \brief SAMD20 SERCOM USART Driver
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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
#include "usart.h"
#include <pinmux.h>
#ifdef USART_ASYNC
#  include "usart_async.h"
#endif

/**
 * \internal Set Configuration of the USART module
 *
 */
enum status_code _usart_set_config(struct usart_dev_inst *const dev_inst,
		const struct usart_conf const *config)
{
	/* Temporary registers. */

	uint16_t baud_val = 0;
	uint32_t usart_freq;
	enum status_code status_code = STATUS_OK;

	/* Temporary registers. */
	uint32_t ctrla = 0;
	uint32_t ctrlb = 0;

	/* Get a pointer to the hardware module instance */
	SercomUsart *const usart_module = &(dev_inst->hw_dev->USART);

	/* TODO: change clock_polarity to enum if we don't get bp's in the
	 * header-file */
	/* Set data order, internal muxing, and clock polarity */
	ctrla = (config->data_order) | (config->mux_settings)
			| (config->clock_polarity_inverted <<
			SERCOM_USART_CTRLA_CPOL_Pos);

	/* Get baud value from mode and clock */
	if (config->transfer_mode == USART_TRANSFER_SYNCHRONOUSLY &&
			!config->use_external_clock) {
		/* Calculate baud value */
		usart_freq = system_gclk_chan_get_hz(SERCOM_GCLK_ID);
		status_code = _sercom_get_sync_baud_val(config->baudrate,
				usart_freq, &baud_val);
	}

	if (config->transfer_mode == USART_TRANSFER_ASYNCHRONOUSLY &&
			!config->use_external_clock) {
		/* Calculate baud value */
		usart_freq = system_gclk_chan_get_hz(SERCOM_GCLK_ID);
		status_code = _sercom_get_async_baud_val(config->baudrate,
				usart_freq, &baud_val);
	}

	if (config->transfer_mode == USART_TRANSFER_ASYNCHRONOUSLY &&
			config->use_external_clock) {
		/* Calculate baud value */
		status_code = _sercom_get_async_baud_val(config->baudrate,
				config->ext_clock_freq,
				&baud_val);
	}

	/* Check if calculating the baud rate failed */
	if (status_code != STATUS_OK) {
		/* Abort */
		return status_code;
	}

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/*Set baud val */
	usart_module->BAUD.reg = baud_val;

	/* Set sample mode */
	ctrla |= config->transfer_mode;
	ctrla |= SERCOM_USART_CTRLA_MODE(0);

	if (config->use_external_clock == false) {
		ctrla |= SERCOM_USART_CTRLA_CSRC;
	}

	/* Set stopbits and character size */
	ctrlb = config->stopbits | config->char_size;

	/* set parity mode */
	if (config->parity != USART_PARITY_NONE) {
		ctrla |= SERCOM_USART_CTRLA_FORM(1);
		ctrlb |= config->parity;
	} else {
		ctrla |= SERCOM_USART_CTRLA_FORM(0);
	}

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Write configuration to CTRLB */
	usart_module->CTRLB.reg = ctrlb;

	_usart_wait_for_sync(dev_inst);
	/* Write configuration to CTRLA */
	usart_module->CTRLA.reg = ctrla;

	return STATUS_OK;
}

/**
 * \brief Initializes the device
 *
 * Initializes the USART device based on the setting specified in the
 * configuration struct. This will leave the device in an enabled state
 * after initialization.
 *
 * \param[out] dev_inst Pointer to USART device
 * \param[in]  hw_dev   Pointer to USART hardware instance
 * \param[in]  config   Pointer to configuration struct
 *
 * \return Status of the initialization
 *
 * \retval STATUS_OK                       The initialization was successful
 * \retval STATUS_ERR_BUSY                 The USART module is occupied with
 *                                         resetting itself
 * \retval STATUS_ERR_DENIED               The USART have not been disabled in
 *                                         advance of initialization
 * \retval STATUS_ERR_INVALID_ARG          The configuration struct contains
 *                                         invalid configuration for the
 * sample_mode
 * \retval STATUS_ERR_ALREADY_INITIALIZED  The SERCOM instance has already been
 *                                         initialized with different clock
 * configuration
 * \retval STATUS_ERR_BAUD_UNAVAILABLE     The BAUD rate given by the
 * configuration
 *                                         struct due to sample_mode or clock
 * frequency
 */
enum status_code usart_init(struct usart_dev_inst *const dev_inst,
		Sercom *const hw_dev, const struct usart_conf *const config)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(hw_dev);
	Assert(config);

	struct system_gclk_chan_conf gclk_chan_conf;
	enum status_code status_code = STATUS_OK;
	uint32_t sercom_index = 0;
	uint32_t gclk_index = 0;
	struct system_pinmux_conf pin_conf;

	uint32_t pad0 = config->pinout_pad0;
	uint32_t pad1 = config->pinout_pad1;
	uint32_t pad2 = config->pinout_pad2;
	uint32_t pad3 = config->pinout_pad3;

	/* Assign module pointer to software instance struct */
	dev_inst->hw_dev = hw_dev;
	/* Get a pointer to the hardware module instance */
	SercomUsart *const usart_module = &(dev_inst->hw_dev->USART);

	/* Set up the GCLK for the module */
	system_gclk_chan_get_config_defaults(&gclk_chan_conf);

	sercom_index = _sercom_get_sercom_inst_index(dev_inst->hw_dev);

	gclk_index =  sercom_index + 13;

	gclk_chan_conf.source_generator = config->generator_source;
	system_gclk_chan_set_config(gclk_index, &gclk_chan_conf);
	system_gclk_chan_set_config(SERCOM_GCLK_ID, &gclk_chan_conf);
	system_gclk_chan_enable(gclk_index);
	system_gclk_chan_enable(SERCOM_GCLK_ID);
	system_pinmux_get_config_defaults(&pin_conf);

	/* Enable the user interface clock in the PM */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC,
			(1 << (sercom_index + 2)));

	/* Configure Pins */
	system_pinmux_get_config_defaults(&pin_conf);
	pin_conf.direction    = SYSTEM_PINMUX_PIN_DIR_INPUT;

	/* SERCOM PAD0 */
	if (pad0 == PINMUX_DEFAULT) {
		pad0 = _sercom_get_default_pad(hw_dev, 0);
	}

	pin_conf.mux_position = pad0 & 0xFFFF;
	system_pinmux_pin_set_config(pad0 >> 16, &pin_conf);

	/* SERCOM PAD1 */
	if (pad1 == PINMUX_DEFAULT) {
		pad1 = _sercom_get_default_pad(hw_dev, 1);
	}

	pin_conf.mux_position = pad1 & 0xFFFF;
	system_pinmux_pin_set_config(pad1 >> 16, &pin_conf);

	/* SERCOM PAD2 */
	if (pad2 == PINMUX_DEFAULT) {
		pad2 = _sercom_get_default_pad(hw_dev, 2);
	}

	pin_conf.mux_position = pad2 & 0xFFFF;

	system_pinmux_pin_set_config(pad2 >> 16, &pin_conf);

	/* SERCOM PAD3 */
	if (pad3 == PINMUX_DEFAULT) {
		pad3 = _sercom_get_default_pad(hw_dev, 3);
	}

	pin_conf.mux_position = pad3 & 0xFFFF;
	system_pinmux_pin_set_config(pad3 >> 16, &pin_conf);

	/* Wait for synchronization to be complete*/
	_usart_wait_for_sync(dev_inst);

	while (usart_module->CTRLA.reg & SERCOM_USART_CTRLA_SWRST) {
	}

	if (usart_module->CTRLA.reg & SERCOM_USART_CTRLA_ENABLE) {
		/* Module have to be disabled before initialization. Abort. */
		return STATUS_ERR_DENIED;
	}

#ifdef USART_ASYNC
	/* Temporary variables */
	uint8_t i;
	uint8_t instance_index;

	/* Initialize parameters */
	for (i = 0; i < USART_CALLBACK_N; i++) {
		dev_inst->callback[i]        = NULL;
	}
	dev_inst->tx_buffer_ptr              = NULL;
	dev_inst->rx_buffer_ptr              = NULL;
	dev_inst->remaining_tx_buffer_length = 0x0000;
	dev_inst->remaining_rx_buffer_length = 0x0000;
	dev_inst->callback_reg_mask          = 0x00;
	dev_inst->callback_enable_mask       = 0x00;
	dev_inst->rx_status                  = STATUS_OK;
	dev_inst->tx_status                  = STATUS_OK;

	/* Set interrupt handler and register USART software module struct in
	 * look-up table */
	instance_index = _sercom_get_sercom_inst_index(dev_inst->hw_dev);
	/* _sercom_set_handler(instance_index, (void *)&usart_async_handler); */
	_sercom_instances[instance_index] = dev_inst;
#endif
	/* Set configuration according to the config struct */
	status_code = _usart_set_config(dev_inst, config);

	return status_code;
}

/**
 * \brief Transmit a character via the USART
 *
 * This non-blocking function will receive a character via the
 * USART.
 *
 * param[in]   dev_inst Pointer to the software instance struct
 * param[out]  tx_data  Data to transfer
 *
 * \return     Status of the operation
 * \retval     STATUS_OK           If the operation was completed
 * \retval     STATUS_ERR_BUSY     If the operation was not completed,
 *                                 due to the USART module being busy.
 */
enum status_code usart_write(struct usart_dev_inst *const dev_inst,
		const uint16_t tx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

#ifdef USART_ASYNC
	/* Check if the USART is busy doing asynchronous operation. */
	if (dev_inst->remaining_tx_buffer_length > 0) {
		return STATUS_ERR_BUSY;
	}

#else
	/* Check if USART is ready for new data */
	if (!usart_is_interrupt_flag_set(dev_inst,
			USART_INTERRUPT_FLAG_DATA_BUFFER_EMPTY)) {
		/* Return error code */
		return STATUS_ERR_BUSY;
	}
#endif
	/* Get a pointer to the hardware module instance */
	SercomUsart *const usart_module = &(dev_inst->hw_dev->USART);

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Write data to USART module */
	usart_module->DATA.reg = tx_data;

	return STATUS_OK;
}

/**
 * \brief Receive a character via the USART
 *
 * This non-blocking function will receive a character via the
 * USART.
 *
 * param[in]   dev_inst Pointer to the software instance struct
 * param[out]  rx_data  Pointer to received data
 *
 * \return     Status of the operation
 * \retval     STATUS_OK                If the operation was completed
 * \retval     STATUS_ERR_BUSY          If the operation was not completed,
 *                                      due to the USART module being busy.
 * \retval     STATUS_ERR_BAD_FORMAT    If the operation was not completed,
 *                                      due to mismatch configuration mismatch
 *                                      between USART and the sender.
 * \retval     STATUS_ERR_BAD_OVERFLOW  If the operation was not completed,
 *                                      due to the baud rate being to low or the
 *                                      system frequency being to high.
 * \retval     STATUS_ERR_BAD_DATA      If the operation was not completed, due
 *                                      to data being corrupted.
 */
enum status_code usart_read(struct usart_dev_inst *const dev_inst,
		uint16_t *const rx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Error variable */
	uint16_t error_code;

#ifdef USART_ASYNC
	/* Check if the USART is busy doing asynchronous operation. */
	if (dev_inst->remaining_rx_buffer_length > 0) {
		return STATUS_ERR_BUSY;
	}

#else
	/* Check if USART has new data */
	if (!usart_is_interrupt_flag_set(dev_inst,
			USART_INTERRUPT_FLAG_RX_COMPLETE)) {
		/* Return error code */
		return STATUS_ERR_BUSY;
	}
#endif

	/* Get a pointer to the hardware module instance */
	SercomUsart *const usart_module = &(dev_inst->hw_dev->USART);

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Read out the status code and mask away all but the 4 LSBs*/
	error_code
		= (uint8_t)(usart_module->STATUS.reg &
			SERCOM_USART_STATUS_MASK);

	/* Check if an error has occurred during the receiving */
	if (error_code) {
		/* Check which error occurred */
		if (error_code & SERCOM_USART_STATUS_FERR) {
			/* Clear flag by writing a 1 to it and
			 * return with an error code */
			usart_module->STATUS.reg &= SERCOM_USART_STATUS_FERR;
			return STATUS_ERR_BAD_FORMAT;
		} else if (error_code & SERCOM_USART_STATUS_BUFOVF) {
			/* Clear flag by writing a 1 to it and
			 * return with an error code */
			usart_module->STATUS.reg &= SERCOM_USART_STATUS_BUFOVF;
			return STATUS_ERR_OVERFLOW;
		} else if (error_code & SERCOM_USART_STATUS_PERR) {
			/* Clear flag by writing a 1 to it and
			 * return with an error code */
			usart_module->STATUS.reg &= SERCOM_USART_STATUS_PERR;
			return STATUS_ERR_BAD_DATA;
		}
	}

	/* Read data from USART module */
	*rx_data = usart_module->DATA.reg;

	return STATUS_OK;
}

/**
 * \brief Transmit a buffer of \ref length length characters via USART
 *
 * This blocking function will transmit a block of \ref length length characters
 * via the USART
 *
 * \note Using this function in combination with the asynchronous functions is
 *       not recommended as it has no functionality to check if there is an
 *       ongoing asynchronous operation running or not.
 *
 * \param[in]     dev_inst Pointer to USART software instance struct
 * \param[out]    tx_data  Pointer to data to transmit
 * \param[length] number   Number of characters to transmit
 *
 * \return        Status of the operation
 * \retval        STATUS_OK                If operation was completed
 * \retval        STATUS_ERR_INVALID_ARG   If operation was not completed,
 *                                         due to invalid arguments
 * \retval        STATUS_ERR_TIMEOUT       If operation was not completed,
 *                                         due to USART module timing out
 */
enum status_code usart_write_buffer(struct usart_dev_inst *const dev_inst,
		const uint8_t *tx_data, uint16_t length)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Timeout variables */
	uint16_t i = 0;
	uint16_t timeout;
#ifdef USART_CUSTOM_TIMEOUT
	timeout = USART_CUSTOM_TIMEOUT;
#else
	timeout = USART_DEFAULT_TIMEOUT;
#endif

	/* Check if the buffer length is valid */
	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(dev_inst);

	/* Blocks while buffer is being transferred */
	while (length--) {
		/* Wait for the USART to be ready for new data and abort
		* operation if it doesn't get ready within the timeout*/
		for (i = 0; i < timeout; i++) {
			if (usart_is_interrupt_flag_set(dev_inst,
					USART_INTERRUPT_FLAG_RX_COMPLETE)) {
				break;
			} else if (i == timeout) {
				return STATUS_ERR_TIMEOUT;
			}
		}

		/* Check if the character size exceeds 8 bit */
		if (dev_inst->char_size == USART_CHAR_SIZE_9BIT) {
			/* Increment 8 bit pointer by two */
			usart_write(dev_inst, (uint16_t)*(tx_data));
			tx_data += 2;
		} else {
			/* Increment 8 bit pointer by one */
			usart_write(dev_inst, (uint16_t)*(tx_data++));
		}
	}

	return STATUS_OK;
}

/**
 * \brief Receive a buffer of \ref length length characters via USART
 *
 * This blocking function will receive a block of \ref length length characters
 * via the USART.
 *
 * \note Using this function in combination with the asynchronous functions is
 *       not recommended as it has no functionality to check if there is an
 *       ongoing asynchronous operation running or not.
 *
 * \param[in]     dev_inst Pointer to USART software instance struct
 * \param[out]    tx_data  Pointer to data to transmit
 * \param[length] number   Number of characters to transmit
 *
 * \return     Status of the operation
 * \retval     STATUS_OK                If operation was completed
 * \retval     STATUS_ERR_INVALID_ARG   If operation was not completed, due
 *                                     to invalid arguments
 * \retval     STATUS_ERR_TIMEOUT       If operation was not completed, due
 *                                      to USART module timing out
 * \retval     STATUS_ERR_BAD_FORMAT    If the operation was not completed,
 *                                      due to mismatch configuration mismatch
 *                                      between USART and the sender.
 * \retval     STATUS_ERR_BAD_OVERFLOW  If the operation was not completed,
 *                                      due to the baud rate being to low or the
 *                                      system frequency being to high.
 * \retval     STATUS_ERR_BAD_DATA      If the operation was not completed, due
 *                                      to data being corrupted.
 */
enum status_code usart_read_buffer(struct usart_dev_inst *const dev_inst,
		const uint8_t *rx_data, uint16_t length)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Timeout variables */
	uint16_t i = 0;
	uint16_t timeout;

#ifdef USART_CUSTOM_TIMEOUT
	timeout = USART_CUSTOM_TIMEOUT;
#else
	timeout = USART_DEFAULT_TIMEOUT;
#endif
	/* Check if the buffer length is valid */
	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Blocks while buffer is being received */
	while (length--) {
		/* Wait for the USART to have new data and abort operation if it
		 * doesn't get ready within the timeout*/
		for (i = 0; i < timeout; i++) {
			if (usart_is_interrupt_flag_set(dev_inst,
					USART_INTERRUPT_FLAG_RX_COMPLETE)) {
				break;
			} else if (i == timeout) {
				return STATUS_ERR_TIMEOUT;
			}
		}

		/* Check if the character size exceeds 8 bit */
		if (dev_inst->char_size == USART_CHAR_SIZE_9BIT) {
			/* Increment the 8 bit data pointer by two */
			usart_read(dev_inst, (uint16_t *)(rx_data));
			rx_data += 2;
		} else {
			/* Increment the 8 bit data pointer by one */
			usart_read(dev_inst, (uint16_t *)(rx_data++));
		}
	}

	return STATUS_OK;
}
