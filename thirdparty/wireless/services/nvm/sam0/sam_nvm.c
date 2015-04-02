/**
 * \file
 *
 * \brief Non volatile memories management for SAM devices
 *
 * Copyright (c) 2013-2014 Atmel Corporation. All rights reserved.
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
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include "nvm.h"
#include "common_nvm.h"
#include "conf_board.h"
#include "system_interrupt.h"
#include "string.h"
#include "system.h"
extern int main(void);
typedef struct
{
	uint32_t image_start_addr;
	uint32_t image_size;
} swap_images_req_t;
status_code_t nvm_sam0_read(mem_type_t mem, uint32_t address,
		uint8_t *const buffer,
		uint32_t len)__attribute__((section(".boot")));
status_code_t nvm_memcpy(
		const uint32_t destination_address,
		uint8_t *const buffer,
		uint16_t length,
		bool erase_flag) __attribute__((section(".boot")));
void flash_swap(swap_images_req_t *param)__attribute__((section(".boot")));
/* Parameters for swapping image */

void (*func) (void) = (void(*)(void))0x000;
/**
 * \internal Pointer to the NVM MEMORY region start address
 */
#define NVM_MEMORY        ((volatile uint16_t *)FLASH_ADDR)
#define PAGE_NUMBER_SHIFT              6
status_code_t nvm_read(mem_type_t mem, uint32_t address, void *buffer,
		uint32_t len)
{
	nvm_sam0_read(mem, address, buffer, len);
	return STATUS_OK;
}

status_code_t nvm_sam0_read(mem_type_t mem, uint32_t address,
		uint8_t *const buffer,
		uint32_t len)
{
	switch (mem) {
		/* Get a pointer to the module hardware instance */
		Nvmctrl *const nvm_module = NVMCTRL;

	case INT_FLASH:

		/* Check if the module is busy */
		if (!nvm_is_ready()) {
			return STATUS_BUSY;
		}

		/* Clear error flags */
		nvm_module->STATUS.reg |= NVMCTRL_STATUS_MASK;

		uint32_t page_address = address / 2;

		/* NVM _must_ be accessed as a series of 16-bit words, perform
		 * manual copy
		 * to ensure alignment */
		for (uint16_t i = 0; i < len; i += 2) {
			/* Fetch next 16-bit chunk from the NVM memory space */
			uint16_t data = NVM_MEMORY[page_address++];

			/* Copy first byte of the 16-bit chunk to the
			 *destination buffer */
			buffer[i] = (data & 0xFF);

			/* If we are not at the end of a read request with an
			 * odd byte count,
			 * store the next byte of data as well */
			if (i < (len - 1)) {
				buffer[i + 1] = (data >> 8);
			}
		}

		break;

	default:
		return ERR_INVALID_ARG;
	}

	return STATUS_OK;
}

status_code_t nvm_memcpy(
		const uint32_t destination_address,
		uint8_t *const buffer,
		uint16_t length,
		bool erase_flag)
{
	enum status_code error_code = STATUS_OK;
	uint8_t row_buffer[NVMCTRL_ROW_PAGES * FLASH_PAGE_SIZE];
	volatile uint8_t *dest_add = (uint8_t *)destination_address;
	const uint8_t *src_buf = buffer;
	uint32_t i;

	/* Calculate the starting row address of the page to update */
	uint32_t row_start_address
		= destination_address &
			~((FLASH_PAGE_SIZE * NVMCTRL_ROW_PAGES) - 1);

	while (length) {
		/* Backup the contents of a row */
		for (i = 0; i < NVMCTRL_ROW_PAGES; i++) {
			do {
				error_code = nvm_read_buffer(
						row_start_address +
						(i * FLASH_PAGE_SIZE),
						(row_buffer +
						(i * FLASH_PAGE_SIZE)),
						FLASH_PAGE_SIZE);
			} while (error_code == STATUS_BUSY);

			if (error_code != STATUS_OK) {
				return error_code;
			}
		}

		/* Update the buffer if necessary */
		for (i = row_start_address;
				i < row_start_address +
				(FLASH_PAGE_SIZE * NVMCTRL_ROW_PAGES); i++) {
			if (length && ((uint8_t *)i == dest_add)) {
				row_buffer[i - row_start_address] = *src_buf++;
				dest_add++;
				length--;
			}
		}

		system_interrupt_enter_critical_section();

		if (erase_flag) {
			/* Erase the row */
			do {
				error_code = nvm_erase_row(row_start_address);
			} while (error_code == STATUS_BUSY);

			if (error_code != STATUS_OK) {
				return error_code;
			}
		}

		/* Write the updated row contents to the erased row */
		for (i = 0; i < NVMCTRL_ROW_PAGES; i++) {
			do {
				error_code = nvm_write_buffer(
						row_start_address +
						(i * FLASH_PAGE_SIZE),
						(row_buffer +
						(i * FLASH_PAGE_SIZE)),
						FLASH_PAGE_SIZE);
			} while (error_code == STATUS_BUSY);

			if (error_code != STATUS_OK) {
				return error_code;
			}
		}

		system_interrupt_leave_critical_section();

		row_start_address += NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE;
	}

	return error_code;
}

