#ifndef __DMA_IF_H
#define __DMA_IF_H

//#include "typedefs.h"
#include <stdint.h>

typedef void (*dma_transfer_complete)(void *ptr,uint32_t bytes_transfered);

typedef enum _edma_ch_num {
	CHANNEL_0 = 0,
	CHANNEL_1,
	CHANNEL_2,
	CHANNEL_3
}dma_ch_num;


typedef enum _dma_peripheral_num {
#ifndef CFG_NMC2000B0	
	MEMORY_DMA_PERIPHERAL	= 0,
	UART0RX_DMA_PERIPHERAL,
	UART0TX_DMA_PERIPHERAL,
	UART1RX_DMA_PERIPHERAL,
	UART1TX_DMA_PERIPHERAL,
	SPI0RX_DMA_PERIPHERAL,
	SPI0TX_DMA_PERIPHERAL,
	TIMER0_DMA_PERIPHERAL = 10,
	DUALTIMER0_DMA_PERIPHERAL,
	SPI1RX_DMA_PERIPHERAL,
	SPI1TX_DMA_PERIPHERAL,
	I2C0RX_DMA_PERIPHERAL,
	I2C0TX_DMA_PERIPHERAL,
#else
	MEMORY_DMA_PERIPHERAL	= 0,
	UART0RX_DMA_PERIPHERAL,
	UART0TX_DMA_PERIPHERAL,
	UART1RX_DMA_PERIPHERAL,
	UART1TX_DMA_PERIPHERAL,
	SPI0RX_DMA_PERIPHERAL,
	SPI0TX_DMA_PERIPHERAL,
	SPI1RX_DMA_PERIPHERAL,
	SPI1TX_DMA_PERIPHERAL,
	I2C0RX_DMA_PERIPHERAL,
	I2C0TX_DMA_PERIPHERAL,
	I2C1RX_DMA_PERIPHERAL,
	I2C1TX_DMA_PERIPHERAL,
	DUALTIMER0_DMA_PERIPHERAL = 15,
	TIMER0_DMA_PERIPHERAL,
#endif
}dma_peripheral_num;

typedef struct _transfer_config {
	uint32_t	start_addr;
	uint32_t	length;
	uint8_t	incr_addr;
	//uint8_t	block_mode;
	uint8_t	periph_mode;
	uint8_t	periph_num;
	uint8_t periph_delay;
	uint8_t outstanding_mode;
	uint16_t	burst_size;
	uint16_t wait_limit;
}dma_config;


typedef struct _transfer_desc {
	dma_config	src_config;
	dma_config	dest_config;
	struct _transfer_desc *next;
}transfer_desc;

typedef struct _block_transfer {
	uint8_t enable;
	uint16_t frame_width;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t auto_retry;
	uint8_t swap;
}block_transfer;

typedef struct _dma_channel_config {
	uint8_t	periph_mode;
	uint8_t	periph_num;
	uint8_t periph_delay;
	uint8_t outstanding_mode;
	uint8_t	num_rd_token;
	uint8_t num_wr_token;
	uint16_t	burst_size;
	uint16_t wait_limit;
	block_transfer block_config;
}dma_channel_config;

typedef struct _dma_req {
	//dma_config src_config;
	//dma_config dest_config;
	uint8_t channel;
	uint8_t	tr_start;
	transfer_desc	desc;
	block_transfer block_mode;
	uint8_t joint_mode;
	dma_transfer_complete dma_cb;
	void *cb_context;
}dma_req;

void dma_init(uint8_t channel);
void dma_test(void);
void submit_dma_transfer_req(dma_req *req);
#endif 	//__DMA_IF_H
