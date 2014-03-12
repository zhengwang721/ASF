/**
 * \file
 *
 * \brief  Pulse Density Modulation Interface Controller Example
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
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

/**
 * \mainpage  Pulse Density Modulation Interface Controller Example
 *
 * \section Purpose
 * This example demonstrates the use of the PDMIC module and reads data from a PDM microphone.
 *
 *
 * \section Requirements
 *
 * This example can be used on any SAMG board.
 * A PDM microphone must be connected on PA9 (PDMDAT) and PA10 (PDMCLK).
 *
 * \section Description
 * This example will continuously read 30 ms blocks of PDM data from the microphone via DMA and
 * place the filtered PCM data into the two buffers in the audio_buffer array. This means
 * the application can process 30 ms of data at a time, while the module continues to read
 * from the microphone.
 *
 * Configuration of the PDM module is as follows:
 * - 1MHz PDM clock
 * - Oversampling ratio 64
 * - Data size 16 bits
 * - All digital signal processing enabled
 * 
 *
 * \section Usage
 * When running the example, 30 ms of data from the connected PDM microphone will be read into the
 * audio_buffer array at a time.
 *
 */
#include <asf.h>

/*
 * Size of each PCM sample.
 * 1 MHz PDM clock, decimation of 64 -> Sampling rate 16kHz
 * 480 samples gives us 30 milliseconds of data.
 */
#define PCM_BUF_SIZE 480

/* Prescaler value for 1MHz PDM clock when running on 48MHz */
#define PRESCALER (48000000/(2*1000000))-1

/* Globally available PDMIC module struct */
struct pdm_instance pdm;
/* Two buffers to store the PCM data */
int16_t audio_buffer[2][PCM_BUF_SIZE];
/* Global variable to say which of the buffers should be used */
uint8_t select;
 
 
 /* Function to initalize the PDC transfer */
 static void init_pdc(void)
 {
	 struct pdc_packet packet;
	 Pdc *pdmic_pdc;
	 pdmic_pdc = pdmic_get_pdc_base(PDMIC0);
	 
	 /* Set buffer address and size */
	 packet.ul_addr = (uint32_t)&(audio_buffer[select][0]);
	 packet.ul_size = PCM_BUF_SIZE;

	 /* Initialize and enable PDC */
	 pdc_rx_init(pdmic_pdc, &packet, NULL);
	 pdc_enable_transfer(pdmic_pdc, PERIPH_PTCR_RXTEN);
 }
 
/* Callback for PDC transfer complete */
static void buffer_callback(const struct pdm_instance *const module)
{
	/* Toggle which buffer to fill */
	select ^= 1;
	/* Configure PDC to fill next buffer */
	init_pdc();
	/* Re-enable interrupt */
	pdm_enable_interrupt(&pdm, PDMIC_INTERRUPT_SRC_TRANSFER_END);
}

/* Function to initialize callback */
static void init_callback(void)
{
	/* Register callback for PDC transfer end */
	pdm_register_callback(&pdm, buffer_callback, PDM_CALLBACK_TRANSFER_END);
	/* Enable callback for PDC transfer end */
	pdm_enable_callback(&pdm, PDM_CALLBACK_TRANSFER_END);
	/* Enable interrupt for PDC transfer end */
	pdm_enable_interrupt(&pdm, PDMIC_INTERRUPT_SRC_TRANSFER_END);
	/* Enable IRQ */
	NVIC_EnableIRQ((IRQn_Type) PDMIC0_IRQn);
}

/* Function to setup the PDM module */
static void init_pdm(void)
{
	/* PDM configuration structure */
	struct pdm_config conf;
	/* Get default configuration */
	pdm_get_config_default(&conf);

	/* Prescaler for 1 MHz PDM clock */
	conf.prescal = PRESCALER;
	/* Set gain to 1 - if not, all conversions are 0 */
	conf.gain = 1;
	/* Oversampling ratio */
	conf.oversampling_ratio = PDMIC_OVERSAMPLING_RATIO_64;
	/* Data size */
	conf.conver_data_size = PDMIC_CONVERTED_DATA_SIZE_16;
	
	/* Initialize PDMIC0 with configuration */
	pdm_init(&pdm, PDMIC0, &conf);	
	
	/* Mux PDM pins */
	pio_set_peripheral(PIOA, PIO_PERIPH_B, PIO_PA10 | PIO_PA9);

}

int main (void)
{
	/* Configure PDM module*/
	init_pdm();
	/* Configure PDC */
	init_pdc();
	/* Configure callback */
	init_callback();

	Enable_global_interrupt();

	/* Enable PDM module */
	pdm_enable(&pdm);
	
	while(1) {
		/* PDM Data is continuously being read into audio_buffer */
	}
	
}
