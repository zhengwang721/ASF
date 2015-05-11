/**
 *
 * \file
 *
 * \brief SAM Segment Liquid Crystal Display(SLCD) Controller.
 *
 * This file defines a useful set of functions for the SLCD on SAM devices.
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

#include <slcd.h>
#include <system.h>


#if !defined(SLCD_CLOCK_SOURCE)
#  warning  SLCD_CLOCK_SOURCE is not defined, assuming SLCD_CLOCK_SELECTION_ULP32K.
#  define SLCD_CLOCK_SOURCE SLCD_CLOCK_SELECTION_ULP32K
#endif

/**
 * \brief Initializes SLCD configurations struct to defaults.
 *
 * Initailizes SLCD configuration struct to predefined safe default settings.
 *
 * \param[in] config Pointer to an instance of \ref struct slcd_config
 *
 */
void slcd_get_config_defaults(struct slcd_config *config)
{
	if (!config) {
		return ;
	}
	config->duty = SLCD_DUTY_STATIC;
	config->bias = SLCD_BIAS_STATIC;
	config->clock_presc = SLCD_CLOCK_PRESC_16;
	config->clock_div = SLCD_CLOCK_DIV_1;
	config->xvlcd = SLCD_VLCD_INTERNAL;
	config->run_in_standby = false;
	config->waveform_mode = SLCD_LOW_POWER_WAVEFORM_MODE;
	config->dmfcs = SLCD_DISPLAY_MEMORY_UPDATE_SEL_FC0;
	config->ref_refresh_freq = SLCD_REF_REFRESH_FREQ_2KHZ;
	config->power_refresh_freq = SLCD_POWER_REFRESH_FREQ_2KHZ;
	config->low_resistance_duration = 0;
	config->enable_low_resistance = false;
	config->bias_buffer_duration = 0;
	config->enable_bias_buffer = false;
	config->enable_ext_bias_capacitor = false;
	config->power_mode = SLCD_POWER_MODE_AUTO;
}

/**
 * \brief Initialize SLCD module.
 *
 * \param[in] config  Pointer to an SLCD configuration structure
 *
 *  \note SLCD cannot be Initialized while it is enabled.
 *
 * \return Status of the configuration procedure.
 * \retval STATUS_OK SLCD configuration went successful
 * \retval STATUS_ERR_INVALID_ARG  If an invalid configuration was supplied
 */
enum status_code slcd_init(struct slcd_config *const config)
{
	if (!config) {
		return STATUS_ERR_INVALID_ARG;
	}
	slcd_disable();
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, MCLK_APBCMASK_SLCD);
	
	/* Select SLCD clock */
	OSC32KCTRL->SLCDCTRL.reg = SLCD_CLOCK_SOURCE;

	slcd_reset();
	
	SLCD->CTRLA.reg = SLCD_CTRLA_DUTY(config->duty) | SLCD_CTRLA_BIAS(config->bias)
					 | SLCD_CTRLA_PRESC(config->clock_presc) | SLCD_CTRLA_CKDIV(config->clock_div) 
					 | (config->xvlcd << SLCD_CTRLA_XVLCD_Pos)
					 | (config->run_in_standby << SLCD_CTRLA_RUNSTDBY_Pos)
					 | (config->waveform_mode << SLCD_CTRLA_WMOD_Pos)
					 | SLCD_CTRLA_DMFCS(config->dmfcs)
					 | SLCD_CTRLA_RRF(config->ref_refresh_freq)
					 | SLCD_CTRLA_PRF(config->power_refresh_freq)
					 | (config->waveform_mode << SLCD_CTRLA_WMOD_Pos);
   SLCD->CTRLB.reg = SLCD_CTRLB_BBD(config->bias_buffer_duration)
  					| (config->enable_bias_buffer << SLCD_CTRLB_BBEN_Pos)
  					| SLCD_CTRLB_LRD(config->low_resistance_duration)
  					| (config->enable_low_resistance << SLCD_CTRLB_LREN_Pos);

   while (slcd_get_vlcd_ready_status()) {
   }
   SLCD->CTRLC.reg |= SLCD_CTRLC_LPPM(config->power_mode);
 
   return STATUS_OK;
}

