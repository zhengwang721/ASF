/**
 * \file
 *
 * \brief Analog Comparator interface driver for SAM4L.
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

#include "acifc.h"
#include "sysclk.h"
#include "sleepmgr.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

struct ac_dev_inst *_ac_inst;

/**
 * \defgroup sam_drivers_acifc_group Analog Comparator (AC)
 *
 * See \ref sam_acifc_quickstart.
 *
 * Driver for the Analog Comparator. This driver provides access to the main
 * features of the AC controller.
 *
 * @{
 */

/**
 * \internal Set configurations to module.
 *
 * \param  dev_inst Pointer to device instance structure.
 * \param  cfg      Configuration structure with configurations to set.
 *
 */
static inline void _ac_set_config(struct ac_dev_inst *const dev_inst, 
		struct ac_config *const cfg)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(cfg);

	dev_inst->hw_dev->ACIFC_CTRL |= 
			(cfg->event_trigger ? ACIFC_CTRL_EVENTEN : 0);
}

/**
 * \brief Initializes the requested AC Hardware module.
 *
 * Enables the source clock for the requested AC Hardware module and initializes
 * it with the given configuration.
 *
 * \param  dev_inst Pointer to device instance struct.
 * \param  ac       Pointer to the hardware instance.
 * \param  cfg      Pointer to the configuration struct.
 *
 * \return              Status of initialization.
 * \retval STATUS_OK Module initiated correctly.
 * \retval STATUS_ERR_DENIED If module has been enabled.
 * \retval STATUS_ERR_BUSY If module is busy comparing.
 *
 */
enum status_code ac_init(struct ac_dev_inst *const dev_inst, Acifc *const ac,
		struct ac_config *const cfg)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(ac);
	Assert(cfg);

	uint32_t ac_ctrl = ac->ACIFC_CTRL;
	/* Check if module is enabled. */
	if (ac_ctrl & ACIFC_CTRL_EN) {
		return STATUS_ERR_DENIED;
	}

	/* Check if comparison is in progress. */
	if (ac_ctrl & ACIFC_CTRL_USTART){
		return STATUS_ERR_BUSY;
	}

	uint32_t i;
	for (i = 0; i < _AC_CALLBACK_N; i++) {
		dev_inst->callbacks[i] = 0;
	}

	/* Enable and configure device instance */
	dev_inst->hw_dev = ac;
	sysclk_enable_peripheral_clock(ac);
	_ac_set_config(dev_inst, cfg);

	_ac_inst = dev_inst;

	return STATUS_OK;
}

#define AC_NB_OF_CH 8
/**
 * \brief Configure the specified AC channel
 *
 * \param dev_inst   Device structure pointer
 * \param channel    AC channel number
 * \param cfg        Configuration for AC channel
 *
 */
void ac_ch_set_config(struct ac_dev_inst *const dev_inst, uint32_t channel,
		struct ac_ch_config *cfg)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(channel < AC_NB_OF_CH);
	Assert(cfg);

	uint32_t ac_conf = 0;
	ac_conf = ACIFC_CONF_HYS(cfg->hysteresis_voltage) |
			(cfg->always_on ? ACIFC_CONF_ALWAYSON : 0) |
			(cfg->fast_mode ? ACIFC_CONF_FAST : 0) |
			(cfg->event_negative ? ACIFC_CONF_EVENN : 0) |
			(cfg->event_positive ? ACIFC_CONF_EVENP : 0) |
			ACIFC_CONF_INSELN(cfg->negative_input) |
			ACIFC_CONF_MODE(cfg->comparator_mode) |
			ACIFC_CONF_IS(interrupt_setting);

	dev_inst->hw_dev->ACIFC_CONF[channel].ACIFC_CONF = ac_conf;
}

#define AC_NB_OF_WIN (AC_NB_OF_CH >> 1)
/**
 * \brief Configure one AC channel in Window mode
 *
 * \param dev_inst   Device structure pointer
 * \param window     AC window number
 * \param cfg        Configuration for AC window
 *
 */
