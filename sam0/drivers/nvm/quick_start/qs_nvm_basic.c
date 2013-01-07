/**
 * \file
 *
 * \brief SAMD20 Non Volatile Memory Driver Quick Start
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
#include <asf.h>

void configure_nvm(void);

//! [nvm_basic_setup]
/* Buffer with same size of a page */
//! [nvm_basic_def]
/*   */
#define BUFFER_SIZE 0x40
//! [nvm_basic_def]

/* Configuration function */
void configure_nvm(void)
{
	/* Variable for the function */
	//! [nvm_basic_var1]
	struct nvm_config config;
	//! [nvm_basic_var1]

	/* Initialize the NVM controller */
	//! [nvm_basic_setup_load]
	nvm_get_config_defaults(&config);
	//! [nvm_basic_setup_load]
	//! [nvm_basic_setup_init]
	nvm_init(&config);
	//! [nvm_basic_setup_init]
	//! [nvm_basic_setup_main]
}
//! [nvm_basic_setup]


int main(void)
{
//! [nvm_basic_main]
	//! [nvm_basic_setup_main]
	/* Variables for the QuickStart */
	//! [nvm_basic_var2]
	uint32_t buf[BUFFER_SIZE];
	//! [nvm_basic_var2]

	/* Configure the NVM */
	//! [nvm_basic_config]
	configure_nvm();
	//! [nvm_basic_config]

	/* Copy content from page 0 to page 1 */
	//! [nvm_basic_read_page]
	nvm_read_page(0, (uint32_t *)&buf);
	//! [nvm_basic_read_page]
	//! [nvm_basic_write_page]
	nvm_write_page(1, (uint32_t *)&buf);
	//! [nvm_basic_write_page]

	/* Infinite loop */
	//! [nvm_basic_inf_loop]
	while (true) {
		/* Intentionally left empty */
	}
	//! [nvm_basic_inf_loop]

	/* Return from main loop */
	//! [nvm_basic_return]
	return 0;
	//! [nvm_basic_return]

//! [nvm_basic_main]
}