/**
 * \brief Enables the SLCD module.
 *
 * Enables the SLCD module once it has been configured, ready for use. Most
 * module configuration parameters cannot be altered while the module is enabled.
 */

void slcd_enable(void)
{
	SLCD->CTRLA.reg |= SLCD_CTRLA_ENABLE;
	
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}

/**
 * \brief Disables the SLCD module.
 *
 * Disables the SLCD module.
 */
void slcd_disable(void)
{
	SLCD->CTRLA.reg &= ~(SLCD_CTRLA_ENABLE);
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}

/**
 * \brief Check if SLCD module is enabled or not.
 *
 * Check if SLCD module is enabled or not.
 *
 * \return Enable status.
 * \retval true SLCD module is enabled
 * \retval true SLCD module is disabled
 */

bool slcd_is_enabled(void)
{
	return ((SLCD->CTRLA.reg & SLCD_CTRLA_ENABLE) == SLCD_CTRLA_ENABLE)
}

/**
 * \brief Reset the SLCD module.
 *
 * Reset the SLCD module.
 */
void slcd_reset(void)
{
	slcd_disable();
	SLCD->CTRLA.reg |= SLCD_CTRLA_SWRST;
	while (slcd_is_syncing()) {
		/* Wait for synchronization */
	}
}

/**
 * \brief Set the SLCD fine contrast.
 *
 * The LCD contrast is defined by the value of VLCD voltage. The higher is the 
 * VLCD voltage, the higher is the contrast. The software contrast adjustment 
 * is only possible in internal supply mode.
 * In internal supply mode, VLCD is in the range 2.5V to 3.5V. VLCD can be 
 * adjusted with 16 steps, each step is 60 mV.The contrast value can be written
 * at any time.
 *
 * \param[in] contrast Contrast value
 *
 * \return Status of set contrast.
 * \retval STATUS_OK SLCD contrast set successful.
 * \retval STATUS_ERR_INVALID_ARG  SLCD is not working in internal supply mode.
 */
void slcdc_set_contrast(enum slcd_contrast contrast)
{
	
	if (SLCD->CTRLA.bit.XVLCD) {
		return STATUS_ERR_INVALID_ARG;
	}
	uint16_t temp = SLCD->CTRLC.reg;
	
	temp &= ~ SLCD_CTRLC_CTST(0xf);
	temp |= SLCD_CTRLC_CTST(contrast);
	
	SLCD->CTRLC.reg = temp;
	
}

/**
 * \brief Initializes SLCD blink configurations struct to defaults.
 *
 * Initailizes SLCD blink configuration struct to predefined safe default settings.
 *
 * \param[in] config Pointer to an instance of \ref struct slcd_blink_config
 *
 */
void slcd_blink_get_config_defaults(struct slcd_blink_config *blink_config)
{
	if (!blink_config) {
		return ;
	}
	blink_config->fc = SLCD_FRAME_COUNTER_0;
	blink_config->blink_mode = SLCD_BLINK_ALL_SEG;
	blink_config->blink_seg0_mask = 0;
	blink_config->blink_seg1_mask = 0;
}

/**
 * \brief Set SLCD blink mode.
 *
 * Set SLCD blink mode.
 *
 *  \note SLCD blink cannot be set while module or blink is enabled.
 *
 * \param[in] config  Pointer to an SLCD blink configuration structure
 *
 * \return Status of the configuration procedure.
 * \retval STATUS_OK SLCD blink configuration went successful
 * \retval STATUS_ERR_INVALID_ARG  If blink  configuration failed
 */

enum status_code  slcd_blink_set_config(struct slcd_blink_config *const blink_config)
{
	if (!blink_config) {
		return STATUS_ERR_INVALID_ARG;
	}

	SLCD->BCFG.reg = (blink_config->blink_mode << SLCD_BCFG_MODE_Pos)
					| SLCD_BCFG_FCS(blink_config->fc)
					| SLCD_BCFG_BSS0(blink_config->blink_seg0_mask)
					| SLCD_BCFG_BSS1(blink_config->blink_seg1_mask);
	return STATUS_OK;
}

