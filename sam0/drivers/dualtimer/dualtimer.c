/**
 * \file
 *
 * \brief SAM DUALTIMER Driver for SAMB11
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
#include "dualtimer.h"

/**
 * \brief Initializes config with predefined default values.
 *
 * This function will initialize a given Dualtimer configuration structure to
 * a set of known default values. This function should be called on
 * any new instance of the configuration structures before being
 * modified by the user application.
 *
 * \param[out]  config  Pointer to a DUALTIMER module configuration structure to set
 */
void dualtimer_get_config_defaults(struct dualtimer_config *config)
{
	config->counter_mode[0] = DUALTIMER_COUNTER_WRAPPING;
	config->counter_mode[1] = DUALTIMER_COUNTER_WRAPPING;
	
	config->counter_size[0] = DUALTIMER_COUNTER_SIZE_32BIT;
	config->counter_size[1] = DUALTIMER_COUNTER_SIZE_32BIT;

	config->clock_prescaler[0] = DUALTIMER_CLOCK_PRESCALER_DIV1;
	config->clock_prescaler[1] = DUALTIMER_CLOCK_PRESCALER_DIV1;

	config->timer_mode[0] = DUALTIMER_PERIODIC_MODE;
	config->timer_mode[1] = DUALTIMER_PERIODIC_MODE;

	config->interrup_enable[0] = false;
	config->interrup_enable[1] = false;
	
	config->load_value[0] = CONF_DUALTIMER_TIMER1_LOAD;
	config->load_value[1] = CONF_DUALTIMER_TIMER2_LOAD;
	
	config->integration_test_enable = false;
}

/**
 * \brief Initializes Dualtimer module instance.
 *
 * Initializes the Dualtimer module, based on the given
 * configuration values.
 *
 * \param[in]     config       Pointer to the Dualtimer configuration options struct
 *
 * \return Status of the initialization procedure.
 */
void dualtimer_init(const struct dualtimer_config *config)
{
	DUALTIMER0->TIMER1CONTROL.reg = 
			DUALTIMER_TIMER1CONTROL_ONE_SHOT_COUNT(config->counter_mode[0]) |
			DUALTIMER_TIMER1CONTROL_TIMER_SIZE(config->counter_size[0]) |
			DUALTIMER_TIMER1CONTROL_TIMERPRE(config->clock_prescaler[0]) |
			DUALTIMER_TIMER1CONTROL_TIMER_MODE(config->timer_mode[0]);
	if (config->interrup_enable[0]) {
		DUALTIMER0->TIMER1CONTROL.reg |= DUALTIMER_TIMER1CONTROL_INTERRUPT_ENABLE;
	}
	DUALTIMER0->TIMER1LOAD.reg = config->load_value[0];
	
	DUALTIMER0->TIMER2CONTROL.reg = 
			DUALTIMER_TIMER2CONTROL_ONE_SHOT_COUNT(config->counter_mode[1]) |
			DUALTIMER_TIMER2CONTROL_TIMER_SIZE(config->counter_size[1]) |
			DUALTIMER_TIMER2CONTROL_TIMERPRE(config->clock_prescaler[1]) |
			DUALTIMER_TIMER2CONTROL_TIMER_MODE(config->timer_mode[1]);
	if (config->interrup_enable[1]) {
		DUALTIMER0->TIMER2CONTROL.reg |= DUALTIMER_TIMER2CONTROL_INTERRUPT_ENABLE;
	}
	DUALTIMER0->TIMER2LOAD.reg = config->load_value[1];
	
	DUALTIMER0->TIMERITCR.reg = config->integration_test_enable;
}

/**
 * \brief Get Dualtimer module timer1/timer2 current value.
 *
 * \param[in]     timer        Timer1/Timer2
 *
 * \retval Timer1/Timer2 current value
 */
uint32_t dualtimer_get_current_value(enum dualtimer_timer timer)
{
	if (timer == DUALTIMER_TIMER1) {
		return DUALTIMER0->TIMER1VALUE.reg;
	} else {
		return DUALTIMER0->TIMER2VALUE.reg;
	}
}

/**
 * \brief Set Dualtimer module timer1/timer2 load value.
 *
 * \param[in]     timer        Timer1/Timer2
 * \param[in]     value        Load value
 */
void dualtimer_set_load_value(enum dualtimer_timer timer,
		uint32_t value)
{
	if (timer == DUALTIMER_TIMER1) {
		DUALTIMER0->TIMER1LOAD.reg = value;
	} else {
		DUALTIMER0->TIMER2LOAD.reg = value;
	}
}

/**
 * \brief Get Dualtimer module timer1/timer2 load value.
 *
 * \param[in]     timer        Timer1/Timer2
 *
 * \retval Timer1/Timer2 load value
 */
uint32_t dualtimer_get_load_value(enum dualtimer_timer timer)
{
	if (timer == DUALTIMER_TIMER1) {
		return DUALTIMER0->TIMER1LOAD.reg;
	} else {
		return DUALTIMER0->TIMER2LOAD.reg;
	}
}

/**
 * \brief Set Dualtimer module timer1/timer2 background load value.
 *
 * \param[in]     timer        Timer1/Timer2
 * \param[in]     value        Background load value
 */
