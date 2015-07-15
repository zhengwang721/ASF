/**
 * \file
 *
 * \brief SAM QUAD DECODER Driver for SAMB11
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
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
#include "qdec.h"

/**
 * \brief Initializes config with predefined default values.
 *
 * This function will initialize a given QUAD DECODER configuration structure to
 * a set of known default values. This function should be called on
 * any new instance of the configuration structures before being
 * modified by the user application.
 *
 * The default configuration is as follows:
 *  \li Select input clock as 26MHz 
 *  \li Set counter upper threshold as 32767
 *  \li Set counter lower threshold as -32768
 *  \li Set pinmux_pad[0] as MEGAMUX_SEL29_QUAD_DEC_X_IN_A
 *  \li Set pinmux_pad[1] as MEGAMUX_SEL29_QUAD_DEC_X_IN_B
 *
 * \param[out]  config  Pointer to a QUAD DECODER module configuration structure to set
 */
void qdec_get_config_defaults(struct qdec_config *config)
{
	config->clock_sel = QDEC_CLK_INPUT_0;
	config->threshold_upper = 32767;
	config->threshold_lower = -32768;
	config->pinmux_pad[0] = 
			PINMUX_MEGAMUX_SEL29_QDEC_X_A_PIN(PIN_LP_GPIO_15);
	config->pinmux_pad[1] = 
			PINMUX_MEGAMUX_SEL30_QDEC_X_B_PIN(PIN_LP_GPIO_16);
}

/**
 * \brief Initializes QUAD DECODER module instance.
 *
 * Initializes the QUAD DECODER module, based on the given
 * configuration values.
 *
 * \param[in]     qdec         Axis(x,y,z) of QUAD DECODER instance
 * \param[in]     config       Pointer to the QAUD DECODER configuration options struct
 */
void qdec_init(enum qdec_axis qdec,
		const struct qdec_config *config)
{
	switch (qdec) {
	case QDEC_AXIS_X:
		LPMCU_MISC_REGS0->QUAD_DEC_1_CTRL.reg = 
				LPMCU_MISC_REGS_QUAD_DEC_1_CTRL_CLOCK_SEL(config->clock_sel);
		LPMCU_MISC_REGS0->QUAD_DEC_1_THRESHOLD.reg = 
				LPMCU_MISC_REGS_QUAD_DEC_1_THRESHOLD_UPPER(config->threshold_upper);
		LPMCU_MISC_REGS0->QUAD_DEC_1_THRESHOLD.reg = 
				LPMCU_MISC_REGS_QUAD_DEC_1_THRESHOLD_LOWER(config->threshold_lower);
		break;
	case QDEC_AXIS_Y:
		LPMCU_MISC_REGS0->QUAD_DEC_2_CTRL.reg = 
				LPMCU_MISC_REGS_QUAD_DEC_2_CTRL_CLOCK_SEL(config->clock_sel);
		LPMCU_MISC_REGS0->QUAD_DEC_2_THRESHOLD.reg = 
				LPMCU_MISC_REGS_QUAD_DEC_2_THRESHOLD_UPPER(config->threshold_upper);
		LPMCU_MISC_REGS0->QUAD_DEC_2_THRESHOLD.reg = 
				LPMCU_MISC_REGS_QUAD_DEC_2_THRESHOLD_LOWER(config->threshold_lower);
		break;
	case QDEC_AXIS_Z:
		LPMCU_MISC_REGS0->QUAD_DEC_3_CTRL.reg = 
				LPMCU_MISC_REGS_QUAD_DEC_3_CTRL_CLOCK_SEL(config->clock_sel);
		LPMCU_MISC_REGS0->QUAD_DEC_3_THRESHOLD.reg = 
				LPMCU_MISC_REGS_QUAD_DEC_3_THRESHOLD_UPPER(config->threshold_upper);
		LPMCU_MISC_REGS0->QUAD_DEC_3_THRESHOLD.reg = 
				LPMCU_MISC_REGS_QUAD_DEC_3_THRESHOLD_LOWER(config->threshold_lower);
		break;
	}

	gpio_pinmux_cofiguration(config->pinmux_pad[0] >> 16,
			(uint16_t)(config->pinmux_pad[0] & 0xFFFF));
	gpio_pinmux_cofiguration(config->pinmux_pad[1] >> 16,
			(uint16_t)(config->pinmux_pad[1] & 0xFFFF));
}

