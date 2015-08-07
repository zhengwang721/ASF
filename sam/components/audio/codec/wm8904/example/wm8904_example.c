/**
 * \file
 *
 * \brief WM8904 example for SAM.
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

/**
 * \mainpage WM8904 Example
 *
 * \section Purpose
 *
 * This example uses the Synchronous Serial Controller (SSC) of SAM devices
 * to output an audio stream through the on-board WM8904 CODEC.
 *
 * \section Requirements
 *
 * This example can be used with SAM Xplained board kits with the on-board WM8904
 * CODEC.
 *
 * \section Description
 * This program plays sound from PC via Line-In. The audio stream is
 * sent through the SSC interface connected to the on-board WM8904, enabling 
 * the sound to be audible using a pair of headphones.
 *
 * The code can be roughly broken down as follows:
 * - Enable the clock.
 * - Initialize and configure the Codec.
 * - Configure and enable the SSC.
 * - Configure and enable the XDMA.
 *
 * \section Usage
 *
 * -# Build the program and download it into the Xplained board.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 baud rate
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *    \code
	-- WM8904 example xxx --
	-- SAMxx-xx
	-- Compiled: xxx xx xxxx xx:xx:xx --
\endcode
 */

#include <asf.h>

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

#define STRING_EOL    "\r"
#define STRING_HEADER "-- WM8904 Example --\r" \
"-- "BOARD_NAME" --\r" \
"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/** Wav feature. */
#define SAMPLE_RATE             (48000)
/** Wav slot per frame */
#define SLOT_BY_FRAME           (1)
/** Bits per slot */
#define BITS_BY_SLOT            (16)

/** Micro-block length for single transfer  */
#define MICROBLOCK_LEN       0x1000

/** XDMA channel used in this example. */
#define XDMA_CH_SSC_RX    0
#define XDMA_CH_SSC_TX    1

/** XDMA Descriptor */
#define TOTAL_BUFFERS    4

/** */
static lld_view1 linklist_write[TOTAL_BUFFERS];
static lld_view1 linklist_read[TOTAL_BUFFERS];

static uint16_t AudioBuffer[TOTAL_BUFFERS * MICROBLOCK_LEN * (BITS_BY_SLOT/8)];

/**
 *  \brief Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

static void configure_ssc(void)
{
	clock_opt_t tx_clk_option, rx_clk_option;
	data_frame_opt_t tx_data_frame_option, rx_data_frame_option;

	/* Initialize clock */
	pmc_enable_periph_clk(ID_SSC);

	/* Reset SSC */
	ssc_reset(SSC);

	/* Transmitter clock mode configuration. */
	tx_clk_option.ul_cks = SSC_TCMR_CKS_TK;
	tx_clk_option.ul_cko = SSC_TCMR_CKO_NONE;
	tx_clk_option.ul_cki = 0;
	tx_clk_option.ul_ckg = SSC_TCMR_CKG_CONTINUOUS;
	tx_clk_option.ul_start_sel = SSC_TCMR_START_TF_EDGE;
	tx_clk_option.ul_sttdly = SSC_TCMR_STTDLY(1);
	tx_clk_option.ul_period = SSC_TCMR_PERIOD(0);
	/* Transmitter frame mode configuration. */
	tx_data_frame_option.ul_datlen = BITS_BY_SLOT - 1;
	tx_data_frame_option.ul_msbf = SSC_TFMR_MSBF;
	tx_data_frame_option.ul_datnb = 0;
	tx_data_frame_option.ul_fslen = SLOT_BY_FRAME - 1;
	tx_data_frame_option.ul_fslen_ext = 0;
	tx_data_frame_option.ul_fsos = SSC_TFMR_FSOS_NONE;
	tx_data_frame_option.ul_fsedge = SSC_TFMR_FSEDGE_POSITIVE;
	/* Configure the SSC transmitter to I2S mode. */
	ssc_set_transmitter(SSC, &tx_clk_option, &tx_data_frame_option);

	/* Receiver clock mode configuration. */
	rx_clk_option.ul_cks = SSC_RCMR_CKS_TK;
	rx_clk_option.ul_cko = SSC_RCMR_CKO_NONE;
	rx_clk_option.ul_cki = SSC_RCMR_CKI;
	rx_clk_option.ul_ckg = SSC_RCMR_CKG_CONTINUOUS;
	rx_clk_option.ul_start_sel = SSC_RCMR_START_RF_EDGE;
	rx_clk_option.ul_sttdly = SSC_RCMR_STTDLY(1);
	rx_clk_option.ul_period = SSC_RCMR_PERIOD(0);
	/* Receiver frame mode configuration. */
	rx_data_frame_option.ul_datlen = BITS_BY_SLOT - 1;
	rx_data_frame_option.ul_msbf = SSC_RFMR_MSBF;
	rx_data_frame_option.ul_datnb = 0;
	rx_data_frame_option.ul_fslen = SLOT_BY_FRAME - 1;
	rx_data_frame_option.ul_fslen_ext = 0;
	rx_data_frame_option.ul_fsos = SSC_TFMR_FSOS_NONE;
	rx_data_frame_option.ul_fsedge = SSC_TFMR_FSEDGE_POSITIVE;
	/* Configure the SSC transmitter to I2S mode. */
	ssc_set_receiver(SSC, &rx_clk_option, &rx_data_frame_option);

	/* Disable transmitter first */
	ssc_disable_tx(SSC);
	ssc_disable_rx(SSC);

	/* Disable All Interrupt */
	ssc_disable_interrupt(SSC, 0xFFFFFFFF);
}

