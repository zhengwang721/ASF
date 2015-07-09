/**
 *
 * \file
 *
 * \brief SAM SLCD Quick Start.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>

struct dma_resource example_resource_abm;
struct dma_resource example_resource_acm;
COMPILER_ALIGNED(16)
DmacDescriptor example_descriptor_acm ;
DmacDescriptor example_descriptor_abm ;
struct usart_module usart_instance;

/*Charactor map 0-9,A-Z*/
const uint32_t charactor_map[] = {
	0x2e74,0x440,0x23c4,0x25c4,0x5e0,0x25a4,0x27a4,0x444,0x27e4,0x25e4, /*0-9*/
	0x7e4,0xa545,0x2224,0xa445,0x23a4,0x3a4,0x2724, /*A-G*/
	0x7e0,0xa005,0x2640,0x12b0,0x2220,0x678,0x1668, /*H-N*/
	0x2664,0x3e4,0x3664,0x13e4,0x25a4,0x8005,/*O-T*/
	0x2660,0xa30,0x1e60,0x1818,0x8018,0x2814/*U-Z*/
};

/* HELLO ATMEL map */
uint32_t display_array[11]={0x7e0,0x23a4,0x2220,0x2220,0x2664,0,0x7e4,0x8005,0x678,0x23a4,0x2220};//HELLO ATMEL
#define DISPLAY_ARRAY_SIZE (sizeof(display_array)/sizeof(uint32_t))

/*ICON COM/SEG map */
#define C42412A_ICON_USB            1, 1
#define C42412A_ICON_BAT            0, 0
#define C42412A_ICON_ATMEL          0, 1

/**
 *  Configure serial console.
 */
static void configure_console(void)
{
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	config_usart.baudrate    = 38400;
	config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	stdio_serial_init(&usart_instance, EDBG_CDC_MODULE, &config_usart);
	usart_enable(&usart_instance);
}

static void configure_dma_acm(void)
{

	struct dma_resource_config rx_config;

	dma_get_config_defaults(&rx_config);

	rx_config.peripheral_trigger = SLCD_DMAC_ID_ACMDRDY;
	rx_config.trigger_action = DMA_TRIGGER_ACTON_BEAT;

	dma_allocate(&example_resource_acm, &rx_config);

	struct dma_descriptor_config acm_descriptor_config;

	dma_descriptor_get_config_defaults(&acm_descriptor_config);

	acm_descriptor_config.beat_size = DMA_BEAT_SIZE_WORD;
	acm_descriptor_config.src_increment_enable = true;
	acm_descriptor_config.block_transfer_count = DISPLAY_ARRAY_SIZE;
	acm_descriptor_config.step_selection = DMA_STEPSEL_SRC;
	acm_descriptor_config.dst_increment_enable = false;
	acm_descriptor_config.source_address = (uint32_t)display_array+sizeof(display_array);
	acm_descriptor_config.destination_address = (uint32_t)&SLCD->CMDATA.reg;
    acm_descriptor_config.next_descriptor_address =  (uint32_t)&example_descriptor_acm;

	dma_descriptor_create(&example_descriptor_acm, &acm_descriptor_config);

	dma_add_descriptor(&example_resource_acm, &example_descriptor_acm);
	dma_start_transfer_job(&example_resource_acm);
}
#if 0
static void configure_dma_abm(void)
{

	struct dma_resource_config rx_config;

	dma_get_config_defaults(&rx_config);

	rx_config.peripheral_trigger = AES_DMAC_ID_RD;
	rx_config.trigger_action = DMA_TRIGGER_ACTON_BLOCK;

	dma_allocate(&example_resource_rx, &rx_config);

	struct dma_descriptor_config rx_descriptor_config;

	dma_descriptor_get_config_defaults(&rx_descriptor_config);

	rx_descriptor_config.beat_size = DMA_BEAT_SIZE_WORD;
	rx_descriptor_config.src_increment_enable = false;
	rx_descriptor_config.block_transfer_count = AES_EXAMPLE_REFBUF_SIZE;
	rx_descriptor_config.source_address = (uint32_t)&(AES->INDATA);
	rx_descriptor_config.destination_address =
		(uint32_t)output_data + sizeof(output_data);

	dma_descriptor_create(&example_descriptor_rx, &rx_descriptor_config);

	dma_add_descriptor(&example_resource_rx, &example_descriptor_rx);
}
#endif
int main(void)
{
	struct slcd_config config;

	system_init();
	configure_console();
	delay_init();
	
	/* Turn on the backlight. */
	port_pin_set_output_level(SLCD_BACLKLIGHT,true);

	printf("SLCD example starts\r\n");

	slcd_get_config_defaults(&config);
	slcd_init(&config);
	slcd_set_contrast(0x8);

	configure_dma_acm();

	slcd_enable();

	slcd_set_display_memory();
	delay_s(1);
	slcd_clear_display_memory();
	slcd_set_pixel(C42412A_ICON_USB);
	slcd_set_pixel(C42412A_ICON_BAT);
	slcd_set_pixel(C42412A_ICON_ATMEL);
	delay_s(2);

	slcd_character_map_set(SLCD_AUTOMATED_CHAR_START_FROM_BOTTOM_RIGHT,3);
	for(uint32_t i = 0 ; i < 5 ; i++) {
		slcd_character_write_data(0,4+i*4,charactor_map[10+i],0xFF4002);
	}
	delay_s(2);

	slcd_disable();
	struct slcd_blink_config blink_config;
	slcd_blink_get_config_defaults(&blink_config);
	blink_config.blink_all_seg = false;
	slcd_blink_set_config(&blink_config);

	for(uint32_t i=0; i<4; i++){
		slcd_set_blink_pixel(i,0);
		slcd_set_blink_pixel(i,1);
	}

	slcd_set_frame_counter(SLCD_FRAME_COUNTER_0,true,0x18);
	slcd_enable_frame_counter(SLCD_FRAME_COUNTER_0);

	slcd_enable_blink();
	slcd_enable();
	
	delay_s(2);
	slcd_disable();
	struct slcd_automated_char_config acm_config;
	slcd_automated_char_get_config_default(&acm_config);
	acm_config.order = SLCD_AUTOMATED_CHAR_START_FROM_BOTTOM_RIGHT;
	acm_config.fc = SLCD_FRAME_COUNTER_1;
	acm_config.mode = SLCD_AUTOMATED_CHAR_SCROLL;
	acm_config.seg_line_num = 3;
	acm_config.start_seg_line = 4;
	acm_config.row_digit_num = 5;
	acm_config.digit_num = 5;
	acm_config.scrolling_step = sizeof(display_array) - 5 + 1;
	acm_config.com_line_num = 4;
	acm_config.data_mask = 0xFF4002;
	slcd_automated_char_set_config(&acm_config);

	slcd_set_frame_counter(SLCD_FRAME_COUNTER_1,true,0x1E);
	slcd_enable_frame_counter(SLCD_FRAME_COUNTER_1);
	
	slcd_enable_automated_character();
	slcd_enable();
    while(1){

    }

}