void ac_win_set_config(struct ac_dev_inst *const dev_inst,
		uint32_t window, struct ac_win_config *cfg)
{
	/* Sanity check arguments. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(channel < AC_NB_OF_WIN);
	Assert(cfg);

	uint32_t ac_confw = 0;
	ac_confw = (cfg->enable ? ACIFC_CONFW_WFEN : 0) |
			(cfg->event_enable ? ACIFC_CONFW_WEVEN : 0) |
			ACIFC_CONFW_WEVSRC(cfg->event_source) |
			ACIFC_CONFW_WIS(cfg->interrupt_setting);

	dev_inst->hw_dev->ACIFC_CONFW[window].ACIFC_CONFW = ac_confw;
}

/**
 * \brief Enable ACIFC Module.
 *
 * \param dev_inst   Device structure pointer
 *
 */
void acifc_enable(struct ac_dev_inst *const dev_inst)
{
	sleepmgr_lock_mode(SLEEPMGR_BACKUP);
	dev_inst->hw_dev->ACIFC_CTRL |= ACIFC_CTRL_EN;
}

/**
 * \brief Disable ACIFC Module.
 *
 * \param dev_inst   Device structure pointer
 *
 */
void acifc_disable(struct ac_dev_inst *const dev_inst)
{
	dev_inst->hw_dev->ACIFC_CTRL &= ~ACIFC_CTRL_EN;
	sleepmgr_unlock_mode(SLEEPMGR_BACKUP);
}

/**
 * \brief Register callback for the specified callback type.
 *
 * When called, the given callback function will be associated with the
 * specified callback type.
 *
 * \param dev_inst   Device structure pointer
 * \param callback   Pointer to the function desired for the specified
 * callback type
 * \param type       Specifies the callback type to register.
 */
void ac_register_callback(struct ac_dev_inst *const dev_inst,
		ac_async_callback_t callback, enum ac_callback_type type)
{
	/* Sanity check. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);
	Assert(callback);

	/* Register callback. */
	dev_inst->callbacks[type] = callback;
}

/**
 * \brief Unregister callback for the specified callback type.
 *
 * When called, the currently registered callback for the given callback type
 * will be removed.
 *
 * \param dev_inst   Device structure pointer
 * \param type       Specifies the callback type to register.
 */
void ac_unregister_callback(struct ac_dev_inst *const dev_inst,
		enum ac_callback_type type)
{
	/* Sanity check. */
	Assert(dev_inst);
	Assert(dev_inst->hw_dev);

	/* Unregister callback. */
	dev_inst->callbacks[type] = 0;
}

/**
 * \brief Get AC interrupt status.
 *
 * \param dev_inst Device structure pointer.
 *
 */
static inline uint32_t _ac_get_interrupt_status(
		struct ac_dev_inst *const dev_inst)
{
	return dev_inst->hw_dev->ACFIC_ISR;
}

/**
 * \brief Get AC interrupt mask.
 *
 * \param dev_inst Device structure pointer.
 *
 */
static inline uint32_t _ac_get_interrupt_mask(
		struct ac_dev_inst *const dev_inst)
{
	return dev_inst->hw_dev->ACFIC_IMR;
}

/**
 * \brief Clear AC interrupt status.
 *
 * \param dev_inst Device structure pointer.
 * \param status   Interrupt status to be clear.
 *
 */
static inline void _ac_clear_interrupt_status(
		struct ac_dev_inst *const dev_inst, uint32_t status)
{
	dev_inst->hw_dev->ACFIC_ICR = status;
}

#define AC_CH_MAX_INT_NB  16
#define AC_WIN_MAX_INT_NB 4
#define AC_WIN_INT_OFFSET 24
/**
 * \brief Interrupt handler for ACIFC interrupt.
 */
void ACIFC_Handler(void)
{
	uint32_t irq_status = _ac_get_interrupt_status(_ac_inst);
	uint32_t irq_mask = _ac_get_interrupt_mask(_ac_inst);
	uint32_t status_masked = irq_status & irq_mask;

	uint32_t i = 0;
	for (; i < AC_CH_MAX_INT_NB; i++) {
		if (status_masked & (1 << i)) {
			if (i & 1) {
				_ac_inst->callbacks[AC_CALLBACK_CONVERSION_COMPLETED](_ac_inst, (i - 1) / 2);
			} else {
				_ac_inst->callbacks[AC_CALLBACK_STARTUP_TIME_ELAPSED](_ac_inst, i / 2);
			}
		}
	}

	for (i = AC_WIN_INT_OFFSET; i < AC_WIN_INT_OFFSET + AC_WIN_MAX_INT_NB; i++) {
		if (status_masked & (1 << i)) {
			_ac_inst->callbacks[AC_CALLBACK_WINDOW_INTERRUPT](_ac_inst, i - AC_WIN_INT_OFFSET);
		}
	}

	_ac_clear_interrupt_status(_ac_inst, irq_status);
}

//@}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
