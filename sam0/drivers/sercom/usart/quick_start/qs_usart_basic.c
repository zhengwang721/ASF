#ifdef USART_ASYNC
#  include <usart_async.h>
#else
#  include <usart.h>
#endif
int main (void) 
{
	struct usart_dev_inst dev_inst;
	struct usart_conf config_struct;

	usart_async_callback_t *callback_function;

	usart_get_config_defaults(&config_struct);
	usart_init(&dev_inst,&SERCOM0,&config_struct);

        //usart_async_register_callback(&dev_inst, &callback_function, USART_CALLBACK_TYPE_RX_COMPLETED);	

}

