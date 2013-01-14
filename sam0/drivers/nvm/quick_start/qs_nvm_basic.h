/**
 * \file
 *
 * \brief SAMD20 Non Volatile Memory Driver Quick Start
 *
 * Copyright (C) 2012-2013 Atmel Corporation. All rights reserved.
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

/**
 * \page nvm_basic_use_case Quick Start Guide for the NVM module - Basic Use Case
 *
 * In this use case, the NVM module is configured for:
 *  \li Power reduction mode enabled after sleep until first NVM access
 *  \li Manual (explicit) page write commands required to commit data
 *  \note The NVM driver takes care of checking this configuration in the \ref
 *  nvm_write_page() function.
 *  \li Zero wait states when reading FLASH memory
 *  \li No memory space for the EEPROM
 *  \li No protected bootloader section
 *
 * This use case sets up the NVM controller to copy from one to another page in
 * flash.
 *
 * \section nvm_basic_use_case_qs Quick Start
 *
 * \subsection nvm_basic_use_case_setup_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection nvm_basic_use_case_code Code
 * Copy-paste the following setup code to your user application:
 * \snippet qs_nvm_basic.c nvm_basic_setup
 *
 * Add to user application main():
 * \snippet qs_nvm_basic.c nvm_basic_main
 *
 * \section nvm_basic_use_case_flow Workflow
 * -# Size of the buffer
 *  - \snippet qs_nvm_basic.c nvm_basic_def
 * -# Create configuration function
 *   -# Create variable for the configuration struct
 *    - \snippet qs_nvm_basic.c nvm_basic_var1
 *   -# Load the default configurations into the config struct
 *    - \snippet qs_nvm_basic.c nvm_basic_setup_load
 *   -# Initialize the NVM module with the default configuration
 *    - \snippet qs_nvm_basic.c nvm_basic_setup_init
 * -# Create variable for buffer
 *  - \snippet qs_nvm_basic.c nvm_basic_var2
 * -# Configure the NVM
 *  - \snippet qs_nvm_basic.c nvm_basic_config
 * -# Read page 0 from NVM memory into the buffer
 *  - \snippet qs_nvm_basic.c nvm_basic_read_page
 * -# Write content of buffer to page 1 in the NVM memory
 *  - \snippet qs_nvm_basic.c nvm_basic_write_page
 * -# Return from main loop
 *  - \snippet qs_nvm_basic.c nvm_basic_return
 *
 *
 *
 */

