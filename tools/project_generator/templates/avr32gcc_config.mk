#
# Copyright (c) 2009-2010 Atmel Corporation. All rights reserved.
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
PRJ_PATH = $ASF_ROOT$

# Target CPU architecture: ap, ucr1, ucr2 or ucr3
ARCH = $ASF_ARCH$

# Target part: none, ap7xxx or uc3xxxxx
PART = $ASF_MCU_STRIP$

# Target device flash memory details (used by the avr32program programming
# tool: [cfi|internal]@address
FLASH = internal@0x80000000

# Clock source to use when programming; xtal, extclk or int
PROG_CLOCK = $ASF_PROG_CLOCK$

# Application target name. Given with suffix .a for library and .elf for a
# standalone application.
TARGET = $ASF_PROJECT$.$ASF_PROJECT_TYPE$

# List of C source files.
CSRCS = $ASF_CSRC$

# List of assembler source files.
ASSRCS = $ASF_ASSRC$

# List of include paths.
INC_PATH = $ASF_INC_PATH$

# Additional search paths for libraries.
LIB_PATH = $ASF_LIB_PATH$

# List of libraries to use during linking.
LIBS = $ASF_LIBS$

# Path relative to top level directory pointing to a linker script.
LINKER_SCRIPT = $ASF_LINKER_SCRIPT$

# Additional options for debugging. By default the common Makefile.in will
# add -g3.
DBGFLAGS = $ASF_DBGFLAGS$

# Application optimization used during compilation and linking:
# -O0, -O1, -O2, -O3 or -Os
OPTIMIZATION = $ASF_OPTLEVEL$

# Extra flags to use when archiving.
ARFLAGS = $ASF_ARFLAGS$

# Extra flags to use when assembling.
ASFLAGS = $ASF_ASFLAGS$

# Extra flags to use when compiling.
CFLAGS = $ASF_CFLAGS$

# Extra flags to use when preprocessing.
#
# Preprocessor symbol definitions
#   To add a definition use the format "-D name[=definition]".
#   To cancel a definition use the format "-U name".
#
# The most relevant symbols to define for the preprocessor are:
#   BOARD      Target board in use, see boards/board.h for a list.
#   EXT_BOARD  Optional extension board in use, see boards/board.h for a list.
CPPFLAGS = $ASF_CPPFLAGS$

# Extra flags to use when linking
LDFLAGS = $ASF_LDFLAGS$