/**
 * \brief Enable QUAD DECODER clock.
 *
 * Enable the QUAD DECODER clock, based on the given axis
 *
 * \param[in]     qdec         Axis(x,y,z) of QUAD DECODER instance
 */
void qdec_clock_enable(enum qdec_axis qdec)
{
	switch (qdec) {
	case QDEC_AXIS_X:
		LPMCU_MISC_REGS0->LPMCU_CLOCK_ENABLES_0.reg |= 
				LPMCU_MISC_REGS_LPMCU_CLOCK_ENABLES_0_QUAD_DEC_1_CLK_EN;
		break;
	case QDEC_AXIS_Y:
		LPMCU_MISC_REGS0->LPMCU_CLOCK_ENABLES_0.reg |= 
				LPMCU_MISC_REGS_LPMCU_CLOCK_ENABLES_0_QUAD_DEC_2_CLK_EN;
		break;
	case QDEC_AXIS_Z:
		LPMCU_MISC_REGS0->LPMCU_CLOCK_ENABLES_0.reg |= 
				LPMCU_MISC_REGS_LPMCU_CLOCK_ENABLES_0_QUAD_DEC_3_CLK_EN;
		break;
	}
}

/**
 * \brief Disable QUAD DECODER clock.
 *
 * Disable the QUAD DECODER clock, based on the given axis
 *
 * \param[in]     qdec         Axis(x,y,z) of QUAD DECODER instance
 */
void qdec_clock_disable(enum qdec_axis qdec)
{
	switch (qdec) {
	case QDEC_AXIS_X:
		LPMCU_MISC_REGS0->LPMCU_CLOCK_ENABLES_0.reg &= 
				~LPMCU_MISC_REGS_LPMCU_CLOCK_ENABLES_0_QUAD_DEC_1_CLK_EN;
		break;
	case QDEC_AXIS_Y:
		LPMCU_MISC_REGS0->LPMCU_CLOCK_ENABLES_0.reg &= 
				~LPMCU_MISC_REGS_LPMCU_CLOCK_ENABLES_0_QUAD_DEC_2_CLK_EN;
		break;
	case QDEC_AXIS_Z:
		LPMCU_MISC_REGS0->LPMCU_CLOCK_ENABLES_0.reg &= 
				~LPMCU_MISC_REGS_LPMCU_CLOCK_ENABLES_0_QUAD_DEC_3_CLK_EN;
		break;
	}
}

/**
 * \brief Enable QUAD DECODER module instance.
 *
 * Enable the QUAD DECODER module instance, based on the given axis
 *
 * \param[in]     qdec         Axis(x,y,z) of QUAD DECODER instance
 */
void qdec_enable(enum qdec_axis qdec)
{
	switch (qdec) {
	case QDEC_AXIS_X:
		LPMCU_MISC_REGS0->QUAD_DEC_1_CTRL.reg |= 
				LPMCU_MISC_REGS_QUAD_DEC_1_CTRL_ENABLE;
		break;
	case QDEC_AXIS_Y:
		LPMCU_MISC_REGS0->QUAD_DEC_2_CTRL.reg |= 
				LPMCU_MISC_REGS_QUAD_DEC_2_CTRL_ENABLE;
		break;
	case QDEC_AXIS_Z:
		LPMCU_MISC_REGS0->QUAD_DEC_3_CTRL.reg |= 
				LPMCU_MISC_REGS_QUAD_DEC_3_CTRL_ENABLE;
		break;
	}
}

/**
 * \brief Disable QUAD DECODER module instance.
 *
 * Disable the QUAD DECODER module instance, based on the given axis
 *
 * \param[in]     qdec         Axis(x,y,z) of QUAD DECODER instance
 */
