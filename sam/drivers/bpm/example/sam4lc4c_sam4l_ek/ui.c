/**
 * \file
 *
 * \brief User interface for BPM example.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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

#include <asf.h>
#include "ui.h"
#include "conf_board.h"

#include "board_monitor.h"
sam4l_status_t sam4l_status = {
	.voltage_scaling = VOLTAGE_SCALING_12V,
	.sleep_mode = SLEEP_MODE_RUN,
	.cpu_freq = 12000000,
	.cpu_src = CPU_SRC_RC4M,
};
/* Power scaling value -> board monitor status */
voltage_scaling_t ps_statuses[] = {
	VOLTAGE_SCALING_18V, VOLTAGE_SCALING_12V
};
/* Sleep modes -> board monitor status */
sleep_mode_t sleep_statuses[] = {
	SLEEP_MODE_RUN,
	SLEEP_MODE_NA, SLEEP_MODE_NA, SLEEP_MODE_NA, SLEEP_MODE_NA, // Sleep 0-3
	SLEEP_MODE_WAIT,
	SLEEP_MODE_RETENTION,
	SLEEP_MODE_BACKUP
};

/**
 * Enable necessary clocks for USARTs
 */
static void ui_enable_clocks(void)
{
	sysclk_enable_hsb_module(SYSCLK_PBA_BRIDGE);
	sysclk_enable_peripheral_clock(REMOTE_TASK_USART);
}

/**
 * Disable clocks for UI
 */
static void ui_disable_clocks(void)
{
	sysclk_disable_peripheral_clock(REMOTE_TASK_USART);
	sysclk_disable_hsb_module(SYSCLK_PBA_BRIDGE);
}

void ui_init(void)
{
	ui_enable_clocks();
	bm_init();
	ui_disable_clocks();
}

void ui_show_backup_wakeup_cause(uint32_t wakeup_cause)
{
	UNUSED(wakeup_cause);
}

void ui_show_mode_info(uint8_t mode, const mode_config_t *mode_config)
{
	UNUSED(mode);
	ui_enable_clocks();
	sam4l_status.voltage_scaling = ps_statuses[mode_config->ps_value];
	sam4l_status.sleep_mode = sleep_statuses[mode_config->sleep_mode];
	bm_send_mcu_status(sam4l_status.voltage_scaling,
		sam4l_status.sleep_mode,sam4l_status.cpu_freq,
		sam4l_status.cpu_src);
	ui_disable_clocks();
}

void ui_show_wakeup_info(uint8_t mode, uint8_t wakeup_events)
{
	UNUSED(mode);
	UNUSED(wakeup_events);
}