void dualtimer_set_bg_load_value(enum dualtimer_timer timer,
		uint32_t value)
{
	if (timer == DUALTIMER_TIMER1) {
		DUALTIMER0->TIMER1BGLOAD.reg = value;
	} else {
		DUALTIMER0->TIMER2BGLOAD.reg = value;
	}
}

/**
 * \brief Get Dualtimer module timer1/timer2 background load value.
 *
 * \param[in]     timer        Timer1/Timer2
 *
 * \retval Timer1/Timer2 background load value
 */
uint32_t dualtimer_get_bg_load_value(enum dualtimer_timer timer)
{
	if (timer == DUALTIMER_TIMER1) {
		return DUALTIMER0->TIMER1BGLOAD.reg;
	} else {
		return DUALTIMER0->TIMER2BGLOAD.reg;
	}
}

/**
 * \brief Get Dualtimer module timer1/timer2 interrupt status
 *
 * \param[in]     timer        Timer1/Timer2
 *
 * \retval The interrupt status of timer1/timer2
 */
uint8_t dualtimer_get_interrupt_status(enum dualtimer_timer timer)
{
	if (timer == DUALTIMER_TIMER1) {
		return DUALTIMER0->TIMER1MIS.reg;
	} else {
		return DUALTIMER0->TIMER2MIS.reg;
	}
}

/**
 * \brief Get Dualtimer module timer1/timer2 raw interrupt status
 *
 * \param[in]     timer        Timer1/Timer2
 *
 * \retval The raw interrupt status of timer1/timer2
 */
uint8_t dualtimer_get_interrupt_status_raw(enum dualtimer_timer timer)
{
	if (timer == DUALTIMER_TIMER1) {
		return DUALTIMER0->TIMER1RIS.reg;
	} else {
		return DUALTIMER0->TIMER2RIS.reg;
	}
}

/**
 * \brief Clear Dualtimer module timer1/timer2 interrupt status
 *
 * Clear the Dualtimer module timer1/timer2 interrupt status
 *
 * \param[in]     timer        Timer1/Timer2
 */
void dualtimer_clear_interrupt_status(enum dualtimer_timer timer)
{
	if (timer == DUALTIMER_TIMER1) {
		DUALTIMER0->TIMER1INTCLR.reg = 1;
	} else {
		DUALTIMER0->TIMER2INTCLR.reg = 1;
	}
}

/**
 * \brief Set Dualtimer module timer1/timer2 enable
 *
 * Enable the Dualtimer module timer1/timer2
 *
 * \param[in]     timer        Timer1/Timer2
 */
void dualtimer_enable(enum dualtimer_timer timer)
{
	if (timer == DUALTIMER_TIMER1) {
		DUALTIMER0->TIMER1CONTROL.reg |= DUALTIMER_TIMER1CONTROL_TIMER_ENABLE;
	} else {
		DUALTIMER0->TIMER2CONTROL.reg |= DUALTIMER_TIMER2CONTROL_TIMER_ENABLE;
	}
}

/**
 * \brief Set Dualtimer module timer1/timer2 disable
 *
 * Disable the Dualtimer module timer1/timer2
 *
 * \param[in]     timer        Timer1/Timer2
 */
void dualtimer_disable(enum dualtimer_timer timer)
{
	if (timer == DUALTIMER_TIMER1) {
		DUALTIMER0->TIMER1CONTROL.reg &= ~DUALTIMER_TIMER1CONTROL_TIMER_ENABLE;
	} else {
		DUALTIMER0->TIMER2CONTROL.reg &= ~DUALTIMER_TIMER2CONTROL_TIMER_ENABLE;
	}
}

/**
 * \brief Enable Dualtimer module integration test mode
 *
 * Enable the Dualtimer module integration test mode
 */
void dualtimer_integration_test_enable(void)
{
	DUALTIMER0->TIMERITCR.reg = true;
}

/**
 * \brief Disable Dualtimer module integration test mode
 *
 * Disable the Dualtimer module integration test mode
 */
void dualtimer_integration_test_disable(void)
{
	DUALTIMER0->TIMERITCR.reg = false;
}

/**
 * \brief Disable Dualtimer module integration test mode
 *
 * Disable the Dualtimer module integration test mode
 *
 * \param[in]     timer        Value output on TIMINT1
 * \param[in]     timer        Value output on TIMINT2
 */
void dualtimer_integration_test_set_interrupt(
		bool timer1, bool timer2)
{
	if (timer1 && timer2) {
		DUALTIMER0->TIMERITOP.reg =
				DUALTIMER_TIMERITOP_INT_TEST_TIMINT1_VALUE |
				DUALTIMER_TIMERITOP_INT_TEST_TIMING2_VALUE;
	} else if (timer1 && (!timer2)) {
		DUALTIMER0->TIMERITOP.reg =
				DUALTIMER_TIMERITOP_INT_TEST_TIMINT1_VALUE;
	} else if ((!timer1) && timer2) {
		DUALTIMER0->TIMERITOP.reg =
				DUALTIMER_TIMERITOP_INT_TEST_TIMING2_VALUE;
	} else {
		DUALTIMER0->TIMERITOP.reg = 0;
	}
}