static void configure_xdma(void)
{
    uint16_t *src;
    uint8_t i;
    
	xdmac_channel_config_t xdmac_channel_cfg = {0};

	/* Initialize and enable DMA controller */
	pmc_enable_periph_clk(ID_XDMAC);

	/* Initialize channel config */
	xdmac_channel_cfg.mbr_cfg = XDMAC_CC_TYPE_PER_TRAN
								| XDMAC_CC_MBSIZE_SINGLE
								| XDMAC_CC_DSYNC_PER2MEM
								| XDMAC_CC_CSIZE_CHK_1
								| XDMAC_CC_DWIDTH_HALFWORD
								| XDMAC_CC_SIF_AHB_IF1
								| XDMAC_CC_DIF_AHB_IF0
								| XDMAC_CC_SAM_FIXED_AM
								| XDMAC_CC_DAM_INCREMENTED_AM
								| XDMAC_CC_PERID(33);
 	xdmac_configure_transfer(XDMAC, XDMA_CH_SSC_RX, &xdmac_channel_cfg);

	/* Initialize linked list descriptor */
    src = &AudioBuffer[0];
    for(i = 0; i < TOTAL_BUFFERS; i++) {
	    linklist_read[i].mbr_ubc = XDMAC_UBC_NVIEW_NDV1
									| XDMAC_UBC_NDE_FETCH_EN
									| XDMAC_UBC_NSEN_UPDATED
									| XDMAC_CUBC_UBLEN(MICROBLOCK_LEN);
	    linklist_read[i].mbr_sa  = (uint32_t)&(SSC->SSC_RHR);
	    linklist_read[i].mbr_da = (uint32_t)(src);
	    if ( i == (TOTAL_BUFFERS - 1)) {
		    linklist_read[i].mbr_nda = (uint32_t)&linklist_read[0];
	    } else {
		    linklist_read[i].mbr_nda = (uint32_t)&linklist_read[i + 1];
	    }
	    src += MICROBLOCK_LEN ;
    }

	xdmac_channel_set_descriptor_control(XDMAC, XDMA_CH_SSC_RX, XDMAC_CNDC_NDVIEW_NDV1 |
										XDMAC_CNDC_NDE_DSCR_FETCH_EN |
										XDMAC_CNDC_NDSUP_SRC_PARAMS_UPDATED |
										XDMAC_CNDC_NDDUP_DST_PARAMS_UPDATED);
	xdmac_channel_set_descriptor_addr(XDMAC, XDMA_CH_SSC_RX, (uint32_t)(&linklist_read[0]), 0);

	xdmac_enable_interrupt(XDMAC, XDMA_CH_SSC_RX);
	xdmac_channel_enable_interrupt(XDMAC, XDMA_CH_SSC_RX, XDMAC_CIE_LIE);

    xdmac_channel_cfg.mbr_cfg = XDMAC_CC_TYPE_PER_TRAN
								| XDMAC_CC_MBSIZE_SINGLE
								| XDMAC_CC_DSYNC_MEM2PER
								| XDMAC_CC_CSIZE_CHK_1
								| XDMAC_CC_DWIDTH_HALFWORD
								| XDMAC_CC_SIF_AHB_IF0
								| XDMAC_CC_DIF_AHB_IF1
								| XDMAC_CC_SAM_INCREMENTED_AM
								| XDMAC_CC_DAM_FIXED_AM
								| XDMAC_CC_PERID(32);
    xdmac_configure_transfer(XDMAC, XDMA_CH_SSC_TX, &xdmac_channel_cfg);

    src = &AudioBuffer[0];
	for(i = 0; i < TOTAL_BUFFERS; i++) {
	    linklist_write[i].mbr_ubc = XDMAC_UBC_NVIEW_NDV1
									| XDMAC_UBC_NDE_FETCH_EN
									| XDMAC_UBC_NSEN_UPDATED
									| XDMAC_CUBC_UBLEN(MICROBLOCK_LEN);
	    linklist_write[i].mbr_sa = (uint32_t)(src);
	    linklist_write[i].mbr_da = (uint32_t)&(SSC->SSC_THR);
	    if ( i == (TOTAL_BUFFERS - 1 )) {
		    linklist_write[i].mbr_nda = (uint32_t)&linklist_write[0];
	    } else {
		    linklist_write[i].mbr_nda = (uint32_t)&linklist_write[i+1];
	    }
	    src += MICROBLOCK_LEN;
    }

	xdmac_channel_set_descriptor_control(XDMAC, XDMA_CH_SSC_TX, XDMAC_CNDC_NDVIEW_NDV1
										| XDMAC_CNDC_NDE_DSCR_FETCH_EN
										| XDMAC_CNDC_NDSUP_SRC_PARAMS_UPDATED
										| XDMAC_CNDC_NDDUP_DST_PARAMS_UPDATED);
	xdmac_channel_set_descriptor_addr(XDMAC, XDMA_CH_SSC_TX, (uint32_t)(&linklist_write[0]), 0);

	xdmac_enable_interrupt(XDMAC, XDMA_CH_SSC_TX);
	xdmac_channel_enable_interrupt(XDMAC, XDMA_CH_SSC_TX, XDMAC_CIE_LIE);

	/*Enable XDMA interrupt */
	NVIC_ClearPendingIRQ(XDMAC_IRQn);
	NVIC_SetPriority(XDMAC_IRQn ,1);
	NVIC_EnableIRQ(XDMAC_IRQn);

}

