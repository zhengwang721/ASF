#ifndef __SERCOM_INTERRUPTS__
#define __SERCOM_INTERRUPTS__

#define SERCOM_INSTANCES 4
#include <usart.h>
#include <sercom.h>


/*
struct _sercom_dev_inst {
	union {
		//struct spi_dev_inst spi;
		struct usart_dev_inst usart;
		//struct i2c_dev_inst i2c;		
	};
};




/* Lookup table containing sercom instances which have registered callback functions */
uint32_t _sercom_instances[SERCOM_INSTANCES];

void SERCOM_Handler(void);


#endif /* __SERCOM_INTERRUPTS__ */
