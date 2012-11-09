#include <sercom_interrupts.h>

uint8_t _sercom_get_current_irq_index(void)
{
	/* Find in some core register */
}


/*
void SERCOM0_Handler(void) {
	sercom_handler(0);
}
void SERCOM1_Handler(void) {
	sercom_handler(1);
}
void SERCOM2_Handler(void) {
	sercom_handler(2);
}
void SERCOM3_Handler(void) {
	sercom_handler(3);
}
*/


/* Interrupt Service Routine */
void SERCOM_Handler(void)
{
	uint8_t instance = _sercom_get_current_irq_index();
	uint16_t callback_status;

	/* Sercom mode is contained in the first byte of dev_inst */
	uint8_t mode = *(uint8_t *)_sercom_instances[instance];

	switch(mode) {
		case SERCOM_MODE_SPI:
		{
			/* Device instance */
			//struct spi_async_dev_inst dev_inst =
			//(struct spi_async_dev_inst *)mode;
			/* TODO: Implementation for the SPI async driver */
			break;
		}
		case SERCOM_MODE_USART:
		{
			struct usart_dev_inst *dev_inst = (struct usart_dev_inst *)_sercom_instances[instance];

			/* Read and mask interrupt flag register */
			callback_status =  dev_inst->hw_dev->USART.INTFLAGS
					&dev_inst->callback_reg_mask
					&dev_inst->callback_enable_mask;

			if (callback_status & SERCOM_USART_RXCIF_bm){
				dev_inst->callback[USART_CALLBACK_TYPE_BUFFER_TRANSMITTED](dev_inst);
			}

			if (callback_status & SERCOM_USART_TXCIF_bm){
				dev_inst->callback[USART_CALLBACK_TYPE_BUFFER_RECEIVED](dev_inst);
			}

			if (callback_status & SERCOM_USART_DREIF_bm){
				dev_inst->callback[USART_CALLBACK_TYPE_BUFFER_EMPTY](dev_inst);
			}

//			if (dev_inst->hw_dev.STATUS &
//					SERCOM_USART)
			break;
		}
		case SERCOM_MODE_I2C:
		{
			//struct usart_async_dev_inst dev_inst =
			//(struct twi_async_dev_inst *)mode;
			/* TODO: Implementation for the TWI async driver */
			break;
		}
		default:
		{
			Assert(false);
		}
	}
/*
	if (_sercom_instances[index] != NULL){
		if (dev_inst->hw_dev.STATUS & USART_RECEIVE_COMPLETE_IF_bm)

	} else {
		Assert(false);
	}
	*/
}