static void configure_codec(void)
{
	uint16_t data = 0;
	/* check that WM8904 is present */
	wm8904_write_register(WM8904_SW_RESET_AND_ID, 0xFFFF);
	data = wm8904_read_register(WM8904_SW_RESET_AND_ID);
	if(data != 0x8904) {
		printf("WM8904 not found!\n\r");
		while(1);
	}

	wm8904_write_register(WM8904_BIAS_CONTROL_0, 0x0008);
	wm8904_write_register(WM8904_VMID_CONTROL_0, 0x0047);
	delay_ms(5);
	wm8904_write_register(WM8904_VMID_CONTROL_0, 0x0043);
	wm8904_write_register(WM8904_BIAS_CONTROL_0, 0x0009);
	wm8904_write_register(WM8904_POWER_MANAGEMENT_0, 0x0003);
	wm8904_write_register(WM8904_POWER_MANAGEMENT_2, 0x0003);
	wm8904_write_register(WM8904_DAC_DIGITAL_1, 0x0000);
	wm8904_write_register(WM8904_ANALOGUE_OUT12_ZC, 0x0000);
	wm8904_write_register(WM8904_CHARGE_PUMP_0, 0x0001);
	wm8904_write_register(WM8904_CLASS_W_0, 0x0001);
	wm8904_write_register(WM8904_FLL_CONTROL_1, 0x0000);
	wm8904_write_register(WM8904_FLL_CONTROL_2, 0x0704);
	wm8904_write_register(WM8904_FLL_CONTROL_3, 0x8000);
	wm8904_write_register(WM8904_FLL_CONTROL_4, 0x1760);
	wm8904_write_register(WM8904_FLL_CONTROL_1, 0x0005);
	delay_ms(5);
	wm8904_write_register(WM8904_CLOCK_RATES_1, 0x0C05);
	wm8904_write_register(WM8904_CLOCK_RATES_0, 0x0000);
	wm8904_write_register(WM8904_CLOCK_RATES_2, 0x4006);
	wm8904_write_register(WM8904_AUDIO_INTERFACE_1, 0x0042);
	wm8904_write_register(WM8904_AUDIO_INTERFACE_2, 0x0008);
	wm8904_write_register(WM8904_AUDIO_INTERFACE_3, 0x0820);
	wm8904_write_register(WM8904_POWER_MANAGEMENT_6, 0x000F);
	delay_ms(5);
	wm8904_write_register(WM8904_ANALOGUE_LEFT_INPUT_0, 0x0010);
	wm8904_write_register(WM8904_ANALOGUE_RIGHT_INPUT_0, 0x0010);
	wm8904_write_register(WM8904_ANALOGUE_LEFT_INPUT_1, 0x0044);
	wm8904_write_register(WM8904_ANALOGUE_RIGHT_INPUT_1, 0x0044);
	wm8904_write_register(WM8904_ANALOGUE_HP_0, 0x0011);
	wm8904_write_register(WM8904_ANALOGUE_HP_0, 0x0033);
	wm8904_write_register(WM8904_DC_SERVO_0, 0x000F);
	wm8904_write_register(WM8904_DC_SERVO_1, 0x00F0);
	delay_ms(100);
	wm8904_write_register(WM8904_ANALOGUE_HP_0, 0x0077);
	wm8904_write_register(WM8904_ANALOGUE_HP_0, 0x00FF);
	wm8904_write_register(WM8904_ANALOGUE_OUT1_LEFT, 0x00B9);
	wm8904_write_register(WM8904_ANALOGUE_OUT1_RIGHT, 0x00B9);
	delay_ms(100);
}

int main(void)
{
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();

	/* Initialize the console UART. */
	configure_console();
	printf(STRING_HEADER);

	/* Initialize WM8904 */
	if (wm8904_init() != TWIHS_SUCCESS) {
		printf("-E-\tWM8904 initialization failed.\r");
		while (1) {
			/* Capture error */
		}
	}
	
	/* Configure CODEC */
	configure_codec();

	/* Configure SSC */
	configure_ssc();

	/* Configure XDMA */
	configure_xdma();

    /* Enable the DAC master clock */
	pmc_pck_set_prescaler(PMC_PCK_2, PMC_MCKR_PRES_CLK_1);
	pmc_pck_set_source(PMC_PCK_2, PMC_MCKR_CSS_SLOW_CLK);
	pmc_enable_pck(PMC_PCK_2);

    /* Start playing */
	ssc_enable_rx(SSC);
	xdmac_channel_enable(XDMAC, XDMA_CH_SSC_RX);
	delay_ms(300);
	ssc_enable_tx(SSC);
	xdmac_channel_enable(XDMAC, XDMA_CH_SSC_TX);

	while(1);
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
