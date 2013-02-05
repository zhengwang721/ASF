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

#ifndef GLOC_H_INCLUDED
#define GLOC_H_INCLUDED

#include "compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup sam_drivers_gloc_group Glue Logic Controller (GLOC)
 *
 * See \ref sam_gloc_quickstart.
 *
 * Driver for the Glue Logic Controller. This driver provides access to the
 * main features of the Glue Logic controller.
 *
 * @{
 */

/**
 * \brief GLOC LUT configuration structure.
 *
 * Configuration structure for a GLOC LUT instance. This
 * structure could be initialized by the \ref gloc_lut_get_config_defaults()
 * function before being modified by the user application.
 */
struct gloc_lut_config {
	/** True for enable, false for disable. */
	bool filter;
	/** Input enable mask. */
	uint8_t input_mask;
	/** Truth table value. */
	uint16_t truth_table_value;
};

/**
 * \brief Glue Logic Controller driver software instance structure.
 *
 * Device instance structure for a Glue Logic Controller driver instance. This
 * structure should be initialized by the \ref gloc_init() function to
 * associate the instance with a particular hardware module of the device.
 */
struct gloc_dev_inst {
	/** Base address of the GLOC module. */
	Gloc *hw_dev;
	/** Pointer to GLOC LUT configuration structure. */
	struct gloc_lut_config  *gloc_lut_cfg[GLOC_LUTS];
};

void gloc_lut_get_config_defaults(struct gloc_lut_config *const cfg);
void gloc_init(struct gloc_dev_inst *const dev_inst, Gloc *const gloc,
		struct gloc_lut_config *const cfg);
void gloc_enable(struct gloc_dev_inst *const dev_inst);
void gloc_disable(struct gloc_dev_inst *const dev_inst);
void gloc_lut_set_config(struct gloc_dev_inst *const dev_inst);

//@}

#ifdef __cplusplus
}
#endif

#endif /* GLOC_H_INCLUDED */
