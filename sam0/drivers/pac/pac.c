/**
 * \file
 *
 * \brief SAM0+ Peripheral Access Controller Driver
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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

#include "pac.h"

#warning "Will need to be updated!"
/*! Infinite loop if CPU exception is detected */
void HardFault_Handler(void)
{
	while(1){}
}

/**
 * \brief Lock a given peripheral's registers.
 *
 * Use this function to deny write access to a given peripheral's registers.
 *
 * \warning Locking an already locked peripheral will cause an exception.
 *
 * \param[in] peripheral enum value for the peripheral to be locked.
 * \param[in] key bitwise inverse of peripheral, used as key for security
 * enhancement. See \ref bitwise_code
 * \return Status of the configuration procedure.
 * \retval STATUS_OK If the peripheral was successfully locked.
 * \retval STATUS_ERR_INVALID_ARG	If invalid argument(s) were supplied.
 */
__no_inline enum status_code system_peripheral_lock(
		enum system_peripheral_flag peripheral,
		uint32_t key)
{
	/* Bit to be set in desired register is given by bit 5:0 */
	uint8_t register_bit_pos = peripheral % 32;

	/* Value of which PAC register to use is given by bit 31:6 */
	uint8_t register_pos = peripheral / 32;

	/* Check if key is correct. */
	if (~peripheral != key){
		/* If key is not correct, do hard fault. */
		HardFault_Handler();
	}

#warning "Mock-up only!"
	switch (register_pos) {
#ifdef PAC1
		case 0:
			PAC1.WPSET = (1 << register_bit_pos);
			break;
#endif
#ifdef PAC2
		case 1:
			PAC2.WPSET = (1 << register_bit_pos);
			break;
#endif
#ifdef PAC3
		case 2:
			PAC3.WPSET = (1 << register_bit_pos);
			break;
#endif
		default:
			Assert(false);
			return STATUS_ERR_INVALID_ARG;
	}
	return STATUS_OK;
}

/**
 * \brief Unlock a given peripheral's registers.
 *
 * Use this function to allow write access to a given peripheral's registers.
 *
 * \warning Unlocking an already unlocked peripheral will cause an exception.
 *
 * \param[in] peripheral enum value for the peripheral to be unlocked.
 * \param[in] key bitwise inverse of peripheral, used as key for security
 * enhancement. See \ref bitwise_code.
 * \return Status of the configuration procedure.
 * \retval STATUS_OK If the peripheral was successfully unlocked.
 * \retval STATUS_ERR_INVALID_ARG If invalid argument(s) were supplied.
 */
__no_inline enum status_code system_peripheral_unlock(
		enum system_peripheral_flag peripheral,
		uint32_t key)
{
	/* Bit to be set in desired register is given by bit 5:0 */
	uint8_t register_bit_pos = peripheral % 32;

	/* Value of which PAC register to use is given by bit 31:6 */
	uint8_t register_pos = peripheral / 32;

	/* Check if key is correct. */
	if (~peripheral != key){
		/* If key is not correct, do hard fault. */
		HardFault_Handler();
	}

#warning "Mock-up only!"
	switch (register_pos) {
#ifdef PAC1
		case 0:
			PAC1.WPCLR = (1 << register_bit_pos);
			break;
#endif
#ifdef PAC2
		case 1:
			PAC2.WPCLR = (1 << register_bit_pos);
			break;
#endif
#ifdef PAC3
		case 2:
			PAC3.WPCLR = (1 << register_bit_pos);
			break;
#endif
		default:
			Assert(false);
			return STATUS_ERR_INVALID_ARG;
	}
	return STATUS_OK;
}
