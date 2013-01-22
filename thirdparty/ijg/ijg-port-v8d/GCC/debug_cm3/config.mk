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
PRJ_PATH = ../../../../..

# Target CPU architecture: cortex-m3, cortex-m4
ARCH = cortex-m3

# Target part: none, sam3n4 or sam4l4aa
PART = sam3s4c

# Application target name. Given with suffix .a for library and .elf for a
# standalone application.
TARGET_FLASH = ..\libjpeg_armgcc_debug_cm3.a

# List of C source files.
CSRCS = \
		thirdparty/ijg/v8d/jaricom.c \
		thirdparty/ijg/v8d/jcapimin.c \
		thirdparty/ijg/v8d/jcapistd.c \
		thirdparty/ijg/v8d/jcarith.c \
		thirdparty/ijg/v8d/jccoefct.c \
		thirdparty/ijg/v8d/jccolor.c \
		thirdparty/ijg/v8d/jcdctmgr.c \
		thirdparty/ijg/v8d/jchuff.c \
		thirdparty/ijg/v8d/jcinit.c \
		thirdparty/ijg/v8d/jcmainct.c \
		thirdparty/ijg/v8d/jcmarker.c \
		thirdparty/ijg/v8d/jcmaster.c \
		thirdparty/ijg/v8d/jcomapi.c \
		thirdparty/ijg/v8d/jcparam.c \
		thirdparty/ijg/v8d/jcprepct.c \
		thirdparty/ijg/v8d/jcsample.c \
		thirdparty/ijg/v8d/jctrans.c \
		thirdparty/ijg/v8d/jdapimin.c \
		thirdparty/ijg/v8d/jdapistd.c \
		thirdparty/ijg/v8d/jdarith.c \
		thirdparty/ijg/v8d/jdcoefct.c \
		thirdparty/ijg/v8d/jdcolor.c \
		thirdparty/ijg/v8d/jddctmgr.c \
		thirdparty/ijg/v8d/jdhuff.c \
		thirdparty/ijg/v8d/jdinput.c \
		thirdparty/ijg/v8d/jdmainct.c \
		thirdparty/ijg/v8d/jdmarker.c \
		thirdparty/ijg/v8d/jdmaster.c \
		thirdparty/ijg/v8d/jdmerge.c \
		thirdparty/ijg/v8d/jdpostct.c \
		thirdparty/ijg/v8d/jdsample.c \
		thirdparty/ijg/v8d/jdtrans.c \
		thirdparty/ijg/v8d/jerror.c \
		thirdparty/ijg/v8d/jfdctflt.c \
		thirdparty/ijg/v8d/jfdctfst.c \
		thirdparty/ijg/v8d/jfdctint.c \
		thirdparty/ijg/v8d/jidctflt.c \
		thirdparty/ijg/v8d/jidctfst.c \
		thirdparty/ijg/v8d/jidctint.c \
		thirdparty/ijg/v8d/jmemmgr.c \
		thirdparty/ijg/v8d/jmemnobs.c \
		thirdparty/ijg/v8d/jquant1.c \
		thirdparty/ijg/v8d/jquant2.c \
		thirdparty/ijg/v8d/jutils.c \
		thirdparty/ijg/ijg-port-v8d/jdatadst_mem.c \
		thirdparty/ijg/ijg-port-v8d/jdatasrc_mem.c \
		thirdparty/ijg/ijg-port-v8d/libjpeg.c

# List of assembler source files.
ASSRCS = 

# List of include paths.
INC_PATH = \
		thirdparty/ijg/ijg-port-v8d \
		thirdparty/ijg/v8d \

# Additional search paths for libraries.
LIB_PATH =  \
       thirdparty/CMSIS/Lib/GCC                          

# List of libraries to use during linking.
LIBS =  \
       arm_cortexM3l_math                                

# Path relative to top level directory pointing to a linker script.

# Path relative to top level directory pointing to a linker script.

# Project type parameter: all, sram or flash
PROJECT_TYPE        = flash

# Additional options for debugging. By default the common Makefile.in will
# add -g3.
DBGFLAGS =

# Application optimization used during compilation and linking:
# -O0, -O1, -O2, -O3 or -Os
OPTIMIZATION = -O0

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
       -D ARM_MATH_CM3=true 		\
       -D __SAM3S4C__

# Extra flags to use when linking
LDFLAGS = \
