/**
 * \file
 *
 * \brief SAMD20 Serial Peripheral Interface Driver
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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
#include "spi_interrupt.h"

/**
 * \internal
 * Writes of a buffer with a given length
 *
 * \param[in]  module   Pointer to SPI software instance struct
 * \param[in]  tx_data  Pointer to data to be transmitted
 * \param[in]  length   Length of data buffer
 *
 */
static void _spi_write_buffer(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint16_t length)
{
	Assert(module);
	Assert(tx_data);

	/* Write parameters to the device instance */
	module->remaining_tx_buffer_length = length;
	module->tx_buffer_ptr = tx_data;
	module->tx_status = STATUS_BUSY;

	if (module->dir == SPI_DIRECTION_IDLE) {
		module->dir = SPI_DIRECTION_WRITE;
	} else {
		module->dir = SPI_DIRECTION_BOTH;
	}

	/* Get a pointer to the hardware module instance */
	SercomSpi *const hw = &(module->hw->SPI);

	/* Enable the Data Register Empty, TX and RX Complete Interrupt */
	hw->INTENSET.reg = (SPI_INTERRUPT_FLAG_DATA_REGISTER_EMPTY |
			SPI_INTERRUPT_FLAG_TX_COMPLETE | SPI_INTERRUPT_FLAG_RX_COMPLETE);
}

/**
 * \internal
 * Setup SPI to read a buffer with a given length
 *
 * \param[in]  module   Pointer to SPI software instance struct
 * \param[in]  rx_data  Pointer to data to be received
 * \param[in]  length   Length of data buffer
 *
 */
static void _spi_read_buffer(
		struct spi_module *const module,
		uint8_t *rx_data,
		uint16_t length)
{
	Assert(module);
	Assert(rx_data);

	/* Set length for the buffer and the pointer, and let
	 * the interrupt handler do the rest */
	module->remaining_rx_buffer_length = length;
	module->remaining_dummy_buffer_length = length;
	module->rx_buffer_ptr = rx_data;
	module->rx_status = STATUS_BUSY;

	if (module->dir == SPI_DIRECTION_IDLE) {
		module->dir = SPI_DIRECTION_READ;
	} else {
		module->dir = SPI_DIRECTION_BOTH;
	}
	/* Get a pointer to the hardware module instance */
	SercomSpi *const hw = &(module->hw->SPI);

	/* Enable the RX Complete Interrupt */
	hw->INTENSET.reg = SPI_INTERRUPT_FLAG_RX_COMPLETE;

	if (module->mode == SPI_MODE_MASTER && module->dir == SPI_DIRECTION_READ) {
		/* Enable Data Register Empty interrupt if needed */
		hw->INTENSET.reg = SPI_INTERRUPT_FLAG_DATA_REGISTER_EMPTY;
	}
}

/**
 * \brief Registers a SPI callback function
 *
 * Registers a callback function which is implemented by the user.
 *
 * \note The callback must be enabled by \ref spi_register_callback, in order
 *       for the interrupt handler to call it when the conditions for the
 *       callback type is met.
 *
 * \param[in]  module         Pointer to USART software instance struct
 * \param[in]  callback_func  Pointer to callback function
 * \param[in]  callback_type  Callback type given by an enum
 *
 */
void spi_register_callback(
		struct spi_module *const module,
		spi_callback_t callback_func,
		enum spi_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(callback_func);

	/* Register callback function */
	module->callback[callback_type] = callback_func;

	/* Set the bit corresponding to the callback_type */
	module->registered_callback |= (1 << callback_type);
}

/**
 * \brief Unregisters a SPI callback function
 *
 * Unregisters a callback function which is implemented by the user.
 *
 * \param[in] module         Pointer to SPI software instance struct
 * \param[in] callback_type  Callback type given by an enum
 *
 */
void spi_unregister_callback(
		struct spi_module *const module,
		enum spi_callback callback_type)
{
	/* Sanity check arguments */
	Assert(module);

	/* Unregister callback function */
	module->callback[callback_type] = NULL;

	/* Clear the bit corresponding to the callback_type */
	module->registered_callback &= ~(0 << callback_type);
}

/**
 * \brief Asynchronous buffer write
 *
 * Sets up the driver to write to the SPI from a given buffer. If registered
 * and enabled, a callback function will be called when the write is finished.
 *
 * \param[in]  module   Pointer to USART software instance struct
 * \param[out] tx_data  Pointer to data buffer to receive
 * \param[in]  length   Data buffer length
 *
 * \returns Status of the write request operation.
 * \retval STATUS_OK               If the operation completed successfully
 * \retval STATUS_ERR_BUSY         If the SPI was already busy with a write
 *                                 operation
 * \retval STATUS_ERR_INVALID_ARG  If requested write length was zero
 */
