/**
 * \file
 *
 * \brief Glue Logic driver for SAM4L.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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

#include "gloc.h"
#include "sysclk.h"
#include "sleepmgr.h"
#include "conf_gloc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Get the default GLOC module configuration:
 * Filter: enable
 * Input mask: 0xF, all four inputs are enabled
 * Truth table value: 0x00
 *
 * \param cfg Pointer to GLOC configuration array.
 */
void gloc_lut_get_config_defaults(struct gloc_lut_config *const cfg)
{
	/* Sanity check arguments */
	Assert(cfg);

	uint32_t i;
	struct gloc_lut_config *p_temp_cfg = cfg;

	for (i = 0; i < GLOC_LUTS; i++) {
		p_temp_cfg->filter = true;
		p_temp_cfg->input_mask = 0xF;
		p_temp_cfg->truth_table_value = 0x00;
		p_temp_cfg++;
	}
}

/**
 * \brief Initialize the GLOC module.
 *
 * \param dev_inst Device structure pointer.
 * \param gloc Base address of the GLOC instance.
 * \param cfg Pointer to GLOC configuration array.
 *
 */
void gloc_init(struct gloc_dev_inst *const dev_inst, Gloc *const gloc,
		struct gloc_lut_config *const cfg)
{
	/* Sanity check arguments */
	Assert(dev_inst);
	Assert(gloc);
	Assert(cfg);

	uint32_t i;
	struct gloc_lut_config *p_temp_cfg = cfg;

	dev_inst->hw_dev = gloc;

	for (i = 0; i < GLOC_LUTS; i++) {
		dev_inst->gloc_lut_cfg[i] = p_temp_cfg;
		p_temp_cfg++;
	}
	/* Enable APB clock for GLOC */
	sysclk_enable_peripheral_clock(dev_inst->hw_dev);

	/* Initialize the GLOC with new configurations */
	gloc_lut_set_config(dev_inst);

	/* Disable APB clock for GLOC */
	sysclk_disable_peripheral_clock(dev_inst->hw_dev);
}

/**
 * \brief Enable the GLOC.
 *
 * \param dev_inst Device structure pointer.
 *
 */
void gloc_enable(struct gloc_dev_inst *const dev_inst)
{
	struct genclk_config gencfg;

	sysclk_enable_peripheral_clock(dev_inst->hw_dev);
	sleepmgr_lock_mode(SLEEPMGR_SLEEP_0);
	genclk_config_defaults(&gencfg, GLOC_GCLK_NUM);
	genclk_enable_source(CONFIG_GLOC_GENCLK_SRC);
	genclk_config_set_source(&gencfg, CONFIG_GLOC_GENCLK_SRC);
	genclk_config_set_divider(&gencfg, CONFIG_GLOC_GENCLK_DIV);
	genclk_enable(&gencfg, GLOC_GCLK_NUM);
}

/**
 * \brief Disable the GLOC.
 *
 * \param dev_inst Device structure pointer.
 *
 */
void gloc_disable(struct gloc_dev_inst *const dev_inst)
{
	sysclk_disable_peripheral_clock(dev_inst->hw_dev);
	sleepmgr_unlock_mode(SLEEPMGR_SLEEP_0);
}

/**
 * \brief Configure the GLOC.
 *
 * \param  dev_inst Device structure pointer.
 *
 */
void gloc_lut_set_config(struct gloc_dev_inst *const dev_inst)
{
	uint32_t i;

	for (i = 0; i < GLOC_LUTS; i++) {
		if (dev_inst->gloc_lut_cfg[i]->filter) {
			dev_inst->hw_dev->GLOC_LUT[i].GLOC_CR = GLOC_CR_FILTEN |
					GLOC_CR_AEN(dev_inst->gloc_lut_cfg[i]->input_mask);
		} else {
			dev_inst->hw_dev->GLOC_LUT[i].GLOC_CR =
					GLOC_CR_AEN(dev_inst->gloc_lut_cfg[i]->input_mask);
		}
		dev_inst->hw_dev->GLOC_LUT[i].GLOC_TRUTH =
				dev_inst->gloc_lut_cfg[i]->truth_table_value;
	}
}

#ifdef __cplusplus
}
#endif

