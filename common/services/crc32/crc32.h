/**
 * \file
 *
 * \brief 32-bit CRC header.
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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

#ifndef CRC32_H
#define CRC32_H

#include <compiler.h>


//! Type to contain 32-bit CRC.
typedef uint32_t crc32_t;


#ifdef __cplusplus
extern "C" {
#endif

enum status_code crc32_recalculate(const void *data, size_t length, crc32_t *crc);

#ifdef __cplusplus
}
#endif


/**
 * \brief Calculate 32-bit CRC for initial block
 *
 * This function calculates the CRC for the specified data block, which may be
 * first of an arbitrary number of blocks.
 *
 * The actual calculation is done in \ref crc_recalculate(), while this function
 * just sets up the initial CRC value.
 *
 * \param[in] data Address of data.
 * \param[in] length Length of data.
 * \param[out] crc Address of variable to store the calculated CRC in.
 *
 * \return Status of calculation.
 * \retval STATUS_OK if calculation succeeded.
 * \retval <other> if calculation failed somehow.
 *
 * \note To calculate the CRC of multiple blocks, use this function first, then
 * \ref crc32_recalculate() for the following blocks.
 */
static inline enum status_code crc32_calculate(const void *data, size_t length,
		crc32_t *crc)
{
	*crc = 0;

	return crc32_recalculate(data, length, crc);
}

#endif // CRC32_H