status_code_t nvm_write(mem_type_t mem, uint32_t address, void *buffer,
		uint32_t len)
{
	switch (mem) {
	case INT_FLASH:

		if (STATUS_OK != nvm_memcpy(address, buffer, len, true)) {
		}

		{
			return ERR_INVALID_ARG;
		}
		break;

	default:
		return ERR_INVALID_ARG;
	}

	return STATUS_OK;
}
status_code_t nvm_page_swap (mem_type_t mem, uint32_t start_addr, uint32_t size)
{
	switch (mem) {
		case INT_FLASH:
		{
			swap_images_req_t param;
			param.image_size = size;
			param.image_start_addr = start_addr;
			flash_swap(&param);
		}
		break;

		default:
		/* Other memories not supported */
		return ERR_INVALID_ARG;
	}

	return STATUS_OK;
}



status_code_t nvm_init(mem_type_t mem)
{
	if (INT_FLASH == mem) {
		struct nvm_config config;
		/* Get the default configuration */
		nvm_get_config_defaults(&config);

		/* Enable automatic page write mode */
		config.manual_page_write = false;

		/* Set wait state to 1 */
		config.wait_states = 2;

		/* Set the NVM configuration */
		nvm_set_config(&config);

		return STATUS_OK;
	}

	return ERR_INVALID_ARG;
}
void flash_swap(swap_images_req_t *param)
{
	uint16_t current_page_number = param->image_start_addr >> PAGE_NUMBER_SHIFT;
	uint16_t data_page_number = current_page_number;
	//common_boot_req_t common_boot_req;
	uint32_t page_start_addr,page_start_addr1;
	uint16_t saving_page_number = 0;
	uint32_t  data_byte[16];

	__ASM volatile ("cpsid i");

	while (param->image_size)
	{
		data_page_number = param->image_start_addr >> PAGE_NUMBER_SHIFT;

		//if (data_page_number != current_page_number)
		//{
			/* Store data from flash buffer to flash page */
			page_start_addr = param->image_start_addr;
			nvm_read(INT_FLASH,page_start_addr,&data_byte,64);
			page_start_addr1 = (uint32_t)saving_page_number << PAGE_NUMBER_SHIFT;
			nvm_memcpy(page_start_addr1,&data_byte,sizeof(data_byte),true);
			//delay_ms(20);
			//current_page_number = data_page_number;
			saving_page_number++;
		//}

		
		//nvm_read(INT_FLASH,page_start_addr+1,&data_byte[1],1);\\\\\
		/* Fill temporary page buffer */

		//flash_fill_page_buffer(page_start_addr,sizeof(data_byte),&data_byte);
        //  nvm_memcpy(page_start_addr,data_byte,sizeof(data_byte),true);
		param->image_start_addr += sizeof(data_byte);
		param->image_size -= sizeof(data_byte);
        if(param->image_size < 64)
		{   
			page_start_addr = param->image_start_addr;
			nvm_read(INT_FLASH,page_start_addr,&data_byte,param->image_size);
			page_start_addr1 = (uint32_t)saving_page_number << PAGE_NUMBER_SHIFT;
			nvm_memcpy(page_start_addr1,data_byte,param->image_size,true);
			param-> image_size = 0;
		}
	}
	/* flush last page into flash */
	/*page_start_addr = (uint32_t)saving_page_number << PAGE_NUMBER_SHIFT;
	flash_program_page(page_start_addr);*/
	system_reset();
	//main();
}