enum status_code spi_write_buffer_job(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint16_t length)
{
	Assert(module);
	Assert(tx_data);

	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Check if the SPI is busy transmitting */
	if (module->remaining_tx_buffer_length > 0 || (module->tx_status == STATUS_BUSY)) {
		return STATUS_BUSY;
	}

	/* Issue internal write */
	_spi_write_buffer(module, tx_data, length);

	return STATUS_OK;
}

/**
 * \brief Asynchronous buffer read
 *
 * Sets up the driver to read from the SPI to a given buffer. If registered
 * and enabled, a callback function will be called when the read is finished.
 *
 * \param[in]  module   Pointer to SPI software instance struct
 * \param[out] rx_data  Pointer to data buffer to receive
 * \param[in]  length   Data buffer length
 *
 * \returns Status of the operation
 * \retval  STATUS_OK               If the operation completed successfully
 * \retval  STATUS_ERR_BUSY         If the SPI was already busy with a read
 *                                  operation
 * \retval  STATUS_ERR_INVALID_ARG  If requested read length was zero
 */
enum status_code spi_read_buffer_job(
		struct spi_module *const module,
		uint8_t *rx_data,
		uint16_t length)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(rx_data);

	if (length == 0) {
		return STATUS_ERR_INVALID_ARG;
	}

	/* Check if the SPI is busy */
	if (module->remaining_rx_buffer_length > 0 || (module->rx_status == STATUS_BUSY)) {
		return STATUS_BUSY;
	}

	/* Issue internal read */
	_spi_read_buffer(module, rx_data, length);
	return STATUS_OK;
}

/**
 * \brief Aborts an ongoing job
 *
 * This function will abort the specified job type.
 *
 * \param[in]  module    Pointer to SPI software instance struct
 * \param[in]  job_type  Type of job to abort
 */
void spi_abort_job(
		struct spi_module *const module,
		enum spi_job_type job_type)
{
	/* Pointer to the hardware module instance */
	SercomSpi *const spi_hw
		= &(module->hw->SPI);

	if (job_type == SPI_JOB_READ_BUFFER) {
		/* Abort read buffer job */
		module->rx_status = STATUS_ABORTED;
		module->remaining_rx_buffer_length = 0;
		module->remaining_dummy_buffer_length = 0;
		if (module->dir == SPI_DIRECTION_READ) {
			spi_hw->INTENCLR.reg = SPI_INTERRUPT_FLAG_RX_COMPLETE;
			module->dir = SPI_DIRECTION_IDLE;
		} else if (module->dir == SPI_DIRECTION_BOTH) {
			module->dir = SPI_DIRECTION_WRITE;
		}
	} else {
		/* Abort write buffer job */
		module->tx_status = STATUS_ABORTED;
		module->remaining_tx_buffer_length = 0;
                if (module->dir == SPI_DIRECTION_WRITE) {
                  spi_hw->INTENCLR.reg = SPI_INTERRUPT_FLAG_DATA_REGISTER_EMPTY
                        | SPI_INTERRUPT_FLAG_TX_COMPLETE;
                    module->dir = SPI_DIRECTION_IDLE;
                } else if (module->dir == SPI_DIRECTION_BOTH) {
                  module->dir = SPI_DIRECTION_READ;
                }
	}
}

/**
 * \brief Retrieves the current status of a job.
 *
 * Retrieves the current statue of a job that was previously issued.
 *
 * \param[in]  module    Pointer to SPI software instance struct
 * \param[in]  job_type  Type of job to check
 *
 * \return Current job status
 */
enum status_code spi_get_job_status(
		const struct spi_module *const module,
		enum spi_job_type job_type)
{
	if (job_type == SPI_JOB_READ_BUFFER) {
		return module->rx_status;
	} else {
		return module->tx_status;
	}
}

/**
 * \internal
 * Writes a character from the TX buffer to the Data register.
 *
 * \param[in,out]  module  Pointer to SPI software instance struct
 */
static void _spi_write(
		struct spi_module *const module)
{
	/* Pointer to the hardware module instance */
	SercomSpi *const spi_hw = &(module->hw->SPI);

	/* Write value will be at least 8-bits long */
	uint16_t data_to_send = *(module->tx_buffer_ptr);
	/* Increment 8-bit pointer */
	(module->tx_buffer_ptr)++;