/**
 * \brief Enable the specified pixel/segment in the SLCD display memory.
 *
 * \param[in] pix_com Pixel/segment COM coordinate,within [0-7]
 * \param[in] pix_seg Pixel/segment SEG coordinate within [0-43]
 */
 void slcd_set_pixel(
		uint8_t pix_com,
		uint8_t pix_seg)
{
	if ((pix_com < SLCD_MAX_COM) &&
			(pix_seg < SLCD_MAX_SEG)) {
		uint64_t register_value = *(&(SLCD->SDATAL0.reg) + pix_com*4) 
				| (*(&(SLCD->SDATAH0.reg) + pix_com*4) << 32) ;
		register_value |= ((uint64_t)1 << pix_seg);
		
		*(&(SLCD->SDATAL0.reg) + pix_com*4) = register_value & SLCD_SDATAL0_MASK;
		*(&(SLCD->SDATAH0.reg) + pix_com*4) = (register_value >> 32) & SLCD_SDATAL0_MASK;
	}
}

/**
 * \brief Disable the specified pixel/segment in the SLCD display memory.
 *
 * \param[in] pix_com Pixel/segment COM coordinate
 * \param[in] pix_seg Pixel/segment SEG coordinate
 */
 void slcd_clear_pixel(uint8_t pix_com, uint8_t pix_seg)
{
	if ((pix_com < SLCD_MAX_COM) &&
			(pix_seg < SLCD_MAX_SEG)) {
		uint64_t register_value = *(&(SLCD->SDATAL0.reg) + pix_com*4) 
				| (*(&(SLCD->SDATAH0.reg) + pix_com*4) << 32) ;

		register_value &= ~((uint64_t)1 << pix_seg);
		*(&(SLCD->SDATAL0.reg) + pix_com*4) = register_value & SLCD_SDATAL0_MASK;
		*(&(SLCD->SDATAH0.reg) + pix_com*4) = (register_value >> 32) & SLCD_SDATAL0_MASK;
		
	}
}

/**
 * \brief Set the specified segment in the SLCD display memory.
 *
 * \param[in] pix_seg Pixel/segment SEG coordinate
 * \param[in] byte_offset Byte offset in display memory,
 */
void slcd_set_seg_data(uint8_t seg_data,uint8_t byte_offset)
{
	uint32_t temp = SLCD->ISDATA.reg;
	
	temp &= (~ SLCD_ISDATA_SDATA(0xff)) | (~ SLCD_ISDATA_OFF(0x3f));
	
	temp |= SLCD_ISDATA_SDATA(seg_data)
		 | SLCD_ISDATA_OFF(byte_offset);
	
	SLCD->ISDATA.reg = temp;
}
/**
 * \brief Mask specified segments data.
 *
 * When When specified segments mask is 1, segments data is not written
 * to display memory
 *
 * \param[in] pix_seg Pixel/segment SEG coordinate
 */
void slcd_set_seg_mask(uint8_t seg_mask)
{
	uint32_t temp = SLCD->ISDATA.reg;
	
	temp &= ~ SLCD_ISDATA_SDMASK(0xff);
	temp |= SLCD_ISDATA_SDMASK(seg_mask);
	
	SLCD->ISDATA.reg = temp;
}

/**
 * \brief Initializes SLCD Automated Character configurations struct to defaults.
 *
 * Initailizes SLCD Automated Character configuration struct to predefined safe default settings.
 *
 * \param[in] config Pointer to an instance of \ref struct slcd_automated_char_config
 *
 */
void slcd_automated_char_get_config_default(
		struct slcd_automated_char_config *config)
{
	if (!config) {
		return ;
	}
	config->order = SLCD_AUTOMATED_CHAR_START_FROM_BOTTOM_LEFT;
	config->fc = SLCD_FRAME_COUNTER_0;
	config->mode = SLCD_AUTOMATED_CHAR_SEQ;
	config->seg_line_num = 0;
	config->start_seg_line = 0;
	config->row_digit_num = 1;
	config->digit_num = 0;
	config->scrolling_step = 1;
	config->com_line_num = 1;
	
}