void qdec_disable(enum qdec_axis qdec)
{
	switch (qdec) {
	case QDEC_AXIS_X:
		LPMCU_MISC_REGS0->QUAD_DEC_1_CTRL.reg &= 
				~LPMCU_MISC_REGS_QUAD_DEC_1_CTRL_ENABLE;
		break;
	case QDEC_AXIS_Y:
		LPMCU_MISC_REGS0->QUAD_DEC_2_CTRL.reg &= 
				~LPMCU_MISC_REGS_QUAD_DEC_2_CTRL_ENABLE;
		break;
	case QDEC_AXIS_Z:
		LPMCU_MISC_REGS0->QUAD_DEC_3_CTRL.reg &= 
				~LPMCU_MISC_REGS_QUAD_DEC_3_CTRL_ENABLE;
		break;
	}
}

/**
 * \brief Get QUAD DECODER IRQ status
 *
 * Get the IRQ status of quad decoders
 *
 * \return The IRQ status of quad decoders 
 */
uint8_t qdec_get_irq_status(void)
{
	return LPMCU_MISC_REGS0->QUAD_DEC_IRQS.reg;
}

/**
 * \brief Clear QUAD DECODER IRQ status
 *
 * Clear the IRQ status of quad decoder, based on the given axis
 *
 * \param[in]     qdec         Axis(x,y,z) of QUAD DECODER instance
 */
void qdec_clear_irq_status(enum qdec_axis qdec)
{
	switch (qdec) {
	case QDEC_AXIS_X:
		LPMCU_MISC_REGS0->QUAD_DEC_1_CTRL.reg |= 
				~LPMCU_MISC_REGS_QUAD_DEC_1_CTRL_ENABLE;
		break;
	case QDEC_AXIS_Y:
		LPMCU_MISC_REGS0->QUAD_DEC_2_CTRL.reg |= 
				~LPMCU_MISC_REGS_QUAD_DEC_2_CTRL_ENABLE;
		break;
	case QDEC_AXIS_Z:
		LPMCU_MISC_REGS0->QUAD_DEC_3_CTRL.reg |= 
				~LPMCU_MISC_REGS_QUAD_DEC_3_CTRL_ENABLE;
		break;
	}
}

/**
 * \brief Get QUAD DECODER current counter
 *
 * Get the current counter of quad decoder, based on the given axis
 *
 * \param[in]     qdec         Axis(x,y,z) of QUAD DECODER instance
 *
 * \return The current counter of quad decoder instance
 */
int16_t qdec_get_counter(enum qdec_axis qdec)
{
	switch (qdec) {
	case QDEC_AXIS_X:
		return LPMCU_MISC_REGS0->QUAD_DEC_1_STATUS.bit.COUNT;
	case QDEC_AXIS_Y:
		return LPMCU_MISC_REGS0->QUAD_DEC_2_STATUS.bit.COUNT;
	case QDEC_AXIS_Z:
		return LPMCU_MISC_REGS0->QUAD_DEC_3_STATUS.bit.COUNT;
	}
}

/**
 * \brief Resets the QUAD DECODER module
 *
 * This function will reset the QUAD DECODER module to its 
 * power on default values and disable it.
 *
 * \param[in]     qdec         Axis(x,y,z) of QUAD DECODER instance
 */
void qdec_reset(enum qdec_axis qdec)
{
	switch (qdec) {
	case QDEC_AXIS_X:
		LPMCU_MISC_REGS0->LPMCU_GLOBAL_RESET_0.reg &=
				~LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_QUAD_DEC_1_RSTN;
		LPMCU_MISC_REGS0->LPMCU_GLOBAL_RESET_0.reg |=
				LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_QUAD_DEC_1_RSTN;
		break;
	case QDEC_AXIS_Y:
		LPMCU_MISC_REGS0->LPMCU_GLOBAL_RESET_0.reg &=
				~LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_QUAD_DEC_2_RSTN;
		LPMCU_MISC_REGS0->LPMCU_GLOBAL_RESET_0.reg |=
				LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_QUAD_DEC_2_RSTN;
		break;
	case QDEC_AXIS_Z:
		LPMCU_MISC_REGS0->LPMCU_GLOBAL_RESET_0.reg &=
				~LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_QUAD_DEC_3_RSTN;
		LPMCU_MISC_REGS0->LPMCU_GLOBAL_RESET_0.reg |=
				LPMCU_MISC_REGS_LPMCU_GLOBAL_RESET_0_QUAD_DEC_3_RSTN;
		break;
	}
}