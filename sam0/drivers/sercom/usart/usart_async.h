#ifndef __USART_ASYNC_H__
#define __USART_ASYNC_H__


#include <stdlib.h>
#include <usart.h>



uint8_t _sercom_get_module_irq_index(struct usart_dev_inst *const dev_inst);


/**
 * \name Callback Management
 * {@
 */
void usart_async_register_callback(struct usart_dev_inst *const dev_inst,
		const usart_async_callback_t *const callback_func,
		enum usart_callback_type callback_type);

void usart_async_unregister_callback(struct usart_dev_inst *dev_inst,
		enum usart_callback_type callback_type);

enum status_code usart_async_enable_callback(struct usart_dev_inst *const dev_inst,
		enum usart_callback_type callback_type);

enum status_code usart_async_disable_callback(struct usart_dev_inst *const dev_inst,
		enum usart_callback_type callback_type);

/**
 * @}
 */


/**
 * \name Writing and reading
 * {@
 */
enum status_code usart_async_write(struct usart_dev_inst *const dev_inst,
		const uint16_t tx_data);

enum status_code usart_async_read(struct usart_dev_inst *const dev_inst,
		uint16_t *const rx_data);

enum status_code usart_async_write_buffer(struct usart_dev_inst *const dev_inst,
		uint8_t *tx_data, uint16_t length);

enum status_code usart_async_read_buffer(struct usart_dev_inst *const dev_inst,
		uint8_t *rx_data, uint16_t length);

void usart_async_cancel_transmission(struct usart_dev_inst
		*const dev_inst);

void usart_async_cancel_reception(struct usart_dev_inst
		*const dev_inst);

/**
 * @}
 */

/**
 * \brief Check if the whole buffer has been transmitted
 * \ref usart_write_buffer()\ \ref usart_read_buffer()
 */
static inline bool usart_async_is_buffer_transmitted(struct usart_dev_inst *dev_inst)
{
#ifdef USART_ASYNC
//		return flag & !(dev_inst->remaining_tx_buffer_length);
#endif	
//		return flag;
}

/**
 * \brief Check if the whole buffer has been received
 * \ref usart_write_buffer()\ \ref usart_read_buffer()
 */
static inline bool usart_async_is_buffer_received(struct usart_dev_inst *dev_inst)
{	
#ifdef USART_ASYNC 
//	return !(dev_inst->remaining_rx_buffer_length);
#endif
//	return flag;
}

#endif /* __USART_ASYNC_H__ */
