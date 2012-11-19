#include <usart_async.h>

/**
 * \internal 
 *
 *
 * param[]
 *
 * returns 
 * retval  
 */
uint8_t _sercom_get_module_irq_index(struct usart_dev_inst *const dev_inst)
{


	switch((uint32_t)dev_inst->hw_dev){
		//TODO: fix casting	
		/*
		case (uint32_t)SERCOM0:
			return 0;

		case SERCOM1:
			return 1;

		case SERCOM2:
			return 2;

		case SERCOM3:
			return 3;
		*/

		case 0: 
			return 120;

		default:
			Assert(false);
	}
}

void _usart_async_write(struct usart_dev_inst *const dev_inst, uint8_t *tx_data, uint16_t length) {

	/* Write parameters to the device instance  */	
	dev_inst->remaining_tx_buffer_length = length;
	/* Check if the pointer is valid */
	if (length > 1) { 
		dev_inst->tx_buffer_ptr = tx_data; 
	} else {
		dev_inst->tx_buffer_ptr = NULL;

	/* Get a pointer to the hardware module instance */
//	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Load data into the USART TX data buffer */
//	usart_module->DATA |= (tx_data & SERCOM_USART_DATA_gm);
}

void _usart_async_read(struct usart_dev_inst *const dev_inst, uint8_t *rx_data, uint16_t length) {
	
	/* Set length for the buffer and the pointer, and let 
	 * the interrupt handler do the rest */
	dev_inst->remaining_rx_buffer_length = length;
	dev_inst->rx_buffer_ptr;
}


/**
 * \brief 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
void usart_async_register_callback(struct usart_dev_inst *const dev_inst,
		const usart_async_callback_t *const callback_func,
		enum usart_callback_type callback_type)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(callback_func);

	/* Register callback function */
	dev_inst->callback[callback_type] = callback_func;
	//TODO: fix casting
	_sercom_instances[_sercom_get_module_irq_index(dev_inst)] =
			*(uint32_t *)dev_inst;

	/* Set the bit corresponding to the callback_type */
	dev_inst->callback_reg_mask |= (1 << callback_type);
}

/**
 * \brief 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
void usart_async_unregister_callback(struct usart_dev_inst *const dev_inst,
		enum usart_callback_type callback_type)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Unregister callback function */
	dev_inst->callback[callback_type] = NULL;
	_sercom_instances[_sercom_get_module_irq_index(dev_inst)] = 0;

	/* Clear the bit corresponding to the callback_type */
	dev_inst->callback_reg_mask |= (0 << callback_type);
}



/**
 * \brief 
 *
 *
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
enum status_code usart_async_enable_callback(struct usart_dev_inst *const dev_inst,
		enum usart_callback_type callback_type)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Enable callback */
	dev_inst->callback_enable_mask |= (0 << callback_type);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Enable the interrupt flag */
	switch (callback_type){
		case USART_CALLBACK_TYPE_BUFFER_EMPTY:
			usart_module->INTENSET = SERCOM_USART_DREIF_bm;
			break;

		case USART_CALLBACK_TYPE_BUFFER_TRANSMITTED:
			usart_module->INTENSET = SERCOM_USART_TXCIF_bm;
			break;

		case USART_CALLBACK_TYPE_BUFFER_RECEIVED:
			usart_module->INTENSET = SERCOM_USART_RXCIF_bm;
			break;

		default:
			Assert(false);
			return STATUS_ERR_INVALID_ARG;

	}

	return STATUS_OK;

}

/**
 * \brief 
 *
 *
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
enum status_code usart_async_disable_callback(struct usart_dev_inst *const dev_inst,
		enum usart_callback_type callback_type)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Disable callback */
	dev_inst->callback_enable_mask |= (1 << callback_type);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Disable the interrupt flag */
	switch (callback_type){
		case USART_CALLBACK_TYPE_BUFFER_EMPTY:
			usart_module->INTENCLR = SERCOM_USART_DREIF_bm;
			break;

		case USART_CALLBACK_TYPE_BUFFER_TRANSMITTED:
			usart_module->INTENCLR = SERCOM_USART_TXCIF_bm;
			break;

		case USART_CALLBACK_TYPE_BUFFER_RECEIVED:
			usart_module->INTENCLR = SERCOM_USART_RXCIF_bm;
			break;

		default:
			Assert(false);
			return STATUS_ERR_INVALID_ARG;

	}

	return STATUS_OK;
}



/**
 * \brief 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
enum status_code usart_async_write(struct usart_dev_inst *const dev_inst,
		const uint16_t tx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);


	/* Check if the USART transmitter is busy */
	if (!usart_is_data_buffer_empty(dev_inst)) {
		return STATUS_ERR_BUSY;
	}

	_usart_async_write(dev_inst, tx_data, 1);
	
	return STATUS_OK;
}


/**
 * \brief 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
//TODO: differ on number of bits for char_size??
enum status_code usart_async_read(struct usart_dev_inst *const dev_inst,
		uint16_t *const rx_data)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Check if the USART receiver is busy */
	if (dev_inst->remaining_rx_buffer_length) {
		return STATUS_ERR_BUSY;
	}

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Read data from the USART RX data buffer */
	*rx_data = (usart_module->DATA & SERCOM_USART_DATA_gm);

	return STATUS_OK;
}

/**
 * \brief 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
enum status_code usart_async_write_buffer(struct usart_dev_inst *const dev_inst,
		uint8_t *tx_data, uint16_t length)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Check if the USART transmitter is busy */
	if (!usart_is_data_buffer_empty(dev_inst)) {
		return STATUS_ERR_BUSY;
	}

	/* Issue asynchronous write */
	_usart_async_write(dev_inst, tx_data, length);
		
	return STATUS_OK;
}

/**
 * \brief 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
//TODO: fix
enum status_code usart_async_read_buffer(struct usart_dev_inst *const dev_inst,
		uint8_t *rx_data, uint16_t length)
{
	/* Sanity check arguments */
	Assert(dev_inst);

	/* Check if the USART receiver is busy */
	if (dev_inst->remaining_rx_buffer_length) {
		return STATUS_ERR_BUSY;
	}

	/* Write the first data from the buffer */
	//TODO: remove??
	//usart_async_read(dev_inst, rx_data);

	/* Store values in device instance */
	//dev_inst->rx_buffer_ptr = ++rx_data;
	//dev_inst->remaining_rx_buffer_length = --length;
	
	//TODO: add
	//_usart_async_read(dev_inst, rx_data, length);

	return STATUS_OK;

}

/**
 * \brief 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
void usart_async_cancel_transmission(struct usart_dev_inst
		*const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Clear the interrupt flag in order to prevent the transmission
	 * complete callback to fire */
	usart_module->INTFLAGS |= SERCOM_USART_TXCIF_bm;

	/* Clear the software transmission buffer */
	dev_inst->remaining_tx_buffer_length = 0;
}

/**
 * \brief 
 *
 *
 * param[]
 * param[]
 * param[]
 *
 * returns 
 * retval  
 */
void usart_async_cancel_reception(struct usart_dev_inst
		*const dev_inst)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Get a pointer to the hardware module instance */
	SERCOM_USART_t *const usart_module = &(dev_inst->hw_dev->USART);

	/* Clear the interrupt flag in order to prevent the receive
	 * complete callback to fire */
	usart_module->INTFLAGS |= SERCOM_USART_RXCIF_bm;

	/* Clear the software reception buffer */
	dev_inst->remaining_rx_buffer_length = 0;
}