	if (module->character_size == SPI_CHARACTER_SIZE_9BIT) {
		data_to_send |= ((*(module->tx_buffer_ptr)) << 8);
		/* Increment 8-bit pointer */
		(module->tx_buffer_ptr)++;
	}

	/* Write the data to send*/
	spi_hw->DATA.reg = data_to_send & SERCOM_SPI_DATA_MASK;

	/* Decrement remaining buffer length */
	(module->remaining_tx_buffer_length)--;
}

/**
 * \internal
 * Writes a dummy character from the to the Data register.
 *
 * \param[in,out]  module  Pointer to SPI software instance struct
 */
static void _spi_write_dummy(
		struct spi_module *const module)
{
	/* Pointer to the hardware module instance */
	SercomSpi *const spi_hw = &(module->hw->SPI);

	/* Write dummy byte */
	spi_hw->DATA.reg = 0xAA;

	/* Decrement remaining dummy buffer length */
	module->remaining_dummy_buffer_length--;
}

/**
 * \internal
 * Reads a character from the Data register to the RX buffer.
 *
 * \param[in,out]  module  Pointer to SPI software instance struct
 */
static void _spi_read(
		struct spi_module *const module)
{
	/* Pointer to the hardware module instance */
	SercomSpi *const spi_hw = &(module->hw->SPI);

	uint16_t received_data = (spi_hw->DATA.reg & SERCOM_SPI_DATA_MASK);

	/* Read value will be at least 8-bits long */
	*(module->rx_buffer_ptr) = received_data;
	/* Increment 8-bit pointer */
	module->rx_buffer_ptr += 1;

	if(module->character_size == SPI_CHARACTER_SIZE_9BIT) {
		/* 9-bit data, write next received byte to the buffer */
		*(module->rx_buffer_ptr) = (received_data >> 8);
		/* Increment 8-bit pointer */
		module->rx_buffer_ptr += 1;
	}

	/* Decrement length of the remaining buffer */
	module->remaining_rx_buffer_length--;
}

/**
 * \internal
 *
 * Handles interrupts as they occur, and it will run callback functions
 * which are registered and enabled.
 *
 * \note This function will be called by the Sercom_Handler, and should
 *       not be called directly from any application code.
 *
 * \param[in]  instance  ID of the SERCOM instance calling the interrupt
 *                       handler.
 */
