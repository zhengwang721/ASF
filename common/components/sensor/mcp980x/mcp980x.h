/**
 * \file
 *
 * \brief MCP980X driver
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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
 
#ifndef MCP980X_H_INCLUDED
#define MCP980X_H_INCLUDED

#include "twi_master.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

//! Checking board configuration of the MCP980X Temperature Sensor
#if !defined(BOARD_MCP980X_TWI_INSTANCE)
# warning The MCP980X TWI instance has not been defined. Using default settings.
# define BOARD_MCP980X_TWI_INSTANCE 0 /* TWI instance (TWI0) */
#endif
#if !defined(BOARD_MCP980X_ADDR)
# warning The MCP980X TWI address has not been defined. Using default settings.
# define BOARD_MCP980X_ADDR 0 /* TWI sensor address (0x48u) */
#endif

//! \brief MCP980X Configuration Register Setting
#define MCP980X_CONFIG_ONE_SHOT_ENABLE            (1 << 7)

#define MCP980X_CONFIG_RESOLUTION_9_BIT           (0 << 5)
#define MCP980X_CONFIG_RESOLUTION_10_BIT          (1 << 5)
#define MCP980X_CONFIG_RESOLUTION_11_BIT          (2 << 5)
#define MCP980X_CONFIG_RESOLUTION_12_BIT          (3 << 5)

#define MCP980X_CONFIG_FAULT_QUEUE_1              (0 << 3)
#define MCP980X_CONFIG_FAULT_QUEUE_2              (1 << 3)
#define MCP980X_CONFIG_FAULT_QUEUE_4              (2 << 3)
#define MCP980X_CONFIG_FAULT_QUEUE_6              (3 << 3)

#define MCP980X_CONFIG_ALERT_POLARITY_ACTIVE_HIGH (1 << 2)
#define MCP980X_CONFIG_ALERT_POLARITY_ACTIVE_LOW  (0 << 2)

#define MCP980X_CONFIG_INTERRUPT_MODE             (1 << 1)
#define MCP980X_CONFIG_COMPARATOR_MODE            (0 << 1)

#define MCP980X_CONFIG_SHUTDOWN_ENABLE            (1 << 0)

uint32_t mcp980x_get_temperature(int8_t *p_integer, 
		uint32_t *p_decimal);
uint32_t mcp980x_set_temperature_limit(int8_t c_integer, uint32_t ul_decimal);
uint32_t mcp980x_get_temperature_limit(int8_t *p_integer, uint32_t *p_decimal);
uint32_t mcp980x_set_temperature_hysteresis(int8_t c_integer, 
		uint32_t ul_decimal);
uint32_t mcp980x_get_temperature_hysteresis(int8_t *p_integer,
		uint32_t *p_decimal);
uint32_t mcp980x_set_configuration(uint8_t uc_config);
uint32_t mcp980x_get_configuration(uint8_t *p_config);
uint32_t mcp980x_init(void);
uint32_t mcp980x_enable(void);
uint32_t mcp980x_disable(void);
uint32_t mcp980x_one_shot_mode(void);

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* MCP980X_H_INCLUDED */
