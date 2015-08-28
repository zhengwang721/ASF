#
# Copyright (c) 2011 Atmel Corporation. All rights reserved.
#
# \asf_license_start
#
# \page License
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. The name of Atmel may not be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# 4. This software may only be redistributed and used in connection with an
#    Atmel microcontroller product.
#
# THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
# EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# \asf_license_stop
#

# Path to top level ASF directory relative to this project directory.
PRJ_PATH = ../../../../../..

# Target CPU architecture: cortex-m3, cortex-m4
ARCH = cortex-m0

# Target part: none, sam3n4 or sam4l4aa
PART = samb11g18a

# Application target name. Given with suffix .a for library and .elf for a
# standalone application.
TARGET_FLASH = ble_smart_sdk_lib_lib_flash.elf
TARGET_SRAM = ble_smart_sdk_lib_lib_sram.elf

# List of C source files.
CSRCS = \
       common/utils/interrupt/interrupt_sam_nvic.c        \
       sam0/utils/cmsis/samb11/source/gcc/startup_samb11.c \
       sam0/utils/cmsis/samb11/source/system_samb11.c     \
       sam0/utils/syscalls/gcc/syscalls.c                 \
       thirdparty/wireless/ble_smart_sdk_lib/ble_stack/api_controller_interface/dbg_task.c \
       thirdparty/wireless/ble_smart_sdk_lib/ble_stack/api_controller_interface/error.c \
       thirdparty/wireless/ble_smart_sdk_lib/ble_stack/api_controller_interface/gapc_task.c \
       thirdparty/wireless/ble_smart_sdk_lib/ble_stack/api_controller_interface/gapm_task.c \
       thirdparty/wireless/ble_smart_sdk_lib/ble_stack/api_controller_interface/gattc_task.c \
       thirdparty/wireless/ble_smart_sdk_lib/ble_stack/api_controller_interface/gattm_task.c \
       thirdparty/wireless/ble_smart_sdk_lib/ble_stack/api_controller_interface/htpt_task.c \
       thirdparty/wireless/ble_smart_sdk_lib/ble_stack/api_core/event.c \
       thirdparty/wireless/ble_smart_sdk_lib/ble_stack/api_core/gap.c \
       thirdparty/wireless/ble_smart_sdk_lib/ble_stack/api_core/gatt_client.c \
       thirdparty/wireless/ble_smart_sdk_lib/ble_stack/api_core/gatt_server.c \
       thirdparty/wireless/ble_smart_sdk_lib/ble_stack/api_core/security.c \
       thirdparty/wireless/ble_smart_sdk_lib/ble_stack/profiles/htpt.c \
       thirdparty/wireless/ble_smart_sdk_lib/plf/interface.c

# List of assembler source files.
ASSRCS = 

# List of include paths.
INC_PATH = \
       common/boards                                      \
       common/utils                                       \
       sam0/utils                                         \
       sam0/utils/cmsis/samb11/include                    \
       sam0/utils/cmsis/samb11/source                     \
       sam0/utils/header_files                            \
       sam0/utils/preprocessor                            \
       thirdparty/CMSIS/Include                           \
       thirdparty/CMSIS/Lib/GCC                           \
       thirdparty/wireless/ble_smart_sdk_lib/ble_stack/include \
       thirdparty/wireless/ble_smart_sdk_lib/include      \
       thirdparty/wireless/ble_smart_sdk_lib/lib          \
       thirdparty/wireless/ble_smart_sdk_lib/lib/atsamb11g18a \
       thirdparty/wireless/ble_smart_sdk_lib/lib/atsamb11g18a/dummy \
       thirdparty/wireless/ble_smart_sdk_lib/plf \
       thirdparty/wireless/ble_smart_sdk_lib/lib/atsamb11g18a/gcc

# Additional search paths for libraries.
LIB_PATH =  \
       thirdparty/CMSIS/Lib/GCC                          

# List of libraries to use during linking.
LIBS =  \
       arm_cortexM0l_math                                

# Path relative to top level directory pointing to a linker script.
LINKER_SCRIPT_FLASH = sam0/utils/linker_scripts/samb11/gcc/samb11g18a_sram.ld
LINKER_SCRIPT_SRAM  = sam0/utils/linker_scripts/samb11/gcc/samb11g18a_sram.ld

# Path relative to top level directory pointing to a linker script.
DEBUG_SCRIPT_FLASH = 
DEBUG_SCRIPT_SRAM  = 

# Project type parameter: all, sram or flash
PROJECT_TYPE        = flash

# Additional options for debugging. By default the common Makefile.in will
# add -g3.
DBGFLAGS = 

# Application optimization used during compilation and linking:
# -O0, -O1, -O2, -O3 or -Os
OPTIMIZATION = -O1

# Extra flags to use when archiving.
ARFLAGS = 

# Extra flags to use when assembling.
ASFLAGS = 

# Extra flags to use when compiling.
CFLAGS = 

# Extra flags to use when preprocessing.
#
# Preprocessor symbol definitions
#   To add a definition use the format "-D name[=definition]".
#   To cancel a definition use the format "-U name".
#
# The most relevant symbols to define for the preprocessor are:
#   BOARD      Target board in use, see boards/board.h for a list.
#   EXT_BOARD  Optional extension board in use, see boards/board.h for a list.
CPPFLAGS = \
       -D ARM_MATH_CM0PLUS=true                           \
       -D BOARD=DUMMY_BOARD

# Extra flags to use when linking
LDFLAGS = \

# Pre- and post-build commands
PREBUILD_CMD = 
POSTBUILD_CMD = 