/**
 * \brief Set SLCD Automated Character.
 *
 * Set Automated Character mode.
 *
 *  \note SLCD automated character mode cannot be set while module or 
 * automated character is enabled.
 *
 * \param[in] config  Pointer to an SLCD automated character configuration structure
 *
 * \return Status of the configuration procedure.
 * \retval STATUS_OK SLCD automated character configuration went successful
 * \retval STATUS_ERR_INVALID_ARG  If automated character  configuration failed
 */
enum status_code slcd_automated_char_set_config(
		struct slcd_automated_char_config *const config)
{
	if (!config) {
		return STATUS_ERR_INVALID_ARG;
	}

	SLCD->CMCFG.reg = SLCD_CMCFG_NSEG(config->seg_line_num)
					 | (config->order  << SLCD_CMCFG_DEC_Pos);
	SLCD->ACMCFG.reg = SLCD_ACMCFG_FCS(config->fc)
					 | (config->mode << SLCD_ACMCFG_MODE_Pos)
					 | SLCD_ACMCFG_STSEG(config->start_seg_line)
					 | SLCD_ACMCFG_NDROW(config->row_digit_num)
					 | SLCD_ACMCFG_NDIG(config->digit_num)
					 | SLCD_ACMCFG_STEPS(config->scrolling_step)
					 | SLCD_ACMCFG_NCOM(config->com_line_num);

	return STATUS_OK;
}
/**
 * \brief Write segments data to display memory in automated character mode.
 *
 * \param[in] seg_data Pixel/segment data
 */
void slcd_automated_char_write_data(uint32_t seg_data)
{
	while (slcd_get_char_writing_status()) {
	}
	SLCD->CMDATA.reg = SLCD_CMDATA_SDATA(seg_data);
}

/**
 * \brief Segments data mask.
 *
 * \param[in] data_mask Segments data mask
 */
void slcd_automated_char_set_mask(uint32_t data_mask)
{
	SLCD->CMDMASK.reg = SLCD_CMDMASK_SDMASK(data_mask);
}

/**
 * \brief Segments COM/SEG lines index.
 *
 * \param[in] com_line_index COM line index
 * \param[in] seg_line_index Segments line index
 */

void slcd_automated_char_set_index(uint8_t com_line_index,uint8_t seg_line_index)
{
	SLCD->CMINDEX.reg = SLCD_CMINDEX_SINDEX(seg_line_index)
						| SLCD_CMINDEX_CINDEX(com_line_index);
}

/**
 * \brief Initializes circular shift configurations struct to defaults.
 *
 * Initailizes circular shift configuration struct to predefined safe default settings.
 *
 * \param[in] config Pointer to an instance of \ref struct slcd_circular_shift_config
 *
 */
void slcd_circular_shift_get_config_defaults(
		struct slcd_circular_shift_config *const config)
{
	if (!config) {
		return ;
	}
	config->fc = SLCD_FRAME_COUNTER_0;
	config->dir = SLCD_CIRCULAR_SHIFT_LEFT;
	config->size = 0;
	config->data = 0;
}

/**
 * \brief Set SLCD circular shift.
 *
 * Set circular shift mode.
 *
 * \note SLCD circular shift mode cannot be set while module or circular shift is enabled.
 *
 * \param[in] config  Pointer to an SLCD circular shift configuration structure
 *
 * \return Status of the configuration procedure.
 * \retval STATUS_OK SLCD circular shift configuration went successful
 * \retval STATUS_ERR_INVALID_ARG  If circular shift configuration failed
 */

enum status_code slcd_circular_shift_set_config(
		struct slcd_circular_shift_config *const config)
{
	if (!config) {
		return STATUS_ERR_INVALID_ARG;
	}
	SLCD->CSRCFG.reg = SLCD_CSRCFG_FCS(config->fc)
					 | (config->dir << SLCD_CSRCFG_DIR_Pos)
					 | SLCD_CSRCFG_SIZE(config->size)
					 | SLCD_CSRCFG_DATA(config->data);

	return STATUS_OK;
}