void _spi_interrupt_handler(
		uint8_t instance)
{
	/* Get device instance from the look-up table */
	struct spi_module *module
		= (struct spi_module *)_sercom_instances[instance];

	/* Pointer to the hardware module instance */
	SercomSpi *const spi_hw = &(module->hw->SPI);

	/* Combine callback registered and enabled masks. */
	uint8_t callback_mask =
			module->enabled_callback & module->registered_callback;

	/* Read and mask interrupt flag register */
	uint16_t interrupt_status = (spi_hw->INTFLAG.reg & spi_hw->INTENSET.reg);

	if (interrupt_status & SPI_INTERRUPT_FLAG_DATA_REGISTER_EMPTY) {
		if (module->mode == SPI_MODE_MASTER) {
			if (module->dir == SPI_DIRECTION_READ) {
				/* Send dummy byte when reading */
				_spi_write_dummy(module);

				/* Check if more dummy bytes should be sent */
				if (module->remaining_dummy_buffer_length == 0) {
					/* Disable the Data Register Empty Interrupt */
					spi_hw->INTENCLR.reg
						= SPI_INTERRUPT_FLAG_DATA_REGISTER_EMPTY;
				}
			} else if (module->remaining_tx_buffer_length) {
				/* Send next byte from buffer */
				_spi_write(module);

				if (module->remaining_dummy_buffer_length) {
					/* Decrement dummy buffer length if needed */
					(module->remaining_dummy_buffer_length)--;
				}

				/* Check if it was the last transmission */
				if (module->remaining_tx_buffer_length == 0) {
					/* Disable the Data Register Empty Interrupt */
					spi_hw->INTENCLR.reg
							= SPI_INTERRUPT_FLAG_DATA_REGISTER_EMPTY;
				}
			}
		} else {
			/* Write next byte from buffer in slave mode */
			_spi_write(module);

			if (module->remaining_tx_buffer_length == 0) {
				/* Disable the Data Register Empty Interrupt */
				spi_hw->INTENCLR.reg
						= SPI_INTERRUPT_FLAG_DATA_REGISTER_EMPTY;
			}
		}
	} else if (interrupt_status & SPI_INTERRUPT_FLAG_TX_COMPLETE) {
		if (module->mode == SPI_MODE_MASTER &&
				(module->remaining_tx_buffer_length == 0)){
			/* Disable interrupt */
			spi_hw->INTENCLR.reg = SPI_INTERRUPT_FLAG_TX_COMPLETE;
			module->tx_status = STATUS_OK;

			/* Change direction*/
			if (module->dir == SPI_DIRECTION_BOTH) {
				module->dir = SPI_DIRECTION_READ;
				/* Enable Data Register Empty Interrupt */
				spi_hw->INTENSET.reg
						= SPI_INTERRUPT_FLAG_DATA_REGISTER_EMPTY;
			} else {
				module->dir = SPI_DIRECTION_IDLE;
				spi_hw->INTENCLR.reg = SPI_INTERRUPT_FLAG_RX_COMPLETE;
			}

			/* Run callback if registered and enabled */
			if (callback_mask & (1 << SPI_CALLBACK_BUFFER_TRANSMITTED)){
				(module->callback[SPI_CALLBACK_BUFFER_TRANSMITTED])(module);
			}
		} else if (module->mode == SPI_MODE_SLAVE) {
			/* Transaction ended by master, stop ongoing transmissions */
			spi_hw->INTENCLR.reg =
					SPI_INTERRUPT_FLAG_TX_COMPLETE |
					SPI_INTERRUPT_FLAG_RX_COMPLETE |
					SPI_INTERRUPT_FLAG_DATA_REGISTER_EMPTY;
			spi_hw->INTFLAG.reg = SPI_INTERRUPT_FLAG_TX_COMPLETE;

			module->tx_status = STATUS_OK;
			module->rx_status = STATUS_OK;
			module->dir = SPI_DIRECTION_IDLE;
			module->remaining_tx_buffer_length = 0;
			module->remaining_rx_buffer_length = 0;

			/* Run callback if registered and enabled */
			if (callback_mask & (1 << SPI_CALLBACK_SLAVE_TRANSMISSION_COMPLETE)){
				(module->callback[SPI_CALLBACK_SLAVE_TRANSMISSION_COMPLETE])(module);
			}
		}
	} else if (interrupt_status & SPI_INTERRUPT_FLAG_RX_COMPLETE) {
		/* Check for overflow */
		if (spi_hw->STATUS.reg & SERCOM_SPI_STATUS_BUFOVF) {
			if (module->dir == SPI_DIRECTION_READ || module->dir == SPI_DIRECTION_BOTH) {
				/* Store the error code */
				module->rx_status = STATUS_ERR_OVERFLOW;

				/* Run callback if registered and enabled */
				if (callback_mask & (1 << SPI_CALLBACK_ERROR)) {
					(module->callback[SPI_CALLBACK_ERROR])(module);
				}
			}

			/* Clear overflow flag */
			spi_hw->STATUS.reg |= SERCOM_SPI_STATUS_BUFOVF;
		}
		if (module->dir == SPI_DIRECTION_WRITE) {
			/* Flush data register when writing */
			uint16_t flush = spi_hw->DATA.reg;
			UNUSED(flush);

			if (module->remaining_tx_buffer_length == 0) {
				/* Write complete */
				module->dir = SPI_DIRECTION_IDLE;
				module->tx_status = STATUS_OK;

				spi_hw->INTENCLR.reg = SPI_INTERRUPT_FLAG_RX_COMPLETE;

				if (module->mode == SPI_MODE_SLAVE &&
						(callback_mask & (1 << SPI_CALLBACK_BUFFER_TRANSMITTED))) {

					/* Run callback for slave if registered and enabled */
					(module->callback[SPI_CALLBACK_BUFFER_TRANSMITTED])(module);
				}
			}
		} else if (module->rx_status != STATUS_ABORTED) {
			/* Read data register */
			_spi_read(module);

			/* Check if the last character have been received */
			if(module->remaining_rx_buffer_length == 0 &&
					!(module->dir == SPI_DIRECTION_WRITE)) {
				module->rx_status = STATUS_OK;

				if (module->dir == SPI_DIRECTION_BOTH) {
					module->dir = SPI_DIRECTION_WRITE;
				} else if (module->dir == SPI_DIRECTION_READ){
					/* Disable RX Complete Interrupt and set status */
					spi_hw->INTENCLR.reg = SPI_INTERRUPT_FLAG_RX_COMPLETE;
					module->dir = SPI_DIRECTION_IDLE;
				}

				/* Run callback if registered and enabled */
				if (callback_mask & (1 << SPI_CALLBACK_BUFFER_RECEIVED)) {
					(module->callback[SPI_CALLBACK_BUFFER_RECEIVED])(module);
				}
			}
		}
	}
}
